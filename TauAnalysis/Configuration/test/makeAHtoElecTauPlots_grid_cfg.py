import FWCore.ParameterSet.Config as cms
import sys
import os

from TauAnalysis.Configuration.recoSampleDefinitionsAHtoElecTau_grid_cfi \
        import recoSampleDefinitionsAHtoElecTau

from TauAnalysis.Configuration.plotAHtoElecTau_drawJobs_cfi import \
        drawJobConfigurator_AHtoElecTau_woBtag, plots_AHtoElecTau, \
        drawJobConfigurator_AHtoElecTau_wBtag,\
        drawJobConfigurator_AHtoElecTau_woBtagSS,\
        drawJobConfigurator_AHtoElecTau_wBtagSS\

from TauAnalysis.Configuration.makePlots2_grid import makePlots
import TauAnalysis.Configuration.userRegistry as reg

# import utility function to enable factorization
from TauAnalysis.Configuration.tools.factorizationTools import \
        enableFactorization_makeAHtoElecTauPlots_grid2

process = cms.Process('makeAHtoElecTauPlots')

channel = 'AHtoElecTau'

#reg.overrideJobId(channel, '2010Dec23_lxbatch')
#reg.overrideJobId(channel, 'Run33')
#reg.overrideJobId(channel, 'Run37sysTanc')
#reg.overrideJobId(channel, 'Run37sys')
#reg.overrideJobId(channel, 'Run42OldTaNCfinal')

inputFilePath = reg.getHarvestingFilePath(channel)
jobId = reg.getJobId(channel)

plotsDirectory = './plots'
outputFileNameMaker = lambda channel: 'plots%s_all.root' % channel

analyzer_draw_jobs = [
    [ "ahElecTauAnalyzerOS_woBtag",
     drawJobConfigurator_AHtoElecTau_woBtag,
     "plotAHtoElecTauOS_woBtag_#PLOT#.pdf" ],
    [ "ahElecTauAnalyzerOS_wBtag",
     drawJobConfigurator_AHtoElecTau_wBtag,
     "plotAHtoElecTauOS_wBtag_#PLOT#.pdf"  ],

    [ "ahElecTauAnalyzerSS_woBtag",
     drawJobConfigurator_AHtoElecTau_woBtagSS,
     "plotAHtoElecTauSS_woBtag_#PLOT#.pdf" ],

    [ "ahElecTauAnalyzerSS_wBtag",
     drawJobConfigurator_AHtoElecTau_wBtagSS,
     "plotAHtoElecTauSS_wBtag_#PLOT#.pdf"  ],
]

process.load("TauAnalysis.Configuration.dumpAHtoElecTau_grid_cff")

makePlots(process, channel = channel,
          samples = recoSampleDefinitionsAHtoElecTau,
          inputFilePath = inputFilePath, jobId = jobId,
          analyzer_drawJobConfigurator_indOutputFileName_sets =
          analyzer_draw_jobs,
          # Uncomment to enable local file running
		  #skimStatFileMapper = reg.makeSkimStatFileMapper(channel),
		  #skimFilterStatistic = 'evtSelDiMuPairZmumuHypothesisVetoByLooseIsolation',
          drawJobTemplate = plots_AHtoElecTau,
          enableFactorizationFunction = enableFactorization_makeAHtoElecTauPlots_grid2,
          dqmDirectoryFilterStatistics = {
              'factorizationDisabled' : 'ahElecTauAnalyzerOS_woBtag/FilterStatistics',
              'factorizationEnabled' : 'ahElecTauAnalyzerOS_woBtag_factorizedWithElectronIsolation/FilterStatistics'
          },
          dumpDQMStore = False,
          outputFileNameMaker = outputFileNameMaker,
          plotsDirectory = plotsDirectory)

# print-out all python configuration parameter information
#print process.dumpPython()
