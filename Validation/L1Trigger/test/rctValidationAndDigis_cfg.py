import FWCore.ParameterSet.Config as cms

process = cms.Process("RCTVAL")


process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'GR_R_42_V24::All' #PUT LATEST GLOBAL TAG HERE 

# UNCOMMENT THIS LINE TO RUN ON SETTINGS FROM THE DATABASE
# IF WE ARE TESTING THE RCT  WE DONT WANT THE DATABASE
#process.es_prefer_GlobalTag = cms.ESPrefer('PoolDBESSource', 'GlobalTag')

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
    '/store/data/Run2011A/MinimumBias/RAW-RECO/ValSkim-08Nov2011-v1/0000/006A22F4-190F-E111-9A37-001A92810AB6.root'    
    )
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )



process.rctVal = cms.EDAnalyzer('RctValidation',
                egamma = cms.InputTag('rctDigis'),
                genEGamma = cms.InputTag("crap"), ##IF YOU DO EFFICIENCY PUT HERE THE GOOD OBJECTS
                directory = cms.string("L1TEMU/RCTRelVal/photons")
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


