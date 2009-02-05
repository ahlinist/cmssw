#ifndef TrajectoryAtValidHit_H
#define TrajectoryAtValidHit_H

#include "TrackingTools/PatternTools/interface/TrajectoryMeasurement.h"
#include "DataFormats/GeometryCommonDetAlgo/interface/MeasurementError.h"
#include "DataFormats/GeometryCommonDetAlgo/interface/MeasurementVector.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"

class Topology;
class TransientTrackingRecHit;
class StripTopology;
class PixelTopology;
class TrajectoryAtValidHit {
public:

  TrajectoryAtValidHit( const TrajectoryMeasurement&, const TrackerGeometry * tracker, const uint mono = 0);

  double localX() const;
  double localY() const;
  double localErrorX() const;
  double localErrorY() const;
 
  double localZ() const;

  double globalX() const;
  double globalY() const;
  double globalZ() const;

  uint monodet_id() const;

  LocalPoint project(const GeomDet *det, const GeomDet* projdet, LocalPoint position, LocalVector trackdirection) const; 
  bool isDoubleSided(uint iidd) const;
 
private:

  TrajectoryStateOnSurface theCombinedPredictedState;
  float locX_temp,locY_temp;
  float locX,locY;
  float locXError, locYError;
  uint iidd;

  ConstReferenceCountingPointer<TransientTrackingRecHit> theHit;
};

#endif
