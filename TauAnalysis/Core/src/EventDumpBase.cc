#include "TauAnalysis/Core/interface/EventDumpBase.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "TauAnalysis/DQMTools/interface/generalAuxFunctions.h"

#include <iostream>
#include <fstream>

EventDumpBase::EventDumpBase(const edm::ParameterSet& cfg)
{
  //std::cout << "<EventDumpBase::EventDumpBase>:" << std::endl;

  if ( cfg.exists("output") ) {
    std::string output = cfg.getParameter<std::string>("output");
    //std::cout << " output = " << output << std::endl;

    if ( output == "std::cout" ) {
      //std::cout << "--> setting outputStream to std::cout." << std::endl;
      outputStream_ = &std::cout;
      isOutputFile_ = false;
    } else if ( output == "std::cerr" ) {
      //std::cout << "--> setting outputStream to std::cerr." << std::endl;
      outputStream_ = &std::cerr;
      isOutputFile_ = false;
    } else if ( output != "" ) {
      //std::cout << "--> setting outputStream to std::ofstream." << std::endl;
      outputStream_ = new std::ofstream(output.data(), std::ios::out);
      isOutputFile_ = true;
    } else {
      edm::LogError ("EventDumpBase::EventDumpBase") << " Invalid Configuration Parameter output = " << output
						     << " --> skipping !!";
      cfgError_ = 1;
    }
  } else {
    //std::cout << " no outputFileName specified in configuration parameters" << std::endl;
    //std::cout << "--> setting outputStream to std::cout." << std::endl;

    outputStream_ = &std::cout;
    isOutputFile_ = false;
  }

  vstring cfgTriggerConditions = ( cfg.exists("triggerConditions") ) ? cfg.getParameter<vstring>("triggerConditions") : vstring();
  for ( vstring::const_iterator cfgTriggerCondition = cfgTriggerConditions.begin();
	cfgTriggerCondition != cfgTriggerConditions.end(); ++cfgTriggerCondition ) {
    int errorFlag;
    std::string cfgTriggerCondition_noWhiteSpace = replace_string(*cfgTriggerCondition, " ", "", 0, UINT_MAX, errorFlag);
    //std::cout << " cfgTriggerCondition_noWhiteSpace = " << cfgTriggerCondition_noWhiteSpace << std::endl;
  
    size_t posSeparator_filterName = cfgTriggerCondition_noWhiteSpace.find(":");
    if ( posSeparator_filterName == std::string::npos ) {
      edm::LogError ("EventDumpBase::EventDumpBase") << " Failed to parse triggerCondition = " << (*cfgTriggerCondition)
						     << " --> skipping !!";
      cfgError_ = 1;
      continue;
    }
	
    std::string filterName = std::string(cfgTriggerCondition_noWhiteSpace, 0, posSeparator_filterName);
    //std::cout << " filterName = " << filterName << std::endl;

    size_t startTriggerCondition = posSeparator_filterName + 1;
    while ( startTriggerCondition < cfgTriggerCondition_noWhiteSpace.length() ) {
      size_t posSeparator_triggerCondition = cfgTriggerCondition_noWhiteSpace.find(",", startTriggerCondition);
      size_t endTriggerCondition = ( posSeparator_triggerCondition != std::string::npos ) ? 
	posSeparator_triggerCondition : cfgTriggerCondition_noWhiteSpace.length();
      
      std::string triggerCondition = std::string(cfgTriggerCondition_noWhiteSpace, startTriggerCondition, endTriggerCondition - startTriggerCondition);
      //std::cout << " triggerCondition = " << triggerCondition << std::endl;
    
      triggerConditions_[filterName].push_back(triggerCondition);

      startTriggerCondition = endTriggerCondition + 1;
    }
  }

  alwaysTriggered_ = ( cfg.exists("triggerAlways") ) ? cfg.getParameter<bool>("triggerAlways") : false;
}

EventDumpBase::~EventDumpBase()
{
//--- close output file
  if ( isOutputFile_ ) delete outputStream_;
}

