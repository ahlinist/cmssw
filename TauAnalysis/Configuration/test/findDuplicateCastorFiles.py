#!/usr/bin/env python

import re
import sys
import subprocess
import shlex
import time

import TauAnalysis.Configuration.userRegistry as reg

#--------------------------------------------------------------------------------
# Find duplicate files resulting from failing crab jobs or
# crab jobs which were erroneously submitted multiple times
#
# Author: Christian Veelken (UC Davis)
# 
# Examples:
# 
# ./findDuplicateCastorFiles.py channel duplicatefiles.list
#
#   after checking badfiles.list is okay,
#
# cat duplicatefiles.list | xargs -n 1 rfrm
#--------------------------------------------------------------------------------

print("<findDuplicateCastorFiles>:")

options = {
    'executable_ls' : {
        'castor' : 'nsls',
        'local'  : 'ls'
    }
}

if not (len(sys.argv) >= 3 and len(sys.argv) <= 4):
    raise ValueError("Usage: findDuplicateCastorFiles.py channel jobId badfiles.list")

channel = sys.argv[1]
jobId = None
if len(sys.argv) >= 4:
    jobId = sys.argv[2]
outputFileName = sys.argv[len(sys.argv) - 1]

castorFilePath = None
if channel.find('/castor/') == 0:
    # 'channel' points to files on castor
    castorFilePath = channel
elif channel.find('/') == 0:
    # 'channel' points to files on local disk
    castorFilePath = channel
else:
    # 'channel' really corresponds to analysis channel,
    # get file path from TauAnalysis/Configuration/python/userRegistry.py
    castorFilePath = '/castor/cern.ch/' + reg.getAnalysisFilePath(channel)
    castorFilePath = castorFilePath.replace('//', '/')
    castorFilePath = castorFilePath.replace('/castor/cern.ch/castor/cern.ch/', '/castor/cern.ch/')
print(" castorFilePath = %s" % castorFilePath)

mode = None
if castorFilePath.find('/castor/') == 0: 
    mode = 'castor'
else:
    mode = 'local'

if jobId is None:
    reg.overrideJobId(channel, '2011Jul06') # CV: need to overwrite this in order to match Mauro's filenames
    jobId = reg.getJobId(channel)
print(" jobId = %s" % jobId)

commandLine = '%s %s' % (options['executable_ls'][mode], castorFilePath)
args = shlex.split(commandLine)
retval = subprocess.Popen(args, stdout = subprocess.PIPE)
#retval.wait()

files = retval.stdout.read().split('\n')
#print(" files = %s" % files)

fileName_regex = r"(?P<fileName_base>[a-zA-Z0-9_]+)_(?P<gridJob>\d*)(_(?P<gridTry>\d*))*_(?P<hash>[a-zA-Z0-9]*).root"
fileName_matcher = re.compile(fileName_regex)

fileNamesAndProperties_dict = {}

for file in files:
    # skip files from different submissions
    if file.find(jobId) == -1:
	continue

    if not fileName_matcher.match(file):
        continue

    fileName = castorFilePath + '/' + file
    fileName = fileName.replace('//', '/')
    # skip entry referring to castorFilePath directory 
    if len(fileName) > (len(castorFilePath) + 1):
        commandLine = '%s -l %s' % (options['executable_ls'][mode], fileName)
        args = shlex.split(commandLine)
        retval = subprocess.Popen(args, stdout = subprocess.PIPE)

        fileInfos = retval.stdout.read().split('\n')

        for fileInfo in fileInfos:
            items = fileInfo.split()
            #print("items: %s" % items)

            if len(items) > 5:
                fileSize = int(items[4])
                #print(" fileSize = %i" %  fileSize)

                date_and_time = time.mktime(time.strptime("%s %s %s" % (items[5], items[6], items[7]), "%b %d %H:%M"))
                #print(" date_and_time = %f" % date_and_time)

                fileName_base = str(fileName_matcher.match(file).group('fileName_base'))
                #print(" fileName_base = %s" %  fileName_base)

                fileNameAndProperties_entry = {
                    'fileName'      : fileName,
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

print("execute 'cat duplicatefiles.list | xargs -n 1 rfrm' to delete duplicated files.")
