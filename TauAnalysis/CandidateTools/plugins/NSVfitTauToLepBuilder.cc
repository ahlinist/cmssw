#include "TauAnalysis/CandidateTools/interface/NSVfitTauDecayBuilderBase.h"
#include "TauAnalysis/CandidateTools/interface/SVfitLegTrackExtractor.h"
#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitTauToLepHypothesis.h"
#include "DataFormats/TauReco/interface/PFTauDecayMode.h"

template <typename T>
int getDecayMode(const T*)
{
  assert(0); // force template specializations for pat::Electrons/pat::Muons to be used
}

template <>
int getDecayMode<pat::Electron>(const pat::Electron*)
{
  return reco::PFTauDecayMode::tauDecaysElectron;
}

template <>
int getDecayMode<pat::Muon>(const pat::Muon*)
{
  return reco::PFTauDecayMode::tauDecayMuon;
}

template<typename T>
class NSVfitTauToLepBuilder : public NSVfitTauDecayBuilderBase 
{
 public:
  NSVfitTauToLepBuilder(const edm::ParameterSet& cfg)
    : NSVfitTauDecayBuilderBase(cfg)
  {}

  NSVfitTauDecayHypothesis* buildSpecific(const edm::Ptr<reco::Candidate> particle, const std::string& label, int barcode) const 
  {
    NSVfitTauToLepHypothesis<T>* hypothesis = new NSVfitTauToLepHypothesis<T>(particle, label, barcode);
    const T* objPtr = dynamic_cast<const T*>(&(*particle));
    assert(objPtr);
    hypothesis->decayMode_ = getDecayMode(&(*objPtr));
    return hypothesis;
  }

  // The two neutrion system in a leptonic decay can have mass.
  bool nuSystemIsMassless() const { return false; }
  
  virtual int getDecayMode(const reco::Candidate* candidate) const 
  {
    const T* objPtr = dynamic_cast<const T*>(candidate);
    assert(objPtr);
    return getDecayMode(&(*objPtr));
  }

  virtual std::vector<reco::TrackBaseRef> extractTracks(const reco::Candidate* candidate) const 
  {
    const T* objPtr = dynamic_cast<const T*>(candidate);
    assert(objPtr);
    return trackExtractor_(*objPtr);
  }
  
  virtual void beginJobSpecific(NSVfitAlgorithmBase* algorithm) 
  {
    // No need to request additional parameters. 
  }

  virtual void applyFitParameterSpecific(NSVfitTauDecayHypothesis* hypothesis, double* param) const 
  {
    // Nothing to do for leptonic case.
  }

 private:
  SVfitLegTrackExtractor<T> trackExtractor_;
};

typedef NSVfitTauToLepBuilder<pat::Electron> NSVfitTauToElecBuilder;
typedef NSVfitTauToLepBuilder<pat::Muon> NSVfitTauToMuBuilder;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(NSVfitSingleParticleBuilderPluginFactory, NSVfitTauToElecBuilder, "NSVfitTauToElecBuilder");
DEFINE_EDM_PLUGIN(NSVfitSingleParticleBuilderPluginFactory, NSVfitTauToMuBuilder, "NSVfitTauToMuBuilder");

