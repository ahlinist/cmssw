#include "TauAnalysis/RecoTools/interface/ZllRecoilCorrectionHistManager.h"

#include "DataFormats/Candidate/interface/Candidate.h"

#include "TauAnalysis/CandidateTools/interface/candidateAuxFunctions.h"

#include <TMath.h>

ZllRecoilCorrectionHistManager::ZllRecoilCorrectionHistManager(const edm::ParameterSet& cfg)
{}

ZllRecoilCorrectionHistManager::~ZllRecoilCorrectionHistManager()
{
  for ( std::vector<histogramsUvsQtNumVtxType*>::iterator it = histogramsUvsQtNumVtxBinned_.begin();
	it != histogramsUvsQtNumVtxBinned_.end(); ++it ) {
    delete (*it);
  }
}

void ZllRecoilCorrectionHistManager::bookHistograms(TFileDirectory& dir)
{
  histogramLepPlusPt_          = book1D(dir, "lPlusPt",            "P_{T}^{l+}",                              40,          0. ,         100.);
  histogramLepPlusEta_         = book1D(dir, "lPlusEta",           "#eta_{l+}",                               50,         -2.5,         +2.5);
  histogramLepPlusPhi_         = book1D(dir, "lPlusPhi",           "#phi_{l+}",                               36, -TMath::Pi(), +TMath::Pi());

  histogramLepMinusPt_         = book1D(dir, "lMinusPt",           "P_{T}^{l-}",                              40,          0. ,         100.);
  histogramLepMinusEta_        = book1D(dir, "lMinusEta",          "#eta_{l-}",                               50,         -2.5,         +2.5);
  histogramLepMinusPhi_        = book1D(dir, "lMinusPhi",          "#phi_{l-}",                               36, -TMath::Pi(), +TMath::Pi());

  histogramZllCandPt_          = book1D(dir, "ZllCandPt",          "P_{T}^{Z}",                               40,          0. ,         100.);
  histogramZllCandEta_         = book1D(dir, "ZllCandEta",         "#eta_{Z}",                                50,         -2.5,         +2.5);
  histogramZllCandPhi_         = book1D(dir, "ZllCandPhi",         "#phi_{Z}",                                36, -TMath::Pi(), +TMath::Pi());
  histogramZllCandMass_        = book1D(dir, "ZllCandMass",        "M(l+ l-)",                                60,         60. ,         120.);
  
  histogramJetPtAbsEtaLt11_    = book1D(dir, "jetPtAbsEtaLt11",    "P_{T}^{jet} (|#eta_{jet}| < 1.1)",       100,          0. ,         250.);
  histogramJetResAbsEtaLt11_   = book1D(dir, "jetResAbsEtaLt11",   "Jet res. (|#eta_{jet}| < 1.1)",           40,         -1. ,         +1.);
  histogramJetPtAbsEta11to17_  = book1D(dir, "jetPtAbsEta11to17",  "P_{T}^{jet} (1.1 < |#eta_{jet}| < 1.7)", 100,          0. ,         250.);
  histogramJetResAbsEta11to17_ = book1D(dir, "jetResAbsEta11to17", "Jet res. (1.1 < |#eta_{jet}| < 1.7)",     40,         -1. ,         +1.);
  histogramJetPtAbsEta17to23_  = book1D(dir, "jetPtAbsEta17to23",  "P_{T}^{jet} (1.7 < |#eta_{jet}| < 2.3)", 100,          0. ,         250.);
  histogramJetResAbsEta17to23_ = book1D(dir, "jetResAbsEta17to23", "Jet res. (1.7 < |#eta_{jet}| < 2.3)",     40,         -1. ,         +1.);
  histogramJetPtAbsEtaGt23_    = book1D(dir, "jetPtAbsEtaGt23",    "P_{T}^{jet} (|#eta_{jet}| > 2.3)",       100,          0. ,         250.);
  histogramJetResAbsEtaGt23_   = book1D(dir, "jetResAbsEtaGt23",   "Jet res. (|#eta_{jet}| > 2.3)",           40,         -1. ,         +1.);

  histogramMEtS_               = book1D(dir, "metS",               "E_{T}^{miss}",                            30,          0.0,         60.0);
  histogramMEtL_               = book1D(dir, "metL",               "E_{T}^{miss}",                            75,          0.0,        150.0);
  histogramMEtProjParlZ_       = book1D(dir, "metProjParlZ",       "E_{T}^{miss} Proj. parallel Z",           75,        -75.0,        +75.0);
  histogramMEtProjPerpZ_       = book1D(dir, "metProjPerpZ",       "E_{T}^{miss} Proj. perp. Z",              50,        -50.0,        +50.0);

  const int qTnumBins = 22;
  double qTbinning[qTnumBins + 1] = { 
    0., 2.5, 5., 7.5, 10., 12.5, 15., 17.5, 20., 22.5, 25., 27.5, 30., 35., 40., 45., 50., 60., 70., 80., 100., 120., 150. 
  };
  histogramUparlDivQtVsQt_ = book2D(dir, "uParlDivQtVsQt", "u_{#parallel}/q_{T} vs q_{T}",                           
				    qTnumBins, qTbinning, 400,  -5.0,   +5.0);
  histogramUparlVsQt_      = book2D(dir, "uParlVsQt",      "u_{#parallel} vs q_{T}",                           
				    qTnumBins, qTbinning, 130, -250.0,  +75.0);
  histogramUperpDivQtVsQt_ = book2D(dir, "uPerpDivQtVsQt", "u_{#perp}/q_{T} vs q_{T}",                           
				    qTnumBins, qTbinning, 400,  -5.0,   +5.0);
  histogramUperpVsQt_      = book2D(dir, "uPerpVsQt",      "u_{#perp} vs q_{T}",                           
				    qTnumBins, qTbinning,  60, -75.0,  +75.0);
  
  histogramsUvsQtNumVtxBinned_.push_back(new histogramsUvsQtNumVtxType(this, dir, qTnumBins, qTbinning, -1,  2));
  histogramsUvsQtNumVtxBinned_.push_back(new histogramsUvsQtNumVtxType(this, dir, qTnumBins, qTbinning,  3,  5));
  histogramsUvsQtNumVtxBinned_.push_back(new histogramsUvsQtNumVtxType(this, dir, qTnumBins, qTbinning,  6,  8));
  histogramsUvsQtNumVtxBinned_.push_back(new histogramsUvsQtNumVtxType(this, dir, qTnumBins, qTbinning,  9, 11));
  histogramsUvsQtNumVtxBinned_.push_back(new histogramsUvsQtNumVtxType(this, dir, qTnumBins, qTbinning, 12, -1));

  for ( int iNumVtx = 1; iNumVtx <= 25; ++iNumVtx ) {
    histogramsUvsQtNumVtxBinned_.push_back(new histogramsUvsQtNumVtxType(this, dir, qTnumBins, qTbinning, iNumVtx, iNumVtx));
  }

  histogramVtxMultiplicity_  = book1D(dir, "numVertices",        "Num. Vertices",                             25,         -0.5,         24.5);
  histogramRhoNeutral_       = book1D(dir, "rhoNeutral",         "#rho_{neutral}",                            50,          0. ,          12.);
}

