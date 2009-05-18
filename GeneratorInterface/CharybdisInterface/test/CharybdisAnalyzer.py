#!/usr/bin/env cmsRun
# -*- coding: utf-8 -*-
# Configuration file for example Charybdis produced BH event analyzer.

import FWCore.ParameterSet.Config as cms

process = cms.Process("TEST")
process.TFileService = cms.Service("TFileService", 
                                   fileName = cms.string("histos.root") ) # This is where the histograms go

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1) # Analyze all available events
    )

process.load("FWCore.MessageService.MessageLogger_cfi")

process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring('file:Charybdis.root') # ROOT file with BH events
                            )

process.analyzer = cms.EDAnalyzer("CharybdisAnalyzer")
process.p = cms.Path(process.analyzer)
