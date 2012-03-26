
/** \class NSVfitTauToLepBuilder
 *
 * Auxiliary class reconstructing tau --> e/mu decays and
 * building NSVfitTauToLepHypothesis objects;
 * used by NSVfit algorithm
 *
 * \author Evan Friis, Christian Veelken; UC Davis
 *
 * \version $Revision: 1.18 $
 *
 * $Id: NSVfitTauToLepBuilder.cc,v 1.18 2011/07/22 15:01:55 veelken Exp $
 *
 */

#include "FWCore/Utilities/interface/Exception.h"

#include "DataFormats/TauReco/interface/PFTauDecayMode.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitTauDecayBuilder.h"
#include "TauAnalysis/CandidateTools/interface/NSVfitSingleParticleTrackExtractor.h"
#include "TauAnalysis/CandidateTools/interface/NSVfitParameter.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitTauToDaughtersHypothesisBaseT1T2.h"

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
    if ( this->verbosity_ ) std::cout << "<NSVfitTauToLepBuilder::build>:" << std::endl;

    inputParticleMap::const_iterator particlePtr = inputParticles.find(prodParticleLabel_);
    assert(particlePtr != inputParticles.end());

    NSVfitTauToDaughtersHypothesisBaseT1T2<NSVfitTauDecayHypothesis, T>* hypothesis = 
      new NSVfitTauToDaughtersHypothesisBaseT1T2<NSVfitTauDecayHypothesis, T>(particlePtr->second, prodParticleLabel_, barcodeCounter_);
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
    if ( !objPtr ) 
      throw cms::Exception("NSVfitTauToLepBuilder")
	<< "Failed to extract tracks. Please check if correct Builder plugin type is used !!\n";
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

