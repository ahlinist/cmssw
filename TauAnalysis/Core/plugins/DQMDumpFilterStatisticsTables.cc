#include "TauAnalysis/Core/plugins/DQMDumpFilterStatisticsTables.h"

#include "TauAnalysis/DQMTools/interface/dqmAuxFunctions.h"
#include "TauAnalysis/DQMTools/interface/generalAuxFunctions.h"

// framework & common header files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

//DQM services
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include <TPRegexp.h>
#include <TObjArray.h>
#include <TObjString.h>
#include <TString.h>

#include <iostream>
#include <fstream>
#include <map>

DQMDumpFilterStatisticsTables::DQMDumpFilterStatisticsTables(const edm::ParameterSet& cfg)
{
  //std::cout << "<DQMDumpFilterStatisticsTables::DQMDumpFilterStatisticsTables>:" << std::endl;

  cfgError_ = 0;

  filterStatisticsService_ = new FilterStatisticsService();

  edm::ParameterSet dqmDirectoryEntries = cfg.getParameter<edm::ParameterSet>("dqmDirectories");
  vstring dqmDirectoryEntryNames = dqmDirectoryEntries.getParameterNamesForType<std::string>();
  for ( vstring::const_iterator dqmDirectoryEntryName = dqmDirectoryEntryNames.begin(); 
	dqmDirectoryEntryName != dqmDirectoryEntryNames.end(); ++dqmDirectoryEntryName ) {
    std::string dqmDirectoryEntry = dqmDirectoryEntries.getParameter<std::string>(*dqmDirectoryEntryName);

    processes_.push_back(*dqmDirectoryEntryName);

    dqmDirectories_[*dqmDirectoryEntryName] = dqmDirectoryEntry;
  }

  if ( processes_.size() == 0 ) {
    edm::LogError("DQMDumpFilterStatisticsTables") 
      << " Configuration Parameter dqmDirectories contains no Entries --> skipping !!";
    cfgError_ = 1;
  }

  columnsSummaryTable_ = ( cfg.exists("columnsSummaryTable") ) ? cfg.getParameter<vstring>("columnsSummaryTable") : vstring();

  printSummaryTableOnly_ = ( cfg.exists("printSummaryTableOnly") ) ? cfg.getParameter<bool>("printSummaryTableOnly") : false;
}

DQMDumpFilterStatisticsTables::~DQMDumpFilterStatisticsTables() 
{
  delete filterStatisticsService_;

  for ( std::map<std::string, FilterStatisticsTable*>::iterator it = filterStatisticsTables_.begin();
	it != filterStatisticsTables_.end(); ++it ) {
    delete it->second;
  }
}

void DQMDumpFilterStatisticsTables::analyze(const edm::Event&, const edm::EventSetup&)
{
//--- nothing to be done yet
}

typedef std::map<int, double> row_type;
typedef std::map<int, row_type> table_type;

void printSummaryTable(std::ostream& stream, unsigned widthNameColumn, unsigned widthNumberColumns,
		       const std::string& summaryTableType, 
		       const std::vector<std::string>& columnLabels, const std::vector<std::string>& filterTitles,
		       table_type& table, size_t numFilters, size_t numProcesses)
{
  stream << "Summary Table for " << summaryTableType << " Selection:" << std::endl;
  stream << std::endl;
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
  for ( size_t iFilter = 0; iFilter < numFilters; ++iFilter ) {
    stream << std::setw(widthNameColumn) << std::left << filterTitles[iFilter];
    for ( size_t iProcess = 0; iProcess < numProcesses; ++iProcess ) {
      stream << " ";
      stream << std::setw(widthNumberColumns) << std::fixed << std::setprecision(2) << std::right << table[iFilter][iProcess];
    }
    stream << std::endl;
  } 
  for ( unsigned iCharacter = 0; iCharacter < (widthNameColumn + columnLabels.size()*widthNumberColumns + 4); ++iCharacter ) {
    stream << "-";
  }
  stream << std::endl << std::endl;  
}

