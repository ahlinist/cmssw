#ifndef TauAnalysis_CandidateTools_plugins_NSVfitResonanceLikelihoodPrior_h
#define TauAnalysis_CandidateTools_plugins_NSVfitResonanceLikelihoodPrior_h

/** \class NSVfitResonanceLikelihoodPrior
 *
 * Correct tau decay likelihood for effect of visible Pt cuts
 *
 * \author Christian Veelken; LLR
 *
 */

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitResonanceLikelihood.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitResonanceHypothesis.h"

#include <TFormula.h>

#include <vector>

class NSVfitResonanceLikelihoodPrior : public NSVfitResonanceLikelihood
{
 public:

  NSVfitResonanceLikelihoodPrior(const edm::ParameterSet&);
  ~NSVfitResonanceLikelihoodPrior();

  void beginJob(NSVfitAlgorithmBase*) const {}

  void beginCandidate(const NSVfitResonanceHypothesis*) const {};

  double operator()(const NSVfitResonanceHypothesis*, int polHandedness) const;

 private:

  TFormula* function_;
  double xMin_;
  double xMax_;
  int numParameter_;
  std::vector<double> parameter_;

  double power_;
};


#endif /* end of include guard: TauAnalysis_CandidateTools_plugins_NSVfitResonanceLikelihoodPrior_h */
