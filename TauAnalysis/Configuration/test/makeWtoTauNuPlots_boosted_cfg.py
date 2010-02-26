import FWCore.ParameterSet.Config as cms

process = cms.Process('makeWtoTauNuPlotsBoosted')

process.load("TauAnalysis.Configuration.dumpWtoTauNu_cff")
process.load("TauAnalysis.Configuration.plotWtoTauNu_cff")

process.DQMStore = cms.Service("DQMStore")

process.maxEvents = cms.untracked.PSet(            
    input = cms.untracked.int32(0)         
)

process.source = cms.Source("EmptySource")

# define directory from which .root files containing the histograms get loaded
#process.loadWtoTauNu.inputFilePath = cms.string("rfio:/castor/cern.ch/user/l/liis/CMSSW_3_1_X/WTauNuPlots10TeV")
process.loadWtoTauNu.inputFilePath = cms.string("rfio:/castor/cern.ch/user/l/liis/CMSSW_3_1_X/WTauNuPlots7TeV")

# import utility function to enable factorization
from TauAnalysis.Configuration.tools.factorizationTools import enableFactorization_makeWtoTauNuPlots_boosted
enableFactorization_makeWtoTauNuPlots_boosted(process)

process.makeWtoTauNuPlotsBoosted = cms.Sequence(
    process.loadWtoTauNu
    +process.addWtoTauNuBoosted
    +process.saveWtoTauNuBoosted
    +process.dumpWtoTauNu_boosted
    +process.plotWtoTauNu_boosted
    )

process.p = cms.Path(process.makeWtoTauNuPlotsBoosted)

# print-out all python configuration parameter information
#print process.dumpPython()
