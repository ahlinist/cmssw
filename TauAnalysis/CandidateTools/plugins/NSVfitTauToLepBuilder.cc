#include "TauAnalysis/CandidateTools/interface/NSVfitTauDecayBuilderBase.h"
#include "TauAnalysis/CandidateTools/interface/NSVfitSingleParticleTrackExtractor.h"
#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitTauToLepHypothesis.h"
#include "TauAnalysis/CandidateTools/interface/NSVfitParameter.h"
#include "DataFormats/TauReco/interface/PFTauDecayMode.h"

/** \class NSVfitTauToLepBuilder
 *
 * Auxiliary class reconstructing tau --> e/mu decays and
 * building NSVfitTauToLepHypothesis objects;
 * used by NSVfit algorithm
 *
 * \author Evan Friis, Christian Veelken; UC Davis
 *
 * \version $Revision: 1.13 $
 *
 * $Id: NSVfitTauToLepBuilder.cc,v 1.13 2011/03/31 16:31:33 veelken Exp $
 *
 */

template<typename T>
class NSVfitTauToLepBuilder : public NSVfitTauDecayBuilderBase 
{
 public:
  NSVfitTauToLepBuilder(const edm::ParameterSet& cfg)
    : NSVfitTauDecayBuilderBase(cfg)
  {}

  void beginJob(NSVfitAlgorithmBase* algorithm) 
  {
    NSVfitTauDecayBuilderBase::beginJob(algorithm);

    idxFitParameter_nuInvMass_ = getFitParameterIdx(algorithm, prodParticleLabel_, nSVfit_namespace::kTau_nuInvMass);

    //if ( verbosity_ ) print(std::cout);
  }

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

  virtual std::vector<const reco::Track*> extractTracks(const reco::Candidate* candidate) const 
  {
    const T* objPtr = dynamic_cast<const T*>(candidate);
    assert(objPtr);
    return trackExtractor_(*objPtr);
  }

 private:
  NSVfitSingleParticleTrackExtractor<T> trackExtractor_;
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

