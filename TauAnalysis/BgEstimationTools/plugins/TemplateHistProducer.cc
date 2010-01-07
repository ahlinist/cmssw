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
    vParameterSet cfgHistograms = cfg.getParameter<vParameterSet>("config");
    for ( vParameterSet::const_iterator cfgHistogram = cfgHistograms.begin();
	  cfgHistogram != cfgHistograms.end(); ++cfgHistogram ) {
      readHistEntry(*cfgHistogram);
    }
  } else {
    readHistEntry(cfg);
  }

  vstring branchNames_eventWeight = ( cfg.exists("branchNamesEventWeight") ) ? 
    cfg.getParameter<vstring>("branchNamesEventWeight") : vstring();
  for ( vstring::const_iterator branchName_eventWeight = branchNames_eventWeight.begin();
	branchName_eventWeight != branchNames_eventWeight.end(); ++branchName_eventWeight ) {
    eventWeightEntryType eventWeightEntry(*branchName_eventWeight);
    eventWeights_.push_back(eventWeightEntry);
  }

  norm_ = ( cfg.exists("norm") ) ? cfg.getParameter<double>("norm") : -1.;
}

TemplateHistProducer::~TemplateHistProducer()
{
//--- nothing to be done yet...
}

void TemplateHistProducer::readHistEntry(const edm::ParameterSet& cfg)
{
  //std::cout << "<TemplateHistProducer::readHistEntry>:" << std::endl;

  histEntryType histEntry;

  typedef std::vector<edm::ParameterSet> vParameterSet;
  vParameterSet cfgVariables = cfg.getParameter<vParameterSet>("variables");
  for ( vParameterSet::const_iterator cfgVariable = cfgVariables.begin();
	cfgVariable != cfgVariables.end(); ++cfgVariable ) {
    histEntryType::axisEntryType axisEntry;

    axisEntry.branchName_ = cfgVariable->getParameter<std::string>("branchName");
    axisEntry.numBins_ = cfgVariable->getParameter<unsigned>("numBins");
    axisEntry.binEdges_.Set(axisEntry.numBins_ + 1);

    if ( cfgVariable->exists("min") && cfgVariable->exists("max") && cfgVariable->exists("binEdges") ) {
      edm::LogError ("readHistEntry") << " Configuration Parameters 'min' and 'max'" 
				      << " and Configuration Parameter 'binEdges' are mutually exclusive !!";
      cfgError_ = 1;
    } else if ( cfgVariable->exists("min") && cfgVariable->exists("max") ) {
      double min = cfgVariable->getParameter<double>("min");
      double max = cfgVariable->getParameter<double>("max");

      double binWidth = (max - min)/axisEntry.numBins_;

      for ( unsigned iBin = 0; iBin <= axisEntry.numBins_; ++iBin ) {
	axisEntry.binEdges_[iBin] = min + iBin*binWidth;
      }
    } else if ( cfgVariable->exists("binEdges") ) {
      typedef std::vector<double> vdouble;
      vdouble binEdges = cfgVariable->getParameter<vdouble>("binEdges");

      if ( binEdges.size() == (axisEntry.numBins_ + 1) ) {
	for ( unsigned iBin = 0; iBin <= axisEntry.numBins_; ++iBin ) {
	  axisEntry.binEdges_[iBin] = binEdges[iBin];
	}
      } else {
	edm::LogError ("readHistEntry") << " Number of Entries in Configuration Parameter 'binEdges' = " << binEdges.size() 
					<< " does not match value of (numBins + 1) = " << (axisEntry.numBins_ + 1) << " !!";
	cfgError_ = 1;
      }
    } else {
      edm::LogError ("readHistEntry") << " Either Configuration Parameters 'min' and 'max'" 
				      << " or Configuration Parameter 'binEdges' must be specified !!";
      cfgError_ = 1;
    } 
    histEntry.axisEntries_.push_back(axisEntry);
  }

  std::string meName = cfg.getParameter<std::string>("meName");
  if ( meName.find("/") != std::string::npos ) {
    size_t posSeparator = meName.find_last_of("/");

    if ( posSeparator < (meName.length() - 1) ) {
      histEntry.dqmDirectory_store_ = std::string(meName, 0, posSeparator);
      histEntry.meName_ = std::string(meName, posSeparator + 1);
    } else {
      edm::LogError ("readHistEntry") << " Invalid Configuration Parameter 'meName' = " << meName << " !!";
      cfgError_ = 1;
    }
  } else {
    histEntry.dqmDirectory_store_ = "";
    histEntry.meName_ = meName;
  }

  histEntry.sumWeights_ = ( cfg.exists("sumWeights") ) ? cfg.getParameter<bool>("sumWeights") : true;

  //std::cout << " dqmDirectory_store = " << histEntry.dqmDirectory_store_ << std::endl;
  //std::cout << " meName = " << histEntry.meName_ << std::endl;

  histograms_.push_back(histEntry);
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
			     << " --> histograms will NOT be filled !!";
    return;
  }

  DQMStore& dqmStore = (*edm::Service<DQMStore>());

