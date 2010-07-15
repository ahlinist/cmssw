import FWCore.ParameterSet.Config as cms

slhcSimpleVertexAnalysis = cms.EDAnalyzer("SLHCPrimaryVertexAnalyzer",
    simG4 = cms.InputTag("g4SimHits"),
    outputFile = cms.untracked.string('slhcSimpleVertexAnalyzer.root'),
    vtxSample = cms.untracked.vstring('offlinePrimaryVertices','offlinePrimaryVerticesWithBS'),
    verbose = cms.untracked.bool(True),
    recoTrackProducer = cms.untracked.string('generalTracks')
)



