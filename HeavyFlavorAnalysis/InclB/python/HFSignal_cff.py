import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.MagneticField_cff import *
from Configuration.StandardSequences.Services_cff import *
from Configuration.StandardSequences.Geometry_cff import *
#from TrackingTools.TransientTrack.TransientTrackBuilder_cfi import *
from Configuration.StandardSequences.FrontierConditions_GlobalTag_cff import *
GlobalTag.globaltag = "IDEAL_V9::All" # or "" or... 


signalDump = cms.EDFilter("HFDumpSignal",
    muonLabel = cms.untracked.string('muons'),
    jetsLabel = cms.untracked.string('sis5TrackJets'),
    tracksLabel = cms.untracked.string('trackCandidates'),
    vertexLabel = cms.untracked.string('offlinePrimaryVerticesWithBS'),
    verbose = cms.untracked.int32(0)
)

SignalDump = cms.Sequence(signalDump)

 
