import FWCore.ParameterSet.Config as cms

process = cms.Process("RecoMuon")
# Messages
process.load("FWCore.MessageService.MessageLogger_cfi")


process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")

process.GlobalTag.globaltag = 'IDEAL_V9::All'

process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring('/store/data/Commissioning08/Cosmics/RAW-RECO/CRAFT_ALL_V12_229_Tosca090322_ReReco_FromTrackerPointing_v1/0005/EEEB64C0-3E37-DE11-84A1-001A92971AA8.root')
                            )

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(20000)
)

process.TFileService=cms.Service('TFileService',
                                 fileName=cms.string('MyCosmicMuonPlots.root')
                                 )


process.muonAnalysis = cms.EDAnalyzer("CosmicMuonAnalyzer",
                                      MuonCollection = cms.untracked.string('muons'),
                                      )


process.p = cms.Path(process.muonAnalysis)




