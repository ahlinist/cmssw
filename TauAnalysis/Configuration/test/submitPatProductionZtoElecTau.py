#!/usr/bin/env python

from TauAnalysis.Configuration.submitToBatch import submitToBatch
from TauAnalysis.Configuration.makeReplacementsPatProduction import makeReplacementsPatProduction

# name of the directory (either on afs area or castor)
# to which all .root files produced by the cmsRun job will be copied
outputFilePath = "/castor/cern.ch/user/j/jkolb/elecTauPatTuples/summer09/"

# small cmsRun job for testing purposes...
#submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau", sample = "ZtoElecTau_Ztautau_part01",
#              replFunction = makeReplacementsPatProduction, replacements = "maxEvents = 100",
#              job = "PatProduction", queue = "1nh", outputFilePath = outputFilePath)

#--------------------------------------------------------------------------------
#
# Monte Carlo samples from Summer'08 production
# reprocessed with CMSSW_2_2_3, skimmed by Jeff Kolb
#
# NOTE: The jobs get submitted to the '1nd' queue,
#       which allows for an execution time of the cmsRun jobs of up to 24 hours
#       (the queues are {'1nh' (1 hour), '1nd' (24 hours) and '1nw' (1 week execution time limit);
#        see https://twiki.cern.ch/twiki/bin/view/CMS/CMSUKCMSSWBatch for details about the CERN batch system)           
#
#--------------------------------------------------------------------------------

# 10TeV samples

# Z --> tau tau jobs
for i in range(23):
	submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau", 
		sample = "ZtoElecTau_Ztautau_10TeV_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsPatProduction, replacements = "maxEvents = -1",
		job = "PatProduction", queue = "1nd", outputFilePath = outputFilePath)

# Z --> e e jobs
for i in range(57):
	submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau", 
		sample = "ZtoElecTau_Zee_10TeV_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsPatProduction, replacements = "maxEvents = -1",
		job = "PatProduction", queue = "1nd", outputFilePath = outputFilePath)

# Photon + jets jobs
submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau", sample = "ZtoElecTau_PhotonPlusJets_Pt15to20_10TeV",
	replFunction = makeReplacementsPatProduction, replacements = "maxEvents = -1",
	job = "PatProduction", queue = "8nh", outputFilePath = outputFilePath)
submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau", sample = "ZtoElecTau_PhotonPlusJets_Pt20to30_10TeV",
	replFunction = makeReplacementsPatProduction, replacements = "maxEvents = -1",
	job = "PatProduction", queue = "8nh", outputFilePath = outputFilePath)
submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau", sample = "ZtoElecTau_PhotonPlusJets_Pt30to50_10TeV",
	replFunction = makeReplacementsPatProduction, replacements = "maxEvents = -1",
	job = "PatProduction", queue = "8nh", outputFilePath = outputFilePath)

# QCD_BCtoE jobs
for i in range(53):
	submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau",
		sample = "ZtoElecTau_QCD_BCtoE_Pt20to30_10TeV_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsPatProduction, replacements = "maxEvents = -1",
		job = "PatProduction", queue = "1nd", outputFilePath = outputFilePath)
for i in range(68):
	submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau",
		sample = "ZtoElecTau_QCD_BCtoE_Pt30to80_10TeV_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsPatProduction, replacements = "maxEvents = -1",
		job = "PatProduction", queue = "1nd", outputFilePath = outputFilePath)
for i in range(45):
		submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau",
		sample = "ZtoElecTau_QCD_BCtoE_Pt80to170_10TeV_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsPatProduction, replacements = "maxEvents = -1",
		job = "PatProduction", queue = "1nd", outputFilePath = outputFilePath)    

# QCD_EMenriched jobs
for i in range(175):
	submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau",
		sample = "ZtoElecTau_QCD_EMenriched_Pt20to30_10TeV_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsPatProduction, replacements = "maxEvents = -1",
		job = "PatProduction", queue = "1nd", outputFilePath = outputFilePath)
for i in range(254):    
	submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau",
	sample = "ZtoElecTau_QCD_EMenriched_Pt30to80_10TeV_part%(i)02d" % {"i" : (i + 1)},
	replFunction = makeReplacementsPatProduction, replacements = "maxEvents = -1",
	job = "PatProduction", queue = "1nd", outputFilePath = outputFilePath)
for i in range(121):    
	submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau",
		sample = "ZtoElecTau_QCD_EMenriched_Pt80to170_10TeV_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsPatProduction, replacements = "maxEvents = -1",
		job = "PatProduction", queue = "1nd", outputFilePath = outputFilePath)    

# W/Z + jets jobs
for i in range(80):
	submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau", 
		sample = "ZtoElecTau_WplusJets_10TeV_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsPatProduction, replacements = "maxEvents = -1",
		job = "PatProduction", queue = "1nd", outputFilePath = outputFilePath)

