import FWCore.ParameterSet.Config as cms

process = cms.Process("OWNPARTICLES")
process.load("Configuration.StandardSequences.FakeConditions_cff")

process.load("Configuration.StandardSequences.Geometry_cff")

process.load("L1Trigger.Configuration.L1DummyConfig_cff")

process.load("L1Trigger.Configuration.SimL1Emulator_cff")

process.load("L1Trigger.TechnicalTriggers.l1TechTrigTestDigis_cfi")

process.MessageLogger = cms.Service("MessageLogger")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('/store/relval/2008/7/20/RelVal-RelValQCD_Pt_80_120-1216579481/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/CMSSW_2_1_0_pre9-RelVal-1216579481-IDEAL_V5-unmerged/0000/0280B738-0757-DD11-9ED6-000423D99B3E.root')
)

process.Products = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('test.root')
)

process.p = cms.Path(process.l1TechTrigTestDigis*process.SimL1Emulator)
process.end = cms.EndPath(process.Products)

