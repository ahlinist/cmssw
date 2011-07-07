#!/usr/bin/env python

import sys
import subprocess
import shlex
import time

print("<submitCopyToCastorJobs_grid>:")

samples = {
    ##'data2011A_tauPlusX_AOD'    : {
    ##    'dbsName' : "/TauPlusX/Run2011A-PromptReco-v1/AOD",
    ##    'dbsURL'  : "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
    ##    'type'    : "Data"
    ##},
    ##'data2011A_tauPlusX_RECO'   : {
    ##    'dbsName' : "/TauPlusX/Run2011A-PromptReco-v1/RECO",
    ##    'dbsURL'  : "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
    ##    'type'    : "Data"
    ##},
    'Ztautau_embedded_part1'   : {
        'dbsName' : "/SingleMu/fruboes-20110703_embed_goldenMuMuSkim_42_TauPlusX_Run2011A_PromptReco_v4-3d3970b15f2245948e2e9363d6dd6994/USER",
        'dbsURL'  :  "http://cmsdbsprod.cern.ch/cms_dbs_ph_analysis_01/servlet/DBSServlet",
        'type'    : "Data"
    },
    'Ztautau_embedded_part2'   : {
        'dbsName' : "/SingleMu/fruboes-20110703_embed_goldenMuMuSkim_42_muEra_SingleMu_Run2011A_May10ReReco_v1ex-3d3970b15f2245948e2e9363d6dd6994/USER",
        'dbsURL'  :  "http://cmsdbsprod.cern.ch/cms_dbs_ph_analysis_01/servlet/DBSServlet",
        'type'    : "Data"
    }
}

castorFilePath = "/castor/cern.ch/user/v/veelken/CMSSW_4_2_x/skims/ZtoMuTau/"
crabFilePath = "/afs/cern.ch/user/v/veelken/scratch0/CMSSW_4_2_4_patch1/src/TauAnalysis/Skimming/test/crab/"

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
process.GlobalTag.globaltag = cms.string('START42_V12::All')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'file:/data2/friis/CMSSW_4_2_X/skims/06-27-MatthewsZTTEvents/crab_0_110627_082505/ZTTCands_merged_v1.root'
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
dbs_url = DBS_URL
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
   crabConfig = crabConfig.replace("DBS_URL", sampleOptions['dbsURL'])
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
    
