import FWCore.ParameterSet.Config as cms

process = cms.Process("ANALYSIS")
process.MessageLogger = cms.Service("MessageLogger",
    cout = cms.untracked.PSet(
        default = cms.untracked.PSet(
            limit = cms.untracked.int32(0)
        )
    ),
    destinations = cms.untracked.vstring('cout')
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:/data/covarell/evtgen-validation/testMCpool.root')
)

process.Test = cms.EDAnalyzer("BsAnalyzer",
    HistOutFile = cms.untracked.string('Test.root'),
    theSrc = cms.untracked.string('evtgenproducer')
)

process.p1 = cms.Path(process.Test)

