#include "TauAnalysis/Core/interface/FilterStatisticsService.h"

#include "TauAnalysis/DQMTools/interface/dqmAuxFunctions.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DQMServices/Core/interface/DQMStore.h"

#include <iostream>
#include <iomanip>

FilterStatisticsService::monElement::monElement(const std::string& name)
{
  if ( edm::Service<DQMStore>().isAvailable() ) {
    DQMStore& dqmStore = (*edm::Service<DQMStore>());
    num_ = dqmStore.bookInt(std::string(name).append("_num"));
    numWeighted_ = dqmStore.bookFloat(std::string(name).append("_numWeighted"));
  } else { 
    edm::LogError ("FilterStatisticsService::monElement") << " Failed to access dqmStore !!";
    num_ = NULL;
    numWeighted_ = NULL;
    return;
  }
}

FilterStatisticsService::monElement::~monElement()
{
// nothing to be done yet...
}
 
void FilterStatisticsService::monElement::update(bool filterPassed, double eventWeight)
{
  if ( filterPassed ) {
    num_->Fill(num_->getIntValue() + 1);
    numWeighted_->Fill(numWeighted_->getFloatValue() + eventWeight);
  }
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

FilterStatisticsService::filterEntry::filterEntry(const std::string& name, const std::string& title, unsigned position,
						  const std::string& dqmDirectory_store)
{
  if ( edm::Service<DQMStore>().isAvailable() ) {
    DQMStore& dqmStore = (*edm::Service<DQMStore>());

    dqmDirectory_store_ = dqmDirectory_store;
    dqmStore.setCurrentFolder(dqmDirectory_store_);
 
    std::ostringstream fullTitle;
    fullTitle << std::setw(3) << position << ". : " << title;
    name_ = dqmStore.bookString("name", fullTitle.str());

    processed_ = new monElement("processed");
    passed_ = new monElement("passed");
    exclRejected_ = new monElement("exclRejected");
    processed_cumulative_ = new monElement("processed_cumulative");
    passed_cumulative_ = new monElement("passed_cumulative");
  } else {
    edm::LogError ("FilterStatisticsService::filterEntry") << " Failed to access dqmStore !!";
    name_ = NULL;
    processed_ = NULL;
    passed_ = NULL;
    exclRejected_ = NULL;
    processed_cumulative_ = NULL;
    passed_cumulative_ = NULL;
  }
}

FilterStatisticsService::filterEntry::~filterEntry()
{
  delete processed_;
  delete passed_;
  delete exclRejected_;
  delete processed_cumulative_;
  delete passed_cumulative_;
}

void FilterStatisticsService::filterEntry::update(bool filterPassed_cumulative, bool previousFiltersPassed, bool filterPassed_individual, 
						  unsigned numFiltersPassed_individual, unsigned numFiltersRejected_individual, 
						  double eventWeight)
{
  processed_->update(true, eventWeight);
  passed_->update(filterPassed_individual, eventWeight);
  exclRejected_->update(!filterPassed_individual && numFiltersRejected_individual == 1, eventWeight);
  processed_cumulative_->update(previousFiltersPassed, eventWeight);
  passed_cumulative_->update(previousFiltersPassed && filterPassed_cumulative, eventWeight);
}

void FilterStatisticsService::filterEntry::printNumber(std::ostream& stream, unsigned width, 
						       const FilterStatisticsService::monElement* number) const
{
  stream << " ";
  stream << std::setw(width - 10) << std::setprecision(3) << std::right 
	 << number->num_->getIntValue();
  for ( unsigned iCharacter = 0; iCharacter < 10; ++iCharacter ) {
    stream << " ";
  }
} 

void FilterStatisticsService::filterEntry::printPercentage(std::ostream& stream, unsigned width,
							   const FilterStatisticsService::monElement* numerator, 
							   const FilterStatisticsService::monElement* denominator) const
{
  stream << " ";
  stream << std::setw(width - 11) << std::setprecision(3) << std::right 
	 << 100.*numerator->num_->getIntValue()/denominator->num_->getIntValue() << "%";
  for ( unsigned iCharacter = 0; iCharacter < 10; ++iCharacter ) {
    stream << " ";
  }
} 

void FilterStatisticsService::filterEntry::print(std::ostream& stream, unsigned widthNameColumn, unsigned widthNumberColumns) const
{
  //std::cout << "<FilterStatisticsService::filterEntry::print>:" << std::endl;
  stream << std::setw(widthNameColumn) << std::left << name_->getStringValue();
  printNumber(stream, widthNameColumn, passed_cumulative_);
  printPercentage(stream, widthNumberColumns, passed_cumulative_, processed_cumulative_);
  printPercentage(stream, widthNumberColumns, passed_cumulative_, processed_);
  printPercentage(stream, widthNumberColumns, exclRejected_, processed_);
  stream << std::endl;
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

FilterStatisticsService::FilterStatisticsService(const edm::ParameterSet& cfg)
{
  //std::cout << "<FilterStatisticsService::FilterStatisticsService>:" << std::endl;

  cfgError_ = 0;

  name_ = cfg.getParameter<std::string>("name");
  //std::cout << " name = " << name_ << std::endl;

  dqmDirectory_store_ = cfg.getParameter<std::string>("dqmDirectory_store");
  //std::cout << " dqmDirectory_store = " << dqmDirectory_store_ << std::endl;

  typedef std::vector<edm::ParameterSet> vParameterSet;

  if ( cfg.exists("config") ) {
    unsigned filterId = 1;
    vParameterSet cfgFilters = cfg.getParameter<vParameterSet>("config");
    for ( vParameterSet::const_iterator cfgFilter = cfgFilters.begin(); 
	  cfgFilter != cfgFilters.end(); ++cfgFilter ) {
      std::string filterName = cfgFilter->getParameter<std::string>("filterName");
      std::string filterTitle = ( cfgFilter->exists("filterTitle") ) ? 
	cfgFilter->getParameter<std::string>("filterTitle") : filterName;
      
      std::string dqmDirectory_filter = dqmDirectoryName(dqmDirectory_store_).append(filterName);

      filterEntry* entry = new filterEntry(filterName, filterTitle, filterId, dqmDirectory_filter);
      filterEntries_.push_back(filterEntry_type(filterName, entry));
      ++filterId;
    }
  }

  numEventsProcessed_ = 0;
  numEventsPassedAllFilters_ = 0;
}

FilterStatisticsService::~FilterStatisticsService()
{
  for ( std::vector<filterEntry_type>::const_iterator filterEntry = filterEntries_.begin();
	filterEntry != filterEntries_.end(); ++filterEntry ) {
    delete filterEntry->second;
  }
}

void FilterStatisticsService::update(const filterResults_type& filterResults_cumulative, 
				     const filterResults_type& filterResults_individual, double eventWeight)
{
//--- check that configuration parameters contain no errors
  if ( cfgError_ ) {
    edm::LogError ("FilterStatisticsService::update") << " Error in Configuration ParameterSet --> skipping !!";
    return;
  }  

//--- first pass through filterResults: 
//    count number of filters which passed/rejected the event
  unsigned numFiltersPassed_individual = 0;
  unsigned numFiltersRejected_individual = 0;
  for ( filterResults_type::const_iterator filterResult_individual = filterResults_individual.begin();
	filterResult_individual != filterResults_individual.end(); ++filterResult_individual ) {
    if (  filterResult_individual->second ) ++numFiltersPassed_individual;
    if ( !filterResult_individual->second ) ++numFiltersRejected_individual;
  }

  unsigned numFiltersPassed_cumulative = 0;
  unsigned numFiltersRejected_cumulative = 0;
  for ( filterResults_type::const_iterator filterResult_cumulative = filterResults_cumulative.begin();
	filterResult_cumulative != filterResults_cumulative.end(); ++filterResult_cumulative ) {
    if (  filterResult_cumulative->second ) ++numFiltersPassed_cumulative;
    if ( !filterResult_cumulative->second ) ++numFiltersRejected_cumulative;
  }

//--- second pass through filterResults: 
//    update statistics of individual filters
  bool previousFiltersPassed = true;
  for ( filterResults_type::const_iterator filterResult_cumulative = filterResults_cumulative.begin();
	filterResult_cumulative != filterResults_cumulative.end(); ++filterResult_cumulative ) {
    const std::string& filterName = filterResult_cumulative->first;
    bool filterPassed_cumulative = filterResult_cumulative->second;
    
    filterResults_type::const_iterator filterResult_individual = filterResults_individual.end();
    for ( filterResults_type::const_iterator it = filterResults_individual.begin();
	  it != filterResults_individual.end(); ++it ) {
      if ( it->first == filterName ) {
	filterResult_individual = it;
	break;
      }
    }
    if ( filterResult_individual == filterResults_individual.end() ) {
      edm::LogError ("FilterStatisticsService::update") << " Failed to find filterResult_individual for filterName = " << filterName
							<< " --> skipping !!";     
      continue;
    }

    bool filterPassed_individual = filterResult_individual->second;

    filterEntry* entry = NULL;
    for ( std::vector<filterEntry_type>::const_iterator it = filterEntries_.begin();
	  it != filterEntries_.end(); ++it ) {
      if ( it->first == filterName ) {
	entry = it->second;
	break;
      }
    }
    if ( !entry ) {
      edm::LogError ("FilterStatisticsService::update") << " Failed to access filterConfigEntry for filterName = " << filterName
							<< " --> skipping !!";     
      continue;
    }

    entry->update(filterPassed_cumulative, previousFiltersPassed, filterPassed_individual,
		  numFiltersPassed_individual, numFiltersRejected_individual, eventWeight);
      
    if ( !filterPassed_cumulative ) previousFiltersPassed = false;
  }

  ++numEventsProcessed_;
  if ( numFiltersRejected_cumulative == 0 ) ++numEventsPassedAllFilters_;
}

void FilterStatisticsService::print(std::ostream& stream, unsigned widthNameColumn, unsigned widthNumberColumns) const
{
  //std::cout << "<FilterStatisticsService::print>:" << std::endl;
  stream << "Filter Statistics for " << name_ << std::endl;
  stream << std::endl;
  stream << " number of events processed = " << numEventsProcessed_ << "," 
	 << " of which " <<  numEventsPassedAllFilters_ << " passed all Filters" << std::endl;
  stream << std::endl;
  stream << std::setw(widthNameColumn) << std::left << "Cut";
  std::list<std::string> columnLabels;
  columnLabels.push_back(std::string("Passed"));
  columnLabels.push_back(std::string("Efficiency"));
  columnLabels.push_back(std::string("cumul. Efficiency"));
  columnLabels.push_back(std::string("excl. Rejection"));
  for ( std::list<std::string>::const_iterator columnLabel = columnLabels.begin();
	columnLabel != columnLabels.end(); ++columnLabel ) {
    stream << " "; 
    for ( unsigned iCharacter = 0; iCharacter < (widthNumberColumns - columnLabel->length()); ++iCharacter ) {
      stream << " ";
    }
    //stream << " " << std::setw(widthNumberColumns) << std::right << (*columnLabel);
    stream << " " << std::setw(columnLabel->length()) << std::left << (*columnLabel);
  }
  stream << std::endl;
  for ( unsigned iCharacter = 0; iCharacter < widthNameColumn + columnLabels.size()*(widthNumberColumns + 2); ++iCharacter ) {
    stream << "-";
  }
  stream << std::endl;
  for ( std::vector<filterEntry_type>::const_iterator it = filterEntries_.begin();
	it != filterEntries_.end(); ++it ) {
    filterEntry* entry = it->second;
    entry->print(stream, widthNameColumn, widthNumberColumns);
  }
  for ( unsigned iCharacter = 0; iCharacter < widthNameColumn + columnLabels.size()*(widthNumberColumns + 2); ++iCharacter ) {
    stream << "-";
  }
  stream << std::endl << std::endl;
}


