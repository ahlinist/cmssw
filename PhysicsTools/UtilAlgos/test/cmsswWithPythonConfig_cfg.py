import FWCore.ParameterSet.Config as cms

process = cms.Process("Test")

process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring(
    "/store/relval/CMSSW_4_2_0/RelValZTT/GEN-SIM-RECO/START42_V9-v1/0054/107DB9B4-7D5E-E011-91E9-001A92810AEA.root"
  )
)
### Maximal Number of Events
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )

process.MessageLogger = cms.Service("MessageLogger")

process.muonAnalyzer = cms.EDAnalyzer("WrappedEDMuonAnalyzer",
  muons = cms.InputTag("muons"),                                             
)

process.TFileService = cms.Service("TFileService",
  fileName = cms.string('analyzeCMSSWHistograms.root')
)

process.p = cms.Path(process.muonAnalyzer)

