#!/usr/bin/env python

import TauAnalysis.Configuration.tools.castor as castor
from TauAnalysis.Configuration.tools.jobtools import make_bsub_script

import os
import re

configFile = 'produceTauIdEffMeasPATTuple_cfg.py'

version = 'v1'

samples = {
    'Data_runs160329to163869' : {
        'skimFilePath' : '/castor/cern.ch/user/v/veelken/CMSSW_4_2_x/skims/GoldenZmumu/runs160329to163869/',
        'numInputFilesPerJob' : 5,
        'HLTprocessName' : 'HLT',
        'isMC' : False
    },
    'Data_runs165071to167913' : {
        'skimFilePath' : '/castor/cern.ch/user/v/veelken/CMSSW_4_2_x/skims/GoldenZmumu/runs165071to167913/',
        'numInputFilesPerJob' : 5,
        'HLTprocessName' : 'HLT',
        'isMC' : False
    },
    'simDYtoMuMu' : {
        'skimFilePath' : '/castor/cern.ch/user/v/veelken/CMSSW_4_2_x/skims/GoldenZmumu/simDYtoMuMu/',
        'numInputFilesPerJob' : 5,
        'HLTprocessName' : 'HLT',
        'isMC' : True
    }
}

executable_bsub = 'bsub'

lxbatch_queue = '1nw'

samplesToAnalyze = [
    'Data_runs160329to163869',
    'Data_runs165071to167913',
    'simDYtoMuMu'
]

outputFilePath = "/castor/cern.ch/user/v/veelken/CMSSW_4_2_x/PATtuples/ZllRecoilCorrection"

if not os.path.isdir("lxbatch"):
    os.mkdir('lxbatch')

configFilePath = os.path.join([ os.getcwd(), "lxbatch" ])

if not os.path.isdir("lxbatch_log"):
    os.mkdir('lxbatch_log')

logFilePath = os.path.join([ os.getcwd(), "lxbatch_log" ])

# Function that maps a sample name to its skim file
def input_mapper(inputFileNames):
    inputFile_regex = \
      r"goldenZmumuEvents_[a-zA-Z0-9_]+_AOD_(?P<gridJob>\d*)(_(?P<gridTry>\d*))*_(?P<hash>[a-zA-Z0-9]*).root" %
    inputFile_matcher = re.compile(inputFile_regex)
    for input_file in inputFileNames:
        if inputFile_matcher.match(inputFileName):
            yield input_file

# Define what output ntuple file name a sample will have
def output_mapper(sample, jobId, version):
    output_file = 'ZllRecoilCorrectionPATtuple_%s_%s_%s.root' % (sample, version, jobId)
    return output_file

# Split list of inputFileNames into groups of size N
def chunks(inputFileNames, N):
    for idx in range(0, len(inputFileNames), N):
        yield inputFileNames[idx:idx + N]

# Function to prepare customized config files specific to Tau(ED)Ntuple production
def customizeConfigFile(sampleName, jobId, version, inputFileNames, cfgFileName_original, cfgFileName_modified = None):
    cfgFile_original = open(cfgFileName_original, "r")
    cfg_original = cfgFile_original.read()
    cfgFile_original.close()

    cfg_modified = cfg_original.replace("#__", "")
    isMC = samples[sampleName]['isMC']
    cfg_modified = cfg_modified.replace("#isMC#", isMC)
    HLTprocessName = samples[sampleName]['HLTprocessName']
    cfg_modified = cfg_modified.replace("#HLTprocessName#", "'%s'" % HLTprocessName)

    cfg_modified.append("process.source.fileNames = cms.vstring('%s')\n" % inputFileNames)
    cfg_modified.append("process.patTupleOutputModule.fileName = cms.untracked.string('%s')\n" %
                        output_mapper(sampleToAnalyze, jobId, version))
    cfg_modified.append("process.savePUreweightHistograms.outputFileName = cms.string('%s')\n" %
                        os.path.join(outputFilePath, 'ZllRecoilCorrectionPUreweightHistograms_%s_%s_%s.root' %
                                     (sampleName, version, jobId)))

    if cfgFileName_modified is None:
        cfgFileName_modified = cfgFileName_original.replace("_cfg.py", "_customized_%s_%s_cfg.py" % (sampleName, jobId))
    cfgFile_modified = open(cfgFileName_modified, "w")
    cfgFile_modified.write(cfg_modified)
    cfgFile_modified.close()

    return cfgFileName_modified

if len(samplesToAnalyze) == 0:
    samplesToAnalyze = samples.keys()

#--------------------------------------------------------------------------------
# Build config files for producing PAT-tuples on lxbatch
bsubScriptFileNames = {}
bsubJobNames        = {}
for sampleToAnalyze in samplesToAnalyze:

    bsubScriptFileNames[sampleToAnalyze] = {}
    bsubJobNames[sampleToAnalyze]        = {}

    inputFileNames = [ file_info['path'] for file_info in castor.nslsl(samples[sampleToAnalyze]['skimFilePath']) ]
    print "inputFileNames = %s" % inputFileNames
    
    inputFileNames_matched = [ input_file for input_file in input_mapper(inputFileNames) ]
    print "inputFileNames_matched = %s" % inputFileNames_matched
    
    for jobId, inputFileNames_chunk in enumerate(chunks(inputFileNames_matched, samples[sampleToAnalyze]['numInputFilesPerJob'])):
        # Build script for batch job submission;
        # the None in the tuple indicates that batch job has no dependencies on other batch jobs
        input_files_and_jobs = \
          [ (None, os.path.join(inputFilePath, inputFileName)) for inputFileName in inputFileNames_chunk ]

        configFileName = customizeConfigFile(sampleToAnalyze, jobId, version, inputFileNames_chunk, configFile)

        outputFileName = output_mapper(sampleToAnalyze, jobId, version)

        logFileName = configFileName.replace('_cfg.py', '.log')
        def log_file_maker(job_hash):
            return os.path.join(logFilePath, logFileName)

        jobName, bsubScript = make_bsub_script(
            os.path.join(outputFilePath, outputFileName),
            input_files_and_jobs,
            log_file_maker,
            "cmsRun %s" % os.path.join(configFilePath, configFileName))
                    
        bsubScriptFileName = \
          os.path.join(configFilePath, logFileName.replace(".log", ".sh"))
        bsubScriptFile = open(bsubScriptFileName, "w")
        bsubScriptFile.write(bsubScript)
        bsubScriptFile.close()

        bsubScriptFileNames[sampleToAnalyze][jobId] = bsubScriptFileName

        bsubJobName = "ZllRecoilPATtuple%s%i" % (sampleToAnalyze, jobId)
        bsubJobNames[sampleToAnalyze][jobId] = bsubJobName
#--------------------------------------------------------------------------------

# create "master" shell script
shFileName_master = "submit_lxbatch_ZllRecoilPATTupleProduction_all.sh"
shFile_master = open(shFileName_master, "w")
for sampleToAnalyze in samplesToAnalyze:
    for jobId in bsubScriptFileNames[sampleToAnalyze].keys():
    shFile_master.write("%s -q %s -J %s < %s\n" %
      (executable_bsub,
       bsubQueue,
       bsubJobNames[sampleToAnalyze][jobId],
       bsubScriptFileNames[sampleToAnalyze][jobId)))
shFile_master.close()

print "\n"
print "Run ./%s to submit **all** jobs" % shFileName_master
os.chmod(shFileName_master, 0755)

