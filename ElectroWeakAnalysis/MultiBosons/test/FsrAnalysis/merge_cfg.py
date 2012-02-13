import FWCore.ParameterSet.VarParsing as VarParsing
import FWCore.ParameterSet.Config as cms

## Setup 'analysis'  options.
options = VarParsing.VarParsing ('analysis')

## Modify default option values here.
options.maxEvents = 10 # -1 means all events
options.outputFile = 'VGammaPAT_DimuonSkim'
options.inputFiles = [
    ('file:/mnt/tier2/store/user/veverka/DoubleMu/'
      'Run2011B-16Jan2012-v1_condor_Dimuon_AOD-42X-v10/'
      '21f287fe198824ddf494e2faf5b7ea65/VGammaPAT_DimuonSkim_1_1_ujn.root')
    ]

## Get and parse the command line arguments.
options.parseArguments()

process = cms.Process('MERGE')

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(False))
process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(options.maxEvents)
    )

process.source = cms.Source(
    "PoolSource",
    fileNames = cms.untracked.vstring() + options.inputFiles
    )

process.output = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string(options.outputFile),
    ## Max file size before starting new file, in kilobytes. Default: ~2TB.
    maxSize = cms.untracked.int32(1000000),
    dropMetaData = cms.untracked.string('ALL'),
    )

process.endpath = cms.EndPath(process.output)


