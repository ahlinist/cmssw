#include "TrackingAnalysis/Cosmics/interface/TrajectoryAtValidHit.h"
#include "TrackingTools/TrackFitters/interface/TrajectoryStateCombiner.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateOnSurface.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateTransform.h"
#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"
#include "Geometry/CommonTopologies/interface/StripTopology.h"
#include "Geometry/CommonTopologies/interface/PixelTopology.h"
#include "DataFormats/GeometryCommonDetAlgo/interface/MeasurementError.h"
#include "DataFormats/GeometryCommonDetAlgo/interface/MeasurementVector.h"
#include "DataFormats/SiStripDetId/interface/StripSubdetector.h"
#include "DataFormats/SiStripDetId/interface/TIBDetId.h"
#include "DataFormats/SiStripDetId/interface/TIDDetId.h"
#include "DataFormats/SiStripDetId/interface/TOBDetId.h"
#include "Geometry/TrackerGeometryBuilder/interface/GluedGeomDet.h"


using namespace std;
TrajectoryAtValidHit::TrajectoryAtValidHit( const TrajectoryMeasurement& tm, const TrackerGeometry* tracker)
{

  theCombinedPredictedState = TrajectoryStateCombiner().combine( tm.forwardPredictedState(),
  								 tm.backwardPredictedState());

  theHit = tm.recHit();  
  
  uint iidd = theHit->geographicalId().rawId();
  
  
  StripSubdetector strip=StripSubdetector(iidd);
  unsigned int subid=strip.subdetId();
  uint laytib = 1000;
  uint laytob = 1000;
  float xB = 0.; 
  float yB = 0.;
  //set bounds for point to be within to be counted in the study
  if (subid ==  StripSubdetector::TIB) { 
    TIBDetId tibid(iidd);
    laytib =tibid.layer();
    xB = 0.3;
    yB = 0.5;
  }
  if (subid ==  StripSubdetector::TOB) { 
    TOBDetId tobid(iidd);
    laytob =tobid.layer();
    xB = 0.3;
    yB = 1.0;
  }
  
  
  LocalVector monoco, stereoco;
  LocalPoint pmonoco, pstereoco;

  const GeomDetUnit * monodet;
  const GeomDetUnit * stereodet;
 
  locX_temp = theCombinedPredictedState.localPosition().x();
  locY_temp = theCombinedPredictedState.localPosition().y();
  monodet = (GeomDetUnit*)theHit->det();
  stereodet = (GeomDetUnit*)theHit->det();
  

  // Restrict the bound regions for better understanding of the modul assignment. 

  LocalPoint BoundedPoint;
  float xx, yy ,zz;

  // Insert the bounded values 

  if (locX_temp < 0. ) xx  = locX_temp - xB;
  else  xx = locX_temp + xB;
  if (locY_temp < 0. ) yy = locY_temp - yB;
  else  yy = locY_temp + yB;
    
  zz = theCombinedPredictedState.localPosition().z();
  
  
  BoundedPoint = LocalPoint(xx,yy,zz);
  
  if ( monodet->surface().bounds().inside(BoundedPoint)) {
    
    locX = locX_temp;
    locY = locY_temp;
  }
  else {
    // hit is within xB, yB from the edge of the detector, so throw it out 
    locX = 2000.;
    locY = 2000.;
  }
}

double TrajectoryAtValidHit::localX() const
{
  return locX;
}
double TrajectoryAtValidHit::localY() const
{
  return locY;
}
double TrajectoryAtValidHit::localZ() const
{
  return theCombinedPredictedState.localPosition().z();
}
double TrajectoryAtValidHit::localErrorX() const
{
  return sqrt(theCombinedPredictedState.localError().positionError().xx());
}
double TrajectoryAtValidHit::localErrorY() const
{
  return sqrt(theCombinedPredictedState.localError().positionError().yy());
}
double TrajectoryAtValidHit::globalX() const
{
  return theCombinedPredictedState.globalPosition().x();
}

double TrajectoryAtValidHit::globalY() const
{
  return theCombinedPredictedState.globalPosition().y();
}
double TrajectoryAtValidHit::globalZ() const
{
  return theCombinedPredictedState.globalPosition().z();
}

bool TrajectoryAtValidHit::InValid() const
{
  return IsInvHit;
}
