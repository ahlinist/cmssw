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
    '/store/data/Run2011A/MinimumBias/RAW-RECO/ValSkim-08Nov2011-v1/0000/006A22F4-190F-E111-9A37-001A92810AB6.root'    
    )
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )



process.rctVal = cms.EDAnalyzer('RctValidation',
                ecalTPGs = cms.InputTag("ecalDigis:EcalTriggerPrimitives"),
                hcalTPGs = cms.InputTag("hcalDigis"),
                rctEGamma = cms.InputTag('gctDigis'),
                gctEGamma = cms.VInputTag(cms.InputTag('gctDigis','isoEm'),
                                          cms.InputTag('gctDigis','nonIsoEm')
                ),                          
                genEGamma = cms.InputTag("crap"), ##PUT YOUR REF OBJECTS HERE
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
    process.RawToDigi*
    process.rctDigis*
    process.rctVal
)


