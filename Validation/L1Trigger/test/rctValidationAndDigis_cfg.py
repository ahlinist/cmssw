import FWCore.ParameterSet.Config as cms

process = cms.Process("RCTVAL")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load("DQMServices.Core.DQM_cfg")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = cms.string('STARTUP3X_V15::All')



process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        '/store/relval/CMSSW_3_5_0_pre3/RelValZEE/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP3X_V15-v1/0006/EC614F61-D103-DF11-9D4C-0030487CD6DE.root',
                '/store/relval/CMSSW_3_5_0_pre3/RelValZEE/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP3X_V15-v1/0005/F20AD303-4803-DF11-80BD-0030487C8E02.root',
                '/store/relval/CMSSW_3_5_0_pre3/RelValZEE/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP3X_V15-v1/0005/DC0CB3C8-4803-DF11-8C4C-0030487C6A66.root',
                '/store/relval/CMSSW_3_5_0_pre3/RelValZEE/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP3X_V15-v1/0005/CED8BD33-4703-DF11-840A-0030487C7392.root',
                '/store/relval/CMSSW_3_5_0_pre3/RelValZEE/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP3X_V15-v1/0005/BC4D6570-4603-DF11-85DC-0030487CD6DA.root',
                '/store/relval/CMSSW_3_5_0_pre3/RelValZEE/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP3X_V15-v1/0005/9AB25F01-4B03-DF11-99CF-0030487D05B0.root',
                '/store/relval/CMSSW_3_5_0_pre3/RelValZEE/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP3X_V15-v1/0005/5EF1D819-4D03-DF11-9CDB-0030487A322E.root',
                '/store/relval/CMSSW_3_5_0_pre3/RelValZEE/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP3X_V15-v1/0005/38FB0ADE-4F03-DF11-BBF3-0030487CD180.root',
                '/store/relval/CMSSW_3_5_0_pre3/RelValZEE/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP3X_V15-v1/0005/36C4E317-4903-DF11-AA98-0030487C635A.root',
                '/store/relval/CMSSW_3_5_0_pre3/RelValZEE/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP3X_V15-v1/0005/2EF278FD-4803-DF11-A275-0030487CAEAC.root',
                '/store/relval/CMSSW_3_5_0_pre3/RelValZEE/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP3X_V15-v1/0005/226EED6C-4503-DF11-AAB7-0030487CD710.root',
                '/store/relval/CMSSW_3_5_0_pre3/RelValZEE/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP3X_V15-v1/0005/0A6F875B-4703-DF11-9106-0030487CD812.root',
                '/store/relval/CMSSW_3_5_0_pre3/RelValZEE/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP3X_V15-v1/0005/025950C9-4703-DF11-B7E5-0030487C6088.root'
        
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


process.simRctDigis = cms.EDProducer("L1RCTProducer",
    hcalDigis = cms.VInputTag(cms.InputTag("simHcalTriggerPrimitiveDigis")),
    useDebugTpgScales = cms.bool(False),
    useEcal = cms.bool(True),
    useHcal = cms.bool(True),
    ecalDigis = cms.VInputTag(cms.InputTag("simEcalTriggerPrimitiveDigis")),
    BunchCrossings = cms.vint32(0)                      
)


process.validation = cms.Path(
    process.simRctDigis*
    process.rctValidation
)




process.postProcess  = cms.EndPath(process.rctHarvesting+
                                   process.rctQualityTests+
                                   process.dqmSaver)

process.schedule = cms.Schedule(process.validation,process.postProcess)
