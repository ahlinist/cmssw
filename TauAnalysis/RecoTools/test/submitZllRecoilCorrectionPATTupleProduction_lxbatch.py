#!/usr/bin/env python

import TauAnalysis.Configuration.tools.castor as castor
from TauAnalysis.Configuration.tools.jobtools import make_bsub_script
from TauAnalysis.Configuration.tools.harvestingLXBatch import make_harvest_scripts

import os
import re
import time

configFile = 'produceZllRecoilCorrectionPATTuple_cfg.py'

version = 'v5_0'

samples = {
    'Data_runs190450to190892' : {
        'skimFilePath' : '/castor/cern.ch/user/v/veelken/CMSSW_5_2_x/skims/GoldenZmumu/2012Apr12/',
        'numInputFilesPerJob' : 5,
        'HLTprocessName' : 'HLT',
        'isMC' : False
    },
    'ZplusJets_madgraph' : {
        'skimFilePath' : '/castor/cern.ch/user/v/veelken/CMSSW_5_2_x/skims/GoldenZmumu/2012Apr12/',
        'numInputFilesPerJob' : 3,
        'HLTprocessName' : 'HLT',
        'isMC' : True
    },
    'TTplusJets_madgraph' : {
        'skimFilePath' : '/castor/cern.ch/user/v/veelken/CMSSW_5_2_x/skims/GoldenZmumu/2012Apr12/',
        'numInputFilesPerJob' : 3,
        'HLTprocessName' : 'HLT',
        'isMC' : True
    }
}

lxbatch_queue = '1nw'

samplesToAnalyze = [
    #'Data_runs190450to190892',
    'ZplusJets_madgraph',
    #'TTplusJets_madgraph'
]

skipExistingPATtuples = True

outputFilePath = "/castor/cern.ch/user/v/veelken/CMSSW_5_2_x/PATtuples/ZllRecoilCorrection/%s" % version

executable_bsub = 'bsub'
executable_waitForLXBatchJobs = 'python %s/src/TauAnalysis/Configuration/python/tools/waitForLXBatchJobs.py' % os.environ['CMSSW_BASE']
executable_rfrm = '- rfrm' # CV: ignore error code returned by 'rfrm' in case file on castor does not exist
executable_hadd = 'hadd -f'
executable_shell = '/bin/csh'

try:
    castor.rfstat(outputFilePath)
except RuntimeError:
    # outputFilePath does not yet exist, create it
    print "outputFilePath does not yet exist, creating it."
    os.system("rfmkdir %s" % outputFilePath)
    os.system("rfchmod 777 %s" % outputFilePath)

if not os.path.isdir("lxbatch"):
    os.mkdir('lxbatch')

configFilePath = os.path.join(os.getcwd(), "lxbatch")

if not os.path.isdir("lxbatch_log"):
    os.mkdir('lxbatch_log')

logFilePath = os.path.join(os.getcwd(), "lxbatch_log")

# Function that maps a sample name to its skim file
def input_mapper(inputFileNames, sampleToAnalyze):
    inputFile_regex = \
      r"[a-zA-Z0-9_/:.]*goldenZmumuEvents_%s_[a-zA-Z0-9]+_AOD_(?P<gridJob>\d*)(_(?P<gridTry>\d*))*_(?P<hash>[a-zA-Z0-9]*).root" % \
        sampleToAnalyze
    inputFile_matcher = re.compile(inputFile_regex)
    for input_file in inputFileNames:
        if inputFile_matcher.match(input_file):
            yield "".join(["rfio:", input_file])

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
    isMC_string = None
    if isMC:
        isMC_string = "True"
    else:
        isMC_string = "False"
    cfg_modified = cfg_modified.replace("#isMC#", isMC_string)
    HLTprocessName = samples[sampleName]['HLTprocessName']
    cfg_modified = cfg_modified.replace("#HLTprocessName#", "'%s'" % HLTprocessName)

    cfg_modified += "\n"
    cfg_modified += "process.source.fileNames = cms.untracked.vstring(%s)\n" % \
                      [ "".join([ "file:", inputFileName ]) for inputFileName in inputFileNames ]
    cfg_modified += "process.patTupleOutputModule.fileName = cms.untracked.string('%s')\n" % \
                      output_mapper(sampleName, jobId, version)
    cfg_modified += "process.savePUreweightHistograms.outputFileName = cms.string('%s')\n" % \
                      'ZllRecoilCorrectionPUreweightHistograms_%s_%s_%s.root' % (sampleName, version, jobId)

    if cfgFileName_modified is None:
        cfgFileName_modified = cfgFileName_original.replace("_cfg.py", "_%s_%s_cfg.py" % (sampleName, jobId))
    cfgFile_modified = open(cfgFileName_modified, "w")
    cfgFile_modified.write(cfg_modified)
    cfgFile_modified.close()

    return cfgFileName_modified

if len(samplesToAnalyze) == 0:
    samplesToAnalyze = samples.keys()

