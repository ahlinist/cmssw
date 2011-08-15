#include "TauAnalysis/RecoTools/interface/ZllRecoilCorrectionHistManager.h"

#include "DataFormats/Candidate/interface/Candidate.h"

#include <TMath.h>

ZllRecoilCorrectionHistManager::ZllRecoilCorrectionHistManager(const edm::ParameterSet& cfg)
{}

ZllRecoilCorrectionHistManager::~ZllRecoilCorrectionHistManager()
{
// nothing to be done yet...
}

void ZllRecoilCorrectionHistManager::bookHistograms(TFileDirectory& dir)
{
  histogramLepPlusPt_       = book1D(dir, "lPlusPt",            "P_{T}^{l+}",                            40,          0. ,         100.);
  histogramLepPlusEta_      = book1D(dir, "lPlusEta",           "#eta_{l+}",                             50,         -2.5,         +2.5);
  histogramLepPlusPhi_      = book1D(dir, "lPlusPhi",           "#phi_{l+}",                             36, -TMath::Pi(), +TMath::Pi());

  histogramLepMinusPt_      = book1D(dir, "lMinusPt",           "P_{T}^{l-}",                            40,          0. ,         100.);
  histogramLepMinusEta_     = book1D(dir, "lMinusEta",          "#eta_{l-}",                             50,         -2.5,         +2.5);
  histogramLepMinusPhi_     = book1D(dir, "lMinusPhi",          "#phi_{l-}",                             36, -TMath::Pi(), +TMath::Pi());

  histogramZllCandPt_       = book1D(dir, "ZllCandPt",          "P_{T}^{Z}",                             40,          0. ,         100.);
  histogramZllCandEta_      = book1D(dir, "ZllCandEta",         "#eta_{Z}",                              50,         -2.5,         +2.5);
  histogramZllCandPhi_      = book1D(dir, "ZllCandPhi",         "#phi_{Z}",                              36, -TMath::Pi(), +TMath::Pi());
  histogramZllCandMass_     = book1D(dir, "ZllCandMass",        "M(l+ l-)",                              60,         60. ,         120.);
  
  histogramMEt_             = book1D(dir, "met",                "E_{T}^{miss}",                          50,          0.0,        100.0);
  histogramMEtProjParlZ_    = book1D(dir, "metProjParlZ",       "E_{T}^{miss} Proj. parallel Z",         50,        -50.0,        +50.0);
  histogramMEtProjPerpZ_    = book1D(dir, "metProjPerpZ",       "E_{T}^{miss} Proj. perp. Z",            50,        -50.0,        +50.0);

  histogramVtxMultiplicity_ = book1D(dir, "numVertices",        "Num. Vertices",                         20,         -0.5,         19.5);
  histogramRho_             = book1D(dir, "rho",                "#rho_{neutral}",                        50,          0. ,          20.);
}

void ZllRecoilCorrectionHistManager::fillHistograms(
       const reco::CompositeCandidate& ZllCand, const pat::MET& met, size_t vtxMultiplicity, double rho, double evtWeight)
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
  
  histogramMEt_->Fill(met.pt(), evtWeight);

  if ( ZllCand.pt() > 0. ) {
    double qT = ZllCand.pt();
    double qX = ZllCand.px();
    double qY = ZllCand.py();
    
    double metPx = met.px();
    double metPy = met.py();

    double metProjParlZ = (qX*metPx + qX*metPy)/qT;
    double metProjPerpZ = (qX*metPy - qY*metPx)/qT;
    
    histogramMEtProjParlZ_->Fill(metProjParlZ, evtWeight);
    histogramMEtProjPerpZ_->Fill(metProjPerpZ, evtWeight);
  }

  histogramVtxMultiplicity_->Fill(vtxMultiplicity, evtWeight);
  histogramRho_->Fill(rho, evtWeight);
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
