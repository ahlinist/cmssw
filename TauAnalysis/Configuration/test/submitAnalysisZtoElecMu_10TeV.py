#!/usr/bin/env python

from TauAnalysis.Configuration.submitToBatch import submitToBatch
from TauAnalysis.Configuration.makeReplacementsAnalysis import makeReplacementsAnalysis

# name of the directory (either on afs area or castor)
# to which all .root files produced by the cmsRun job will be copied
outputFilePath = "/castor/cern.ch/user/v/veelken/CMSSW_3_1_2/plots/ZtoElecMu/10TeV/"

inputFileType = "RECO/AOD"
#inputFileType = "PATTuple"

inputFilePath = "/castor/cern.ch/user/v/veelken/CMSSW_3_1_2/patTuples/ZtoElecMu/"

#--------------------------------------------------------------------------------
#
# Monte Carlo samples from Summer'09 production
# reprocessed with CMSSW_3_1_2, skimmed by Sunil and Artur
#
# NOTE: The jobs get submitted to the '1nd' queue,
#       which allows for an execution time of the cmsRun jobs of up to 24 hours
#       (the queues are {'1nh' (1 hour), '1nd' (24 hours) and '1nw' (1 week execution time limit);
#        see https://twiki.cern.ch/twiki/bin/view/CMS/CMSUKCMSSWBatch for details about the CERN batch system)           
#
#--------------------------------------------------------------------------------

# Z --> tau tau jobs
for i in range(4):
    submitToBatch(configFile = "runZtoElecMu_cfg.py", channel = "ZtoElecMu",
                  sample = "Ztautau_10TeV_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsAnalysis, replacements =
                  "maxEvents = -1; globalTag = START3X_V27::All; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = false; estimateSysUncertainties = false",
                  job = "analysis", queue = "1nd", outputFilePath = outputFilePath)

# Z --> mu mu jobs
for i in range(5):
    submitToBatch(configFile = "runZtoElecMu_cfg.py", channel = "ZtoElecMu",
                  sample = "Zmumu_10TeV_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsAnalysis, replacements =
                  "maxEvents = -1; globalTag = START3X_V27::All; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = false; estimateSysUncertainties = false",
                  job = "analysis", queue = "1nd", outputFilePath = outputFilePath)

# Z --> e e + jets jobs
for i in range(3):
    submitToBatch(configFile = "runZtoElecMu_cfg.py", channel = "ZtoElecMu",
                  sample = "ZeePlusJets_10TeV_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsAnalysis, replacements =
                  "maxEvents = -1; globalTag = START3X_V27::All; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = false; estimateSysUncertainties = false",
                  job = "analysis", queue = "1nd", outputFilePath = outputFilePath)    

# pp --> mu X QCD jobs
for i in range(2):
    submitToBatch(configFile = "runZtoElecMu_cfg.py", channel = "ZtoElecMu",
                  sample = "InclusivePPmuX_10TeV_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsAnalysis, replacements =
                  "maxEvents = -1; globalTag = START3X_V27::All; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = true; estimateSysUncertainties = false",
                  job = "analysis", queue = "1nd", outputFilePath = outputFilePath)

for i in range(25):
    submitToBatch(configFile = "runZtoElecMu_cfg.py", channel = "ZtoElecMu",
                  sample = "PPmuXptGt20_10TeV_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsAnalysis, replacements =
                  "maxEvents = -1; globalTag = START3X_V27::All; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = true; estimateSysUncertainties = false",
                  job = "analysis", queue = "1nd", outputFilePath = outputFilePath)

# W + jets jobs
for i in range(5):
    submitToBatch(configFile = "runZtoElecMu_cfg.py", channel = "ZtoElecMu",
                  sample = "WplusJets_10TeV_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsAnalysis, replacements =
                  "maxEvents = -1; globalTag = START3X_V27::All; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = false; estimateSysUncertainties = false",
                  job = "analysis", queue = "1nd", outputFilePath = outputFilePath)

# ttbar + jets jobs
for i in range(12):
    submitToBatch(configFile = "runZtoElecMu_cfg.py", channel = "ZtoElecMu",
                  sample = "TTplusJets_10TeV_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsAnalysis, replacements =
                  "maxEvents = -1; globalTag = START3X_V27::All; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = false; estimateSysUncertainties = false",
                  job = "analysis", queue = "1nd", outputFilePath = outputFilePath)


