import FWCore.ParameterSet.Config as cms
import os

from TauAnalysis.Configuration.recoSampleDefinitionsAHtoMuTau_7TeV_grid_cfi \
        import recoSampleDefinitionsAHtoMuTau_7TeV

from TauAnalysis.Configuration.plotAHtoMuTau_drawJobs_cfi import \
        drawJobConfigurator_AHtoMuTau_woBtag, plots_AHtoMuTau, \
        drawJobConfigurator_AHtoMuTau_wBtag

from TauAnalysis.Configuration.makePlots2_grid import makePlots
from TauAnalysis.Configuration.userRegistry import getHarvestingFilePath, \
        getJobId, overrideJobId

# import utility function to enable factorization
from TauAnalysis.Configuration.tools.factorizationTools import \
        enableFactorization_makeAHtoMuTauPlots_grid2

process = cms.Process('makeAHtoMuTauPlots')

process.load("TauAnalysis.Configuration.dumpAHtoMuTau_grid_cff")
##process.load("TauAnalysis.Configuration.plotAHtoMuTau_grid_cff")

channel = 'AHtoMuTau'
overrideJobId(channel, 'Run28')
inputFilePath = getHarvestingFilePath(channel)
jobId = getJobId(channel)

# Uncomment to enable local file running
#inputFilePath = os.path.join(inputFilePath, 'local')

makePlots(process, channel = channel,
          samples = recoSampleDefinitionsAHtoMuTau_7TeV,
          inputFilePath = inputFilePath, jobId = jobId,
          analyzer_drawJobConfigurator_indOutputFileName_sets = [
              [ "ahMuTauAnalyzer_woBtag",
               drawJobConfigurator_AHtoMuTau_woBtag,
               "plotAHtoMuTau_woBtag_#PLOT#.pdf" ],
            [ "ahMuTauAnalyzer_wBtag",
              drawJobConfigurator_AHtoMuTau_wBtag,
             "plotAHtoMuTau_wBtag_#PLOT#.pdf"  ]
          ],
          drawJobTemplate = plots_AHtoMuTau,
          enableFactorizationFunction = enableFactorization_makeAHtoMuTauPlots_grid2,
          dqmDirectoryFilterStatistics = {
              'factorizationDisabled' : 'ahMuTauAnalyzer_woBtag/FilterStatistics',
              'factorizationEnabled' : 'ahMuTauAnalyzer_woBtag_factorizedWithMuonIsolation/FilterStatistics'
          },
          dumpDQMStore = False)

# print-out all python configuration parameter information
#print process.dumpPython()
