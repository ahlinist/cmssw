import FWCore.ParameterSet.Config as cms

from TauAnalysis.Configuration.recoSampleDefinitionsAHtoElecTau_grid_cfi \
        import recoSampleDefinitionsAHtoElecTau

from TauAnalysis.Configuration.plotAHtoElecTau_drawJobs_cfi import \
        drawJobConfigurator_AHtoElecTau, plots_AHtoElecTau

from TauAnalysis.Configuration.makePlots2_grid import makePlots
from TauAnalysis.Configuration.userRegistry import getHarvestingFilePath, \
        getJobId

# import utility function to enable factorization
from TauAnalysis.Configuration.tools.factorizationTools import \
        enableFactorization_makeAHtoElecTauPlots_grid

process = cms.Process('makeZtoElecTauPlots')

#from TauAnalysis.Configuration.dumpZtoElecTau_grid_cff import dumpZtoElecTau as dumpZtoElecTau_stdCuts
#process.dumpZtoElecTau_stdCuts = dumpZtoElecTau_stdCuts
process.load("TauAnalysis.Configuration.dumpAHtoElecTau_grid_cff")

channel = 'AHtoElecTau'
inputFilePath = getHarvestingFilePath(channel)
jobId = getJobId(channel)

makePlots(process, channel = channel,
          samples = recoSampleDefinitionsAHtoElecTau,
          inputFilePath = inputFilePath, jobId = jobId,
          analyzer_drawJobConfigurator_indOutputFileName_sets = [
              [ "zElecTauAnalyzer", drawJobConfigurator_AHtoElecTau, "plotAHtoElecTau_#PLOT#.png" ]
          ],
          drawJobTemplate = plots_AHtoElecTau,
          enableFactorizationFunction = enableFactorization_makeAHtoElecTauPlots_grid,
          dqmDirectoryFilterStatistics = {
              'factorizationDisabled' : 'zElecTauAnalyzer/FilterStatistics',
              'factorizationEnabled' : 'zElecTauAnalyzer_factorizedWithElectronIsolation/FilterStatistics'
          },
          dumpDQMStore = False)

# print-out all python configuration parameter information
#print process.dumpPython()