#--------------------------------------------------------------------------------
# Build config files for producing PAT-tuples on lxbatch
bsubFileNames       = {}
bsubScriptFileNames = {}
bsubJobNames        = {}
for sampleToAnalyze in samplesToAnalyze:

    print "checking sample %s" % sampleToAnalyze

    bsubFileNames[sampleToAnalyze]       = {}
    bsubScriptFileNames[sampleToAnalyze] = {}
    bsubJobNames[sampleToAnalyze]        = {}

    inputFilePath = samples[sampleToAnalyze]['skimFilePath']
    print " inputFilePath = %s" % inputFilePath
    
    inputFileNames = [ file_info['path'] for file_info in castor.nslsl(inputFilePath) ]
    #print " inputFileNames = %s" % inputFileNames
    
    inputFileNames_matched = [ os.path.basename(input_file) for input_file in input_mapper(inputFileNames, sampleToAnalyze) ]
    #print "inputFileNames_matched = %s" % inputFileNames_matched
    print "--> found %i inputFiles" % len(inputFileNames_matched)
    
    for jobId, inputFileNames_chunk in enumerate(chunks(inputFileNames_matched, samples[sampleToAnalyze]['numInputFilesPerJob'])):
        # Build script for batch job submission;
        # the None in the tuple indicates that batch job has no dependencies on other batch jobs
        input_files_and_jobs = \
          [ (None, os.path.join(inputFilePath, inputFileName)) for inputFileName in inputFileNames_chunk ]

        configFileName = os.path.join(configFilePath, configFile.replace("_cfg.py", "_%s_%s_cfg.py" % (sampleToAnalyze, jobId)))
        #print " configFileName = %s" % configFileName
        customizeConfigFile(sampleToAnalyze, jobId, version, inputFileNames_chunk, configFile, configFileName)

        outputFileName = output_mapper(sampleToAnalyze, jobId, version)

        logFileName = os.path.basename(configFileName.replace('_cfg.py', '.log'))
        def log_file_maker(job_hash):
            return os.path.join(logFilePath, logFileName)

        histogramFileName = 'ZllRecoilCorrectionPUreweightHistograms_%s_%s_%s.root' % (sampleToAnalyze, version, jobId)

        jobName, bsubScript = make_bsub_script(
            os.path.join(outputFilePath, outputFileName),
            input_files_and_jobs,
            log_file_maker,
            "cmsRun %s" % os.path.join(configFilePath, configFileName),
            add_output_files = [ histogramFileName ])
        bsubFileNames[sampleToAnalyze][jobId] = [ outputFileName, histogramFileName ]
                    
        bsubScriptFileName = \
          os.path.join(configFilePath, logFileName.replace(".log", ".sh"))
        bsubScriptFile = open(bsubScriptFileName, "w")
        bsubScriptFile.write(bsubScript)
        bsubScriptFile.close()

        bsubScriptFileNames[sampleToAnalyze][jobId] = bsubScriptFileName

        bsubJobName = "ZllRecoilPATtuple%s_%i" % (sampleToAnalyze, jobId)
        bsubJobNames[sampleToAnalyze][jobId] = bsubJobName
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
#
# build shell script for running 'hadd' in order to collect all histograms
# for same Data/MC sample into single .root file
#
bsubFileNames_harvesting     = {}
bsubJobNames_harvesting      = {}
bjobListFileNames_harvesting = {}
for sampleToAnalyze in samplesToAnalyze:
    bsubFileNames_harvesting[sampleToAnalyze]     = {}
    bsubJobNames_harvesting[sampleToAnalyze]      = {}
    bjobListFileNames_harvesting[sampleToAnalyze] = {}

    plot_regex = r"ZllRecoilCorrectionPUreweightHistograms_[a-zA-Z0-9_]+_[a-zA-Z0-9_]+_[0-9]+.root"
    skim_regex = r"dont match anything"
        
    def local_copy_mapper(sample):
        return os.path.join(
            outputFilePath,
            'ZllRecoilCorrectionPUreweightHistograms_%s_%s_%s.root' % (sampleToAnalyze, version, jobId))
    
    inputFileInfos = []
    for jobId in bsubScriptFileNames[sampleToAnalyze].keys():
        for inputFileName in bsubFileNames[sampleToAnalyze][jobId]:
            inputFileInfo = {
                'path'        : os.path.join(outputFilePath, inputFileName),
                'size'        : 1,           # dummy
                'time'        : time.localtime(),
                'file'        : inputFileName,
                'permissions' : 'mrw-r--r--' # "ordinary" file access permissions
            }
            #print "inputFileInfo = %s" % inputFileInfo
            inputFileInfos.append(inputFileInfo)

    retVal_make_harvest_scripts = make_harvest_scripts(
        plot_regex,
        skim_regex,
        channel = "ZllRecoilCorrection",
        sampleToAnalyze = sampleToAnalyze,
        job_id = version,
        input_files_info = inputFileInfos,
        harvester_command = executable_hadd,
        abort_on_rfcp_error = False,
        castor_output_directory = outputFilePath,
        script_directory = configFilePath,
        merge_script_name = \
        os.path.join(configFilePath, "_".join(['submit', sampleToAnalyze, 'merge']) + '.sh'),
        local_copy_mapper = local_copy_mapper,
        chunk_size = 2.e+9, # 2 GB
        run_merging = False,
        check_old_files = False,
        max_bsub_concurrent_file_access = 250,
        verbosity = 0
    )

    bsubFileNames_harvesting[sampleToAnalyze] = retVal_make_harvest_scripts

    bsubJobName = "harvest%s" % sampleToAnalyze
    bsubJobNames_harvesting[sampleToAnalyze] = bsubJobName

    bjobListFileName_harvesting = os.path.join(configFilePath, "batchJobs_harvesting_%s.lst" % sampleToAnalyze)
    bjobListFile_harvesting = open(bjobListFileName_harvesting, "w")
    for jobId in bsubScriptFileNames[sampleToAnalyze].keys():
        bjobListFile_harvesting.write("%s\n" % bsubJobNames[sampleToAnalyze][jobId])
    bjobListFile_harvesting.close()
    bjobListFileNames_harvesting[sampleToAnalyze] = bjobListFileName_harvesting
