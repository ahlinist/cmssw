#!/usr/bin/env python

from TauAnalysis.Configuration.submitToBatch import submitToBatch
from TauAnalysis.Configuration.makeReplacementsAnalysis import makeReplacementsAnalysis


# name of the directory (either on afs area or castor) to which all .root files produced by the cmsRun job will be copied
outputFilePath = "/castor/cern.ch/user/l/liis/CMSSW_3_1_X/WTauNuPlots7TeV"

inputFileType = "RECO/AOD"
#inputFileType = "PATTuple"

inputFilePath = "/castor/cern.ch/user/l/liis/SkimNov09/"

#--------------------------------------------------------------------------------
# NOTE: The jobs get submitted to the '1nd' queue,
#       which allows for an execution time of the cmsRun jobs of up to 24 hours
#       (the queues are {'1nh' (1 hour), '1nd' (24 hours) and '1nw' (1 week execution time limit);
#        see https://twiki.cern.ch/twiki/bin/view/CMS/CMSUKCMSSWBatch for details about the CERN batch system)           
#--------------------------------------------------------------------------------


# W --> tau nu jobs
for i in range(5):
    submitToBatch(configFile = "runWtoTauNu_cfg.py", channel = "WtoTauNu", sample = "Wtaunu_7TeV_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsAnalysis, replacements = "maxEvents = -1; inputFileType = "+inputFileType+";inputFilePath = "+inputFilePath+"; applyFactorization = false; estimateSysUncertainties = false",
                  job = "analysis", queue = "1nd", outputFilePath = outputFilePath)
# W --> mu nu jobs
for i in range(12):
    submitToBatch(configFile = "runWtoTauNu_cfg.py", channel = "WtoTauNu", sample = "Wmunu_7TeV_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsAnalysis, replacements = "maxEvents = -1; inputFileType = "+inputFileType+";inputFilePath = "+inputFilePath+"; applyFactorization = false; estimateSysUncertainties = false",
                  job = "analysis", queue = "1nd", outputFilePath = outputFilePath)
# W --> e nu jobs
for i in range(10):
    submitToBatch(configFile = "runWtoTauNu_cfg.py", channel = "WtoTauNu", sample = "Wenu_7TeV_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsAnalysis, replacements = "maxEvents = -1; inputFileType = "+inputFileType+";inputFilePath = "+inputFilePath+"; applyFactorization = false; estimateSysUncertainties = false",
                  job = "analysis", queue = "1nd", outputFilePath = outputFilePath)

# qcd jobs
for i in range(14):#14
    submitToBatch(configFile = "runWtoTauNu_cfg.py", channel = "WtoTauNu", sample = "qcd_W_7TeV_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsAnalysis, replacements = "maxEvents = -1; inputFileType = "+inputFileType+";inputFilePath = "+inputFilePath+"; applyFactorization = false; estimateSysUncertainties = false",
                  job = "analysis", queue = "1nd", outputFilePath = outputFilePath)
