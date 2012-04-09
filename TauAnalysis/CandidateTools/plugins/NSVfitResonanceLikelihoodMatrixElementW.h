#ifndef TauAnalysis_CandidateTools_plugins_NSVfitResonanceLikelihoodMatrixElementW_h
#define TauAnalysis_CandidateTools_plugins_NSVfitResonanceLikelihoodMatrixElementW_h

/** \class NSVfitResonanceLikelihoodMatrixElementW
 *
 * Compute likelihood for W boson to decay into charged lepton + neutrino,
 * to be compatible with matrix element of V-A electroweak decay
 * (taking polarization of W boson into account)
 *
 * \author Christian Veelken; LLR
 *
 */

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitResonanceLikelihood.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitResonanceHypothesis.h"

class NSVfitResonanceLikelihoodMatrixElementW : public NSVfitResonanceLikelihood
{
 public:

  NSVfitResonanceLikelihoodMatrixElementW(const edm::ParameterSet&);
  ~NSVfitResonanceLikelihoodMatrixElementW();

  void beginJob(NSVfitAlgorithmBase*) const {}

  void beginCandidate(const NSVfitResonanceHypothesis*) const {};

  double operator()(const NSVfitResonanceHypothesis*, int) const;

 private:

  bool applySinThetaFactor_; 

  double power_;
};


#endif /* end of include guard: TauAnalysis_CandidateTools_plugins_NSVfitResonanceLikelihoodMatrixElementW_h */
