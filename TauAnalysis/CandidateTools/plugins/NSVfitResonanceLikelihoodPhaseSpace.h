#ifndef TauAnalysis_CandidateTools_plugins_NSVfitResonanceLikelihoodPhaseSpace_h
#define TauAnalysis_CandidateTools_plugins_NSVfitResonanceLikelihoodPhaseSpace_h

/** \class NSVfitResonanceLikelihoodPhaseSpace
 *
 * Compute likelihood for resonance to decay into pairs of tau leptons,
 * assuming constant matrix element, so that energy and angular distribution 
 * of decay products are solely determined by phase-space
 *
 * \author Christian Veelken; LLR
 *
 */

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitResonanceLikelihood.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitResonanceHypothesis.h"

class NSVfitResonanceLikelihoodPhaseSpace : public NSVfitResonanceLikelihood
{
 public:

  NSVfitResonanceLikelihoodPhaseSpace(const edm::ParameterSet&);
  ~NSVfitResonanceLikelihoodPhaseSpace();

  void beginJob(NSVfitAlgorithmBase*) const {}

  void beginCandidate(const NSVfitResonanceHypothesis*) const {};

  double operator()(const NSVfitResonanceHypothesis*, int) const;

 private:

  bool applySinThetaFactor_; 

  double power_;
};


#endif /* end of include guard: TauAnalysis_CandidateTools_plugins_NSVfitResonanceLikelihoodPhaseSpace_h */
