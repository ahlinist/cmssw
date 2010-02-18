import FWCore.ParameterSet.Config as cms

process = cms.Process("RCTVAL")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load("DQMServices.Core.DQM_cfg")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = cms.string('START3X_V21::All')



process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        '/store/relval/CMSSW_3_5_0/RelValZEE/GEN-SIM-DIGI-RAW-HLTDEBUG/START3X_V21-v1/0013/D42B6EF5-4313-DF11-B66E-003048D15DDA.root',
                '/store/relval/CMSSW_3_5_0/RelValZEE/GEN-SIM-DIGI-RAW-HLTDEBUG/START3X_V21-v1/0013/AE31EBF1-4413-DF11-8C53-0018F3D09688.root',
                '/store/relval/CMSSW_3_5_0/RelValZEE/GEN-SIM-DIGI-RAW-HLTDEBUG/START3X_V21-v1/0013/A2E8AFBC-4113-DF11-853F-0017313F02E2.root',
                '/store/relval/CMSSW_3_5_0/RelValZEE/GEN-SIM-DIGI-RAW-HLTDEBUG/START3X_V21-v1/0013/9EA41A83-4413-DF11-91F5-001A92971BC8.root',
                '/store/relval/CMSSW_3_5_0/RelValZEE/GEN-SIM-DIGI-RAW-HLTDEBUG/START3X_V21-v1/0013/98C368EB-4213-DF11-BAF5-001A92971B32.root',
                '/store/relval/CMSSW_3_5_0/RelValZEE/GEN-SIM-DIGI-RAW-HLTDEBUG/START3X_V21-v1/0013/967754F7-4313-DF11-B2A9-003048678DD6.root',
                '/store/relval/CMSSW_3_5_0/RelValZEE/GEN-SIM-DIGI-RAW-HLTDEBUG/START3X_V21-v1/0013/785D544F-4213-DF11-BF69-0026189438F6.root',
                '/store/relval/CMSSW_3_5_0/RelValZEE/GEN-SIM-DIGI-RAW-HLTDEBUG/START3X_V21-v1/0013/5C9214A7-4313-DF11-8CAD-001731AF68AB.root',
                '/store/relval/CMSSW_3_5_0/RelValZEE/GEN-SIM-DIGI-RAW-HLTDEBUG/START3X_V21-v1/0013/584AD107-6313-DF11-A822-001731AF6997.root',
                '/store/relval/CMSSW_3_5_0/RelValZEE/GEN-SIM-DIGI-RAW-HLTDEBUG/START3X_V21-v1/0013/4AF12D9B-4313-DF11-94F9-001731A2876F.root',
        
    )
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )

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
