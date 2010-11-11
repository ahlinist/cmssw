#include "TauAnalysis/Core/interface/FilterStatisticsService.h"

#include "TauAnalysis/DQMTools/interface/dqmAuxFunctions.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include <iostream>
#include <iomanip>
#include <algorithm>

const std::string meOptionsNum = std::string(meOptionsSeparator).append("a1").append(meOptionsSeparator).append("s0");
const std::string meNameSuffixNum = std::string("_num").append(meOptionsNum);
const std::string meOptionsNumWeighted = std::string(meOptionsSeparator).append("a1").append(meOptionsSeparator).append("s1");
const std::string meNameSuffixNumWeighted = std::string("_numWeighted").append(meOptionsNumWeighted);
const std::string meTitleSeparator = ". :";

FilterStatisticsService::FilterStatisticsService()
{}

FilterStatisticsService::~FilterStatisticsService()
{
//--- nothing to be done yet...
}

FilterStatisticsTable* FilterStatisticsService::createFilterStatisticsTable(const edm::ParameterSet& cfg) const
{
  return new FilterStatisticsTable(cfg);
}

FilterStatisticsElement* loadFilterStatisticsElement(DQMStore& dqmStore, const std::string& dqmDirectory, const std::string& elementName)
{
//--- load MonitorElements holding the number of
//    unweighted and weighted event counts 

  std::string meName_num = dqmDirectoryName(dqmDirectory).append(elementName).append(meNameSuffixNum);
  MonitorElement* meNum = dqmStore.get(meName_num);
  if ( !meNum ) {
    edm::LogError ("loadFilterStatisticsElement") 
      << " Failed to load meName_num = " << meName_num 
      << " from dqmDirectory = " << dqmDirectory << " --> skipping !!";
    return 0;
  }
  int64_t num = meNum->getIntValue();

  std::string meName_numWeighted = dqmDirectoryName(dqmDirectory).append(elementName).append(meNameSuffixNumWeighted);
  MonitorElement* meNumWeighted = dqmStore.get(meName_numWeighted);
  if ( !meNumWeighted ) {
    edm::LogError ("loadFilterStatisticsElement") 
      << " Failed to load meName_numWeighted = " << meName_numWeighted 
      << " from dqmDirectory = " << dqmDirectory << " --> skipping !!";
    return 0;
  }
  double numWeighted = meNumWeighted->getFloatValue();

  return new FilterStatisticsElement(elementName, num, numWeighted);
}

bool operator<(const std::pair<std::string, FilterStatisticsRow*>& row_1, const std::pair<std::string, FilterStatisticsRow*>& row_2)
{
  return row_1.second->filterId() < row_2.second->filterId();
} 

FilterStatisticsTable* FilterStatisticsService::loadFilterStatisticsTable(const std::string& dqmDirectory) const
{
  //std::cout << "<FilterStatisticsService::loadFilterStatisticsTable>:" << std::endl;

//--- check if DQMStore is available;
//    print an error message and return if not
  if ( !edm::Service<DQMStore>().isAvailable() ) {
    edm::LogError ("FilterStatisticsService::loadFilterStatisticsTable") 
      << " Failed to access dqmStore !!";
    return 0;
  }

  DQMStore& dqmStore = (*edm::Service<DQMStore>());

  FilterStatisticsTable* filterStatisticsTable = new FilterStatisticsTable();

//--- load MonitorElement holding name of FilterStatisticsTable
  //std::cout << " dqmDirectory = " << dqmDirectory << std::endl; 
  dqmStore.setCurrentFolder(dqmDirectory);

  MonitorElement* meFilterStatisticsTableName = dqmStore.get(dqmDirectoryName(dqmDirectory).append("name"));
  //std::cout << " meFilterStatisticsTableName = " << meFilterStatisticsTableName << std::endl;
  filterStatisticsTable->name_ = ( meFilterStatisticsTableName ) ? meFilterStatisticsTableName->getStringValue() : "undefined";
  //std::cout << "--> name = " << filterStatisticsTable->name_ << std::endl;

//--- check for DQM subdirectories
//    and iteratively load all MonitorElements stored in them
  std::vector<std::string> dirNames = dqmStore.getSubdirs();
  //std::cout << "dirNames.size = " << dirNames.size() << std::endl;
  for ( std::vector<std::string>::const_iterator dirName = dirNames.begin();
	dirName != dirNames.end(); ++dirName ) {
    //std::cout << "dirName = " << (*dirName) << std::endl;
    std::string subDirName = dqmSubDirectoryName(dqmDirectory, *dirName);
    std::string dqmDirectory_row = dqmDirectoryName(dqmDirectory).append(subDirName);
    dqmStore.setCurrentFolder(dqmDirectory_row);

    int filterId = -1;
    std::string filterName = *dirName;
    std::string filterTitle = "undefined";

//--- load MonitorElement indicating results of which event filter
//    are stored in DQM directory
    MonitorElement* meTitle = dqmStore.get(dqmDirectoryName(dqmDirectory_row).append("name"));
    std::string meTitleStr = meTitle->getStringValue();
    size_t posSeparator = meTitleStr.find(meTitleSeparator);
    filterId = atoi(std::string(meTitleStr, 0, posSeparator).data());
    filterTitle = std::string(meTitleStr, posSeparator + meTitleSeparator.length());

    FilterStatisticsRow* row = new FilterStatisticsRow(filterId, filterName, filterTitle);

//--- load number of events passing and failing event filter
//    and store as data-members of FilterStatisticsRow 
//    (two separate numbers per filter for unweighted and weighted event count)
    row->numEvents_processed_ = loadFilterStatisticsElement(dqmStore, dqmDirectory_row, fsElement::processed);
    row->numEvents_passed_ = loadFilterStatisticsElement(dqmStore, dqmDirectory_row, fsElement::passed);
    row->numEvents_exclRejected_ = loadFilterStatisticsElement(dqmStore, dqmDirectory_row, fsElement::exclRejected);
    row->numEvents_processed_cumulative_ = loadFilterStatisticsElement(dqmStore, dqmDirectory_row, fsElement::processed_cumulative);
    row->numEvents_passed_cumulative_ = loadFilterStatisticsElement(dqmStore, dqmDirectory_row, fsElement::passed_cumulative);

    filterStatisticsTable->rows_.push_back(FilterStatisticsTable::rowEntry_type(filterName, row));
  }

  if ( !filterStatisticsTable->rows_.size() ) {
    edm::LogError ("FilterStatisticsService::loadFilterStatisticsTable") 
      << " Failed to load any FilterStatisticsRows"
      << " --> skipping !!";
    return 0;
  }

//--- sort rows of FilterStatisticsTable by filterId
  std::sort(filterStatisticsTable->rows_.begin(), filterStatisticsTable->rows_.end()); 

  filterStatisticsTable->numEvents_processed_ = filterStatisticsTable->rows_.front().second->numEvents_processed_->num_;
  filterStatisticsTable->numEvents_passedAllFilters_ = filterStatisticsTable->rows_.back().second->numEvents_passed_cumulative_->num_;

  return filterStatisticsTable;
}

