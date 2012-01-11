#!/usr/bin/env python

import TauAnalysis.Configuration.tools.castor as castor

import json
import os
import re
import sys
import subprocess
import shlex
import time

#--------------------------------------------------------------------------------
# Check status of crab jobs and resubmit jobs that are "stuck"
# in 'Submitted', 'Ready' or 'Scheduled' state for more than one day
#
# Author: Christian Veelken (UC Davis)
# 
# Examples:
# 
# ./crabSitter.py 
#
#--------------------------------------------------------------------------------

print("<crabSitter>:")

crabFilePath = '/tmp/veelken/crab'

statusFileName = 'crabSitter.json'
jobName_regex = r'crabdirProduceFakeRatePATtuple_(?P<sample>[a-zA-Z0-9]*)_(?P<channel>[a-zA-Z0-9]*)_patV2_1'
jobName_matcher = re.compile(jobName_regex)

executable_ls = 'ls'
executable_crab = 'crab'

def runCommand(commandLine):
    print(commandLine)
    args = shlex.split(commandLine)
    retVal = subprocess.Popen(args, stdout = subprocess.PIPE)
    retVal.wait()
    return retVal

jobStatus_dict = {}
if os.path.exists(statusFileName):
    if os.path.isfile(statusFileName):
        print("Loading previous Status of crab Jobs from File %s" % statusFileName)
        statusFile = open(statusFileName, "r")
        jobStatus_dict = json.load(statusFile)
        #print(jobStatus_dict)
        statusFile.close()
    else:
        raise ValueError("File %s exists, but is a directory !!" % statusFileName)
else:
    print("File %s does not yet exist...creating it." % statusFileName)

crabStatus_regex       = r'\s*>>>>>>>>>\s+[0-9]+ [J|j]obs\s+(?P<jobStatus>[a-zA-Z]+)\s*'
crabStatus_matcher     = re.compile(crabStatus_regex)
jobIdList_regex        = r'\s*List of [J|j]obs(\s+[a-zA-Z]+)?\s*:\s*(?P<jobIdList>[0-9\-,]+)*\s*'
jobIdList_matcher      = re.compile(jobIdList_regex)
multipleJobs_regex     = r'(?P<jobId_first>[0-9]+)-(?P<jobId_last>[0-9]+)'
multipleJobs_matcher   = re.compile(multipleJobs_regex)
singleJob_regex        = r'(?P<jobId>[0-9]+)'
singleJob_matcher      = re.compile(singleJob_regex)

key_value_pair_regex   = r'(?P<key>[a-zA-Z0-9_]+)\s*=\s*(?P<value>.+)\s*'
key_value_pair_matcher = re.compile(key_value_pair_regex)
storage_path_regex     = r'[a-zA-Z0-9_/]*\?SFN=(?P<storage_path>[a-zA-Z0-9_/\.]+)\s*'
storage_path_matcher   = re.compile(storage_path_regex)

current_time = time.time()
print("current Time = %s" % time.asctime(time.gmtime(current_time)))

if jobStatus_dict.has_key('lastUpdate_time'):
    print("last Update = %s" % time.asctime(time.gmtime(jobStatus_dict['lastUpdate_time'])))
jobStatus_dict['lastUpdate_time'] = current_time

time_limit = 60*60*24 # maximum time (= one day) for which crab jobs are allowed to stay
                      # in 'Submitted', 'Ready' or 'Scheduled' state before they get automatically resubmitted 

shellScriptCommands = []

crabJobs = runCommand('%s %s' % (executable_ls, crabFilePath)).stdout.readlines()
for crabJob in crabJobs:
    crabJob_match = jobName_matcher.match(crabJob)
    if crabJob_match:
        
        crabJob = crabJob.replace('\n', '')
        print("crabJob = %s" % crabJob)
        if not jobStatus_dict.has_key(crabJob):
            jobStatus_dict[crabJob] = {}
            
        sample = crabJob_match.group('sample')
        #print("sample = %s" % sample)
        channel = crabJob_match.group('channel')
        #print("channel = %s" % channel)
        
        jobIds_force_resubmit = []
        
        #crabStatus = runCommand('%s -status -c %s' % (executable_crab, crabJob))
        #crabStatus_lines = crabStatus.stdout.readlines()
        
        crabStatus_lines = \
