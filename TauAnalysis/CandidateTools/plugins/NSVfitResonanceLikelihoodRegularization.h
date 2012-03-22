#ifndef TauAnalysis_CandidateTools_plugins_NSVfitResonanceLikelihoodMassPenalty_h
#define TauAnalysis_CandidateTools_plugins_NSVfitResonanceLikelihoodRegularization_h

/** \class NSVfitResonanceLikelihoodRegularization
 *
 * Adds a penalty term for high masses.  
 * The return value of operator() is configurable via python.
 *
 * Options used in the past are:
 *  o TMath::Log(mass)
 *  o TMath::Log(TMath::Max([0], [1]*([2] - TMath::Erf((x - [3])*[4]))))
 *    with p0 = 5.00e-3, p1 = 4.21e-2, p2 = 2.52e-2, p3 = 4.40e+1, p4 = -6.90e-3 
 *   (efficiency of gg --> Higgs --> mu + tau_had channel in 2010 analysis)
 *  o TMath::Log(TMath::Max([0], [1]*([2] - TMath::Erf((x - [3])*[4]))))
 *    with p0 = 2.50e-3, p1 = 2.49e-2, p2 = 7.78e-2, p3 = 5.63e+1, p4 = -7.53e-3 
 *   (efficiency of gg --> Higgs --> e + tau_had channel in 2010 analysis)
 #  o TMath::Log(pt)
 *
 * \author Evan Friis, Christian Veelken; UC Davis
 *
 */

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitResonanceLikelihood.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitResonanceHypothesis.h"

#include "TFormula.h"

class NSVfitResonanceLikelihoodRegularization : public NSVfitResonanceLikelihood
{
 public:

  NSVfitResonanceLikelihoodRegularization(const edm::ParameterSet&);
  ~NSVfitResonanceLikelihoodRegularization();

  void beginJob(NSVfitAlgorithmBase*) const {}

  void beginCandidate(const NSVfitResonanceHypothesis*) const {};

  double operator()(const NSVfitResonanceHypothesis*, int) const;

 private:

  TFormula* nll_formula_;
  
  double power_;
};


#endif /* end of include guard: TauAnalysis_CandidateTools_plugins_NSVfitResonanceLikelihoodRegularization2_h */
