import FWCore.ParameterSet.Config as cms

process = cms.Process("ANA")

# Print every message
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1

# Load RAW data (or RECO)
process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
    '/store/data/Run2011B/Jet/RECO/PromptReco-v1/000/175/975/EA1195E0-C3DD-E011-8D10-BCAEC518FF44.root',
    '/store/data/Run2011B/Jet/RECO/PromptReco-v1/000/175/975/D89DF3AE-E5DD-E011-B7C1-001D09F34488.root',
    '/store/data/Run2011B/Jet/RECO/PromptReco-v1/000/175/975/4828385D-DFDD-E011-8826-001D09F25041.root',
    '/store/data/Run2011B/Jet/RECO/PromptReco-v1/000/175/975/38DB41C5-ACDD-E011-A70E-001D09F241B9.root',
    '/store/data/Run2011B/Jet/RECO/PromptReco-v1/000/175/975/10C33618-FADD-E011-8FD9-001D09F253FC.root',
    '/store/data/Run2011B/Jet/RECO/PromptReco-v1/000/175/975/1088020D-D4DD-E011-BEAB-0030486780EC.root',
    '/store/data/Run2011B/Jet/RECO/PromptReco-v1/000/175/975/0A550845-F2DD-E011-8081-001D09F23944.root'
    )
                            
                            )

# Process 100 events
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10000))


#------------------------------------------------------------
# If using RAW data, do Hcal RECO
#------------------------------------------------------------

process.load("Configuration.StandardSequences.Geometry_cff")
process.load("RecoJets.Configuration.CaloTowersES_cfi")
process.load("Configuration.StandardSequences.RawToDigi_Data_cff")
process.load("RecoLocalCalo.Configuration.hcalLocalReco_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'GR_P_V22::All'


# Analyzer
process.ana = cms.EDAnalyzer("HcalPFGRecHitAna")

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('hcal_rechits_r175975.root')
)


#process.p       = cms.Path(process.hcalDigis * process.hbheprereco  * process.horeco * process.hfreco * process.ana)
process.p       = cms.Path(process.ana)

