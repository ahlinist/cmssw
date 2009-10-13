#!/usr/bin/env python

from TauAnalysis.Configuration.submitToBatch import submitToBatch
from TauAnalysis.Configuration.makeReplacementsAnalysis import makeReplacementsAnalysis

# name of the directory (either on afs area or castor)
# to which all .root files produced by the cmsRun job will be copied
outputDirectory = "/castor/cern.ch/user/j/jkolb/elecTauAnalysis/hists/"
inputFilePath = "/castor/cern.ch/user/j/jkolb/elecTauAnalysis/patTuples/"

# small cmsRun job for testing purposes...
#submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau", sample = "ZtoElecTau_Ztautau_part01",
#              replFunction = makeReplacementsAnalysis, replacements = "maxEvents = 100; applyFactorization = false",
#              job = "analysis", queue = "1nh", outputDirectory = outputDirectory)

#--------------------------------------------------------------------------------
#
# Monte Carlo samples from Summer'08 production
# reprocessed with CMSSW_2_2_3, skimmed by Jeff Kolb
#
# NOTE: The jobs get submitted to the '8nh' queue,
#       which allows for an execution time of the cmsRun jobs of up to 24 hours
#       (the queues are {'1nh' (1 hour), '8nh' (24 hours) and '1nw' (1 week execution time limit);
#        see https://twiki.cern.ch/twiki/bin/view/CMS/CMSUKCMSSWBatch for details about the CERN batch system)           
#
#--------------------------------------------------------------------------------

# Z --> tau tau jobs
for i in range(10):
	submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau", sample = "ZtoElecTau_Ztautau_part%(i)02d" % {"i" : (i + 1)}, 
		replFunction = makeReplacementsAnalysis, 
		replacements = "maxEvents = -1; inputFilePath = " + inputFilePath + "/; applyFactorization = false",
		job = "analysis", queue = "8nh", outputDirectory = outputDirectory)

# Z --> e e jobs
for i in range(24):
	submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau", sample = "ZtoElecTau_Zee_part%(i)02d" % {"i" : (i + 1)}, 
		replFunction = makeReplacementsAnalysis, 
		replacements = "maxEvents = -1; inputFilePath = " + inputFilePath + "/; applyFactorization = false",
		job = "analysis", queue = "8nh", outputDirectory = outputDirectory)

# Photon + jets jobs
submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau", sample = "ZtoElecTau_PhotonJets_Pt15to20", replFunction = makeReplacementsAnalysis, 
	replacements = "maxEvents = -1; inputFilePath = " + inputFilePath + "/; applyFactorization = true",
	job = "analysis", queue = "8nh", outputDirectory = outputDirectory)
submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau", sample = "ZtoElecTau_PhotonJets_Pt20to25", replFunction = makeReplacementsAnalysis, 
	replacements = "maxEvents = -1; inputFilePath = " + inputFilePath + "/; applyFactorization = true",
	job = "analysis", queue = "8nh", outputDirectory = outputDirectory)
submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau", sample = "ZtoElecTau_PhotonJets_Pt25to30", replFunction = makeReplacementsAnalysis, 
	replacements = "maxEvents = -1; inputFilePath = " + inputFilePath + "/; applyFactorization = true",
	job = "analysis", queue = "8nh", outputDirectory = outputDirectory)
submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau", sample = "ZtoElecTau_PhotonJets_Pt30to35", replFunction = makeReplacementsAnalysis, 
	replacements = "maxEvents = -1; inputFilePath = " + inputFilePath + "/; applyFactorization = true",
	job = "analysis", queue = "8nh", outputDirectory = outputDirectory)
submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau", sample = "ZtoElecTau_PhotonJets_PtGt35", replFunction = makeReplacementsAnalysis, 
	replacements = "maxEvents = -1; inputFilePath = " + inputFilePath + "/; applyFactorization = true",
	job = "analysis", queue = "8nh", outputDirectory = outputDirectory)

