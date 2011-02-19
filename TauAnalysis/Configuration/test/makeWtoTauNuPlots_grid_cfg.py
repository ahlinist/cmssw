import FWCore.ParameterSet.Config as cms

from TauAnalysis.Configuration.recoSampleDefinitionsWtoTauNu_grid_cfi import recoSampleDefinitionsWtoTauNu_grid

from TauAnalysis.Configuration.plotWtoTauNu_drawJobs_cfi import *

from TauAnalysis.Configuration.makePlots2_grid import makePlots
from TauAnalysis.Configuration.userRegistry import getHarvestingFilePath, getJobId

process = cms.Process('makeWtoTauNuPlots')

process.load("TauAnalysis.Configuration.dumpWtoTauNu_grid_cff")
#from TauAnalysis.Configuration.dumpWtoTauNu_grid_cff import dumpWtoTauNu 

channel = 'WtoTauNu'
inputFilePath = getHarvestingFilePath(channel)
jobId = getJobId(channel)

makePlots(process, channel = channel,
          samples = recoSampleDefinitionsWtoTauNu_grid,
          inputFilePath = inputFilePath, jobId = jobId,
          analyzer_drawJobConfigurator_indOutputFileName_sets = [
    [ "wTauNuAnalyzer", drawJobConfigurator_WtoTauNu, "plotWtoTauNu_#PLOT#.png" ]
    ],
          drawJobTemplate = plots_WtoTauNu,
#          enableFactorizationFunction = enableFactorization_makeWtoTauNuPlots_grid,
          dqmDirectoryFilterStatistics = {
    'factorizationDisabled' : 'wTauNuAnalyzer/FilterStatistics',
#    'factorizationEnabled' : 'wTauNuAnalyzer_factorizedWithTauIsolation/FilterStatistics'
    },
          dumpDQMStore = True)

# print-out all python configuration parameter information
#print process.dumpPython()
