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

  branchName_objValue_ = cfg.getParameter<std::string>("branchName");
  branchName_eventWeight_ = ( cfg.exists("branchNameEventWeight") ) ? cfg.getParameter<std::string>("branchNameEventWeight") : "";

  std::string meName = cfg.getParameter<std::string>("meName");
  if ( meName.find("/") != std::string::npos ) {
    size_t posSeparator = meName.find_last_of("/");

    if ( posSeparator < (meName.length() - 1) ) {
      dqmDirectory_store_ = std::string(meName, 0, posSeparator);
      meName_ = std::string(meName, posSeparator + 1);
    } else {
      edm::LogError ("TemplateHistProducer") << " Invalid Configuration Parameter 'meName' = " << meName << " !!";
      cfgError_ = 1;
    }
  } else {
    dqmDirectory_store_ = "";
    meName_ = meName;
  }

  norm_ = ( cfg.exists("norm") ) ? cfg.getParameter<double>("norm") : -1.;

  numBinsX_ = cfg.getParameter<unsigned>("numBinsX");
  xMin_ = ( cfg.exists("xMin") ) ? cfg.getParameter<double>("xMin") : 0.;
  xMax_ = ( cfg.exists("xMax") ) ? cfg.getParameter<double>("xMax") : 0.;
  xBins_ = ( cfg.exists("xBins") ) ? cfg.getParameter<vdouble>("xBins") : vdouble();

  if ( (cfg.exists("xMin") && cfg.exists("xMax") && cfg.exists("xBins")) || 
       (!(cfg.exists("xMin") && cfg.exists("xMax")) && !cfg.exists("xBins"))) {
    edm::LogError ("TemplateHistProducer") << " Either Configuration Parameters 'xMin' and 'xMax'" 
					   << " or Configuration Parameter 'xBins' must be specified !!";
    cfgError_ = 1;
  }

  if ( cfg.exists("xBins") && xBins_.size() != (numBinsX_ + 1) ) {
    edm::LogError ("TemplateHistProducer") << " Number of Entries in Configuration Parameter 'xBins' = " << xBins_.size() 
					   << " does not match value of (numBinsX + 1) = " << (numBinsX_ + 1) << " !!";
    cfgError_ = 1;
  }
  
  //std::cout << " dqmDirectory_store = " << dqmDirectory_store_ << std::endl;
  //std::cout << " meName = " << meName_ << std::endl;
}

TemplateHistProducer::~TemplateHistProducer()
{
  //std::cout << "<TemplateHistProducer::~TemplateHistProducer>:" << std::endl;

  //std::cout << " deleting selEventsTree..." << std::endl;
  if ( selEventsTree_ != allEventsTree_ ) delete selEventsTree_;

  //std::cout << " deleting allEventsTree..." << std::endl;
  delete allEventsTree_;
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

  if ( dqmDirectory_store_ != "" ) dqmStore.setCurrentFolder(dqmDirectory_store_);

  if ( xBins_.size() ) {
    unsigned numBins = xBins_.size();
    float* xBins = new float[numBins];
    for ( unsigned iBin = 0; iBin < numBins; ++iBin ) {
      xBins[iBin] = xBins_[iBin];
    }
    hTemplate_ = dqmStore.book1D(meName_, meName_, numBinsX_, xBins);
    delete xBins;
  } else {
    hTemplate_ = dqmStore.book1D(meName_, meName_, numBinsX_, xMin_, xMax_);
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
  selEventsTree_->SetBranchAddress(branchName_objValue_.data(), &objValue_);
  if ( branchName_eventWeight_ != "" ) selEventsTree_->SetBranchAddress(branchName_eventWeight_.data(), &eventWeight_);

//--- read all entries from chain;
//    fill histogram
  double sumWeighted = 0.;

  int numEntries = selEventsTree_->GetEntries();
  for ( int iEntry = 0 ; iEntry < numEntries; ++iEntry ) {
    selEventsTree_->GetEvent(iEntry);  

    if ( branchName_eventWeight_ != "" ) {
      hTemplate_->Fill(objValue_, eventWeight_);
      sumWeighted += eventWeight_;
    } else {
      hTemplate_->Fill(objValue_);
      sumWeighted += 1.;
    }
  }

  std::cout << " sum of weights = " << sumWeighted << std::endl;

//--- normalize histogram 
//    (to unit area)
  if ( norm_ != -1. ) {
    TH1* histogram = hTemplate_->getTH1();
    if ( histogram->Integral() != 0. ) {
      if ( !histogram->GetSumw2N() ) histogram->Sumw2();
      histogram->Scale(norm_/histogram->Integral());
    }
  }

//--- set all bin errors to zero
//    (including underflow and overflow bins)
//
//    WARNING: to be enabled for testing purposes only !!
//
/*
  TH1* histogram = hTemplate_->getTH1();
  unsigned numBins = histogram->GetNbinsX();
  for ( unsigned iBin = 0; iBin <= numBins; ++iBin ) {
    histogram->SetBinError(iBin, 0.);
  }
 */
  std::cout << "done." << std::endl;
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(TemplateHistProducer);
