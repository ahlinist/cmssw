import FWCore.ParameterSet.Config as cms

vertexDump = cms.EDAnalyzer("HFDumpVertex",
    verbose = cms.untracked.int32(0),
    simvertexLabel = cms.untracked.string('simG4'),
    vertexTracksLabel= cms.untracked.string('generalTracks'),
    vertexLabel = cms.untracked.string('offlinePrimaryVerticesWithBS')
)

VertexDump = cms.Sequence(vertexDump)

