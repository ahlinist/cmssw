#!/usr/bin/env python

'''

Harvest ZtoMuTau plots on LXBATCH

'''

import TauAnalysis.Configuration.userRegistry as reg

import os
import re

from TauAnalysis.Configuration.tools.harvestingLXBatch import make_harvest_scripts
from TauAnalysis.Configuration.tools.harvesting import castor_source, clean_by_crab_id

channel = 'ZtoMuTau'

reg.overrideJobId(channel, '2011Apr09_HPSloose')

analysisFilePath = reg.getAnalysisFilePath(channel)
harvestingFilePath = reg.getHarvestingFilePath(channel)
jobId = reg.getJobId(channel)
tmpFilePath = reg.getBatchHarvestLocation(channel)

SAMPLES_TO_ANALYZE = [
    'data_TauPlusX_Run2011A_PromptReco',
    'DYtautauM10to20_powheg',
    'Ztautau_powheg',
    'qqZll',
    'DYmumuM10to20_pythia',
    'Zmumu_powheg',
    'PPmuXptGt20Mu15',
    'WplusJets_madgraph',
    'WW',
    'WZ',
    'ZZ',
    'TTplusJets_madgraph'
]

print analysisFilePath
print tmpFilePath
    
plot_regex = r"plots_%s_(?P<sample>\w+?)_%s_(?P<gridJob>\d*)(_(?P<gridTry>\d*))*_(?P<hash>[a-zA-Z0-9]*).root" % (channel, jobId)
skim_regex = r"final_events_%s_(?P<sample>\w+?)_%s_(?P<gridJob>\d*)(_(?P<gridTry>\d*))*_(?P<hash>[a-zA-Z0-9]*).root" % (channel, jobId)

def matches_either(files):
    # Check if the file matches either of the regexes we are interested in.
    # We do this to skip extra files in the directories before we pass them to
    # clean_by_crab_id
    plot_matcher = re.compile(plot_regex)
    skim_matcher = re.compile(skim_regex)
    for file in files:
        if plot_matcher.match(file['file']) or skim_matcher.match(file['file']):
            yield file

def local_copy_mapper(sample):
    " Define where we want to copy the final output locally "
    return os.path.join(
        harvestingFilePath,
        "_".join(['harvested', channel, sample, jobId]) + ".root")            

make_harvest_scripts(
    plot_regex,
    skim_regex,
    channel,
    clean_by_crab_id(matches_either(castor_source(analysisFilePath))),
    tmpFilePath,
    local_copy_mapper = local_copy_mapper,
    chunk_size = 2.e+9, # 3 GB
)
