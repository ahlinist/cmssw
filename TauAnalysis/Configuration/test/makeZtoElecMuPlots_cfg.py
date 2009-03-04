import FWCore.ParameterSet.Config as cms

process = cms.Process('makeZtoElecMuPlots')

process.load("TauAnalysis.Configuration.plotZtoElecMu_cff")

process.DQMStore = cms.Service("DQMStore")

process.maxEvents = cms.untracked.PSet(            
  input = cms.untracked.int32(0)         
)

process.source = cms.Source("EmptySource")

process.p = cms.Path(process.makeZtoElecMuPlots)

# print-out all python configuration parameter information
#print process.dumpPython()
