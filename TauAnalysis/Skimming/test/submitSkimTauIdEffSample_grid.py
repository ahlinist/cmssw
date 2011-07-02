#!/usr/bin/env python

from TauAnalysis.TauIdEfficiency.recoSampleDefinitionsTauIdEfficiency_7TeV_grid_cfi import recoSampleDefinitionsTauIdEfficiency_7TeV
from TauAnalysis.Configuration.submitAnalysisToGrid import submitAnalysisToGrid
from TauAnalysis.Configuration.userRegistry import getJobId

import os
import subprocess

channel = 'ZtoMuTau_tauIdEff'
configFile = 'skimTauIdEffSample_cfg.py'
#jobId = getJobId(channel)
jobId = '2011Jun26'

#outputFilePath = '/castor/cern.ch/user/m/mverzett/tagprobe/skims/TauIdEffMeas_2011May13/'
outputFilePath = '/user/v/veelken/CMSSW_4_2_x/skims/TauIdEffMeas/'

pfCandidateCollection = "particleFlow" # pile-up removal disabled
#pfCandidateCollection = "pfNoPileUp"   # pile-up removal enabled

samplesToAnalyze = [
    'data_SingleMu_Run2011A_May10ReReco_v1',
    'data_SingleMu_Run2011A_PromptReco_v4',
    'Ztautau_pythia',
    'Zmumu_pythia',
    'PPmuXptGt20Mu15',
    'WplusJets_madgraph',
    'TTplusJets_madgraph'
]

# Define what output file name a skimmed sample will have
def output_mapper(channel, sample, jobId):
    output_file = "tauIdEffSample_%s_%s_RECO.root" % (sample, jobId)
    return output_file

# Function to prepare customized config files specific to TauIdEff. skim 
def customizeConfigFile(sampleName, cfgFileName_original, cfgFileName_modified = None):
    cfgFile_original = open(cfgFileName_original, "r")
    cfg_original = cfgFile_original.read()
    cfgFile_original.close()

    cfg_modified = cfg_original.replace("#__", "")
    isMC = "False"
    if recoSampleDefinitionsTauIdEfficiency_7TeV['RECO_SAMPLES'][sampleName]['type'] != 'Data':
        isMC = "True"
    cfg_modified = cfg_modified.replace("#isMC#", isMC)
    HLTprocessName = 'HLT'
    if 'hlt' in recoSampleDefinitionsTauIdEfficiency_7TeV['RECO_SAMPLES'][sampleName].keys():
        HLTprocessName = recoSampleDefinitionsTauIdEfficiency_7TeV['RECO_SAMPLES'][sampleName]['hlt'].getProcessName()
    cfg_modified = cfg_modified.replace("#HLTprocessName#", "'%s'" % HLTprocessName)
    cfg_modified = cfg_modified.replace("#pfCandidateCollection#", "'%s'" % pfCandidateCollection)
    applyZrecoilCorrection = "False"
    if 'applyZrecoilCorrection' in recoSampleDefinitionsTauIdEfficiency_7TeV['RECO_SAMPLES'][sampleName].keys() and \
      recoSampleDefinitionsTauIdEfficiency_7TeV['RECO_SAMPLES'][sampleName]['applyZrecoilCorrection']:
        applyZrecoilCorrection = "True"                
    cfg_modified = cfg_modified.replace("#applyZrecoilCorrection#", "%s" % applyZrecoilCorrection)

    if cfgFileName_modified is None:
        cfgFileName_modified = cfgFileName_original.replace("_cfg.py", "_customized_%s_cfg.py" % sampleName)
    cfgFile_modified = open(cfgFileName_modified, "w")
    cfgFile_modified.write(cfg_modified)
    cfgFile_modified.close()

    return cfgFileName_modified

if len(samplesToAnalyze) == 0:
    samplesToAnalyze = recoSampleDefinitionsTauIdEfficiency_7TeV['SAMPLES_TO_ANALYZE']

for sampleToAnalyze in samplesToAnalyze:

    # prepare customized config file as basis for further modifications by "TauAnalysis machinery"...
    configFile_customized = customizeConfigFile(sampleToAnalyze, configFile)

    # apply further modifications and submit job to lxbatch
    submitAnalysisToGrid(configFile = configFile_customized, channel = channel, jobId = jobId,
                         samples = recoSampleDefinitionsTauIdEfficiency_7TeV,
                         samplesToAnalyze = [ sampleToAnalyze ],
                         disableFactorization = True, disableSysUncertainties = True, disableZrecoilCorrections = True,
                         outputFilePath = outputFilePath, outputFileMap = output_mapper)

    # move customized config file to "./crab" subdirectory
    #
    # NOTE: "TauAnalysis machinery" does not work if customized config file
    #       is created in "./crab" subdirectory from the start
    #
    subprocess.call("mv %s crab" % configFile_customized, shell = True)





