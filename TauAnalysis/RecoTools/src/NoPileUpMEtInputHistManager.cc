#include "TauAnalysis/RecoTools/interface/NoPileUpMEtInputHistManager.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Math/interface/deltaR.h"

#include "TauAnalysis/CandidateTools/interface/candidateAuxFunctions.h"
#include "TauAnalysis/CandidateTools/interface/neuralMtautauAuxFunctions.h"

#include <TMath.h>
#include <TVectorD.h>

NoPileUpMEtInputHistManager::NoPileUpMEtInputHistManager(const edm::ParameterSet& cfg)
{
  src_ = cfg.getParameter<edm::InputTag>("src");
  inputsToPlot_ = cfg.getParameter<vstring>("inputsToPlot");
}

NoPileUpMEtInputHistManager::~NoPileUpMEtInputHistManager()
{
  for ( std::vector<histogramEntryType*>::iterator it = histogramEntries_.begin();
	it != histogramEntries_.end(); ++it ) {
    delete (*it);
  }
}

void NoPileUpMEtInputHistManager::bookHistograms(TFileDirectory& dir)
{
  for ( vstring::const_iterator inputToPlot = inputsToPlot_.begin();
	inputToPlot != inputsToPlot_.end(); ++inputToPlot ) {
    TFileDirectory subdir = dir.mkdir(*inputToPlot);
    histogramEntryType* histogramEntry = new histogramEntryType(this, subdir, edm::InputTag(src_.label(), *inputToPlot));
    histogramEntries_.push_back(histogramEntry);
  }
}

void NoPileUpMEtInputHistManager::fillHistograms(
       const reco::CompositeCandidate& ZllCand, const std::vector<pat::Muon>& muons, 
       const std::vector<pat::Jet>& jets, 
       const fwlite::Event& evt, double evtWeight)
{  
  int numBTagJetsCorrPtGt30 = 0;
  const std::string bTagDiscr = "trackCountingHighEffBJetTags";
  double bTagDiscr_cut = 3.3;
    
  for ( std::vector<pat::Jet>::const_iterator jet = jets.begin();
	jet != jets.end(); ++jet ) {
    bool isMuonOverlap = false;
    for ( std::vector<pat::Muon>::const_iterator muon = muons.begin();
	  muon != muons.end(); ++muon ) {
      if ( deltaR(jet->p4(), muon->p4()) < 0.5 ) isMuonOverlap = true;
    }
    if ( isMuonOverlap ) continue;
    
    reco::Candidate::LorentzVector rawJetP4 = jet->correctedP4("Uncorrected");

    reco::Candidate::LorentzVector corrJetP4 = jet->p4();
    // CV: require pseudo-rapidity of "raw" and corrected jet to match,
    //     in order to avoid problem with "unphysical" corrected jets of negative energy
    //     for which the energy becomes positive and the sign of eta "flips"
    //    (cf. https://hypernews.cern.ch/HyperNews/CMS/get/JetMET/1311/1.html)
    if ( TMath::Abs(rawJetP4.eta() - corrJetP4.eta()) < 0.1 ) {    
      if ( jet->bDiscriminator(bTagDiscr.data()) > bTagDiscr_cut ) {
	if ( corrJetP4.pt() > 30. ) ++numBTagJetsCorrPtGt30;
      }
    }
  }


  if ( ZllCand.mass() > 76. && ZllCand.mass() < 106. && numBTagJetsCorrPtGt30 == 0 ) { // cuts to suppress TTbar background
    for ( std::vector<histogramEntryType*>::iterator histogramEntry = histogramEntries_.begin();
	  histogramEntry != histogramEntries_.end(); ++histogramEntry ) {
      (*histogramEntry)->fillHistograms(ZllCand.p4(), evt, evtWeight);
    }
  }
}

void NoPileUpMEtInputHistManager::scaleHistograms(double factor)
{
  for ( std::vector<TH1*>::iterator histogram = histograms_.begin();
	histogram != histograms_.end(); ++histogram ) {
    if ( !(*histogram)->GetSumw2N() ) (*histogram)->Sumw2(); // CV: compute "proper" errors before scaling histogram
    (*histogram)->Scale(factor);
  }
}

TH1* NoPileUpMEtInputHistManager::book1D(TFileDirectory& dir, const std::string& distribution, const std::string& title, int numBins, double min, double max)
{
  TH1* retVal = dir.make<TH1D>(distribution.data(), title.data(), numBins, min, max);
  histograms_.push_back(retVal);
  return retVal;
}
