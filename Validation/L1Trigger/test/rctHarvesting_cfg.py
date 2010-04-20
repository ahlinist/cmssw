import FWCore.ParameterSet.Config as cms

process = cms.Process("RCTVALH")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load("DQMServices.Core.DQM_cfg")
#process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#process.GlobalTag.globaltag = cms.string('START3X_V21::All')


process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
 $inputFileNames
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

#rocess.load("Validation.L1Trigger.rctValidation_cff")

process.EDMtoMEConverter = cms.EDAnalyzer("EDMtoMEConverter",
                                              Name = cms.untracked.string('EDMtoMEConverter'),
                                              Verbosity = cms.untracked.int32(0), # 0 provides no output
                                                                                  # 1 provides basic output
                                              Frequency = cms.untracked.int32(50),
                                              convertOnEndLumi = cms.untracked.bool(True),
                                              convertOnEndRun = cms.untracked.bool(True)
                                          )


process.validation = cms.Path(
    process.EDMtoMEConverter
)




process.postProcess  = cms.EndPath(#process.rctHarvesting+
                                   #process.rctQualityTests+
                                   process.dqmSaver)

process.schedule = cms.Schedule(process.validation,process.postProcess)
