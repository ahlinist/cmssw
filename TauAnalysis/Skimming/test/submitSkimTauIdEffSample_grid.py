#!/usr/bin/env python

from TauAnalysis.Configuration.recoSampleDefinitionsZtoMuTau_7TeV_grid_cfi import recoSampleDefinitionsZtoMuTau_7TeV
from TauAnalysis.Configuration.submitAnalysisToGrid import submitAnalysisToGrid
from TauAnalysis.Configuration.userRegistry import getJobId

import os

channel = 'ZtoMuTau_tauIdEff'
configFile = 'skimTauIdEffSample_cfg.py'
#jobId = getJobId(channel)
jobId = '2011Jan17'

outputFilePath = '/user/v/veelken/CMSSW_3_8_x/skims/TauIdEffMeas/'

samplesToAnalyze = [
    # modify in case you want to submit jobs for some of the samples only...
    'data_Mu_Run2010A_Nov4ReReco',
    'data_Mu_Run2010B_Nov4ReReco'
]

# Define what output file name a skimmed sample will have
def output_mapper(channel, sample, jobId):
    output_file = "tauIdEffSample_%s_%s_RECO.root" % (sample, jobId)
    return output_file

submitAnalysisToGrid(configFile = configFile, channel = channel, jobId = jobId,
                     samples = recoSampleDefinitionsZtoMuTau_7TeV,
                     samplesToAnalyze = samplesToAnalyze,
                     disableFactorization = True, disableSysUncertainties = True, disableZrecoilCorrections = True,
                     outputFilePath = outputFilePath, outputFileMap = output_mapper)


