#!/usr/bin/env python

from TauAnalysis.Configuration.recoSampleDefinitionsZtoMuTau_7TeV_grid_cfi import recoSampleDefinitionsZtoMuTau_7TeV
#from TauAnalysis.Configuration.recoSampleDefinitionsAHtoMuTau_7TeV_grid_cfi import recoSampleDefinitionsAHtoMuTau_7TeV
from TauAnalysis.Configuration.submitAnalysisToGrid import submitAnalysisToGrid
from TauAnalysis.Configuration.userRegistry import getAnalysisFilePath, getJobId

channel = 'ZtoMuTau'
configFile = 'runZtoMuTau_cfg.py'
outputFilePath = getAnalysisFilePath(channel)
jobId = getJobId(channel)

samplesToAnalyze = [
    # modify in case you want to submit crab jobs for some of the samples only...    
    'data_SingleMu_Run2011A_May10ReReco_skim',
    #'Ztautau_embedded_part1_skim',
    #'Ztautau_embedded_part2_skim',
    #'DYtautauM10to20_pythia',
    #'DYmumuM10to20_powheg',
    #'qqZll',
    #'Ztautau_powheg_skim',
    #'Zmumu_powheg_skim',
    #'PPmuXptGt20Mu15_skim',
    #'WplusJets_madgraph_skim',
    #'TTplusJets_madgraph_skim'    
]

#import copy
#samplesToAnalyze = copy.deepcopy(recoSampleDefinitionsZtoMuTau_7TeV['SAMPLES_TO_ANALYZE'])
#samplesToAnalyze.extend([
#    'A130', 'bbA130',
#    'A200', 'bbA200',
#    'A300', 'bbA300',
#    'A450', 'bbA450'
#])

#enableSystematics = True
enableSystematics = False

submitAnalysisToGrid(configFile = configFile, channel = 'ZtoMuTau',
                     samples = recoSampleDefinitionsZtoMuTau_7TeV, outputFilePath = outputFilePath, jobId = jobId,
                     samplesToAnalyze = samplesToAnalyze, disableSysUncertainties = not enableSystematics,
                    samplesToSkip = ['WW','WZ','ZZ','DYmumuM10to20_powheg','DYtautauM10to20_pythia','qqZll'],
                    outsideCERN = True,
                    useCastor = False,
                     create = True,
                     submit = True)
if samplesToAnalyze.count('WW') > 0 or len(samplesToAnalyze) == 0:
    submitAnalysisToGrid(configFile = configFile, channel = channel,
                         samples = recoSampleDefinitionsZtoMuTau_7TeV, outputFilePath = FilePath, jobId = jobId,
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

