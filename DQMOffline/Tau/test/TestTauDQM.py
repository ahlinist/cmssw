#!/usr/bin/env cmsRun
'''This script is made to test DQM Offline sequences for PFTau'''

import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing
import os
import sys

process = cms.Process("OurDQM")

process.source = cms.Source(
    "PoolSource",
    fileNames = cms.untracked.vstring(
        '/store/relval/CMSSW_5_2_5_cand1/RelValZTT/GEN-SIM-RECO/START52_V9-v1/0262/127870A8-E291-E111-8730-003048678AE2.root',
        '/store/relval/CMSSW_5_2_5_cand1/RelValZTT/GEN-SIM-RECO/START52_V9-v1/0261/DC675EEB-5791-E111-BFB3-00304867C0EA.root',
        '/store/relval/CMSSW_5_2_5_cand1/RelValZTT/GEN-SIM-RECO/START52_V9-v1/0261/0E350EFB-5791-E111-9A01-002618943809.root'
        )
    )


options = VarParsing.VarParsing()

options.register( 'maxEvents',
                   -1,
                   VarParsing.VarParsing.multiplicity.singleton,
                   VarParsing.VarParsing.varType.int,
                   "Specify events to run."
                )

options.register( 'rerunTaus',
                   False,
                   VarParsing.VarParsing.multiplicity.singleton,
                   VarParsing.VarParsing.varType.bool,
                   "rerun tauID."
                )

options.register( 'sequence',
                  "ALL",
                  VarParsing.VarParsing.multiplicity.singleton,
                  VarParsing.VarParsing.varType.string,
                  "Specify the sequence to be run.\
                        \n\t\tOptions:\
                        \n\t\t\tALL               (All DQMOffline Sequences)\
                        \n\t\t\tRealData          (Jets faking taus)\
                        \n\t\t\tRealMuonsData     (Iso Muons faking taus)\
                        \n\t\t\tRealTausData      (Real Taus from ZMuTau)\
                        \n\t\t\tRealElectronsData (Iso Electrons faking taus)\n"
                 )

options.register( 'step',
                  "DQMOffline+harvesting",
                  VarParsing.VarParsing.multiplicity.singleton,
                  VarParsing.VarParsing.varType.string,
                  "Specify step to be run.\
                        \n\t\tOptions:\
                        \n\t\t\tDQMOffline+harvesting  (full cycle)\
                        \n\t\t\tDQMOffline             (DQM Only)\n"
                 )

options.register( 'gtag',
                  None,
                  VarParsing.VarParsing.multiplicity.singleton,
                  VarParsing.VarParsing.varType.string,
                  "Specify the global tag."
                 )

# command options defined in Validation/RecoTau/python/ValidationOptions_cfi
options.parseArguments()

#Sets MaxEvents
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(options.maxEvents)
)

# DQM store, PDT sources etc
process.load("Configuration.StandardSequences.Services_cff")

fileName = 'TauDQM_'+options.sequence+'_'+options.step+'.root'

if os.path.exists(fileName):# and not options.gridJob:
   print "Output file %s already exists!  OK to overwrite?" % fileName
   while True:
      input = raw_input("Please enter [y/n] ")
      if (input == 'y'):
         break
      elif (input == 'n'):
         print " ...exiting."
         sys.exit()

process.schedule = cms.Schedule()

if options.rerunTaus:
    process.load("Configuration.StandardSequences.Geometry_cff")
    process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
    process.load("Configuration.StandardSequences.MagneticField_cff")
    process.load("RecoTauTag.Configuration.RecoPFTauTag_cff")
    process.runPFTau = cms.Path(process.PFTau)
    process.schedule.append(process.runPFTau)

# Specify conditions if desired
if options.gtag:
   process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
   process.GlobalTag.globaltag = options.gtag

process.load("DQMOffline.Tau.DQMSequences_cfi")

process.dqmTau = cms.Sequence()
if options.sequence != 'ALL':
    if options.step.find('DQMOffline') != -1:
        process.dqmTau *= getattr(process,'produceDenominator'+options.sequence)
        process.dqmTau *= getattr(process,'TauValNumeratorAndDenominator'+options.sequence)
    if options.step.find('harvesting') != -1:
        process.dqmTau *= getattr(process,'TauEfficiencies'+options.sequence) #gets the sequence with normalizer included
else:
    if options.step.find('DQMOffline') != -1:
        process.dqmTau *= process.produceDenoms
        process.dqmTau *= process.pfTauRunDQMValidation
    if options.step.find('harvesting') != -1:
        process.dqmTau *= process.runTauEff
    
process.saveTauEff = cms.EDAnalyzer("TauDQMSimpleFileSaver",
  outputFileName = cms.string(fileName)
)

process.dqmTau *= process.saveTauEff
process.dqmTauPath = cms.Path(process.dqmTau)

process.schedule.append(process.dqmTauPath)
