#!/usr/bin/env python

import os
import shlex
import subprocess

crabFilePath = "/data1/veelken/CMSSW_3_8_x/crab/Skimming/"

jobId = "2011Feb03b"

#cmsswSkimPath = "muonCaloTauSkimPath"
cmsswSkimPath = "muonPFTauShrinkingConeSkimPath"

def _runCommand(commandLine):

    #print(commandLine)
    args = shlex.split(commandLine)
    retVal = subprocess.Popen(args, stdout = subprocess.PIPE)
    retVal.wait()

    return retVal

jobs = _runCommand('ls %s' % crabFilePath).stdout.read().split()

scriptFileName = "makeSkimReport_crab.csh"
scriptFile = open("%s" % scriptFileName, "w")

for job in jobs:
    if not (job.find("crabdir") != -1 and job.find(jobId) != -1):
	continue

    jobFilePath = os.path.join(crabFilePath, job) 

    commandLine = "crab -getoutput -c %s" % jobFilePath
    scriptFile.write("%s\n" % commandLine)

    if job.find("data") != -1:
        commandLine = "crab -report -c %s" % jobFilePath
        scriptFile.write("%s\n" % commandLine)
        commandLine = "lumiCalc.py -c frontier://LumiProd/CMS_LUMI_PROD -i %s --nowarning overview >& %s" % \
                     (os.path.join(jobFilePath, "res/lumiSummary.json"), os.path.join(jobFilePath, "lumiCalc.out"))
        scriptFile.write("%s\n" % commandLine)

scriptFile.close()

#subprocess.call("source %s" % scriptFileName, shell = True)

for job in jobs:
    if not (job.find("crabdir") != -1 and job.find(jobId) != -1):
	continue

    jobFilePath = os.path.join(crabFilePath, job)

    numEventsTotal_processed = 0
    numEventsTotal_passed = 0

    files = _runCommand('ls %s' % os.path.join(jobFilePath, "res")).stdout.read().split()
    for file in files:
        if not (file.find("CMSSW") != -1 and file.find(".stdout") != -1):
            continue

        outputFileName = os.path.join(jobFilePath, "res", file)
        outputFile = open("%s" % outputFileName, "r")

        isPathSummary = False
        
        for line in outputFile:
            if not line.find("TrigReport") != -1:
                continue

            if line.find("---------- Path   Summary ------------") != -1:
                isPathSummary = True
            elif line.find("----------") != -1:
                isPathSummary = False

            if not isPathSummary:
                continue
            
            if line.find(cmsswSkimPath) != -1:
                columns = line.split()
                #print "columns", columns

                numEvents_processed = int(columns[3])
                #print "numEvents_processed", numEvents_processed
                numEventsTotal_processed += numEvents_processed
                
                numEvents_passed = int(columns[4])
                #print "numEvents_passed", numEvents_passed
                numEventsTotal_passed += numEvents_passed

    print("job = %s:" % job)
    print(" num. Events processed = %i" % numEventsTotal_processed)
    print(" num. Events passed = %i" % numEventsTotal_passed)
    if numEventsTotal_processed > 0:
        print("--> skim Efficiency = %f" % (float(numEventsTotal_passed)/float(numEventsTotal_processed)))
