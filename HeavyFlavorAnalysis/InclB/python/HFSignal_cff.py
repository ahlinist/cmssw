import FWCore.ParameterSet.Config as cms



signalDump = cms.EDAnalyzer("HFDumpSignal",
    usejetforip = cms.untracked.int32(1),
    muonLabel = cms.untracked.string('muons'),
    jetsLabel = cms.untracked.string('myak5TrackJets'),
    tracksLabel = cms.untracked.string('alltrackCandidates'),
    vertexLabel = cms.untracked.string('offlinePrimaryVerticesWithBS'),
    verbose = cms.untracked.int32(0)
)

SignalDump = cms.Sequence(signalDump)

 
