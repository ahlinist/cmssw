#include "TauAnalysis/CandidateTools/interface/SVfitTrackExtrapolation.h"

#include "TauAnalysis/CandidateTools/interface/svFitAuxFunctions.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateClosestToPoint.h"
#include <TRotation.h>
#include <TMath.h>

using namespace SVfit_namespace;

SVfitTrackExtrapolation::SVfitTrackExtrapolation(const reco::TransientTrack& transientTrack, const AlgebraicVector3& refPoint) 
  : errorFlag_(0) 
{
//--- compute point of closest approach of track to reference point
  GlobalPoint refPoint_global(refPoint.At(0), refPoint.At(1), refPoint.At(2));

  TrajectoryStateClosestToPoint dcaPosition = transientTrack.trajectoryStateClosestToPoint(refPoint_global);
  if ( TMath::IsNaN(dcaPosition.position().x()) ||
       TMath::IsNaN(dcaPosition.position().y()) ||
       TMath::IsNaN(dcaPosition.position().z()) ) {
    edm::LogWarning ("TrackExtrapolation")
      << " Failed to extrapolate track: Pt = " << transientTrack.track().pt() << ","
      << " eta = " << transientTrack.track().eta() << ", phi = " << transientTrack.track().phi()*180./TMath::Pi()
      << " @ point(" << refPoint_global.x() << ", " << refPoint_global.y() << ", " << refPoint_global.z() << ") "
      << " --> skipping !!";
    errorFlag_ = 1;
  }

  AlgebraicMatrix33 covMatrix = dcaPosition.theState().cartesianError().position().matrix_new();

  initialize(dcaPosition.referencePoint(), dcaPosition.position(), dcaPosition.momentum(), covMatrix);
}

void 
SVfitTrackExtrapolation::initialize(const GlobalPoint& refPoint, 
				    const GlobalPoint& dcaPosition,
				    const GlobalVector& tangent, 
				    const AlgebraicMatrix33& covMatrix) 
{
  refPoint_(0) = refPoint.x();
  refPoint_(1) = refPoint.y();
  refPoint_(2) = refPoint.z();

  dcaPosition_(0) = dcaPosition.x();
  dcaPosition_(1) = dcaPosition.y();
  dcaPosition_(2) = dcaPosition.z();

  double tangent_mag2 = tangent.mag2();
  if ( tangent_mag2 > 0. ) {
    double tangent_mag = TMath::Sqrt(tangent_mag2);
    tangent_(0) = tangent.x()/tangent_mag;
    tangent_(1) = tangent.y()/tangent_mag;
    tangent_(2) = tangent.z()/tangent_mag;
  } else {
    tangent_(0) = 0.;
    tangent_(1) = 0.;
    tangent_(2) = 0.;
  }

  covMatrix_ = covMatrix;
}

