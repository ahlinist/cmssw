#include "TauAnalysis/BgEstimationTools/plugins/TemplateHistProducer.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DQMServices/Core/interface/DQMStore.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include <iostream>

const float epsilon = 1.e-3;

TemplateHistProducer::TemplateHistProducer(const edm::ParameterSet& cfg)
  : allEventsTree_(0), selEventsTree_(0),
    cfgError_(0)
{
  //std::cout << "<TemplateHistProducer::TemplateHistProducer>:" << std::endl;

  fileNames_ = cfg.getParameter<vstring>("fileNames");

  treeName_ = cfg.getParameter<std::string>("treeName");

  treeSelection_ = cfg.getParameter<std::string>("treeSelection");

  if ( cfg.exists("config") ) {
    typedef std::vector<edm::ParameterSet> vParameterSet;
    vParameterSet cfgJobs = cfg.getParameter<vParameterSet>("config");
    for ( vParameterSet::const_iterator cfgJob = cfgJobs.begin();
	  cfgJob != cfgJobs.end(); ++cfgJob ) {
      readJobEntry(*cfgJob);
    }
  } else {
    readJobEntry(cfg);
  }

  branchName_eventWeight_ = ( cfg.exists("branchNameEventWeight") ) ? cfg.getParameter<std::string>("branchNameEventWeight") : "";

  norm_ = ( cfg.exists("norm") ) ? cfg.getParameter<double>("norm") : -1.;
}

TemplateHistProducer::~TemplateHistProducer()
{
  //std::cout << "<TemplateHistProducer::~TemplateHistProducer>:" << std::endl;

  //std::cout << " deleting selEventsTree..." << std::endl;
  if ( selEventsTree_ != allEventsTree_ ) delete selEventsTree_;

  //std::cout << " deleting allEventsTree..." << std::endl;
  delete allEventsTree_;
}

void TemplateHistProducer::readJobEntry(const edm::ParameterSet& cfg)
{
  //std::cout << "<TemplateHistProducer::readJobEntry>:" << std::endl;

  jobEntryType jobEntry;

  jobEntry.branchName_objValue_ = cfg.getParameter<std::string>("branchName");

  std::string meName = cfg.getParameter<std::string>("meName");
  if ( meName.find("/") != std::string::npos ) {
    size_t posSeparator = meName.find_last_of("/");

    if ( posSeparator < (meName.length() - 1) ) {
      jobEntry.dqmDirectory_store_ = std::string(meName, 0, posSeparator);
      jobEntry.meName_ = std::string(meName, posSeparator + 1);
    } else {
      edm::LogError ("readJobEntry") << " Invalid Configuration Parameter 'meName' = " << meName << " !!";
      cfgError_ = 1;
    }
  } else {
    jobEntry.dqmDirectory_store_ = "";
    jobEntry.meName_ = meName;
  }

  jobEntry.numBinsX_ = cfg.getParameter<unsigned>("numBinsX");
  jobEntry.xMin_ = ( cfg.exists("xMin") ) ? cfg.getParameter<double>("xMin") : 0.;
  jobEntry.xMax_ = ( cfg.exists("xMax") ) ? cfg.getParameter<double>("xMax") : 0.;
  jobEntry.xBins_ = ( cfg.exists("xBins") ) ? cfg.getParameter<vdouble>("xBins") : vdouble();

  if ( (cfg.exists("xMin") && cfg.exists("xMax") && cfg.exists("xBins")) || 
       (!(cfg.exists("xMin") && cfg.exists("xMax")) && !cfg.exists("xBins"))) {
    edm::LogError ("readJobEntry") << " Either Configuration Parameters 'xMin' and 'xMax'" 
				   << " or Configuration Parameter 'xBins' must be specified !!";
    cfgError_ = 1;
  }

  if ( cfg.exists("xBins") && jobEntry.xBins_.size() != (jobEntry.numBinsX_ + 1) ) {
    edm::LogError ("readJobEntry") << " Number of Entries in Configuration Parameter 'xBins' = " << jobEntry.xBins_.size() 
				   << " does not match value of (numBinsX + 1) = " << (jobEntry.numBinsX_ + 1) << " !!";
    cfgError_ = 1;
  }
  
  //std::cout << " dqmDirectory_store = " << jobEntry.dqmDirectory_store_ << std::endl;
  //std::cout << " meName = " << jobEntry.meName_ << std::endl;

  jobs_.push_back(jobEntry);
}

