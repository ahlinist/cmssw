import FWCore.ParameterSet.Config as cms

process = cms.Process("RP")
process.load("Configuration.StandardSequences.Services_cff")

process.load("Configuration.StandardSequences.Geometry_cff")

process.load("Configuration.StandardSequences.MagneticField_cff")

process.load("Configuration.StandardSequences.FakeConditions_cff")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.load("Configuration.StandardSequences.RawToDigi_cff")

process.load("Configuration.StandardSequences.Reconstruction_cff")

process.load("Configuration.EventContent.EventContent_cff")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)
process.source = cms.Source("PoolSource",
    catalog = cms.untracked.string('PoolFileCatalog.xml'),
    fileNames = cms.untracked.vstring('file:digi.root')
)

process.RECO = cms.OutputModule("PoolOutputModule",
    process.RECOSIMEventContent,
    fileName = cms.untracked.string('reco.root')
)

process.p1 = cms.Path(process.RawToDigi*process.reconstruction)
process.outpath = cms.EndPath(process.RECO)

