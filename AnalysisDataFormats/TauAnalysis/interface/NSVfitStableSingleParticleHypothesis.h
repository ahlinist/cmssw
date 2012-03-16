#ifndef AnalysisDataFormats_TauAnalysis_NSVfitStableSingleParticleHypothesis_h
#define AnalysisDataFormats_TauAnalysis_NSVfitStableSingleParticleHypothesis_h

/*
 * NSVfitStableSingleParticleHypothesis
 *
 * Class representing electrons or muons not originating from tau lepton decays
 * (e.g. produced in decays of W bosons)
 *
 * NOTE: this class is used to make persistent solutions of NSVfitAlgorithmByLikelihoodMaximization;
 *       solutions of NSVfitAlgorithmByIntegration are of type NSVfitStableSingleParticleHypothesisBase
 *
 * Authors: Christian Veelken, LLR
 *
 */

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Common/interface/Ptr.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitStableSingleParticleHypothesisBaseT.h"
#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitSingleParticleHypothesisBase.h"

class NSVfitStableSingleParticleHypothesis : public NSVfitStableSingleParticleHypothesisBaseT<NSVfitSingleParticleHypothesis>
{
 public:

  NSVfitStableSingleParticleHypothesis() {}
  NSVfitStableSingleParticleHypothesis(const edm::Ptr<reco::Candidate>& particle, const std::string& name, int barcode)
    : NSVfitStableSingleParticleHypothesisBaseT<NSVfitSingleParticleHypothesis>(particle, name, barcode)
  {
    p4_ = particle->p4();
  }
  NSVfitStableSingleParticleHypothesis(const NSVfitStableSingleParticleHypothesis& bluePrint)
    : NSVfitStableSingleParticleHypothesisBaseT<NSVfitSingleParticleHypothesis>(bluePrint)
  {}
  
  ~NSVfitStableSingleParticleHypothesis() {}

  virtual NSVfitStableSingleParticleHypothesis& operator=(const NSVfitStableSingleParticleHypothesis& bluePrint)
  {
    NSVfitStableSingleParticleHypothesisBaseT<NSVfitSingleParticleHypothesis>::operator=(bluePrint);
    return (*this);
  }

  NSVfitStableSingleParticleHypothesis* clone() const { return new NSVfitStableSingleParticleHypothesis(*this); }
  NSVfitSingleParticleHypothesisBase* reduceToBase() const { return this->clone(); }

  /// unit-vector in lepton flight direction
  const reco::Candidate::Vector& p3_unit() const { return p3_unit_; }

  friend class NSVfitResonanceBuilderW;

 protected:

  /// unit-vector in lepton flight direction
  reco::Candidate::Vector p3_unit_;
};

#endif /* end of include guard: AnalysisDataFormats_TauAnalysis_NSVfitStableSingleParticleHypothesis_h */
