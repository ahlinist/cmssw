import FWCore.ParameterSet.Config as cms

process = cms.Process('makeZtoElecTauPlots')

process.load("TauAnalysis.Configuration.dumpZtoElecTau_cff")
process.load("TauAnalysis.Configuration.plotZtoElecTau_cff")

process.DQMStore = cms.Service("DQMStore")

process.maxEvents = cms.untracked.PSet(            
    input = cms.untracked.int32(0)         
)

process.source = cms.Source("EmptySource")

# import utility function to enable factorization
from TauAnalysis.Configuration.factorizationTools import enableFactorization_makeZtoElecTauPlots
enableFactorization_makeZtoElecTauPlots(process)

process.makeZtoElecTauPlots = cms.Sequence( process.loadZtoElecTau
                                           +process.addZtoElecTau
                                           +process.saveZtoElecTau
                                           +process.dumpZtoElecTau
                                           +process.plotZtoElecTau 
										   )

process.p = cms.Path(process.makeZtoElecTauPlots)

# print-out all python configuration parameter information
#print process.dumpPython()
