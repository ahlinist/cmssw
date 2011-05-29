#ifndef TauAnalysis_CandidateTools_NSVfitTauToLepLikelihoodPhaseSpace_h
#define TauAnalysis_CandidateTools_NSVfitTauToLepLikelihoodPhaseSpace_h

/** \class NSVfitTauToLeptonLikelihoodPhaseSpace
 *
 * Plugin to compute likelihood for electron (muon) to be compatible 
 * with tau --> e nu nu (tau --> mu nu nu) three-body decay,
 * assuming constant matrix element, so that energy and angular distribution 
 * of decay products are solely determined by phase-space
 * 
 * \author Evan Friis, Christian Veelken; UC Davis
 *
 * \version $Revision: 1.4 $
 *
 * $Id: NSVfitTauToLepLikelihoodPhaseSpace.h,v 1.4 2011/03/06 11:31:11 veelken Exp $
 *
 */

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitSingleParticleLikelihood.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitSingleParticleHypothesisBase.h"

template <typename T>
class NSVfitTauToLepLikelihoodPhaseSpace : public NSVfitSingleParticleLikelihood
{
 public:
  NSVfitTauToLepLikelihoodPhaseSpace(const edm::ParameterSet&);
  ~NSVfitTauToLepLikelihoodPhaseSpace();

  void beginJob(NSVfitAlgorithmBase*);

  double operator()(const NSVfitSingleParticleHypothesis*) const;
};

#endif
