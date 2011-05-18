#!/usr/bin/env python

from TauAnalysis.Configuration.recoSampleDefinitionsWtoTauNu_grid_cfi import recoSampleDefinitionsWtoTauNu_grid
from TauAnalysis.Configuration.submitAnalysisToGrid import submitAnalysisToGrid
from TauAnalysis.Configuration.userRegistry import getAnalysisFilePath, getJobId

channel = 'WtoTauNu_bgEstTemplate'
configFile = 'runBgEstTemplateProductionWtoTauNu_cfg.py'
analysisFilePath = getAnalysisFilePath(channel)
jobId = getJobId(channel)

samplesToAnalyze = [
    # modify in case you want to submit crab jobs for some of the samples only
    'data',
    'Wtaunu',
    'Wmunu',
    'Wenu',
    'Ztautau',
    'QCD'
]

#enableSystematics = True
enableSystematics = False

# Submit analysis jobs to the grid;
#submitAnalysisToGrid(configFile = configFile, channel = channel,
#                     samples = recoSampleDefinitionsWtoTauNu_grid, outputFilePath = outputFilePath, jobId = jobId,
#                     samplesToAnalyze = samplesToAnalyze, samplesToSkip = None, disableFactorization = True, disableSysUncertainties = True)

#submit analysis jobs to the grid
submitAnalysisToGrid(configFile = configFile, channel = channel,
                     samples = recoSampleDefinitionsWtoTauNu_grid, outputFilePath = analysisFilePath, jobId = jobId,
                     samplesToAnalyze = samplesToAnalyze, disableSysUncertainties = not enableSystematics,
                     create = True, submit = True, cfgdir = 'crab', inputFileMap = None, outputFileMap = None,
                     enableEventDumps = False, enableFakeRates = False, processName = None, saveFinalEvents = False)
