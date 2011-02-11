#!/usr/bin/env python

from TauAnalysis.Configuration.submitToBatch import submitToBatch
from TauAnalysis.Configuration.makeReplacementsAnalysis import makeReplacementsAnalysis


# name of the directory (either on afs area or castor) to which all .root files produced by the cmsRun job will be copied
outputFilePath = "/castor/cern.ch/user/l/liis/CMSSW_38X/Histograms"
inputFilePath = "/castor/cern.ch/user/a/abdollah/HLT/TrigEfficiency/"

inputFileType = "RECO/AOD"
#inputFileType = "PATTuple"


#--------------------------------------------------------------------------------
# NOTE: The jobs get submitted to the '1nd' queue,
#       which allows for an execution time of the cmsRun jobs of up to 24 hours
#       (the queues are {'1nh' (1 hour), '1nd' (24 hours) and '1nw' (1 week execution time limit);
#        see https://twiki.cern.ch/twiki/bin/view/CMS/CMSUKCMSSWBatch for details about the CERN batch system)           
#--------------------------------------------------------------------------------


# W --> tau nu jobs
for i in range(10):
    submitToBatch(configFile = "runWtoTauNu_cfg.py", channel = "WtoTauNu",
                  sample = "Wtaunu_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsAnalysis, replacements =
                  "maxEvents = -1; globalTag = START38_V12::All; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = false; estimateSysUncertainties = false",
                  job = "analysis", queue = "1nd", outputFilePath = outputFilePath, type = "mc")
    

# W --> e nu jobs
for i in range(33):
    submitToBatch(configFile = "runWtoTauNu_cfg.py", channel = "WtoTauNu",
                  sample = "Wenu_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsAnalysis, replacements =
                  "maxEvents = -1; globalTag = START38_V12::All; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = false; estimateSysUncertainties = false",
                  job = "analysis", queue = "1nd", outputFilePath = outputFilePath, type = "mc")

# W --> mu nu jobs
for i in range(30):
    submitToBatch(configFile = "runWtoTauNu_cfg.py", channel = "WtoTauNu",
sample = "Wmunu_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsAnalysis, replacements =
                  "maxEvents = -1; globalTag = START38_V12::All; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = false; estimateSysUncertainties = false",
                  job = "analysis", queue = "1nd", outputFilePath = outputFilePath, type = "mc")    
        
# Z --> tau tau jobs
for i in range(30):
    submitToBatch(configFile = "runWtoTauNu_cfg.py", channel = "WtoTauNu",
                  sample = "ZplusJets_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsAnalysis, replacements =
                  "maxEvents = -1; globalTag = START38_V12::All; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = false; estimateSysUncertainties = false",
                  job = "analysis", queue = "1nd", outputFilePath = outputFilePath, type = "mc")

# qcd jobs
for i in range(174):
    submitToBatch(configFile = "runWtoTauNu_cfg.py", channel = "WtoTauNu",
                  sample = "qcd_W_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsAnalysis, replacements =
                  "maxEvents = -1; globalTag = START38_V12::All; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = false; estimateSysUncertainties = false",
                  job = "analysis", queue = "1nd", outputFilePath = outputFilePath, type = "mc")
        
