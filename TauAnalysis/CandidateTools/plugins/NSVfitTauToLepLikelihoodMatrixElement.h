#ifndef TauAnalysis_CandidateTools_NSVfitTauToLepLikelihoodMatrixElement_h
#define TauAnalysis_CandidateTools_NSVfitTauToLepLikelihoodMatrixElement_h

/** \class NSVfitTauToLepLikelihoodMatrixElement
 *
 * Plugin for computing likelihood for tau lepton decay 
 *  tau- --> e- nu nu (tau- --> mu- nu nu)
 * to be compatible with matrix element of V-A electroweak decay
 * (ignoring tau lepton polarization effects)
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.1 $
 *
 * $Id: NSVfitTauToLepLikelihoodMatrixElement.h,v 1.1 2012/03/08 10:35:45 veelken Exp $
 *
 */

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitSingleParticleLikelihood.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitSingleParticleHypothesisBase.h"

template <typename T>
class NSVfitTauToLepLikelihoodMatrixElement : public NSVfitSingleParticleLikelihood
{
 public:
  NSVfitTauToLepLikelihoodMatrixElement(const edm::ParameterSet&);
  ~NSVfitTauToLepLikelihoodMatrixElement();
  
  void beginJob(NSVfitAlgorithmBase*);
  
  double operator()(const NSVfitSingleParticleHypothesis*) const;

 private:
  bool applySinThetaFactor_;
};

#endif
