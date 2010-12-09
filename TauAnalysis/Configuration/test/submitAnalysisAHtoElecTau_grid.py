#!/usr/bin/env python

from TauAnalysis.Configuration.recoSampleDefinitionsAHtoElecTau_grid_cfi import recoSampleDefinitionsAHtoElecTau
from TauAnalysis.Configuration.submitAnalysisToGrid import submitAnalysisToGrid
from TauAnalysis.Configuration.userRegistry import getAnalysisFilePath, getJobId

channel = 'AHtoElecTau'
configFile = 'runAHtoElecTau_cfg.py'
analysisFilePath = getAnalysisFilePath(channel)
jobId = getJobId(channel)

samplesToAnalyze = [
	'A120_T3',
	'Ztautau_PU156BX'
]

enableSystematics = True
#enableSystematics = False

submitAnalysisToGrid(configFile = configFile, channel = 'AHtoElecTau',
                     samples = recoSampleDefinitionsAHtoElecTau,
                     outputFilePath = analysisFilePath, jobId = jobId,
                     samplesToAnalyze = samplesToAnalyze,
                     disableSysUncertainties = not enableSystematics,
                     submit = False, saveFinalEvents=False)