void DQMDumpFilterStatisticsTables::endJob()
{
  std::cout << "<DQMDumpFilterStatisticsTables::endJob>:" << std::endl;

//--- check that configuration parameters contain no errors
  if ( cfgError_ ) {
    edm::LogError ("endjob") 
      << " Error in Configuration ParameterSet --> FilterStatisticsTables will NOT be printed-out !!";
    return;
  }

//--- check that DQMStore service is available
  if ( !edm::Service<DQMStore>().isAvailable() ) {
    edm::LogError ("endJob") 
      << " Failed to access dqmStore --> FilterStatisticsTables will NOT be printed-out !!";
    return;
  }

//--- load FilterStatisticsTables from DQM directories
  for ( vstring::const_iterator process = processes_.begin();
	process != processes_.end(); ++process ) {
    const std::string& dqmDirectory = dqmDirectories_[*process];
    std::cout << "retrieving FilterStatisticsTable for process = " << (*process) 
	      << " from dqmDirectory = " << dqmDirectory << "..." << std::endl;

    FilterStatisticsTable* filterStatisticsTable = filterStatisticsService_->loadFilterStatisticsTable(dqmDirectory); 

    if ( filterStatisticsTable ) {
      filterStatisticsTables_[*process] = filterStatisticsTable;
    } else {
      edm::LogError ("DQMDumpFilterStatisticsTables") 
	<< " Failed to load FilterStatisticsTable from dqmDirectory = " << dqmDirectory
	<< " --> FilterStatisticsTables will NOT be printed-out !!";
      return;
    }
  }

//--- print FilterStatisticsTables
  if ( !printSummaryTableOnly_ ) {
    for ( std::vector<std::string>::const_iterator process = processes_.begin();
	  process != processes_.end(); ++process ) {
      FilterStatisticsTable* filterStatisticsTable = filterStatisticsTables_[*process];
      std::cout << "Filter Statistics for process = " << (*process) << ":" << std::endl;
      filterStatisticsTable->print(std::cout);
    }
  }

//--- print filter statistics (cut-flow) summary tables
  for ( vstring::const_iterator columnSummaryTable = columnsSummaryTable_.begin();
	columnSummaryTable != columnsSummaryTable_.end(); ++columnSummaryTable ) {

    if ( filterStatisticsTables_.begin() == filterStatisticsTables_.end() ) continue;

    table_type table;

    std::vector<std::string> columnLabels;
    columnLabels.push_back(FilterStatisticsRow::columnLabels()[0]);

//--- check that number of rows in column
//    and labels of cuts match for all processes;
//    print error message, if not
    const FilterStatisticsTable* refFilterStatisticsTable = filterStatisticsTables_.begin()->second;
    std::vector<std::string> refFilterTitleColumn = refFilterStatisticsTable->extractFilterTitleColumn();
    size_t numFilters = refFilterTitleColumn.size();

    size_t numProcesses = processes_.size();
    for ( size_t iProcess = 0; iProcess < numProcesses; ++iProcess ) {
      const std::string& process = processes_[iProcess];

      FilterStatisticsTable* filterStatisticsTable = filterStatisticsTables_[process];

      std::vector<std::string> filterTitleColumn = filterStatisticsTable->extractFilterTitleColumn();
      if ( filterTitleColumn.size() != numFilters ) {
	edm::LogError ("DQMDumpFilterStatisticsTables") 
	  << " Number of entries in Filter Title columns do not match"
	  << " (process = " << process << ": current = " << filterTitleColumn.size() << "," 
	  << " reference = " << numFilters << ")"
	  << " --> FilterStatistics summary Tables will NOT be printed-out !!";
	std::cout << "filterTitleColumn = " << format_vstring(filterTitleColumn) << std::endl;
	std::cout << "refFilterTitleColumn = " << format_vstring(refFilterTitleColumn) << std::endl;
	return;
      } else {
	for ( size_t iFilter = 0; iFilter < numFilters; ++iFilter ) {
	  if ( filterTitleColumn[iFilter] != refFilterTitleColumn[iFilter] ) {
	    edm::LogError ("DQMDumpFilterStatisticsTables") 
	      << " Entries in Filter Title columns do not match"
	      << " (process = " << process << ", row = " << iFilter << ":" 
	      << " current = " << filterTitleColumn[iFilter] << ", reference = " << refFilterTitleColumn[iFilter] << ")"
	      << " --> FilterStatistics summary Tables will NOT be printed-out !!";
	    std::cout << "filterTitleColumn = " << format_vstring(filterTitleColumn) << std::endl;
	    std::cout << "refFilterTitleColumn = " << format_vstring(refFilterTitleColumn) << std::endl;
	    return;
	  }
	}
      } 

      std::vector<double> column = filterStatisticsTable->extractColumn(*columnSummaryTable, true);
      if ( column.size() != numFilters ) {
	edm::LogError ("DQMDumpFilterStatisticsTables") 
	  << " Number of entries in Title and Number columns do not match"
	  << " (process = " << process << ": title entries = " << numFilters << ","
	  << " number entries = " << column.size() << ")"
	  << " --> FilterStatistics summary Tables will NOT be printed-out !!";
	std::cout << "column = " << format_vdouble(column) << std::endl;
	std::cout << "refFilterTitleColumn = " << format_vstring(refFilterTitleColumn) << std::endl;
	return;
      }

      for ( size_t iFilter = 0; iFilter < numFilters; ++iFilter ) {
	table[iFilter][iProcess] = column[iFilter];
      }

      columnLabels.push_back(process);
    }

    printSummaryTable(std::cout, 42, 18, *columnSummaryTable, columnLabels, refFilterTitleColumn, table, numFilters, numProcesses);
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(DQMDumpFilterStatisticsTables);
