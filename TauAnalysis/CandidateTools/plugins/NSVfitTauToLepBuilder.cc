#include "TauAnalysis/CandidateTools/interface/NSVfitTauDecayBuilderBase.h"
#include "TauAnalysis/CandidateTools/interface/SVfitLegTrackExtractor.h"
#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitTauToLepHypothesis.h"
#include "DataFormats/TauReco/interface/PFTauDecayMode.h"

template<typename T>
class NSVfitTauToLepBuilder : public NSVfitTauDecayBuilderBase 
{
 public:
  NSVfitTauToLepBuilder(const edm::ParameterSet& cfg)
    : NSVfitTauDecayBuilderBase(cfg)
  {}

  NSVfitSingleParticleHypothesisBase* build(const NSVfitTauDecayBuilderBase::inputParticleMap& inputParticles) const 
  {
    inputParticleMap::const_iterator particlePtr = inputParticles.find(prodParticleLabel_);
    assert(particlePtr != inputParticles.end());

    NSVfitTauToLepHypothesis<T>* hypothesis = new NSVfitTauToLepHypothesis<T>(particlePtr->second, prodParticleLabel_, barcodeCounter_);
    ++barcodeCounter_;

    NSVfitTauDecayBuilderBase::initialize(hypothesis, particlePtr->second.get());

    return hypothesis;
  }

  // The two neutrion system in a leptonic tau decay can have non-zero mass.
  bool nuSystemIsMassless() const { return false; }
  
  virtual int getDecayMode(const reco::Candidate* candidate) const 
  {
    assert(0); // force template specializations for pat::Electrons/pat::Muons to be used
  }

  virtual std::vector<reco::TrackBaseRef> extractTracks(const reco::Candidate* candidate) const 
  {
    const T* objPtr = dynamic_cast<const T*>(candidate);
    assert(objPtr);
    return trackExtractor_(*objPtr);
  }

 private:
  SVfitLegTrackExtractor<T> trackExtractor_;
};

template<>
int NSVfitTauToLepBuilder<pat::Electron>::getDecayMode(const reco::Candidate* candidate) const 
{
  return reco::PFTauDecayMode::tauDecaysElectron;
}

template <>
int NSVfitTauToLepBuilder<pat::Muon>::getDecayMode(const reco::Candidate* candidate) const
{
  return reco::PFTauDecayMode::tauDecayMuon;
}

typedef NSVfitTauToLepBuilder<pat::Electron> NSVfitTauToElecBuilder;
typedef NSVfitTauToLepBuilder<pat::Muon> NSVfitTauToMuBuilder;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(NSVfitSingleParticleBuilderPluginFactory, NSVfitTauToElecBuilder, "NSVfitTauToElecBuilder");
DEFINE_EDM_PLUGIN(NSVfitSingleParticleBuilderPluginFactory, NSVfitTauToMuBuilder, "NSVfitTauToMuBuilder");