//--- book template histograms
  for ( std::vector<histEntryType>::iterator histEntry = histograms_.begin();
	histEntry != histograms_.end(); ++histEntry ) {
    
    if ( histEntry->dqmDirectory_store_ != "" ) dqmStore.setCurrentFolder(histEntry->dqmDirectory_store_);
    
    if ( histEntry->axisEntries_.size() == 1 ) {
      histEntry->me_ = dqmStore.book1D(histEntry->meName_, histEntry->meName_, 
				       histEntry->axisEntries_[0].numBins_, histEntry->axisEntries_[0].binEdges_.GetArray()); 
    } else if ( histEntry->axisEntries_.size() == 2 ) {
      histEntry->me_ = dqmStore.book2D(histEntry->meName_, histEntry->meName_, 
				       histEntry->axisEntries_[0].numBins_, histEntry->axisEntries_[0].binEdges_.GetArray(),
				       histEntry->axisEntries_[1].numBins_, histEntry->axisEntries_[1].binEdges_.GetArray());
    } else {
      edm::LogError ("endJob") << " Invalid dimension = " << histEntry->axisEntries_.size() 
			       << " requested for histogram = " << histEntry->meName_ << " --> skipping !!";
      continue;
    }

    if ( histEntry->sumWeights_ ) histEntry->me_->getTH1()->Sumw2();
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
  for ( std::vector<histEntryType>::iterator histEntry = histograms_.begin();
	histEntry != histograms_.end(); ++histEntry ) {
    for ( std::vector<histEntryType::axisEntryType>::iterator axisEntry = histEntry->axisEntries_.begin();
	  axisEntry != histEntry->axisEntries_.end(); ++axisEntry ) {
      selEventsTree_->SetBranchAddress(axisEntry->branchName_.data(), &axisEntry->value_);
    }
  }
  
  for ( std::vector<eventWeightEntryType>::iterator eventWeightEntry = eventWeights_.begin();
	eventWeightEntry != eventWeights_.end(); ++eventWeightEntry ) {
    std::cout << "--> setting address of event weight branch = " << eventWeightEntry->branchName_ << std::endl;
    selEventsTree_->SetBranchAddress(eventWeightEntry->branchName_.data(), &eventWeightEntry->value_);
  }

//--- read all entries from chain;
//    fill template histograms
  std::map<std::string, double> numEvents;
    
  int numEntries = selEventsTree_->GetEntries();
  for ( int iEntry = 0 ; iEntry < numEntries; ++iEntry ) {
    selEventsTree_->GetEvent(iEntry);  

    double eventWeight = 1.;
    for ( std::vector<eventWeightEntryType>::iterator eventWeightEntry = eventWeights_.begin();
	  eventWeightEntry != eventWeights_.end(); ++eventWeightEntry ) {
      eventWeight *= eventWeightEntry->value_;

      numEvents[eventWeightEntry->branchName_] += eventWeight;
    }

    //std::cout << " eventWeight = " << eventWeight << std::endl;

    for ( std::vector<histEntryType>::iterator histEntry = histograms_.begin();
	  histEntry != histograms_.end(); ++histEntry ) {
      if ( histEntry->me_->getTH1()->GetDimension() == 1 ) {
	histEntry->me_->Fill(histEntry->axisEntries_[0].value_, eventWeight);
      } else if ( histEntry->me_->getTH1()->GetDimension() == 2 ) {
	histEntry->me_->Fill(histEntry->axisEntries_[0].value_, histEntry->axisEntries_[1].value_, eventWeight);
      }
    }
  }
  
  if ( selEventsTree_ != allEventsTree_ ) delete selEventsTree_;
  delete allEventsTree_;

  for ( std::map<std::string, double>::const_iterator numEvents_i = numEvents.begin();
	numEvents_i != numEvents.end(); ++numEvents_i ) {
    std::cout << " sum of weights after " << numEvents_i->first << " = " << numEvents_i->second << std::endl;
  }
  
//--- normalize template histograms
//    (to unit area)
  if ( norm_ != -1. ) {
    for ( std::vector<histEntryType>::iterator histEntry = histograms_.begin();
	  histEntry != histograms_.end(); ++histEntry ) {
      TH1* histogram = histEntry->me_->getTH1();
      if ( histogram->Integral() != 0. ) {
	if ( !histogram->GetSumw2N() ) histogram->Sumw2();
	histogram->Scale(norm_/histogram->Integral());
      }
    }
  }

  std::cout << "done." << std::endl;
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(TemplateHistProducer);
