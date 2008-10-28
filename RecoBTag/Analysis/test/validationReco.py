#! /usr/bin/env cmsRun
import FWCore.ParameterSet.Config as cms

process = cms.Process("validation")

# keep the logging output to a nice level
process.load("FWCore.MessageLogger.MessageLogger_cfi")

# data
process.maxEvents = cms.untracked.PSet( 
    input = cms.untracked.int32(-1) 
)
process.source = cms.Source ("PoolSource",
    fileNames = cms.untracked.vstring (
    )
)

# default configuration with fake conditions
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.GlobalTag.globaltag = 'IDEAL_V9::All'

# validation plots
process.load("RecoBTag.Analysis.bTagAnalysis_cfi")

# run btag, then validation
process.btag  = cms.Path(process.btagging)
process.plots = cms.Path(process.bTagAnalysis)
  
process.schedule = cms.Schedule(process.btag, process.plots)