#--------------------------------------------------------------------------------

def make_MakeFile_vstring(list_of_strings):
    retVal = ""
    for i, string_i in enumerate(list_of_strings):
        if i > 0:
            retVal += " "
        retVal += string_i
    return retVal

# done building config files, now build Makefile...
makeFileName = "Makefile_ZllRecoilCorrectionPATTupleProduction"
makeFile = open(makeFileName, "w")
makeFile.write("\n")
jobsZllRecoilCorrectionPATtupleProduction = []
existingOutputFiles = [ file_info['file'] for file_info in castor.nslsl(outputFilePath) ]
for sampleToAnalyze in samplesToAnalyze:
    for jobId in bsubScriptFileNames[sampleToAnalyze].keys():
        outputFilesExist = True
        for outputFileName in bsubFileNames[sampleToAnalyze][jobId]:
            if not outputFileName in existingOutputFiles:
                outputFilesExist = False
        if skipExistingPATtuples and outputFilesExist:
            print "Output files for sample = %s, jobId = %s exist --> skipping !!" % (sampleToAnalyze, jobId)
        else:
            jobsZllRecoilCorrectionPATtupleProduction.append(bsubJobNames[sampleToAnalyze][jobId])
    jobsZllRecoilCorrectionPATtupleProduction.append(bsubJobNames_harvesting[sampleToAnalyze])        
makeFile.write("all: %s\n" % make_MakeFile_vstring(jobsZllRecoilCorrectionPATtupleProduction))
makeFile.write("\techo 'Finished running ZllRecoilCorrectionPATTupleProduction.'\n")
makeFile.write("\n")
for sampleToAnalyze in samplesToAnalyze:
    for jobId in bsubScriptFileNames[sampleToAnalyze].keys():
        if bsubJobNames[sampleToAnalyze][jobId] in jobsZllRecoilCorrectionPATtupleProduction:
            makeFile.write("%s:\n" % bsubJobNames[sampleToAnalyze][jobId])
            makeFile.write("\t%s -q %s -J %s < %s\n" %
              (executable_bsub,
               lxbatch_queue,
               bsubJobNames[sampleToAnalyze][jobId],
               bsubScriptFileNames[sampleToAnalyze][jobId]))
    makeFile.write("\n")
for sampleToAnalyze in samplesToAnalyze:
    bsubJobNames_sample = []
    for jobId in bsubScriptFileNames[sampleToAnalyze].keys():
        if bsubJobNames[sampleToAnalyze][jobId] in jobsZllRecoilCorrectionPATtupleProduction:
            bsubJobNames_sample.append(bsubJobNames[sampleToAnalyze][jobId])
    makeFile.write("%s: %s\n" %
      (bsubJobNames_harvesting[sampleToAnalyze],
       make_MakeFile_vstring(bsubJobNames_sample)))
    makeFile.write("\t%s %s\n" %
      (executable_waitForLXBatchJobs,
       bjobListFileNames_harvesting[sampleToAnalyze]))
    makeFile.write("\t%s %s\n" %
      (executable_shell,
       bsubFileNames_harvesting[sampleToAnalyze]['harvest_script_name']))
    makeFile.write("\n")
makeFile.write(".PHONY: clean\n")
makeFile.write("clean:\n")
for sampleToAnalyze in samplesToAnalyze:
    for jobId in bsubScriptFileNames[sampleToAnalyze].keys():
        for outputFileName in bsubFileNames[sampleToAnalyze][jobId]:
            makeFile.write("\t%s %s\n" %
              (executable_rfrm,
               os.path.join(outputFilePath, outputFileName)))
for sampleToAnalyze in samplesToAnalyze:
    makeFile.write("\t%s %s\n" %
      (executable_rfrm,
       bsubFileNames_harvesting[sampleToAnalyze]))
makeFile.write("\techo 'Finished deleting old files.'\n")
makeFile.write("\n")
makeFile.close()

print("Finished building Makefile. Now execute 'make -j 8 -f %s'." % makeFileName)
