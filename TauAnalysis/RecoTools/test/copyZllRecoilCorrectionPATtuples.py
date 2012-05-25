#!/usr/bin/env python

import TauAnalysis.Configuration.tools.castor as castor
import TauAnalysis.TauIdEfficiency.tools.castor_mirror2 as castor_mirror

import subprocess
import shlex

version = "v5_19"

# Get all the skim files from the castor directory
sourceFilePath = "/castor/cern.ch/user/v/veelken/CMSSW_5_2_x/PATtuples/ZllRecoilCorrection/%s/" % version

source_files = [ file_info['path'] for file_info in castor.nslsl(sourceFilePath) ]

targetFilePath = "/data2/veelken/CMSSW_5_2_x/PATtuples/ZllRecoilCorrection/%s/" % version

samplesToCopy = [
    # modify in case you want to submit jobs for some of the samples only...
]

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
    
    print("copying %s --> %s" % (source_file, target_file))
    files_to_copy.append(source_file)

#castor_mirror.mirror_files(castor_mirror.needs_local_copy(files_to_copy, [ targetFilePath ]), [ targetFilePath ], max_jobs = 10, skipWaiting = False)
castor_mirror.mirror_files(castor_mirror.needs_local_copy(files_to_copy, [ targetFilePath ]), [ targetFilePath ], max_jobs = 1, skipWaiting = True)
