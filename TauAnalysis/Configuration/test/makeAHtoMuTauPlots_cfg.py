import FWCore.ParameterSet.Config as cms

process = cms.Process('makeAHtoMuTauPlots')

process.load("TauAnalysis.Configuration.dumpAHtoMuTau_cff")
process.load("TauAnalysis.Configuration.plotAHtoMuTau_cff")

process.DQMStore = cms.Service("DQMStore")

process.maxEvents = cms.untracked.PSet(            
    input = cms.untracked.int32(0)         
)

process.source = cms.Source("EmptySource")

# define directory from which .root files containing histograms for individual processes get loaded
process.loadAHtoMuTau.inputFilePath = cms.string(
    "rfio:/castor/cern.ch/user/v/veelken/CMSSW_3_6_x/plots/AHtoMuTau/7TeV/"
)

# import utility function to enable factorization
from TauAnalysis.Configuration.tools.factorizationTools import enableFactorization_makeAHtoMuTauPlots
enableFactorization_makeAHtoMuTauPlots(process, modName_addAHtoMuTau_AHsum_tautau = "addAHtoMuTau_AHsum120_tautau")

process.dumpDQMStore = cms.EDAnalyzer("DQMStoreDump")

# define name and directory in which .root file containing all histograms gets saved
##process.saveAHtoMuTau.outputFileName = cms.string(
##    "/castor/cern.ch/user/v/veelken/CMSSW_3_6_x/plots/AHtoMuTau/7TeV/plotsAHtoMuTau_all.root"
##)

process.makeAHtoMuTauPlots = cms.Sequence(
    process.loadAHtoMuTau
   + process.addAHtoMuTau
   + process.dumpDQMStore
   + process.saveAHtoMuTau
   + process.dumpAHtoMuTau
   + process.plotAHtoMuTau
)

process.p = cms.Path(process.makeAHtoMuTauPlots)

# print-out all python configuration parameter information
#print process.dumpPython()