void TemplateHistProducer::endJob()
{
  std::cout << "<TemplateHistProducer::endJob>:" << std::endl;

//--- check that configuration parameters contain no errors
  if ( cfgError_ ) {
    edm::LogError ("endJob") << " Error in Configuration ParameterSet" 
			     << " --> histogram will NOT be filled !!";
    return;
  }

//--- check that DQMStore service is available
  if ( !edm::Service<DQMStore>().isAvailable() ) {
    edm::LogError ("endJob") << " Failed to access dqmStore" 
			     << " --> histogram will NOT be filled !!";
    return;
  }

  DQMStore& dqmStore = (*edm::Service<DQMStore>());

//--- book template histograms
  for ( std::vector<jobEntryType>::iterator jobEntry = jobs_.begin();
	jobEntry != jobs_.end(); ++jobEntry ) {
    
    if ( jobEntry->dqmDirectory_store_ != "" ) dqmStore.setCurrentFolder(jobEntry->dqmDirectory_store_);
    
    if ( jobEntry->xBins_.size() ) {
      unsigned numBinEdges = jobEntry->xBins_.size();
      float* xBins = new float[numBinEdges];
      for ( unsigned iBin = 0; iBin < numBinEdges; ++iBin ) {
	xBins[iBin] = jobEntry->xBins_[iBin];
      }
      jobEntry->me_ = dqmStore.book1D(jobEntry->meName_, jobEntry->meName_, numBinEdges - 1, xBins);
      delete xBins;
    } else {
      jobEntry->me_ = dqmStore.book1D(jobEntry->meName_, jobEntry->meName_, jobEntry->numBinsX_, jobEntry->xMin_, jobEntry->xMax_);
    }
  }

//--- create chain of all ROOT files 
//    specified in fileNames configuration parameter
  allEventsTree_ = new TChain(treeName_.data());
  for ( std::vector<std::string>::const_iterator fileName = fileNames_.begin();
	fileName != fileNames_.end(); ++fileName ) {
    std::cout << "--> opening " << fileName->data() << "..." << std::endl;
    allEventsTree_->Add(fileName->data());
  }

  std::cout << " entries in Tree = " << allEventsTree_->GetEntries() << std::endl;

//--- copy tree of all events passing selection
  selEventsTree_ = ( treeSelection_ != "" ) ? allEventsTree_->CopyTree(treeSelection_.data()) : allEventsTree_;
  std::cout << " selection = " << treeSelection_ << std::endl;
  std::cout << " entries passing selection = " << selEventsTree_->GetEntries() << std::endl;

//--- set branch adresses
  for ( std::vector<jobEntryType>::iterator jobEntry = jobs_.begin();
	jobEntry != jobs_.end(); ++jobEntry ) {
    selEventsTree_->SetBranchAddress(jobEntry->branchName_objValue_.data(), &jobEntry->objValue_);
  }
  
  if ( branchName_eventWeight_ != "" ) {
    selEventsTree_->SetBranchAddress(branchName_eventWeight_.data(), &eventWeight_);
  } else {
    eventWeight_ = 1.;
  }

//--- read all entries from chain;
//    fill template histograms
  double sumWeighted = 0.;

  int numEntries = selEventsTree_->GetEntries();
  for ( int iEntry = 0 ; iEntry < numEntries; ++iEntry ) {
    selEventsTree_->GetEvent(iEntry);  

    for ( std::vector<jobEntryType>::iterator jobEntry = jobs_.begin();
	  jobEntry != jobs_.end(); ++jobEntry ) {
      jobEntry->me_->Fill(jobEntry->objValue_, eventWeight_);
    }

    sumWeighted += eventWeight_;
  }

  std::cout << " sum of weights = " << sumWeighted << std::endl;

//--- normalize template histograms
//    (to unit area)
  if ( norm_ != -1. ) {
    for ( std::vector<jobEntryType>::iterator jobEntry = jobs_.begin();
	  jobEntry != jobs_.end(); ++jobEntry ) {
      TH1* histogram = jobEntry->me_->getTH1();
      if ( histogram->Integral() != 0. ) {
	if ( !histogram->GetSumw2N() ) histogram->Sumw2();
	histogram->Scale(norm_/histogram->Integral());
      }
    }
  }

//--- set all bin errors to zero
//    (including underflow and overflow bins)
//
//    WARNING: to be enabled for testing purposes only !!
//
/*
  for ( std::vector<jobEntryType>::iterator jobEntry = jobs_.begin();
        jobEntry != jobs_.end(); ++jobEntry ) {
    TH1* histogram = jobEntry->me_->getTH1();
    unsigned numBins = histogram->GetNbinsX();
    for ( unsigned iBin = 0; iBin <= numBins; ++iBin ) {
      histogram->SetBinError(iBin, 0.);
    }
  }
 */
  std::cout << "done." << std::endl;
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(TemplateHistProducer);
