#!/usr/bin/env python
from TauAnalysis.Configuration.recoSampleDefinitionsAHtoMuTau_7TeV_grid_cfi \
        import recoSampleDefinitionsAHtoMuTau_7TeV as samples

import TauAnalysis.Configuration.submitAnalysisToLXBatch as submit
import TauAnalysis.Configuration.userRegistry as reg
import TauAnalysis.Configuration.tools.castor as castor
import os

channel = 'AHtoMuTau'
configFile = 'runAHtoMuTau_cfg.py'

#reg.overrideJobId(channel, 'Run35SYS')
#reg.overrideJobId(channel, 'Run35SYStight')
reg.overrideJobId(channel, 'Run35SYSloose')
#reg.overrideJobId(channel, 'Run33FR')

# If this is a list, only the items in the list will be analyzed.
powheg_samples = [sample for sample in samples['SAMPLES_TO_ANALYZE']
                  if sample.find('POWHEG') != -1 ]

fake_rate_samples = [sample for sample in samples['SAMPLES_TO_ANALYZE']
                     if samples['RECO_SAMPLES'][sample]['enableFakeRates']]

factorized_samples = [sample for sample in samples['SAMPLES_TO_ANALYZE']
                     if samples['RECO_SAMPLES'][sample]['factorize']]

samplesToAnalyze = []

# Where we will send the output on castor
outputPath = reg.getAnalysisFilePath(channel)
jobId = reg.getJobId(channel)
# Figure out where our root files were stored for the desired skim
skimPath = reg.getSkimEvents(channel)

# Get all the skim files from the castor directory
skim_files = [os.path.join(skimPath, file) for file in
    filter(lambda x: x.startswith('skim_'), (
    file_info['file'] for file_info in castor.nslsl(skimPath)))]

def inputFileMapper(channel, sample, jobId):
    for file in skim_files:
        if file.find('_' + sample + '_') != -1:
            yield file

def outputFileMapper(channel, sample, jobId):
    return "plots_%s_%s_%s.root" % (channel, sample, jobId)

enableFakeRates = False
enableSystematics = False
changeTauId = None
# Disable factorization, as we apply muon iso on the Run32 skim
if jobId == 'Run33FR':
    enableFakeRates = True
    for sample in samples['SAMPLES_TO_ANALYZE']:
        if samples['RECO_SAMPLES'][sample]['factorize']:
            print "Disabling factorization for", sample
            samples['RECO_SAMPLES'][sample]['factorize'] = False
if jobId == 'Run35SYS':
    enableFakeRates = False
    enableSystematics = True
if jobId == 'Run35SYSloose':
    enableFakeRates = False
    enableSystematics = True
    changeTauId = "tauID('byTaNCloose') > 0.5"
if jobId == 'Run35SYStight':
    enableFakeRates = False
    enableSystematics = True
    changeTauId = "tauID('byTaNCtight') > 0.5"


submit.submitAnalysisToLXBatch(
    configFile=configFile,
    channel=channel,
    samples=samples,
    samplesToAnalyze = samplesToAnalyze,
    outputDirectory = outputPath,
    jobId = jobId,
    disableSysUncertainties = not enableSystematics,
    enableFakeRates = enableFakeRates,
    inputFileMap = inputFileMapper,
    outputFileMap = outputFileMapper,
    changeTauId = changeTauId,
    processName = 'lxbatch',
)
