#!/usr/bin/env python

from TauAnalysis.Configuration.submitToBatch import submitToBatch
from TauAnalysis.Configuration.makeReplacementsAnalysis import makeReplacementsAnalysis

# name of the directory (either on afs area or castor)
# to which all .root files produced by the cmsRun job will be copied
outputFilePath = "/castor/cern.ch/user/j/jkolb/elecTauAnalysis/summer09/hists/"
#outputFilePath = "/castor/cern.ch/user/v/veelken/plots/ZtoElecTau/"

#inputFileType = "RECO/AOD"
inputFileType = "PATTuple"

inputFilePath = "/castor/cern.ch/user/j/jkolb/elecTauPatTuples/summer09/"

# small cmsRun job for testing purposes...
#submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau",
#              sample = "Ztautau_part01",
#              replFunction = makeReplacementsAnalysis, replacements =
#	       "maxEvents = 100; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = false; estimateSysUncertainties = false",
#              job = "analysis", queue = "1nh", outputFilePath = outputFilePath)

#--------------------------------------------------------------------------------
#
# Monte Carlo samples from Summer'09 production, both 7TeV and 10TeV energies
#
# NOTE: The jobs get submitted to the '8nh' queue,
#       which allows for an execution time of the cmsRun jobs of up to 24 hours
#       (the queues are {'1nh' (1 hour), '8nh' (24 hours) and '1nw' (1 week execution time limit);
#        see https://twiki.cern.ch/twiki/bin/view/CMS/CMSUKCMSSWBatch for details about the CERN batch system)           
#
#--------------------------------------------------------------------------------

#
# 10 TeV samples
#

# Z --> tau tau jobs
for i in range(23):
    submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau",
		  sample = "Ztautau_10TeV_part%(i)02d" % {"i" : (i + 1)},
		  replFunction = makeReplacementsAnalysis, replacements =
                  "maxEvents = -1; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = false; estimateSysUncertainties = false",
		  job = "analysis", queue = "8nh", outputFilePath = outputFilePath)

# Z --> e e jobs
for i in range(57):
    submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau",
		  sample = "Zee_10TeV_part%(i)02d" % {"i" : (i + 1)},
		  replFunction = makeReplacementsAnalysis, replacements =
                  "maxEvents = -1; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = false; estimateSysUncertainties = false",
		  job = "analysis", queue = "8nh", outputFilePath = outputFilePath)

# Photon + jets jobs
submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau",
	      sample = "PhotonPlusJets_Pt15to20_10TeV",
	      replFunction = makeReplacementsAnalysis, replacements =
              "maxEvents = -1; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = false; estimateSysUncertainties = false",
	      job = "analysis", queue = "8nh", outputFilePath = outputFilePath)
submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau",
	      sample = "PhotonPlusJets_Pt20to30_10TeV",
	      replFunction = makeReplacementsAnalysis, replacements =
              "maxEvents = -1; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = false; estimateSysUncertainties = false",
	      job = "analysis", queue = "8nh", outputFilePath = outputFilePath)
submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau",
	      sample = "PhotonPlusJets_Pt30to50_10TeV",
	      replFunction = makeReplacementsAnalysis, replacements =
              "maxEvents = -1; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = false; estimateSysUncertainties = false",
	      job = "analysis", queue = "8nh", outputFilePath = outputFilePath)

# QCD_BCtoE jobs
for i in range(53):
    submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau",
		  sample = "QCD_BCtoE_Pt20to30_10TeV_part%(i)02d" % {"i" : (i + 1)},
		  replFunction = makeReplacementsAnalysis, replacements =
                  "maxEvents = -1; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = true; estimateSysUncertainties = false",
		  job = "analysis", queue = "8nh", outputFilePath = outputFilePath)
    
for i in range(68):
    submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau",
		  sample = "QCD_BCtoE_Pt30to80_10TeV_part%(i)02d" % {"i" : (i + 1)},
		  replFunction = makeReplacementsAnalysis, replacements =
                  "maxEvents = -1; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = true; estimateSysUncertainties = false",
		  job = "analysis", queue = "8nh", outputFilePath = outputFilePath)

for i in range(45):
    submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau",
	 	  sample = "QCD_BCtoE_Pt80to170_10TeV_part%(i)02d" % {"i" : (i + 1)},
		  replFunction = makeReplacementsAnalysis, replacements =
                  "maxEvents = -1; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = true; estimateSysUncertainties = false",
		  job = "analysis", queue = "8nh", outputFilePath = outputFilePath)    

