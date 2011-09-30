#!/usr/bin/env python

import sys
import subprocess
import shlex
import threading
import Queue
import time

import TauAnalysis.Configuration.userRegistry as reg

#--------------------------------------------------------------------------------
# Find all files on castor which have zero size or
# cause an error when opened by ROOT
#
# Author: Christian Veelken (UC Davis)
# 
# Examples:
# 
# ./findBadCastorFiles.py channel badfiles.list
#
#   after checking badfiles.list is okay,
#
# cat badfiles.list | xargs -n 1 rfrm
#--------------------------------------------------------------------------------

print("<findBadCastorFiles>:")

options = {
    'executable_ls' : {
        'castor' : 'nsls',
        'local'  : 'ls'
    },
    'fileName_prefix' : {
        'castor' : 'rfio:',
        'local'  : 'file:'
    }
}

if not (len(sys.argv) >= 3 and len(sys.argv) <= 4):
    raise ValueError("Usage: findBadCastorFiles.py channel jobId badfiles.list")

channel = sys.argv[1]
jobId = None
if len(sys.argv) >= 4:
    jobId = sys.argv[2]
outputFileName = sys.argv[len(sys.argv) - 1]

numThreads = 100

def write_fileNames(results_queue, outputFile):
    while True:
        try:
            fileName = results_queue.get()
            print("--> found 'bad' file: %s" % fileName)
            outputFile.write(fileName + '\n')
        finally:
            results_queue.task_done()

class Worker(threading.Thread):

    def __init__(self, work_queue, results_queue, mode):
        super(Worker, self).__init__()
        self.work_queue = work_queue
        self.results_queue = results_queue
        self.mode = mode

    def run(self):
        while True:
            try:
                fileName = self.work_queue.get()
                self.process(fileName)
            finally:
                self.work_queue.task_done()

    def process(self, fileName):
        commandLine = '%s -l %s' % (options['executable_ls'][mode], fileName)
        args = shlex.split(commandLine)
        retval = subprocess.Popen(args, stdout = subprocess.PIPE)

        fileInfos = retval.stdout.read().split('\n')

        for fileInfo in fileInfos:
            items = fileInfo.split()
            #print("items: %s" % items)

            if len(items) > 5:
                fileSize = items[4]        
                if fileSize == 0:                    
                    self.results_queue.put(fileName)

        time.sleep(1)            

        commandLine = 'edmFileUtil %s%s' % (options['fileName_prefix'][mode], fileName)
        print("calling %s..." % commandLine)
        args = shlex.split(commandLine)
        retval = subprocess.Popen(args, stdout = subprocess.PIPE)
        retval.wait()

        status = retval.stdout.read()

        if status.find("FatalRootError") != -1:
            self.results_queue.put(fileName)

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

work_queue = Queue.Queue()
results_queue = Queue.Queue()

for iThread in range(numThreads):
    worker = Worker(work_queue, results_queue, mode)
    worker.daemon = True
    worker.start()

outputFile = open(outputFileName, "w")

results_thread = threading.Thread(target = lambda: write_fileNames(results_queue, outputFile))
results_thread.daemon = True
results_thread.start()

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

for file in files:
    # skip files from different submissions
    if file.find(jobId) == -1:
	continue

    fileName = castorFilePath + '/' + file
    fileName = fileName.replace('//', '/')
    # skip entry referring to castorFilePath directory 
    if len(fileName) > (len(castorFilePath) + 1):
        work_queue.put(fileName)

work_queue.join()
results_queue.join()

outputFile.close()

print("execute 'cat badfiles.list | xargs -n 1 rfrm' to delete bad files.")
