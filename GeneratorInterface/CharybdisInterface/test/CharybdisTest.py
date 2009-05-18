#!/usr/bin/env cmsRun
# -*- coding: utf-8 -*-
# Example configuration file for generating BH events with Charybdis

import FWCore.ParameterSet.Config as cms
from Configuration.GenProduction.PythiaUESettings_cfi import * 

process = cms.Process("Gen")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.destinations = cms.untracked.vstring("cout")
process.MessageLogger.cout = cms.untracked.PSet(threshold = cms.untracked.string("INFO"))


process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
    generator = cms.PSet(
        initialSeed = cms.untracked.uint32(123456789),
        engineName = cms.untracked.string('HepJamesRandom')
    )
)

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(10))

process.source = cms.Source("EmptySource")
process.generator = cms.EDProducer("CharybdisProducer",
        maxEventsToPrint = cms.untracked.uint32(0),
        pythiaPylistVerbosity = cms.untracked.uint32(1),
        comEnergy = cms.double(10000.0),
        allowEmptyEvents = cms.bool(False),
    
        # Charybdis parameter defaults
        MPLNCK = cms.double(1000.0),
        MINMSS = cms.double(5000.0),
        MAXMASS = cms.double(-1), #(Default  -1: => sqrt(S)) ",
        THWMAX = cms.double(1000.0), #Maximum Hawking temperature (GeV)"
        RMMINM = cms.double(1000.0), #Min BH mass, ends boiling (GeV)"
        Nsearch = cms.int32(10000),
        MSSDEF = cms.int32(2), 
        TOTDIM = cms.int32(7), # = 4+n, where n is the number of extra dimensions
        NBODY = cms.int32(2), 
        MSSDEC = cms.int32(3), #BH decays to heavy particles?
        GTSCA = cms.bool(False), 
        TIMVAR = cms.bool(False), 
        GRYBDY = cms.bool(False), 
        KINCUT = cms.bool(False), 
        YRCSEC = cms.bool(False), #Use Yoshino-Rychkov factors in cross-section?",
        RMBOIL = cms.bool(False), #Remnant decay model: boiling at maximum Hawking temperature"
    
        # pythia6 settings
        PythiaParameters = cms.PSet(
	    pythiaUESettingsBlock,
            parameterSets = cms.vstring('pythiaUESettings') #Information: http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/CMSSW/Configuration/GenProduction/python/PythiaUESettings_cfi.py?revision=1.3&view=markup&pathrev=CMSSW_3_1_0_pre7
            )

    )

process.load("Configuration.EventContent.EventContent_cff")

process.GEN = cms.OutputModule("PoolOutputModule",
    process.FEVTSIMEventContent,
    dataset = cms.untracked.PSet(dataTier = cms.untracked.string('GEN')),
    fileName = cms.untracked.string('Charybdis.root')
    )

configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.1 $'),
    name = cms.untracked.string('$Source: /cvs_server/repositories/CMSSW/CMSSW/GeneratorInterface/CharybdisInterface/test/CharybdisTest.py,v $'),
    annotation = cms.untracked.string('Charybdis test cfg file for sqrt(s)=10 TeV')
    )

process.p = cms.Path(process.generator)
process.outpath = cms.EndPath(process.GEN)
process.schedule = cms.Schedule(process.p,process.outpath)
