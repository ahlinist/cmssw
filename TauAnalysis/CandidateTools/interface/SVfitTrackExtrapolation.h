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

namespace SVfit { namespace track {
class TrackExtrapolation {
  public:
    // Default construct
    TrackExtrapolation() {}
    // Constructor from a transient track and reference spatial point
    TrackExtrapolation(const reco::TransientTrack&, const AlgebraicVector3&);
    // Constructor from values for testing
    TrackExtrapolation(
        const GlobalPoint& refPoint,
        const GlobalPoint& dcaPosition,
        const GlobalVector& tangent,
        const AlgebraicMatrix33& covMatrix);

    const AlgebraicVector3& tangent() const { return tangent_; }
    const AlgebraicVector3& dcaPosition() const { return dcaPosition_; }
    const AlgebraicVector3& refPoint() const { return refPoint_; }
    const AlgebraicMatrix33& covariance() const { return covMatrix_; }
    // The appoximate spatial error on the track
    double approximateTrackError() const { return approximateTrackError_; }

    // Compute the displacent of a test point from the extrapolated track.
    AlgebraicVector3 displacementFromTrack(const AlgebraicVector3&) const;

    // Log-likelihood given a secondary vertex
    double logLikelihood(const AlgebraicVector3&) const;

    // Log-likelihood given the displacement
    double logLikelihoodFromDisplacement(const AlgebraicVector3&) const;

  private:
    // Implementation of the constructors
    void construct(
        const GlobalPoint& refPoint,
        const GlobalPoint& dcaPosition,
        const GlobalVector& tangent,
        const AlgebraicMatrix33& covMatrix);

    AlgebraicVector3 tangent_;
    AlgebraicVector3 dcaPosition_;
    AlgebraicVector3 refPoint_;
    AlgebraicMatrix33 invRotationMatrix_;
    AlgebraicMatrix33 covMatrix_;
    AlgebraicMatrix33 rotCovMatrix_;
    AlgebraicMatrix22 rotCovMatrix2_;
    // Inverted rotated covariance matrix
    AlgebraicMatrix22 invRotCovMatrx2_;
    // Determinant of rotated covariance matrix
    double detRotCovMatrix2_;
    // Get the error in the direction of the ref. point
    double approximateTrackError_;

    int errorFlag_;
};
}} // end namespace SVfit::track

// Print to stream
std::ostream& operator<<(std::ostream& stream,
    const SVfit::track::TrackExtrapolation& trk);
#endif