bool isFulfilled(const std::string& filterName, const std::string& triggerCondition,
		 const std::vector<std::pair<std::string, bool> >& filterResults_cumulative, 
		 const std::vector<std::pair<std::string, bool> >& filterResults_individual)
{
  //std::cout << "<isFulFilled>:" << std::endl;
  //std::cout << " filterName = " << filterName << std::endl;
  //std::cout << " triggerCondition = " << triggerCondition << std::endl;

//--- first pass through filterResults: 
//    count number of filters which passed/rejected the event
  unsigned numFiltersPassed_individual = 0;
  unsigned numFiltersRejected_individual = 0;
  for ( EventDumpBase::filterResults_type::const_iterator filterResult_individual = filterResults_individual.begin();
	filterResult_individual != filterResults_individual.end(); ++filterResult_individual ) {
    if (  filterResult_individual->second ) ++numFiltersPassed_individual;
    if ( !filterResult_individual->second ) ++numFiltersRejected_individual;
  }

//--- second pass through filterResults: 
//    update statistics of individual filters
  bool previousFiltersPassed = true;
  for ( EventDumpBase::filterResults_type::const_iterator filterResult_cumulative = filterResults_cumulative.begin();
	filterResult_cumulative != filterResults_cumulative.end(); ++filterResult_cumulative ) {
    const std::string& filterName_i = filterResult_cumulative->first;
    //std::cout << " filterName_i = " << filterName_i << std::endl;

    bool filterPassed_cumulative = filterResult_cumulative->second;
    //std::cout << " filterPassed_cumulative = " << filterPassed_cumulative << std::endl;
    
    EventDumpBase::filterResults_type::const_iterator filterResult_individual = filterResults_individual.end();
    for ( EventDumpBase::filterResults_type::const_iterator it = filterResults_individual.begin();
	  it != filterResults_individual.end(); ++it ) {
      if ( it->first == filterName_i ) {
	filterResult_individual = it;
	break;
      }
    }
    if ( filterResult_individual == filterResults_individual.end() ) {
      edm::LogError ("isFulFilled") << " Failed to find filterResult_individual for filterName = " << filterName
				    << " --> skipping !!";     
      continue;
    }

    bool filterPassed_individual = filterResult_individual->second;

    if ( filterName_i == filterName ) {
      if ( triggerCondition == "passed" ) {
	return filterPassed_individual;
      } else if ( triggerCondition == "rejected" ) {
	return !filterPassed_individual;
      } else if ( triggerCondition == "passed_cumulative" ) {
	return previousFiltersPassed && filterPassed_cumulative;
      } else if ( triggerCondition == "rejected_cumulative" ) {
	return previousFiltersPassed && !filterPassed_cumulative;
      } else if ( triggerCondition == "exclRejected" ) {
	return numFiltersRejected_individual == 1 && !filterPassed_individual;
      } else {
	edm::LogError ("isFulFilled") << " Undefined triggerCondition = " << triggerCondition << " !!";
	return false;
      }
    }
      
    if ( !filterPassed_cumulative ) previousFiltersPassed = false;
  }

  edm::LogError ("isFulFilled") << " No filter with filterName = " << filterName << " defined !!";
  return false;
}

void EventDumpBase::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup, 
			    const filterResults_type& filterResults_cumulative, 
			    const filterResults_type& filterResults_individual, 
			    double eventWeight) 
{
  //std::cout << "<EventDumpBase::analyze>:" << std::endl;

  vstring triggerConditions_fulfilled;
  for ( std::map<std::string, vstring>::const_iterator filterEntry = triggerConditions_.begin();
	filterEntry != triggerConditions_.end(); ++filterEntry ) {
    const std::string& filterName = filterEntry->first;
    for ( vstring::const_iterator triggerCondition = filterEntry->second.begin();
	  triggerCondition != filterEntry->second.end(); ++triggerCondition ) {
      if ( isFulfilled(filterName, *triggerCondition, filterResults_cumulative, filterResults_individual) ) {
	std::ostringstream triggerCondition_fulfilled;
	triggerCondition_fulfilled << filterName << ":" << (*triggerCondition);
	triggerConditions_fulfilled.push_back(triggerCondition_fulfilled.str());
      }
    }
  }

  if ( triggerConditions_fulfilled.size() >= 1 || alwaysTriggered_ ) {
    print(iEvent, iSetup, filterResults_cumulative, filterResults_individual, eventWeight);
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

EDM_REGISTER_PLUGINFACTORY(EventDumpPluginFactory, "EventDumpPluginFactory");

