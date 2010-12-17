import FWCore.ParameterSet.Config as cms
import sys
import os

from TauAnalysis.Configuration.recoSampleDefinitionsAHtoMuTau_7TeV_grid_cfi \
        import recoSampleDefinitionsAHtoMuTau_7TeV, make_flattened_samples

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
reg.overrideJobId(channel, 'Run35SYS')
inputFilePath = reg.getHarvestingFilePath(channel)
jobId = reg.getJobId(channel)

plotsDirectory = './plots'
outputFileNameMaker = lambda channel: 'plots%s_all.root' % channel

# Check if we want to override what's in reco sample definitions
if len(sys.argv) > 2:
    mode = sys.argv[2]
    print "Using plot mode: ", mode
    plotsDirectory += "_" + mode
    if not os.path.exists(plotsDirectory):
        os.makedirs(plotsDirectory)
    print "Putting plots in", plotsDirectory
    oldOutputFileNameMaker = outputFileNameMaker
    outputFileNameMaker = lambda channel: oldOutputFileNameMaker(channel).replace(
        '.root', '.' + mode + '.root')
    print "Output filename:", outputFileNameMaker(channel)

    print recoSampleDefinitionsAHtoMuTau_7TeV['FLATTENED_SAMPLES_TO_PLOT']

    if mode.lower() == 'sm':
        # Just do SM
        recoSampleDefinitionsAHtoMuTau_7TeV['SAMPLES_TO_PRINT'][:] = []
        recoSampleDefinitionsAHtoMuTau_7TeV['SAMPLES_TO_PLOT'][:] = [
            'data',
            'ZtautauSum'
            'TTplusJets',
            'Zmumu',
            'WplusJetsSum',
            'qcdSum',
        ]
    elif mode.lower() == 'bsm':
        recoSampleDefinitionsAHtoMuTau_7TeV['SAMPLES_TO_PLOT'][:] = []
        recoSampleDefinitionsAHtoMuTau_7TeV['SAMPLES_TO_PRINT'][:] = []
        map(recoSampleDefinitionsAHtoMuTau_7TeV['SAMPLES_TO_PRINT'].append,
            ('A%sSum' % mass for mass in
             ['90', '100', '130', '160', '200', '250', '350'])
           )
    elif mode.lower() == 'bsmsplit':
        # Don't merge Higgs samples
        recoSampleDefinitionsAHtoMuTau_7TeV['SAMPLES_TO_PLOT'][:] = []
        recoSampleDefinitionsAHtoMuTau_7TeV['SAMPLES_TO_PRINT'][:] = []
        map(recoSampleDefinitionsAHtoMuTau_7TeV['SAMPLES_TO_PRINT'].append,
            ('A%s' % mass for mass in
             ['90', '100', '130', '160', '200', '250', '350'])
           )
        map(recoSampleDefinitionsAHtoMuTau_7TeV['SAMPLES_TO_PRINT'].append,
            ('bbA%s' % mass for mass in
             ['90', '100', '130', '160', '200', '250', '350'])
           )

    recoSampleDefinitionsAHtoMuTau_7TeV['FLATTENED_SAMPLES_TO_PLOT'] = \
            make_flattened_samples()
    print recoSampleDefinitionsAHtoMuTau_7TeV['FLATTENED_SAMPLES_TO_PLOT']


analyzer_draw_jobs = [
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
]

makePlots(process, channel = channel,
          samples = recoSampleDefinitionsAHtoMuTau_7TeV,
          inputFilePath = inputFilePath, jobId = jobId,
          analyzer_drawJobConfigurator_indOutputFileName_sets =
          analyzer_draw_jobs,
          # Uncomment to enable local file running
          skimStatFileMapper = reg.makeSkimStatFileMapper(channel),
          skimFilterStatistic = 'evtSelDiMuPairZmumuHypothesisVetoByLooseIsolation',
          drawJobTemplate = plots_AHtoMuTau,
          enableFactorizationFunction = enableFactorization_makeAHtoMuTauPlots_grid2,
          dqmDirectoryFilterStatistics = {
              'factorizationDisabled' : 'ahMuTauAnalyzerOS_woBtag/FilterStatistics',
              'factorizationEnabled' : 'ahMuTauAnalyzerOS_woBtag_factorizedWithMuonIsolation/FilterStatistics'
          },
          dumpDQMStore = False,
          outputFileNameMaker = outputFileNameMaker,
          plotsDirectory = plotsDirectory)

# print-out all python configuration parameter information
#print process.dumpPython()
