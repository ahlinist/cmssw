#!/usr/bin/env python

from TauAnalysis.Configuration.submitToBatch import submitToBatch
from TauAnalysis.Configuration.makeReplacementsAnalysis import makeReplacementsAnalysis

# name of the directory (either on afs area or castor)
# to which all .root files produced by the cmsRun job will be copied
outputDirectory = "/castor/cern.ch/user/v/veelken/plots/AHtoElecMu/"

# small cmsRun job for testing purposes...
#submitToBatch(configFile = "runAHtoElecMu_cfg.py", channel = "AHtoElecMu", sample = "Ztautau",
#              replFunction = makeReplacementsAnalysis, replacements = "maxEvents = 100; applyFactorization = false",
#              job = "analysis", queue = "1nh", outputDirectory = outputDirectory)

#--------------------------------------------------------------------------------
#
# Monte Carlo samples from Summer'08 production
# reprocessed with CMSSW_2_2_3, skimmed by Sunil and Giuseppe
#
# NOTE: The jobs get submitted to the '1nd' queue,
#       which allows for an execution time of the cmsRun jobs of up to 24 hours
#       (the queues are {'1nh' (1 hour), '1nd' (24 hours) and '1nw' (1 week execution time limit);
#        see https://twiki.cern.ch/twiki/bin/view/CMS/CMSUKCMSSWBatch for details about the CERN batch system)           
#
#--------------------------------------------------------------------------------

# Higgs jobs (mass = 115 GeV)
submitToBatch(configFile = "runAHtoElecMu_cfg.py", channel = "AHtoElecMu", sample = "AH115tautau",
              replFunction = makeReplacementsAnalysis, replacements = "maxEvents = -1; applyFactorization = false",
              job = "analysis", queue = "1nd", outputDirectory = outputDirectory)
submitToBatch(configFile = "runAHtoElecMu_cfg.py", channel = "AHtoElecMu", sample = "AH115bbtautau",
              replFunction = makeReplacementsAnalysis, replacements = "maxEvents = -1; applyFactorization = false",
              job = "analysis", queue = "1nd", outputDirectory = outputDirectory)
submitToBatch(configFile = "runAHtoElecMu_cfg.py", channel = "AHtoElecMu", sample = "AH115tautau2l",
              replFunction = makeReplacementsAnalysis, replacements = "maxEvents = -1; applyFactorization = false",
              job = "analysis", queue = "1nd", outputDirectory = outputDirectory)
for i in range(22):
    submitToBatch(configFile = "runAHtoElecMu_cfg.py", channel = "AHtoElecMu", sample = "AH115bbtautau2l_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsAnalysis, replacements = "maxEvents = -1; applyFactorization = false",
                  job = "analysis", queue = "1nd", outputDirectory = outputDirectory)
    
# Higgs jobs (mass = 160 GeV)
submitToBatch(configFile = "runAHtoElecMu_cfg.py", channel = "AHtoElecMu", sample = "AH160tautau",
              replFunction = makeReplacementsAnalysis, replacements = "maxEvents = -1; applyFactorization = false",
              job = "analysis", queue = "1nd", outputDirectory = outputDirectory)
submitToBatch(configFile = "runAHtoElecMu_cfg.py", channel = "AHtoElecMu", sample = "AH160bbtautau",
              replFunction = makeReplacementsAnalysis, replacements = "maxEvents = -1; applyFactorization = false",
              job = "analysis", queue = "1nd", outputDirectory = outputDirectory)
submitToBatch(configFile = "runAHtoElecMu_cfg.py", channel = "AHtoElecMu", sample = "AH160tautau2l",
              replFunction = makeReplacementsAnalysis, replacements = "maxEvents = -1; applyFactorization = false",
              job = "analysis", queue = "1nd", outputDirectory = outputDirectory)
for i in range(26):
    submitToBatch(configFile = "runAHtoElecMu_cfg.py", channel = "AHtoElecMu", sample = "AH160bbtautau2l_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsAnalysis, replacements = "maxEvents = -1; applyFactorization = false",
                  job = "analysis", queue = "1nd", outputDirectory = outputDirectory)

# Higgs analysis specific background jobs
submitToBatch(configFile = "runAHtoElecMu_cfg.py", channel = "AHtoElecMu", sample = "VQQ",
              replFunction = makeReplacementsAnalysis, replacements = "maxEvents = -1; applyFactorization = false",
              job = "analysis", queue = "1nd", outputDirectory = outputDirectory)
submitToBatch(configFile = "runAHtoElecMu_cfg.py", channel = "AHtoElecMu", sample = "WW",
              replFunction = makeReplacementsAnalysis, replacements = "maxEvents = -1; applyFactorization = false",
              job = "analysis", queue = "1nd", outputDirectory = outputDirectory)
submitToBatch(configFile = "runAHtoElecMu_cfg.py", channel = "AHtoElecMu", sample = "TW",
              replFunction = makeReplacementsAnalysis, replacements = "maxEvents = -1; applyFactorization = false",
              job = "analysis", queue = "1nd", outputDirectory = outputDirectory)

# Z --> tau tau jobs 
submitToBatch(configFile = "runAHtoElecMu_cfg.py", channel = "AHtoElecMu", sample = "Ztautau",
              replFunction = makeReplacementsAnalysis, replacements = "maxEvents = -1; applyFactorization = false",
              job = "analysis", queue = "1nd", outputDirectory = outputDirectory)

# W/Z + jets jobs
for i in range(2):
    submitToBatch(configFile = "runAHtoElecMu_cfg.py", channel = "AHtoElecMu", sample = "WplusJets_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsAnalysis, replacements = "maxEvents = -1; applyFactorization = false",
                  job = "analysis", queue = "1nd", outputDirectory = outputDirectory)

submitToBatch(configFile = "runAHtoElecMu_cfg.py", channel = "AHtoElecMu", sample = "ZplusJets",
              replFunction = makeReplacementsAnalysis, replacements = "maxEvents = -1; applyFactorization = false",
              job = "analysis", queue = "1nd", outputDirectory = outputDirectory)

# TT + jets jobs
for i in range(22):
    submitToBatch(configFile = "runAHtoElecMu_cfg.py", channel = "AHtoElecMu", sample = "TTJets_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsAnalysis, replacements = "maxEvents = -1; applyFactorization = false",
                  job = "analysis", queue = "1nd", outputDirectory = outputDirectory)
