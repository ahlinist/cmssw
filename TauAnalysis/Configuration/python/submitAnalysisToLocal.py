import FWCore.ParameterSet.Config as cms

import copy
import glob
import os
import re
import subprocess
import shlex

from TauAnalysis.Configuration.prepareConfigFile2 import getNewConfigFileName, prepareConfigFile, PLOT_FILES_PREFIX
from TauAnalysis.Configuration.userRegistry import getAnalysisFilePath, getJobId, getPickEventsPath

def getInputFiles(jobInfo = None, inputFileMap = None, maxEvents = 25000):

    #print("<getInputFiles>:")
    #print(" maxEvents = %i" % maxEvents) 
    
    # check that jobInfo and inputFileMap
    # parameters are defined and non-empty
    if jobInfo is None:
        raise ValueError("Undefined 'jobInfo' Parameter !!")
    if inputFileMap is None:
        raise ValueError("Undefined 'inputFileMap' Parameter !!")

    inputFilePath = getPickEventsPath(jobInfo['channel'])

    inputFilePattern = inputFileMap(jobInfo['channel'], jobInfo['sample'], jobInfo['id'])
    if inputFilePattern.rfind('/') != -1:
        inputFilePattern = inputFilePattern[inputFilePattern.rfind('/') + 1:]
    inputFilePattern = inputFilePattern.replace("*", "[0-9a-zA-Z]*")

    files = None

    if inputFilePath.find("/castor") != -1:
        raise ValueError("Input files on castor not supported yet !!")
    else:
        commandLine = 'ls %s' % inputFilePath
        args = shlex.split(commandLine)
        retVal = subprocess.Popen(args, stdout = subprocess.PIPE)
        retVal.wait()
        
        files = retVal.stdout.read().split()

    fileInfos = []    

    for file in files:
        if not re.match(inputFilePattern, file):
            continue
        
        fileName = None
        if inputFilePath.find("/castor") != -1:
            fileName = "rfio:%s" % os.path.join(inputFilePath, file)
        else:
            fileName = "file:%s" % os.path.join(inputFilePath, file)
        commandLine = 'edmFileUtil %s' % fileName
        #print("commandLine = '%s'" % commandLine)
        args = shlex.split(commandLine)
        retval = subprocess.Popen(args, stdout = subprocess.PIPE)
        retval.wait()
            
        items = retval.stdout.read().split()
        #print(items)

        numEvents = int(items[3])
	print("file %s has %i events" % (fileName, numEvents))

        numJobs = (numEvents // maxEvents)
	if (numEvents % maxEvents) > 0:
	    numJobs = numJobs + 1

        if numJobs == 1:
            fileInfos.append({
                'fileName' : fileName,
                'maxEvents' : -1,
                'skipEvents' : 0
            })
        else:
            for job in range(numJobs):
                fileInfos.append({
                    'fileName' : fileName,
                    #'fileName' : fileName.replace(".root", "_%i.root" % (job + 1)),
                    'maxEvents' : maxEvents,
                    #'maxEvents' : 10,
                    'skipEvents' : (job*maxEvents)
                })
                
    #print("fileInfos:")
    #print(fileInfos)

    return fileInfos

def submitAnalysisToLocal(configFile = None, channel = None, samples = None, jobId = None,
                          samplesToAnalyze = None, samplesToSkip = None,
                          disableFactorization = False,
                          disableSysUncertainties = False,
                          maxEvents = 25000, maxJobsConcurrently = 8,
                          cfgdir = 'local', submit = False, logFilePath = None,
                          inputFileMap = None, outputFileMap = None,
                          enableEventDumps = False,
                          enableFakeRates = False,
                          processName = None,
                          saveFinalEvents = False):
    
    """
    Submit analysis job (event selection, filling of histogram)
    to local machine
    """

    # check that configFile, channel, samples and jobId
    # parameters are defined and non-empty
    if configFile is None:
        raise ValueError("Undefined 'configFile' Parameter !!")
    if channel is None:
        raise ValueError("Undefined 'channel' Parameter !!")
    if samples is None:
        raise ValueError("Undefined 'samples' Parameter !!")
    if jobId is None:
        raise ValueError("Undefined 'jobId' Parameter !!")
    if logFilePath is None:
        raise ValueError("Undefined 'logFilePath' Parameter !!")

    cfgFiles = []

    # Loop over the samples to be analyzed
    for sample in samples['SAMPLES_TO_ANALYZE']:
        # Skip submitting crab job in case
        #  o list of samples for which crab jobs are to be submitted has been
        #    explicitely specified
        #  o sample has explicitely been requested to be skipped
        if samplesToAnalyze:
            if sample not in samplesToAnalyze:
                print "Skipping", sample
                continue
        if samplesToSkip:
            if sample in samplesToSkip:
                print "Skipping", sample
                continue

        sample_info = samples['RECO_SAMPLES'][sample]
        
        # Make job info
        jobInfo = {
            'channel' : channel,
            'sample' : sample,
            'id' : jobId
        }

        newConfigFile = getNewConfigFileName(configFile, cfgdir, sample, jobId, label = "@local")

        fileInfos = getInputFiles(jobInfo, inputFileMap, maxEvents = maxEvents)
        if len(fileInfos) > 0:
            print("Submitting %s in %i part(s)" % (sample, len(fileInfos)))
        else:
            print("No local input files for %s found !!" % sample)

        for job, fileInfo in enumerate(fileInfos):
            print("job %i:" % (job + 1))
            
            sample_infos = copy.deepcopy(samples)

            sample_infos['RECO_SAMPLES'][sample].update({ 'maxEvents'  : fileInfo['maxEvents']  })
            sample_infos['RECO_SAMPLES'][sample].update({ 'skipEvents' : fileInfo['skipEvents'] })

            input_files = [ fileInfo['fileName'] ]

            output_file = outputFileMap(channel, sample, jobId)
            if len(fileInfos) > 1:
                output_file = output_file.replace(".root", "_%i.root" % (job + 1))
                #print(" output_file = %s" % output_file)

            jobConfigFile = newConfigFile
            if len(fileInfos) > 1:
                jobConfigFile = jobConfigFile.replace("_cfg.py", "_%i_cfg.py" % (job + 1))

            #--------------------------------------------------------------------
            # CV: temporary "hack" for producing (ED)Ntuples for tau id. efficiency measurement
            jobCustomizations = []
            jobCustomizations.append("if hasattr(process, 'ntupleOutputModule'):")
            jobCustomizations.append("    process.ntupleOutputModule.fileName = '%s'" % output_file)
            HLTprocessName = 'HLT'
            if 'hlt' in sample_infos['RECO_SAMPLES'][sample].keys():
                HLTprocessName = sample_infos['RECO_SAMPLES'][sample]['hlt'].getProcessName()
            jobCustomizations.append("if hasattr(process, 'hltMu'):")
            jobCustomizations.append("    process.hltMu.selector.src = cms.InputTag('TriggerResults::%s')" % HLTprocessName)
            jobCustomizations.append("process.patTrigger.processName = '%s'" % HLTprocessName)
            jobCustomizations.append("process.patTriggerEvent.processName = '%s'" % HLTprocessName)
            jobCustomizations.append("if hasattr(process, 'prePatProductionSequence'):")
            jobCustomizations.append("    process.prePatProductionSequence.remove(process.prePatProductionSequenceGen)")
            if sample_infos['RECO_SAMPLES'][sample]['type'] == 'Data':
                jobCustomizations.append("if hasattr(process, 'ntupleProducer'):")
                jobCustomizations.append("    delattr(process.ntupleProducer.sources, 'tauGenJets')")
                jobCustomizations.append("    delattr(process.ntupleProducer.sources, 'genJets')")
                jobCustomizations.append("    delattr(process.ntupleProducer.sources, 'genPhaseSpaceEventInfo')")
                jobCustomizations.append("    delattr(process.ntupleProducer.sources, 'genPileUpEventInfo')")
            #jobCustomizations.append("print process.dumpPython()")
            #--------------------------------------------------------------------

            prepareConfigFile(
              configFile = configFile, jobInfo = jobInfo, newConfigFile = jobConfigFile,
              sample_infos = sample_infos,
              disableFactorization = disableFactorization,
              disableSysUncertainties = disableSysUncertainties,
              input_files = input_files, output_file = output_file,
              enableEventDumps = enableEventDumps, enableFakeRates = enableFakeRates,
              processName = processName,
              saveFinalEvents = saveFinalEvents,
              customizations = jobCustomizations)

            cfgFiles.append(jobConfigFile)

    # Build a script to run all jobs locally

    script = '''
#!/usr/bin/env python

import os
import subprocess
import shlex
import threading
import Queue
    
class Worker(threading.Thread):

    def __init__(self, work_queue):
        super(Worker, self).__init__()
        self.work_queue = work_queue

    def run(self):
        while True:
            try:
                cfgFileName = self.work_queue.get()
                self.process(cfgFileName)
            finally:
                self.work_queue.task_done()
    '''
    script += "\n" 
    script += "    def process(self, cfgFileName):\n"
    script += "        logFilePath = '%s'\n" % logFilePath
    script += "        logFileName = os.path.basename(cfgFileName).replace('_cfg.py', '.log')\n"
    script += "        commandLine = 'rm -f %s' % os.path.join(logFilePath, logFileName)\n"
    script += "        commandLine = 'nice cmsRun %s >& %s' % (cfgFileName, os.path.join(logFilePath, logFileName))\n"
    script += "        print('calling %s...' % commandLine)\n"
    script += "        subprocess.call(commandLine, shell = True)\n"
    script += "\n"  
    script += "work_queue = Queue.Queue()\n"
    script += "\n"
    script += "for iThread in range(%i):\n" % maxJobsConcurrently
    script += "    worker = Worker(work_queue)\n"
    script += "    worker.daemon = True\n"
    script += "    worker.start()\n"
    script += "\n"
    for cfgFile in cfgFiles:
        script += "work_queue.put('%s')\n" % cfgFile
    script += "\n"
    script += "work_queue.join()\n"

    scriptFileName = 'local/runAnalysis_%s_%s.py' % (channel, jobId)
    scriptFile = open(scriptFileName, 'w')
    scriptFile.write(script)

    print("Built %s script" % scriptFileName)

    if submit:
        subprocess.call("python %s" % scriptFileName, shell = True)

    
