import FWCore.ParameterSet.Config as cms

from TauAnalysis.Configuration.recoSampleDefinitionsZtoElecTau_grid_cfi \
        import recoSampleDefinitionsZtoElecTau

from TauAnalysis.Configuration.plotZtoElecTau_drawJobs_cfi import \
        drawJobConfigurator_ZtoElecTauOS, drawJobConfigurator_ZtoElecTauSS, plots_ZtoElecTau

from TauAnalysis.Configuration.makePlots2_grid import makePlots
from TauAnalysis.Configuration.userRegistry import getHarvestingFilePath, \
        getJobId

# import utility function to enable factorization
from TauAnalysis.Configuration.tools.factorizationTools import \
        enableFactorization_makeZtoElecTauPlots_grid2

process = cms.Process('makeZtoElecTauPlots')

process.load("TauAnalysis.Configuration.dumpZtoElecTau_grid_cff")

channel = 'ZtoElecTau'
inputFilePath = getHarvestingFilePath(channel)
jobId = getJobId(channel)

makePlots(process, channel = channel,
          samples = recoSampleDefinitionsZtoElecTau,
          inputFilePath = inputFilePath, jobId = jobId,
          analyzer_drawJobConfigurator_indOutputFileName_sets = [
              [ "zElecTauAnalyzerOS", drawJobConfigurator_ZtoElecTauOS, "plotZtoElecTauOS_#PLOT#.png" ],
              [ "zElecTauAnalyzerSS", drawJobConfigurator_ZtoElecTauSS, "plotZtoElecTauSS_#PLOT#.png" ]
          ],
          drawJobTemplate = plots_ZtoElecTau,
          enableFactorizationFunction = enableFactorization_makeZtoElecTauPlots_grid2,
          dqmDirectoryFilterStatistics = {
              'factorizationDisabled' : 'zElecTauAnalyzerOS/FilterStatistics',
              'factorizationEnabled' : 'zElecTauAnalyzerOS_factorizedWithElectronIsolation/FilterStatistics'
          },
          dumpDQMStore = False)

# print-out all python configuration parameter information
#print process.dumpPython()
