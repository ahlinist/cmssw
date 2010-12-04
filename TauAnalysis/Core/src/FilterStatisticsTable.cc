#include "TauAnalysis/Core/interface/FilterStatisticsTable.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include <iostream>
#include <iomanip>

std::vector<std::string> FilterStatisticsRow::columnLabels_;

FilterStatisticsRow::FilterStatisticsRow(int filterId, const std::string& filterName, const std::string& filterTitle)
  : filterId_(filterId), filterName_(filterName), filterTitle_(filterTitle)
{
//--- initialize "regular" data-members
  numEvents_processed_ = new FilterStatisticsElement(fsElement::processed);
  numEvents_passed_ = new FilterStatisticsElement(fsElement::passed);
  numEvents_exclRejected_ = new FilterStatisticsElement(fsElement::exclRejected);
  numEvents_processed_cumulative_ = new FilterStatisticsElement(fsElement::processed_cumulative);
  numEvents_passed_cumulative_ = new FilterStatisticsElement(fsElement::passed_cumulative);

//--- initialize static data-mebers
  columnLabels_.resize(kNumColumns);
  columnLabels_[FilterStatisticsRow::kFilterTitle] = "Cut";
  columnLabels_[FilterStatisticsRow::kPassed_cumulative] = "Passed";
  columnLabels_[FilterStatisticsRow::kEff_cumulative] = "cumul. Efficiency";
  columnLabels_[FilterStatisticsRow::kEff_marginal] = "margin. Efficiency";
  columnLabels_[FilterStatisticsRow::kEff_individual] = "indiv. Efficiency";
  columnLabels_[FilterStatisticsRow::kExclRejected] = "excl. Rejection";
  columnLabels_[FilterStatisticsRow::kProcessed] = "Processed";
}

FilterStatisticsRow::~FilterStatisticsRow()
{
  delete numEvents_processed_;
  delete numEvents_passed_;
  delete numEvents_exclRejected_;
  delete numEvents_processed_cumulative_;
  delete numEvents_passed_cumulative_;
}

void FilterStatisticsRow::update(bool filterPassed_cumulative, bool previousFiltersPassed, bool filterPassed_individual, 
				 unsigned numFiltersPassed_individual, unsigned numFiltersRejected_individual, 
				 double eventWeight_processed, double eventWeight_passed)
{
  numEvents_processed_->update(true, eventWeight_processed);
  numEvents_passed_->update(filterPassed_individual, eventWeight_processed);
  numEvents_exclRejected_->update(!filterPassed_individual && numFiltersRejected_individual == 1, eventWeight_processed);
  numEvents_processed_cumulative_->update(previousFiltersPassed, eventWeight_processed);
  numEvents_passed_cumulative_->update(previousFiltersPassed && filterPassed_cumulative, eventWeight_passed);
}

double compEff(double numerator, double denominator)
{
  return ( denominator > 0. ) ? numerator/(double)denominator : 0.;
}

double FilterStatisticsRow::extractNumber(const std::string& columnLabel, bool weighted) const
{
  if ( columnLabel == columnLabels_[FilterStatisticsRow::kPassed_cumulative] ) {
    return ( weighted ) ? numEvents_passed_cumulative_->numWeighted() : numEvents_passed_cumulative_->num();
  } else if ( columnLabel == columnLabels_[FilterStatisticsRow::kEff_cumulative] ) {
    return ( weighted ) ? compEff(numEvents_passed_cumulative_->numWeighted(), numEvents_processed_->numWeighted())
      : compEff(numEvents_passed_cumulative_->num(), numEvents_processed_->num());
  } else if ( columnLabel == columnLabels_[FilterStatisticsRow::kEff_marginal] ) {
    return ( weighted ) ? compEff(numEvents_passed_cumulative_->numWeighted(), numEvents_processed_cumulative_->numWeighted())
      : compEff(numEvents_passed_cumulative_->num(), numEvents_processed_cumulative_->num());
  } else if ( columnLabel == columnLabels_[FilterStatisticsRow::kEff_individual] ) {
    return ( weighted ) ? compEff(numEvents_passed_->numWeighted(), numEvents_processed_->numWeighted())
      : compEff(numEvents_passed_->num(), numEvents_processed_->num());
  } else if ( columnLabel == columnLabels_[FilterStatisticsRow::kExclRejected] ) {
    return ( weighted ) ? numEvents_exclRejected_->numWeighted() : numEvents_exclRejected_->num();
  } else if ( columnLabel == columnLabels_[FilterStatisticsRow::kProcessed] ) {
     return ( weighted ) ? numEvents_processed_->numWeighted() : numEvents_processed_->num();
  }

  edm::LogError ("FilterStatisticsRow::extractNumber") << " columnLabel = " << columnLabel << " undefined !!";
  return -1;
}

