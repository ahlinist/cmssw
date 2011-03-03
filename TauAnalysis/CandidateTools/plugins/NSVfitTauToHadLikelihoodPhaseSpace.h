#ifndef TauAnalysis_CandidateTools_NSVfitTauToHadLikelihoodPhaseSpace_h
#define TauAnalysis_CandidateTools_NSVfitTauToHadLikelihoodPhaseSpace_h

/** \class NSVfitTauToLeptonLikelihoodPhaseSpace
 *
 * Plugin to compute likelihood for system of hadrons to be compatible 
 * with tau --> tau-jet + nu two-body decay,
 * assuming constant matrix element, so that energy and angular distribution 
 * of decay products are solely determined by phase-space
 * 
 * \author Evan Friis, Christian Veelken; UC Davis
 *
 * \version $Revision: 1.2 $
 *
 * $Id: NSVfitTauToHadLikelihoodPhaseSpace.h,v 1.2 2011/02/27 17:00:35 veelken Exp $
 *
 */

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitSingleParticleLikelihood.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitSingleParticleHypothesisBase.h"

class NSVfitTauToHadLikelihoodPhaseSpace : public NSVfitSingleParticleLikelihood
{
 public:
  NSVfitTauToHadLikelihoodPhaseSpace(const edm::ParameterSet&);
  ~NSVfitTauToHadLikelihoodPhaseSpace();

  void beginJob(NSVfitAlgorithmBase*) const;

  double operator()(const NSVfitSingleParticleHypothesisBase*) const;
};

#endif
