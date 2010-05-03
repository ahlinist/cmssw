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

process.counts = cms.EDAnalyzer("CandViewCountAnalyzer",
  nbins = cms.untracked.uint32(10),
  histograms = cms.untracked.VPSet(
    cms.PSet( src = cms.untracked.InputTag("vgMuons") ),
    cms.PSet( src = cms.untracked.InputTag("vgMuonsGlobal") ),
    cms.PSet( src = cms.untracked.InputTag("dimuons") ),
    cms.PSet( src = cms.untracked.InputTag("dimuonsGlobal") ),
    cms.PSet( src = cms.untracked.InputTag("cleanPatMuons") ),
    cms.PSet( src = cms.untracked.InputTag("selectedPatMuonsTriggerMatch") ),
  )
)

process.load("ElectroWeakAnalysis.MultiBosons.Histogramming.mmgHistoSequences_cff")

process.p = cms.Path(
  process.counts + process.mmgHistoSequence
)
