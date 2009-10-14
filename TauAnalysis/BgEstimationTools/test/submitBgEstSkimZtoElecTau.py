#!/usr/bin/env python

from TauAnalysis.Configuration.submitToBatch import submitToBatch
from TauAnalysis.BgEstimationTools.makeReplacementsBgEstSkim import makeReplacementsBgEstSkim

# name of the directory (either on afs area or castor)
# to which all .root files produced by the cmsRun job will be copied
outputDirectory = "/castor/cern.ch/user/v/veelken/bgEstSkim/ZtoElecTau_looseElectronSel/"

# small cmsRun job for testing purposes...
#submitToBatch(configFile = "skimZtoElecTau_cfg.py", channel = "ZtoElecTau", sample = "Ztautau_part01",
#              replFunction = makeReplacementsBgEstSkim, replacements = "maxEvents = 100",
#              job = "bgEstSkim", queue = "1nh", outputDirectory = outputDirectory)

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

# Z --> tau tau jobs
for i in range(10):
    submitToBatch(configFile = "skimZtoElecTau_cfg.py", channel = "ZtoElecTau", sample = "Ztautau_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsBgEstSkim, replacements = "maxEvents = -1",
                  job = "bgEstSkim", queue = "1nd", outputDirectory = outputDirectory)

# Z --> e e jobs
for i in range(24):
    submitToBatch(configFile = "skimZtoElecTau_cfg.py", channel = "ZtoElecTau", sample = "Zee_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsBgEstSkim, replacements = "maxEvents = -1",
                  job = "bgEstSkim", queue = "1nd", outputDirectory = outputDirectory)

# Photon + jets jobs
submitToBatch(configFile = "skimZtoElecTau_cfg.py", channel = "ZtoElecTau", sample = "PhotonJets_Pt15to20",
              replFunction = makeReplacementsBgEstSkim, replacements = "maxEvents = -1",
              job = "bgEstSkim", queue = "1nd", outputDirectory = outputDirectory)
submitToBatch(configFile = "skimZtoElecTau_cfg.py", channel = "ZtoElecTau", sample = "PhotonJets_Pt20to25",
              replFunction = makeReplacementsBgEstSkim, replacements = "maxEvents = -1",
              job = "bgEstSkim", queue = "1nd", outputDirectory = outputDirectory)
submitToBatch(configFile = "skimZtoElecTau_cfg.py", channel = "ZtoElecTau", sample = "PhotonJets_Pt25to30",
              replFunction = makeReplacementsBgEstSkim, replacements = "maxEvents = -1",
              job = "bgEstSkim", queue = "1nd", outputDirectory = outputDirectory)
submitToBatch(configFile = "skimZtoElecTau_cfg.py", channel = "ZtoElecTau", sample = "PhotonJets_Pt30to35",
              replFunction = makeReplacementsBgEstSkim, replacements = "maxEvents = -1",
              job = "bgEstSkim", queue = "1nd", outputDirectory = outputDirectory)
submitToBatch(configFile = "skimZtoElecTau_cfg.py", channel = "ZtoElecTau", sample = "PhotonJets_PtGt35",
              replFunction = makeReplacementsBgEstSkim, replacements = "maxEvents = -1",
              job = "bgEstSkim", queue = "1nd", outputDirectory = outputDirectory)

# QCD_BCtoE jobs
for i in range(28):
    submitToBatch(configFile = "skimZtoElecTau_cfg.py", channel = "ZtoElecTau",
                  sample = "QCD_BCtoE_Pt20to30_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsBgEstSkim, replacements = "maxEvents = -1",
                  job = "bgEstSkim", queue = "1nd", outputDirectory = outputDirectory)
for i in range(46):
    submitToBatch(configFile = "skimZtoElecTau_cfg.py", channel = "ZtoElecTau",
                  sample = "QCD_BCtoE_Pt30to80_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsBgEstSkim, replacements = "maxEvents = -1",
                  job = "bgEstSkim", queue = "1nd", outputDirectory = outputDirectory)
for i in range(26):
    submitToBatch(configFile = "skimZtoElecTau_cfg.py", channel = "ZtoElecTau",
                  sample = "QCD_BCtoE_Pt80to170_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsBgEstSkim, replacements = "maxEvents = -1",
                  job = "bgEstSkim", queue = "1nd", outputDirectory = outputDirectory)    

# QCD_EMenriched jobs
for i in range(53):
    submitToBatch(configFile = "skimZtoElecTau_cfg.py", channel = "ZtoElecTau",
                  sample = "QCD_EMenriched_Pt20to30_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsBgEstSkim, replacements = "maxEvents = -1",
                  job = "bgEstSkim", queue = "1nd", outputDirectory = outputDirectory)
for i in range(220):    
    submitToBatch(configFile = "skimZtoElecTau_cfg.py", channel = "ZtoElecTau",
                  sample = "QCD_EMenriched_Pt30to80_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsBgEstSkim, replacements = "maxEvents = -1",
                  job = "bgEstSkim", queue = "1nd", outputDirectory = outputDirectory)
for i in range(59):    
    submitToBatch(configFile = "skimZtoElecTau_cfg.py", channel = "ZtoElecTau",
                  sample = "QCD_EMenriched_Pt80to170_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsBgEstSkim, replacements = "maxEvents = -1",
                  job = "bgEstSkim", queue = "1nd", outputDirectory = outputDirectory)    

# W/Z + jets jobs
for i in range(34):
    submitToBatch(configFile = "skimZtoElecTau_cfg.py", channel = "ZtoElecTau", sample = "WplusJets_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsBgEstSkim, replacements = "maxEvents = -1",
                  job = "bgEstSkim", queue = "1nd", outputDirectory = outputDirectory)

for i in range(16):
    submitToBatch(configFile = "skimZtoElecTau_cfg.py", channel = "ZtoElecTau", sample = "ZeePlusJets_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsBgEstSkim, replacements = "maxEvents = -1",
                  job = "bgEstSkim", queue = "1nd", outputDirectory = outputDirectory)
    submitToBatch(configFile = "skimZtoElecTau_cfg.py", channel = "ZtoElecTau", sample = "ZtautauPlusJets_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsBgEstSkim, replacements = "maxEvents = -1",
                  job = "bgEstSkim", queue = "1nd", outputDirectory = outputDirectory)

# TT + jets jobs
for i in range(32):
    submitToBatch(configFile = "skimZtoElecTau_cfg.py", channel = "ZtoElecTau", sample = "TTplusJets_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsBgEstSkim, replacements = "maxEvents = -1",
                  job = "bgEstSkim", queue = "1nd", outputDirectory = outputDirectory)


