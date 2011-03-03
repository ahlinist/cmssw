#ifndef TauAnalysis_CandidateTools_NSVfitEventLikelihoodMEt_h
#define TauAnalysis_CandidateTools_NSVfitEventLikelihoodMEt_h

/** \class NSVfitEventLikelihoodMEt
 *
 * Plugin for computing likelihood for neutrinos produced in tau lepton decays
 * to match missing transverse momentum reconstructed in the event
 *
 * \author Evan Friis, Christian Veelken; UC Davis
 *
 * \version $Revision: 1.1 $
 *
 * $Id: NSVfitEventLikelihoodMEt.h,v 1.1 2011/02/27 16:45:16 veelken Exp $
 *
 */

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/Handle.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitEventLikelihood.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitEventHypothesis.h"

#include <TFormula.h>

class NSVfitEventLikelihoodMEt : public NSVfitEventLikelihood
{
 public:
  NSVfitEventLikelihoodMEt(const edm::ParameterSet&);
  ~NSVfitEventLikelihoodMEt();

  void beginJob(NSVfitAlgorithmBase*) const;
  void beginCandidate(const NSVfitEventHypothesis*) const;

  double operator()(const NSVfitEventHypothesis*) const;

 private:
  TFormula* parSigma_;
  TFormula* parBias_;

  TFormula* perpSigma_;
  TFormula* perpBias_;

  mutable double qX_;
  mutable double qY_;
  mutable double qT_;
};

#endif
