#
# $Id: dump_DATA_42X.py,v 1.2 2011/05/22 12:57:08 meridian Exp $
#
#  configuration to dump ntuples in data
#   the only diff should be for jetmet corrections
#
import FWCore.ParameterSet.Config as cms
import sys
import os
import imp
import FWCore.ParameterSet.VarParsing as VarParsing

# contains all details of what to run and defines the process and the path
#from  JetMETCorrections.GammaJet.dumper_process_cff import process

# read in process from file
filename  = 'common_dump_config.py'
handle = open(filename, 'r')
cfo = imp.load_source("pycfg", filename, handle)
process = cfo.process
handle.close()

#Parsing options from command line
options = VarParsing.VarParsing ('analysis')

options.register(
    'diPhotonSkim', '',
    VarParsing.VarParsing.multiplicity.singleton,
    VarParsing.VarParsing.varType.int,
    "diPhotonSkim switch")
options.register(
    'diPhotonSkim2010', '',
    VarParsing.VarParsing.multiplicity.singleton,
    VarParsing.VarParsing.varType.int,
    "diPhotonSkim2010 switch")
options.register(
    'GlobalTag', '',
    VarParsing.VarParsing.multiplicity.singleton,
    VarParsing.VarParsing.varType.string,
    "GlobalTag name")

options.diPhotonSkim=0
options.diPhotonSkim2010=0
#default GT for 42 reprocessing
options.GlobalTag='GR_R_42_V19::All'

options.parseArguments()

if (options.diPhotonSkim==1):
    process.load('Configuration.Skimming.PDWG_DiPhoton_SD_cff')
    process.CaloIdIsoPath = cms.Path( process.CaloIdIsoPhotonPairsFilter * process.analysisSequence)
    process.R9IdPath = cms.Path( process.R9IdPhotonPairsFilter * process.analysisSequence)
    process.schedule = cms.Schedule(process.CaloIdIsoPath, process.R9IdPath)
elif (options.diPhotonSkim2010==1):
    process.load('Configuration.Skimming.PDWG_DiPhoton_SD_cff')
    process.CaloIdIsoPath = cms.Path( process.CaloIdIsoPhotonPairsFilter * process.analysisSequence)
    process.DiPhotonHltFilter.HLTPaths = ["HLT_DoublePhoton*"]
    process.hltDiPhotonCaloIdIsoObjectProducer.triggerName = cms.string("HLT_DoublePhoton.*")
    process.schedule = cms.Schedule(process.CaloIdIsoPath)
else:
    process.p = cms.Path(process.analysisSequence)

## DO NOT CHANGE THE PATH HERE! New modules should be added ONLY in the common configuration 
#  only paramaters should be changes for data and MC
process.source.fileNames = cms.untracked.vstring(
    'file:/cmshome/meridian/data/Photon_42_2010ReReco_AOD.root'
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10000)
)

# Global tag
# Remember to change it and adapt to your needs #
process.GlobalTag.globaltag = cms.string(options.GlobalTag)

##  apply only L2 and L3 jet corrections in MC
process.metMuonJESCorAK5.corrector = "ak5CaloL2L3Residual"

process.myanalysis.JetCorrectionService_akt5 = cms.string('ak5CaloL1L2L3Residual')
process.myanalysis.JetCorrectionService_akt7 = cms.string('ak7CaloL1L2L3Residual')
process.myanalysis.JetCorrectionService_jptak5 = cms.string('ak5JPTL1L2L3Residual')
process.myanalysis.JetCorrectionService_jptak7 = cms.string('ak7JPTL1L2L3Residual')
process.myanalysis.JetCorrectionService_pfakt5 = cms.string('ak5PFL1FastL2L3Residual')
process.myanalysis.JetCorrectionService_pfakt7 = cms.string('ak7PFL1FastL2L3Residual')


#print process.dumpPython()
