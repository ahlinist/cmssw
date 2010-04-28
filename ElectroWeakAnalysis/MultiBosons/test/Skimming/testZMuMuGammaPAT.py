import FWCore.ParameterSet.Config as cms

process = cms.Process("ANALYSIS")

## Message logger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )

process.source = cms.Source("PoolSource",
     fileNames = cms.untracked.vstring(
      'file:testZMuMuGammaSubskim.root'
    )
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.TFileService = cms.Service("TFileService",
  fileName = cms.string("histo_test.root")
)


process.countVgMuons = cms.EDAnalyzer("CandViewCountAnalyzer",
  src = cms.untracked.InputTag("vgMuons")
)

process.countDimuons = cms.EDAnalyzer("CandViewCountAnalyzer",
  src = cms.untracked.InputTag("dimuons")
)

process.countPatMuons = cms.EDAnalyzer("CandViewCountAnalyzer",
  src = cms.untracked.InputTag("selectedPatMuons")
)

process.eca = cms.EDAnalyzer("EventContentAnalyzer")

process.p = cms.Path(
#    process.eca +
    process.countVgMuons +
    process.countDimuons +
    process.countPatMuons
)
