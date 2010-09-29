import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Reconstruction_cff import *
from SimGeneral.TrackingAnalysis.trackingParticles_cfi import *
from Configuration.StandardSequences.MagneticField_38T_cff import *
from PhysicsTools.RecoAlgos.allTracks_cfi import *
from PhysicsTools.RecoAlgos.allTrackCandidates_cfi import *
from PhysicsTools.HepMCCandAlgos.allTracksGenParticlesMatch_cfi import *
from Geometry.TrackerGeometryBuilder.trackerGeometry_cfi import *
from TrackingTools.TransientTrack.TransientTrackBuilder_cfi import *
allTracksGenParticlesMatch.src = "allTrackCandidates"
from HeavyFlavorAnalysis.InclB.HFTrackJets_cff import *

matching = cms.EDProducer( "MCTruthDeltaRMatcherNew",
                           src = cms.InputTag("allTrackCandidates"),
                           matched = cms.InputTag("genParticles"),
                           distMin = cms.double(0.15)
                         )


anal = cms.EDAnalyzer("EventContentAnalyzer")

<<<<<<< HFTracks_cff.py
trackDump = cms.EDAnalyzer("HFDumpTracks",    
=======
trkDump = cms.EDAnalyzer("HFDumpTracks",
    
>>>>>>> 1.3
    tracksLabel = cms.untracked.string('generalTracks'),
    tracksLabel2 = cms.untracked.string('alltrackCandidates'),
    muonsLabel = cms.untracked.InputTag("muons"),
    verbose = cms.untracked.int32(0),
    doTruthMatching = cms.untracked.int32(2),
    vertexLabel = cms.untracked.string('offlinePrimaryVerticesWithBS'),
    jetsLabel = cms.untracked.string('myak5TrackJets'),
    #reco matching
    trackingParticlesLabel = cms.untracked.string('trackingParticles'),#reco matching
#aod matching     
    associatorLabel3 = cms.untracked.string('matching'),      
    genParticlesLabel = cms.untracked.string('genParticles'),           
    trackcandsLabel = cms.untracked.string('allTrackCandidates')            
)


TrackDump = cms.Sequence(allTrackCandidates*matching*trackDump)
TrackDumpAOD = cms.Sequence(trackDump)
