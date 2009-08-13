#include "TauAnalysis/BgEstimationTools/plugins/DQMHistSubtractor.h"

#include "TauAnalysis/DQMTools/interface/dqmAuxFunctions.h"
#include "TauAnalysis/BgEstimationTools/interface/histogramAuxFunctions.h"

// framework & common header files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

//DQM services
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include <TH1.h>
#include <TMath.h>

#include <iostream>

DQMHistSubtractor::DQMHistSubtractor(const edm::ParameterSet& cfg)
{
  //std::cout << "<DQMHistSubtractor::DQMHistSubtractor>:" << std::endl;

//--- check if multiple histograms or just a single one should be normalized
  if ( cfg.exists("config") ) {
    typedef std::vector<edm::ParameterSet> vParameterSet;
    vParameterSet cfgJobs = cfg.getParameter<vParameterSet>("config");
    for ( vParameterSet::const_iterator cfgJob = cfgJobs.begin();
	  cfgJob != cfgJobs.end(); ++cfgJob ) {
      jobEntryType jobEntry;
      jobEntry.meNameMinuend_ = cfgJob->getParameter<std::string>("meNameMinuend");
      jobEntry.meNameSubtrahend_ = cfgJob->getParameter<std::string>("meNameSubtrahend");
      jobEntry.meNameDifference_ = cfgJob->getParameter<std::string>("meNameDifference");
      jobs_.push_back(jobEntry);
    } 
  } else {
    jobEntryType jobEntry;
    jobEntry.meNameMinuend_ = cfg.getParameter<std::string>("meNameMinuend");
    jobEntry.meNameSubtrahend_ = cfg.getParameter<std::string>("meNameSubtrahend");
    jobEntry.meNameDifference_ = cfg.getParameter<std::string>("meNameDifference");
    jobs_.push_back(jobEntry);
  }
}

DQMHistSubtractor::~DQMHistSubtractor() 
{
//--- nothing to be done yet
}

void DQMHistSubtractor::analyze(const edm::Event&, const edm::EventSetup&)
{
//--- nothing to be done yet
}

void DQMHistSubtractor::endJob()
{
  std::cout << "<DQMHistSubtractor::endJob>:" << std::endl;

//--- check that DQMStore service is available
  if ( !edm::Service<DQMStore>().isAvailable() ) {
    edm::LogError ("endJob") << " Failed to access dqmStore --> histograms will NOT be subtracted !!";
    return;
  }

  DQMStore& dqmStore = (*edm::Service<DQMStore>());

  for ( std::vector<jobEntryType>::const_iterator jobEntry = jobs_.begin();
	jobEntry != jobs_.end(); ++jobEntry ) {
    MonitorElement* meMinuend = dqmStore.get(jobEntry->meNameMinuend_);
    TH1* histoMinuend = ( meMinuend ) ? meMinuend->getTH1() : 0;
    MonitorElement* meSubtrahend = dqmStore.get(jobEntry->meNameSubtrahend_);
    TH1* histoSubtrahend = ( meSubtrahend ) ? meSubtrahend->getTH1() : 0;
    if ( !(histoMinuend && histoSubtrahend) ) {
      if ( !histoMinuend    ) edm::LogError ("endJob") << " Failed to retrieve histogram " << jobEntry->meNameMinuend_ << " from dqmStore !!";
      if ( !histoSubtrahend ) edm::LogError ("endJob") << " Failed to retrieve histogram " << jobEntry->meNameSubtrahend_ << " from dqmStore !!";
      edm::LogError ("endJob") << " --> histogram = " << jobEntry->meNameDifference_ << " will NOT be produced !!";
      continue;
    }

//--- check that histograms representing minuend and subtrahend
//    have the same binning
    if ( !isCompatibleBinning(histoMinuend, histoSubtrahend) ) {
      edm::LogError ("endJob") << " Incompatible binning of histograms " << jobEntry->meNameMinuend_ << " and " << jobEntry->meNameSubtrahend_ 
			       << " --> histogram = " << jobEntry->meNameDifference_ << " will NOT be produced !!";
      continue;
    }
  
    if ( !histoMinuend->GetSumw2N()    ) histoMinuend->Sumw2();
    if ( !histoSubtrahend->GetSumw2N() ) histoSubtrahend->Sumw2();

    TH1F* histoDifference = (TH1F*)histoMinuend->Clone();
    if ( !histoDifference->GetSumw2N() ) histoDifference->Sumw2();

    unsigned numBins = histoMinuend->GetNbinsX();
    for ( unsigned iBin = 0; iBin <= numBins; ++iBin ) {
      double binContent = histoMinuend->GetBinContent(iBin) - histoSubtrahend->GetBinContent(iBin);
      histoDifference->SetBinContent(iBin, binContent);

      double binError2_minuend = histoMinuend->GetBinError(iBin)*histoMinuend->GetBinError(iBin);
      double binError2_subtrahend = histoSubtrahend->GetBinError(iBin)*histoSubtrahend->GetBinError(iBin);
      double binError = TMath::Sqrt(binError2_minuend + binError2_subtrahend);
      histoDifference->SetBinError(iBin, binError);
    }

    std::string diffHistogramName, diffHistogramDirectory;
    separateMonitorElementFromDirectoryName(jobEntry->meNameDifference_, diffHistogramName, diffHistogramDirectory);
    //std::cout << " meNameDifference = " << jobEntry->meNameDifference_ << std::endl;
    //std::cout << " diffHistogramDirectory = " << diffHistogramDirectory << std::endl;
    //std::cout << " diffHistogramName = " << diffHistogramName << std::endl;
    if ( diffHistogramDirectory != "" ) dqmStore.setCurrentFolder(diffHistogramDirectory);
    dqmStore.book1D(diffHistogramName, histoDifference);
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(DQMHistSubtractor);