# QCD_BCtoE jobs
for i in range(28):
	submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau", 
		sample = "ZtoElecTau_QCD_BCtoE_Pt20to30_part%(i)02d" % {"i" : (i + 1)}, replFunction = makeReplacementsAnalysis, 
		replacements = "maxEvents = -1; inputFilePath = " + inputFilePath + "/; applyFactorization = true",
		job = "analysis", queue = "8nh", outputDirectory = outputDirectory)

for i in range(46):
	submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau",
		sample = "ZtoElecTau_QCD_BCtoE_Pt30to80_part%(i)02d" % {"i" : (i + 1)}, replFunction = makeReplacementsAnalysis, 
		replacements = "maxEvents = -1; inputFilePath = " + inputFilePath + "/; applyFactorization = true",
		job = "analysis", queue = "8nh", outputDirectory = outputDirectory)

for i in range(26):
	submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau",
		sample = "ZtoElecTau_QCD_BCtoE_Pt80to170_part%(i)02d" % {"i" : (i + 1)}, replFunction = makeReplacementsAnalysis, 
		replacements = "maxEvents = -1; inputFilePath = " + inputFilePath + "/; applyFactorization = true",
		job = "analysis", queue = "8nh", outputDirectory = outputDirectory)    

# QCD_EMenriched jobs
for i in range(53):
	submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau",
		sample = "ZtoElecTau_QCD_EMenriched_Pt20to30_part%(i)02d" % {"i" : (i + 1)}, replFunction = makeReplacementsAnalysis, 
		replacements = "maxEvents = -1; inputFilePath = " + inputFilePath + "/; applyFactorization = true",
		job = "analysis", queue = "8nh", outputDirectory = outputDirectory)

for i in range(220):    
	submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau",
		sample = "ZtoElecTau_QCD_EMenriched_Pt30to80_part%(i)02d" % {"i" : (i + 1)}, replFunction = makeReplacementsAnalysis, 
		replacements = "maxEvents = -1; inputFilePath = " + inputFilePath + "/; applyFactorization = true",
		job = "analysis", queue = "8nh", outputDirectory = outputDirectory)

for i in range(59):    
	submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau",
		sample = "ZtoElecTau_QCD_EMenriched_Pt80to170_part%(i)02d" % {"i" : (i + 1)}, replFunction = makeReplacementsAnalysis, 
		replacements = "maxEvents = -1; inputFilePath = " + inputFilePath + "/; applyFactorization = true",
		job = "analysis", queue = "8nh", outputDirectory = outputDirectory)    

# W/Z + jets jobs
for i in range(34):
	submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau", 
		sample = "ZtoElecTau_WplusJets_part%(i)02d" % {"i" : (i + 1)}, replFunction = makeReplacementsAnalysis, 
		replacements = "maxEvents = -1; inputFilePath = " + inputFilePath + "/; applyFactorization = false",
		job = "analysis", queue = "8nh", outputDirectory = outputDirectory)

for i in range(16):
	submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau", 
		sample = "ZtoElecTau_ZeePlusJets_part%(i)02d" % {"i" : (i + 1)}, replFunction = makeReplacementsAnalysis, 
		replacements = "maxEvents = -1; inputFilePath = " + inputFilePath + "/; applyFactorization = false",
		job = "analysis", queue = "8nh", outputDirectory = outputDirectory)
	submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau", 
		sample = "ZtoElecTau_ZtautauPlusJets_part%(i)02d" % {"i" : (i + 1)}, replFunction = makeReplacementsAnalysis, 
		replacements = "maxEvents = -1; inputFilePath = " + inputFilePath + "/; applyFactorization = false",
		job = "analysis", queue = "8nh", outputDirectory = outputDirectory)

# TT + jets jobs
for i in range(32):
	submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau", 
		sample = "ZtoElecTau_TTplusJets_part%(i)02d" % {"i" : (i + 1)}, replFunction = makeReplacementsAnalysis, 
		replacements = "maxEvents = -1; inputFilePath = " + inputFilePath + "/; applyFactorization = false",
		job = "analysis", queue = "8nh", outputDirectory = outputDirectory)