# QCD_EMenriched jobs
for i in range(175):
    submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau",
		  sample = "QCD_EMenriched_Pt20to30_10TeV_part%(i)02d" % {"i" : (i + 1)},
		  replFunction = makeReplacementsAnalysis, replacements =
                  "maxEvents = -1; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = true; estimateSysUncertainties = false",
		  job = "analysis", queue = "1nd", outputFilePath = outputFilePath)

for i in range(254):    
    submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau",
		  sample = "QCD_EMenriched_Pt30to80_10TeV_part%(i)02d" % {"i" : (i + 1)},
		  replFunction = makeReplacementsAnalysis, replacements =
                  "maxEvents = -1; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = true; estimateSysUncertainties = false",
		  job = "analysis", queue = "1nd", outputFilePath = outputFilePath)

for i in range(121):    
    submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau",
		  sample = "QCD_EMenriched_Pt80to170_10TeV_part%(i)02d" % {"i" : (i + 1)},
		  replFunction = makeReplacementsAnalysis, replacements =
                  "maxEvents = -1; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = true; estimateSysUncertainties = false",
		  job = "analysis", queue = "1nd", outputFilePath = outputFilePath)    

# W/Z + jets jobs
for i in range(80):
    submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau", 
		  sample = "WplusJets_10TeV_part%(i)02d" % {"i" : (i + 1)},
		  replFunction = makeReplacementsAnalysis, replacements =
		  "maxEvents = -1; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = false; estimateSysUncertainties = false",
		  job = "analysis", queue = "8nh", outputFilePath = outputFilePath)

for i in range(51):
    submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau", 
		  sample = "ZeePlusJets_10TeV_part%(i)02d" % {"i" : (i + 1)},
		  replFunction = makeReplacementsAnalysis, replacements =
		  "maxEvents = -1; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = false; estimateSysUncertainties = false",
		  job = "analysis", queue = "8nh", outputFilePath = outputFilePath)
    submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau", 
		  sample = "ZtautauPlusJets_10TeV_part%(i)02d" % {"i" : (i + 1)},
		  replFunction = makeReplacementsAnalysis, replacements =
		  "maxEvents = -1; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = false; estimateSysUncertainties = false",
		  job = "analysis", queue = "8nh", outputFilePath = outputFilePath)
    
# TT + jets jobs
for i in range(58):
    submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau",
		  sample = "TTplusJets_10TeV_part%(i)02d" % {"i" : (i + 1)},
		  replFunction = makeReplacementsAnalysis, replacements =
		  "maxEvents = -1; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = false; estimateSysUncertainties = false",
		  job = "analysis", queue = "8nh", outputFilePath = outputFilePath)

#
# 7 TeV samples
#

# Z --> tau tau jobs
for i in range(20):
    submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau",
		  sample = "Ztautau_7TeV_part%(i)02d" % {"i" : (i + 1)},
		  replFunction = makeReplacementsAnalysis, replacements =
                  "maxEvents = -1; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = false; estimateSysUncertainties = false",
		  job = "analysis", queue = "8nh", outputFilePath = outputFilePath)

# Z --> e e jobs
for i in range(52):
    submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau",
		  sample = "Zee_7TeV_part%(i)02d" % {"i" : (i + 1)},
		  replFunction = makeReplacementsAnalysis, replacements =
                  "maxEvents = -1; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = false; estimateSysUncertainties = false",
		  job = "analysis", queue = "8nh", outputFilePath = outputFilePath)

# Photon + jets jobs
submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau",
	      sample = "PhotonPlusJets_Pt15to20_7TeV",
	      replFunction = makeReplacementsAnalysis, replacements =
              "maxEvents = -1; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = false; estimateSysUncertainties = false",
	      job = "analysis", queue = "8nh", outputFilePath = outputFilePath)
submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau",
	      sample = "PhotonPlusJets_Pt20to30_7TeV",
	      replFunction = makeReplacementsAnalysis, replacements =
              "maxEvents = -1; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = false; estimateSysUncertainties = false",
	      job = "analysis", queue = "8nh", outputFilePath = outputFilePath)
submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau",
	      sample = "PhotonPlusJets_Pt30to50_7TeV",
	      replFunction = makeReplacementsAnalysis, replacements =
              "maxEvents = -1; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = false; estimateSysUncertainties = false",
	      job = "analysis", queue = "8nh", outputFilePath = outputFilePath)

# QCD_BCtoE jobs
for i in range(57):
    submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau",
		  sample = "QCD_BCtoE_Pt20to30_7TeV_part%(i)02d" % {"i" : (i + 1)},
		  replFunction = makeReplacementsAnalysis, replacements =
                  "maxEvents = -1; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = true; estimateSysUncertainties = false",
		  job = "analysis", queue = "8nh", outputFilePath = outputFilePath)
    
for i in range(59):
    submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau",
		  sample = "QCD_BCtoE_Pt30to80_7TeV_part%(i)02d" % {"i" : (i + 1)},
		  replFunction = makeReplacementsAnalysis, replacements =
                  "maxEvents = -1; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = true; estimateSysUncertainties = false",
		  job = "analysis", queue = "8nh", outputFilePath = outputFilePath)

for i in range(33):
    submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau",
	 	  sample = "QCD_BCtoE_Pt80to170_7TeV_part%(i)02d" % {"i" : (i + 1)},
		  replFunction = makeReplacementsAnalysis, replacements =
                  "maxEvents = -1; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = true; estimateSysUncertainties = false",
		  job = "analysis", queue = "8nh", outputFilePath = outputFilePath)    

# QCD_EMenriched jobs
for i in range(174):
    submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau",
		  sample = "QCD_EMenriched_Pt20to30_7TeV_part%(i)02d" % {"i" : (i + 1)},
		  replFunction = makeReplacementsAnalysis, replacements =
                  "maxEvents = -1; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = true; estimateSysUncertainties = false",
		  job = "analysis", queue = "1nd", outputFilePath = outputFilePath)

for i in range(264):    
    submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau",
		  sample = "QCD_EMenriched_Pt30to80_7TeV_part%(i)02d" % {"i" : (i + 1)},
		  replFunction = makeReplacementsAnalysis, replacements =
                  "maxEvents = -1; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = true; estimateSysUncertainties = false",
		  job = "analysis", queue = "1nd", outputFilePath = outputFilePath)

for i in range(157):    
    submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau",
		  sample = "QCD_EMenriched_Pt80to170_7TeV_part%(i)02d" % {"i" : (i + 1)},
		  replFunction = makeReplacementsAnalysis, replacements =
                  "maxEvents = -1; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = true; estimateSysUncertainties = false",
		  job = "analysis", queue = "1nd", outputFilePath = outputFilePath)    

# W/Z + jets jobs
for i in range(82):
    submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau", 
		  sample = "WplusJets_7TeV_part%(i)02d" % {"i" : (i + 1)},
		  replFunction = makeReplacementsAnalysis, replacements =
		  "maxEvents = -1; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = false; estimateSysUncertainties = false; estimateSysUncertainties = false",
		  job = "analysis", queue = "8nh", outputFilePath = outputFilePath)

for i in range(56):
    submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau", 
		  sample = "ZeePlusJets_7TeV_part%(i)02d" % {"i" : (i + 1)},
		  replFunction = makeReplacementsAnalysis, replacements =
		  "maxEvents = -1; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = false; estimateSysUncertainties = false; estimateSysUncertainties = false",
		  job = "analysis", queue = "8nh", outputFilePath = outputFilePath)
    submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau", 
		  sample = "ZtautauPlusJets_7TeV_part%(i)02d" % {"i" : (i + 1)},
		  replFunction = makeReplacementsAnalysis, replacements =
		  "maxEvents = -1; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = false; estimateSysUncertainties = false; estimateSysUncertainties = false",
		  job = "analysis", queue = "8nh", outputFilePath = outputFilePath)
    
# TT + jets jobs
for i in range(56):
    submitToBatch(configFile = "runZtoElecTau_cfg.py", channel = "ZtoElecTau",
		  sample = "TTplusJets_7TeV_part%(i)02d" % {"i" : (i + 1)},
		  replFunction = makeReplacementsAnalysis, replacements =
		  "maxEvents = -1; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = false; estimateSysUncertainties = false; estimateSysUncertainties = false",
		  job = "analysis", queue = "8nh", outputFilePath = outputFilePath)
