#include "TauAnalysis/CandidateTools/interface/NSVfitTauDecayBuilder.h"
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
 * \version $Revision: 1.15 $
 *
 * $Id: NSVfitTauToLepBuilder.cc,v 1.15 2011/05/29 17:58:22 veelken Exp $
 *
 */

template<typename T>
class NSVfitTauToLepBuilder : public NSVfitTauDecayBuilder 
{
 public:
  NSVfitTauToLepBuilder(const edm::ParameterSet& cfg)
    : NSVfitTauDecayBuilder(cfg)
  {}

  void beginJob(NSVfitAlgorithmBase* algorithm) 
  {
    NSVfitTauDecayBuilder::beginJob(algorithm);

    idxFitParameter_nuInvMass_ = getFitParameterIdx(algorithm, prodParticleLabel_, nSVfit_namespace::kTau_nuInvMass);

    //if ( verbosity_ ) print(std::cout);
  }

  NSVfitSingleParticleHypothesis* build(const NSVfitTauDecayBuilder::inputParticleMap& inputParticles) const 
  {
    inputParticleMap::const_iterator particlePtr = inputParticles.find(prodParticleLabel_);
    assert(particlePtr != inputParticles.end());

    NSVfitTauToLepHypothesis<T>* hypothesis = 
      new NSVfitTauToLepHypothesis<T>(particlePtr->second, prodParticleLabel_, barcodeCounter_);
    ++barcodeCounter_;

    NSVfitTauDecayBuilder::initialize(hypothesis, particlePtr->second.get());

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

