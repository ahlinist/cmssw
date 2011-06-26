#!/usr/bin/env python

from TauAnalysis.Configuration.recoSampleDefinitionsZtoMuTau_7TeV_grid_cfi import recoSampleDefinitionsZtoMuTau_7TeV
from TauAnalysis.Configuration.recoSampleDefinitionsAHtoMuTau_7TeV_grid_cfi import recoSampleDefinitionsAHtoMuTau_7TeV
from TauAnalysis.Configuration.submitAnalysisToGrid import submitAnalysisToGrid
from TauAnalysis.Configuration.userRegistry import getAnalysisFilePath, getJobId

channel = 'ZtoMuTau'
configFile = 'runZtoMuTau_cfg.py'
analysisFilePath = getAnalysisFilePath(channel)
jobId = getJobId(channel)

samplesToAnalyze = [
    # modify in case you want to submit crab jobs for some of the samples only...    
]

import copy
samplesToAnalyze = copy.deepcopy(recoSampleDefinitionsZtoMuTau_7TeV['SAMPLES_TO_ANALYZE'])
#samplesToAnalyze.extend([
#    'A130', 'bbA130',
#    'A200', 'bbA200',
#    'A300', 'bbA300',
#    'A450', 'bbA450'
#])

#enableSystematics = True
enableSystematics = False

submitAnalysisToGrid(configFile = configFile, channel = 'ZtoMuTau',
                     samples = recoSampleDefinitionsAHtoMuTau_7TeV, outputFilePath = analysisFilePath, jobId = jobId,
                     samplesToAnalyze = samplesToAnalyze, disableSysUncertainties = not enableSystematics)
