#include "TauAnalysis/Core/plugins/DQMDumpBinningResults.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include "TauAnalysis/DQMTools/interface/dqmAuxFunctions.h"
#include "TauAnalysis/DQMTools/interface/generalAuxFunctions.h"
#include "TauAnalysis/Core/interface/binningAuxFunctions.h"

DQMDumpBinningResults::DQMDumpBinningResults(const edm::ParameterSet& cfg)
  : binningService_(0), 
    cfgError_(0)
{
  //std::cout << "<DQMDumpBinningResults::DQMDumpBinningResults>:" << std::endl;

  edm::ParameterSet cfgBinningService = cfg.getParameter<edm::ParameterSet>("binningService");

  edm::ParameterSet dqmDirectoryEntries = cfgBinningService.getParameter<edm::ParameterSet>("dqmDirectories");
  vstring dqmDirectoryEntryNames = dqmDirectoryEntries.getParameterNamesForType<std::string>();
  for ( vstring::const_iterator dqmDirectoryEntryName = dqmDirectoryEntryNames.begin(); 
	dqmDirectoryEntryName != dqmDirectoryEntryNames.end(); ++dqmDirectoryEntryName ) {
    std::string dqmDirectoryEntry = dqmDirectoryEntries.getParameter<std::string>(*dqmDirectoryEntryName);
    
    processes_.push_back(*dqmDirectoryEntryName);
    
    dqmDirectories_[*dqmDirectoryEntryName] = dqmDirectoryEntry;
  }
  
  if ( processes_.size() == 0 ) {
    edm::LogError("DQMDumpBinningResults") 
      << " Configuration Parameter dqmDirectories contains no Entries --> skipping !!";
    cfgError_ = 1;
  }
  
  std::string binningService_pluginType = cfgBinningService.getParameter<std::string>("pluginType");
  binningService_ = BinningServicePluginFactory::get()->create(binningService_pluginType, cfgBinningService);
}

DQMDumpBinningResults::~DQMDumpBinningResults() 
{
  for ( std::map<std::string, BinningBase*>::iterator it = binningResults_.begin();
	it != binningResults_.end(); ++it ) {
    delete it->second;
  }

  delete binningService_;
}

void DQMDumpBinningResults::analyze(const edm::Event&, const edm::EventSetup&)
{
//--- nothing to be done yet
}

BinningBase* DQMDumpBinningResults::loadBinningResults(const std::string& dqmDirectory)
{
  std::cout << "<DQMDumpBinningResults::loadBinningResults>:" << std::endl;
  std::cout << " dqmDirectory = " << dqmDirectory << std::endl;

//--- check that BinningService is available
  if ( !binningService_ ) {
    edm::LogError ("loadBinning") 
      << " BinningService not initialized --> binning results will NOT be loaded !!";
    return 0;
  }

  return binningService_->loadBinningResults(dqmDirectory);
}

void DQMDumpBinningResults::endJob()
{
  //std::cout << "<DQMDumpBinningResults::endJob>:" << std::endl;

//--- check that configuration parameters contain no errors
  if ( cfgError_ ) {
    edm::LogError ("endjob") << " Error in Configuration ParameterSet --> binning results will NOT be printed-out !!";
    return;
  }

//--- check that DQMStore service is available
  if ( !edm::Service<DQMStore>().isAvailable() ) {
    edm::LogError ("endJob") << " Failed to access dqmStore --> binning results will NOT be printed-out !!";
    return;
  }

//--- load objects inheriting from BinningBase class from DQM directories
  for ( vstring::const_iterator process = processes_.begin();
	process != processes_.end(); ++process ) {
    const std::string& dqmDirectory = dqmDirectories_[*process];
    std::cout << "retrieving Binning results for process = " << (*process) 
	      << " from dqmDirectory = " << dqmDirectory << "..." << std::endl;

    BinningBase* binningResult = loadBinningResults(dqmDirectory); 

    if ( binningResult ) {
      binningResults_[*process] = binningResult;
    } else {
      edm::LogError ("DQMDumpBinningResults") 
	<< " Failed to load Binning result from dqmDirectory = " << dqmDirectory
	<< " --> Binning results will NOT be printed-out !!";
      return;
    }
  }

//--- print objects inheriting from BinningBase class 
  for ( std::vector<std::string>::const_iterator process = processes_.begin();
	process != processes_.end(); ++process ) {
    BinningBase* binningResult = binningResults_[*process];
    std::cout << "Binning result for process = " << (*process) << ":" << std::endl;
    binningResult->print(std::cout);
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(DQMDumpBinningResults);
