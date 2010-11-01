#!/usr/bin/env python

#--------------------------------------------------------------------------------
# Merge all EDM files stored in castor directory
# given as first function argument into single output file,
# the name of which needs to be given as second function argument
#
#  e.g. ./mergeEDMfiles.py
#          "/castor/cern.ch/user/v/veelken/CMSSW_3_8_x/skims/AHtoMuTau/selEvents/*runs132440to145761*"
#          /data1/veelken/CMSSW_3_8_x/skims/AHtoMuTau/selEvents_AHtoMuTau_woBtag_runs145762to148058_RECO.root
#
# Author: Christian Veelken, UC Davis
#
#--------------------------------------------------------------------------------

cfgFileName = "mergeEDMfiles_cfg.py"

import sys
import subprocess

import TauAnalysis.Configuration.tools.castor as castor

inputFilePath = sys.argv[1]
outputFileName = sys.argv[2]

cfg = """
import FWCore.ParameterSet.Config as cms

process = cms.Process("mergeEDMfiles")

process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 5000
process.load('Configuration/StandardSequences/GeometryIdeal_cff')
process.load('Configuration/StandardSequences/MagneticField_cff')
process.load('Configuration/StandardSequences/Reconstruction_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = cms.string('START38_V12::All')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.source = cms.Source("PoolSource",
    #fileNames = cms.untracked.vstring()                          
)

process.outputModule = cms.OutputModule("PoolOutputModule",                                              
    #fileName = cms.untracked.string()
)

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

process.o = cms.EndPath(process.outputModule)
"""

print("reading input files...")
inputFileNames = castor.nsls(inputFilePath)
#print("inputFileNames:")
#print(inputFileNames)

isFirst = True
for inputFileName in inputFileNames:
    castorInputFileName = "rfio:%s" % inputFileName
    if isFirst:
        cfg += "\nprocess.source.fileNames = cms.untracked.vstring('%s')" % castorInputFileName
        isFirst = False
    else:
        cfg += "\nprocess.source.fileNames.extend(cms.untracked.vstring('%s'))" % castorInputFileName
cfg += "\nprocess.outputModule.fileName = cms.untracked.string('%s')" % outputFileName

# write config file
print("preparing config file...")
cfgFile = open(cfgFileName, "w")
cfgFile.write(cfg)
cfgFile.close()    

# start cmsRun executable
print("starting cmsRun executable...")
subprocess.call("cmsRun %s" % cfgFileName, shell = True)
subprocess.call("rm %s" % cfgFileName, shell = True)