void ZllRecoilCorrectionHistManager::fillHistograms(
       const reco::CompositeCandidate& ZllCand, 
       const std::vector<pat::Jet>& jets, const pat::MET& met, size_t vtxMultiplicity, double rhoNeutral, double evtWeight)
{
  assert(ZllCand.numberOfDaughters() == 2);

  reco::Candidate::LorentzVector p4LepPlus;
  if      ( ZllCand.daughter(0)->charge() > +0.5 ) p4LepPlus = ZllCand.daughter(0)->p4();
  else if ( ZllCand.daughter(1)->charge() > +0.5 ) p4LepPlus = ZllCand.daughter(1)->p4();
  else assert(0);

  histogramLepPlusPt_->Fill(p4LepPlus.pt(), evtWeight);
  histogramLepPlusEta_->Fill(p4LepPlus.eta(), evtWeight); 
  histogramLepPlusPhi_->Fill(p4LepPlus.phi(), evtWeight); 

  reco::Candidate::LorentzVector p4LepMinus;
  if      ( ZllCand.daughter(0)->charge() < -0.5 ) p4LepMinus = ZllCand.daughter(0)->p4();
  else if ( ZllCand.daughter(1)->charge() < -0.5 ) p4LepMinus = ZllCand.daughter(1)->p4();
  else assert(0);

  histogramLepMinusPt_->Fill(p4LepMinus.pt(), evtWeight);
  histogramLepMinusEta_->Fill(p4LepMinus.eta(), evtWeight); 
  histogramLepMinusPhi_->Fill(p4LepMinus.phi(), evtWeight); 

  histogramZllCandPt_->Fill(ZllCand.pt(), evtWeight);
  histogramZllCandEta_->Fill(ZllCand.eta(), evtWeight);
  histogramZllCandPhi_->Fill(ZllCand.phi(), evtWeight);
  histogramZllCandMass_->Fill(ZllCand.mass(), evtWeight);
  
  for ( std::vector<pat::Jet>::const_iterator jet = jets.begin();
	jet != jets.end(); ++jet ) {
    if      ( TMath::Abs(jet->eta()) < 1.1 ) histogramJetPtAbsEtaLt11_->Fill(jet->pt(), evtWeight);
    else if ( TMath::Abs(jet->eta()) < 1.7 ) histogramJetPtAbsEta11to17_->Fill(jet->pt(), evtWeight);
    else if ( TMath::Abs(jet->eta()) < 2.3 ) histogramJetPtAbsEta17to23_->Fill(jet->pt(), evtWeight);
    else                                     histogramJetPtAbsEtaGt23_->Fill(jet->pt(), evtWeight);

    if ( jet->genJet() && jet->genJet()->pt() > 0. ) {
      double jetRes = (jet->pt() - jet->genJet()->pt())/jet->genJet()->pt();
      if      ( TMath::Abs(jet->eta()) < 1.1 ) histogramJetResAbsEtaLt11_->Fill(jetRes, evtWeight);
      else if ( TMath::Abs(jet->eta()) < 1.7 ) histogramJetResAbsEta11to17_->Fill(jetRes, evtWeight);
      else if ( TMath::Abs(jet->eta()) < 2.3 ) histogramJetResAbsEta17to23_->Fill(jetRes, evtWeight);
      else                                     histogramJetResAbsEtaGt23_->Fill(jetRes, evtWeight);
    }
  }

  histogramMEtS_->Fill(met.pt(), evtWeight);
  histogramMEtL_->Fill(met.pt(), evtWeight);

  if ( ZllCand.pt() > 0. && met.pt() < 60. ) { // CV: fit Z-recoil correction parameters for events with MEt < 60 GeV only,
                                               //     as di-boson and TTbar backgrounds dominate in high MEt tail
    double qT = ZllCand.pt();
    double qX = ZllCand.px();
    double qY = ZllCand.py();
    
    double metPx = met.px();
    double metPy = met.py();

    double metProjParlZ = (qX*metPx + qX*metPy)/qT;
    double metProjPerpZ = (qX*metPy - qY*metPx)/qT;
    
    histogramMEtProjParlZ_->Fill(metProjParlZ, evtWeight);
    histogramMEtProjPerpZ_->Fill(metProjPerpZ, evtWeight);

    int errorFlag = 0;
    std::pair<double, double> uT = compMEtProjU(ZllCand.p4(), met.px(), met.py(), errorFlag);
    if ( !errorFlag ) {
      double uParl = uT.first;
      double uPerp = uT.second;
      if ( qT > 0. ) histogramUparlDivQtVsQt_->Fill(qT, uParl/qT, evtWeight);
      histogramUparlVsQt_->Fill(qT, uParl, evtWeight);
      if ( qT > 0. ) histogramUperpDivQtVsQt_->Fill(qT, uPerp/qT, evtWeight);
      histogramUperpVsQt_->Fill(qT, uPerp, evtWeight);

      for ( std::vector<histogramsUvsQtNumVtxType*>::iterator it = histogramsUvsQtNumVtxBinned_.begin();
	    it != histogramsUvsQtNumVtxBinned_.end(); ++it ) {
	if ( ((*it)->numVtxMin_ == -1 || (int)vtxMultiplicity >= (*it)->numVtxMin_) &&
	     ((*it)->numVtxMax_ == -1 || (int)vtxMultiplicity <= (*it)->numVtxMax_) ) {
	  if ( qT > 0. ) (*it)->histogramUparlDivQtVsQt_->Fill(qT, uParl/qT, evtWeight);
	  (*it)->histogramUparlVsQt_->Fill(qT, uParl, evtWeight);
	  if ( qT > 0. ) (*it)->histogramUperpDivQtVsQt_->Fill(qT, uPerp/qT, evtWeight);
	  (*it)->histogramUperpVsQt_->Fill(qT, uPerp, evtWeight);
	}
      }
    }
  }

  histogramVtxMultiplicity_->Fill(vtxMultiplicity, evtWeight);
  histogramRhoNeutral_->Fill(rhoNeutral, evtWeight);
}

