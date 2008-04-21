import FWCore.ParameterSet.Config as cms

from MagneticField.Engine.volumeBasedMagneticField_cfi import *
from Geometry.CMSCommonData.cmsIdealGeometryXML_cfi import *
from Geometry.TrackerGeometryBuilder.trackerGeometry_cfi import *
from Geometry.TrackerNumberingBuilder.trackerNumberingGeometry_cfi import *
from RecoTracker.GeometryESProducer.TrackerRecoGeometryESProducer_cfi import *
from RecoLocalTracker.SiStripRecHitConverter.StripCPEfromTrackAngle_cfi import *
from RecoLocalTracker.SiStripRecHitConverter.SiStripRecHitMatcher_cfi import *
from RecoLocalTracker.SiPixelRecHits.PixelCPEParmError_cfi import *
from RecoTracker.TransientTrackingRecHit.TransientTrackingRecHitBuilder_cfi import *
from RecoTracker.MeasurementDet.MeasurementTrackerESProducer_cfi import *
from TrackingTools.MaterialEffects.MaterialPropagator_cfi import *
from RecoTracker.TkSeedingLayers.TTRHBuilderWithoutAngle4MixedTriplets_cfi import *
from RecoTracker.TkSeedingLayers.TTRHBuilderWithoutAngle4MixedPairs_cfi import *
from RecoTracker.TkSeedingLayers.MixedLayerTriplets_cfi import *
from RecoTracker.TkSeedingLayers.PixelLayerTriplets_cfi import *
from RecoTracker.TkSeedGenerator.GlobalSeedsFromTripletsWithVertices_cfi import *

