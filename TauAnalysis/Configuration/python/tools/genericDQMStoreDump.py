#!/usr/bin/env cmsRun

import FWCore.ParameterSet.Config as cms
import sys

# Check if someone used cmsRun instead of running as a script
argument_offset = 0
if sys.argv[0] == 'cmsRun':
    argument_offset = 1

if not len(sys.argv) > 1+argument_offset:
    sys.exit("Usage: %s  [inputFile1] ..." % sys.argv[0])

inputFileName = sys.argv[(1+argument_offset)]

print "input harvesting file:", inputFileName

process = cms.Process('generateEventNumberDump')
process.DQMStore = cms.Service("DQMStore")

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(0))

process.source = cms.Source("EmptySource")

process.dumpDQMStore = cms.EDAnalyzer("DQMStoreDump")

process.load = cms.EDAnalyzer("DQMFileLoader",
    toMerge = cms.PSet(
        inputFileNames = cms.vstring(inputFileName),
        dqmDirectory_store = cms.string('/')
    )
)

process.dump = cms.Sequence(
    process.load
   +process.dumpDQMStore
)

process.p = cms.Path(process.dump)
