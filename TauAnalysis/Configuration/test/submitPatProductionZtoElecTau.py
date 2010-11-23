#!/usr/bin/env python

from TauAnalysis.Configuration.submitToBatch import submitToBatch
from TauAnalysis.Configuration.makeReplacementsPatProduction import makeReplacementsPatProduction

# name of the directory (either on afs area or castor)
# to which all .root files produced by the cmsRun job will be copied
outputFilePath = "/castor/cern.ch/user/j/jkolb/elecTauPatTuples/fall10/current/"

# small cmsRun job for testing purposes...
#submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau", sample = "Ztautau_part01",
#              replFunction = makeReplacementsPatProduction, replacements = "maxEvents = 100",
#              job = "PatProduction", queue = "1nh", outputFilePath = outputFilePath, type = "mc")

#--------------------------------------------------------------------------------
#
# Monte Carlo samples from fall 2010 production
# processed with CMSSW_38_x, skimmed by Jeff Kolb
#
# NOTE: The jobs get submitted to the '1nd' queue,
#       which allows for an execution time of the cmsRun jobs of up to 24 hours
#       (the queues are {'1nh' (1 hour), '1nd' (24 hours) and '1nw' (1 week execution time limit);
#        see https://twiki.cern.ch/twiki/bin/view/CMS/CMSUKCMSSWBatch for details about the CERN batch system)           
#
#--------------------------------------------------------------------------------

#
# 7TeV data samples
#

for i in range(402):
	submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau", 
		sample = "Data2010A_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsPatProduction, replacements = "maxEvents = -1; globalTag = GR_R_38X_V14::All",
		job = "PatProduction", queue = "1nd", outputFilePath = outputFilePath, type = "data")

for i in range(417,1097):
	submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau", 
		sample = "Data2010B_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsPatProduction, replacements = "maxEvents = -1; globalTag = GR10_P_V10::All",
		job = "PatProduction", queue = "1nd", outputFilePath = outputFilePath, type = "data")

#
# 7TeV MC samples
#

# Z --> tau tau jobs
for i in range(21):
	submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau", 
		sample = "Ztautau_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsPatProduction, replacements = "maxEvents = -1; globalTag = START38_V13::All",
		job = "PatProduction", queue = "1nd", outputFilePath = outputFilePath, type = "mc")

# Z --> e e jobs
for i in range(81):
	submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau", 
		sample = "Zee_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsPatProduction, replacements = "maxEvents = -1; globalTag = START38_V12::All",
		job = "PatProduction", queue = "1nd", outputFilePath = outputFilePath, type = "mc")

# Photon + jets jobs
for i in range(3):
	submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau", 
		sample = "PhotonPlusJets_Pt15to30_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsPatProduction, replacements = "maxEvents = -1; globalTag = START38_V12::All",
		job = "PatProduction", queue = "8nh", outputFilePath = outputFilePath, type = "mc")
for i in range(4):
	submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau", 
		sample = "PhotonPlusJets_Pt30to50_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsPatProduction, replacements = "maxEvents = -1; globalTag = START38_V12::All",
		job = "PatProduction", queue = "8nh", outputFilePath = outputFilePath, type = "mc")
for i in range(4):
	submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau", 
		sample = "PhotonPlusJets_Pt50to80_part%(i)02d" % {"i" : (i + 1)}, 
		replFunction = makeReplacementsPatProduction, replacements = "maxEvents = -1; globalTag = START38_V12::All",
		job = "PatProduction", queue = "8nh", outputFilePath = outputFilePath, type = "mc")

#for i in range(10):
#	submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau",
#		sample = "PhotonPlusJets_Pt30_part%(i)02d" % {"i" : (i + 1)},
#		replFunction = makeReplacementsPatProduction, replacements = "maxEvents = -1; globalTag = START38_V12::All",
#		job = "PatProduction", queue = "1nd", outputFilePath = outputFilePath, type = "mc")

# QCD_BCtoE jobs
for i in range(101):
	submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau",
		sample = "QCD_BCtoE_Pt20to30_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsPatProduction, replacements = "maxEvents = -1; globalTag = START38_V12::All",
		job = "PatProduction", queue = "1nd", outputFilePath = outputFilePath, type = "mc")
for i in range(201):
	submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau",
		sample = "QCD_BCtoE_Pt30to80_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsPatProduction, replacements = "maxEvents = -1; globalTag = START38_V12::All",
		job = "PatProduction", queue = "1nd", outputFilePath = outputFilePath, type = "mc")
for i in range(206):
		submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau",
		sample = "QCD_BCtoE_Pt80to170_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsPatProduction, replacements = "maxEvents = -1; globalTag = START38_V12::All",
		job = "PatProduction", queue = "1nd", outputFilePath = outputFilePath, type = "mc")    

# QCD_EMenriched jobs
for i in range(226):
	submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau",
		sample = "QCD_EM_Pt20to30_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsPatProduction, replacements = "maxEvents = -1; globalTag = START38_V12::All",
		job = "PatProduction", queue = "1nd", outputFilePath = outputFilePath, type = "mc")
for i in range(479):    
	submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau",
		sample = "QCD_EM_Pt30to80_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsPatProduction, replacements = "maxEvents = -1; globalTag = START38_V12::All",
		job = "PatProduction", queue = "1nd", outputFilePath = outputFilePath, type = "mc")
for i in range(201):    
	submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau",
		sample = "QCD_EM_Pt80to170_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsPatProduction, replacements = "maxEvents = -1; globalTag = START38_V12::All",
		job = "PatProduction", queue = "1nd", outputFilePath = outputFilePath, type = "mc")    

# W jobs
for i in range(80):
	submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau", 
		sample = "WtoENu_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsPatProduction, replacements = "maxEvents = -1; globalTag = START38_V12::All",
		job = "PatProduction", queue = "1nd", outputFilePath = outputFilePath, type = "mc")

for i in range(32):
	submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau", 
		sample = "WtoTauNu_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsPatProduction, replacements = "maxEvents = -1; globalTag = START38_V12::All",
		job = "PatProduction", queue = "1nd", outputFilePath = outputFilePath, type = "mc")

# TT + jets jobs
for i in range(55):
	submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau", 
		sample = "TTbar_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsPatProduction, replacements = "maxEvents = -1; globalTag = START38_V12::All",
		job = "PatProduction", queue = "1nd", outputFilePath = outputFilePath, type = "mc")

