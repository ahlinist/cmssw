#!/usr/bin/env python

import re
import sys
import subprocess
import shlex
import time

#--------------------------------------------------------------------------------
# Kill all lxbatch jobs with specified names
#
# Author: Christian Veelken (UC Davis)
# 
# Examples:
# 
# ./killLXbatchJobs.py
#--------------------------------------------------------------------------------

print("<killLXbatchJobs>:")

minJobId = 0
maxJobId = 999999999
killJobsById = False

jobsToKill = []
killJobsByName = False

args = sys.argv[1:]
#print("len(args) = %i" % len(args))

isArgsValid = False
isArgsError = False

currMode = None

for index, arg in enumerate(args):

    #print("index = %i" % index)
    #print("arg = %s" % arg)
    
    if arg == "--range" and (index + 2) < len(args):
        minJobId = int(args[index + 1])
        maxJobId = int(args[index + 2])
        killJobsById = True
        isArgsValid = True
        currMode = 'jobId'
    elif arg == "--names":
        currMode = 'jobName'
    elif currMode == 'jobName':
        jobsToKill.append(arg)
        killJobsByName = True
        isArgsValid = True

if not isArgsValid:
    raise ValueError("Usage: killLXbatchJobs.py --range XXXXXX YYYYYY --names name1 name2...")

#print("minJobId = %i" % minJobId)
#print("maxJobId = %i" % maxJobId)
#print("killJobsById = %s" % killJobsById)

#print("jobsToKill = %s" % jobsToKill)
#print("killJobsByName = %s" % killJobsByName)

commandLine = 'bjobs -w'
args = shlex.split(commandLine)
retval = subprocess.Popen(args, stdout = subprocess.PIPE)
#retval.wait()

jobs = retval.stdout.read().split('\n')
#print(" jobs = %s" % jobs)

for jobInfo in jobs:
    items = jobInfo.split()
    #print("items: %s" % items)

    if len(items) < 6 or items[0] == "JOBID":
        continue
    
    jobId = int(items[0])
    jobName = str(items[6])

    isJobToKill = False
    if killJobsById:
        if jobId >= minJobId and jobId <= maxJobId:
            isJobToKill = True
    if killJobsByName:
        for jobToKill in jobsToKill:
            if jobName.find(jobToKill) != -1:
                isJobToKill = True

    if isJobToKill:
        print("killing job %i, name = %s" % (jobId, jobName))
        commandLine = 'bkill %i' % jobId
        subprocess.call(commandLine, shell = True)