void FilterStatisticsService::saveFilterStatisticsElement(DQMStore& dqmStore, const FilterStatisticsElement* element) const
{
//--- create and save MonitorElements holding the number of
//    unweighted and weighted event counts 
  MonitorElement* meNum = dqmStore.bookInt(std::string(element->name_).append(meNameSuffixNum));
  meNum->Fill(element->num_);

  MonitorElement* meNumWeighted = dqmStore.bookFloat(std::string(element->name_).append(meNameSuffixNumWeighted));
  meNumWeighted->Fill(element->numWeighted_);
}

void FilterStatisticsService::saveFilterStatisticsTable(const std::string& dqmDirectory, 
							const FilterStatisticsTable* filterStatisticsTable) const
{
//--- check that FilterStatisticsService objects holds a FilterStatisticsTable
  if ( !filterStatisticsTable ) {
    edm::LogError ("FilterStatisticsService::saveFilterStatisticsTable") 
      << " FilterStatisticsTable not initialized !!";
    return;
  }

//--- check if DQMStore is available;
//    print an error message and return if not
  if ( !edm::Service<DQMStore>().isAvailable() ) {
    edm::LogError ("FilterStatisticsService::saveFilterStatisticsTable") 
      << " Failed to access dqmStore !!";
    return;
  }

  DQMStore& dqmStore = (*edm::Service<DQMStore>());

//--- create and save MonitorElement holding name of FilterStatisticsTable
  dqmStore.setCurrentFolder(dqmDirectory);
  dqmStore.bookString("name", filterStatisticsTable->name_);

//--- iterate over rows of FilterStatisticsTable;
//    each row represents the statistics of one event filter
//    and gets stored in a separate DQM directory
  for ( std::vector<FilterStatisticsTable::rowEntry_type>::const_iterator it = filterStatisticsTable->rows_.begin();
	it != filterStatisticsTable->rows_.end(); ++it ) {
    const FilterStatisticsRow* row = it->second;

    int filterId = row->filterId_;
    const std::string& filterName = row->filterName_;
    const std::string& filterTitle = row->filterTitle_;

    std::string dqmDirectory_row = dqmDirectoryName(dqmDirectory).append(filterName);
    dqmStore.setCurrentFolder(dqmDirectory_row);

//--- create and save MonitorElement indicating results of which event filter
//    are stored in DQM directory
    std::ostringstream meTitleStr;
    meTitleStr << std::setw(3) << filterId << meTitleSeparator << filterTitle;
    dqmStore.bookString("name", meTitleStr.str());

//--- iterate over data-members of FilterStatisticsRow 
//    and store number of events passing and failing event filter
//    (two separate numbers per filter for unweighted and weighted event count)
    saveFilterStatisticsElement(dqmStore, row->numEvents_processed_);
    saveFilterStatisticsElement(dqmStore, row->numEvents_passed_);
    saveFilterStatisticsElement(dqmStore, row->numEvents_exclRejected_);
    saveFilterStatisticsElement(dqmStore, row->numEvents_processed_cumulative_);
    saveFilterStatisticsElement(dqmStore, row->numEvents_passed_cumulative_);
  }
}
