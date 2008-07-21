import FWCore.ParameterSet.Config as cms

process = cms.Process("OWNPARTICLES")
process.load("L1Trigger.TechnicalTriggers.l1TechTrigTestDigis_cfi")

process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration/StandardSequences.FakeConditions_cff")

process.load("L1Trigger.Configuration.L1DummyConfig_cff")

process.load("L1Trigger.Configuration.SimL1Emulator_cff")

process.MessageLogger = cms.Service("MessageLogger")

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:TTbar_cfi_GEN_SIM_DIGI.root')
)

process.Products = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('example.root')
)

process.p = cms.Path(process.l1TechTrigTestDigis*process.SimL1Emulator)
process.end = cms.EndPath(process.Products)

