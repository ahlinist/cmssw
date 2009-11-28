#include "TauAnalysis/Core/plugins/PFMEtHistManager.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/Common/interface/View.h"

#include "TauAnalysis/Core/interface/histManagerAuxFunctions.h"

#include <TMath.h>

PFMEtHistManager::PFMEtHistManager(const edm::ParameterSet& cfg)
  : HistManagerBase(cfg)
{
  //std::cout << "<PFMEtHistManager::PFMEtHistManager>:" << std::endl;

  metSrc_ = cfg.getParameter<edm::InputTag>("metSource");
  //std::cout << " metSrc = " << metSrc_ << std::endl;
  
  if ( cfg.exists("metSignificanceSource") ) metSignificanceSrc_ = cfg.getParameter<edm::InputTag>("metSignificanceSource");
  //std::cout << " metSignificanceSrc = " << metSignificanceSrc_ << std::endl;
}

PFMEtHistManager::~PFMEtHistManager()
{
//--- nothing to be done yet...
}

void PFMEtHistManager::bookHistogramsImp()
{
  //std::cout << "<PFMEtHistManager::bookHistogramsImp>:" << std::endl;
  
  hMEtPt_ = book1D("MEtPt", "MEtPt", 75, 0., 150.);
  hMEtPhi_ = book1D("MEtPhi", "MEtPhi", 36, -TMath::Pi(), +TMath::Pi());
  hMEtPx_ = book1D("MEtPx", "MEtPx", 150, -150., 150.);
  hMEtPy_ = book1D("MEtPy", "MEtPy", 150, -150., 150.);
  
  hMEtSignificance_ = book1D("MEtSignificance", "MEtSignificance", 101, -0.5, 100.05);
  
  hMEtPtDiffGen_ = book1D("MEtPtDiffGen", "MEtPtDiffGen", 100, -50., +50.);
  hMEtPtResGen_ = book1D("MEtPtDiffGen", "MEtPtDiffGen", 100, -5.0, +5.0);
  hMEtPtRecVsGen_ = book2D("MEtPtRecVsGen", "MEtPtRecVsGen", 30, 0., 150., 30, 0., 150.);
  hMEtPhiDiffGen_ = book1D("MEtPhiDiffGen", "MEtPhiDiffGen", 72, -TMath::Pi(), +TMath::Pi());
  hMEtPhiRecVsGen_ = book2D("MEtPhiRecVsGen", "MEtPhiRecVsGen", 36, -TMath::Pi(), +TMath::Pi(), 36, -TMath::Pi(), +TMath::Pi());
  
  hGenMEtPt_ = book1D("GenMEt_Pt", "GenMEt_Pt", 75, 0., 150.);
  hGenMEtPhi_ = book1D("GenMEt_Phi", "GenMEt_Phi", 36, -TMath::Pi(), +TMath::Pi());
}

void PFMEtHistManager::fillHistogramsImp(const edm::Event& evt, const edm::EventSetup& es, double evtWeight)

{  
  //std::cout << "<PFMEtHistManager::fillHistogramsImp>:" << std::endl; 

  edm::Handle<std::vector<pat::MET> > patMETs;
  evt.getByLabel(metSrc_, patMETs);

  double metSignificance = -1.;
  if ( metSignificanceSrc_.label() != "" ) {
     edm::Handle<edm::View<reco::CaloMET> > recoMETs;
     evt.getByLabel(metSignificanceSrc_, recoMETs);

     if ( recoMETs->size() == 1 ) metSignificance = recoMETs->begin()->metSignificance();
  }

  if ( patMETs->size() == 1 ) {
    const pat::MET& theEventMET = (*patMETs->begin());

    hMEtPt_->Fill(theEventMET.pt(), evtWeight);
    hMEtPhi_->Fill(theEventMET.phi(), evtWeight);
    hMEtPx_->Fill(theEventMET.px(), evtWeight);
    hMEtPy_->Fill(theEventMET.py(), evtWeight);

    hMEtSignificance_->Fill(metSignificance, evtWeight);

    if ( theEventMET.genMET() ) {
      hMEtPtDiffGen_->Fill(theEventMET.pt() - theEventMET.genMET()->pt(), evtWeight);
      hMEtPtResGen_->Fill((theEventMET.pt() - theEventMET.genMET()->pt())/TMath::Sqrt(theEventMET.genMET()->pt()), evtWeight);
      hMEtPtRecVsGen_->Fill(theEventMET.genMET()->pt(), theEventMET.pt(), evtWeight);
      hMEtPhiDiffGen_->Fill(theEventMET.phi() - theEventMET.genMET()->phi(), evtWeight);
      hMEtPhiRecVsGen_->Fill(theEventMET.genMET()->phi(), theEventMET.phi(), evtWeight);
      
      hGenMEtPt_->Fill(theEventMET.genMET()->pt(), evtWeight);
      hGenMEtPhi_->Fill(theEventMET.genMET()->phi(), evtWeight);
    }
  } else {
    edm::LogError ("PFMEtHistManager::fillHistograms") << " Exactly one MET object expected per event --> skipping !!";
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, PFMEtHistManager, "PFMEtHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, PFMEtHistManager, "PFMEtHistManager");

#include "TauAnalysis/Core/interface/HistManagerAdapter.h"

typedef HistManagerAdapter<PFMEtHistManager> PFMEtAnalyzer;

DEFINE_ANOTHER_FWK_MODULE(PFMEtAnalyzer);

