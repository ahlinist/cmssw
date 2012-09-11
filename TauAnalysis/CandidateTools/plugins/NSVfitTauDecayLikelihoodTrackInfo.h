#ifndef TauAnalysis_CandidateTools_NSVfitTauDecayLikelihoodTrackInfo_h
#define TauAnalysis_CandidateTools_NSVfitTauDecayLikelihoodTrackInfo_h

/** \class NSVfitTauDecayLikelihoodTrackInfo
 *
 * Plugin to compute likelihood for tracks of tau lepton decay products
 * to be compatible with originating from hypothetic secondary (tau lepton decay) vertex
 *
 * \author Evan Friis, Christian Veelken; UC Davis
 *
 * \version $Revision: 1.5 $
 *
 * $Id: NSVfitTauDecayLikelihoodTrackInfo.h,v 1.5 2012/09/01 08:45:45 veelken Exp $
 *
 */

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitSingleParticleLikelihood.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitSingleParticleHypothesisBase.h"

#include "TauAnalysis/CandidateTools/interface/SVfitLegTrackExtractor.h"
#include "TauAnalysis/CandidateTools/interface/SVfitTrackExtrapolation.h"

class NSVfitTauDecayLikelihoodTrackInfo : public NSVfitSingleParticleLikelihood
{
 public:
  NSVfitTauDecayLikelihoodTrackInfo(const edm::ParameterSet&);
  ~NSVfitTauDecayLikelihoodTrackInfo();

  void beginJob(NSVfitAlgorithmBase*);
  void beginCandidate(const NSVfitSingleParticleHypothesis*);

  double operator()(const NSVfitSingleParticleHypothesis*, int) const;

 private:
  bool useLifetimeConstraint_;
  bool useBackwardsPenaltyTerm_;

  bool ignore3Prongs_;
  bool ignore1Prongs_;

  double sfProdVertexCov_;
  double sfDecayVertexCov_;

  NSVfitAlgorithmBase* algorithm_;

  double decayDistance_lab_shift_lowerLimit_;
  double decayDistance_lab_shift_upperLimit_;
};

#endif
