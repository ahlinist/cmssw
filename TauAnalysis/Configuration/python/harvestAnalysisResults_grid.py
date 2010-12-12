import FWCore.ParameterSet.Config as cms

import os
import subprocess

import TauAnalysis.Configuration.tools.castor as castor
import TauAnalysis.Configuration.tools.crab as crab
from TauAnalysis.Configuration.tools.harvestingMakefile import buildMakefile

def harvestAnalysisResults(channel = None, samples = None, inputFilePath = None,
                           outputFilePath = None, jobId = None,
                           tmpFilePath = None,
                           # Pre-scale and factorize samples
                           ana_defs = None, plot_defs = None, plotters = None,
                           use_job_report = False):

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

    files_and_times = []

    #if not ana_defs or not use_job_report:
    if not use_job_report:
        # Use CASTOR to find the files to merge
        print "Finding CASTOR files"
        inputFilePath = '/castor/cern.ch' + '/' + inputFilePath
        inputFilePath = inputFilePath.replace('//', '/')
        print(" inputFilePath = " + inputFilePath)
        files_in_castor = castor.nsls(inputFilePath)

        files_to_process = [file for file in files_in_castor
                            if file.find('_%s_' % jobId) != -1]

        # Sort files by modified time
        print "Sorting by modified time"
        if len(files_to_process) > 0:
            print castor.last_modified(files_to_process[1])
        files_and_times = [
            (castor.last_modified(file), file)
            for file in files_to_process
        ]
        files_and_times.sort()

    else:
        print "Using job reports to find output files"
        for sample in samples['SAMPLES_TO_ANALYZE']:
            crab_dir = os.path.join(
                'crab', 'crabdir_run%s_%s_%s' % (channel, sample, jobId))
            print "Getting output files from:", crab_dir
            files_and_times.extend(
                (None, file) for file in crab.map_lfns_to_castor(
                    crab.lfns(crab_dir)))

    plot_harvest_jobs = []
    skim_harvest_jobs = []

    for sample in samples['SAMPLES_TO_ANALYZE']:
        print "Finding input files for", sample
        output_file = "harvested_%s_%s_%s.root" % (channel, sample, jobId)
        output_path = os.path.join(outputFilePath, output_file)
        files_to_merge = list(
            'rfio:%s' % file for time, file in files_and_times
            if file.find('plots_%s_%s_%s_' % (channel, sample, jobId)) != -1)
        plot_harvest_jobs.append( (sample, output_path, files_to_merge) )
        # Get final event skims that need to be merged
        event_files_to_merge = list(
            'rfio:%s' % file for time, file in files_and_times
            if file.find('final_events_%s_%s_%s_' %
                         (channel, sample, jobId)) != -1)
        skim_output_path = os.path.join(
            outputFilePath, "skim_%s_%s_%s.root" % (channel, sample, jobId))
        skim_harvest_jobs.append(
            (sample, skim_output_path, event_files_to_merge))

    print "Creating Makefile for histogram files"
    MakefileName = 'Makefile.harvest_%s_%s' % (channel, jobId)
    buildMakefile(plot_harvest_jobs, tmpFilePath, MakefileName,
                  merge_per_job = 7, ana_defs = ana_defs, plot_defs = plot_defs,
                  plotters = plotters)

    print "Creating Makefile for skimmed event files"
    skim_MakefileName = "Makefile.mergeSkims_%s_%s" % (channel, jobId)
    buildMakefile(skim_harvest_jobs, tmpFilePath, skim_MakefileName,
                  merge_per_job = 7, harvest_tool = 'genericSkimMerger.py')

    print "Makefile built. In order to start harvesting, execute 'make -f %s -j 8 -k'" % MakefileName

    #print "Starting harvesting..."
    #makeCommand = "make -f %s -j 8 -k" % MakefileName
    #subprocess.call(makeCommand, shell = True)

