#!/usr/bin/env python

from TauAnalysis.Skimming.recoSampleDefinitionsGoldenZmumu_7TeV_grid_cfi import recoSampleDefinitionsGoldenZmumu_7TeV
from TauAnalysis.Configuration.submitAnalysisToGrid import submitAnalysisToGrid
from TauAnalysis.Configuration.userRegistry import getJobId

import os
import subprocess

configFile = 'skimGoldenZmumu_cfg.py'
jobId = '2012Apr12'

outputFilePath = '/castor/cern.ch/user/v/veelken/CMSSW_5_2_x/skims/GoldenZmumu/%s/' % jobId

samplesToAnalyze = [
    ##'Data_runs190456to190688'
    ##'Data_runs190456to191276'
    'Data_runs190456to191859' 
    ##'ZplusJets_madgraph',
    ##'TTplusJets_madgraph'
]

# Define what output file name a skimmed sample will have
def output_mapper(channel, sample, jobId):
    output_file = "goldenZmumuEvents_%s_%s_AOD.root" % (sample, jobId)
    return output_file

# Function to prepare customized config files specific to TauIdEff. skim 
def customizeConfigFile(sampleName, cfgFileName_original, cfgFileName_modified = None):
    cfgFile_original = open(cfgFileName_original, "r")
    cfg_original = cfgFile_original.read()
    cfgFile_original.close()

    cfg_modified = cfg_original.replace("#__", "")
    isMC = "False"
    if recoSampleDefinitionsGoldenZmumu_7TeV['RECO_SAMPLES'][sampleName]['type'] != 'Data':
        isMC = "True"
    cfg_modified = cfg_modified.replace("#isMC#", isMC)

    if cfgFileName_modified is None:
        cfgFileName_modified = cfgFileName_original.replace("_cfg.py", "_customized_%s_cfg.py" % sampleName)
    cfgFile_modified = open(cfgFileName_modified, "w")
    cfgFile_modified.write(cfg_modified)
    cfgFile_modified.close()

    return cfgFileName_modified

if len(samplesToAnalyze) == 0:
    samplesToAnalyze = recoSampleDefinitionsGoldenZmumu_7TeV['SAMPLES_TO_ANALYZE']

for sampleToAnalyze in samplesToAnalyze:

    # prepare customized config file as basis for further modifications by "TauAnalysis machinery"...
    configFile_customized = customizeConfigFile(sampleToAnalyze, configFile)

    # apply further modifications and submit job to grid
    submitAnalysisToGrid(configFile = configFile_customized, channel = "goldenZmumu", jobId = jobId,
                         samples = recoSampleDefinitionsGoldenZmumu_7TeV,
                         samplesToAnalyze = [ sampleToAnalyze ],
                         disableFactorization = True, disableSysUncertainties = True, disableZrecoilCorrections = True,
                         outputFilePath = outputFilePath, outputFileMap = output_mapper, savePlots = False)

    # move customized config file to "./crab" subdirectory
    #
    # NOTE: "TauAnalysis machinery" does not work if customized config file
    #       is created in "./crab" subdirectory from the start
    #
    subprocess.call("mv %s crab" % configFile_customized, shell = True)





