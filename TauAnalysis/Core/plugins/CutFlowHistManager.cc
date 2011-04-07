#include "TauAnalysis/Core/plugins/CutFlowHistManager.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"

#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

#include "TauAnalysis/CandidateTools/interface/generalAuxFunctions.h"

#include <TAxis.h>

#include <iostream>

CutFlowHistManager::CutFlowHistManager(const edm::ParameterSet& cfg)
  : HistManagerBase(cfg)
{
  //std::cout << "<JetHistManager::JetHistManager>:" << std::endl;

  srcEventSelFlags_ = cfg.getParameter<vInputTag>("srcEventSelFlags");

  srcGenPileUp_ = cfg.getParameter<edm::InputTag>("srcGenPileUp");
  srcRecVertices_ = cfg.getParameter<edm::InputTag>("srcRecVertices");

  std::string mode_string = cfg.getParameter<std::string>("mode");
  if      ( mode_string == "gen" ) mode_ = kGen;
  else if ( mode_string == "rec" ) mode_ = kRec;
  else throw cms::Exception("CutFlowHistManager") 
    << "Invalid Configuration parameter mode = " << mode_string << "!!\n";

  firstBin_ = cfg.getParameter<unsigned>("firstBin");
  lastBin_ = cfg.getParameter<unsigned>("lastBin");
}

CutFlowHistManager::~CutFlowHistManager()
{
//--- nothing to be done yet...
}

void CutFlowHistManager::bookHistogramsImp()
{
  //std::cout << "<JetHistManager::bookHistogramsImp>:" << std::endl;
  
  for ( unsigned iBin = firstBin_ ; iBin <= lastBin_; ++iBin ) {
    bookCutFlowHistogram((int)iBin);
  }

  bookCutFlowHistogram(-1);
}

void CutFlowHistManager::fillHistogramsImp(const edm::Event& evt, const edm::EventSetup& es, double evtWeight)
{  
  //std::cout << "<CutFlowHistManager::fillHistogramsImp>:" << std::endl; 

  std::vector<bool> evtSelFlags;
  for ( vInputTag::const_iterator srcEventSelFlag = srcEventSelFlags_.begin();
	srcEventSelFlag != srcEventSelFlags_.end(); ++srcEventSelFlag ) {
    edm::Handle<bool> evtSelFlag;
    evt.getByLabel(*srcEventSelFlag, evtSelFlag);
    evtSelFlags.push_back(*evtSelFlag);
  }

  //std::cout << " evtSelFlags = " << format_vbool(evtSelFlags) << std::endl;
  
  int multiplicity = -1;
  if ( mode_ == kGen ) {
    typedef std::vector<PileupSummaryInfo> vPileupSummaryInfo;
    edm::Handle<vPileupSummaryInfo> genPileUp;
    evt.getByLabel(srcGenPileUp_, genPileUp);
    if ( genPileUp->size() == 1 ) multiplicity = genPileUp->begin()->getPU_NumInteractions();
  } else if ( mode_ == kRec ) {
    edm::Handle<reco::VertexCollection> recoVertices;
    evt.getByLabel(srcRecVertices_, recoVertices);
    multiplicity = recoVertices->size();
  }
  
  for ( std::vector<histogramEntryType>::iterator histogram = histograms_.begin();
	histogram != histograms_.end(); ++histogram ) {
    if ( histogram->multiplicity_ == multiplicity ||
	 histogram->multiplicity_ == -1           ) fillCutFlowHistogram(histogram->hCutFlow_, evtSelFlags, evtWeight);
  }
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

void CutFlowHistManager::bookCutFlowHistogram(int multiplicity)
{
  histogramEntryType histogram;

  std::ostringstream histogramName;
  histogramName << "CutFlow";
  if ( multiplicity >= 0 ) histogramName << "ForMultiplicity" << multiplicity;
  histogram.hCutFlow_ = book1D(histogramName.str(), histogramName.str(), srcEventSelFlags_.size(), -0.5, srcEventSelFlags_.size() - 0.5);
  TAxis* xAxis = histogram.hCutFlow_->getTH1()->GetXaxis();
  
  unsigned numEventSelFlags = srcEventSelFlags_.size();
  for ( unsigned iEventSelFlag = 0; iEventSelFlag < numEventSelFlags; ++iEventSelFlag ) {
    xAxis->SetBinLabel(1 + iEventSelFlag, srcEventSelFlags_[iEventSelFlag].label().data());
  }

  histogram.multiplicity_ = multiplicity;
  
  histograms_.push_back(histogram);
}

void CutFlowHistManager::fillCutFlowHistogram(MonitorElement* histogram, const std::vector<bool>& eventSelFlags, double evtWeight)
{
  unsigned numEventSelFlags = eventSelFlags.size();
  for ( unsigned iEventSelFlag = 0; iEventSelFlag < numEventSelFlags; ++iEventSelFlag ) {
    if ( eventSelFlags[iEventSelFlag] ) histogram->Fill(iEventSelFlag, evtWeight);
    else return; // apply cuts "cumulatively"
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, CutFlowHistManager, "CutFlowHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, CutFlowHistManager, "CutFlowHistManager");

#include "TauAnalysis/Core/interface/HistManagerAdapter.h"

typedef HistManagerAdapter<CutFlowHistManager> CutFlowAnalyzer;

DEFINE_FWK_MODULE(CutFlowAnalyzer);
