#!/usr/bin/env cmsRun

'''

Factorize the a analysis sample.  Currently specialized for AHtoMuTau

Author: Evan K. Friis (UC Davis)

'''

import FWCore.ParameterSet.Config as cms
import sys

# Check if someone used cmsRun instead of running as a script
argument_offset = 0
if sys.argv[0] == 'cmsRun':
    argument_offset = 1

if not len(sys.argv) > 3+argument_offset:
    sys.exit("Usage: %s [outputFile] [inputFile1] [sample]")

outputFileName = sys.argv[1+argument_offset]
inputFileName = sys.argv[(2+argument_offset)]
sampleName = sys.argv[(3+argument_offset)]

print "Out:", outputFileName
print "In:", inputFileName
print "Sample: ", sampleName

process = cms.Process('genericScaler')
process.DQMStore = cms.Service("DQMStore")

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(0))

process.source = cms.Source("EmptySource")

process.loader = cms.EDAnalyzer(
    "DQMFileLoader",
    toMerge = cms.PSet(
        inputFileNames = cms.vstring(inputFileName),
        dqmDirectory_store = cms.string('/'),
    )
)

# For the factorizer to hook into
process.loadAndFactorizeAHtoMuTauSamples = cms.Sequence()

import TauAnalysis.Configuration.tools.factorizationTools as tools
tools.enableFactorization_makeAHtoMuTauPlots_grid(
    process, samplesToFactorize = [ sampleName ], relevantMergedSamples = [])


process.save = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string(outputFileName)
)
process.path = cms.Path(
    process.loader*
    process.loadAndFactorizeAHtoMuTauSamples*
    process.save
)


