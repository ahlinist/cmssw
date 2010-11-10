#include "TauAnalysis/CandidateTools/interface/SVfitTrackExtrapolation.h"

#include "TauAnalysis/CandidateTools/interface/svFitAuxFunctions.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateClosestToPoint.h"
#include <TRotation.h>
#include <TMath.h>

using namespace SVfit_namespace;

namespace SVfit { namespace track {

TrackExtrapolation::TrackExtrapolation(const reco::TransientTrack& transientTrack, const AlgebraicVector3& refPoint)
: errorFlag_(0)
{
  //std::cout << "<TrackExtrapolation::TrackExtrapolation>:" << std::endl;

  //std::cout << "refPoint:" << std::endl;
  //std::cout << " x = " << refPoint.At(0) << ", y = " << refPoint.At(1) << ", z = " << refPoint.At(2) << std::endl;

  //--- compute point of closest approach of track to reference point
  GlobalPoint refPoint_global(refPoint.At(0), refPoint.At(1), refPoint.At(2));

  TrajectoryStateClosestToPoint dcaPosition = transientTrack.trajectoryStateClosestToPoint(refPoint_global);
  if (TMath::IsNaN(dcaPosition.position().x()) ||
      TMath::IsNaN(dcaPosition.position().y()) ||
      TMath::IsNaN(dcaPosition.position().z()) ) {
    edm::LogWarning ("TrackExtrapolation")
        << " Failed to extrapolate track: Pt = " << transientTrack.track().pt() << ","
        << " eta = " << transientTrack.track().eta() << ", phi = " << transientTrack.track().phi()*180./TMath::Pi()
        << " --> skipping !!";
    errorFlag_ = 1;
  }

  dcaPosition_(0) = dcaPosition.position().x();
  dcaPosition_(1) = dcaPosition.position().y();
  dcaPosition_(2) = dcaPosition.position().z();
  //std::cout << "dcaPosition:" << std::endl;
  //std::cout << " x = " << dcaPosition_.At(0) << ", y = " << dcaPosition_.At(1) << ", z = " << dcaPosition_.At(2) << std::endl;

  tangent_(0) = dcaPosition.momentum().x();
  tangent_(1) = dcaPosition.momentum().y();
  tangent_(2) = dcaPosition.momentum().z();
  //std::cout << "tangent:" << std::endl;
  //std::cout << " x = " << tangent_.At(0) << ", y = " << tangent_.At(1) << ", z = " << tangent_.At(2) << std::endl;

  TVector3 tangent_vector(tangent_.At(0), tangent_.At(1), tangent_.At(2));

  TRotation rotation;
  rotation.SetZAxis(tangent_vector);
  TRotation invRotation = rotation.Inverse();

  TVector3 test = invRotation*tangent_vector;
  //std::cout << "test:" << std::endl;
  //std::cout << " x = " << test.x() << ", y = " << test.y() << ", z = " << test.z() << std::endl;
  const double epsilon = 1.e-5;
  assert(TMath::Abs(test.x()) < epsilon && TMath::Abs(test.y()) < epsilon);

  invRotationMatrix_(0, 0) = invRotation.XX();
  invRotationMatrix_(0, 1) = invRotation.XY();
  invRotationMatrix_(0, 2) = invRotation.XZ();
  invRotationMatrix_(1, 0) = invRotation.YX();
  invRotationMatrix_(1, 1) = invRotation.YY();
  invRotationMatrix_(1, 2) = invRotation.YZ();
  invRotationMatrix_(2, 0) = invRotation.ZX();
  invRotationMatrix_(2, 1) = invRotation.ZY();
  invRotationMatrix_(2, 2) = invRotation.ZZ();
  //std::cout << "invRotationMatrix:" << std::endl;
  //invRotationMatrix_.Print(std::cout);
  //std::cout << std::endl;

  //--- compute covariance matrix in rotated coordinates:
  //     return x^T Vxx^-1 x = x^T R^-1 R Vxx^-1 R^-1   R x  // R^-1 = R^T (rotation matrices are orthogonal)
  //                         = x^T R^T  R Vxx^-1 R^-1   R x  // (R x)^T = x^T R^T
  //                         = (R x)^T (R^-1 Vxx R)^-1 (R x) // y := R x
  //                         = y^T (R^T Vxx R)^-1 y          // Vyy := R^T Vxx R
  //                         = y^T Vyy^-1 y
  AlgebraicMatrix33 covMatrix = dcaPosition.theState().cartesianError().position().matrix_new();
  //std::cout << "covMatrix:" << std::endl;
  //covMatrix.Print(std::cout);
  //std::cout << std::endl;

  rotCovMatrix_ = ROOT::Math::Transpose(invRotationMatrix_)*covMatrix*invRotationMatrix_;
  //std::cout << "covMatrix in rotated coordinates:" << std::endl;
  //rotCovMatrix_.Print(std::cout);
  //std::cout << std::endl;

  for ( unsigned iRow = 0; iRow < 2; ++iRow ) {
    for ( unsigned iColumn = 0; iColumn < 2; ++iColumn ) {
      rotCovMatrix2_(iRow, iColumn) = rotCovMatrix_(iRow, iColumn);
    }
  }
}

double TrackExtrapolation::logLikelihood(const AlgebraicVector3& sv) const {
  // Solve for the closest point on the line to the test point
  // The line is defined by  P(t) = P1 + t*(P2-P1), where P1 = dcaPosition
  // and P2 = dcaPosition + tangent();  P3 is the test point. At DCA P,
  // (P3 - P(t)).(P2-P1) == 0
  using namespace TMath;
  double x1 = dcaPosition_.At(0);
  double y1 = dcaPosition_.At(1);
  double z1 = dcaPosition_.At(2);

  double x2 = dcaPosition_.At(0) + tangent_.At(0);
  double y2 = dcaPosition_.At(1) + tangent_.At(1);
  double z2 = dcaPosition_.At(2) + tangent_.At(2);

  double x3 = sv.At(0);
  double y3 = sv.At(1);
  double z3 = sv.At(2);

  double t = (Power(x1,2) + x2*x3 - x1*(x2 + x3) + Power(y1,2) - y1*y2 - y1*y3 + y2*y3 + Power(z1,2) - z1*z2 - z1*z3 + z2*z3)/
   (Power(x1,2) - 2*x1*x2 + Power(x2,2) + Power(y1,2) - 2*y1*y2 + Power(y2,2) + Power(z1,2) - 2*z1*z2 + Power(z2,2));

  AlgebraicVector3 dcaOnTrack = dcaPosition_ + t*(tangent_);

  AlgebraicVector3 displacement = sv - dcaOnTrack;
  //std::cout << "track likelihood - got displacement" << displacement << std::endl;
  return logLikelihoodFromDisplacement(displacement);
}

double TrackExtrapolation::logLikelihoodFromDisplacement(
    const AlgebraicVector3& displacement) const
{
  //std::cout << "<TrackExtrapolation::logLikelihood>:" << std::endl;

  if ( errorFlag_ ) return 0.;

  AlgebraicVector3 rotDisplacement = invRotationMatrix_*displacement;
  //std::cout << "displacement in rotated coordinates:" << std::endl;
  //rotDisplacement.Print(std::cout);
  //std::cout << std::endl;

  AlgebraicVector2 rotDisplacement2;
  rotDisplacement2(0) = rotDisplacement(0);
  rotDisplacement2(1) = rotDisplacement(1);

  double logLikelihood = logGaussianNd(rotDisplacement2, rotCovMatrix2_);

  //--- add "penalty" term in case displacement has component opposite to direction of track momentum
  if ( rotDisplacement(2) < 0. ) {
    //double penaltyTerm = -0.5*square(rotDisplacement(2)/TMath::Sqrt(rotCovMatrix_(2, 2)));
    double penaltyTerm = -square(10.*rotDisplacement(2));
    //std::cout << "displacement in track direction is negative"
    //	        << " --> adding 'penalty' term  = " << penaltyTerm << " to likelihood !!" << std::endl;
    logLikelihood += penaltyTerm;
  }

  return logLikelihood;
}


}} // end namespace SVfit::track
