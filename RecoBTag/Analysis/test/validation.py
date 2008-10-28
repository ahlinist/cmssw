#! /usr/bin/env cmsRun
import FWCore.ParameterSet.Config as cms

process = cms.Process("validation")

#keep the logging output to a nice level
process.load("FWCore.MessageLogger.MessageLogger_cfi")

# please note: this should not be needed; just a temporary hack
#process.load("RecoBTag.Configuration.RecoBTag_cff")

# data
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
    )
)

# validation plots
process.load("RecoBTag.Analysis.bTagAnalysis_cfi")

# run validation
process.plots = cms.Path(process.bTagAnalysis)
