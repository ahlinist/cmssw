import FWCore.ParameterSet.Config as cms
import sys
import os
sys.setrecursionlimit(10000)

from TauAnalysis.Configuration.recoSampleDefinitionsAHtoElecTau_grid_cfi \
        import recoSampleDefinitionsAHtoElecTau

from TauAnalysis.Configuration.plotAHtoElecTau_drawJobs_cfi import \
        drawJobConfiguratorDict,\
        plots_AHtoElecTau,\
        useSSdataForQCD

from TauAnalysis.Configuration.makePlots2_grid import makePlots
import TauAnalysis.Configuration.userRegistry as reg

# import utility function to enable factorization
from TauAnalysis.Configuration.tools.factorizationTools import \
        enableFactorization_makeAHtoElecTauPlots_grid2

process = cms.Process('makeAHtoElecTauPlots')

channel = 'AHtoElecTau'

categories = ['wBtag','woBtag','ZeroOneJets','VBF','Boosted']

reg.overrideJobId(channel, 'Run41')

inputFilePath = reg.getHarvestingFilePath(channel)
jobId = reg.getJobId(channel)

plotsDirectory = './plots'
outputFileNameMaker = lambda channel: 'plots%s_all.root' % channel

analyzer_draw_jobs = []

for cat in categories:
    for sign in ['OS','SS']:
        analyzer_draw_jobs.append(
            [ 'ahElecTauAnalyzer' + sign + '_' + cat,
            drawJobConfiguratorDict[sign+'_'+cat],
            'plotAHtoElecTauOS_' + cat + '_#PLOT#.png'])


# attach FilterStatistics dump modules to process
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
          #enableFactorizationFunction = enableFactorization_makeAHtoElecTauPlots_grid2,
          dqmDirectoryFilterStatistics = {
              'factorizationDisabled' : 'ahElecTauAnalyzerOS_woBtag/FilterStatistics',
              'factorizationEnabled' : 'ahElecTauAnalyzerOS_woBtag_factorizedWithElectronIsolation/FilterStatistics'
          },
          dumpDQMStore = False,
          outputFileNameMaker = outputFileNameMaker,
          plotsDirectory = plotsDirectory)


bgYieldCorrections = {
    'woBtag' : {
        #  desired number / lumi-weighted number (sum if necessary) / processing inefficiency
        'ZtautauEmbeddedSum' : 12232./12548, 
        'ZtautauSum' : 12232./12548, 
        'ZeeSum' : 6974./6104,
        'WplusJets_madgraph_skim' : 2512./4903,
        'VVsum' : 0.01/50.7,
        'TTplusJets_madgraph_skim' : 58.8/30.2,
        'qcdFromData' : 6536./8189 # desired QCD sum / SS data 
    },
    'wBtag' : {
        #  desired number / lumi-weighted number (sum if necessary) / processing inefficiency
        'ZtautauSum' : 151.2/132.6, 
        'ZeeSum' : 56.3/46.1,
        'WplusJets_madgraph_skim' : 42./65.5,
        'VVsum' : 0.01/1.2,
        'TTplusJets_madgraph_skim' : 78.3/62.2,
        'qcdFromData' : 36./122 # desired QCD sum / SS data 
    },
    'VBF' : {
        #  desired number / lumi-weighted number (sum if necessary) / processing inefficiency
        'ZtautauSum' : 25.9/19.5, 
        'ZeeSum' : 28.0/15.2,
        'WplusJets_madgraph_skim' : 0.01/9.2,
        'VVsum' : 0.01/0.2,
        'TTplusJets_madgraph_skim' : 5.4/12.8,
        'qcdFromData' : 8.47/37 # desired QCD sum / SS data 
    },
    'ZeroOneJets' : {
        #  desired number / lumi-weighted number (sum if necessary) / processing inefficiency
        'ZtautauEmbeddedSum' : 11310./11930, 
        'ZtautauSum' : 11310./11930, 
        'ZeeSum' : 6786./6014,
        'WplusJets_madgraph_skim' : 1509./2839,
        'VVsum' : 60./31,
        'TTplusJets_madgraph_skim' : 0.01/43.6,
        'qcdFromData' : 5455./7656.5 # desired QCD sum / SS data 
    },
    'Boosted' : {
        #  desired number / lumi-weighted number (sum if necessary) / processing inefficiency
        'ZtautauSum' : 67./65, 
        'ZeeSum' : 4.4/8,
        'WplusJets_madgraph_skim' : 0.01/25,
        'TTplusJets_madgraph_skim' : 2.8/2.3,
        'VVsum' : 1.1/0.6,
        'qcdFromData' : 0.001/5 # desired QCD sum / SS data 
    }
}

# use SS data events for QCD shape
useSSdataForQCD(process, recoSampleDefinitionsAHtoElecTau, channel, categories, jobId, bgYieldCorrections)

# load from previously-produced plots file
process.loadFinalAHtoElecTauSamples = cms.EDAnalyzer("DQMFileLoader",
    inputFilePath = cms.string('/afs/crc.nd.edu/group/NDCMS/data02/jkolb/TauResults/' + jobId),
    all = cms.PSet(
        inputFileNames = cms.vstring('plotsAHtoElecTau_all.root'),
        dqmDirectory_store = cms.string('/'),
        scaleFactor = cms.double(1.0)
    )
)
#process.p = cms.Path(process.loadFinalAHtoElecTauSamples + process.plotAHtoElecTauSequence)

# print-out all python configuration parameter information
print process.dumpPython()
