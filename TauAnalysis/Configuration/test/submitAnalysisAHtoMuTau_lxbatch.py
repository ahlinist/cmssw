#!/usr/bin/env python
from TauAnalysis.Configuration.recoSampleDefinitionsAHtoMuTau_7TeV_grid_cfi \
        import recoSampleDefinitionsAHtoMuTau_7TeV as samples

import TauAnalysis.Configuration.submitAnalysisToLXBatch as submit
import TauAnalysis.Configuration.userRegistry as reg
import TauAnalysis.Configuration.tools.castor as castor
import os

channel = 'AHtoMuTau'
configFile = 'runAHtoMuTau_cfg.py'

reg.overrideJobId(channel, 'Run33SYS')

# If this is a list, only the items in the list will be analyzed.
samplesToAnalyze = None

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

enableSystematics = True
enableFakeRates = False

# If we are running with fake rates, only analyze samples when it matters.
if enableFakeRates:
    samplesToAnalyze = [sample for sample in samples['SAMPLES_TO_ANALYZE']
                        if samples['RECO_SAMPLES'][sample]['enableFakeRates']]

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
    processName = 'lxbatch',
)
