import FWCore.ParameterSet.Config as cms

process = cms.Process("RCTVAL")


process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'GR_R_42_V24::All' #PUT LATEST GLOBAL TAG HERE 

# UNCOMMENT THIS LINE TO RUN ON SETTINGS FROM THE DATABASE
# IF WE ARE TESTING THE RCT  WE DONT WANT THE DATABASE
#process.es_prefer_GlobalTag = cms.ESPrefer('PoolDBESSource', 'GlobalTag')


process.load("DQMServices.Core.DQM_cfg")
process.load("DQMServices.Components.DQMEnvironment_cfi")


process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
              '/store/data/Run2011B/MinimumBias/RAW-RECO/ValSkim-PromptSkim-v1/0000/00257D72-F9DC-E011-A401-00261894392C.root',
              '/store/data/Run2011B/MinimumBias/RAW-RECO/ValSkim-PromptSkim-v1/0000/002CD2CA-4BF9-E011-8377-001A92971B04.root',
              '/store/data/Run2011B/MinimumBias/RAW-RECO/ValSkim-PromptSkim-v1/0000/0086BA65-33EE-E011-9BEC-0018F3D09652.root',
              '/store/data/Run2011B/MinimumBias/RAW-RECO/ValSkim-PromptSkim-v1/0000/00A7606D-22F9-E011-8437-001A92971BDC.root',
              '/store/data/Run2011B/MinimumBias/RAW-RECO/ValSkim-PromptSkim-v1/0000/00C112D0-ABE6-E011-98B7-0018F3D096C6.root',
              '/store/data/Run2011B/MinimumBias/RAW-RECO/ValSkim-PromptSkim-v1/0000/00C632B2-04EB-E011-BD0A-0018F3D095EC.root',
              '/store/data/Run2011B/MinimumBias/RAW-RECO/ValSkim-PromptSkim-v1/0000/0203D9EF-6AEE-E011-8FC4-003048678BAA.root',
              '/store/data/Run2011B/MinimumBias/RAW-RECO/ValSkim-PromptSkim-v1/0000/02094552-C2EA-E011-8D86-0018F3D096F0.root',
              '/store/data/Run2011B/MinimumBias/RAW-RECO/ValSkim-PromptSkim-v1/0000/022787E9-0BF0-E011-88E4-00261894392F.root',
              '/store/data/Run2011B/MinimumBias/RAW-RECO/ValSkim-PromptSkim-v1/0000/02350F5F-37EB-E011-B5B9-001A92810ADC.root',
              '/store/data/Run2011B/MinimumBias/RAW-RECO/ValSkim-PromptSkim-v1/0000/02431D41-85F9-E011-89A0-001A92971AD0.root',
              '/store/data/Run2011B/MinimumBias/RAW-RECO/ValSkim-PromptSkim-v1/0000/024B3FB7-88F6-E011-B556-00304867920C.root',
              '/store/data/Run2011B/MinimumBias/RAW-RECO/ValSkim-PromptSkim-v1/0000/0268062B-C200-E111-8BDF-00248C0BE018.root',
              '/store/data/Run2011B/MinimumBias/RAW-RECO/ValSkim-PromptSkim-v1/0000/0270A6AE-0BEB-E011-A87A-001A928116B2.root',
              '/store/data/Run2011B/MinimumBias/RAW-RECO/ValSkim-PromptSkim-v1/0000/02768AE9-ABE4-E011-909D-0026189438FC.root',
              '/store/data/Run2011B/MinimumBias/RAW-RECO/ValSkim-PromptSkim-v1/0000/027B85ED-24FE-E011-8D87-001A92810AAE.root',
              '/store/data/Run2011B/MinimumBias/RAW-RECO/ValSkim-PromptSkim-v1/0000/02A38E0C-C3FB-E011-9EC1-002618943916.root',
              '/store/data/Run2011B/MinimumBias/RAW-RECO/ValSkim-PromptSkim-v1/0000/02CB1413-EFDE-E011-9075-0026189438F3.root',
              '/store/data/Run2011B/MinimumBias/RAW-RECO/ValSkim-PromptSkim-v1/0000/02CF7AFB-F4E0-E011-9999-001A92810AA4.root',
              '/store/data/Run2011B/MinimumBias/RAW-RECO/ValSkim-PromptSkim-v1/0000/02D514A9-AF00-E111-8C22-002618943948.root',
              '/store/data/Run2011B/MinimumBias/RAW-RECO/ValSkim-PromptSkim-v1/0000/02FCE968-59E6-E011-B79E-001A92810ADE.root',
              '/store/data/Run2011B/MinimumBias/RAW-RECO/ValSkim-PromptSkim-v1/0000/046DDD2A-3DEE-E011-9566-002618943983.root',
              '/store/data/Run2011B/MinimumBias/RAW-RECO/ValSkim-PromptSkim-v1/0000/0483A87A-56F9-E011-8CE2-00261894391C.root',
              '/store/data/Run2011B/MinimumBias/RAW-RECO/ValSkim-PromptSkim-v1/0000/048AD0D4-5AF3-E011-93A9-001A92971B20.root',
              '/store/data/Run2011B/MinimumBias/RAW-RECO/ValSkim-PromptSkim-v1/0000/04BEE840-7606-E111-8BA4-0018F3D0962C.root',
              '/store/data/Run2011B/MinimumBias/RAW-RECO/ValSkim-PromptSkim-v1/0000/04C14451-20E8-E011-B9CA-001A92971B0C.root',
              '/store/data/Run2011B/MinimumBias/RAW-RECO/ValSkim-PromptSkim-v1/0000/04FDD887-FDEA-E011-8BEC-00304867902E.root',
              '/store/data/Run2011B/MinimumBias/RAW-RECO/ValSkim-PromptSkim-v1/0000/061C6C80-CAE4-E011-9EE1-0018F3D09630.root',
              '/store/data/Run2011B/MinimumBias/RAW-RECO/ValSkim-PromptSkim-v1/0000/062719C5-29E3-E011-908C-0026189438D2.root',
              '/store/data/Run2011B/MinimumBias/RAW-RECO/ValSkim-PromptSkim-v1/0000/067263D5-12E1-E011-8EA4-00304867916E.root',
              '/store/data/Run2011B/MinimumBias/RAW-RECO/ValSkim-PromptSkim-v1/0000/0698E06D-3EE8-E011-AC74-002618943832.root',
              '/store/data/Run2011B/MinimumBias/RAW-RECO/ValSkim-PromptSkim-v1/0000/06AE8000-67F9-E011-BC02-001A928116C0.root',
              '/store/data/Run2011B/MinimumBias/RAW-RECO/ValSkim-PromptSkim-v1/0000/0836B47B-9DF3-E011-AD9E-0026189438D7.root',
              '/store/data/Run2011B/MinimumBias/RAW-RECO/ValSkim-PromptSkim-v1/0000/0854E007-BA05-E111-B877-001BFCDBD11E.root',
              '/store/data/Run2011B/MinimumBias/RAW-RECO/ValSkim-PromptSkim-v1/0000/085D23ED-34EB-E011-ACAF-002618943896.root',
              '/store/data/Run2011B/MinimumBias/RAW-RECO/ValSkim-PromptSkim-v1/0000/0865F4F7-2DE6-E011-AC95-002618943894.root'
    )
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )


