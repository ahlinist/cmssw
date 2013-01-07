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
# Find duplicate files resulting from failing crab jobs or
# crab jobs which were erroneously submitted multiple times
#
# Author: Christian Veelken (UC Davis)
# 
# Examples:
# 
# ./findDuplicateCastorFiles.py channel badfiles.list
#
#   after checking badfiles.list is okay,
#
# cat badfiles.list | xargs -n 1 rfrm
#--------------------------------------------------------------------------------

print("<findDuplicateCastorFiles>:")

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

if not (len(sys.argv) >= 3 and len(sys.argv) <= 4):
    raise ValueError("Usage: findDuplicateCastorFiles.py channel jobId badfiles.list")

channel = sys.argv[1]
jobId = None
if len(sys.argv) >= 4:
    jobId = sys.argv[2]
outputFileName = sys.argv[len(sys.argv) - 1]

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

if jobId is None:
    reg.overrideJobId(channel, '2011Oct30') # CV: need to overwrite this in order to match Mauro's filenames
    jobId = reg.getJobId(channel)
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

fileNamesAndProperties_dict = {}

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

        fileSize = None
        date_and_time = None
        
        if mode == 'castor':
            fileSize = file['size']
            date_and_time = file['time']
        elif mode == 'eos':
            fileSize = file['size']
            date_and_time = file['time']    
        else:
            commandLine = '%s -l %s' % (options['executable_ls'][mode], os.path.join(inputFilePath, fileName))
            args = shlex.split(commandLine)
            retval = subprocess.Popen(args, stdout = subprocess.PIPE)

            fileInfos = retval.stdout.read().split('\n')

            for fileInfo in fileInfos:
                items = fileInfo.split()
                #print("items: %s" % items)

                if len(items) > 5:
                    fileSize = int(items[4])
                    date_and_time = time.mktime(time.strptime("%s %s %s" % (items[5], items[6], items[7]), "%b %d %H:%M"))

        #print(" fileName = %s" %  fileName)
        fileName_base = str(fileName_matcher.match(fileName).group('fileName_base'))

        #print(" fileSize = %i" %  fileSize)
        #print(" date_and_time = %f" % date_and_time)
        #print(" fileName_base = %s" %  fileName_base)

        fileNameAndProperties_entry = {
            'fileName'      : os.path.join(inputFilePath, fileName),
            'fileSize'      : fileSize,
            'date_and_time' : date_and_time
        }

        if fileName_base not in fileNamesAndProperties_dict:
            fileNamesAndProperties_dict[fileName_base] = [ fileNameAndProperties_entry ]
        else:
            fileNamesAndProperties_dict[fileName_base].append(fileNameAndProperties_entry)

outputFile = open(outputFileName, "w")
for fileName_base, fileNameAndProperties_entry in fileNamesAndProperties_dict.items():
    if len(fileNameAndProperties_entry) > 1:
        indexToKeep = 0
        for index, duplicate in enumerate(fileNameAndProperties_entry):
            if duplicate['fileSize']      >  fileNameAndProperties_entry[indexToKeep]['fileSize'] or \
              (duplicate['fileSize']      == fileNameAndProperties_entry[indexToKeep]['fileSize'] and \
               duplicate['date_and_time'] >  fileNameAndProperties_entry[indexToKeep]['date_and_time']):
                indexToKeep = index

        for index, duplicate in enumerate(fileNameAndProperties_entry):
            if index != indexToKeep:
                print("--> found duplicate file to remove: %s" % fileNameAndProperties_entry[index]['fileName'])
                print(" (keeping %s)" % fileNameAndProperties_entry[indexToKeep]['fileName'])
                outputFile.write("%s\n" % fileNameAndProperties_entry[index]['fileName'])
outputFile.close()

print("execute 'cat %s | xargs -n 1 %s' to delete duplicated files." % (outputFileName, options['executable_rm'][mode]))
