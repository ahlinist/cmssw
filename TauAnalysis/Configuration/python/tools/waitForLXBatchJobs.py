#!/usr/bin/env python

import re
import shlex
import subprocess
import sys
import time

'''

Wait until all batch jobs have finished processing.
The names of the batch jobs are specified in a text file,
the name of which needs to be given as command-line parameter.

(format: name of one batch job per line)

Author: Evan Friis, Christian Veelken (UC Davis)

'''

if len(sys.argv) != 2:
    print "Usage: %s bjobListFile.lst" % sys.argv[0]
    sys.exit()

# read names of batch jobs
bjobListFile = open(sys.argv[1], "r")
bjobNames = bjobListFile.read().splitlines()
bjobListFile.close()

print "%s: waiting for following lxbatch jobs to finish:" % sys.argv[0]
for i, bjobName in enumerate(bjobNames):
    print " %i: %s" % (i, bjobName)

regExpr = re.compile("Job <(?P<sample>\w+?)> is not found")

waitTime = 0

numJobs = len(bjobNames)
while numJobs >= 1:
    iJob = 0
    while iJob < numJobs:
        #
        # check status of i-th job:
        # if return value of 'bjobs' command matches regular expression,
        # it means the batch job has finished processing and may be removed from list of jobs included in future checks
        #
        # NOTE: return value of 'bjobs' command is written to stderr, not stdout !!
        #
        #print "checking status of job %i: %s" % (iJob, bjobNames[i])
        bjobStatusCommand = "bjobs -J %s" % bjobNames[iJob]
        bjobStatus = subprocess.Popen(shlex.split(bjobStatusCommand), stderr = subprocess.PIPE)
        lines = bjobStatus.stderr.read().splitlines()
        
        bjobStatus_finished = False
        for line in lines:
            #print "line = %s" % line
            if regExpr.match(line) is not None:
                bjobStatus_finished = True

        if bjobStatus_finished:
            #print "job %s has finished processing." % bjobNames[i]
            del bjobNames[iJob]
            numJobs = len(bjobNames)
            
        ++iJob
            
    if numJobs >= 1:  
        ++waitTime
        print "%s: have waited %i minutes" % (sys.argv[0], waitTime)
        time.sleep(60)
        
print "%s: all batch jobs have finished after %i minutes" % (sys.argv[0], waitTime)
print "%s: waiting for another 5 minutes..." % sys.argv[0]

# CV: wait for another 5 minutes after all jobs have finished processing,
#     in order to allow for all output files to be written to castor and properly closed
time.sleep(300)
