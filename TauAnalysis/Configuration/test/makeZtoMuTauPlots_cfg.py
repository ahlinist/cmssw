import FWCore.ParameterSet.Config as cms

process = cms.Process('makeZtoMuTauPlots')

process.load("TauAnalysis.Configuration.dumpZtoMuTau_cff")
process.load("TauAnalysis.Configuration.plotZtoMuTau_cff")

process.DQMStore = cms.Service("DQMStore")

process.maxEvents = cms.untracked.PSet(            
    input = cms.untracked.int32(0)         
)

process.source = cms.Source("EmptySource")

# define directory from which .root files containing the histograms get loaded
process.loadZtoMuTau.inputFilePath = cms.string("rfio:/castor/cern.ch/user/v/veelken/CMSSW_3_6_x/plots/ZtoMuTau/7TeV/")
##process.loadZtoMuTau.inputFilePath = cms.string("rfio:/castor/cern.ch/user/v/veelken/CMSSW_3_3_x/plots/ZtoMuTau/7TeV/")
#process.loadZtoMuTau.inputFilePath = cms.string("rfio:/castor/cern.ch/user/l/lusito/ZtoMuTauAnalysis/")

# import utility function to enable factorization
from TauAnalysis.Configuration.tools.factorizationTools import enableFactorization_makeZtoMuTauPlots
enableFactorization_makeZtoMuTauPlots(process)

process.dumpDQMStore = cms.EDAnalyzer("DQMStoreDump")

process.makeZtoMuTauPlots = cms.Sequence(
    process.loadZtoMuTau
   #+ process.dumpDQMStore
   + process.addZtoMuTau
   + process.saveZtoMuTau
   + process.dumpZtoMuTau
   + process.plotZtoMuTau
)

process.p = cms.Path(process.makeZtoMuTauPlots)

# print-out all python configuration parameter information
#print process.dumpPython()
