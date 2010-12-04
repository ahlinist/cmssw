#!/usr/bin/env python

from TauAnalysis.Configuration.recoSampleDefinitionsZtoMuTau_7TeV_grid_cfi import recoSampleDefinitionsZtoMuTau_7TeV
from TauAnalysis.Configuration.submitAnalysisToGrid import submitAnalysisToGrid
from TauAnalysis.Configuration.userRegistry import getJobId

import os

channel = 'ZtoMuTau'
configFile = 'skimTauIdEffSample_cfg.py'
jobId = getJobId(channel)

outputFilePath = '/user/v/veelken/CMSSW_3_8_x/skims/TauIdEffMeas/'

samplesToAnalyze = [
    # modify in case you want to submit jobs for some of the samples only...
    'data_Mu_Run2010A_Sep17ReReco'
]

# Define what output file name a skimmed sample will have
def output_mapper(channel, sample, jobId):
    output_file = "tauIdEffSample_%s_%s_RECO.root" % (sample, jobId)
    return output_file

submitAnalysisToGrid(configFile = configFile, channel = channel, jobId = jobId,
                     samples = recoSampleDefinitionsZtoMuTau_7TeV,
                     samplesToAnalyze = samplesToAnalyze,
                     disableFactorization = True, disableSysUncertainties = True,
                     outputFilePath = outputFilePath, outputFileMap = output_mapper)


