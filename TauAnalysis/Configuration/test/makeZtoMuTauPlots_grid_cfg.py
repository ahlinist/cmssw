import FWCore.ParameterSet.Config as cms

from TauAnalysis.Configuration.recoSampleDefinitionsZtoMuTau_7TeV_grid_cfi import recoSampleDefinitionsZtoMuTau_7TeV
from TauAnalysis.Configuration.plotZtoMuTau_drawJobs_cfi import *
from TauAnalysis.Configuration.makePlots2_grid import makePlots
from TauAnalysis.Configuration.userRegistry import getHarvestingFilePath, getJobId

# import utility function to enable factorization
from TauAnalysis.Configuration.tools.factorizationTools import enableFactorization_makeZtoMuTauPlots_grid

process = cms.Process('makeZtoMuTauPlots')

process.load("TauAnalysis.Configuration.dumpZtoMuTau_grid_cff")

channel = 'ZtoMuTau'
inputFilePath = getHarvestingFilePath(channel)
jobId = getJobId(channel)

makePlots(process, channel = channel, samples = recoSampleDefinitionsZtoMuTau_7TeV,
          inputFilePath = inputFilePath, jobId = jobId,
          analyzer_drawJobConfigurator_indOutputFileName_sets = [
              [ "zMuTauAnalyzer", drawJobConfigurator_ZtoMuTau, "plotZtoMuTau_#PLOT#.png" ]
          ],
          drawJobTemplate = plots_ZtoMuTau,
          enableFactorizationFunction = enableFactorization_makeZtoMuTauPlots_grid,
          dqmDirectoryFilterStatistics = {
              'factorizationDisabled' : 'zMuTauAnalyzer/FilterStatistics',
              'factorizationEnabled' : 'zMuTauAnalyzer_factorizedWithMuonIsolation/FilterStatistics'
          },
          dumpDQMStore = False)

# print-out all python configuration parameter information
#print process.dumpPython()
