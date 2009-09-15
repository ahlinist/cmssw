import FWCore.ParameterSet.Config as cms

vertexDump = cms.EDFilter("HFDumpVertex",
    verbose = cms.untracked.int32(0),
    vertexLabel = cms.untracked.string('offlinePrimaryVertices')
)

VertexDump = cms.Sequence(vertexDump)

