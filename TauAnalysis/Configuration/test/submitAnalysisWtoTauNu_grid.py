#!/usr/bin/env python

from TauAnalysis.Configuration.recoSampleDefinitionsWtoTauNu_grid_cfi import recoSampleDefinitionsWtoTauNu_grid
from TauAnalysis.Configuration.submitAnalysisToGrid import submitAnalysisToGrid
from TauAnalysis.Configuration.userRegistry import getAnalysisFilePath, getJobId

channel = 'WtoTauNu'
configFile = 'runWtoTauNu_cfg.py'
analysisFilePath = getAnalysisFilePath(channel)
jobId = getJobId(channel)

samplesToAnalyze = [
    'data_BTau_Run2010B_Nov4ReReco',
    'Wtaunu',
    'Wenu',
    'Wmunu',
    'Ztautau',
    'QCD'
    ]

#enableSystematics = True
enableSystematics = False

submitAnalysisToGrid(configFile = configFile, channel = channel,
                     samples = recoSampleDefinitionsWtoTauNu_grid, outputFilePath = analysisFilePath, jobId = jobId,
                     samplesToAnalyze = samplesToAnalyze, disableSysUncertainties = not enableSystematics,
                     create = True, submit = True, cfgdir = 'crab', inputFileMap = None, outputFileMap = None,
                     enableEventDumps = False, enableFakeRates = False, processName = None, saveFinalEvents = False)
