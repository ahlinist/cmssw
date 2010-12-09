import FWCore.ParameterSet.Config as cms

from TauAnalysis.Configuration.recoSampleDefinitionsZtoElecTau_grid_cfi \
        import recoSampleDefinitionsZtoElecTau

from TauAnalysis.Configuration.plotZtoElecTau_drawJobs_cfi import \
        drawJobConfigurator_ZtoElecTau, plots_ZtoElecTau

from TauAnalysis.Configuration.makePlots2_grid import makePlots
from TauAnalysis.Configuration.userRegistry import getHarvestingFilePath, \
        getJobId

# import utility function to enable factorization
from TauAnalysis.Configuration.tools.factorizationTools import \
        enableFactorization_makeZtoElecTauPlots_grid

process = cms.Process('makeZtoElecTauPlots')

from TauAnalysis.Configuration.dumpZtoElecTau_grid_cff import dumpZtoElecTau as dumpZtoElecTau_stdCuts
process.dumpZtoElecTau_stdCuts = dumpZtoElecTau_stdCuts
#process.load("TauAnalysis.Configuration.dumpZtoElecTau_grid_cff")

channel = 'ZtoElecTau_stdCuts'
inputFilePath = getHarvestingFilePath(channel)
jobId = getJobId(channel)

makePlots(process, channel = channel,
          samples = recoSampleDefinitionsZtoElecTau,
          inputFilePath = inputFilePath, jobId = jobId,
          analyzer_drawJobConfigurator_indOutputFileName_sets = [
              [ "zElecTauAnalyzer", drawJobConfigurator_ZtoElecTau, "plotZtoElecTau_#PLOT#.png" ]
          ],
          drawJobTemplate = plots_ZtoElecTau,
          enableFactorizationFunction = enableFactorization_makeZtoElecTauPlots_grid,
          dqmDirectoryFilterStatistics = {
              'factorizationDisabled' : 'zElecTauAnalyzer/FilterStatistics',
              'factorizationEnabled' : 'zElecTauAnalyzer_factorizedWithElectronIsolation/FilterStatistics'
          },
          dumpDQMStore = False)

# print-out all python configuration parameter information
#print process.dumpPython()
