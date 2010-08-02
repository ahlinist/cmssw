import FWCore.ParameterSet.Config as cms

simtrkDump = cms.EDAnalyzer("HFDumpSimTracks",
    
    tracksLabel = cms.untracked.string('g4SimHits'),
    verbose = cms.untracked.int32(0)
)


SimTrackDump = cms.Sequence(simtrkDump)

