import FWCore.ParameterSet.Config as cms

import os
import subprocess

import TauAnalysis.Configuration.tools.castor as castor
from TauAnalysis.Configuration.tools.harvestingMakefile import buildMakefile

def harvestAnalysisResults(channel = None, samples = None, inputFilePath = None, outputFilePath = None, jobId = None,
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

    print "Finding CASTOR files"
    inputFilePath = '/castor/cern.ch' + '/' + inputFilePath
    inputFilePath = inputFilePath.replace('//', '/')
    print(" inputFilePath = " + inputFilePath)
    files_in_castor = castor.nsls(inputFilePath)

    files_to_process = [file for file in files_in_castor if file.find('_%s_' % jobId) != -1]

    # Sort files by modified time
    print "Sorting by modified time"
    if len(files_to_process) > 0: 
        print castor.last_modified(files_to_process[1])
    files_and_times = [ 
        (castor.last_modified(file), file)
        for file in files_to_process
    ]

    for time, file in files_and_times:
        pass

    files_and_times.sort()

    harvest_jobs = []

    for sample in samples['SAMPLES_TO_ANALYZE']:
        print "Finding input files for", sample
        output_file = "harvested_%s_%s_%s.root" % (channel, sample, jobId)
        output_path = os.path.join(outputFilePath, output_file)
        files_to_merge = list('rfio:%s' % file for time, file in files_and_times 
                          if file.find('_%s_%s_%s_' % (channel, sample, jobId)) != -1)

        harvest_jobs.append( (sample, output_path, files_to_merge) )

    print "Creating Makefile"
    MakefileName = 'Makefile.harvest_%s_%s' % (channel, jobId)
    buildMakefile(harvest_jobs, tmpFilePath, MakefileName, merge_per_job = 7)

    print "Starting harvesting..."
    makeCommand = "make -f %s -j 8 -k" % MakefileName
    subprocess.call(makeCommand, shell = True)

