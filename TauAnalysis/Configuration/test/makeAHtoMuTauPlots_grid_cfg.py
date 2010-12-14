import FWCore.ParameterSet.Config as cms
import os

from TauAnalysis.Configuration.recoSampleDefinitionsAHtoMuTau_7TeV_grid_cfi \
        import recoSampleDefinitionsAHtoMuTau_7TeV

from TauAnalysis.Configuration.plotAHtoMuTau_drawJobs_cfi import \
        drawJobConfigurator_AHtoMuTau_woBtag, plots_AHtoMuTau, \
        drawJobConfigurator_AHtoMuTau_wBtag,\
        drawJobConfigurator_AHtoMuTau_woBtagSS,\
        drawJobConfigurator_AHtoMuTau_wBtagSS\

from TauAnalysis.Configuration.makePlots2_grid import makePlots
import TauAnalysis.Configuration.userRegistry as reg

# import utility function to enable factorization
from TauAnalysis.Configuration.tools.factorizationTools import \
        enableFactorization_makeAHtoMuTauPlots_grid2

process = cms.Process('makeAHtoMuTauPlots')

process.load("TauAnalysis.Configuration.dumpAHtoMuTau_grid_cff")

channel = 'AHtoMuTau'
#reg.overrideJobId(channel, 'Run33')
inputFilePath = reg.getHarvestingFilePath(channel)
jobId = reg.getJobId(channel)

# Uncomment to enable local file running
def skimStatFileMapper(sample):
    return os.path.join(
        '/data1/friis/Run32',  'harvested_%s_%s_Run32.root' % (
            channel, sample))
        #getHarvestingFilePath(channel), 'harvested_%s_%s_%s.root' % (
        #    channel, sample, jobId))


makePlots(process, channel = channel,
          samples = recoSampleDefinitionsAHtoMuTau_7TeV,
          inputFilePath = inputFilePath, jobId = jobId,
          analyzer_drawJobConfigurator_indOutputFileName_sets = [
              [ "ahMuTauAnalyzerOS_woBtag",
               drawJobConfigurator_AHtoMuTau_woBtag,
               "plotAHtoMuTauOS_woBtag_#PLOT#.pdf" ],

              [ "ahMuTauAnalyzerOS_wBtag",
               drawJobConfigurator_AHtoMuTau_wBtag,
               "plotAHtoMuTauOS_wBtag_#PLOT#.pdf"  ],

              [ "ahMuTauAnalyzerSS_woBtag",
               drawJobConfigurator_AHtoMuTau_woBtagSS,
               "plotAHtoMuTauSS_woBtag_#PLOT#.pdf" ],

              [ "ahMuTauAnalyzerSS_wBtag",
               drawJobConfigurator_AHtoMuTau_wBtagSS,
               "plotAHtoMuTauSS_wBtag_#PLOT#.pdf"  ],
          ],
          # Uncomment to enable local file running
          #skimStatFileMapper = skimStatFileMapper,
          skimFilterStatistic = 'evtSelDiMuPairZmumuHypothesisVetoByLooseIsolation',
          drawJobTemplate = plots_AHtoMuTau,
          enableFactorizationFunction = enableFactorization_makeAHtoMuTauPlots_grid2,
          dqmDirectoryFilterStatistics = {
              'factorizationDisabled' : 'ahMuTauAnalyzerOS_woBtag/FilterStatistics',
              'factorizationEnabled' : 'ahMuTauAnalyzerOS_woBtag_factorizedWithMuonIsolation/FilterStatistics'
          },
          dumpDQMStore = False)

# print-out all python configuration parameter information
#print process.dumpPython()
