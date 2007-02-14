#include "TrackingTools/TransientTrack/interface/GsfTransientTrack.h"
#include "DataFormats/TrackReco/interface/TrackExtra.h"
#include "DataFormats/TrackingRecHit/interface/TrackingRecHit.h"
#include "DataFormats/TrackingRecHit/interface/TrackingRecHitFwd.h"
#include "Geometry/CommonDetUnit/interface/GeomDet.h"
#include "Geometry/Records/interface/GlobalTrackingGeometryRecord.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "TrackingTools/GeomPropagators/interface/AnalyticalPropagator.h"
#include "TrackingTools/GsfTools/interface/GsfPropagatorAdapter.h"
#include "TrackingTools/GsfTools/interface/MultiTrajectoryStateTransform.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateTransform.h"
#include <iostream>

using namespace reco;
using namespace std;

GsfTransientTrack::GsfTransientTrack() : 
  GsfTrack(), tkr_(), theField(0), initialTSOSAvailable(false),
  initialTSCPAvailable(false)
{
  init();
}

GsfTransientTrack::GsfTransientTrack( const GsfTrack & tk , const MagneticField* field) : 
  GsfTrack(tk), tkr_(), theField(field), initialTSOSAvailable(false),
  initialTSCPAvailable(false) 
{
  init();
  TrajectoryStateTransform theTransform;
  initialFTS = theTransform.initialFreeState(tk, field);
}


GsfTransientTrack::GsfTransientTrack( const GsfTrackRef & tk , const MagneticField* field) : 
  GsfTrack(*tk), tkr_(tk), theField(field), initialTSOSAvailable(false),
  initialTSCPAvailable(false)
{
  init();
  TrajectoryStateTransform theTransform;
  initialFTS = theTransform.initialFreeState(*tk, field);
}

GsfTransientTrack::GsfTransientTrack( const GsfTrack & tk , const MagneticField* field, const edm::ESHandle<GlobalTrackingGeometry>& tg) :
  GsfTrack(tk), tkr_(), theField(field), initialTSOSAvailable(false),
  initialTSCPAvailable(false), theTrackingGeometry(tg)
{
  init();
  TrajectoryStateTransform theTransform;
  initialFTS = theTransform.initialFreeState(tk, field);
}

GsfTransientTrack::GsfTransientTrack( const GsfTrackRef & tk , const MagneticField* field, const edm::ESHandle<GlobalTrackingGeometry>& tg) :
  GsfTrack(*tk), tkr_(tk), theField(field), initialTSOSAvailable(false),
  initialTSCPAvailable(false), theTrackingGeometry(tg)
{
  init();
  TrajectoryStateTransform theTransform;
  initialFTS = theTransform.initialFreeState(*tk, field);
}


GsfTransientTrack::GsfTransientTrack( const GsfTransientTrack & tt ) :
  GsfTrack(tt), tkr_(tt.persistentTrackRef()), theField(tt.field()), 
  initialFTS(tt.initialFreeState()), initialTSOSAvailable(false),
  initialTSCPAvailable(false)
{
  init();
  if (tt.initialTSOSAvailable) {
    initialTSOS= tt.impactPointState();
    initialTSOSAvailable = true;
  }
  if (tt.initialTSCPAvailable) {
    initialTSCP= tt.impactPointTSCP();
    initialTSCPAvailable = true;
  }
}

void GsfTransientTrack::init()
{
  thePropagator = 
    new GsfPropagatorAdapter(AnalyticalPropagator(theField, alongMomentum));
  theTIPExtrapolator = new TransverseImpactPointExtrapolator(*thePropagator);
}


void GsfTransientTrack::setES(const edm::EventSetup& setup) {

  setup.get<GlobalTrackingGeometryRecord>().get(theTrackingGeometry); 

}

void GsfTransientTrack::setTrackingGeometry(const edm::ESHandle<GlobalTrackingGeometry>& tg) {

  theTrackingGeometry = tg;

}


TrajectoryStateOnSurface GsfTransientTrack::impactPointState() const
{
  if (!initialTSOSAvailable) calculateTSOSAtVertex();
  return initialTSOS;
}

TrajectoryStateClosestToPoint GsfTransientTrack::impactPointTSCP() const
{
  if (!initialTSCPAvailable) {
    initialTSCP = builder(initialFTS, initialFTS.position());
    initialTSCPAvailable = true;
  }
  return initialTSCP;
}

TrajectoryStateOnSurface GsfTransientTrack::outermostMeasurementState() const
{
    MultiTrajectoryStateTransform theTransform;
    return theTransform.outerStateOnSurface((*this),*theTrackingGeometry,theField);
}

TrajectoryStateOnSurface GsfTransientTrack::innermostMeasurementState() const
{
    MultiTrajectoryStateTransform theTransform;
    return theTransform.innerStateOnSurface((*this),*theTrackingGeometry,theField);
}

void GsfTransientTrack::calculateTSOSAtVertex() const
{
  TransverseImpactPointExtrapolator tipe(theField);
  initialTSOS = tipe.extrapolate(initialFTS, initialFTS.position());
  initialTSOSAvailable = true;
}

TrajectoryStateOnSurface 
GsfTransientTrack::stateOnSurface(const GlobalPoint & point) const
{
  return theTIPExtrapolator->extrapolate(innermostMeasurementState(), point);
}


TrajectoryStateClosestToPoint 
GsfTransientTrack::trajectoryStateClosestToPoint( const GlobalPoint & point ) const
{
  return builder(stateOnSurface(point), point);
}
