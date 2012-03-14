#ifndef TauAnalysis_CandidateTools_NSVfitTauDecayLikelihoodTrackInfo_h
#define TauAnalysis_CandidateTools_NSVfitTauDecayLikelihoodTrackInfo_h

/** \class NSVfitTauDecayLikelihoodTrackInfo
 *
 * Plugin to compute likelihood for tracks of tau lepton decay products
 * to be compatible with originating from hypothetic secondary (tau lepton decay) vertex
 *
 * \author Evan Friis, Christian Veelken; UC Davis
 *
 * \version $Revision: 1.2 $
 *
 * $Id: NSVfitTauDecayLikelihoodTrackInfo.h,v 1.2 2011/05/29 17:58:22 veelken Exp $
 *
 */

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"

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

  void beginEvent(const edm::Event&, const edm::EventSetup&);
  void beginCandidate(const NSVfitSingleParticleHypothesis*);

  double operator()(const NSVfitSingleParticleHypothesis*, int) const;

 private:
  const TransientTrackBuilder* trackBuilder_;

  mutable std::vector<reco::TransientTrack> selectedTracks_;

  unsigned minNumHits_;
  unsigned minNumPixelHits_;
  double   maxChi2DoF_;
  double   maxDeltaPoverP_;
  double   minPt_;

  bool useLinearApprox_;

  bool useLifetimeConstraint_;

  AlgebraicVector3 pvPosition_;
  mutable std::vector<SVfit::track::TrackExtrapolation> selectedTrackInfo_;
  mutable bool isNewCandidate_;
};

#endif
