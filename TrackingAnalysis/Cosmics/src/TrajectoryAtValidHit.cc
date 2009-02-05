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
#include "DataFormats/SiStripDetId/interface/TECDetId.h"
#include "Geometry/TrackerGeometryBuilder/interface/GluedGeomDet.h"
#include "RecoTracker/MeasurementDet/interface/RecHitPropagator.h"
#include "TrackingTools/TransientTrackingRecHit/interface/TrackingRecHitProjector.h"
#include "RecoTracker/TransientTrackingRecHit/interface/ProjectedRecHit2D.h"


using namespace std;
TrajectoryAtValidHit::TrajectoryAtValidHit( const TrajectoryMeasurement& tm, 
					    const TrackerGeometry* tracker,
					    const uint mono)
{

  theCombinedPredictedState = TrajectoryStateCombiner().combine( tm.forwardPredictedState(),
  								 tm.backwardPredictedState());

  theHit = tm.recHit();  
  
  iidd = theHit->geographicalId().rawId();
  TrajectoryStateTransform tsostransform;
  PTrajectoryStateOnDet* combinedptsod=tsostransform.persistentState( theCombinedPredictedState,iidd);

  StripSubdetector strip=StripSubdetector(iidd);
  unsigned int subid=strip.subdetId();
  // xB and yB are for absolute borders on the trajectories included in the study, sigmaX sigmaY are 
  // significance cuts on the distance from the detector surface
  float xB = 0.; float sigmaX = 5.0;
  float yB = 0.; float sigmaY = 5.0;
  float sigmaYBond = 0.;
  //set bounds for point to be within to be counted in the study
  if (subid ==  StripSubdetector::TOB) { 
    sigmaYBond = 5.0;
  }

  LocalVector monoco;
  LocalPoint pmonoco;
  
  const GeomDetUnit * monodet;
  
  //if module is stereo and from a matched layer then it is from an invalid hit from a matched layer
  // if module is from a double sided layer, check mono value and write out info for either the
  // rphi surface (mono = 1) or the stereo surface (mono = 2)--not the matched hit surface
  //if ( ((iidd & 0x3)==0) && isDoubleSided(iidd) ) {
  if (( mono > 0 ) && isDoubleSided(iidd) ) {
    // find matched hit id, that is the matched hit surface between the two sensors
    uint matched_iidd = iidd-(iidd & 0x3);
    DetId matched_id(matched_iidd);
    
    GluedGeomDet * gdet=(GluedGeomDet *)tracker->idToDet(matched_id);
    const GeomDet * det = tracker->idToDet(iidd);
    GlobalVector gtrkdirco=det->toGlobal(combinedptsod->parameters().momentum());
    
    // get the sensor det indicated by mono
    if (mono == 1) monodet=gdet->stereoDet();
    if (mono == 2) monodet=gdet->monoDet();
    
    monoco=monodet->toLocal(gtrkdirco);
    pmonoco=project(det,monodet,combinedptsod->parameters().position(),monoco);
    
    locX_temp = pmonoco.x(); 
    locY_temp = pmonoco.y(); 
    // just take errors from the traj surface glued or single surface
    locXError = sqrt(theCombinedPredictedState.localError().positionError().xx());
    locYError = sqrt(theCombinedPredictedState.localError().positionError().yy());
    
    //set module id to be mono det
    iidd = monodet->geographicalId().rawId();
    cout << "from trajatvalidhit setting id = " << iidd << endl;
  }
  else {
    locX_temp = theCombinedPredictedState.localPosition().x();
    locY_temp = theCombinedPredictedState.localPosition().y();
    locXError = sqrt(theCombinedPredictedState.localError().positionError().xx());
    locYError = sqrt(theCombinedPredictedState.localError().positionError().yy());
    
    //monodet = theHit->detUnit();
    monodet = (GeomDetUnit*)theHit->det(); //this cast does something different from detUnit()    
    
  }
  
  // this should never be a glued det, only rphi or stero
  cout << "From TrajAtValidHit module " << iidd << "   matched/stereo/rphi = " << ((iidd & 0x3)==0) << "/" << ((iidd & 0x3)==1) << "/" << ((iidd & 0x3)==2) << endl;
    
  // Restrict the bound regions for better understanding of the modul assignment. 

  LocalPoint BoundedPoint;
  float xx, yy ,zz;

  // Insert the bounded values 
  if (locX_temp < 0. ) xx = min(locX_temp - xB,locX_temp - sigmaX*locXError);
  else  xx = max(locX_temp + xB, locX_temp + sigmaX*locXError);

  if (locY_temp < 0. ) yy = min(locY_temp - yB,locY_temp - sigmaY*locYError);
  else  yy = max(locY_temp + yB, locY_temp + sigmaY*locYError);

  zz = theCombinedPredictedState.localPosition().z();

  BoundedPoint = LocalPoint(xx,yy,zz);
  
  if ( monodet->surface().bounds().inside(BoundedPoint) && abs(locY_temp) > sigmaYBond*locYError ){
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

uint TrajectoryAtValidHit::monodet_id() const
{
  return iidd;
}

LocalPoint TrajectoryAtValidHit::project(const GeomDet *det,const GeomDet* projdet,LocalPoint position,LocalVector trackdirection)const
{
  
  GlobalPoint globalpoint=(det->surface()).toGlobal(position);
  
  // position of the initial and final point of the strip in glued local coordinates
  LocalPoint projposition=(projdet->surface()).toLocal(globalpoint);
  
  //correct the position with the track direction
  
  float scale=-projposition.z()/trackdirection.z();
  
  projposition+= scale*trackdirection;
  
  return projposition;
}

bool TrajectoryAtValidHit::isDoubleSided(uint iidd) const {
  StripSubdetector strip=StripSubdetector(iidd);
  unsigned int subid=strip.subdetId();
  uint layer = 0;
  if (subid ==  StripSubdetector::TIB) { 
    TIBDetId tibid(iidd);
    layer = tibid.layer();
    if (layer == 1 || layer == 2) return true;
    else return false;
  }
  else if (subid ==  StripSubdetector::TOB) { 
    TOBDetId tobid(iidd);
    layer = tobid.layer() + 4 ; 
    if (layer == 5 || layer == 6) return true;
    else return false;
  }
  else if (subid ==  StripSubdetector::TID) { 
    TIDDetId tidid(iidd);
    layer = tidid.ring() + 10;
    if (layer == 11 || layer == 12) return true;
    else return false;
  }
  else if (subid ==  StripSubdetector::TEC) { 
    TECDetId tecid(iidd);
    layer = tecid.ring() + 13 ; 
    if (layer == 14 || layer == 15 || layer == 18) return true;
    else return false;
  }
  else
    return false;
}
