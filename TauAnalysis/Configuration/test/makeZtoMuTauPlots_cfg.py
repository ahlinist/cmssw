import FWCore.ParameterSet.Config as cms

process = cms.Process('makeZtoMuTauPlots')

process.load("TauAnalysis.Configuration.dumpZtoMuTau_cff")
process.load("TauAnalysis.Configuration.plotZtoMuTau_cff")

process.DQMStore = cms.Service("DQMStore")

process.maxEvents = cms.untracked.PSet(            
    input = cms.untracked.int32(0)         
)

process.source = cms.Source("EmptySource")

# import utility function to enable factorization
from TauAnalysis.Configuration.factorizationTools import enableFactorization_makeZtoMuTauPlots
enableFactorization_makeZtoMuTauPlots(process)

process.makeZtoMuTauPlots = cms.Sequence( process.loadZtoMuTau
                                         +process.addZtoMuTau
                                         +process.saveZtoMuTau
                                         +process.dumpZtoMuTau
                                         +process.plotZtoMuTau )

process.p = cms.Path(process.makeZtoMuTauPlots)

# print-out all python configuration parameter information
print process.dumpPython()
