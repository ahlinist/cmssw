import FWCore.ParameterSet.Config as cms

process = cms.Process('makeZtoMuTauPlots')

process.load("TauAnalysis.Configuration.plotZtoMuTau_cff")

process.DQMStore = cms.Service("DQMStore")

process.maxEvents = cms.untracked.PSet(            
  input = cms.untracked.int32(0)         
)

process.source = cms.Source("EmptySource")

process.p = cms.Path(process.makeZtoMuTauPlots)

# import utility function to enable factorization
from TauAnalysis.Configuration.factorizationTools import enableFactorization_makeZtoMuTauPlots
enableFactorization_makeZtoMuTauPlots(process)

# print-out all python configuration parameter information
#print process.dumpPython()
