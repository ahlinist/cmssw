#!/usr/bin/env python

'''

Harvest AHtoMu tau plots on LXBATCH

'''

from TauAnalysis.Configuration.tools.harvestingLXBatch import \
        make_harvest_scripts
import TauAnalysis.Configuration.userRegistry as reg
import os

channel = 'AHtoMuTau'

reg.overrideJobId(channel, 'Run33')
analysisFilePath = reg.getAnalysisFilePath(channel)
harvestingFilePath = reg.getHarvestingFilePath(channel)
jobId = reg.getJobId(channel)
tmpFilePath = reg.getBatchHarvestLocation(channel)

# Regexes that define how to parse the filename of a file on castor
plot_regex = r"plots_%s_(?P<sample>\w+?)_%s_(?P<gridJob>\d*)(_(?P<gridTry>\d*))*_(?P<hash>[a-zA-Z0-9]*).root" % (channel, jobId)
skim_regex = r"final_events_%s_(?P<sample>\w+?)_%s_(?P<gridJob>\d*)(_(?P<gridTry>\d*))*_(?P<hash>[a-zA-Z0-9]*).root" % (channel, jobId)

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
    analysisFilePath,
    tmpFilePath,
    local_copy_mapper = local_copy_mapper
)
