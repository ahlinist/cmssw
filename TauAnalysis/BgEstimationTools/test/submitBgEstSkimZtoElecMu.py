#!/usr/bin/env python

from TauAnalysis.Configuration.submitToBatch import submitToBatch
from TauAnalysis.BgEstimationTools.makeReplacementsBgEstSkim import makeReplacementsBgEstSkim

# name of the directory (either on afs area or castor)
# to which all .root files produced by the cmsRun job will be copied
outputDirectory = "/castor/cern.ch/user/v/veelken/bgEstSkim/ZtoElecMu/"

# small cmsRun job for testing purposes...
#submitToBatch(configFile = "skimZtoElecMu_cfg.py", channel = "ZtoElecMu", sample = "Ztautau_part01",
#              replFunction = makeReplacementsBgEstSkim, replacements = "maxEvents = 100",
#              job = "bgEstSkim", queue = "1nh", outputDirectory = outputDirectory)

#--------------------------------------------------------------------------------
#
# Monte Carlo samples from Summer'08 production
# reprocessed with CMSSW_2_2_3, skimmed by Letizia and Monica
#
# NOTE: The jobs get submitted to the '1nd' queue,
#       which allows for an execution time of the cmsRun jobs of up to 24 hours
#       (the queues are {'1nh' (1 hour), '1nd' (24 hours) and '1nw' (1 week execution time limit);
#        see https://twiki.cern.ch/twiki/bin/view/CMS/CMSUKCMSSWBatch for details about the CERN batch system)           
#
#--------------------------------------------------------------------------------

# Z --> tau tau jobs
submitToBatch(configFile = "skimZtoElecMu_cfg.py", channel = "ZtoElecMu", sample = "Ztautau",
              replFunction = makeReplacementsBgEstSkim, replacements = "maxEvents = -1",
              job = "bgEstSkim", queue = "1nd", outputDirectory = outputDirectory)

# Z --> e e jobs
submitToBatch(configFile = "skimZtoElecMu_cfg.py", channel = "ZtoElecMu", sample = "Zee",
              replFunction = makeReplacementsBgEstSkim, replacements = "maxEvents = -1",
              job = "bgEstSkim", queue = "1nd", outputDirectory = outputDirectory)

# Z --> mu mu jobs
submitToBatch(configFile = "skimZtoElecMu_cfg.py", channel = "ZtoElecMu", sample = "Zmumu",
              replFunction = makeReplacementsBgEstSkim, replacements = "maxEvents = -1",
              job = "bgEstSkim", queue = "1nd", outputDirectory = outputDirectory)

# pp --> mu X QCD jobs
submitToBatch(configFile = "skimZtoElecMu_cfg.py", channel = "ZtoElecMu", sample = "InclusivePPmuX",
              replFunction = makeReplacementsBgEstSkim, replacements = "maxEvents = -1",
              job = "bgEstSkim", queue = "1nd", outputDirectory = outputDirectory)

for i in range(21):
    submitToBatch(configFile = "skimZtoElecMu_cfg.py", channel = "ZtoElecMu", sample = "PPmuXptGt20_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsBgEstSkim, replacements = "maxEvents = -1",
                  job = "bgEstSkim", queue = "1nd", outputDirectory = outputDirectory)

# QCD_BCtoE jobs
# (not yet skimmed...)

# QCD_EMenriched jobs
submitToBatch(configFile = "skimZtoElecMu_cfg.py", channel = "ZtoElecMu", sample = "QCD_BCtoE_Pt20to30",
              replFunction = makeReplacementsBgEstSkim, replacements = "maxEvents = -1",
              job = "analysis", queue = "1nd", outputDirectory = outputDirectory)
submitToBatch(configFile = "skimZtoElecMu_cfg.py", channel = "ZtoElecMu", sample = "QCD_BCtoE_Pt30to80",
              replFunction = makeReplacementsBgEstSkim, replacements = "maxEvents = -1",
              job = "analysis", queue = "1nd", outputDirectory = outputDirectory)

# W/Z + jets jobs
for i in range(2):
    submitToBatch(configFile = "skimZtoElecMu_cfg.py", channel = "ZtoElecMu", sample = "WplusJets_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsBgEstSkim, replacements = "maxEvents = -1",
                  job = "bgEstSkim", queue = "1nd", outputDirectory = outputDirectory)

submitToBatch(configFile = "skimZtoElecMu_cfg.py", channel = "ZtoElecMu", sample = "ZeePlusJets",
              replFunction = makeReplacementsBgEstSkim, replacements = "maxEvents = -1",
              job = "bgEstSkim", queue = "1nd", outputDirectory = outputDirectory)
submitToBatch(configFile = "skimZtoElecMu_cfg.py", channel = "ZtoElecMu", sample = "ZmumuPlusJets",
              replFunction = makeReplacementsBgEstSkim, replacements = "maxEvents = -1",
              job = "bgEstSkim", queue = "1nw", outputDirectory = outputDirectory)
submitToBatch(configFile = "skimZtoElecMu_cfg.py", channel = "ZtoElecMu", sample = "ZtautauPlusJets",
              replFunction = makeReplacementsBgEstSkim, replacements = "maxEvents = -1",
              job = "bgEstSkim", queue = "1nd", outputDirectory = outputDirectory)

# TT + jets  jobs
for i in range(9):
    submitToBatch(configFile = "skimZtoElecMu_cfg.py", channel = "ZtoElecMu", sample = "TTplusJets_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsBgEstSkim, replacements = "maxEvents = -1",
                  job = "bgEstSkim", queue = "1nd", outputDirectory = outputDirectory)
