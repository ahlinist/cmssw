#ifndef TauAnalysis_CandidateTools_SVfitTrackExtrapolation_h
#define TauAnalysis_CandidateTools_SVfitTrackExtrapolation_h

/*
 * \class SVfit::track::TrackExtrapolation
 *
 * Class to manage computation of compatability between tracks and candidate
 * decay vertex points.
 *
 * \author Evan Friis, Christian Veelken; UC davis
 *
 */

#include "TrackingTools/TransientTrack/interface/TransientTrack.h"

namespace SVfit { namespace track {
struct TrackExtrapolation {
  TrackExtrapolation(const reco::TransientTrack&, const AlgebraicVector3&);

  const AlgebraicVector3& tangent() const { return tangent_; }
  const AlgebraicVector3& dcaPosition() const { return dcaPosition_; }
  const AlgebraicVector3& refPoint() const { return dcaPosition_; }

  // Log-likelihood given a secondary vertex
  double logLikelihood(const AlgebraicVector3&) const;

  // Log-likelihood given the displacement
  double logLikelihoodFromDisplacement(const AlgebraicVector3&) const;

  AlgebraicVector3 tangent_;
  AlgebraicVector3 dcaPosition_;
  AlgebraicMatrix33 invRotationMatrix_;
  AlgebraicMatrix33 rotCovMatrix_;
  AlgebraicMatrix22 rotCovMatrix2_;

  int errorFlag_;
};
}} // end namespace SVfit::track
#endif
