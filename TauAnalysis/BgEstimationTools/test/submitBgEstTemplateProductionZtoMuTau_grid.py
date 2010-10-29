#!/usr/bin/env python

import os

import TauAnalysis.Configuration.recoSampleDefinitionsZtoMuTau_7TeV_grid_cfi as samples
from TauAnalysis.Configuration.submitAnalysisToGrid import submitAnalysisToGrid

USER_PREFERENCES = {
    'veelken': {
        'OUTPUT_FILE_PATH' : "/user/v/veelken/CMSSW_3_8_x/plots/ZtoMuTau_bgEstTemplate/",
        'JOB_ID' : "7TeV"
    }
}

# Get the current users specifications
USER_CONFIG = USER_PREFERENCES[os.environ['LOGNAME']]

CONFIG_FILE = 'prodBgEstTemplatesZtoMuTau_cfg.py'
OUTPUT_FILE_PATH = USER_CONFIG['OUTPUT_FILE_PATH']
JOB_ID = USER_CONFIG['JOB_ID']

SAMPLE_LIST_OVERRIDE = [
    # modify in case you want to submit crab jobs for some of the samples only...
]

submitAnalysisToGrid(configFile = CONFIG_FILE, channel = 'ZtoMuTau',
                     samples = recoSampleDefinitionsZtoMuTau_7TeV, outputFilePath = OUTPUT_FILE_PATH, jobId = JOB_ID,
                     samplesToAnalyze = SAMPLE_LIST_OVERRIDE, disableSysUncertainties = TRUE)
