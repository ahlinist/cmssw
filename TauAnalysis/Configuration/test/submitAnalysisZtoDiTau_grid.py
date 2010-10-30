#!/usr/bin/env python

from TauAnalysis.Configuration.recoSampleDefinitionsZtoDiTau_7TeV_grid_cfi import recoSampleDefinitionsZtoDiTau_7TeV
from TauAnalysis.Configuration.submitAnalysisToGrid import submitAnalysisToGrid
from TauAnalysis.Configuration.userRegistry import getAnalysisFilePath, getJobId

channel = 'ZtoDiTau'
configFile = 'runZtoDiTau_cfg.py'
analysisFilePath = getAnalysisFilePath(channel)
jobId = getJobId(channel)

samplesToAnalyze = [
    # modify in case you want to submit crab jobs for some of the samples only...
]

enableSystematics = False

submitAnalysisToGrid(configFile = configFile, channel = 'ZtoDiTau',
                     samples = recoSampleDefinitionsZtoDiTau_7TeV, outputFilePath = analysisFilePath, jobId = jobId,
                     samplesToAnalyze = samplesToAnalyze, disableSysUncertainties = not enableSystematics)



