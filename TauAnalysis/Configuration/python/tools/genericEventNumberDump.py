#!/usr/bin/env cmsRun

import FWCore.ParameterSet.Config as cms
import sys

# Check if someone used cmsRun instead of running as a script
argument_offset = 0
if sys.argv[0] == 'cmsRun':
    argument_offset = 1

if not len(sys.argv) > 3+argument_offset:
    sys.exit("Usage: %s [outputFile] [inputFile1] [dqmDirectory] [dqmDirectory] ..." % sys.argv[0])

outputFileName = sys.argv[1+argument_offset]
inputFileName = sys.argv[(2+argument_offset)]
dqmDirectories = sys.argv[(3+argument_offset):]

print "output txt file:", outputFileName
print "input harvesting file:", inputFileName
print "dqm directories:", ", ".join(dqmDirectories)

process = cms.Process('generateEventNumberDump')
process.DQMStore = cms.Service("DQMStore")

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(0))

process.source = cms.Source("EmptySource")

process.load = cms.EDAnalyzer("DQMFileLoader",
    toMerge = cms.PSet(
        inputFileNames = cms.vstring(inputFileName),
        dqmDirectory_store = cms.string('')
    )
)

process.dumpNumbers = cms.EDAnalyzer(
    "DQMDumpRunLumiSectionEventNumbers",
    dqmDirectories = cms.vstring(dqmDirectories),
    runLumiSectionEventNumberFileName = cms.string(outputFileName),
    separator = cms.string(':'),
)

process.dump = cms.Sequence(
    process.load
   +process.dumpNumbers
)

process.p = cms.Path(process.dump)
