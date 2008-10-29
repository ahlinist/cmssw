#! /usr/bin/env cmsRun

import FWCore.ParameterSet.Config as cms

process = cms.Process("merge")

# keep the logging output to a nice level
process.load("FWCore.MessageLogger.MessageLogger_cfi")

# don't run on any events
process.source = cms.Source("EmptySource")
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(0)
)

# merge the histograms
process.load("RecoBTag.Analysis.bTagAnalysis_cfi")
process.bTagAnalysis.update    = True
process.bTagAnalysis.inputfile = 'merged.root'
process.bTagAnalysis.rootfile  = 'jetTagAnalysis.root'

# run the merge step
process.merge = cms.Path(process.bTagAnalysis)

