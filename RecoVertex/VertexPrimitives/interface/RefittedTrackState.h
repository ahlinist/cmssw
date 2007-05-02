#ifndef RefittedTrackState_H
#define RefittedTrackState_H

#include "DataFormats/GeometrySurface/interface/ReferenceCounted.h"
#include "TrackingTools/TrajectoryState/interface/CopyUsingClone.h"
#include "DataFormats/CLHEP/interface/AlgebraicObjects.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include <vector>

/**
 * Pure abstract base class, caching refitted state of the
 * trajectory after the vertex fit is done. Every concrete implementaton
 * deals with its own trajectory representation.
 */
class FreeTrajectoryState;
class TrajectoryStateOnSurface;
class Surface;
class Propagator;

class RefittedTrackState : public ReferenceCounted {

public:

  virtual ~RefittedTrackState(){}

  /**
   * Transformation into a FreeTrajectoryState
   */
  virtual FreeTrajectoryState freeTrajectoryState() const = 0;

  /**
   * Transformation into a TSOS at a given surface
   */
  virtual TrajectoryStateOnSurface trajectoryStateOnSurface(
  		const Surface & surface) const = 0;

  /**
   * Transformation into a TSOS at a given surface, with a given propagator
   */

  virtual TrajectoryStateOnSurface trajectoryStateOnSurface(
		const Surface & surface, const Propagator & propagator) const = 0;

  /**
   * Vector containing the refitted track parameters.
   */
  virtual AlgebraicVector5 parameters() const = 0;

  /**
   * The covariance matrix
   */
  virtual AlgebraicSymMatrix55 covariance() const = 0;

  /**
   * Position at which the momentum is defined.
   */
  virtual GlobalPoint position() const = 0;

  /**
   * Vector containing the parameters describing the momentum as the vertex
   */

  virtual AlgebraicVector3 momentumVector() const = 0;

  /**
   *   The weight of this component in a mixture
   */

  virtual double weight() const = 0;

  /**
   * Returns a new refitted state of the same type, but with another weight.
   * The current state is unchanged.
   */

  virtual ReferenceCountingPointer<RefittedTrackState> stateWithNewWeight
  	(const double newWeight) const = 0;

  virtual std::vector< ReferenceCountingPointer<RefittedTrackState> > components() const = 0;

  virtual reco::TransientTrack transientTrack() const = 0;

};
#endif
