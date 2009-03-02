import FWCore.ParameterSet.Config as cms

process = cms.Process('makeZtoMuTauPlots')

process.load("TauAnalysis.Configuration.plotZtoMuTau_cff")

process.DQMStore = cms.Service("DQMStore")

process.maxEvents = cms.untracked.PSet(            
  input = cms.untracked.int32(0)         
)

process.source = cms.Source("EmptySource")

process.p = cms.Path( process.loadZtoMuTau
                     +process.addZtoMuTau
                     +process.saveZtoMuTau 
                     +process.plotZtoMuTau )
