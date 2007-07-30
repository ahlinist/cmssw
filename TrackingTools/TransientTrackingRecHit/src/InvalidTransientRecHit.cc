#include "TrackingTools/TransientTrackingRecHit/interface/InvalidTransientRecHit.h"
#include "DataFormats/GeometryCommonDetAlgo/interface/ErrorFrameTransformer.h"
#include "DataFormats/TrackingRecHit/interface/InvalidTrackingRecHit.h"
#include "FWCore/Utilities/interface/Exception.h"


InvalidTransientRecHit::InvalidTransientRecHit( const GeomDet* geom, const DetLayer * layer, Type type ) :
  Base( geom, InvalidTrackingRecHit( geom == 0 ? DetId(0) : geom->geographicalId(), type)), 
  layer_(layer)
{
}

const Surface* InvalidTransientRecHit::surface() const {
  if (det() != 0 )  return &(det()->surface()); 
  else if (layer_ != 0)  return &(layer_->surface()); 
  else return 0;
}

