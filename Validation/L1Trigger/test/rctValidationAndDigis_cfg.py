import FWCore.ParameterSet.Config as cms

process = cms.Process("RCTVAL")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load("DQMServices.Core.DQM_cfg")

process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'GR09_R_34X_V4::All'

# UNCOMMENT THIS LINE TO RUN ON SETTINGS FROM THE DATABASE
process.es_prefer_GlobalTag = cms.ESPrefer('PoolDBESSource', 'GlobalTag')

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
       '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/SD_AllMinBias-Dec19thSkim_341_v1/0006/D6AF3A59-D6ED-DE11-9C9A-002618943950.root',
               '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/SD_AllMinBias-Dec19thSkim_341_v1/0006/D6632DA7-DCED-DE11-BCD7-003048679012.root',
               '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/SD_AllMinBias-Dec19thSkim_341_v1/0006/CA0C2CBF-C9ED-DE11-B474-003048678B36.root',
               '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/SD_AllMinBias-Dec19thSkim_341_v1/0006/C8716589-A3EE-DE11-8B5C-001A92810ADE.root',
               '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/SD_AllMinBias-Dec19thSkim_341_v1/0006/B469F519-D2ED-DE11-BB87-003048679012.root',
               '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/SD_AllMinBias-Dec19thSkim_341_v1/0006/AAC6BB72-D8ED-DE11-AE41-00304867906C.root',
               '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/SD_AllMinBias-Dec19thSkim_341_v1/0006/AA504CE0-2CEE-DE11-95F7-002618943945.root',
               '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/SD_AllMinBias-Dec19thSkim_341_v1/0006/A2119371-D8ED-DE11-8B62-002618943950.root',
               '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/SD_AllMinBias-Dec19thSkim_341_v1/0006/986C69A6-C7ED-DE11-A9A8-003048678B36.root',
               '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/SD_AllMinBias-Dec19thSkim_341_v1/0006/92EF9E3C-D4ED-DE11-9624-002618943950.root',
               '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/SD_AllMinBias-Dec19thSkim_341_v1/0006/928D9FA4-DCED-DE11-A219-002618943945.root',
               '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/SD_AllMinBias-Dec19thSkim_341_v1/0006/6E4D22DB-DCED-DE11-BBAB-002618943950.root',
               '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/SD_AllMinBias-Dec19thSkim_341_v1/0006/6A1EED90-DAED-DE11-B91B-002618FDA28E.root',
               '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/SD_AllMinBias-Dec19thSkim_341_v1/0006/56E87FB9-31EE-DE11-AC9D-002618943945.root',
               '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/SD_AllMinBias-Dec19thSkim_341_v1/0006/544E5A1E-D0ED-DE11-9039-0026189438CB.root',
               '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/SD_AllMinBias-Dec19thSkim_341_v1/0006/4AAB8673-D8ED-DE11-ACC8-0030486792B6.root',
               '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/SD_AllMinBias-Dec19thSkim_341_v1/0006/3428C30C-96EE-DE11-9D48-003048678FFA.root',
               '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/SD_AllMinBias-Dec19thSkim_341_v1/0006/285D7FE0-D8ED-DE11-BD55-002618943950.root',
               '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/SD_AllMinBias-Dec19thSkim_341_v1/0006/26E9695A-D6ED-DE11-A8FD-003048678FE0.root',
               '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/SD_AllMinBias-Dec19thSkim_341_v1/0005/FEB60CA7-B9ED-DE11-986C-00261894389F.root',
               '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/SD_AllMinBias-Dec19thSkim_341_v1/0005/FE5B5843-AAED-DE11-A9B2-0026189438F2.root',
               '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/SD_AllMinBias-Dec19thSkim_341_v1/0005/FC1F8B49-AAED-DE11-A66B-0018F3D09630.root',
               '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/SD_AllMinBias-Dec19thSkim_341_v1/0005/FA8F22B1-A1ED-DE11-AD6C-0026189438E8.root',
               '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/SD_AllMinBias-Dec19thSkim_341_v1/0005/FA862491-B9ED-DE11-94D0-00261894393C.root',
               '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/SD_AllMinBias-Dec19thSkim_341_v1/0005/FA2D589D-B9ED-DE11-995F-00261894390B.root',
               '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/SD_AllMinBias-Dec19thSkim_341_v1/0005/F8FD9DD9-A5ED-DE11-AF2C-0030486791AA.root',
               '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/SD_AllMinBias-Dec19thSkim_341_v1/0005/F84513C2-B9ED-DE11-BD44-0026189438D8.root',
               '/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/SD_AllMinBias-Dec19thSkim_341_v1/0005/F28B7E28-A8ED-DE11-82CE-00304867901A.root'
    )
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.load("DQMServices.Core.DQM_cfg")
process.load("DQMServices.Components.DQMEnvironment_cfi")


