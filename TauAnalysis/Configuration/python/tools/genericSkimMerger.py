#!/usr/bin/env cmsRun

'''

Merge EDM files together.  Based on copyPickMerge_cfg by Charles Plager

'''

import FWCore.ParameterSet.Config as cms
import sys

# Check if someone used cmsRun instead of running as a script
argument_offset = 0
if sys.argv[0] == 'cmsRun':
    argument_offset = 1

if not len(sys.argv) > 3+argument_offset:
    sys.exit("Usage: %s [outputFile] [inputFile1] [inputFile2] ..." % sys.argv[0])

outputFileName = sys.argv[1+argument_offset]
inputFileNames = sys.argv[(2+argument_offset):]

process = cms.Process('MERGE')

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))

inputFileNamesPrefixed = [ 'file:' + name for name in inputFileNames ]

process.source = cms.Source (
    "PoolSource",
    fileNames = cms.untracked.vstring (inputFileNamesPrefixed),
    noEventSort = cms.untracked.bool(True),
    duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
    skipBadFiles = cms.untracked.bool(True),
)

process.Out = cms.OutputModule("PoolOutputModule",
        fileName = cms.untracked.string (outputFileName)
)

process.end = cms.EndPath(process.Out)
