import FWCore.ParameterSet.Config as cms

process = cms.Process("TEST")

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
       "file:taste.root"
       )
    )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.load("RecoTauTag.TauAnalysisTools.TauNtupleProducer_cfi")

from RecoTauTag.TauAnalysisTools.tools.ntupleTools import *

makeStandardTauNtuples(process)

process.path = cms.Path(process.buildTauNtuple)

process.out = cms.OutputModule("PoolOutputModule",                                                                                                                                                        
      outputCommands = cms.untracked.vstring("drop *", "keep *_*NtupleProducer_*_*" ),
      verbose = cms.untracked.bool(False),
      fileName = cms.untracked.string("taste_ntupled.root")      
      )

process.endpath = cms.EndPath(process.out)
