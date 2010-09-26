import FWCore.ParameterSet.Config as cms

process = cms.Process("Test")

process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring(
    'file:patTuple.root'
  )
)

process.MessageLogger = cms.Service("MessageLogger")

process.load("PhysicsTools.PatExamples.PatZToMuMuAnalyzer_cfi")

process.TFileService = cms.Service("TFileService",
  fileName = cms.string('analyzePatZToMuMu.root')
)

process.p = cms.Path(process.analyzeZToMuMu)

