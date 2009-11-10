import FWCore.ParameterSet.Config as cms

process = cms.Process("TEST")

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
       "file:taste.root"
       )
    )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.load("RecoTauTag.TauAnalysisTools.fakeRate.dijetTagAndProbe_cfi")

process.path = cms.Path(process.dijetFakeRateNtupleSequence)

process.out = cms.OutputModule("PoolOutputModule",                                                                                                                                                        
      outputCommands = cms.untracked.vstring("drop *", "keep *_*NtupleProducer_*_*" ),
      verbose = cms.untracked.bool(False),
      fileName = cms.untracked.string("dijet_ntuple.root")      
      )

# Get trigger report
process.options = cms.untracked.PSet(
        wantSummary = cms.untracked.bool(True)
        )

process.endpath = cms.EndPath(process.out)
