#include "TauAnalysis/BgEstimationTools/plugins/DQMHistSubtractor.h"

#include "TauAnalysis/DQMTools/interface/dqmAuxFunctions.h"
#include "TauAnalysis/DQMTools/interface/histogramAuxFunctions.h"

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

DQMHistSubtractor::jobEntryType::jobEntryType(const edm::ParameterSet& cfg)
{
  meName_minuend_ = cfg.getParameter<std::string>("meName_minuend");
  meName_minuendErr_ = ( cfg.exists("meName_minuendErr") ) ? cfg.getParameter<std::string>("meName_minuendErr") : "";
  
  meName_subtrahend_ = cfg.getParameter<std::string>("meName_subtrahend");
  meName_subtrahendErr_ = ( cfg.exists("meName_subtrahendErr") ) ? cfg.getParameter<std::string>("meName_subtrahendErr") : "";
  
  meName_difference_ = cfg.getParameter<std::string>("meName_difference");
  meName_differenceErr_ = ( cfg.exists("meName_differenceErr") ) ? cfg.getParameter<std::string>("meName_differenceErr") : "";
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

DQMHistSubtractor::DQMHistSubtractor(const edm::ParameterSet& cfg)
{
  //std::cout << "<DQMHistSubtractor::DQMHistSubtractor>:" << std::endl;

//--- check if multiple histograms or just a single one should be normalized
  if ( cfg.exists("config") ) {
    typedef std::vector<edm::ParameterSet> vParameterSet;
    vParameterSet cfgJobs = cfg.getParameter<vParameterSet>("config");
    for ( vParameterSet::const_iterator cfgJob = cfgJobs.begin();
	  cfgJob != cfgJobs.end(); ++cfgJob ) {
      jobEntryType jobEntry(*cfgJob);
      jobs_.push_back(jobEntry);
    } 
  } else {
    jobEntryType jobEntry(cfg);
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
    MonitorElement* meMinuend = dqmStore.get(jobEntry->meName_minuend_);
    if ( !meMinuend ) {
      edm::LogError ("endJob") 
	<< " Failed to retrieve MonitorElement " << jobEntry->meName_minuend_ << " from dqmStore !!";
      return;
    }
    
    MonitorElement* meSubtrahend = dqmStore.get(jobEntry->meName_subtrahend_);
    if ( !meSubtrahend ) {
      edm::LogError ("endJob") 
	<< " Failed to retrieve MonitorElement " << jobEntry->meName_subtrahend_ << " from dqmStore !!";
      return;
    }

//--- check that MonitorElements for minuend and subtrahend are of the same type
    if ( meMinuend->kind() != meSubtrahend->kind() ) {
      edm::LogError ("endJob") 
	<< " MonitorElements for minuend and subtrahend need to be of the same type !!";
      return;
    }

    int meType_input = meMinuend->kind();

    if ( meType_input == MonitorElement::DQM_KIND_TH1F      ||
	 meType_input == MonitorElement::DQM_KIND_TH1S      ||
	 meType_input == MonitorElement::DQM_KIND_TH2F      ||
	 meType_input == MonitorElement::DQM_KIND_TH2S      ||
	 meType_input == MonitorElement::DQM_KIND_TH3F      ||
	 meType_input == MonitorElement::DQM_KIND_TPROFILE  ||
	 meType_input == MonitorElement::DQM_KIND_TPROFILE2D ) {
      TH1* histoMinuend = meMinuend->getTH1();
      TH1* histoSubtrahend = meSubtrahend->getTH1();

//--- check that histograms representing minuend and subtrahend
//    have the same binning
      if ( !isCompatibleBinning(histoMinuend, histoSubtrahend) ) {
	edm::LogError ("endJob") 
	  << " Incompatible binning of histograms " << jobEntry->meName_minuend_ << " and " << jobEntry->meName_subtrahend_ 
	  << " --> histogram = " << jobEntry->meName_difference_ << " will NOT be produced !!";
	return;
      }
  
      if ( !histoMinuend->GetSumw2N()    ) histoMinuend->Sumw2();
      if ( !histoSubtrahend->GetSumw2N() ) histoSubtrahend->Sumw2();

      std::auto_ptr<TH1> histoDifference(dynamic_cast<TH1*>(histoMinuend->Clone()));
      if ( !histoDifference->GetSumw2N() ) histoDifference->Sumw2();

      unsigned numBins = histoMinuend->GetNbinsX();
      for ( unsigned iBin = 0; iBin <= numBins; ++iBin ) {
	double binContent = histoMinuend->GetBinContent(iBin) - histoSubtrahend->GetBinContent(iBin);
	histoDifference->SetBinContent(iBin, binContent);
	
	double binError_minuend = histoMinuend->GetBinError(iBin);
	double binError_subtrahend = histoSubtrahend->GetBinError(iBin);
	double binError = TMath::Sqrt(binError_minuend*binError_minuend + binError_subtrahend*binError_subtrahend);
	histoDifference->SetBinError(iBin, binError);
      }

//--- register output histogram of difference between minuend and subtrahend as MonitorElement in DQMStore
      dqmRegisterHistogram(dqmStore, histoDifference.release(), jobEntry->meName_difference_);
    } else if ( meType_input == MonitorElement::DQM_KIND_REAL ) {
      double meValue_minuend = meMinuend->getFloatValue();
      double meValue_subtrahend = meSubtrahend->getFloatValue();
      double meValue_difference = meValue_minuend - meValue_subtrahend;
      
      double meValue_differenceErr = 0.; 
      if ( jobEntry->meName_minuendErr_ != "" && jobEntry->meName_subtrahendErr_ != "" ) {
	bool errorFlag;
	double meValue_minuendErr = getValue(dqmStore, jobEntry->meName_minuendErr_, errorFlag);
	double meValue_subtrahendErr = getValue(dqmStore, jobEntry->meName_subtrahendErr_, errorFlag);
	meValue_differenceErr = TMath::Sqrt(meValue_minuendErr*meValue_minuendErr + meValue_subtrahendErr*meValue_subtrahendErr);	
      }

//--- register output value and error of difference between minuend and subtrahend as MonitorElement in DQMStore
      dqmRegisterFloat(dqmStore, meValue_difference, jobEntry->meName_difference_);
      if ( jobEntry->meName_differenceErr_ != "" ) dqmRegisterFloat(dqmStore, meValue_differenceErr, jobEntry->meName_differenceErr_);
    } else {
      edm::LogError ("endJob") 
	<< " MonitorElements to be added of unknown or unsupported type" << " --> skipping !!";
      return;
    }
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(DQMHistSubtractor);
