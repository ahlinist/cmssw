import FWCore.ParameterSet.Config as cms

from TauAnalysis.Configuration.recoSampleDefinitionsZtoDiTau_7TeV_grid_cfi import recoSampleDefinitionsZtoDiTau_7TeV
from TauAnalysis.Configuration.plotZtoDiTau_drawJobs_cfi import *
from TauAnalysis.Configuration.makePlots2_grid import makePlots
from TauAnalysis.Configuration.userRegistry import getHarvestingFilePath, getJobId

# import utility function to enable factorization
from TauAnalysis.Configuration.tools.factorizationTools import enableFactorization_makeZtoDiTauPlots_grid

process = cms.Process('makeZtoDiTauPlots')

process.load("TauAnalysis.Configuration.dumpZtoDiTau_grid_cff")
##process.load("TauAnalysis.Configuration.plotZtoDiTau_grid_cff")

channel = 'ZtoDiTau'
inputFilePath = getHarvestingFilePath(channel)
jobId = getJobId(channel)

makePlots(process, channel = channel, samples = recoSampleDefinitionsZtoDiTau_7TeV,
          inputFilePath = inputFilePath, jobId = jobId,
          analyzer_drawJobConfigurator_indOutputFileName_sets = [
              [ "zDiTauAnalyzer", drawJobConfigurator_ZtoDiTau, "plotZtoDiTau_#PLOT#.png" ]
          ],
          drawJobTemplate = plots_ZtoDiTau,
          enableFactorizationFunction = enableFactorization_makeZtoDiTauPlots_grid,
          dqmDirectoryFilterStatistics = {
              'factorizationDisabled' : 'zDiTauAnalyzer/FilterStatistics',
              'factorizationEnabled' : 'zDiTauAnalyzer_factorizedWithMuonIsolation/FilterStatistics'
          },
          dumpDQMStore = False)

# print-out all python configuration parameter information
#print process.dumpPython()
