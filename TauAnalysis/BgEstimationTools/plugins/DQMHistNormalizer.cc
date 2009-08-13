#include "TauAnalysis/BgEstimationTools/plugins/DQMHistNormalizer.h"

#include "TauAnalysis/DQMTools/interface/dqmAuxFunctions.h"

// framework & common header files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

//DQM services
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include <TH1.h>

#include <iostream>

DQMHistNormalizer::DQMHistNormalizer(const edm::ParameterSet& cfg)
{
  //std::cout << "<DQMHistNormalizer::DQMHistNormalizer>:" << std::endl;

//--- check if multiple histograms or just a single one should be normalized
  if ( cfg.exists("config") ) {
    typedef std::vector<edm::ParameterSet> vParameterSet;
    vParameterSet cfgJobs = cfg.getParameter<vParameterSet>("config");
    for ( vParameterSet::const_iterator cfgJob = cfgJobs.begin();
	  cfgJob != cfgJobs.end(); ++cfgJob ) {
      jobEntryType jobEntry;
      jobEntry.meNameInput_ = cfgJob->getParameter<std::string>("meNameInput");
      jobEntry.meNameOutput_ = cfgJob->getParameter<std::string>("meNameOutput");
      jobs_.push_back(jobEntry);
    }
  } else {
    jobEntryType jobEntry;
    jobEntry.meNameInput_ = cfg.getParameter<std::string>("meNameInput");
    jobEntry.meNameOutput_ = cfg.getParameter<std::string>("meNameOutput");
    jobs_.push_back(jobEntry);
  } 
  
  norm_ = cfg.getParameter<double>("norm");
}

DQMHistNormalizer::~DQMHistNormalizer() 
{
//--- nothing to be done yet
}

void DQMHistNormalizer::analyze(const edm::Event&, const edm::EventSetup&)
{
//--- nothing to be done yet
}

void DQMHistNormalizer::endJob()
{
  std::cout << "<DQMHistNormalizer::endJob>:" << std::endl;

//--- check that DQMStore service is available
  if ( !edm::Service<DQMStore>().isAvailable() ) {
    edm::LogError ("endJob") << " Failed to access dqmStore --> histograms will NOT be normalized !!";
    return;
  }

  DQMStore& dqmStore = (*edm::Service<DQMStore>());
  
  for ( std::vector<jobEntryType>::const_iterator jobEntry = jobs_.begin();
	jobEntry != jobs_.end(); ++jobEntry ) {
    MonitorElement* meInput = dqmStore.get(jobEntry->meNameInput_);
    TH1* histoInput = ( meInput ) ? meInput->getTH1() : 0;
    if ( !histoInput ) {
      edm::LogError ("endJob") << " Failed to retrieve histogram " << jobEntry->meNameInput_ << " from dqmStore !!";
      continue;
    }

    TH1F* histoOutput = (TH1F*)histoInput->Clone();
    if ( !histoOutput->GetSumw2N() ) histoOutput->Sumw2();
    if ( histoOutput->Integral() != 0. ) histoOutput->Scale(norm_/histoOutput->Integral());
  
    std::string outputHistogramName, outputHistogramDirectory;
    separateMonitorElementFromDirectoryName(jobEntry->meNameOutput_, outputHistogramName, outputHistogramDirectory);
    //std::cout << " meNameOutput = " << meNameOutput_ << std::endl;
    //std::cout << " outputHistogramDirectory = " << outputHistogramDirectory << std::endl;
    //std::cout << " outputHistogramName = " << outputHistogramName << std::endl;
    if ( outputHistogramDirectory != "" ) dqmStore.setCurrentFolder(outputHistogramDirectory);
    dqmStore.book1D(outputHistogramName, histoOutput);
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(DQMHistNormalizer);
