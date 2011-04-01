#!/usr/bin/env python

import sys
import subprocess
import shlex
import time

print("<submitCopyToCastorJobs_grid>:")

samples = {
    'A90'    : "/SUSYGluGluToHToTauTau_M-90_7TeV-pythia6-tauola/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO",
    'bbA90'  : "/SUSYBBHToTauTau_M-90_7TeV-pythia6-tauola/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO",
    'A100'   : "/SUSYGluGluToHToTauTau_M-100_7TeV-pythia6-tauola/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO",
    'bbA100' : "/SUSYBBHToTauTau_M-100_7TeV-pythia6-tauola/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO",
    'A120'   : "/SUSYGluGluToHToTauTau_M-120_7TeV-pythia6-tauola/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO",
    'bbA120' : "/SUSYBBHToTauTau_M-120_7TeV-pythia6-tauola/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO",
    'A130'   : "/SUSYGluGluToHToTauTau_M-130_7TeV-pythia6-tauola/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO",
    'bbA130' : "/SUSYBBHToTauTau_M-130_7TeV-pythia6-tauola/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO",
    'A140'   : "/SUSYGluGluToHToTauTau_M-1400_7TeV-pythia6-tauola/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO",
    'bbA140' : "/SUSYBBHToTauTau_M-140_7TeV-pythia6-tauola/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO",
    'A160'   : "/SUSYGluGluToHToTauTau_M-160_7TeV-pythia6-tauola/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO",
    'bbA160' : "/SUSYBBHToTauTau_M-160_7TeV-pythia6-tauola/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO",
    'A180'   : "/SUSYGluGluToHToTauTau_M-180_7TeV-pythia6-tauola/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO",
    'bbA180' : "/SUSYBBHToTauTau_M-180_7TeV-pythia6-tauola/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO",
    'A200'   : "/SUSYGluGluToHToTauTau_M-200_7TeV-pythia6-tauola/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO",
    'bbA200' : "/SUSYBBHToTauTau_M-200_7TeV-pythia6-tauola/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO",
    'A250'   : "/SUSYGluGluToHToTauTau_M-250_7TeV-pythia6-tauola/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO",
    'bbA250' : "/SUSYBBHToTauTau_M-250_7TeV-pythia6-tauola/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO",
    'A300'   : "/SUSYGluGluToHToTauTau_M-300_7TeV-pythia6-tauola/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO",
    'bbA300' : "/SUSYBBHToTauTau_M-300_7TeV-pythia6-tauola/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO",
    'A350'   : "/SUSYGluGluToHToTauTau_M-350_7TeV-pythia6-tauola/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO",
    'bbA350' : "/SUSYBBHToTauTau_M-350_7TeV-pythia6-tauola/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO"
}

castorFilePath = "/castor/cern.ch/user/v/veelken/CMSSW_3_8_x/skims/AHtoMuTau/"
crabFilePath = "/afs/cern.ch/user/v/veelken/scratch0/CMSSW_3_8_7/src/TauAnalysis/Skimming/test/crab/"

cfg_template = """
import FWCore.ParameterSet.Config as cms

process = cms.Process("copyToCastor")

from TauAnalysis.Skimming.EventContent_cff import *

# import of standard configurations for RECOnstruction
# of electrons, muons and tau-jets with non-standard isolation cones
process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 5000
#process.MessageLogger.cerr.threshold = cms.untracked.string('INFO')
process.load('Configuration/StandardSequences/GeometryIdeal_cff')
process.load('Configuration/StandardSequences/MagneticField_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = cms.string('START38_V14::All')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(20)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'rfio:/castor/cern.ch/user/v/veelken/CMSSW_3_6_x/skims/ZtoMuTau/test/Ztautau_1_1_sXK.root'
    )                          
)

dummyEventSelection = cms.untracked.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('')
    )
)

process.copyToCastorOutputModule = cms.OutputModule("PoolOutputModule",                                              
    fileName = cms.untracked.string('copyToCastor.root')
)

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

process.o = cms.EndPath(process.copyToCastorOutputModule)
"""

crab_template = """
[CRAB]

jobtype = cmssw
scheduler = glidein
use_server = 1

[CMSSW]

datasetpath = DATASETPATH
pset = PSET
output_file = OUTPUT_FILE
total_number_of_events = -1
events_per_job = 10000

[USER]

ui_working_dir = UI_WORKING_DIR
return_data = 0
copy_data = 1
storage_element = srm-cms.cern.ch
storage_path = /srm/managerv2?SFN=/castor/cern.ch
user_remote_dir = USER_REMOTE_DIR
"""

def executeCommand(commandLine):
     print(commandLine)
     #args = shlex.split(commandLine)
     #retval = subprocess.Popen(args, stdout = subprocess.PIPE)
     #retval.wait()	
     #subprocess.call(commandLine, shell = True)

for sampleName, sampleDatasetPath in samples.items():
   print("submitting copyToCastor job for sample = %s" % sampleName)

   rootFileName = "%s.root" % sampleName

   cfgFileName = (crabFilePath + "crabCopyToCastor_%s.cfg") % sampleName
   cfgFile = open(cfgFileName, "w")
   cfgFile.write(cfg_template)
   cfgFile.write("process.copyToCastorOutputModule.fileName = cms.untracked.string('%s')" % rootFileName)

   crabFileName = (crabFilePath + "crab_%s.cfg") % sampleName
   crabConfig = str(crab_template)
   crabConfig = crabConfig.replace("DATASETPATH", sampleDatasetPath)
   crabConfig = crabConfig.replace("PSET", cfgFileName)
   crabConfig = crabConfig.replace("OUTPUT_FILE", rootFileName)
   ui_working_dir = (crabFilePath + "crabdirCopyToCastor_%s") % sampleName
   crabConfig = crabConfig.replace("UI_WORKING_DIR", ui_working_dir)
   user_remote_dir = castorFilePath + sampleName
   crabConfig = crabConfig.replace("USER_REMOTE_DIR", user_remote_dir.replace("/castor/cern.ch", ""))
   crabFile = open(crabFileName, "w")
   crabFile.write(crabConfig)
   crabFile.close()

   # need to wait until config files have finished writing...
   time.sleep(1)

   subprocess.call("rfmkdir %s" % user_remote_dir, shell = True)
   subprocess.call("rfchmod 777 %s" % user_remote_dir, shell = True)

   executeCommand("crab -create -cfg %s" % crabFileName)
   #time.sleep(30)
   executeCommand("crab -submit -c %s" % ui_working_dir)
   #time.sleep(30)
