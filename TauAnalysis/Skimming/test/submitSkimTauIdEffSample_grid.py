#!/usr/bin/env python

from TauAnalysis.Configuration.recoSampleDefinitionsZtoMuTau_7TeV_grid_cfi import recoSampleDefinitionsZtoMuTau_7TeV
from TauAnalysis.Configuration.submitAnalysisToGrid import submitAnalysisToGrid
from TauAnalysis.Configuration.userRegistry import getJobId

import os
import subprocess

channel = 'ZtoMuTau_tauIdEff'
configFile = 'skimTauIdEffSample_cfg.py'
#jobId = getJobId(channel)
jobId = '2011Feb03b'

outputFilePath = '/user/v/veelken/CMSSW_3_8_x/skims/TauIdEffMeas/'

pfCandidateCollection = "particleFlow" # pile-up removal disabled
#pfCandidateCollection = "pfNoPileUp"   # pile-up removal enabled

samplesToAnalyze = [
    # modify in case you want to submit jobs for some of the samples only...
    'data_Mu_Run2010A_Nov4ReReco',
    #'data_Mu_Run2010B_Nov4ReReco',
    #'ZtautauPU156bx',
    #'Zmumu_pythia', 
    #'PPmuXptGt20Mu10', 'PPmuXptGt20Mu15',
    #'WplusJets_madgraph',
    #'TTplusJets_madgraph'
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
    if recoSampleDefinitionsZtoMuTau_7TeV['RECO_SAMPLES'][sampleName]['type'] != 'Data':
        isMC = "True"
    cfg_modified = cfg_modified.replace("#isMC#", isMC)
    HLTprocessName = 'HLT'
    if 'hlt' in recoSampleDefinitionsZtoMuTau_7TeV['RECO_SAMPLES'][sampleName].keys():
        HLTprocessName = recoSampleDefinitionsZtoMuTau_7TeV['RECO_SAMPLES'][sampleName]['hlt'].getProcessName()
    cfg_modified = cfg_modified.replace("#HLTprocessName#", "'%s'" % HLTprocessName)
    cfg_modified = cfg_modified.replace("#pfCandidateCollection#", "'%s'" % pfCandidateCollection)
    applyZrecoilCorrection = "False"
    if 'applyZrecoilCorrection' in recoSampleDefinitionsZtoMuTau_7TeV['RECO_SAMPLES'][sampleName].keys() and \
      recoSampleDefinitionsZtoMuTau_7TeV['RECO_SAMPLES'][sampleName]['applyZrecoilCorrection']:
        applyZrecoilCorrection = "True"                
    cfg_modified = cfg_modified.replace("#applyZrecoilCorrection#", "%s" % applyZrecoilCorrection)

    if cfgFileName_modified is None:
        cfgFileName_modified = cfgFileName_original.replace("_cfg.py", "_customized_%s_cfg.py" % sampleName)
    cfgFile_modified = open(cfgFileName_modified, "w")
    cfgFile_modified.write(cfg_modified)
    cfgFile_modified.close()

    return cfgFileName_modified

if len(samplesToAnalyze) == 0:
    samplesToAnalyze = recoSampleDefinitionsZtoMuTau_7TeV['SAMPLES_TO_ANALYZE']

for sampleToAnalyze in samplesToAnalyze:

    # prepare customized config file as basis for further modifications by "TauAnalysis machinery"...
    configFile_customized = customizeConfigFile(sampleToAnalyze, configFile)

    # apply further modifications and submit job to lxbatch
    submitAnalysisToGrid(configFile = configFile_customized, channel = channel, jobId = jobId,
                         samples = recoSampleDefinitionsZtoMuTau_7TeV,
                         samplesToAnalyze = [ sampleToAnalyze ],
                         disableFactorization = True, disableSysUncertainties = True, disableZrecoilCorrections = True,
                         outputFilePath = outputFilePath, outputFileMap = output_mapper)

    # move customized config file to "./crab" subdirectory
    #
    # NOTE: "TauAnalysis machinery" does not work if customized config file
    #       is created in "./crab" subdirectory from the start
    #
    subprocess.call("mv %s crab" % configFile_customized, shell = True)





