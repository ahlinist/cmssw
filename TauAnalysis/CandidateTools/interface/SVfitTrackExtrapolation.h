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
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "DataFormats/GeometryVector/interface/GlobalVector.h"

#include <iostream>

class SVfitTrackExtrapolation 
{
 public:
  // Default construct
  SVfitTrackExtrapolation() {}
  // Constructor from transient track plus reference point
  SVfitTrackExtrapolation(const reco::TransientTrack&, const AlgebraicVector3&);

  const AlgebraicVector3& tangent() const { return tangent_; }
  const AlgebraicVector3& point_of_closest_approach() const { return dcaPosition_; }
  const AlgebraicVector3& refPoint() const { return refPoint_; }
  const AlgebraicMatrix33& covariance() const { return covMatrix_; }

  int errorFlag() const { return errorFlag_; }

 private:
  void initialize(const GlobalPoint&, const GlobalPoint&, const GlobalVector&, const AlgebraicMatrix33&);

  AlgebraicVector3 tangent_;
  AlgebraicVector3 dcaPosition_;
  AlgebraicVector3 refPoint_;
  AlgebraicMatrix33 covMatrix_;
  
  int errorFlag_;
};

#endif
