import FWCore.ParameterSet.Config as cms

process = cms.Process('EWKTauMuon')

process.load("ElectroWeakAnalysis.EWKTau.plotterForMuTau_cfi")

process.DQMStore = cms.Service("DQMStore")

process.maxEvents = cms.untracked.PSet(            
    input = cms.untracked.int32(0)         
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('/store/relval/CMSSW_2_1_7/RelValZMM/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/12F2BEA9-F97D-DD11-8262-000423D9890C.root')
)

process.p = cms.Path( process.plotMuTau )
