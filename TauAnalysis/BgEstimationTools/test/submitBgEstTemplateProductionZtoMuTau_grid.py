#!/usr/bin/env python

from TauAnalysis.Configuration.recoSampleDefinitionsZtoMuTau_7TeV_grid_cfi import recoSampleDefinitionsZtoMuTau_7TeV
from TauAnalysis.Configuration.submitAnalysisToGrid import submitAnalysisToGrid
from TauAnalysis.Configuration.userRegistry import getAnalysisFilePath, getJobId

channel = 'ZtoMuTau_bgEstTemplate'
## rename cfg so crabdir name and user registry entry match 
configFile = 'run' + channel + '_cfg.py'
#configFile = 'runBgEstTemplateProductionZtoMuTau_cfg.py'
outputFilePath = getAnalysisFilePath(channel)
jobId = getJobId(channel)

samplesToAnalyze = [
    # modify in case you want to submit crab jobs for some of the samples only...
    'data_SingleMu_Run2011A_May10ReReco_skim',
    'Ztautau_embedded_part1_skim',
    'Ztautau_embedded_part2_skim',
    'DYtautauM10to20_pythia',
    'DYmumuM10to20_powheg',
	'qqZll',
    'Ztautau_powheg_skim',
    'Zmumu_powheg_skim',
    'PPmuXptGt20Mu15_skim',
    'WplusJets_madgraph_skim',
    'TTplusJets_madgraph_skim'
]

# Submit analysis jobs to the grid;
# disable estimation of systematic uncertainties for all samples except Ztautau
submitAnalysisToGrid(configFile = configFile, channel = channel,
                     samples = recoSampleDefinitionsZtoMuTau_7TeV, outputFilePath = outputFilePath, jobId = jobId,
                     samplesToAnalyze = samplesToAnalyze, 
					 samplesToSkip = ['WW','WZ','ZZ','DYmumuM10to20_powheg','DYtautauM10to20_pythia','qqZll'],
                     disableFactorization = True, disableSysUncertainties = True,
					 outsideCERN = True,
					 useCastor = False,
                     create = True,
                     submit = True)
if samplesToAnalyze.count('WW') > 0 or len(samplesToAnalyze) == 0:
    submitAnalysisToGrid(configFile = configFile, channel = channel,
                         samples = recoSampleDefinitionsZtoMuTau_7TeV, outputFilePath = outputFilePath, jobId = jobId,
                         samplesToAnalyze = ['WW'], samplesToSkip = None, disableSysUncertainties = True,
					 	outsideCERN = False,
					 	useCastor = False,
                         create = True,
                         submit = True)
if samplesToAnalyze.count('WZ') > 0 or len(samplesToAnalyze) == 0:
    submitAnalysisToGrid(configFile = configFile, channel = channel,
                         samples = recoSampleDefinitionsZtoMuTau_7TeV, outputFilePath = outputFilePath, jobId = jobId,
                         samplesToAnalyze = ['WZ'], samplesToSkip = None, disableSysUncertainties = True,
					 	outsideCERN = False,
					 	useCastor = False,
                         create = True,
                         submit = True)
if samplesToAnalyze.count('ZZ') > 0 or len(samplesToAnalyze) == 0:
    submitAnalysisToGrid(configFile = configFile, channel = channel,
                         samples = recoSampleDefinitionsZtoMuTau_7TeV, outputFilePath = outputFilePath, jobId = jobId,
                         samplesToAnalyze = ['ZZ'], samplesToSkip = None, disableSysUncertainties = True,
					 	outsideCERN = False,
					 	useCastor = False,
                         create = True,
                         submit = True)
if samplesToAnalyze.count('DYmumuM10to20_powheg') > 0 or len(samplesToAnalyze) == 0:
    submitAnalysisToGrid(configFile = configFile, channel = channel,
                         samples = recoSampleDefinitionsZtoMuTau_7TeV, outputFilePath = outputFilePath, jobId = jobId,
                         samplesToAnalyze = ['DYmumuM10to20_powheg'], samplesToSkip = None, disableSysUncertainties = True,
					 	outsideCERN = False,
					 	useCastor = False,
                         create = True,
                         submit = True)
if samplesToAnalyze.count('DYtautauM10to20_pythia') > 0 or len(samplesToAnalyze) == 0:
    submitAnalysisToGrid(configFile = configFile, channel = channel,
                         samples = recoSampleDefinitionsZtoMuTau_7TeV, outputFilePath = outputFilePath, jobId = jobId,
                         samplesToAnalyze = ['DYtautauM10to20_pythia'], samplesToSkip = None, disableSysUncertainties = True,
					 	outsideCERN = False,
					 	useCastor = False,
                         create = True,
                         submit = True)
if samplesToAnalyze.count('qqZll') > 0 or len(samplesToAnalyze) == 0:
    submitAnalysisToGrid(configFile = configFile, channel = channel,
                         samples = recoSampleDefinitionsZtoMuTau_7TeV, outputFilePath = outputFilePath, jobId = jobId,
                         samplesToAnalyze = ['qqZll'], samplesToSkip = None, disableSysUncertainties = True,
					 	outsideCERN = False,
					 	useCastor = False,
                         create = True,
                         submit = True)
    
