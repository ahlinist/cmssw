import FWCore.ParameterSet.Config as cms

process = cms.Process("COPY")
process.load("FWCore.MessageLogger.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10)
)
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('/store/RelVal/2007/7/10/RelVal-RelVal152Z-MM-1184071556/0000/14BFDE39-1B2F-DC11-884F-000E0C3F0521.root')
)

process.copyAll = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('mycopy.root')
)

process.printEventNumber = cms.OutputModule("AsciiOutputModule")

process.out = cms.EndPath(process.copyAll*process.printEventNumber)

