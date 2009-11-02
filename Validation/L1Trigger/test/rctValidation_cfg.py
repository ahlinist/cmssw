import FWCore.ParameterSet.Config as cms

process = cms.Process("RCTVAL")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load("DQMServices.Core.DQM_cfg")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = cms.string('MC_3XY_V10::All')



process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        '/store/relval/CMSSW_3_4_0_pre2/RelValZEE/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_3XY_V10-v1/0003/F0A3B9CE-95BD-DE11-8E2C-003048679294.root',
        '/store/relval/CMSSW_3_4_0_pre2/RelValZEE/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_3XY_V10-v1/0003/DE3EB3A8-C0BD-DE11-9156-003048679084.root',
        '/store/relval/CMSSW_3_4_0_pre2/RelValZEE/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_3XY_V10-v1/0003/BCB2A176-8FBD-DE11-92A9-002618943826.root',
        '/store/relval/CMSSW_3_4_0_pre2/RelValZEE/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_3XY_V10-v1/0003/8023DEEF-90BD-DE11-AE6F-001731A283E1.root',
        '/store/relval/CMSSW_3_4_0_pre2/RelValZEE/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_3XY_V10-v1/0003/7AA2A890-98BD-DE11-A586-001731AF6A87.root',
        '/store/relval/CMSSW_3_4_0_pre2/RelValZEE/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_3XY_V10-v1/0003/3C5AEDEF-8EBD-DE11-A0BE-00261894396B.root',
        '/store/relval/CMSSW_3_4_0_pre2/RelValZEE/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_3XY_V10-v1/0003/0EB64073-8FBD-DE11-BEC7-002618943838.root'
    )
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.load("DQMServices.Core.DQM_cfg")
process.load("DQMServices.Components.DQMEnvironment_cfi")


#Reconfigure Environment and saver
#process.dqmEnv.subSystemFolder = cms.untracked.string('HLT/HLTTAU')
#process.DQM.collectorPort = 9091
#process.DQM.collectorHost = cms.untracked.string('pcwiscms10')

process.dqmSaver.saveByRun = cms.untracked.int32(-1)
process.dqmSaver.saveAtJobEnd = cms.untracked.bool(True)
process.dqmSaver.workflow = cms.untracked.string('/A/N/C')
process.dqmSaver.forceRunNumber = cms.untracked.int32(123)

process.load("Validation.L1Trigger.rctValidation_cff")

process.validation = cms.Path(
    process.rctValidation
)

process.postProcess  = cms.EndPath(process.rctHarvesting+
                                   process.rctQualityTests+
                                   process.dqmSaver)

process.schedule = cms.Schedule(process.validation,process.postProcess)
