#!/usr/bin/env cmsRun

import FWCore.ParameterSet.Config as cms
import sys

# Check if someone used cmsRun instead of running as a script
argument_offset = 0
if sys.argv[0] == 'cmsRun':
    argument_offset = 1

if not len(sys.argv) > 3+argument_offset:
    sys.exit("Usage: %s [outputFile] [inputFile1] [sample] [sample_defintions]")

outputFileName = sys.argv[1+argument_offset]
inputFileName = sys.argv[(2+argument_offset)]
sampleName = sys.argv[(3+argument_offset)]
defintions = sys.argv[(4+argument_offset)]

process = cms.Process('genericScaler')
process.DQMStore = cms.Service("DQMStore")

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(0))

process.source = cms.Source("EmptySource")

__import__(defintions)
sample_defs = sys.modules[defintions]
sample_info = sample_defs.RECO_SAMPLES[sampleName]

process.load = cms.EDAnalyzer(
    "DQMFileLoader",
    toMerge = cms.PSet(
        inputFileNames = cms.vstring(inputFileName),
        dqmDirectory_store = cms.string('/harvested/%s' % sampleName),
    )
)

# Auto scale MC samples
if sample_info['type'].lower().find('mc') != -1:
    process.load.toMerge.autoscale = cms.bool(True)
    process.load.toMerge.totalExpectedEventsBeforeSkim = cms.uint32(
        sample_info['events_processed'])
    process.load.toMerge.skimEfficiency = cms.double(sample_info['skim_eff'])
    process.load.toMerge.xSection = cms.double(sample_info['x_sec'])
    process.load.toMerge.targetIntLumi = cms.double(sample_defs.TARGET_LUMI)
    process.load.toMerge.filterToUse = cms.string('genPhaseSpaceCut')
    process.load.toMerge.filterStatisticsLocation = cms.string(
        'ahMuTauAnalyzer_woBtag/FilterStatistics/')
else:
    process.load.toMerge.scaleFactor = cms.double(1.)


# If the sample is factorized, we need to change the location of the filter
if sample_info['factorize']:
    process.load.toMerge.filterStatisticsLocation = cms.string(
        'ahMuTauAnalyzer_woBtag_factorizedWithMuonIsolation/FilterStatistics/')

process.save = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string(outputFileName)
)

process.harvest = cms.Sequence(
    process.load
   + process.save
)

process.p = cms.Path(process.harvest)
