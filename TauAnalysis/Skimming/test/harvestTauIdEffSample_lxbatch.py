#!/usr/bin/env python

import TauAnalysis.Configuration.userRegistry as reg

import os

from TauAnalysis.Configuration.tools.harvestingLXBatch import \
        make_harvest_scripts, castor_source

channel = 'ZtoMuTau_tauIdEff'

reg.overrideJobId(channel, '2010Dec31')

analysisFilePath = reg.getAnalysisFilePath(channel)
harvestingFilePath = reg.getHarvestingFilePath(channel)
jobId = reg.getJobId(channel)
tmpFilePath = reg.getBatchHarvestLocation(channel)

SAMPLES_TO_ANALYZE = [
    'data_Mu_Run2010A_Nov4ReReco',
    'data_Mu_Run2010B_Nov4ReReco',
    'Ztautau_powheg',
    'Zmumu_powheg',
    'PPmuXptGt20Mu10', 'PPmuXptGt20Mu15',
    'WplusJets_madgraph',
    'TTplusJets_madgraph'
]

print analysisFilePath
print tmpFilePath

plot_regex = r"dont match anything"
skim_regex = r"tauIdEffSample_(?P<sample>\w+?)_%s_RECO_(?P<gridJob>\d*)(_(?P<gridTry>\d*))*_(?P<hash>[a-zA-Z0-9]*).root" % (jobId)

def local_copy_mapper(sample):
    " Define where we want to copy the final output locally "
    return os.path.join(
        #'/tmp/friis/Run33SYS',
        #'/tmp/friis/Run32',
        harvestingFilePath,
        "_".join(['harvested', channel, sample, jobId]) + ".root")

make_harvest_scripts(
    plot_regex,
    skim_regex,
    channel,
    castor_source(analysisFilePath),
    tmpFilePath,
    local_copy_mapper = local_copy_mapper,
    chunk_size = 2e9, # 3 GB
)
