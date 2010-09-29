import FWCore.ParameterSet.Config as cms

<<<<<<< HFSimTracks_cff.py
simtrackDump = cms.EDAnalyzer("HFDumpSimTracks",    
=======
simtrkDump = cms.EDAnalyzer("HFDumpSimTracks",
    
>>>>>>> 1.2
    tracksLabel = cms.untracked.string('g4SimHits'),
    verbose = cms.untracked.int32(0)
)


SimTrackDump = cms.Sequence(simtrackDump)

