#!/usr/bin/env python

from TauAnalysis.Configuration.submitToBatch import submitToBatch
from TauAnalysis.Configuration.makeReplacementsAnalysis import makeReplacementsAnalysis

# name of the directory (either on afs area or castor) to which all .root files produced by the cmsRun job will be copied
#outputDirectory = "/afs/cern.ch/user/l/liis/scratch0/CMSSW_2_2_13/src/TauAnalysis/Configuration/test/TESTING"
outputDirectory = "/castor/cern.ch/user/l/liis/WTauNuPlots"

inputFileType = "RECO/AOD"
#inputFileType = "PATTuple"

inputFilePath = "/castor/cern.ch/user/l/liis/SkimJuly09/WtaunuSkim_PFCaloTauMet"

#--------------------------------------------------------------------------------
# NOTE: The jobs get submitted to the '1nd' queue,
#       which allows for an execution time of the cmsRun jobs of up to 24 hours
#       (the queues are {'1nh' (1 hour), '1nd' (24 hours) and '1nw' (1 week execution time limit);
#        see https://twiki.cern.ch/twiki/bin/view/CMS/CMSUKCMSSWBatch for details about the CERN batch system)           
#--------------------------------------------------------------------------------

# W --> tau nu jobs
for i in range(4): #4
    submitToBatch(configFile = "runWtoTauNu_cfg.py", channel = "WtoTauNu", sample = "Wtaunu_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsAnalysis, replacements = "maxEvents = -1; inputFileType = "+inputFileType+";inputFilePath = "+inputFilePath+"; applyFactorization = false",
                  job = "analysis", queue = "1nd", outputDirectory = outputDirectory)

#QCD jobs
for i in range(33): #33
    submitToBatch(configFile = "runWtoTauNu_cfg.py", channel = "WtoTauNu", sample = "qcd_W_part%(i)02d" % {"i" : (i+1)},
                  replFunction = makeReplacementsAnalysis, replacements = "maxEvents = -1; inputFileType = "+inputFileType+";inputFilePath = "+inputFilePath+"; applyFactorization = false",
                  job = "analysis", queue = "1nd", outputDirectory = outputDirectory)

# W --> mu nu jobs
for i in range(13):#13
    submitToBatch(configFile = "runWtoTauNu_cfg.py", channel = "WtoTauNu", sample = "Wmunu_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsAnalysis, replacements = "maxEvents = -1; inputFileType = "+inputFileType+";inputFilePath = "+inputFilePath+"; applyFactorization = false",
                  job = "analysis", queue = "1nd", outputDirectory = outputDirectory)

# W --> e nu jobs
for i in range(10):#10
    submitToBatch(configFile = "runWtoTauNu_cfg.py", channel = "WtoTauNu", sample = "Wenu_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsAnalysis, replacements = "maxEvents = -1; inputFileType = "+inputFileType+";inputFilePath = "+inputFilePath+"; applyFactorization = false",
                  job = "analysis", queue = "1nd", outputDirectory = outputDirectory)

# Z + jets jobs
for i in range(10): #10
    submitToBatch(configFile = "runWtoTauNu_cfg.py", channel = "WtoTauNu", sample = "ZplusJets_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsAnalysis, replacements = "maxEvents = -1; inputFileType = "+inputFileType+";inputFilePath = "+inputFilePath+"; applyFactorization = false",
                  job = "analysis", queue = "1nd", outputDirectory = outputDirectory)
# Ttbar jobs
for i in range(18): #18
    submitToBatch(configFile = "runWtoTauNu_cfg.py", channel = "WtoTauNu", sample = "TTplusJets_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsAnalysis, replacements = "maxEvents = -1; inputFileType = "+inputFileType+";inputFilePath = "+inputFilePath+"; applyFactorization = false",
		  job = "analysis", queue = "1nd", outputDirectory = outputDirectory)
