import FWCore.ParameterSet.Config as cms

process = cms.Process("Test")

process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring(
    '/store/user/rwolf/ttbar/patTuple_PATv2_ttbar_tauola_1.root'
  )
)

process.MessageLogger = cms.Service("MessageLogger")

process.analyzeBasicPat = cms.EDFilter("PatBasicAnalyzer",
  photonSrc   = cms.untracked.InputTag("selectedLayer1Photons"),
  electronSrc = cms.untracked.InputTag("selectedLayer1Electrons"),
  muonSrc     = cms.untracked.InputTag("selectedLayer1Muons"),                                             
  tauSrc      = cms.untracked.InputTag("selectedLayer1Taus"),
  jetSrc      = cms.untracked.InputTag("selectedLayer1Jets"),
  metSrc      = cms.untracked.InputTag("layer1METs")
)

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string('analyzePatBasics.root')
                                   )

process.p = cms.Path(process.analyzeBasicPat)

