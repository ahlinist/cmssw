import FWCore.ParameterSet.Config as cms

process = cms.Process("ANA")

# Print every message
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1

# Load RAW data (or RECO)
process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
    '/store/data/Commissioning11/MinimumBias/RAW/v1/000/156/582/B48F54A6-7A37-E011-8835-000423D996C8.root'
    '/store/data/Commissioning11/MinimumBias/RAW/v1/000/156/582/AABC87A0-7A37-E011-A608-0030487CD7E0.root'
    '/store/data/Commissioning11/MinimumBias/RAW/v1/000/156/582/2417A2A2-7A37-E011-9AA1-001D09F28F1B.root'
    )
                            
                            )

# Process 100 events
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100))

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'GR_R_311_V1::All'

#------------------------------------------------------------
# If using RAW data, do Hcal RECO
#------------------------------------------------------------

process.load("Configuration.StandardSequences.Geometry_cff")
process.load("RecoJets.Configuration.CaloTowersES_cfi")
process.load("Configuration.StandardSequences.RawToDigi_Data_cff")
process.load("RecoLocalCalo.Configuration.hcalLocalReco_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'GR_R_311_V1::All'


# Analyzer
process.ana = cms.EDAnalyzer("HcalPFGRecHitAna")

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('hcal_rechits_r156582.root')
)


process.p       = cms.Path(process.hcalDigis * process.hbheprereco  * process.horeco * process.hfreco * process.ana)

