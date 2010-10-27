#!/usr/bin/env python

from TauAnalysis.Configuration.submitToBatch import submitToBatch
from TauAnalysis.Configuration.makeReplacementsPatProduction import makeReplacementsPatProduction

# name of the directory (either on afs area or castor)
# to which all .root files produced by the cmsRun job will be copied
outputFilePath = "/castor/cern.ch/user/j/jkolb/elecTauPatTuples/spring10/current/"

# small cmsRun job for testing purposes...
#submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau", sample = "Ztautau_part01",
#              replFunction = makeReplacementsPatProduction, replacements = "maxEvents = 100",
#              job = "PatProduction", queue = "1nh", outputFilePath = outputFilePath, type = "mc")

#--------------------------------------------------------------------------------
#
# Monte Carlo samples from Summer'09 production
# reprocessed with CMSSW_3_1_4, skimmed by Jeff Kolb
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

for i in range(417):
	submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau", 
		sample = "Data2010A_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsPatProduction, replacements = "maxEvents = -1; globalTag = GR_R_36X_V12B::All",
		job = "PatProduction", queue = "1nd", outputFilePath = outputFilePath, type = "data")

for i in range(417,959):
	submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau", 
		sample = "Data2010B_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsPatProduction, replacements = "maxEvents = -1; globalTag = GR10_P_V10::All",
		job = "PatProduction", queue = "1nd", outputFilePath = outputFilePath, type = "data")

#
# 7TeV MC samples
#

# Z --> tau tau jobs
for i in range(18):
	submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau", 
		sample = "Ztautau_7TeV_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsPatProduction, replacements = "maxEvents = -1; globalTag = START3X_V27::All",
		job = "PatProduction", queue = "1nd", outputFilePath = outputFilePath, type = "mc")

# Z --> e e jobs
for i in range(44):
	submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau", 
		sample = "Zee_7TeV_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsPatProduction, replacements = "maxEvents = -1; globalTag = START3X_V27::All",
		job = "PatProduction", queue = "1nd", outputFilePath = outputFilePath, type = "mc")

# Photon + jets jobs
submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau", sample = "PhotonPlusJets_Pt15to20",
	replFunction = makeReplacementsPatProduction, replacements = "maxEvents = -1; globalTag = START38_V12::All",
	job = "PatProduction", queue = "8nh", outputFilePath = outputFilePath, type = "mc")
submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau", sample = "PhotonPlusJets_Pt20to30",
	replFunction = makeReplacementsPatProduction, replacements = "maxEvents = -1; globalTag = START38_V12::All",
	job = "PatProduction", queue = "8nh", outputFilePath = outputFilePath, type = "mc")
submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau", sample = "PhotonPlusJets_Pt30to50",
	replFunction = makeReplacementsPatProduction, replacements = "maxEvents = -1; globalTag = START38_V12::All",
	job = "PatProduction", queue = "8nh", outputFilePath = outputFilePath, type = "mc")
submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau", sample = "PhotonPlusJets_Pt50to80",
	replFunction = makeReplacementsPatProduction, replacements = "maxEvents = -1; globalTag = START38_V12::All",
	job = "PatProduction", queue = "8nh", outputFilePath = outputFilePath, type = "mc")
submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau", sample = "PhotonPlusJets_Pt80to120",
	replFunction = makeReplacementsPatProduction, replacements = "maxEvents = -1; globalTag = START38_V12::All",
	job = "PatProduction", queue = "8nh", outputFilePath = outputFilePath, type = "mc")
for i in range(10):
	submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau",
		sample = "PhotonPlusJets_Pt30_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsPatProduction, replacements = "maxEvents = -1; globalTag = START38_V12::All",
		job = "PatProduction", queue = "1nd", outputFilePath = outputFilePath, type = "mc")

# QCD_BCtoE jobs
for i in range(70):
	submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau",
		sample = "QCD_BCtoE_Pt20to30_7TeV_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsPatProduction, replacements = "maxEvents = -1; globalTag = START3X_V27::All",
		job = "PatProduction", queue = "1nd", outputFilePath = outputFilePath, type = "mc")
for i in range(46):
	submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau",
		sample = "QCD_BCtoE_Pt30to80_7TeV_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsPatProduction, replacements = "maxEvents = -1; globalTag = START3X_V27::All",
		job = "PatProduction", queue = "1nd", outputFilePath = outputFilePath, type = "mc")
for i in range(35):
		submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau",
		sample = "QCD_BCtoE_Pt80to170_7TeV_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsPatProduction, replacements = "maxEvents = -1; globalTag = START3X_V27::All",
		job = "PatProduction", queue = "1nd", outputFilePath = outputFilePath, type = "mc")    

# QCD_EMenriched jobs
for i in range(174):
	submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau",
		sample = "QCD_EMenriched_Pt20to30_7TeV_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsPatProduction, replacements = "maxEvents = -1; globalTag = START3X_V27::All",
		job = "PatProduction", queue = "1nd", outputFilePath = outputFilePath, type = "mc")
for i in range(266):    
	submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau",
	sample = "QCD_EMenriched_Pt30to80_7TeV_part%(i)02d" % {"i" : (i + 1)},
	replFunction = makeReplacementsPatProduction, replacements = "maxEvents = -1; globalTag = START3X_V27::All",
	job = "PatProduction", queue = "1nd", outputFilePath = outputFilePath, type = "mc")
for i in range(105):    
	submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau",
		sample = "QCD_EMenriched_Pt80to170_7TeV_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsPatProduction, replacements = "maxEvents = -1; globalTag = START3X_V27::All",
		job = "PatProduction", queue = "1nd", outputFilePath = outputFilePath, type = "mc")    

# W/Z + jets jobs
for i in range(82):
	submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau", 
		sample = "WplusJets_7TeV_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsPatProduction, replacements = "maxEvents = -1; globalTag = START3X_V27::All",
		job = "PatProduction", queue = "1nd", outputFilePath = outputFilePath, type = "mc")

for i in range(56):
	submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau", 
		sample = "ZeePlusJets_7TeV_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsPatProduction, replacements = "maxEvents = -1; globalTag = START3X_V27::All",
		job = "PatProduction", queue = "1nd", outputFilePath = outputFilePath, type = "mc")
	submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau", 
		sample = "ZtautauPlusJets_7TeV_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsPatProduction, replacements = "maxEvents = -1; globalTag = START3X_V27::All",
		job = "PatProduction", queue = "1nd", outputFilePath = outputFilePath, type = "mc")

# TT + jets jobs
for i in range(56):
	submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau", 
		sample = "TTplusJets_7TeV_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsPatProduction, replacements = "maxEvents = -1; globalTag = START3X_V27::All",
		job = "PatProduction", queue = "1nd", outputFilePath = outputFilePath, type = "mc")

