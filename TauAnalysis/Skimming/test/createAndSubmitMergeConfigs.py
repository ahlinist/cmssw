#!/usr/bin/env python

import sys

from TauAnalysis.Configuration.submitToBatch import submitToBatch
from TauAnalysis.Configuration.makeReplacementsMerge import makeReplacementsMerge

#usage message
if len(sys.argv) != 4:
	print '\nUsage: creatAndSubmitMergeConfigs.py <sampleName> <total number of input files> <input files per output file>'
	print 'e.g. sampleName = Ztautau, QCD_EMenriched_Pt20to30, etc.\n'
	print 'template config file mergeSkimOutput_cfg.py must exist and be editied appropriately\n'
	sys.exit(1) 

# get command line args
sampleName = sys.argv[1]
numInputFiles = int(sys.argv[2])
numInputFilesPerOutputFile = int(sys.argv[3])

# calculate file ranges
numOutputFiles = numInputFiles/numInputFilesPerOutputFile
rem = numInputFiles%numInputFilesPerOutputFile
if rem != 0:
	numOutputFiles += 1

# name of the directory (either on afs area or castor)
# to which all .root files produced by the cmsRun job will be copied
castorDirectory = "/castor/cern.ch/user/j/jkolb/eTauSkims/bgEst/"
outputDirectory = castorDirectory + sampleName

# set input file path and base filename
inputFileNameBase = castorDirectory + sampleName + "/skimElecTau_"

# loop over output files, submitting one job each
for i in range(0, numOutputFiles):
	# calculate range of input files for this job
	min = (i * numInputFilesPerOutputFile ) + 1
	max = (i + 1) * numInputFilesPerOutputFile
	if i + 1 == numOutputFiles:
		if rem != 0:
			max = (i * numInputFilesPerOutputFile) + rem
	print "Doing output file",i+1,":",min,"-",max
	
	submitToBatch(configFile = "mergeSkimOutput_cfg.py", channel = "ZtoElecTau", sample = "%(sName)s_%(i)02d" % {"sName" : sampleName, "i" : (i + 1)},
		replFunction = makeReplacementsMerge,
		replacements = "part = %(i)02d; minFileNum = %(min)d; maxFileNum = %(max)d; maxEvents = -1; inputFileNameBase = %(inDir)s" % {"i" : (i + 1), "min" : min, "max" : max, "inDir" : inputFileNameBase },
		job = "merge", queue = "8nh", outputDirectory = outputDirectory, resourceRequest = "", submit = "yes")

