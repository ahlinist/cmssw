#!/usr/bin/env python

import sys
import subprocess
import shlex
import time

print("<submitCopyToCastorJobs_grid>:")

samples = {
    ##'data2011A_tauPlusX_AOD'    : {
    ##    'dbsName' : "/TauPlusX/Run2011A-PromptReco-v1/AOD",
    ##    'type'    : "Data"
    ##},
    ##'data2011A_tauPlusX_RECO'   : {
    ##    'dbsName' : "/TauPlusX/Run2011A-PromptReco-v1/RECO",
    ##    'type'    : "Data"
    ##},
    ##'DYtautau_spring11_powhegZ2' : {
    ##    'dbsName' : "/DYToTauTau_M-20_CT10_TuneZ2_7TeV-powheg-pythia-tauola/Spring11-PU_S1_START311_V1G1-v1/GEN-SIM-RECO",
    ##    'type'    : "MC"
    ##},
    ##'DYtautau_spring11_pythiaZ2debug' : {
    ##    'dbsName' : "/DYToTauTau_M-20_TuneZ2_7TeV-pythia6-tauola/Spring11-PU_S1_START311_V1G1-v1/GEN-SIM-RECODEBUG",
    ##    'type'    : "MC"
    ##},
    ##'DYtautau_spring11flatPU10_pythiaZ2debug' : {
    ##    'dbsName' : "/DYToTauTau_M-20_TuneZ2_7TeV-pythia6-tauola/Spring11-E7TeV_FlatDist10_2011EarlyData_50ns_START311_V1G1-v1/GEN-SIM-RECODEBUG",
    ##    'type'    : "MC"
    ##}
    ##'DYtautau_powhegZ2_aodsim' : {
    ##    'dbsName' : "/DYToTauTau_M-10To20_CT10_TuneZ2_7TeV-powheg-pythia-tauola/Spring11-PU_S1_START311_V1G1-v2/AODSIM",
    ##    'type'    : "MC"
    ##},
    ##'WplusJets_madgraph_aodsim' : {
    ##    'dbsName' : "/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Spring11-PU_S1_START311_V1G1-v1/AODSIM",
    ##    'type'    : "MC"
    ##},
    ##'PPmuXptGt20Mu15_aodsim' : {
    ##    'dbsName' : "/QCD_Pt-20_MuEnrichedPt-15_TuneZ2_7TeV-pythia6/Spring11-PU_S1_START311_V1G1-v1/AODSIM",
    ##    'type'    : "MC"
    ##}
    'Ztautau_winter10'  : {
        'dbsName' : "/DYToTauTau_M-20_TuneZ2_7TeV-pythia6-tauola/Winter10-E7TeV_ProbDist_2010Data_BX156_START39_V8-v1/AODSIM",
        'type'    : "MC"
    }
}

castorFilePath = "/castor/cern.ch/user/v/veelken/CMSSW_4_1_x/skims/ZtoMuTau/"
crabFilePath = "/afs/cern.ch/user/v/veelken/scratch0/CMSSW_4_1_3/src/TauAnalysis/Skimming/test/crab/"

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
process.GlobalTag.globaltag = cms.string('START311_V2::All')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
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
scheduler = glite
use_server = 1

[CMSSW]

datasetpath = DATASETPATH
pset = PSET
output_file = OUTPUT_FILE
JOB_SPLIT_METHOD
number_of_jobs = 1

[USER]

ui_working_dir = UI_WORKING_DIR
return_data = 0
copy_data = 1
storage_element = srm-cms.cern.ch
storage_path = /srm/managerv2?SFN=/castor/cern.ch
user_remote_dir = USER_REMOTE_DIR

[GRID]

se_black_list = T2_US_Nebraska
"""

shellCommands = []

for sampleName, sampleOptions in samples.items():
   print("submitting copyToCastor job for sample = %s" % sampleName)

   rootFileName = "%s.root" % sampleName

   cfgFileName = (crabFilePath + "crabCopyToCastor_%s_cfg.py") % sampleName
   cfgFile = open(cfgFileName, "w")
   cfgFile.write(cfg_template)
   cfgFile.write("process.copyToCastorOutputModule.fileName = cms.untracked.string('%s')" % rootFileName)

   crabFileName = (crabFilePath + "crab_%s.cfg") % sampleName
   crabConfig = str(crab_template)
   crabConfig = crabConfig.replace("DATASETPATH", sampleOptions['dbsName'])
   crabConfig = crabConfig.replace("PSET", cfgFileName)
   if sampleOptions['type'] == "Data":
       crabConfig = crabConfig.replace("JOB_SPLIT_METHOD", "lumis_per_job = 5")
   elif sampleOptions['type'] == "MC":
       crabConfig = crabConfig.replace("JOB_SPLIT_METHOD", "total_number_of_events = 1000")
   else:
       raise ValueError("Undefined sample type = %s !!" %sampleOptions['type'])
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

   shellCommands.append("crab -create -cfg %s" % crabFileName)
   shellCommands.append("crab -submit -c %s" % ui_working_dir)

shellFileName = "submitCopyToCastorJobs_grid.csh"
shellFile = open(shellFileName, "w")
shellFile.write("#!/bin/csh -f\n")
shellFile.write("\n")

for shellCommdand in shellCommands:
    shellFile.write(shellCommdand + '\n')

shellFile.close()

subprocess.call("chmod +x %s" % shellFileName, shell = True)

# need to wait until config files have finished writing...
time.sleep(1)

print("Finished building config files. Now execute 'source %s'." % shellFileName)
#subprocess.call("source %s" % shellFileName, shell = True)
    
