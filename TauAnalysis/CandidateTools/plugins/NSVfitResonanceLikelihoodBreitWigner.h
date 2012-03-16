#ifndef TauAnalysis_CandidateTools_plugins_NSVfitResonanceLikelihoodBreitWigner_h
#define TauAnalysis_CandidateTools_plugins_NSVfitResonanceLikelihoodBreitWigner_h

/** \class NSVfitResonanceLikelihoodBreitWigner
 *
 * Compute likelihood for reconstructed mass of resonance 
 * to be compatible with nominal mass
 *
 * \author Christian Veelken; LLR
 *
 */

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitResonanceLikelihood.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitResonanceHypothesis.h"

class NSVfitResonanceLikelihoodBreitWigner : public NSVfitResonanceLikelihood
{
 public:

  NSVfitResonanceLikelihoodBreitWigner(const edm::ParameterSet&);
  ~NSVfitResonanceLikelihoodBreitWigner();

  void beginJob(NSVfitAlgorithmBase*) const {}

  void beginCandidate(const NSVfitResonanceHypothesis*) const {};

  double operator()(const NSVfitResonanceHypothesis*, int) const;

 private:

  double resonance_mass_;
  double resonance_mass2_;
  double resonance_width_;
  double resonance_width2_;
  
  double power_;
};


#endif /* end of include guard: TauAnalysis_CandidateTools_plugins_NSVfitResonanceLikelihoodBreitWigner_h */