for i in range(51):
	submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau", 
		sample = "ZtoElecTau_ZeePlusJets_10TeV_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsPatProduction, replacements = "maxEvents = -1",
		job = "PatProduction", queue = "1nd", outputFilePath = outputFilePath)
	submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau", 
		sample = "ZtoElecTau_ZtautauPlusJets_10TeV_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsPatProduction, replacements = "maxEvents = -1",
		job = "PatProduction", queue = "1nd", outputFilePath = outputFilePath)

# TT + jets jobs
for i in range(58):
	submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau", 
		sample = "ZtoElecTau_TTplusJets_10TeV_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsPatProduction, replacements = "maxEvents = -1",
		job = "PatProduction", queue = "1nd", outputFilePath = outputFilePath)

#
# 7TeV samples
	
# Z --> tau tau jobs
for i in range(16):
	submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau", 
		sample = "ZtoElecTau_Ztautau_7TeV_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsPatProduction, replacements = "maxEvents = -1",
		job = "PatProduction", queue = "1nd", outputFilePath = outputFilePath)

# Z --> e e jobs
for i in range(52):
	submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau", 
		sample = "ZtoElecTau_Zee_7TeV_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsPatProduction, replacements = "maxEvents = -1",
		job = "PatProduction", queue = "1nd", outputFilePath = outputFilePath)

# Photon + jets jobs
submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau", sample = "ZtoElecTau_PhotonPlusJets_Pt15to20_7TeV",
	replFunction = makeReplacementsPatProduction, replacements = "maxEvents = -1",
	job = "PatProduction", queue = "8nh", outputFilePath = outputFilePath)
submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau", sample = "ZtoElecTau_PhotonPlusJets_Pt20to30_7TeV",
	replFunction = makeReplacementsPatProduction, replacements = "maxEvents = -1",
	job = "PatProduction", queue = "8nh", outputFilePath = outputFilePath)
submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau", sample = "ZtoElecTau_PhotonPlusJets_Pt30to50_7TeV",
	replFunction = makeReplacementsPatProduction, replacements = "maxEvents = -1",
	job = "PatProduction", queue = "8nh", outputFilePath = outputFilePath)

# QCD_BCtoE jobs
for i in range(57):
	submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau",
		sample = "ZtoElecTau_QCD_BCtoE_Pt20to30_7TeV_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsPatProduction, replacements = "maxEvents = -1",
		job = "PatProduction", queue = "1nd", outputFilePath = outputFilePath)
for i in range(59):
	submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau",
		sample = "ZtoElecTau_QCD_BCtoE_Pt30to80_7TeV_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsPatProduction, replacements = "maxEvents = -1",
		job = "PatProduction", queue = "1nd", outputFilePath = outputFilePath)
for i in range(33):
		submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau",
		sample = "ZtoElecTau_QCD_BCtoE_Pt80to170_7TeV_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsPatProduction, replacements = "maxEvents = -1",
		job = "PatProduction", queue = "1nd", outputFilePath = outputFilePath)    

# QCD_EMenriched jobs
for i in range(174):
	submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau",
		sample = "ZtoElecTau_QCD_EMenriched_Pt20to30_7TeV_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsPatProduction, replacements = "maxEvents = -1",
		job = "PatProduction", queue = "1nd", outputFilePath = outputFilePath)
for i in range(264):    
	submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau",
	sample = "ZtoElecTau_QCD_EMenriched_Pt30to80_7TeV_part%(i)02d" % {"i" : (i + 1)},
	replFunction = makeReplacementsPatProduction, replacements = "maxEvents = -1",
	job = "PatProduction", queue = "1nd", outputFilePath = outputFilePath)
for i in range(157):    
	submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau",
		sample = "ZtoElecTau_QCD_EMenriched_Pt80to170_7TeV_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsPatProduction, replacements = "maxEvents = -1",
		job = "PatProduction", queue = "1nd", outputFilePath = outputFilePath)    

# W/Z + jets jobs
for i in range(82):
	submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau", 
		sample = "ZtoElecTau_WplusJets_7TeV_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsPatProduction, replacements = "maxEvents = -1",
		job = "PatProduction", queue = "1nd", outputFilePath = outputFilePath)

for i in range(56):
	submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau", 
		sample = "ZtoElecTau_ZeePlusJets_7TeV_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsPatProduction, replacements = "maxEvents = -1",
		job = "PatProduction", queue = "1nd", outputFilePath = outputFilePath)
	submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau", 
		sample = "ZtoElecTau_ZtautauPlusJets_7TeV_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsPatProduction, replacements = "maxEvents = -1",
		job = "PatProduction", queue = "1nd", outputFilePath = outputFilePath)

# TT + jets jobs
for i in range(56):
	submitToBatch(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau", 
		sample = "ZtoElecTau_TTplusJets_7TeV_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsPatProduction, replacements = "maxEvents = -1",
		job = "PatProduction", queue = "1nd", outputFilePath = outputFilePath)

