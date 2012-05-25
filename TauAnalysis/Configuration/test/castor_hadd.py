#!/usr/bin/env python

import TauAnalysis.Configuration.tools.castor as castor

import os
import re
import shlex
import subprocess
import sys
import time

#--------------------------------------------------------------------------------
# Run 'hadd' for input files stored on castor.
# The output file is copied to the same castor directory.
#
# Author: Christian Veelken (LLR)
# 
# Examples:
# 
# ./castor_hadd.py /castor/cern.ch/user/v/veelken/CMSSW_5_2_x/PATtuples/ZllRecoilCorrection/v5_0 PUreweightHistograms_ZplusJets_madgraph
#
#--------------------------------------------------------------------------------

print("<castor_hadd>:")

if not len(sys.argv) == 3:
    raise ValueError("Usage: castor_hadd.py castor_directory sample")

inputFilePath = sys.argv[1]
print "inputFilePath = %s" % inputFilePath
sample = sys.argv[2]
print "sample = %s" % sample

tmpDir = '/data1/veelken/tmp/castor_hadd'

inputFileName1_regex = \
  r'[a-zA-Z0-9_]*%s_(?P<gridJob>[0-9]+)_(?P<gridTry>[0-9]+)_(?P<hash>[a-zA-Z0-9]+).root' % sample
inputFile1_matcher = re.compile(inputFileName1_regex)
inputFileName2_regex = \
  r'[a-zA-Z0-9_]*%s[a-zA-Z0-9_]*_(?P<jobId>[a-zA-Z0-9_]+).root' % sample
inputFile2_matcher = re.compile(inputFileName2_regex)

executable_rfcp = 'rfcp'
executable_hadd = 'hadd -k'

def runCommand(commandLine):
    print(commandLine)
    args = shlex.split(commandLine)
    retVal = subprocess.Popen(args, stdout = subprocess.PIPE)
    retVal.wait()
    return retVal.stdout.readlines()
    
def format_vstring(list_of_strings):
    retVal = ""
    for i, string_i in enumerate(list_of_strings):
        if i > 0:
            retVal += " "
        retVal += string_i
    return retVal

inputFileNames = []
if inputFilePath.find('/castor/') != -1:
    inputFileNames = [ '%s' % file_info['path'] for file_info in castor.nslsl(inputFilePath) ]
else:
    inputFileNames = [ '%s' % os.path.join(inputFilePath, file_name) for file_name in os.listdir(inputFilePath) ]

#print "inputFileNames = %s" % inputFileNames

inputFileNames_matched = []
for inputFileName in inputFileNames:
    if inputFile1_matcher.match(os.path.basename(inputFileName)) or \
       inputFile2_matcher.match(os.path.basename(inputFileName)):
        inputFileNames_matched.append(inputFileName)

print "inputFileNames_matched = %s" % inputFileNames_matched

if len(inputFileNames_matched) == 0:
    raise ValueError("No input files found !!")

tmpFileNames = []
for inputFileName_matched in inputFileNames_matched:
    tmpFileName = os.path.join(tmpDir, os.path.basename(inputFileName_matched))
    commandLine = None
    if inputFileName_matched.find('/castor/') != -1:
        commandLine = '%s %s %s' % (executable_rfcp, inputFileName_matched, tmpFileName)
    else:
        commandLine = 'cp %s %s' % (inputFileName_matched, tmpFileName)
    runCommand(commandLine)
    tmpFileNames.append(tmpFileName)

outputFileName = os.path.basename(inputFileNames_matched[0])
jobId = None
if inputFile1_matcher.match(outputFileName):
    jobId = "%s_%s_%s" % \
      (inputFile1_matcher.match(outputFileName).group('gridJob'),
       inputFile1_matcher.match(outputFileName).group('gridTry'),
       inputFile1_matcher.match(outputFileName).group('hash'))
elif inputFile2_matcher.match(outputFileName):
    jobId = inputFile2_matcher.match(outputFileName).group('jobId')
else:
    raise ValueError("Failed to compose output file name !!")
outputFileName = outputFileName.replace("%s.root" % jobId, "all.root")
outputFileName = os.path.join(tmpDir, outputFileName)
print "outputFileName = %s" % outputFileName

commandLine = 'rm %s' % outputFileName
runCommand(commandLine)

commandLine = '%s %s %s' % (executable_hadd, outputFileName, format_vstring(tmpFileNames))
runCommand(commandLine)

commandLine = None
if inputFilePath.find('/castor/') != -1:
    commandLine = '%s %s %s' % (executable_rfcp, outputFileName, os.path.join(inputFilePath, os.path.basename(outputFileName)))
else:
    commandLine = 'cp %s %s' % (outputFileName, os.path.join(inputFilePath, os.path.basename(outputFileName)))
runCommand(commandLine)

for tmpFileName in tmpFileNames:
    commandLine = 'rm %s' % tmpFileName
    
print("Finished running 'castor_hadd.py'.")

