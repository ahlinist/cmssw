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
  hMEtPtResGen_ = book1D("MEtPtResGen", "MEtPtResGen", 100, -5.0, +5.0);
  hMEtPtRecVsGen_ = book2D("MEtPtRecVsGen", "MEtPtRecVsGen", 30, 0., 150., 30, 0., 150.);
  hMEtPhiDiffGen_ = book1D("MEtPhiDiffGen", "MEtPhiDiffGen", 72, -TMath::Pi(), +TMath::Pi());
  hMEtPhiRecVsGen_ = book2D("MEtPhiRecVsGen", "MEtPhiRecVsGen", 36, -TMath::Pi(), +TMath::Pi(), 36, -TMath::Pi(), +TMath::Pi());
  
  hGenMEtPt_ = book1D("GenMEt_Pt", "GenMEt_Pt", 75, 0., 150.);
  hGenMEtPhi_ = book1D("GenMEt_Phi", "GenMEt_Phi", 36, -TMath::Pi(), +TMath::Pi());

  hMEtPtDiffVsGenMEtPt_ = bookProfile1D("MEtPtDiffVsGenMEt_Pt", "MEtPtDiffVsGenMEt_Pt", 75, 0., 150.);
  hMEtPhiDiffVsGenMEtPt_ = bookProfile1D("MEtPhiDiffVsGenMEt_Pt", "MEtPhiDiffVsGenMEt_Pt", 75, 0., 150.);

  hMEtParallelDiffGen_ = book1D("MEtParallelDiffGen", "MEtParallelDiffGen", 100, -50., +50.);
  hMEtPerpendicularDiffGen_ = book1D("MEtPerpendicularDiffGen", "MEtPerpendicularDiffGen", 100, 0., 50.);
}

void PFMEtHistManager::fillHistogramsImp(const edm::Event& evt, const edm::EventSetup& es, double evtWeight)

{  
  //std::cout << "<PFMEtHistManager::fillHistogramsImp>:" << std::endl; 

  edm::Handle<std::vector<pat::MET> > patMETs;
  getCollection(evt, metSrc_, patMETs);

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
      double genMEtPt   = theEventMET.genMET()->pt();
      double genMEtPx   = theEventMET.genMET()->px();
      double genMEtPy   = theEventMET.genMET()->py();
      double genMEtPhi  = theEventMET.genMET()->phi();

      double recoMEtPt  = theEventMET.pt();
      double recoMEtPx  = theEventMET.px();
      double recoMEtPy  = theEventMET.py();
      double recoMEtPhi = theEventMET.phi();

      double metDiffPt  = recoMEtPt  - genMEtPt;
      double metDiffPx  = recoMEtPx  - genMEtPx;
      double metDiffPy  = recoMEtPy  - genMEtPy;
      double metDiffPhi = recoMEtPhi - genMEtPhi;

      hMEtPtDiffGen_->Fill(metDiffPt, evtWeight);
      hMEtPtResGen_->Fill(metDiffPt/TMath::Sqrt(genMEtPt), evtWeight);
      hMEtPtRecVsGen_->Fill(genMEtPt, recoMEtPt, evtWeight);
      hMEtPhiDiffGen_->Fill(metDiffPhi, evtWeight);
      hMEtPhiRecVsGen_->Fill(genMEtPhi, recoMEtPhi, evtWeight);
      
      hGenMEtPt_->Fill(genMEtPt, evtWeight);
      hGenMEtPhi_->Fill(genMEtPhi, evtWeight);
/*
  
  CV: temporary work-around until MonitorElement::Fill(double, double, double) is fixed for TProfiles

      hMEtPtDiffVsGenMEtPt_->Fill(genMEtPt, metDiffPt, evtWeight);
      hMEtPhiDiffVsGenMEtPt_->Fill(genMEtPt, metDiffPhi, evtWeight);
 */
      hMEtPtDiffVsGenMEtPt_->getTProfile()->Fill(genMEtPt, metDiffPt, evtWeight);
      hMEtPhiDiffVsGenMEtPt_->getTProfile()->Fill(genMEtPt, metDiffPhi, evtWeight);

      if ( genMEtPt > 0. ) {
	double diffParallel = (metDiffPx*genMEtPx + metDiffPy*genMEtPy)/genMEtPt;	
	double diffPerpendicular = TMath::Abs(metDiffPx*genMEtPy + metDiffPy*genMEtPx)/genMEtPt;
	
	hMEtParallelDiffGen_->Fill(diffParallel, evtWeight);
	hMEtPerpendicularDiffGen_->Fill(diffPerpendicular, evtWeight);
      }
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