void printNumber(std::ostream& stream, unsigned width, double number)
{
  stream << " ";
  stream << std::setw(width) << std::fixed << std::setprecision(1) << std::right << number;
} 

void printPercentage(std::ostream& stream, unsigned width, double number)
{
  stream << " ";
  stream << std::setw(width - 1) << std::fixed << std::setprecision(1) << std::right << 100.*number << "%";
} 

void FilterStatisticsRow::print(std::ostream& stream, unsigned widthNameColumn, unsigned widthNumberColumns) const
{
  stream << std::setw(widthNameColumn) << std::left << filterTitle_;
  printNumber(stream, widthNumberColumns, extractNumber(columnLabels_[FilterStatisticsRow::kPassed_cumulative], true));
  printPercentage(stream, widthNumberColumns, extractNumber(columnLabels_[FilterStatisticsRow::kEff_cumulative], true));
  printPercentage(stream, widthNumberColumns, extractNumber(columnLabels_[FilterStatisticsRow::kEff_marginal], true));
  printPercentage(stream, widthNumberColumns, extractNumber(columnLabels_[FilterStatisticsRow::kEff_individual], true));
  printNumber(stream, widthNumberColumns, extractNumber(columnLabels_[FilterStatisticsRow::kExclRejected], true));
  stream << std::endl;
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

FilterStatisticsTable::FilterStatisticsTable()
{
  name_ = "undefined";

  numEvents_processed_ = 0;
  numEvents_passedAllFilters_ = 0;
}

FilterStatisticsTable::FilterStatisticsTable(const edm::ParameterSet& cfg)
{
  name_ = cfg.getParameter<std::string>("name");

  if ( cfg.exists("config") ) {
    typedef std::vector<edm::ParameterSet> vParameterSet;
    vParameterSet cfgFilters = cfg.getParameter<vParameterSet>("config");
    unsigned filterId = 1;
    for ( vParameterSet::const_iterator cfgFilter = cfgFilters.begin(); 
	  cfgFilter != cfgFilters.end(); ++cfgFilter ) {
      std::string filterName = cfgFilter->getParameter<std::string>("filterName");
      std::string filterTitle = ( cfgFilter->exists("filterTitle") ) ? 
	cfgFilter->getParameter<std::string>("filterTitle") : filterName;

      FilterStatisticsRow* row = new FilterStatisticsRow(filterId, filterName, filterTitle);
      rows_.push_back(rowEntry_type(filterName, row));
      ++filterId;
    }
  }

  numEvents_processed_ = 0;
  numEvents_passedAllFilters_ = 0;
}

FilterStatisticsTable::~FilterStatisticsTable()
{
  for ( std::vector<rowEntry_type>::iterator it = rows_.begin();
	it != rows_.end(); ++it ) {
    delete it->second;
  }
}

void FilterStatisticsTable::update(const filterResults_type& filterResults_cumulative, 
				   const filterResults_type& filterResults_individual, 
				   const eventWeights_type& eventWeights_processed, 
				   const eventWeights_type& eventWeights_passed)
{
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
    
    filterResults_type::const_iterator filterResult_individual = filterResults_individual.find(filterName);
    if ( filterResult_individual == filterResults_individual.end() ) {
      edm::LogError ("FilterStatisticsTable::update") 
	<< " Failed to find filterResult_individual for filterName = " << filterName << " --> skipping !!";     
      continue;
    }

    bool filterPassed_individual = filterResult_individual->second;

    eventWeights_type::const_iterator eventWeight_processed = eventWeights_processed.find(filterName);
    if ( eventWeight_processed == eventWeights_processed.end() ) {
      edm::LogError ("FilterStatisticsTable::update") 
	<< " Failed to find eventWeight_processed for filterName = " << filterName << " --> skipping !!";     
      continue;
    }
    
    eventWeights_type::const_iterator eventWeight_passed = eventWeights_passed.find(filterName);
    if ( eventWeight_passed == eventWeights_passed.end() ) {
      edm::LogError ("FilterStatisticsTable::update") 
	<< " Failed to find eventWeight_passed for filterName = " << filterName << " --> skipping !!";     
      continue;
    }
	
    FilterStatisticsRow* row = 0;
    for ( std::vector<rowEntry_type>::iterator it = rows_.begin();
	  it != rows_.end(); ++it ) {
      if ( it->first == filterName ) {
	row = it->second;
	break;
      }
    }
    if ( !row ) {
      edm::LogError ("FilterStatisticsTable::update") 
	<< " Failed to access FilterStatisticsRow for filterName = " << filterName << " --> skipping !!";     
      continue;
    }

    row->update(filterPassed_cumulative, previousFiltersPassed, filterPassed_individual,
		numFiltersPassed_individual, numFiltersRejected_individual, 
		eventWeight_processed->second, eventWeight_passed->second);
      
    if ( !filterPassed_cumulative ) previousFiltersPassed = false;
  }

  ++numEvents_processed_;
  if ( numFiltersRejected_cumulative == 0 ) ++numEvents_passedAllFilters_;
}

