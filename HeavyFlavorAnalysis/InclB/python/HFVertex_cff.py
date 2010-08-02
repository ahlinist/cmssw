import FWCore.ParameterSet.Config as cms

vertexDump = cms.EDAnalyzer("HFDumpVertex",
    verbose = cms.untracked.int32(0),
    vertexLabel = cms.untracked.string('offlinePrimaryVerticesWithBS')
)

VertexDump = cms.Sequence(vertexDump)

