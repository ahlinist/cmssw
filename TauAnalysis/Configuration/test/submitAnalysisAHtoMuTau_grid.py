#!/usr/bin/env python

from TauAnalysis.Configuration.recoSampleDefinitionsAHtoMuTau_7TeV_grid_cfi import recoSampleDefinitionsAHtoMuTau_7TeV
from TauAnalysis.Configuration.submitAnalysisToGrid import submitAnalysisToGrid
from TauAnalysis.Configuration.userRegistry import getAnalysisFilePath, \
       getJobId, overrideJobId

channel = 'AHtoMuTau'

configFile = 'runAHtoMuTau_cfg.py'
analysisFilePath = getAnalysisFilePath(channel)
jobId = getJobId(channel)

samplesToAnalyze = [
    # modify in case you want to submit crab jobs for some of the samples only...
    'ZtautauPU156bx',
    'A120', 'bbA120',
    'A200', 'bbA200'
]

#enableSystematics = True
enableSystematics = False

submitAnalysisToGrid(configFile = configFile, channel = 'AHtoMuTau',
                     samples = recoSampleDefinitionsAHtoMuTau_7TeV,
                     outputFilePath = analysisFilePath, jobId = jobId,
                     samplesToAnalyze = samplesToAnalyze,
                     disableSysUncertainties = not enableSystematics,
                     submit = True, saveFinalEvents = True)