#Reconfigure Environment and saver
process.dqmEnv.subSystemFolder = cms.untracked.string('HLT/HLTTAU')
process.DQM.collectorPort = 9091
process.DQM.collectorHost = cms.untracked.string('pcwiscms10')

process.dqmSaver.saveByRun = cms.untracked.int32(-1)
process.dqmSaver.saveAtJobEnd = cms.untracked.bool(True)
process.dqmSaver.workflow = cms.untracked.string('/A/N/C')
process.dqmSaver.forceRunNumber = cms.untracked.int32(123)


process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange(
        '123596:2-123596:9999',   # OK
            '123615:70-123615:9999',  # OK
            '123732:62-123732:9999',  # 62 -9999 (Pixel off in 56-61)
            '123815:8-123815:9999',   # 8 - 9999 ( why not 7 ?)
            '123818:2-123818:42',     # OK
            '123908:2-123908:12',     # 2 - 12 (why not 13 ?)
            '124008:1-124008:1',      # OK
            '124009:1-124009:68',     # OK
            '124020:12-124020:94',    # OK
            '124022:66-124022:179',   # OK
            '124023:38-124023:9999',  # OK
            '124024:2-124024:83',     # OK
            '124025:5-124025:13',     # 5 - 13 (why not 3 & 4 ?)
            '124027:24-124027:9999',  # OK
            '124030:2-124030:9999',   # 2 - 9999 ( why not 1 ?)
            '124120:1-124120:9999',   # OK
            '124275:3-124275:30'
            )

# This is for filtering on L1 technical trigger bit: MB and no beam halo
process.load('L1TriggerConfig.L1GtConfigProducers.L1GtTriggerMaskTechTrigConfig_cff')
process.load('HLTrigger/HLTfilters/hltLevel1GTSeed_cfi')
process.hltLevel1GTSeed.L1TechTriggerSeeding = cms.bool(True)
process.hltLevel1GTSeed.L1SeedsLogicalExpression = cms.string('(0 AND (40 OR 41) AND NOT (36 OR 37 OR 38 OR 39))')


#Load The SKIM
process.goodPhotons = cms.EDFilter("PhotonSelector",
                                     src = cms.InputTag("photons"),
                                     cut = cms.string('pt > 3 && abs(eta) < 3.0' ),
                                     filter = cms.bool(True)
                                     )
process.rctVal = cms.EDAnalyzer('RctValidation',
                egamma = cms.InputTag('simRctDigis'),
                genEGamma = cms.InputTag("goodPhotons"),
                directory = cms.string("L1TEMU/RCTRelVal/photons")
)                               



process.simRctDigis = cms.EDProducer("L1RCTProducer",
    hcalDigis = cms.VInputTag(cms.InputTag("hcalDigis")),
    useDebugTpgScales = cms.bool(False),
    useEcal = cms.bool(True),
    useHcal = cms.bool(True),
    ecalDigis = cms.VInputTag(cms.InputTag("ecalDigis:EcalTriggerPrimitives")),
    BunchCrossings = cms.vint32(0)                      
)



process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('L1Trigger.Configuration.L1Config_cff')
process.load('Configuration.StandardSequences.Geometry_cff')

process.validation = cms.Path(
    process.hltLevel1GTSeed*
    process.goodPhotons*
    process.RawToDigi*
    process.simRctDigis*
    process.rctVal
)



#process.hltOutput = cms.OutputModule("PoolOutputModule",
#                         fileName = cms.untracked.string('openhlt.root'),
#                         compression_level = cms.untracked.int32(1),
#                         use_compression = cms.untracked.bool(True),
#                         max_event_size = cms.untracked.int32(7000000)
#)


#process.e = cms.EndPath(process.hltOutput)
#process.schedule = cms.Schedule(process.validation,
##                                process.e                        
#)


process.postProcess  = cms.EndPath(process.dqmSaver)
process.schedule = cms.Schedule(process.validation,process.postProcess)
