#!/usr/bin/env python

from TauAnalysis.Configuration.recoSampleDefinitionsZtoMuTau_7TeV_grid_cfi import recoSampleDefinitionsZtoMuTau_7TeV
from TauAnalysis.Configuration.harvestAnalysisResults_grid import harvestAnalysisResults
from TauAnalysis.Configuration.userRegistry import getAnalysisFilePath, getHarvestingFilePath, getJobId, getTmpFilePath

import os
import subprocess

import TauAnalysis.Configuration.tools.castor as castor
from TauAnalysis.Configuration.tools.harvestingMakefile import buildMakefile

channel = 'ZtoMuTau_tauIdEff'
analysisFilePath = getAnalysisFilePath(channel)
harvestingFilePath = getHarvestingFilePath(channel)
jobId = getJobId(channel)
tmpFilePath = getTmpFilePath(channel)

SAMPLES_TO_ANALYZE = [
    'data_Mu_Run2010A_Nov4ReReco',
    'data_Mu_Run2010B_Nov4ReReco',
    'Ztautau_powheg',
    'Zmumu_powheg',
    'PPmuXptGt20Mu10', 'PPmuXptGt20Mu15',
    'WplusJets_madgraph',
    'TTplusJets_madgraph'
]

def harvestTauIdEffSamples(channel = None, samples = None, inputFilePath = None,
                           outputFilePath = None, jobId = None,
                           tmpFilePath = None):

    # check that channel, samples, inputFilePath, outputFilePath, tmpFilePath and jobId
    # parameters are defined and non-empty
    if channel is None:
        raise ValueError("Undefined channel Parameter !!")
    if samples is None:
        raise ValueError("Undefined samples Parameter !!")
    if inputFilePath is None:
        raise ValueError("Undefined inputFilePath Parameter !!")
    if outputFilePath is None:
        raise ValueError("Undefined outputFilePath Parameter !!")
    if tmpFilePath is None:
        raise ValueError("Undefined tmpFilePath Parameter !!")
    if jobId is None:
        raise ValueError("Undefined jobId Parameter !!")

    if not os.path.exists(tmpFilePath):
        os.mkdir(tmpFilePath)
    if not os.path.exists(outputFilePath):
        os.mkdir(outputFilePath)

    # Use CASTOR to find the files to merge
    print "Finding CASTOR files"
    inputFilePath = '/castor/cern.ch' + '/' + inputFilePath
    inputFilePath = inputFilePath.replace('//', '/')
    print(" inputFilePath = " + inputFilePath)

    files_in_castor_info = castor.nslsl(inputFilePath)

    files_and_times = [
        (file_info['time'], file_info['path'])
        for file_info in files_in_castor_info
        if file_info['file'].find('_%s_' % jobId) != -1 ]
    # Sort files by modified time
    print "Sorting by modified time"
    files_and_times.sort()

    skim_harvest_jobs = []

    for sample in SAMPLES_TO_ANALYZE:
        print "Finding input files for", sample
        # Get final event skims that need to be merged
        event_files_to_merge = list(
            'rfio:%s' % file for time, file in files_and_times
            if file.find('tauIdEffSample_%s_%s_' %
                         (sample, jobId)) != -1)
        skim_output_path = os.path.join(
            outputFilePath, "tauIdEffSkim_%s_%s.root" % (sample, jobId))
        skim_harvest_jobs.append(
            (sample, skim_output_path, event_files_to_merge))

    print "Creating Makefile for skimmed event files"
    skim_MakefileName = "Makefile.mergeTauIdEffSkims_%s" % (jobId)
    buildMakefile(skim_harvest_jobs, tmpFilePath, skim_MakefileName,
                  merge_per_job = 7, harvest_tool = 'genericSkimMerger.py')

    print "Makefile built. In order to start harvesting, execute 'make -f %s -j 8 -k'" % skim_MakefileName

harvestTauIdEffSamples(channel = channel, samples = recoSampleDefinitionsZtoMuTau_7TeV,
                       inputFilePath = analysisFilePath, outputFilePath = harvestingFilePath, jobId = jobId,
                       tmpFilePath = tmpFilePath)
