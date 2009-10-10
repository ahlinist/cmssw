#include "TauAnalysis/DQMTools/plugins/DQMSimpleFileSaver.h"

// framework & common header files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

//DQM services
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include <iostream>

DQMSimpleFileSaver::DQMSimpleFileSaver(const edm::ParameterSet& cfg)
{
  //std::cout << "<DQMSimpleFileSaver::DQMSimpleFileSaver>:" << std::endl;

  cfgError_ = 0;

  dqmDirectories_drop_ = cfg.exists("drop") ? cfg.getParameter<vstring>("drop") : vstring();

  outputFileName_ = cfg.getParameter<std::string>("outputFileName");
  if ( outputFileName_ == "" ) {
    edm::LogError("DQMSimpleFileSaver") << " No outputFileName specified --> histograms will NOT be saved !!";
    cfgError_ = 1;
  } else if ( outputFileName_.find(".root") == std::string::npos ) {
    edm::LogError("DQMSimpleFileSaver") << " Invalid outputFileName = " << outputFileName_ << " --> histograms will NOT be saved !!";
    cfgError_ = 1;
  }
}

DQMSimpleFileSaver::~DQMSimpleFileSaver() 
{
// nothing to be done yet...
}

void DQMSimpleFileSaver::analyze(const edm::Event&, const edm::EventSetup&)
{
// nothing to be done yet...
}

void DQMSimpleFileSaver::endJob()
{
  std::cout << "<DQMSimpleFileSaver::endJob>:" << std::endl;

//--- check that configuration parameters contain no errors
  if ( cfgError_ ) {
    edm::LogError ("endJob") << " Error in Configuration ParameterSet --> histograms will NOT be saved !!";
    return;
  }

//--- check that DQMStore service is available
  if ( !edm::Service<DQMStore>().isAvailable() ) {
    edm::LogError ("endJob") << " Failed to access dqmStore --> histograms will NOT be saved !!";
    return;
  }  

  DQMStore& dqmStore = (*edm::Service<DQMStore>());

//--- delete all MonitorElements in directories specified to be dropped 
//    before saving MonitorElements (the ones that were not dropped) into ROOT file
  for ( vstring::const_iterator dqmDirectory_drop = dqmDirectories_drop_.begin();
	dqmDirectory_drop != dqmDirectories_drop_.end(); ++dqmDirectory_drop ) {
    
    std::cout << " dropping all MonitorElements in directory = " << (*dqmDirectory_drop) << std::endl;

    dqmStore.rmdir(*dqmDirectory_drop);
  }

  //dqmStore.showDirStructure();
  
  dqmStore.save(outputFileName_);      
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(DQMSimpleFileSaver);
