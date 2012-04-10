import FWCore.ParameterSet.Config as cms

import TauAnalysis.Configuration.tools.castor as castor

import os
import re

def replaceConfigFileParam(configFileName_template, configFileName_output, replacements):
    configFile_template = open(configFileName_template, 'r')
    lines_template = configFile_template.readlines()
    lines_output = []
    for line_template in lines_template:
        isReplacement = False
        for replacement in replacements:
            if not len(replacement) == 2:
                raise ValueError("Invalid replacement = %s !!" % replacement)
            replacement_key = replacement[0]
            replacement_value = replacement[1]
            replacement_regex = "#__%s\s*=\s*[#%s#|'#%s#']\s*" % (replacement_key, replacement_key, replacement_key)
            replacement_matcher = re.compile(replacement_regex)
            if replacement_matcher.match(line_template):
                lines_output.append("%s = %s\n" % (replacement_key, replacement_value))
                isReplacement = True
        if not isReplacement:
            lines_output.append(line_template)
    configFile_template.close()
    configFile_output = open(configFileName_output, 'w')
    for line_output in lines_output:
        configFile_output.write("%s" % line_output)
    configFile_output.close()

def buildConfigFile_SVfitEventHypothesisAnalyzer(sampleToAnalyze, channelToAnalyze, metResolution, 
                                                 configFileName_template,
                                                 inputFilePath, numInputFilesPerJob, maxEvents,
                                                 configFilePath, logFilePath, outputFilePath):

    """Build cfg.py file to run SVfit algorithm and fill histograms of SVfit reconstructed mass""" 

    #print "inputFilePath = %s" % inputFilePath

    inputFileNames = None
    if inputFilePath.find('/castor/') != -1:
        inputFileNames = [ file_info['path'] for file_info in castor.nslsl(inputFilePath) ]
    else:
        inputFileNames = os.listdir(inputFilePath)
    #print "inputFileNames = %s" % inputFileNames

    # check if inputFile matches sampleToAnalyze
    inputFileNames_sample = []
    for inputFileName in inputFileNames:        
        if inputFileName.find("".join(['_', sampleToAnalyze, '_'])) != -1 or \
           inputFileName.find("".join(['/', sampleToAnalyze, '_'])) != -1:
            # CV: assume that input file gets copied to local directory before cmsRun gets started
            inputFileNames_sample.append(os.path.basename(inputFileName))

    #print(sampleToAnalyze)
    #print(inputFileNames_sample)

    if len(inputFileNames_sample) == 0:
        print("Sample %s, channel = %s has no input files --> skipping !!" % (sampleToAnalyze, channelToAnalyze))
        return

    inputFileNameGroups_sample = []
    numJobs = (len(inputFileNames_sample) / numInputFilesPerJob)
    if (len(inputFileNames_sample) % numInputFilesPerJob) != 0:
        numJobs = numJobs +1
    for jobId in range(numJobs):
        inputFileIdx_first = jobId*numInputFilesPerJob
        inputFileIdx_last = inputFileIdx_first + numInputFilesPerJob
        if inputFileIdx_last >= len(inputFileNames_sample):
            inputFileIdx_last = len(inputFileNames_sample) - 1
        inputFileNameGroups_sample.append(inputFileNames_sample[inputFileIdx_first:inputFileIdx_last])

    #print "inputFileNameGroups_sample = %s" % inputFileNameGroups_sample

    configFileNames = []
    outputFileNames = []
    logFileNames    = []

    for jobId in range(numJobs):
        
        inputFileNames_string = "[ "
        for inputFileName_sample in inputFileNameGroups_sample[jobId]:
            inputFileNames_string += "'file:%s', " % inputFileName_sample
        inputFileNames_string += " ]"

        sample_type = None
        sample_type_Z_regex = "[Ztautau|ZplusJets|ZToTauTau]"
        sample_type_Z_matcher = re.compile(sample_type_Z_regex)
        sample_type_Higgs_regex = "(((gg|bb|vbf)(Higgs|Phi))|HToTauTau_M-)[0-9]+"
        sample_type_Higgs_matcher = re.compile(sample_type_Higgs_regex)
        if sample_type_Z_matcher.match(sampleToAnalyze):
            sample_type = 'Z'
        elif sample_type_Higgs_matcher.match(sampleToAnalyze):
            sample_type = 'Higgs'
        else:
            raise ValueError("Failed to determine wether sample = %s is Z or Higgs sample !!" % sampleToAnalyze)

        metResolution_string = "None"
        metResolution_label  = "pfMEtResMC"
        if metResolution is not None:
            metResolution_string = "%f" % metResolution
            metResolution_label  = "pfMEtRes%1.0f" % metResolution
            metResolution_label  = metResolution_label.replace(".", "_")
        
        outputFileName = 'svFitPerformanceAnalysisPlots_%s_%s_%s_%i.root' % \
          (sampleToAnalyze, channelToAnalyze, metResolution_label, jobId + 1)
        outputFileNames.append(outputFileName)
 
        replacements = []
        replacements.append([ 'sample',         "'%s'" % sampleToAnalyze       ])
        replacements.append([ 'sample_type',    "'%s'" % sample_type           ])
        replacements.append([ 'channel',        "'%s'" % channelToAnalyze      ])
        replacements.append([ 'metResolution',  "%s"   % metResolution_string  ])
        replacements.append([ 'maxEvents',      "%i"   % maxEvents             ])
        replacements.append([ 'inputFileNames', "%s"   % inputFileNames_string ])
        replacements.append([ 'outputFileName', "'%s'" % outputFileName        ])
                
        configFileName = "svFitPerformanceAnalysisPlots_%s_%s_%s_%i_cfg.py" % \
          (sampleToAnalyze, channelToAnalyze, metResolution_label, jobId)
        configFileName_full = os.path.join(configFilePath, configFileName)
        replaceConfigFileParam(configFileName_template, configFileName_full, replacements)
        configFileNames.append(configFileName)

        logFileName = configFileName.replace('_cfg.py', '.log')
        logFileName_full = os.path.join(logFilePath, logFileName)
        logFileNames.append(logFileName)

    retVal = {}
    retVal['inputFileNames']  = inputFileNameGroups_sample
    retVal['configFileNames'] = configFileNames
    retVal['outputFileNames'] = outputFileNames
    retVal['logFileNames']    = logFileNames

    #print " inputFileNames = %s" % inputFileNames_sample
    #print " configFileNames = %s" % configFileNames
    #print " outputFileNames = %s" % outputFileNames
    #print " logFileNames = %s" % logFileNames

    return retVal
