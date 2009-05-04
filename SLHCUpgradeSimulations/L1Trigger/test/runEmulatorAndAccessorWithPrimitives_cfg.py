import FWCore.ParameterSet.Config as cms

process = cms.Process("L1EMU")

# Load Scales
process.load("L1TriggerConfig.L1ScalesProducers.L1CaloInputScalesConfig_cff")
process.load("L1TriggerConfig.L1ScalesProducers.L1CaloScalesConfig_cff")

# Load Emulator and Accessor
process.load("SLHCUpgradeSimulations.L1Trigger.L1SLHCEmulator_cff")
process.load("SLHCUpgradeSimulations.L1Trigger.SLHCCaloTriggerAccessor_cfi")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.destinations = cms.untracked.vstring("clusteringPatterns")
#process.MessageLogger.clusteringPatterns = cms.untracked.PSet(
#    threshold = cms.untracked.string("INFO")
#    )

process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring(
    "dcap://cmsdcap.hep.wisc.edu:22125//pnfs/hep.wisc.edu/store/user/kflood/EWK_Ztautau_14TeV_2E34B_Run1-Merged/Merged-merge1E34-EWK_Ztautau_14TeV_2E34B_Run1-0068.root"
  )
)

process.load("FastSimulation.Configuration.CommonInputs_cff")
process.load("Configuration.StandardSequences.GeometryPilot2_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'IDEAL_V9::All'

# unpack raw data
process.load("Configuration.StandardSequences.RawToDigi_cff")
process.load("L1Trigger.Configuration.CaloTriggerPrimitives_cff")
process.simEcalTriggerPrimitiveDigis.Label = 'ecalDigis'
process.simHcalTriggerPrimitiveDigis.inputLabel = 'hcalDigis'


process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(20)
)

process.output = cms.OutputModule("PoolOutputModule",
    outputCommands = cms.untracked.vstring('drop *',
        'keep *_L1ExtraMaker_*_L1EMU',
        'keep *_L1CaloTowerProducer_*_L1EMU',
        'keep *_L1CaloClusterProducer_*_L1EMU',
        'keep *_L1JetProducer_*_L1EMU',
        'keep *_L1CaloClusterFilter_*_L1EMU'),

    fileName = cms.untracked.string('L1output.root')
)

process.path = cms.Path(process.ecalDigis+process.hcalDigis+process.CaloTriggerPrimitives+process.L1SLHCEmulator+process.CaloTriggerAccessor)
process.end = cms.EndPath(process.output)
