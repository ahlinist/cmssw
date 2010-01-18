import FWCore.ParameterSet.Config as cms

process = cms.Process('makeWtoTauNuPlots')

process.load("TauAnalysis.Configuration.dumpWtoTauNu_cff")
process.load("TauAnalysis.Configuration.plotWtoTauNu_cff")

process.DQMStore = cms.Service("DQMStore")

process.maxEvents = cms.untracked.PSet(            
    input = cms.untracked.int32(0)         
)

process.source = cms.Source("EmptySource")

# import utility function to enable factorization
#from TauAnalysis.Configuration.tools.factorizationTools import enableFactorization_makeWtoTauNuPlots
#enableFactorization_makeWtoTauNuPlots(process)

process.makeWtoTauNuPlots = cms.Sequence( process.loadWtoTauNu
                                         +process.addWtoTauNu
                                         +process.saveWtoTauNu
                                         +process.dumpWtoTauNu
                                         +process.plotWtoTauNu )

process.p = cms.Path(process.makeWtoTauNuPlots)

# print-out all python configuration parameter information
#print process.dumpPython()
