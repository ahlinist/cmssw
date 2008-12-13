# trivial analyzer example that prints out the HepMGProduct for each event
import FWCore.ParameterSet.Config as cms

process = cms.Process("HEPMCANALYZER")
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:MGtest.root')
)

process.analysis = cms.EDAnalyzer("HepMGProductAnalyzer",
    moduleLabel = cms.untracked.string('source')
)

process.printout = cms.EDAnalyzer("EventContentAnalyzer")

process.p = cms.Path(process.analysis)

