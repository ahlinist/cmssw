import FWCore.ParameterSet.Config as cms

process = cms.Process("Resp")

process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.load('Configuration/StandardSequences/GeometryIdeal_cff')
process.load('Configuration/StandardSequences/MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration/StandardSequences/RawToDigi_Data_cff')
process.load("Configuration.StandardSequences.ReconstructionCosmics_cff")
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.load('Configuration/EventContent/EventContentCosmics_cff')

process.GlobalTag.globaltag = "CRAFT09_R_V4::All"


process.maxEvents = cms.untracked.PSet(
  input = cms.untracked.int32(53)
)
process.source = cms.Source("PoolSource",
 fileNames = cms.untracked.vstring(

'/store/data/CRAFT09/Cosmics/RAW-RECO/TrackingPointing-CRAFT09_R_V4_CosmicsSeq_v1/0008/22996672-DBB9-DE11-A745-001731AF68B1.root'
)
)

process.MessageLogger.cerr.INFO.limit = 1000000
process.MessageLogger.cerr.noTimeStamps = True


process.TFileService = cms.Service("TFileService",
  fileName = cms.string('CRAFT09_Bon_official.root')
)

process.prom = cms.EDFilter("HcalProm",
  baseHtmlDir = cms.untracked.string('.'),
  printPromptHTML = cms.untracked.bool(True),
  printRecHitHTML = cms.untracked.bool(True),
  printJetMetHTML = cms.untracked.bool(True),
  printCaloTowerHTML = cms.untracked.bool(False),
  printDigiHTML = cms.untracked.bool(False),
  printHPDNoiseHTML = cms.untracked.bool(False),
  printMuonHTML = cms.untracked.bool(True)
)

#process.p = cms.Path(process.hcalDigis * process.hbhereco * process.prom)
process.p = cms.Path(process.prom)
