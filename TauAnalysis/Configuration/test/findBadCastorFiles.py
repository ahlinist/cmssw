#!/usr/bin/env python

import sys
import subprocess
import shlex
import threading
import Queue
import time

from TauAnalysis.Configuration.userRegistry import getAnalysisFilePath, getJobId

# Find all files on castor which have zero size or
# cause an error when opened by ROOT
#
# Author: Christian Veelken (UC Davis)
# 
# Examples:
# 
# ./findBadCastorFiles.py channel | xargs -n 1 rfrm
#
# Safer:
# ./findBadCastorFiles.py channel badfiles.list
# after checking badfiles.list is okay,
# cat badfiles.list | xargs -n 1 rfrm

print("<findBadCastorFiles>:")

if len(sys.argv) != 3:
    raise ValueError("Usage: findBadCastorFiles.py channel badfiles.list")

channel = sys.argv[1]
outputFileName = sys.argv[2]

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

    def __init__(self, work_queue, results_queue):
        super(Worker, self).__init__()
        self.work_queue = work_queue
        self.results_queue = results_queue

    def run(self):
        while True:
            try:
                fileName = self.work_queue.get()
                self.process(fileName)
            finally:
                self.work_queue.task_done()

    def process(self, fileName):
        commandLine = 'nsls -l %s' % fileName
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

        commandLine = 'edmFileUtil rfio:%s' % fileName
        print("calling %s..." % commandLine)
        args = shlex.split(commandLine)
        retval = subprocess.Popen(args, stdout = subprocess.PIPE)
        retval.wait()

        status = retval.stdout.read()

        if status.find("FatalRootError") != -1:
            self.results_queue.put(fileName)

work_queue = Queue.Queue()
results_queue = Queue.Queue()

for iThread in range(numThreads):
    worker = Worker(work_queue, results_queue)
    worker.daemon = True
    worker.start()

outputFile = open(outputFileName, "w")

results_thread = threading.Thread(target = lambda: write_fileNames(results_queue, outputFile))
results_thread.daemon = True
results_thread.start()

castorFilePath = '/castor/cern.ch/' + getAnalysisFilePath(channel)
castorFilePath = castorFilePath.replace('//', '/')
castorFilePath = castorFilePath.replace('/castor/cern.ch/castor/cern.ch/', '/castor/cern.ch/')
print(" castorFilePath = %s" % castorFilePath)

jobId = getJobId(channel)
print(" jobId = %s" % jobId)

commandLine = 'nsls %s' % castorFilePath
args = shlex.split(commandLine)
retval = subprocess.Popen(args, stdout = subprocess.PIPE)
#retval.wait()

files = retval.stdout.read().split('\n')

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
