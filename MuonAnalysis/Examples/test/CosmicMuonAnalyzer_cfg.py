import FWCore.ParameterSet.Config as cms

process = cms.Process("RecoMuon")
# Messages
process.load("FWCore.MessageService.MessageLogger_cfi")


process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")

process.GlobalTag.globaltag = 'IDEAL_V9::All'

process.source = cms.Source("PoolSource",
#    fileNames = cms.untracked.vstring('/store/relval/CMSSW_2_2_1/RelValH200ZZ4L/GEN-SIM-RECO/IDEAL_V9_v1/0004/B492FC1B-06C5-DD11-93B9-000423D33970.root')
                            fileNames = cms.untracked.vstring('/store/data/Commissioning08/Cosmics/RAW-RECO/CRAFT_ALL_V12_229_Tosca090322_ReReco_FromTrackerPointing_v1/0005/EEEB64C0-3E37-DE11-84A1-001A92971AA8.root')
                            #                            fileNames = cms.untracked.vstring('file:PATLayer1_Output.fromAOD_full.root')
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(5000)
)

process.TFileService=cms.Service('TFileService',
                                 fileName=cms.string('MyMuonPlots.root')
                                 )


process.muonAnalysis = cms.EDAnalyzer("CosmicMuonAnalyzer",
                                      MuonCollection = cms.untracked.string('muons'),
                                      )


process.p = cms.Path(process.muonAnalysis)




