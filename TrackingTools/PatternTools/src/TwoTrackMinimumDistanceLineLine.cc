#include "TrackingTools/PatternTools/interface/TwoTrackMinimumDistanceLineLine.h"
#include "TrackingTools/TrajectoryParametrization/interface/GlobalTrajectoryParameters.h"
#include "DataFormats/GeometryVector/interface/GlobalVector.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "MagneticField/Engine/interface/MagneticField.h"


bool TwoTrackMinimumDistanceLineLine::calculate(
    const GlobalTrajectoryParameters & theG,
    const GlobalTrajectoryParameters & theH)
{
  GlobalPoint gOrig = theG.position();
  GlobalPoint hOrig = theH.position();
  if ( ( ( theH.charge() != 0. || theG.charge() != 0. ) ) && 
    ((theG.magneticField().inTesla(gOrig).z() != 0.)|| 
  	(theH.magneticField().inTesla(hOrig).z() != 0.)) )
  {
    edm::LogWarning ("TwoTrackMinimumDistanceLineLine")
      << "charge of input track is not zero or field non zero";
    return true;
  };

  GlobalVector gVec = theG.momentum();
  const double gMag= theG.momentum().mag(); double gMag2 = gMag*gMag;
  GlobalVector hVec = theH.momentum();
  const double hMag= theH.momentum().mag(); double hMag2 = hMag*hMag;

  if ( gMag == 0. || hMag == 0. )
  {
    edm::LogWarning ("TwoTrackMinimumDistanceLineLine")
      << "momentum of input trajectory is zero.";
    return true;
  };

  phiG = theG.momentum().phi();
  phiH = theH.momentum().phi();

  double gVec_Dot_hVec = gVec.dot(hVec);
  double norm  = gVec_Dot_hVec*gVec_Dot_hVec - gMag2*hMag2;

  if ( norm == 0 )
  {
    edm::LogWarning ("TwoTrackMinimumDistanceLineLine")
      << "Tracks are parallel.";
    return true;
  }

  GlobalVector posDiff = gOrig - hOrig;

  double tG = (posDiff.dot(gVec) * hMag2 - gVec_Dot_hVec * posDiff.dot(hVec))/ norm;
  double tH = (gVec_Dot_hVec * posDiff.dot(gVec) - posDiff.dot(hVec) * gMag2)/ norm;

  gPos = GlobalPoint ( gOrig + tG * gVec);
  hPos = GlobalPoint ( hOrig + tH * hVec);
//   cout << "TwoTrackMinimumDistanceLineLine "<<gPos<<hPos<<endl;

  pathG = tG * gMag;
  pathH = tH * hMag;
  return false;
}

pair <GlobalPoint, GlobalPoint> TwoTrackMinimumDistanceLineLine::points() const
{
  return pair <GlobalPoint, GlobalPoint > ( gPos, hPos );
}

pair <double, double> TwoTrackMinimumDistanceLineLine::pathLength() const
{
  return pair <double, double> ( pathG, pathH);
}
