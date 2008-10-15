import FWCore.ParameterSet.Config as cms

# Magnetic Field
# Geometries
from Geometry.CommonDetUnit.bareGlobalTrackingGeometry_cfi import *
from RecoMuon.DetLayers.muonDetLayerGeometry_cfi import *
#import TrackingTools.TrackFitters.KFTrajectoryFitterESProducer_cfi
#KFTrajectoryFitterForSTA = TrackingTools.TrackFitters.KFTrajectoryFitterESProducer_cfi.KFTrajectoryFitter.clone()
# Stand Alone Muons Producer
from RecoMuon.SETSeedGenerator.selectorSET_cfi import *



