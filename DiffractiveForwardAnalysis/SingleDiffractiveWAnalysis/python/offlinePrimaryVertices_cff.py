import FWCore.ParameterSet.Config as cms

# Reco Vertex
# initialize magnetic field #########################
from Configuration.StandardSequences.MagneticField_38T_cff import *
from Configuration.StandardSequences.GeometryPilot2_cff import *
from Configuration.StandardSequences.FrontierConditions_GlobalTag_cff import *
GlobalTag.globaltag = 'IDEAL_V11::All'

from TrackingTools.TransientTrack.TransientTrackBuilder_cfi import *
import RecoVertex.PrimaryVertexProducer.OfflinePrimaryVertices_cfi
offlinePrimaryVerticesFromCTFTracksAVF = RecoVertex.PrimaryVertexProducer.OfflinePrimaryVertices_cfi.offlinePrimaryVertices.clone()
import RecoVertex.PrimaryVertexProducer.OfflinePrimaryVertices_cfi
offlinePrimaryVerticesFromCTFTracksKVF = RecoVertex.PrimaryVertexProducer.OfflinePrimaryVertices_cfi.offlinePrimaryVertices.clone()
import RecoVertex.PrimaryVertexProducer.OfflinePrimaryVertices_cfi
offlinePrimaryVerticesFromCTFTracksTKF = RecoVertex.PrimaryVertexProducer.OfflinePrimaryVertices_cfi.offlinePrimaryVertices.clone()
#include "Validation/RecoVertex/data/OffLinePVFromRSTracks.cfi"
vertexreco = cms.Sequence(offlinePrimaryVerticesFromCTFTracksAVF*offlinePrimaryVerticesFromCTFTracksKVF)
offlinePrimaryVerticesFromCTFTracksAVF.algorithm = 'AdaptiveVertexFitter'
offlinePrimaryVerticesFromCTFTracksKVF.algorithm = 'KalmanVertexFitter'
offlinePrimaryVerticesFromCTFTracksTKF.algorithm = 'TrimmedKalmanFinder'


