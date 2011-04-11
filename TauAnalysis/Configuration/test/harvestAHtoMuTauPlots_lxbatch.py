#!/usr/bin/env python

'''

Harvest AHtoMu tau plots on LXBATCH

'''

from TauAnalysis.Configuration.tools.harvestingLXBatch import \
        make_harvest_scripts

from TauAnalysis.Configuration.tools.harvesting import \
        castor_source, crabdir_sources, \
        clean_by_crab_id

import TauAnalysis.Configuration.userRegistry as reg
import os
import glob

channel = 'AHtoMuTau'

for id in ['RunSVTestApr04']:
    reg.overrideJobId(channel, id)
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
            #'/tmp/friis/%s' % jobId,
            "_".join(['harvested', channel, sample, jobId]) + ".root")

    #crab_dirs = glob.glob('crab/crabdir_*_%s' % jobId)
    crab_dirs = [
        'crab/crabdir_runAHtoMuTau_%s_%s' % (sample, jobId)
        for sample in [
            #'data_Mu_Run2010A_Nov4ReReco',
            #'data_Mu_Run2010B_Nov4ReReco',
            #'PPmuXptGt20Mu15',
            'Zmumu_powheg'
        ]
    ]

    make_harvest_scripts(
        plot_regex,
        skim_regex,
        channel,
        #analysisFilePath,
        castor_source(analysisFilePath),
        #clean_by_crab_id(castor_source(analysisFilePath)),
        #crabdir_sources(*crab_dirs),
        tmpFilePath,
        local_copy_mapper = local_copy_mapper
    )