"""
crab:  Version 2.7.9 running on Tue Jan 10 10:28:47 2012 CET (09:28:47 UTC)

crab:   1012 Total Jobs 
 >>>>>>>>> 51 Jobs Submitted 
	List of jobs Submitted: 87-137 
 >>>>>>>>> 162 Jobs Ready 
	List of jobs Ready: 501-572,659-748 
 >>>>>>>>> 353 Jobs Scheduled 
	List of jobs Scheduled: 1-86,225-395,483-487,924-1012 
 >>>>>>>>> 1 Jobs Aborted
	You can resubmit them specifying JOB numbers: crab -resubmit <List of jobs>
	List of jobs: 847-848 
 >>>>>>>>> 444 Jobs Done
	Jobs terminated: retrieve them with: crab -getoutput <List of jobs>
	List of jobs: 138-224,396-482,488-491,493-500,573-658,749-846,849-923 

crab:  You can also follow the status of this task on :
	CMS Dashboard: http://dashb-cms-job-task.cern.ch/taskmon.html#task=veelken_crabdirProduceFakeRatePATtuple_PPmuXptGt20Mu15_WplusJets_patV2_1_6gdr08
	Your task name is: veelken_crabdirProduceFakeRatePATtuple_PPmuXptGt20Mu15_WplusJets_patV2_1_6gdr08 

Log file is /tmp/veelken/crab/crabdirProduceFakeRatePATtuple_PPmuXptGt20Mu15_WplusJets_patV2_1/log/crab.log

""".split('\n')

        # read path and name of output file from crab config file
        crabConfigFileName = os.path.join(crabFilePath, crabJob, 'share/crab.cfg')
        crabConfigFile = open(crabConfigFileName, "r")
        crabConfig_lines = crabConfigFile.readlines()
        outputFileName        = None
        outputFilePath_prefix = None
        outputFilePath_suffix = None
        for crabConfig_line in crabConfig_lines:
            key_value_pair_match = key_value_pair_matcher.match(crabConfig_line)
            if key_value_pair_match:
                key = key_value_pair_match.group('key')
                #print("key = %s" % key)
                value = key_value_pair_match.group('value')
                #print("value = %s" % value)
                if key == 'output_file':
                    outputFileName = value
                elif key == 'user_remote_dir':
                    outputFilePath_suffix = value
                elif key == 'storage_path':    
                    storage_path_match = storage_path_matcher.match(value)
                    if storage_path_match:
                        outputFilePath_prefix = storage_path_match.group('storage_path')
                    else:
                        raise ValueError("Failed to read 'storage_path' from config file %s !!" \
                                         % crabConfigFileName)
        if not (outputFileName and outputFilePath_prefix and outputFilePath_suffix):
            raise ValueError("Failed to read 'output_file', 'user_remote_dir' and 'storage_path' from config file %s !!" \
                             % crabConfigFileName)
        outputFileName_regex = outputFileName.replace('.', '_(?P<jobId>\d+)_(?P<try>\d+)_(?P<hash>[a-zA-Z0-9]+).')
        outputFileName_matcher = re.compile(outputFileName_regex)

        # read list of files existing in output file path
        outputFilePath = outputFilePath_prefix
        if not (outputFilePath.endswith('/') or outputFilePath_suffix.startswith('/')):
            outputFilePath += '/'
        outputFilePath += outputFilePath_suffix
        print("outputFilePath = %s" % outputFilePath)
        outputFileNames = [ file_info['file'] for file_info in castor.nslsl(outputFilePath) ]

        # check if job got aborted or stuck in state 'Submitted', 'Ready' or 'Scheduled' for more than one day
        isMatched_status = False
        for crabStatus_line in crabStatus_lines:
            crabStatus_match = crabStatus_matcher.match(crabStatus_line)
            if crabStatus_match:
                #print("line '%s' matches <jobStatus> pattern" % crabStatus_line)
                jobStatus = crabStatus_match.group('jobStatus')
                #print("jobStatus = '%s'" % jobStatus)
                isMatched_status = True
            elif isMatched_status:
                jobIdList_match = jobIdList_matcher.match(crabStatus_line)
                if jobIdList_match:
                    #print("line '%s' matches <jobIdList> pattern" % crabStatus_line)
                    jobIdList = jobIdList_match.group('jobIdList')
                    #print("jobIdList = %s" % jobIdList)
                    jobId_items = jobIdList.split(',')
                    jobIds = []
                    for jobId_item in jobId_items:
                        #print("jobId_item = '%s'" % jobId_item)
                        multipleJobs_match = multipleJobs_matcher.match(jobId_item)
                        singleJob_match = singleJob_matcher.match(jobId_item)
                        if multipleJobs_match:
                            jobId_first = int(multipleJobs_match.group('jobId_first'))
                            jobId_last = int(multipleJobs_match.group('jobId_last'))
                            #print("jobId: first = %i, last = %i" % (jobId_first, jobId_last))
                            jobIds.extend(range(jobId_first, jobId_last + 1))
                        elif sshellScriptCommandsingleJob_match:
                            jobId = int(singleJob_match.group('jobId'))
                            #print("jobId = '%i'" % jobId)
                            jobIds.append(jobId)
                        else:
                            raise ValueError("Failed to match jobId item '%s' !!" % jobId_item)
                    #print("jobIds = %s" % jobIds)
                    for jobId in jobIds:
                        jobId_string = "%i" % jobId
                        if jobStatus in [ 'Aborted' ]:
                            # resubmit aborted jobs immediately
                            print("Info: jobId = %i got aborted --> resubmitting it" % jobId)
                            jobIds_force_resubmit.append(jobId)
                        elif jobStatus in [ 'Submitted', 'Ready', 'Scheduled' ]:
                            # check if status has not changed for more than one day,
                            # in which case assume that crab job got "stuck" and resubmit it
                            lastJobStatus, lastJobStatusChange_time = jobStatus_dict[crabJob][jobId_string]
                            if (current_time - lastJobStatusChange_time) > time_limit:
                                print("Info: jobId = %i got stuck in state '%s' --> resubmitting it" % (jobId, jobStatus))
                                jobIds_force_resubmit.append(jobId)
                        elif jobStatus in [ 'Done' ]:
                            outputFileNames_matched = []
                            for outputFileName in outputFileNames_matched:
                                outputFileName_match = outputFileName_matcher.match(outputFileName)
                                if outputFileName_match:
                                    outputFileJobId = outputFileName_match.group('jobId')
                                    if outputFileJobId == jobId_string:
                                        outputFileNames_matched.append(outputFileName)
                            if len(outputFileNames_matched) == 0:
                                print("Info: jobId = %i produced no output file --> resubmitting it" % jobId)
                                jobIds_force_resubmit.append(jobId)
                            elif len(outputFileNames_matched) > 1:
                                print("Warning: jobId = %i produced multiple output files = %s !!" % (jobId, outputFileNames_matched))
                            
                        # update job status dictionary
                        if jobStatus_dict[crabJob].has_key(jobId_string):
                            lastJobStatus, lastJobStatusChange_time = jobStatus_dict[crabJob][jobId_string]
                            if jobStatus != lastJobStatus:
                                #print(" Job #%i: updating Status %s --> %s" % (jobId, lastJobStatus, jobStatus))
                                jobStatus_dict[crabJob][jobId_string] = ( jobStatus, current_time )
                        else:
                            #print(" Job #%i: adding Status = %s" % (jobId, jobStatus))
                            jobStatus_dict[crabJob][jobId_string] = ( jobStatus, current_time )
                    isMatched_status = False

        # sort jobIds in ascending order
        jobIds_force_resubmit.sort()
                    
        # resubmit crab jobs which got aborted or stayed in 'Submitted', 'Ready' or 'Scheduled' state
        # for more than one day
        #print("jobIds_force_resubmit = %s" % jobIds_force_resubmit)
        if len(jobIds_force_resubmit) > 0:
            commandLine = '%s -forceResubmit %s -c %s' % \
              (executable_crab, ",".join([ "%i" % jobId for jobId in jobIds_force_resubmit ]), os.path.join(crabFilePath, crabJob))
            shellScriptCommands.append(commandLine)

        print("\n")

#write shell script to resubmit crab jobs
shellScriptFileName = 'crabSitter.csh'
shellScriptFile = open(shellScriptFileName, "w")
shellScriptFile.write("#!/bin/csh -f\n")
for shellScriptCommand in shellScriptCommands:
    shellScriptFile.write("%s\n" % shellScriptCommand)
shellScriptFile.close()    
runCommand('chmod +x %s' % shellScriptFileName)

#print(jobStatus_dict)

statusFile = open(statusFileName, "w")
json.dump(jobStatus_dict, statusFile)
statusFile.close()

print("Finished running 'crabSitter.py'. Now execute './%s'." % shellScriptFileName)
