#include "TauAnalysis/Core/plugins/GenericAnalyzer.h"

#include "TauAnalysis/DQMTools/interface/dqmAuxFunctions.h"
#include "TauAnalysis/DQMTools/interface/generalAuxFunctions.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DQMServices/Core/interface/DQMStore.h"

#include "FWCore/Utilities/interface/InputTag.h"

#include <algorithm>
#include <limits.h>
#include <iostream>

unsigned GenericAnalyzer::analysisSequenceEntry_filter::filterId_ = 1;

GenericAnalyzer::analysisSequenceEntry::analysisSequenceEntry(const std::string& name)
  : name_(name)
{}

GenericAnalyzer::analysisSequenceEntry::~analysisSequenceEntry()
{
// nothing to be done yet...
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

GenericAnalyzer::analysisSequenceEntry_filter::analysisSequenceEntry_filter(const std::string& name, const std::string& title,
									   const edm::ParameterSet& cfgFilter, int& cfgError)
  : analysisSequenceEntry(name)
{
  std::string filterType = cfgFilter.getParameter<std::string>("pluginType");

  if ( cfgFilter.exists("src_cumulative") &&
       cfgFilter.exists("src_individual") ) {
    edm::ParameterSet cfgFilter_cumulative = cfgFilter;
    cfgFilter_cumulative.addParameter<edm::InputTag>("src", cfgFilter.getParameter<edm::InputTag>("src_cumulative"));
    filter_cumulative_ = EventSelectorPluginFactory::get()->create(filterType, cfgFilter_cumulative);
    edm::ParameterSet cfgFilter_individual = cfgFilter;
    cfgFilter_individual.addParameter<edm::InputTag>("src", cfgFilter.getParameter<edm::InputTag>("src_individual"));
    filter_individual_ = EventSelectorPluginFactory::get()->create(filterType, cfgFilter_individual);
  } else {
    filter_cumulative_ = EventSelectorPluginFactory::get()->create(filterType, cfgFilter);
    filter_individual_ = EventSelectorPluginFactory::get()->create(filterType, cfgFilter);
  }

  ++filterId_;

  //print();
}

GenericAnalyzer::analysisSequenceEntry_filter::~analysisSequenceEntry_filter()
{
  delete filter_cumulative_;
  delete filter_individual_;
}

void GenericAnalyzer::analysisSequenceEntry_filter::print() const
{
  std::cout << "<GenericAnalyzer::analysisSequenceEntry_filter::print>:" << std::endl; 
  std::cout << " filter_cumulative = " << filter_cumulative_ << std::endl;
  std::cout << " filter_individual = " << filter_individual_ << std::endl;
  std::cout << " filterId = " << filterId_ << std::endl;
}

bool GenericAnalyzer::analysisSequenceEntry_filter::filter_cumulative(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  edm::Event* iEvent_nonConst = const_cast<edm::Event*>(&iEvent);
  return (*filter_cumulative_)(*iEvent_nonConst, iSetup);
}

bool GenericAnalyzer::analysisSequenceEntry_filter::filter_individual(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  edm::Event* iEvent_nonConst = const_cast<edm::Event*>(&iEvent);
  return (*filter_individual_)(*iEvent_nonConst, iSetup);
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

GenericAnalyzer::analysisSequenceEntry_histManagers::analysisSequenceEntry_histManagers(const std::string& name , 
											const std::list<edm::ParameterSet>& cfgHistManagers)
  : analysisSequenceEntry(name)
{
  for ( std::list<edm::ParameterSet>::const_iterator cfgHistManager = cfgHistManagers.begin(); 
	cfgHistManager != cfgHistManagers.end(); ++cfgHistManager ) {
    std::string histManagerType = cfgHistManager->getParameter<std::string>("pluginType");
    HistManagerBase* histManager = HistManagerPluginFactory::get()->create(histManagerType, *cfgHistManager);
    histManagers_.push_back(histManager);
  }

  //print();
}

GenericAnalyzer::analysisSequenceEntry_histManagers::~analysisSequenceEntry_histManagers()
{
  for ( std::list<HistManagerBase*>::const_iterator histManager = histManagers_.begin();
	histManager != histManagers_.end(); ++histManager ) {
    delete (*histManager);
  }
}

void GenericAnalyzer::analysisSequenceEntry_histManagers::print() const
{
  std::cout << "<GenericAnalyzer::analysisSequenceEntry_histManagers::print>:" << std::endl;
  //vstring histManagerNames;
  //for ( std::list<HistManagerBase*>::const_iterator histManager = histManagers_.begin();
  //  	  histManager != histManagers_.end(); ++histManager ) {
  //  histManagerNames.push_back(std::string(histManager));
  //}
  //std::cout << " histManagers = " << format_vstring(histManagerNames) << std::endl;
  //std::cout << std::endl;
}

void GenericAnalyzer::analysisSequenceEntry_histManagers::beginJob(const edm::EventSetup& iSetup)
{
  for ( std::list<HistManagerBase*>::const_iterator histManager = histManagers_.begin();
	histManager != histManagers_.end(); ++histManager ) {
    (*histManager)->bookHistograms(iSetup);
  }
}

void GenericAnalyzer::analysisSequenceEntry_histManagers::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  for ( std::list<HistManagerBase*>::const_iterator histManager = histManagers_.begin();
	histManager != histManagers_.end(); ++histManager ) {
    (*histManager)->fillHistograms(iEvent, iSetup);
  }
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

void GenericAnalyzer::addFilter(const std::string& filterName, const vstring& saveRunEventNumbers)
{
  //std::cout << "<GenericAnalyzer::addFilter>:" << std::endl;
  //std::cout << " filterName = " << filterName << std::endl;

  std::map<std::string, edm::ParameterSet>::const_iterator it = cfgFilters_.find(filterName);
  if ( it != cfgFilters_.end() ) {
    edm::ParameterSet cfgFilter = it->second;

    std::string filterTitle = ( cfgFilter.exists("title") ) ? cfgFilter.getParameter<std::string>("title") : filterName;

    analysisSequenceEntry_filter* entry = new analysisSequenceEntry_filter(filterName, filterTitle, cfgFilter, cfgError_);
    analysisSequence_.push_back(entry);
  } else {
    edm::LogError ("GenericAnalyzer::addFilter") << " Failed to access configuration parameter for filter = " << filterName
						 << " --> skipping !!";
    cfgError_ = 1;
  }
}

std::string capitalizeFirstCharacter(const std::string& input)
{
  //std::cout << "<capitalizeFirstCharacter>:" << std::endl;
  //std::cout << " input = " << input << std::endl;
  std::string output = input;
  if ( output != "" ) std::transform(output.begin(), ++output.begin(), output.begin(), ::toupper);
  //std::cout << " output = " << output << std::endl;
  return output;
}

std::string dqmSubDirectoryName_filter(const std::string& afterFilterName, const std::string& beforeFilterName)
{
  std::string subDirName;
  if ( afterFilterName != "" ) subDirName.append("after").append(capitalizeFirstCharacter(afterFilterName));
  if ( afterFilterName != "" && beforeFilterName != "" ) subDirName.append("_");
  if ( beforeFilterName != "" ) subDirName.append("before").append(capitalizeFirstCharacter(beforeFilterName));
  return subDirName;
}

void GenericAnalyzer::addHistManagers(const vstring& histManagerNames, 
				     const std::string& afterFilterName, const std::string& beforeFilterName,
				     const vstring& replaceCommands)
{
  //std::cout << "<GenericAnalyzer::addHistManagers>:" << std::endl;

  std::list<edm::ParameterSet> cfgHistManagers;

  for ( vstring::const_iterator histManagerName = histManagerNames.begin();
	histManagerName != histManagerNames.end(); ++histManagerName ) {
    //std::cout << " histManagerName = " << (*histManagerName) << std::endl;

    std::map<std::string, edm::ParameterSet>::const_iterator it = cfgHistManagers_.find(*histManagerName);
    if ( it != cfgHistManagers_.end() ) {
      edm::ParameterSet cfgHistManager = it->second;
      
      for ( vstring::const_iterator replaceCommand = replaceCommands.begin();
	    replaceCommand != replaceCommands.end(); ++replaceCommand ) {
	//std::cout << " replaceCommand = " << (*replaceCommand) << std::endl;
	int errorFlag;
	std::string replaceCommand_noWhiteSpace = replace_string(*replaceCommand, " ", "", 0, UINT_MAX, errorFlag);
	//std::cout << " replaceCommand_noWhiteSpace = " << replaceCommand_noWhiteSpace << std::endl;

	std::string keyword = std::string(*histManagerName).append(".");
	//std::cout << " keyword = " << keyword << std::endl;
	if ( replaceCommand_noWhiteSpace.find(keyword) != std::string::npos ) {
	  std::string replaceCommand_noKeyword = replace_string(replaceCommand_noWhiteSpace, keyword, "", 0, UINT_MAX, errorFlag);
	  //std::cout << " replaceCommand_noKeyword = " << replaceCommand_noKeyword << std::endl; 
	  //std::cout << " (replaceCommand_noKeyword.length() - 1) = " << (replaceCommand_noKeyword.length() - 1) << std::endl;

	  size_t posAssignmentOperator = replaceCommand_noKeyword.find("=");
	  //std::cout << " posAssignmentOperator = " << posAssignmentOperator << std::endl;
	  if ( posAssignmentOperator == std::string::npos ||
	       !(posAssignmentOperator >= 1 && posAssignmentOperator < (replaceCommand_noKeyword.length() - 1)) ) {
	    edm::LogError ("GenericAnalyzer::addHistManagers") << " Failed to parse replaceCommand = " << (*replaceCommand)
							      << " --> skipping !!";
	    cfgError_ = 1;
	    continue;
	  }
	
	  std::string leftHandSide = std::string(replaceCommand_noKeyword, 0, posAssignmentOperator);
	  //std::cout << " leftHandSide = " << leftHandSide << std::endl;
	  std::string rightHandSide = std::string(replaceCommand_noKeyword, posAssignmentOperator + 1);
	  //std::cout << " rightHandSide = " << rightHandSide << std::endl;

//--- check that parameter that is to be replaced
//    exists in configuration parameter set 
//    and is defined to be a supported type
//    (so far, only std::string and edm::InputTag are supported types
//     for configuration parameter replacements)
	  if ( cfgHistManager.existsAs<std::string>(leftHandSide) ) {
	    cfgHistManager.addParameter<std::string>(leftHandSide, rightHandSide);
	  } else if ( cfgHistManager.existsAs<edm::InputTag>(leftHandSide) ) {
	    edm::InputTag rightHandSide_inputTag = rightHandSide;
	    cfgHistManager.addParameter<edm::InputTag>(leftHandSide, rightHandSide_inputTag);
	  } else {
	    edm::LogError ("GenericAnalyzer::addHistManagers") << " Configuration parameter to be replaced = " << leftHandSide 
							       << " does either not exist or is not of a supported type"
							       << " --> skipping !!";
	    cfgError_ = 1;
	    continue;
	  }

//--- store modified configuration ParameterSet
//    (NOTE: by storing the modified configuration ParameterSet,
//           replace statements become cumulative)
	  cfgHistManagers_[it->first] = cfgHistManager;
	}
      }

      cfgHistManager.addParameter<std::string>("afterFilterName", afterFilterName);
      cfgHistManager.addParameter<std::string>("beforeFilterName", beforeFilterName);

      std::string dqmDirectory_store = cfgHistManager.getParameter<std::string>("dqmDirectory_store");
      //std::cout << " dqmDirectory_store = " << dqmDirectory_store << std::endl;
      std::string dirName = dqmDirectoryName(name_).append(dqmSubDirectoryName_filter(afterFilterName, beforeFilterName));
      //std::cout << " dirName = " << dirName << std::endl;
      std::string dirName_full = dqmDirectoryName(dirName).append(dqmDirectory_store);
      //std::cout << " dirName_full = " << dirName_full << std::endl;
      cfgHistManager.addParameter<std::string>("dqmDirectory_store", dirName_full);

      cfgHistManagers.push_back(cfgHistManager);
    } else {
      edm::LogError ("GenericAnalyzer::addHistManagers") << " Failed to access configuration parameter for histManager = " << (*histManagerName)
							<< " --> skipping !!";
      cfgError_ = 1;
      return;
    }
  }

  std::string entryName = dqmDirectoryName(name_).append(dqmSubDirectoryName_filter(afterFilterName, beforeFilterName));
  analysisSequenceEntry_histManagers* entry = new analysisSequenceEntry_histManagers(entryName, cfgHistManagers);
  analysisSequence_.push_back(entry);
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

GenericAnalyzer::GenericAnalyzer(const edm::ParameterSet& cfg)
{
  //std::cout << "<GenericAnalyzer::GenericAnalyzer>:" << std::endl;

  cfgError_ = 0;

  name_ = cfg.getParameter<std::string>("name");
  //std::cout << " name = " << name_ << std::endl;

  typedef std::vector<edm::ParameterSet> vParameterSet;

//--- store configuration parameters for filters
  //std::cout << "--> storing configuration parameters for filters..." << std::endl;
  if ( cfg.exists("eventSelection") ) {
    vParameterSet cfgFilters = cfg.getParameter<vParameterSet>("eventSelection");
    for ( vParameterSet::const_iterator cfgFilter = cfgFilters.begin(); 
	  cfgFilter != cfgFilters.end(); ++cfgFilter ) {
      std::string cfgFilterName = cfgFilter->getParameter<std::string>("pluginName");
      //std::cout << " cfgFilterName = " << cfgFilterName << std::endl;
      cfgFilters_.insert(std::pair<std::string, edm::ParameterSet>(cfgFilterName, *cfgFilter)); 
    }
  }

//--- store configuration parameters for histManagers
  //std::cout << "--> storing configuration parameters for histManagers..." << std::endl;
  if ( cfg.exists("histManagers") ) {
    vParameterSet cfgHistManagers = cfg.getParameter<vParameterSet>("histManagers");
    for ( vParameterSet::const_iterator cfgHistManager = cfgHistManagers.begin(); 
	  cfgHistManager != cfgHistManagers.end(); ++cfgHistManager ) {
      std::string cfgHistManagerName = cfgHistManager->getParameter<std::string>("pluginName");
      //std::cout << " cfgHistManagerName = " << cfgHistManagerName << std::endl;
      cfgHistManagers_.insert(std::pair<std::string, edm::ParameterSet>(cfgHistManagerName, *cfgHistManager));
    }
  }

//--- configure analysisSequence
  //std::cout << "--> configuring analysisSequence..." << std::endl;
  typedef std::vector<edm::ParameterSet> vParameterSet;
  vParameterSet cfgAnalysisSequenceEntries = cfg.getParameter<vParameterSet>("analysisSequence");
  std::string lastFilterName = "";
  for ( vParameterSet::const_iterator cfgAnalysisSequenceEntry = cfgAnalysisSequenceEntries.begin();
	cfgAnalysisSequenceEntry != cfgAnalysisSequenceEntries.end(); ++cfgAnalysisSequenceEntry ) {
    std::string filterName = ( cfgAnalysisSequenceEntry->exists("filter") ) ? 
      cfgAnalysisSequenceEntry->getParameter<std::string>("filter") : "";
    vstring histManagerNames = ( cfgAnalysisSequenceEntry->exists("histManagers") ) ? 
      cfgAnalysisSequenceEntry->getParameter<vstring>("histManagers") : vstring();

    if ( filterName == "" && histManagerNames.size() == 0 ) {
      edm::LogError ("GenericAnalyzer") << " Either filter or histManagers must be specified for sequenceEntries !!";
      cfgError_ = 1;
      continue;
    }
    
    if ( filterName != "" && histManagerNames.size() != 0 ) {
      edm::LogError ("GenericAnalyzer") << " Must not specify filter and histManagers for same sequenceEntry !!";
      cfgError_ = 1;
      continue;
    }

    if ( filterName != "" ) {
      vstring saveRunEventNumbers = cfgAnalysisSequenceEntry->exists("saveRunEventNumbers") ? 
	cfgAnalysisSequenceEntry->getParameter<vstring>("saveRunEventNumbers") : vstring();
      
      addFilter(filterName, saveRunEventNumbers);
      
      lastFilterName = filterName;
    }
    
    if ( histManagerNames.size() != 0 ) {
      std::string nextFilterName = "";
      for ( vParameterSet::const_iterator cfgAnalysisSequenceEntry_nextFilter = cfgAnalysisSequenceEntry;
	    cfgAnalysisSequenceEntry_nextFilter != cfgAnalysisSequenceEntries.end(); ++cfgAnalysisSequenceEntry_nextFilter ) {
	if (  cfgAnalysisSequenceEntry_nextFilter->exists("filter") ) {
	  nextFilterName = cfgAnalysisSequenceEntry_nextFilter->getParameter<std::string>("filter");
	  break;
	}
      }

      vstring replaceCommands = ( cfgAnalysisSequenceEntry->exists("replace") ) ? 
	cfgAnalysisSequenceEntry->getParameter<vstring>("replace") : vstring();
      //std::cout << " replaceCommands = " << format_vstring(replaceCommands) << std::endl;
      
      addHistManagers(histManagerNames, lastFilterName, nextFilterName, replaceCommands);
    }
  }

  if ( cfgError_ ) return;

//--- configure filter statistics service
//    and create filter statistics table
  filterStatisticsService_ = new FilterStatisticsService();
  filterStatisticsService_dqmDirectory_ = dqmDirectoryName(name_).append("FilterStatistics");

  edm::ParameterSet cfgFilterStatisticsTable;
  cfgFilterStatisticsTable.addParameter<std::string>("name", std::string(name_).append("-").append("FilterStatisticsService"));
  vParameterSet filterStatisticsTable_config;
  for ( vParameterSet::const_iterator cfgAnalysisSequenceEntry = cfgAnalysisSequenceEntries.begin();
	cfgAnalysisSequenceEntry != cfgAnalysisSequenceEntries.end(); ++cfgAnalysisSequenceEntry ) {
    if ( cfgAnalysisSequenceEntry->exists("filter") ) {
      std::string filterName = cfgAnalysisSequenceEntry->getParameter<std::string>("filter");
      std::string filterTitle = ( cfgAnalysisSequenceEntry->exists("title") ) ? 
	cfgAnalysisSequenceEntry->getParameter<std::string>("title") : filterName;
          
      edm::ParameterSet config;
      config.addParameter<std::string>("filterName", filterName);
      config.addParameter<std::string>("filterTitle", filterTitle);
      
      filterStatisticsTable_config.push_back(config);
    }
  }
  cfgFilterStatisticsTable.addParameter<vParameterSet>("config", filterStatisticsTable_config);
  filterStatisticsTable_ = filterStatisticsService_->createFilterStatisticsTable(cfgFilterStatisticsTable);
 
//--- configure run & event number service
  edm::ParameterSet cfgRunEventNumberService;
  cfgRunEventNumberService.addParameter<std::string>("name", std::string(name_).append("-").append("RunEventNumberService"));
  cfgRunEventNumberService.addParameter<std::string>("dqmDirectory_store", dqmDirectoryName(name_).append("FilterStatistics"));
  vParameterSet runEventNumberService_config;
  for ( vParameterSet::const_iterator cfgAnalysisSequenceEntry = cfgAnalysisSequenceEntries.begin();
	cfgAnalysisSequenceEntry != cfgAnalysisSequenceEntries.end(); ++cfgAnalysisSequenceEntry ) {
    if ( cfgAnalysisSequenceEntry->exists("filter") ) {
      std::string filterName = cfgAnalysisSequenceEntry->getParameter<std::string>("filter");
      vstring saveRunEventNumbers = cfgAnalysisSequenceEntry->exists("saveRunEventNumbers") ? 
	cfgAnalysisSequenceEntry->getParameter<vstring>("saveRunEventNumbers") : vstring();
      
      edm::ParameterSet config;
      config.addParameter<std::string>("filterName", filterName);
      config.addParameter<vstring>("saveRunEventNumbers", saveRunEventNumbers);
      
      runEventNumberService_config.push_back(config);
    }
  }
  cfgRunEventNumberService.addParameter<vParameterSet>("config", runEventNumberService_config);
  runEventNumberService_ = new RunEventNumberService(cfgRunEventNumberService);

//--- configure eventDumps
  if ( cfg.exists("eventDumps") ) {
    vParameterSet cfgEventDumps = cfg.getParameter<vParameterSet>("eventDumps");
    for ( vParameterSet::const_iterator cfgEventDump = cfgEventDumps.begin(); 
	  cfgEventDump != cfgEventDumps.end(); ++cfgEventDump ) {
      std::string eventDumpName = cfgEventDump->getParameter<std::string>("pluginName");
      //std::cout << " eventDumpName = " << eventDumpName << std::endl;

      std::string eventDumpType = cfgEventDump->getParameter<std::string>("pluginType");
      //std::cout << " eventDumpType = " << eventDumpType << std::endl;

      EventDumpBase* entry = EventDumpPluginFactory::get()->create(eventDumpType, *cfgEventDump);
      eventDumps_.push_back(entry);
    }
  }
}

GenericAnalyzer::~GenericAnalyzer()
{
  for ( std::list<analysisSequenceEntry*>::const_iterator entry = analysisSequence_.begin();
	entry != analysisSequence_.end(); ++entry ) {
    delete (*entry);
  }

  delete filterStatisticsService_;
  delete filterStatisticsTable_;
  delete runEventNumberService_;
}

void GenericAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{  
  //std::cout << "<GenericAnalyzer::analyze>:" << std::endl; 

//--- check that configuration parameters contain no errors
  if ( cfgError_ ) {
    edm::LogError ("GenericAnalyzer::analyze") << " Error in Configuration ParameterSet --> skipping !!";
    return;
  }

  double eventWeight = 1.; // not implemented yet

//--- fill histograms
  typedef std::vector<std::pair<std::string, bool> > filterResults_type;
  filterResults_type filterResults_cumulative;
  bool previousFiltersPassed = true;
  filterResults_type filterResults_individual;
  for ( std::list<analysisSequenceEntry*>::iterator entry = analysisSequence_.begin();
	entry != analysisSequence_.end(); ++entry ) {
    //std::cout << " entryName = " << (*entry)->name_ << std::endl;
    //std::cout << "  entryType = " << (*entry)->type() << std::endl;

    if ( (*entry)->type() == analysisSequenceEntry::kFilter ) {
      bool filterPassed_cumulative = (*entry)->filter_cumulative(iEvent, iSetup);
      //std::cout << "  filterPassed_cumulative = " << filterPassed_cumulative << std::endl;
      
      filterResults_cumulative.push_back(std::pair<std::string, bool>((*entry)->name_, filterPassed_cumulative));
      
      if ( !filterPassed_cumulative ) previousFiltersPassed = false;
      //std::cout << "  previousFiltersPassed = " << previousFiltersPassed << std::endl;

      bool filterPassed_individual = (*entry)->filter_individual(iEvent, iSetup);
      //std::cout << "  filterPassed_individual = " << filterPassed_individual << std::endl;
      
      filterResults_individual.push_back(std::pair<std::string, bool>((*entry)->name_, filterPassed_individual));
    }

    if ( (*entry)->type() == analysisSequenceEntry::kHistManagers ) {
      if ( previousFiltersPassed ) (*entry)->analyze(iEvent, iSetup);
    }
  }

//--- update filter statistics table
  filterStatisticsTable_->update(filterResults_cumulative, filterResults_individual, eventWeight);

//--- save run & event numbers
  runEventNumberService_->update(iEvent.id().run(), iEvent.id().event(), iEvent.luminosityBlock(),
				 filterResults_cumulative, filterResults_individual, eventWeight);

//--- if requested, dump event information 
  for ( std::list<EventDumpBase*>::const_iterator it = eventDumps_.begin();
	it != eventDumps_.end(); ++it ) {
    EventDumpBase* eventDump = (*it);
    eventDump->analyze(iEvent, iSetup, filterResults_cumulative, filterResults_individual, eventWeight);
  }
}

void GenericAnalyzer::beginJob(const edm::EventSetup& iSetup)
{
  //std::cout << "<GenericAnalyzer::beginJob>:" << std::endl;

  benchmark_.Start(std::string("GenericAnalyzer").append("-").append(name_).data());

//--- call beginJob method of each EDFilter/EDAnalyzer
  for ( std::list<analysisSequenceEntry*>::iterator entry = analysisSequence_.begin();
	entry != analysisSequence_.end(); ++entry ) {
    (*entry)->beginJob(iSetup);
  }
}

void GenericAnalyzer::endJob()
{
  //std::cout << "<GenericAnalyzer::endJob>:" << std::endl;

  if ( cfgError_ ) return;

//--- call endJob method of each EDFilter/EDAnalyzer
  for ( std::list<analysisSequenceEntry*>::iterator entry = analysisSequence_.begin();
	entry != analysisSequence_.end(); ++entry ) {
    (*entry)->endJob();
  }

  benchmark_.Show(std::string("GenericAnalyzer").append("-").append(name_).data());

//--- print filter statistics (cut-flow) table
//    and save results in DQM file
  filterStatisticsTable_->print(std::cout);
  filterStatisticsService_->saveFilterStatisticsTable(filterStatisticsService_dqmDirectory_, filterStatisticsTable_);
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(GenericAnalyzer);

