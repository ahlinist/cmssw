#ifndef AnalysisDataFormats_TauAnalysis_NSVfitSingleNeutrinoHypothesis_h
#define AnalysisDataFormats_TauAnalysis_NSVfitSingleNeutrinoHypothesis_h

/*
 * NSVfitSingleNeutrinoHypothesis
 *
 * Class representing neutrinos not originating from tau lepton decays
 * (e.g. produced in decays of W bosons)
 *
 * NOTE: this class is used to make persistent solutions of NSVfitAlgorithmByLikelihoodMaximization;
 *       solutions of NSVfitAlgorithmByIntegration are of type NSVfitSingleNeutrinoHypothesisBase
 *
 * Authors: Evan K. Friis, Christian Veelken, UC Davis
 *
 */

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Common/interface/Ptr.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitSingleNeutrinoHypothesisBaseT.h"
#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitSingleParticleHypothesisBase.h"

class NSVfitSingleNeutrinoHypothesis : public NSVfitSingleNeutrinoHypothesisBaseT<NSVfitSingleParticleHypothesis>
{
 public:

  NSVfitSingleNeutrinoHypothesis() {}
  NSVfitSingleNeutrinoHypothesis(const std::string& name, int barcode)
    : NSVfitSingleNeutrinoHypothesisBaseT<NSVfitSingleParticleHypothesis>(name, barcode)
  {
    p4_ = reco::Candidate::LorentzVector(0.,0.,0.,0.);
  }
  NSVfitSingleNeutrinoHypothesis(const NSVfitSingleNeutrinoHypothesis& bluePrint)
    : NSVfitSingleNeutrinoHypothesisBaseT<NSVfitSingleParticleHypothesis>(bluePrint)
  {}

  ~NSVfitSingleNeutrinoHypothesis() {}

  virtual NSVfitSingleNeutrinoHypothesis& operator=(const NSVfitSingleNeutrinoHypothesis& bluePrint)
  {
    NSVfitSingleNeutrinoHypothesisBaseT<NSVfitSingleParticleHypothesis>::operator=(bluePrint);
    return (*this);
  }

  NSVfitSingleNeutrinoHypothesis* clone() const { return new NSVfitSingleNeutrinoHypothesis(*this); }

  friend class NSVfitResonanceBuilderW;
};

#endif /* end of include guard: AnalysisDataFormats_TauAnalysis_NSVfitSingleNeutrinoHypothesis_h */
