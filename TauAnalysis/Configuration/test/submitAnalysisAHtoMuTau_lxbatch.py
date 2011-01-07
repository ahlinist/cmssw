#!/usr/bin/env python
from TauAnalysis.Configuration.recoSampleDefinitionsAHtoMuTau_7TeV_grid_cfi \
        import recoSampleDefinitionsAHtoMuTau_7TeV as samples

import TauAnalysis.Configuration.submitAnalysisToLXBatch as submit
import TauAnalysis.Configuration.userRegistry as reg
import TauAnalysis.Configuration.tools.castor as castor
import os

channel = 'AHtoMuTau'
configFile = 'runAHtoMuTau_cfg.py'

#reg.overrideJobId(channel, 'Run33')
#reg.overrideJobId(channel, 'Run33FR')
reg.overrideJobId(channel, 'Run37skimForMike')
#reg.overrideJobId(channel, 'Run37sysTanc')
#reg.overrideJobId(channel, 'Run39plain')

powheg_samples = [sample for sample in samples['SAMPLES_TO_ANALYZE']
                  if sample.find('POWHEG') != -1 ]

fake_rate_samples = [sample for sample in samples['SAMPLES_TO_ANALYZE']
                     if samples['RECO_SAMPLES'][sample]['enableFakeRates']]

factorized_samples = [sample for sample in samples['SAMPLES_TO_ANALYZE']
                     if samples['RECO_SAMPLES'][sample]['factorize']]

samples_for_mike = ['WplusJets_madgraph',
                    'PPmuXptGt20Mu10',
                    'PPmuXptGt20Mu15',
                    'data_Mu_Run2010A_Nov4ReReco',
                    'data_Mu_Run2010B_Nov4ReReco' ]

# If this is a list, only the items in the list will be analyzed.
samplesToAnalyze = []
#samplesToAnalyze = fake_rate_samples

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
    #return "final_events_%s_%s_%s.root" % (channel, sample, jobId)

enableFakeRates = False
enableSystematics = False
changeTauId = None
saveFinalEvents = False

if jobId == 'Run39FR' or jobId == 'Run39plain':
    for sample in samples['SAMPLES_TO_ANALYZE']:
        if samples['RECO_SAMPLES'][sample]['factorize']:
            print "Disabling factorization for", sample
            samples['RECO_SAMPLES'][sample]['factorize'] = False
    samplesToAnalyze = fake_rate_samples
    if jobId == 'Run39FR':
        enableFakeRates = True
    else:
        saveFinalEvents = True

if jobId == "Run37sys":
    enableSystematics = True

if jobId == "Run37sysTanc":
    enableSystematics = True
    changeTauId = "tauID('byTaNCloose') > 0.5"

if jobId == "Run37skimForMike":
    saveFinalEvents = True
    samplesToAnalyze = samples_for_mike

#for sample in samples['SAMPLES_TO_ANALYZE']:
    #print "Disabling weights"
    #samples['RECO_SAMPLES'][sample]['applyMuonTriggerEfficiencyCorrection'] = False
    #samples['RECO_SAMPLES'][sample]['applyMuonIsolationEfficiencyCorrection'] = False
    #samples['RECO_SAMPLES'][sample]['applyVertexMultiplicityReweighting'] = False

submit.submitAnalysisToLXBatch(
    configFile=configFile,
    channel=channel,
    samples=samples,
    samplesToAnalyze = samplesToAnalyze,
    outputDirectory = outputPath,
    disableSysUncertainties = not enableSystematics,
    enableFakeRates = enableFakeRates,
    inputFileMap = inputFileMapper,
    outputFileMap = outputFileMapper,
    changeTauId = changeTauId,
    processName = 'lxbatch',
    saveFinalEvents = True,
)