void ZllRecoilCorrectionHistManager::scaleHistograms(double factor)
{
  for ( std::vector<TH1*>::iterator histogram = histograms_.begin();
	histogram != histograms_.end(); ++histogram ) {
    if ( !(*histogram)->GetSumw2N() ) (*histogram)->Sumw2(); // CV: compute "proper" errors before scaling histogram
    (*histogram)->Scale(factor);
  }
}

TH1* ZllRecoilCorrectionHistManager::book1D(
       TFileDirectory& dir, const std::string& distribution, const std::string& title, int numBins, double min, double max)
{
  TH1* retVal = dir.make<TH1D>(distribution.data(), title.data(), numBins, min, max);
  histograms_.push_back(retVal);
  return retVal;
}

TH2* ZllRecoilCorrectionHistManager::book2D(
       TFileDirectory& dir, const std::string& distribution, const std::string& title, 
       int numBinsX, double xMin, double xMax, int numBinsY, double yMin, double yMax)				 
{
  TH2* retVal = dir.make<TH2D>(distribution.data(), title.data(), numBinsX, xMin, xMax, numBinsY, yMin, yMax);
  histograms_.push_back(retVal);
  return retVal;
}

TH2* ZllRecoilCorrectionHistManager::book2D(
       TFileDirectory& dir, const std::string& distribution, const std::string& title, 
       int numBinsX, double* xBinning, int numBinsY, double yMin, double yMax)				 
{
  TH2* retVal = dir.make<TH2D>(distribution.data(), title.data(), numBinsX, xBinning, numBinsY, yMin, yMax);
  histograms_.push_back(retVal);
  return retVal;
}
