import FWCore.ParameterSet.Config as cms

process = cms.Process("HcalZS")

process.load("EventFilter.HcalRawToDigi.HcalRawToDigi_cfi")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)

#-----------------------------
# Hcal Conditions: from Global Conditions Tag 
#-----------------------------
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.connect = 'frontier://Frontier/CMS_COND_21X_GLOBALTAG'
#process.GlobalTag.globaltag = 'CRUZET4_V5P::All' # or any other appropriate
process.GlobalTag.globaltag = 'CRAFT_V4P::All' # or any other appropriate
process.prefer("GlobalTag")


process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.default.limit = 10
process.MessageLogger.cerr.FwkReport.reportEvery = 10


process.source = cms.Source("PoolSource",
    dropMetaData = cms.untracked.bool(True),
    fileNames = cms.untracked.vstring(
    #'/store/data/Commissioning08/Cosmics/RAW/v1/000/069/261/08C24831-AAAA-DD11-B6BD-001617E30D38.root'
    '/store/data/Commissioning08/Cosmics/RAW/v1/000/068/500/102A8454-ECA8-DD11-AA4C-000423D98DC4.root' 
    #'/store/data/Commissioning08/Cosmics/RAW/v1/000/065/947/68153D94-A699-DD11-A1E0-000423D9970C.root' #ZS
    #'/store/data/Commissioning08/Cosmics/RAW/v1/000/064/103/2A983512-E18F-DD11-BE84-001617E30CA4.root' #ZS
    #'/store/data/Commissioning08/Cosmics/RAW/v1/000/066/180/4C2E5A77-3A9A-DD11-9F6D-001617DBD224.root' #ZS
    )
)

#process.HcalZSDigis = cms.EDFilter("HcalRealisticZS",
#digiLabel = cms.InputTag("simHcalUnsuppressedDigis"),
#digiLabel = cms.InputTag("hcalDigis"),
#mode = cms.int32(0),
#HBlevel = cms.int32(8),
#HElevel = cms.int32(8),
#HOlevel = cms.int32(8),
#HFlevel = cms.int32(9)
#)

process.hcalZS = cms.EDAnalyzer ("HcalZS",
                                 thresholdHB = cms.untracked.int32(8),
                                 thresholdHE = cms.untracked.int32(8),
                                 thresholdHO = cms.untracked.int32(8),
                                 thresholdHF = cms.untracked.int32(8)
                                 )

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('histHcalZS.root')
)

#process.p1 = cms.Path(process.hcalDigis*process.HcalZSDigis*process.hcalZS)
process.p1 = cms.Path(process.hcalDigis*process.hcalZS)

