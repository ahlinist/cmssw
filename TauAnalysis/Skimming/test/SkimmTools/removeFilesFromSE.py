#!/usr/bin/env python

import sys, re, os
    
##################
# use it as:
# ./removeFilesFromSE.py crab_0_XXXX 1,2,3,4
# where 1,2,3,4 is the list of runs for which you would like to remove the
# output from SE
#
##################
def removeFiles(path,jobs):
    for job in jobs:
        if len(job.split("-"))==2:
          minJob = int(job.split("-")[0])
          maxJob = int(job.split("-")[1]) + 1
        else:
          minJob = int(job)
          maxJob = int(job) + 1
        for jobNumber in range(minJob,maxJob):
            fileName = "CMSSW_"+str(jobNumber)+".stdout"
            print "----------------------------------------------"
            print "Removing files for job: ",jobNumber
            ##############
            crabLogPath = path+"/res/"+fileName
            if os.path.exists(crabLogPath):
                crabLog = open(crabLogPath,"r")            
                for line in crabLog:
                    pfn = ""
                    if re.search("newPfn",line)!=None:
                        pfn = line.split("=")[1].rstrip("\n")+"="+line.split("=")[2].rstrip("\n")
                        command = "srmrm "+pfn
                        print command
                        os.system(command)          
                crabLog.close()          
##################
##################

if len(sys.argv)==3:
    path = sys.argv[1]
    jobs = sys.argv[2].split(',')
    print "Path: ",path
    print "Jobs: ",jobs
    removeFiles(path,jobs)    

