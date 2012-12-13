#!/usr/bin/env python

import TauAnalysis.Configuration.tools.castor as castor
import TauAnalysis.Configuration.tools.eos as eos

import hashlib
import math
import os
import random
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
if not inputFilePath.endswith("/"):
    inputFilePath += "/"
print "inputFilePath = %s" % inputFilePath
sample = sys.argv[2]
print "sample = %s" % sample

tmpDir = '/data1/veelken/tmp/castor_hadd/'
hash = hashlib.md5()
hash.update("%s%s" % (inputFilePath, sample))
tmpDir += hash.hexdigest()[:8]
if not os.path.exists(tmpDir):
    os.mkdir(tmpDir)

inputFileName1_regex = \
  r'[a-zA-Z0-9_]*%s_(?P<gridJob>[0-9]+)_(?P<gridTry>[0-9]+)_(?P<hash>[a-zA-Z0-9]+).root' % sample
inputFile1_matcher = re.compile(inputFileName1_regex)
inputFileName2_regex = \
  r'[a-zA-Z0-9_]*%s[a-zA-Z0-9_]*_(?P<jobId>[a-zA-Z0-9_]+).root' % sample
inputFile2_matcher = re.compile(inputFileName2_regex)

executable_rfcp   = 'rfcp'
executable_hadd   = 'hadd -k'
executable_make   = 'make'
executable_eos_cp = 'cmsStage'

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
elif inputFilePath.find("/store") != -1:
    inputFileNames = [ file_info['path'] for file_info in eos.lsl(inputFilePath) ]
else:
    inputFileNames = [ '%s' % os.path.join(inputFilePath, file_name) for file_name in os.listdir(inputFilePath) ]

#print "inputFileNames = %s" % inputFileNames

inputFileNames_matched = []
for inputFileName in inputFileNames:
    if inputFile1_matcher.match(os.path.basename(inputFileName)) or \
       inputFile2_matcher.match(os.path.basename(inputFileName)):
        inputFileNames_matched.append(inputFileName)

random.shuffle(inputFileNames_matched)

print "inputFileNames_matched = %s" % inputFileNames_matched

if len(inputFileNames_matched) == 0:
    raise ValueError("No input files found !!")

for inputFileName_matched in inputFileNames_matched:
    if inputFileName_matched.find('/castor/') != -1:
        commandLine = '%s %s' % ('stager_get -M', inputFileName_matched)
        #runCommand(commandLine)

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
outputFileName_level2 = outputFileName.replace("%s.root" % jobId, "all.root")
outputFileName_level2 = os.path.join(tmpDir, outputFileName_level2)
print "outputFileName = %s" % outputFileName_level2

commandLine = 'rm %s' % outputFileName_level2
runCommand(commandLine)

# build Makefile for copyint input files
# (use separate Makefile in order to avoid running more than one rfcp job at the same time)
makeFileName_part1 = "Makefile_castor_hadd_part1_%s" % hash.hexdigest()[:8]
makeFile_part1 = open(makeFileName_part1, "w")
makeFile_part1.write("\n")
tmpFileNames = []
for inputFileName_matched in inputFileNames_matched:
    tmpFileName = os.path.join(tmpDir, os.path.basename(inputFileName_matched))
    tmpFileNames.append(tmpFileName)
makeFile_part1.write("all: %s\n" %
  format_vstring(tmpFileNames))
makeFile_part1.write("\n")
for i, inputFileName_matched in enumerate(inputFileNames_matched):
    tmpFileName = tmpFileNames[i]
    commandLine = None
    if inputFileName_matched.find('/castor/') != -1:
        commandLine = '%s %s %s' % (executable_rfcp, inputFileName_matched, tmpFileName)
    elif inputFileName_matched.find('/store/') != -1:
        commandLine = '%s %s %s' % (executable_eos_cp, inputFileName_matched, tmpFileName)    
    else:
        commandLine = 'cp %s %s' % (inputFileName_matched, tmpFileName)
    makeFile_part1.write("%s:\n" %
      (tmpFileName))
    makeFile_part1.write("\t%s\n" %
      (commandLine))
makeFile_part1.write("\n")
makeFile_part1.close()
commandLine = '%s -j 1 -f %s' % (executable_make, makeFileName_part1)
#raise ValueError("STOP.")
runCommand(commandLine)

# build Makefile for running hadd
# (run hadd in two stages, in order to reduce number of files that need to be added simultaneously)
makeFileName_part2 = "Makefile_castor_hadd_part2_%s" % hash.hexdigest()[:8]
makeFile_part2 = open(makeFileName_part2, "w")
makeFile_part2.write("\n")
makeFile_part2.write("all: %s\n" %
  (outputFileName_level2))
makeFile_part2.write("\n")
numJobs_level1 = int(math.sqrt(len(tmpFileNames)))
outputFileNames_level1 = []
for jobIdx in range(numJobs_level1 + 1):
    outputFileName_level1 = outputFileName.replace("%s.root" % jobId, "part%i.root" % jobIdx)
    outputFileName_level1 = os.path.join(tmpDir, outputFileName_level1)
    firstJob = int(numJobs_level1*jobIdx)
    lastJob = int(numJobs_level1*(jobIdx + 1) - 1)
    if lastJob >= len(tmpFileNames):
        lastJob = len(tmpFileNames)
    if not (lastJob >= firstJob):
        continue
    makeFile_part2.write("%s: %s\n" %
      (outputFileName_level1,
       format_vstring(tmpFileNames[firstJob:lastJob])))
    makeFile_part2.write("\t%s %s %s\n" %
      (executable_hadd,
       outputFileName_level1,
       format_vstring(tmpFileNames[firstJob:lastJob])))
    outputFileNames_level1.append(outputFileName_level1)
makeFile_part2.write("\n")
makeFile_part2.write("%s: %s\n" %
  (outputFileName_level2,
   format_vstring(outputFileNames_level1)))
makeFile_part2.write("\t%s %s %s\n" %
  (executable_hadd,
   outputFileName_level2,
   format_vstring(outputFileNames_level1)))
makeFile_part2.close()
commandLine = '%s -j 4 -f %s' % (executable_make, makeFileName_part2)
#raise ValueError("STOP.")
runCommand(commandLine)

commandLine = None
if inputFilePath.find('/castor/') != -1:
    commandLine = '%s %s %s' % (executable_rfcp, outputFileName, os.path.join(inputFilePath, os.path.basename(outputFileName)))
elif inputFilePath.find('/store/') != -1:
    commandLine = '%s %s %s' % (executable_eos_cp, outputFileName, os.path.join(inputFilePath, os.path.basename(outputFileName)))    
else:
    commandLine = 'cp %s %s' % (outputFileName, os.path.join(inputFilePath, os.path.basename(outputFileName)))
runCommand(commandLine)

#for tmpFileName in tmpFileNames:
#    commandLine = 'rm %s' % tmpFileName
    
print("Finished running 'castor_hadd.py'.")

