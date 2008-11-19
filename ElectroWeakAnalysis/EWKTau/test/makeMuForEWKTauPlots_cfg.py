import FWCore.ParameterSet.Config as cms

process = cms.Process('EWKTauMuon')

process.load("ElectroWeakAnalysis.EWKTau.plotterForMuTau_cff")

process.DQMStore = cms.Service("DQMStore")

process.maxEvents = cms.untracked.PSet(            
    input = cms.untracked.int32(0)         
)

process.source = cms.Source("EmptySource")

process.p = cms.Path( process.loadMuTau
                     +process.addMuTau
                     +process.saveMuTau 
                     +process.plotMuTau )
