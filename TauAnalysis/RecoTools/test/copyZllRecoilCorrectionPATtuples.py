#!/usr/bin/env python

import TauAnalysis.Configuration.tools.castor as castor
import TauAnalysis.TauIdEfficiency.tools.castor_mirror2 as castor_mirror

import os
import subprocess
import shlex
import time

version = 'v6_00'

# Get all the skim files from the castor directory
sourceFilePath = "/castor/cern.ch/user/v/veelken/CMSSW_5_2_x/PATtuples/ZllRecoilCorrection/%s/" % version

targetFilePath = "/data2/veelken/CMSSW_5_3_x/PATtuples/ZllRecoilCorrection/%s/" % version

samplesToCopy = [
    # modify in case you want to submit jobs for some of the samples only...
]

#keepRunning = False
keepRunning = True

def createFilePath_recursively(filePath):
    filePath_items = filePath.split('/')
    currentFilePath = "/"
    for filePath_item in filePath_items:
        currentFilePath = os.path.join(currentFilePath, filePath_item)
        if len(currentFilePath) <= 1:
            continue
        if not os.path.exists(currentFilePath):
            os.mkdir(currentFilePath)

createFilePath_recursively(targetFilePath)

maxLoops = 1
if keepRunning:
    maxLoops = 1000

for iLoop in range(maxLoops):
    print "loop #%i: %s" % (iLoop, time.strftime('%X %x %Z'))

    source_files = [ file_info['path'] for file_info in castor.nslsl(sourceFilePath) ]

    files_to_copy = []

    for source_file in source_files:

        if not (source_file.find("ZllRecoilCorrectionPATtuple") != -1 and source_file.find("%s" % version) != -1):
            continue

        isSampleToCopy = False
        if len(samplesToCopy) == 0:
            isSampleToCopy = True
        for sampleToCopy in samplesToCopy:
            if source_file.find(sampleToCopy) != -1:
                isSampleToCopy = True
        if not isSampleToCopy:
            continue;

        target_file = source_file.replace(sourceFilePath, targetFilePath)
    
        #print("mirroring %s --> %s" % (source_file, target_file))
        files_to_copy.append(source_file)

    #files_to_copy = files_to_copy[len(files_to_copy)-1:0:-1]
    #print "files_to_copy = %s" % files_to_copy

    #castor_mirror.mirror_files(castor_mirror.needs_local_copy(files_to_copy, [ targetFilePath ]), [ targetFilePath ], max_jobs = 3, skipWaiting = True)
    castor_mirror.mirror_files(castor_mirror.needs_local_copy(files_to_copy, [ targetFilePath ]), [ targetFilePath ], max_jobs = 1, skipWaiting = False)

    if (iLoop + 1) < maxLoops:
        time.sleep(3600)

