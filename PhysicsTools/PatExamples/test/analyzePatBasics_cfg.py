import FWCore.ParameterSet.Config as cms

process = cms.Process("Test")

process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring(
    'file:PATLayer1_Output.fromAOD_full.root'
  )
)

process.MessageLogger = cms.Service("MessageLogger")

process.analyzeBasicPat = cms.EDFilter("PatBasicAnalyzer",
  photonSrc   = cms.untracked.InputTag("cleanLayer1Photons"),
  electronSrc = cms.untracked.InputTag("cleanLayer1Electrons"),
  muonSrc     = cms.untracked.InputTag("cleanLayer1Muons"),                                             
  tauSrc      = cms.untracked.InputTag("cleanLayer1Taus"),
  jetSrc      = cms.untracked.InputTag("cleanLayer1Jets"),
  metSrc      = cms.untracked.InputTag("layer1METs")
)

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string('analyzePatBasics.root')
                                   )

process.p = cms.Path(process.analyzeBasicPat)

