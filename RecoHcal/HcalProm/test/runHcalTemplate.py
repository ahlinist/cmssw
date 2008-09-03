import FWCore.ParameterSet.Config as cms

process = cms.Process("HCALTemplate")
process.load("MagneticField.Engine.uniformMagneticField_cfi")
# process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("L1Trigger.Configuration.L1Config_cff")
process.load("L1TriggerConfig.L1GtConfigProducers.Luminosity.lumi1x1032.L1Menu_CRUZET200805_gr7_muon_cff")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)
process.source = cms.Source("PoolSource",
    dropMetaData = cms.untracked.bool(True),
    fileNames = cms.untracked.vstring(
    '/store/data/Commissioning08/BarrelMuon/RECO/CRUZET4_v1/000/058/600/1A31FC6D-4A71-DD11-80AF-000423D60FF6.root'
    )
)

process.l1 = cms.EDFilter("L1GTFilter",
    trigger = cms.string('L1_SingleMu3'),
    dumpTriggerTable = cms.untracked.bool (True)
)

process.hcalTemplate = cms.EDAnalyzer ("HcalTemplate")


process.TFileService = cms.Service("TFileService",
    fileName = cms.string('histHcalTemplate.root')
)

process.p1 = cms.Path(process.hcalTemplate)
process.UniformMagneticFieldESProducer.ZFieldInTesla = 0.001

