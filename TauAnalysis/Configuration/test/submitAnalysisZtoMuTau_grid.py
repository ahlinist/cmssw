#!/usr/bin/env python

from TauAnalysis.Configuration.recoSampleDefinitionsZtoMuTau_7TeV_grid_cfi import recoSampleDefinitionsZtoMuTau_7TeV
from TauAnalysis.Configuration.submitAnalysisToGrid import submitAnalysisToGrid
from TauAnalysis.Configuration.userRegistry import getAnalysisFilePath, getJobId

channel = 'ZtoMuTau'
configFile = 'runZtoMuTau_cfg.py'
analysisFilePath = getAnalysisFilePath(channel)
jobId = getJobId(channel)

samplesToAnalyze = [
    # modify in case you want to submit crab jobs for some of the samples only...
]

#enableSystematics = True
enableSystematics = False

submitAnalysisToGrid(configFile = configFile, channel = 'ZtoMuTau',
                     samples = recoSampleDefinitionsZtoMuTau_7TeV, outputFilePath = analysisFilePath, jobId = jobId,
                     samplesToAnalyze = samplesToAnalyze, disableSysUncertainties = not enableSystematics)
