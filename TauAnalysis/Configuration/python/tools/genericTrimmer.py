#!/usr/bin/env cmsRun

# usage: ../python/tools/genericTrimmer.py test.root /data1/friis/Run10_v2/harvested_AHtoMuTau_Zmumu_Run10.root "drop ahMuTauAnalyzer_wBtag/*"

import FWCore.ParameterSet.Config as cms
import sys

# Check if someone used cmsRun instead of running as a script
argument_offset = 0
if sys.argv[0] == 'cmsRun':
    argument_offset = 1

if not len(sys.argv) > 3+argument_offset:
    sys.exit("Usage: %s [outputFile] [inputFile1] [keep statements] ..."
             % sys.argv[0])

outputFileName = sys.argv[1+argument_offset]
inputFileName = sys.argv[(2+argument_offset)]
keeps = sys.argv[(3+argument_offset):]

process = cms.Process('genericHarvester')
process.DQMStore = cms.Service("DQMStore")

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(0))

process.source = cms.Source("EmptySource")

process.load = cms.EDAnalyzer("DQMFileLoader",
    toMerge = cms.PSet(
        inputFileNames = cms.vstring(inputFileName),
        dqmDirectory_store = cms.string('')
    )
)

process.save = cms.EDAnalyzer(
    "DQMSimpleFileSaver",
    outputFileName = cms.string(outputFileName),
    outputCommands = cms.vstring(keeps)
)

print process.save

process.harvest = cms.Sequence(
    process.load
   + process.save
)

process.p = cms.Path(process.harvest)

print process.dumpPython()
