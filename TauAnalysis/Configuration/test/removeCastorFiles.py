#!/usr/bin/env python

import os
import re
import sys
import subprocess
import shlex
import time

import TauAnalysis.Configuration.userRegistry as reg
import TauAnalysis.Configuration.tools.castor as castor
import TauAnalysis.Configuration.tools.eos as eos

#--------------------------------------------------------------------------------
# Delete all files matching pattern passed as argument
# stored on castor/eos
#
# Author: Christian Veelken (LLR)
# 
# Examples:
# 
# ./findDuplicateCastorFiles.py filepath jodId badfiles.list
#
#   after checking badfiles.list is okay,
#
# cat duplicatefiles.list | xargs -n 1 rfrm
#--------------------------------------------------------------------------------

print("<removeCastorFiles>:")

options = {
    'executable_ls' : {
        'castor' : 'nsls',
        'eos'    : 'eos ls',
        'local'  : 'ls'
    },
    'executable_rm' : {
        'castor' : 'rfrm',
        'eos'    : 'cmsRm',
        'local'  : 'rm'
    }
}

if not len(sys.argv) == 4:
    raise ValueError("Usage: removeCastorFiles.py channel jobId badfiles.list")

channel = sys.argv[1]
jobId = sys.argv[2]
outputFileName = sys.argv[3]

inputFilePath = None
if channel.find('/castor/') == 0:
    # 'channel' points to files on castor
    inputFilePath = channel
elif channel.find('/store/') == 0:
    # 'channel' points to files on eos disk
    inputFilePath = channel
elif channel.find('/') == 0:
    # 'channel' points to files on local disk
    inputFilePath = channel
else:
    # 'channel' really corresponds to analysis channel,
    # get file path from TauAnalysis/Configuration/python/userRegistry.py
    inputFilePath = '/castor/cern.ch/' + reg.getAnalysisFilePath(channel)
    inputFilePath = inputFilePath.replace('//', '/')
    inputFilePath = inputFilePath.replace('/castor/cern.ch/castor/cern.ch/', '/castor/cern.ch/')
print(" inputFilePath = %s" % inputFilePath)

mode = None
if inputFilePath.find('/castor/') == 0: 
    mode = 'castor'
if inputFilePath.find('/store/') == 0: 
    mode = 'eos'    
else:
    mode = 'local'

print(" jobId = %s" % jobId)

if mode == 'castor':
    files = [ file_info for file_info in castor.nslsl(inputFilePath) ]
elif mode == 'eos':
    files = [ file_info for file_info in eos.lsl(inputFilePath) ]
else:
    commandLine = '%s %s' % (options['executable_ls'][mode], inputFilePath)
    args = shlex.split(commandLine)
    retval = subprocess.Popen(args, stdout = subprocess.PIPE)
    #retval.wait()

    files = retval.stdout.read().split('\n')
    #print(" files = %s" % files)

fileName_regex = r"(?P<fileName_base>[a-zA-Z0-9_]+)_(?P<gridJob>\d*)(_(?P<gridTry>\d*))*_(?P<hash>[a-zA-Z0-9]*).root"
fileName_matcher = re.compile(fileName_regex)

fileNames = []

print("--> found %i file" % len(files))

for file in files:

    fileName = None
    if mode == 'castor':
        fileName = file['file']
    elif mode == 'eos':
        fileName = file['file']
    else:
        fileName = file
            
    # skip files from different submissions
    if fileName.find(jobId) == -1:
	continue

    if not fileName_matcher.match(fileName):
        continue

    # skip entry referring to inputFilePath directory 
    if len(fileName) > 1:

        fileNames.append(os.path.join(inputFilePath, fileName))

outputFile = open(outputFileName, "w")
for fileName in fileNames:
    print("--> found file to remove: %s" % fileName)
    outputFile.write("%s\n" % fileName)
outputFile.close()

print("execute 'cat %s | xargs -n 1 %s' to delete duplicated files." % (outputFileName, options['executable_rm'][mode]))
