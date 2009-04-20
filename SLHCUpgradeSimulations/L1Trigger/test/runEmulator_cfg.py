import FWCore.ParameterSet.Config as cms

process = cms.Process("L1EMU")

#Load Scales
process.load("L1TriggerConfig.L1ScalesProducers.L1CaloInputScalesConfig_cff")
process.load("L1TriggerConfig.L1ScalesProducers.L1CaloScalesConfig_cff")

#Load Emulator
process.load("SLHCUpgradeSimulations.L1Trigger.L1SLHCEmulator_cff")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.destinations = cms.untracked.vstring("clusteringPatterns")
#process.MessageLogger.clusteringPatterns = cms.untracked.PSet(
#    threshold = cms.untracked.string("INFO")
#    )


process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
               '/store/relval/CMSSW_2_1_2/RelValZEE/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/IDEAL_V6_10TeV_v7/0000/1A70B818-C06E-DD11-9840-001617C3B5D8.root'

               )
)




process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(20)
)
process.output = cms.OutputModule("PoolOutputModule",
    outputCommands = cms.untracked.vstring('drop *',
        'keep *_*_*_L1EMU'),
    fileName = cms.untracked.string('L1outputEmulator.root')
)
process.path = cms.Path(process.L1SLHCEmulator)
process.end = cms.EndPath(process.output)
