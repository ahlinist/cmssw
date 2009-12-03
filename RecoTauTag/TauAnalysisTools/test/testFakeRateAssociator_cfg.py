import FWCore.ParameterSet.Config as cms

process = cms.Process("TEST")

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
       '/store/relval/CMSSW_3_3_1/RelValZTT/GEN-SIM-RECO/STARTUP31X_V8-v1/0002/601C9604-FDBF-DE11-86FE-000423D98B08.root',
       '/store/relval/CMSSW_3_3_1/RelValZTT/GEN-SIM-RECO/STARTUP31X_V8-v1/0002/36D18EC1-E9BF-DE11-ABF7-000423D987E0.root',
       '/store/relval/CMSSW_3_3_1/RelValZTT/GEN-SIM-RECO/STARTUP31X_V8-v1/0001/DC6E6BC4-C6BF-DE11-870C-0030487A3232.root',
       '/store/relval/CMSSW_3_3_1/RelValZTT/GEN-SIM-RECO/STARTUP31X_V8-v1/0001/D29BAA92-C8BF-DE11-91DF-001617C3B5F4.root',
       '/store/relval/CMSSW_3_3_1/RelValZTT/GEN-SIM-RECO/STARTUP31X_V8-v1/0001/A2582D1C-D0BF-DE11-8DF5-000423D98B08.root',
       '/store/relval/CMSSW_3_3_1/RelValZTT/GEN-SIM-RECO/STARTUP31X_V8-v1/0001/6818CD6D-C7BF-DE11-A25F-003048D2C1C4.root'  
       )
    )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.load("RecoTauTag.TauAnalysisTools.PFTauEfficiencyAssociator_cfi")

process.path = cms.Path(process.shrinkingConeMuEnrichedQCDAssociator
      			*process.shrinkingConeZTTEffSimAssociator)