process.selectedPhotons = cms.EDFilter("PhotonSelector",
   src = cms.InputTag("photons"),
   cut = cms.string('pt>1.0&&hadronicOverEm()<0.05&&ecalRecHitSumEtConeDR03()<3&&maxEnergyXtal()/e3x3()<0.99&&r2x5>.9'),
   filter = cms.bool(True)
)                                       


process.highestPhoton = cms.EDFilter("LargestPtCandViewSelector",
                           src = cms.InputTag("selectedPhotons"),
                           maxNumber = cms.uint32(1)
                         )
#gsfB12
process.rctVal = cms.EDAnalyzer('RctValidation',
                ecalTPGs = cms.InputTag("ecalDigis:EcalTriggerPrimitives"),
                hcalTPGs = cms.InputTag("hcalDigis"),
                rctEGamma = cms.InputTag('rctDigis'),
                gctEGamma = cms.VInputTag(cms.InputTag('gctDigis','isoEm'),
                                          cms.InputTag('gctDigis','nonIsoEm')
                ),                          
                genEGamma = cms.InputTag("selectedPhotons"), ##PUT YOUR REF OBJECTS HERE put electron cuts and stuff here put in h/e
                directory = cms.string("L1T/RCTPhotons/test"),
                outputFileName = cms.untracked.string("analysis.root"),
                maxEt = cms.untracked.double(40),
                binsEt = cms.untracked.int32(40),
                gammaThreshold = cms.untracked.double(2.)              
)                               



process.rctDigis = cms.EDProducer("L1RCTProducer",
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
    process.selectedPhotons*
    process.RawToDigi*
    process.rctDigis*
    process.rctVal
)