std::vector<std::string> FilterStatisticsTable::extractFilterTitleColumn() const
{
  std::vector<std::string> filterTitleColumn;
  for ( std::vector<rowEntry_type>::const_iterator it = rows_.begin();
	it != rows_.end(); ++it ) {
    const FilterStatisticsRow* row = it->second;
    filterTitleColumn.push_back(row->filterTitle());
  }
  return filterTitleColumn;
}

std::vector<double> FilterStatisticsTable::extractColumn(const std::string& columnLabel, bool weighted) const
{
  std::vector<double> column;
  for ( std::vector<rowEntry_type>::const_iterator it = rows_.begin();
	it != rows_.end(); ++it ) {
    const FilterStatisticsRow* row = it->second;
    column.push_back(row->extractNumber(columnLabel, weighted));
  }
  return column;
}

void FilterStatisticsTable::print(std::ostream& stream, unsigned widthNameColumn, unsigned widthNumberColumns) const
{
  stream << "Filter Statistics for " << name_ << std::endl;
  stream << std::endl;
  stream << " number of events processed = " << numEvents_processed_ << "," 
	 << " of which " <<  numEvents_passedAllFilters_ << " passed all Filters" << std::endl;
  stream << std::endl;
  const std::vector<std::string>& columnLabels = FilterStatisticsRow::columnLabels();
  for ( std::vector<std::string>::const_iterator columnLabel = columnLabels.begin();
	columnLabel != columnLabels.end(); ++columnLabel ) {
    if ( columnLabel == columnLabels.begin() ) {
      stream << std::setw(widthNameColumn) << std::left << (*columnLabel);
    } else {
      if ( columnLabel->length() < widthNumberColumns ) {
	for ( unsigned iCharacter = 0; iCharacter < (widthNumberColumns - columnLabel->length()); ++iCharacter ) {
	  stream << " ";
	}
      }
      stream << " " << std::setw(columnLabel->length()) << std::left << (*columnLabel);
    }
  }
  stream << std::endl;
  for ( unsigned iCharacter = 0; iCharacter < (widthNameColumn + columnLabels.size()*widthNumberColumns + 4); ++iCharacter ) {
    stream << "-";
  }
  stream << std::endl;
  for ( std::vector<rowEntry_type>::const_iterator it = rows_.begin();
	it != rows_.end(); ++it ) {
    const FilterStatisticsRow* row = it->second;
    row->print(stream, widthNameColumn, widthNumberColumns);
  }
  for ( unsigned iCharacter = 0; iCharacter < (widthNameColumn + columnLabels.size()*widthNumberColumns + 4); ++iCharacter ) {
    stream << "-";
  }
  stream << std::endl << std::endl;
}



double FilterStatisticsTable::extractNumber(const std::string& row, const std::string& col, bool weighted) const
{
   // Find the appropriate row
   for(std::vector<rowEntry_type>::const_iterator irow = rows_.begin(); irow != rows_.end(); ++irow)
   {
      if(irow->first == row)
      {
         return irow->second->extractNumber(col, weighted);
      }
   }
   edm::LogError ("FilterStatisticsTable::extractNumber") << " filterName = " << row << " undefined !!";
   return -1;
}




