#!/usr/bin/env python


from TauAnalysis.Configuration.submitToBatch import submitToBatch
from TauAnalysis.Configuration.makeReplacementsAnalysis import makeReplacementsAnalysis

# name of the directory (either on afs area or castor)
# to which all .root files produced by the cmsRun job will be copied
outputFilePath = "/castor/cern.ch/user/j/jkolb/elecTauAnalysis/fall10/current/"

#inputFileType = "RECO/AOD"
inputFileType = "PATTuple"

inputFilePath = "/castor/cern.ch/user/j/jkolb/elecTauPatTuples/spring10/current/"

# small cmsRun job for testing purposes...
#submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau",
#              sample = "Ztautau_part01",
#              replFunction = makeReplacementsAnalysis, replacements =
#	       "maxEvents = 100; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = false; estimateSysUncertainties = false",
#              job = "analysis", queue = "1nh", outputFilePath = outputFilePath)

#--------------------------------------------------------------------------------
#
# Monte Carlo samples from Spring 10 production at 7TeV
#
# NOTE: The jobs get submitted to the '8nh' queue,
#       which allows for an execution time of the cmsRun jobs of up to 24 hours
#       (the queues are {'1nh' (1 hour), '8nh' (24 hours) and '1nw' (1 week execution time limit);
#        see https://twiki.cern.ch/twiki/bin/view/CMS/CMSUKCMSSWBatch for details about the CERN batch system)           
#
#--------------------------------------------------------------------------------

#
# 7 TeV data samples
#

for i in range(417):
	submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau",
		sample = "Data_7TeV_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsAnalysis, replacements =
		"maxEvents = -1; globalTag = GR_R_36X_V12B::All; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = false; estimateSysUncertainties = false",
		job = "analysis", queue = "8nh", outputFilePath = outputFilePath, type = "data",
		resourceRequest = None, submit = "yes")

for i in range(417,885):
	submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau",
		sample = "Data2010B_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsAnalysis, replacements =
		"maxEvents = -1; globalTag = GR10_P_V10::All; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = false; estimateSysUncertainties = false",
		job = "analysis", queue = "8nh", outputFilePath = outputFilePath, type = "data",
		resourceRequest = None, submit = "yes")

#
# 7 TeV MC samples
#

# Z --> tau tau jobs
for i in range(21):
	submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau",
		sample = "Ztautau_7TeV_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsAnalysis, replacements =
		"maxEvents = -1; globalTag = START3X_V27::All; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = false; estimateSysUncertainties = false",
		job = "analysis", queue = "8nh", outputFilePath = outputFilePath, type = "mc")

	# Z --> e e jobs
for i in range(84):
	submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau",
		sample = "Zee_7TeV_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsAnalysis, replacements =
		"maxEvents = -1; globalTag = START3X_V27::All; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = false; estimateSysUncertainties = false",
		job = "analysis", queue = "8nh", outputFilePath = outputFilePath, type = "mc")

	# Photon + jets jobs
submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau",
		sample = "PhotonPlusJets_Pt15to20_part01",
		replFunction = makeReplacementsAnalysis, replacements =
		"maxEvents = -1; globalTag = START38_V12::All; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = true; estimateSysUncertainties = false",
		job = "analysis", queue = "8nh", outputFilePath = outputFilePath, type = "mc")
submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau",
		sample = "PhotonPlusJets_Pt20to30_part01",
		replFunction = makeReplacementsAnalysis, replacements =
		"maxEvents = -1; globalTag = START38_V12::All; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = true; estimateSysUncertainties = false",
		job = "analysis", queue = "8nh", outputFilePath = outputFilePath, type = "mc")
submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau",
		sample = "PhotonPlusJets_Pt30to50_part01",
		replFunction = makeReplacementsAnalysis, replacements =
		"maxEvents = -1; globalTag = START38_V12::All; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = true; estimateSysUncertainties = false",
		job = "analysis", queue = "8nh", outputFilePath = outputFilePath, type = "mc")
submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau",
		sample = "PhotonPlusJets_Pt50to80_part01",
		replFunction = makeReplacementsAnalysis, replacements =
		"maxEvents = -1; globalTag = START38_V12::All; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = true; estimateSysUncertainties = false",
		job = "analysis", queue = "8nh", outputFilePath = outputFilePath, type = "mc")
submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau",
		sample = "PhotonPlusJets_Pt80to120_part01",
		replFunction = makeReplacementsAnalysis, replacements =
		"maxEvents = -1; globalTag = START38_V12::All; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = true; estimateSysUncertainties = false",
		job = "analysis", queue = "8nh", outputFilePath = outputFilePath, type = "mc")
for i in range(10):
	submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau",
		sample = "PhotonPlusJets_Pt30_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsAnalysis, replacements =
		"maxEvents = -1; globalTag = START38_V12::All; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = true; estimateSysUncertainties = false",
		job = "analysis", queue = "8nh", outputFilePath = outputFilePath, type = "mc")


# QCD_BCtoE jobs
for i in range(195):
	submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau",
		sample = "QCD_BCtoE_Pt20to30_7TeV_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsAnalysis, replacements =
		"maxEvents = -1; globalTag = START3X_V27::All; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = true; estimateSysUncertainties = false",
		job = "analysis", queue = "8nh", outputFilePath = outputFilePath, type = "mc")

for i in range(167):
	submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau",
		sample = "QCD_BCtoE_Pt30to80_7TeV_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsAnalysis, replacements =
		"maxEvents = -1; globalTag = START3X_V27::All; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = true; estimateSysUncertainties = false",
		job = "analysis", queue = "8nh", outputFilePath = outputFilePath, type = "mc")

for i in range(84):
	submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau",
		sample = "QCD_BCtoE_Pt80to170_7TeV_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsAnalysis, replacements =
		"maxEvents = -1; globalTag = START3X_V27::All; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = true; estimateSysUncertainties = false",
		job = "analysis", queue = "8nh", outputFilePath = outputFilePath, type = "mc")    

# QCD_EMenriched jobs
for i in range(184):
	submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau",
		sample = "QCD_EMenriched_Pt20to30_7TeV_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsAnalysis, replacements =
		"maxEvents = -1; globalTag = START3X_V27::All; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = true; estimateSysUncertainties = false",
		job = "analysis", queue = "1nd", outputFilePath = outputFilePath, type = "mc")

for i in range(1350):    
	submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau",
		sample = "QCD_EMenriched_Pt30to80_7TeV_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsAnalysis, replacements =
		"maxEvents = -1; globalTag = START3X_V27::All; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = true; estimateSysUncertainties = false",
		job = "analysis", queue = "1nd", outputFilePath = outputFilePath, type = "mc")

for i in range(248):    
	submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau",
		sample = "QCD_EMenriched_Pt80to170_7TeV_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsAnalysis, replacements =
		"maxEvents = -1; globalTag = START3X_V27::All; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = true; estimateSysUncertainties = false",
		job = "analysis", queue = "1nd", outputFilePath = outputFilePath, type = "mc")    

# W/Z + jets jobs
for i in range(82):
	submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau", 
		sample = "WplusJets_7TeV_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsAnalysis, replacements =
		"maxEvents = -1; globalTag = START3X_V27::All; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = false; estimateSysUncertainties = false; estimateSysUncertainties = false",
		job = "analysis", queue = "1nd", outputFilePath = outputFilePath, type = "mc")

for i in range(14):
	submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau", 
		sample = "ZeePlusJets_7TeV_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsAnalysis, replacements =
		"maxEvents = -1; globalTag = START3X_V27::All; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = false; estimateSysUncertainties = false; estimateSysUncertainties = false",
		job = "analysis", queue = "1nd", outputFilePath = outputFilePath, type = "mc")
	submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau", 
		sample = "ZtautauPlusJets_7TeV_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsAnalysis, replacements =
		"maxEvents = -1; globalTag = START3X_V27::All; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = false; estimateSysUncertainties = false; estimateSysUncertainties = false",
		job = "analysis", queue = "1nd", outputFilePath = outputFilePath, type = "mc")

# TT + jets jobs
for i in range(52):
	submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau",
		sample = "TTplusJets_7TeV_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsAnalysis, replacements =
		"maxEvents = -1; globalTag = START3X_V27::All; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = false; estimateSysUncertainties = false; estimateSysUncertainties = false",
		job = "analysis", queue = "1nd", outputFilePath = outputFilePath, type = "mc")

