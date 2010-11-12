import FWCore.ParameterSet.Config as cms

from TauAnalysis.Configuration.recoSampleDefinitionsAHtoMuTau_7TeV_grid_cfi \
        import recoSampleDefinitionsAHtoMuTau_7TeV

from TauAnalysis.Configuration.plotAHtoMuTau_drawJobs_cfi import \
        drawJobConfigurator_AHtoMuTau_woBtag, plots_AHtoMuTau, \
        drawJobConfigurator_AHtoMuTau_wBtag

from TauAnalysis.Configuration.makePlots2_grid import makePlots
from TauAnalysis.Configuration.userRegistry import getHarvestingFilePath, \
        getJobId

# import utility function to enable factorization
from TauAnalysis.Configuration.tools.factorizationTools import \
        enableFactorization_makeAHtoMuTauPlots_grid

process = cms.Process('makeAHtoMuTauPlots')

process.load("TauAnalysis.Configuration.dumpAHtoMuTau_grid_cff")
##process.load("TauAnalysis.Configuration.plotAHtoMuTau_grid_cff")

channel = 'AHtoMuTau'
inputFilePath = getHarvestingFilePath(channel)
jobId = getJobId(channel)

makePlots(process, channel = channel,
          samples = recoSampleDefinitionsAHtoMuTau_7TeV,
          inputFilePath = inputFilePath, jobId = jobId,
          analyzer_drawJobConfigurator_indOutputFileName_sets = [
              [ "ahMuTauAnalyzer_woBtag",
               drawJobConfigurator_AHtoMuTau_woBtag,
               "plotAHtoMuTau_woBtag_#PLOT#.png" ],
            #[ "ahMuTauAnalyzer_wBtag",
              #drawJobConfigurator_AHtoMuTau_wBtag,
             #"plotAHtoMuTau_wBtag_#PLOT#.png"  ]
          ],
          drawJobTemplate = plots_AHtoMuTau,
          enableFactorizationFunction = enableFactorization_makeAHtoMuTauPlots_grid,
          dqmDirectoryFilterStatistics = {
              'factorizationDisabled' : 'ahMuTauAnalyzer_woBtag/FilterStatistics',
              'factorizationEnabled' : 'ahMuTauAnalyzer_woBtag_factorizedWithMuonIsolation/FilterStatistics'
          },
          dumpDQMStore = False)

# print-out all python configuration parameter information
#print process.dumpPython()
