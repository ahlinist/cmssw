#include "TauAnalysis/Core/interface/FilterStatisticsService.h"

#include "TauAnalysis/DQMTools/interface/dqmAuxFunctions.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DQMServices/Core/interface/DQMStore.h"

#include <iostream>
#include <iomanip>

const std::string meNamePrefixNum = "_num";
const std::string meNamePrefixNumWeighted = "_numWeighted";
const std::string meTitleSeparator = ". :";

FilterStatisticsService::FilterStatisticsService(const edm::ParameterSet& cfg)
{
  name_ = cfg.getParameter<std::string>("name");

  dqmDirectory_store_ = cfg.getParameter<std::string>("dqmDirectory");

  cfgFilterStatisticsTable_ = new edm::ParameterSet(cfg);

  filterStatisticsTable_ = 0;
}

FilterStatisticsService::~FilterStatisticsService()
{
  delete cfgFilterStatisticsTable_;
  delete filterStatisticsTable_;
}

void FilterStatisticsService::createFilterStatisticsTable()
{
  delete filterStatisticsTable_;
  filterStatisticsTable_ = new FilterStatisticsTable(*cfgFilterStatisticsTable_);
}

FilterStatisticsElement* loadFilterStatisticsElement(DQMStore& dqmStore, const std::string& dqmDirectory, const std::string& elementName)
{
//--- load MonitorElements holding the number of
//    unweighted and weighted event counts 
  std::string meName_num = dqmDirectoryName(dqmDirectory).append(elementName).append(meNamePrefixNum);
  MonitorElement* meNum = dqmStore.get(meName_num);
  long num = meNum->getIntValue();

  std::string meName_numWeighted = dqmDirectoryName(dqmDirectory).append(elementName).append(meNamePrefixNumWeighted);
  MonitorElement* meNumWeighted = dqmStore.get(meName_numWeighted);
  double numWeighted = meNumWeighted->getFloatValue();

  return new FilterStatisticsElement(elementName, num, numWeighted);
}

void FilterStatisticsService::loadFilterStatisticsTable()
{
//--- check if DQMStore is available;
//    print and error message and return if not
  if ( !edm::Service<DQMStore>().isAvailable() ) {
    edm::LogError ("FilterStatisticsService::loadFilterStatisticsTable") << " Failed to access dqmStore !!";
    return;
  }

  DQMStore& dqmStore = (*edm::Service<DQMStore>());

  delete filterStatisticsTable_;
  filterStatisticsTable_ = new FilterStatisticsTable();

//--- load MonitorElement holding name of FilterStatisticsTable
  dqmStore.setCurrentFolder(dqmDirectory_store_);
  MonitorElement* meFilterStatisticsTableName = dqmStore.get("name");
  filterStatisticsTable_->name_ = meFilterStatisticsTableName->getStringValue();

//--- check for DQM subdirectories
//    and iteratively load all MonitorElements stored in them
  std::vector<std::string> dirNames = dqmStore.getSubdirs();
  for ( std::vector<std::string>::const_iterator dirName = dirNames.begin();
	dirName != dirNames.end(); ++dirName ) {
    std::string subDirName = dqmSubDirectoryName_merged(dqmDirectory_store_, *dirName);
    
    std::string dqmDirectory_row = dqmDirectoryName(dqmDirectory_store_).append(subDirName);
    dqmStore.setCurrentFolder(dqmDirectory_row);

    int filterId = -1;
    std::string filterName = *dirName;
    std::string filterTitle = "undefined";

//--- load MonitorElement indicating results of which event filter
//    are stored in DQM directory
    MonitorElement* meTitle = dqmStore.get("name");
    std::string meTitleStr = meTitle->getStringValue();
    size_t posSeparator = meTitleStr.find(meTitleSeparator);
    filterId = atoi(std::string(meTitleStr, 0, posSeparator).data());
    filterTitle = std::string(meTitleStr, posSeparator + meTitleSeparator.length());
    
    std::cout << "filterId = " << filterId << std::endl;
    std::cout << "filterName = " << filterName << std::endl;
    std::cout << "filterTitle = " << filterTitle << std::endl;

    FilterStatisticsRow* row = new FilterStatisticsRow(filterId, filterName, filterTitle);

//--- load number of events passing and failing event filter
//    and store as data-members of FilterStatisticsRow 
//    (two separate numbers per filter for unweighted and weighted event count)
    row->numEvents_processed_ = loadFilterStatisticsElement(dqmStore, dqmDirectory_row, fsElement::processed);
    row->numEvents_passed_ = loadFilterStatisticsElement(dqmStore, dqmDirectory_row, fsElement::passed);
    row->numEvents_exclRejected_ = loadFilterStatisticsElement(dqmStore, dqmDirectory_row, fsElement::exclRejected);
    row->numEvents_processed_cumulative_ = loadFilterStatisticsElement(dqmStore, dqmDirectory_row, fsElement::processed_cumulative);
    row->numEvents_passed_cumulative_ = loadFilterStatisticsElement(dqmStore, dqmDirectory_row, fsElement::passed_cumulative);

    filterStatisticsTable_->rows_[filterName] = row;
  }
}

void FilterStatisticsService::saveFilterStatisticsElement(DQMStore& dqmStore, const FilterStatisticsElement* element) const
{
//--- create and save MonitorElements holding the number of
//    unweighted and weighted event counts 
  MonitorElement* meNum = dqmStore.bookInt(std::string(element->name_).append(meNamePrefixNum));
  meNum->Fill(element->num_);

  MonitorElement* meNumWeighted = dqmStore.bookFloat(std::string(element->name_).append(meNamePrefixNumWeighted));
  meNumWeighted->Fill(element->numWeighted_);
}

void FilterStatisticsService::saveFilterStatisticsTable() const
{
//--- check that FilterStatisticsService objects holds a FilterStatisticsTable
  if ( !filterStatisticsTable_ ) {
    edm::LogError ("FilterStatisticsService::saveFilterStatisticsTable") << " FilterStatisticsTable not initialized !!";
    return;
  }

//--- check if DQMStore is available;
//    print and error message and return if not
  if ( !edm::Service<DQMStore>().isAvailable() ) {
    edm::LogError ("FilterStatisticsService::saveFilterStatisticsTable") << " Failed to access dqmStore !!";
    return;
  }

  DQMStore& dqmStore = (*edm::Service<DQMStore>());

//--- create and save MonitorElement holding name of FilterStatisticsTable
  dqmStore.setCurrentFolder(dqmDirectory_store_);
  dqmStore.bookString("name", filterStatisticsTable_->name_);

//--- iterate over rows of FilterStatisticsTable;
//    each row represents the statistics of one event filter
//    and gets stored in a separate DQM directory
  for ( std::map<std::string, FilterStatisticsRow*>::const_iterator it = filterStatisticsTable_->rows_.begin();
	it != filterStatisticsTable_->rows_.end(); ++it ) {
    const FilterStatisticsRow* row = it->second;

    int filterId = row->filterId_;
    const std::string& filterName = row->filterName_;
    const std::string& filterTitle = row->filterTitle_;

    std::string dqmDirectory_row = dqmDirectoryName(dqmDirectory_store_).append(filterName);
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
