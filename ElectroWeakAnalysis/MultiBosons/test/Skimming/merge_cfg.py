import FWCore.ParameterSet.Config as cms

process = cms.Process('MERGE')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring()
                            )

process.output = cms.OutputModule("PoolOutputModule",
   fileName = cms.untracked.string('merge.root')
)

process.endpath = cms.EndPath(process.output)


