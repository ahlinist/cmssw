#ifndef AnalysisDataFormats_TauAnalysis_NSVfitStableSingleParticleHypothesisBaseT_h
#define AnalysisDataFormats_TauAnalysis_NSVfitStableSingleParticleHypothesisBaseT_h

/*
 * NSVfitStableSingleParticleHypothesisBaseT
 *
 * Class representing electrons or muons not originating from tau lepton decays
 * (e.g. produced in decays of W bosons)
 *
 * NOTE: this base-class is used to make persistent solutions of NSVfitAlgorithmByIntegration;
 *       solutions of NSVfitAlgorithmByLikelihoodMaximization are of type NSVfitStableSingleParticleHypothesisBaseT
 *
 * Authors: Christian Veelken, LLR
 *
 */

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Common/interface/Ptr.h"

template <typename T>
class NSVfitStableSingleParticleHypothesisBaseT : public T
{
 public:

  NSVfitStableSingleParticleHypothesisBaseT() {}
  NSVfitStableSingleParticleHypothesisBaseT(const edm::Ptr<reco::Candidate>& particle, const std::string& name, int barcode)
    : T(particle, name, barcode) 
  {}
  NSVfitStableSingleParticleHypothesisBaseT(const NSVfitStableSingleParticleHypothesisBaseT& bluePrint)
    : T(bluePrint)
  {}

  ~NSVfitStableSingleParticleHypothesisBaseT() {}

  NSVfitStableSingleParticleHypothesisBaseT& operator=(const NSVfitStableSingleParticleHypothesisBaseT& bluePrint)
  {
    T::operator=(bluePrint);
    return (*this);
  }
};

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitSingleParticleHypothesisBase.h"

typedef NSVfitStableSingleParticleHypothesisBaseT<NSVfitSingleParticleHypothesisBase> NSVfitStableSingleParticleHypothesisBase;

#endif /* end of include guard: AnalysisDataFormats_TauAnalysis_NSVfitStableSingleParticleHypothesisBaseT_h */
