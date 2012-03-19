#ifndef AnalysisDataFormats_TauAnalysis_NSVfitSingleNeutrinoHypothesisBaseT_h
#define AnalysisDataFormats_TauAnalysis_NSVfitSingleNeutrinoHypothesisBaseT_h

/*
 * NSVfitSingleNeutrinoHypothesisBaseT
 *
 * Class representing neutrinos not originating from tau lepton decays
 * (e.g. produced in decays of W bosons)
 *
 * NOTE: this base-class is used to make persistent solutions of NSVfitAlgorithmByIntegration;
 *       solutions of NSVfitAlgorithmByLikelihoodMaximization are of type NSVfitTauDecayHypothesis
 *
 * Authors: Evan K. Friis, Christian Veelken, UC Davis
 *
 */

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Common/interface/Ptr.h"

template <typename T>
class NSVfitSingleNeutrinoHypothesisBaseT : public T
{
 public:

  NSVfitSingleNeutrinoHypothesisBaseT() {}
  NSVfitSingleNeutrinoHypothesisBaseT(const std::string& name, int barcode)
    : T(name, barcode) 
  {}
  NSVfitSingleNeutrinoHypothesisBaseT(const NSVfitSingleNeutrinoHypothesisBaseT& bluePrint)
    : T(bluePrint)
  {}

  ~NSVfitSingleNeutrinoHypothesisBaseT() {}

  NSVfitSingleNeutrinoHypothesisBaseT& operator=(const NSVfitSingleNeutrinoHypothesisBaseT& bluePrint)
  {
    T::operator=(bluePrint);
    return (*this);
  }

  NSVfitSingleNeutrinoHypothesisBaseT* clone() const { return new NSVfitSingleNeutrinoHypothesisBaseT(*this); }
  NSVfitSingleParticleHypothesisBase* reduceToBase() const { return this->clone(); }
};

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitSingleParticleHypothesisBase.h"

typedef NSVfitSingleNeutrinoHypothesisBaseT<NSVfitSingleParticleHypothesisBase> NSVfitSingleNeutrinoHypothesisBase;

#endif /* end of include guard: AnalysisDataFormats_TauAnalysis_NSVfitSingleNeutrinoHypothesisBaseT_h */
