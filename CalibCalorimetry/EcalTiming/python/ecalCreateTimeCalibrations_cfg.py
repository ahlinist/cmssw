import FWCore.ParameterSet.Config as cms

process = cms.Process("EcalCreateTimeCalibrations")

# Global Tag -- for original timing calibrations
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'GR_R_43_V3::All'

# Take Offset record from 43X DB -- no longer needed (it's included in the above)
#process.GlobalTag.toGet = cms.VPSet(
#  cms.PSet(record = cms.string("EcalTimeOffsetConstantRcd"),
#           tag = cms.string("EcalTimeOffsetConstant_v01_offline"),
#           connect = cms.untracked.string("frontier://FrontierProd/CMS_COND_43X_ECAL")
#          )
#)


process.MessageLogger = cms.Service("MessageLogger",
    cout = cms.untracked.PSet(threshold = cms.untracked.string('INFO')),
    categories = cms.untracked.vstring('*'),
    destinations = cms.untracked.vstring('cout')
)

process.TFileService = cms.Service("TFileService", 
    fileName = cms.string("ecalCreateTimeCalibs.root"),
    closeFileFast = cms.untracked.bool(True)
    )


process.createTimeCalibs = cms.EDAnalyzer("EcalCreateTimeCalibrations",
  FileNameStart = cms.string("ecalCreateTimeCalibs"),
  ZeroGlobalOffset = cms.bool(False),
  SubtractDBcalibs = cms.bool(True),
  BxIncludeExclude = cms.string("-1"),
  OrbitIncludeExclude = cms.string("-1"),
  TriggerBitIncludeExclude = cms.string("-1"),
  TechTriggerBitIncludeExclude = cms.string("-1"),
  LumiIncludeExclude = cms.string("-1"),
  RunIncludeExclude = cms.string("-1"),
  AvgTimeMin = cms.double(-1),
  AvgTimeMax = cms.double(1),
  MinHitAmpEB = cms.double(15),
  MinHitAmpEE = cms.double(15),
  MaxSwissCross = cms.double(0.95),
  MaxHitTimeEB = cms.double(5),
  MinHitTimeEB = cms.double(-5),
  MaxHitTimeEE = cms.double(5),
  MinHitTimeEE = cms.double(-5),
  InputFileNames = cms.vstring(
         'file:/data/franzoni/data/Ecaltime/skimming-jun17/rhQuinq-SingleEletron-Run2011A-WElectron-PromptSkim-jun16_HADDED.root'
    )
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )
process.source = cms.Source("EmptySource",
       numberEventsInRun = cms.untracked.uint32(1),
       firstRun = cms.untracked.uint32(888888) # Use last IOV
)


process.p = cms.Path(process.createTimeCalibs)
