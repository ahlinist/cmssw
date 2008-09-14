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

  TrajectoryAtValidHit( const TrajectoryMeasurement&, const TrackerGeometry * tracker);

  double localX() const;
  double localY() const;
  double localErrorX() const;
  double localErrorY() const;
 
  double localZ() const;

  double globalX() const;
  double globalY() const;
  double globalZ() const;
  bool InValid() const; 
 
private:

  bool IsInvHit;

  TrajectoryStateOnSurface theCombinedPredictedState;
  float locX_temp,locY_temp;
  float locX,locY;

  ConstReferenceCountingPointer<TransientTrackingRecHit> theHit;
};

#endif
