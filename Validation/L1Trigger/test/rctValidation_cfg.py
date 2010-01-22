import FWCore.ParameterSet.Config as cms

process = cms.Process("RCTVAL")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load("DQMServices.Core.DQM_cfg")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = cms.string('MC_3XY_V10::All')



process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(

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
