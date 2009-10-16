#!/usr/bin/env python

from TauAnalysis.Configuration.submitToBatch import submitToBatch
from TauAnalysis.BgEstimationTools.makeReplacementsBgEstNtupleProd import makeReplacementsBgEstNtupleProd

# name of the directory (either on afs area or castor)
# to which all .root files produced by the cmsRun job will be copied
outputDirectory = "/castor/cern.ch/user/v/veelken/bgEstNtuples/ZtoElecTau_looseElectronSel/"

# small cmsRun job for testing purposes...
#submitToBatch(configFile = "prodNtupleZtoElecTau_cfg.py", channel = "ZtoElecTau",
#              sample = "Ztautau",
#              replFunction = makeReplacementsBgEstNtupleProd, replacements = "maxEvents = 100; skipEvents = 0",
#              job = "bgEstNtupleProd", queue = "1nh", outputDirectory = outputDirectory)

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
for i in range(2):
    submitToBatch(configFile = "prodNtupleZtoElecTau_cfg.py", channel = "ZtoElecTau",
                  sample = "Ztautau_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsBgEstNtupleProd, replacements = "maxEvents = -1; skipEvents = 0",
                  job = "bgEstNtupleProd", queue = "1nd", outputDirectory = outputDirectory)

# Z --> e e jobs
for i in range(3):
    submitToBatch(configFile = "prodNtupleZtoElecTau_cfg.py", channel = "ZtoElecTau",
                  sample = "Zee_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsBgEstNtupleProd, replacements = "maxEvents = -1; skipEvents = 0",
                  job = "bgEstNtupleProd", queue = "1nd", outputDirectory = outputDirectory)

# QCD_BCtoE jobs
for i in range(3):
    submitToBatch(configFile = "prodNtupleZtoElecTau_cfg.py", channel = "ZtoElecTau",
                  sample = "QCD_BCtoE_Pt20to30_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsBgEstNtupleProd, replacements = "maxEvents = -1; skipEvents = 0",
                  job = "bgEstNtupleProd", queue = "1nd", outputDirectory = outputDirectory)
for i in range(5):    
    submitToBatch(configFile = "prodNtupleZtoElecTau_cfg.py", channel = "ZtoElecTau",
                  sample = "QCD_BCtoE_Pt30to80_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsBgEstNtupleProd, replacements = "maxEvents = -1; skipEvents = 0",
                  job = "bgEstNtupleProd", queue = "1nd", outputDirectory = outputDirectory)
for i in range(3):    
    submitToBatch(configFile = "prodNtupleZtoElecTau_cfg.py", channel = "ZtoElecTau",
                  sample = "QCD_BCtoE_Pt80to170_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsBgEstNtupleProd, replacements = "maxEvents = -1; skipEvents = 0",
                  job = "bgEstNtupleProd", queue = "1nd", outputDirectory = outputDirectory)

for i in range(6):
    submitToBatch(configFile = "prodNtupleZtoElecTau_cfg.py", channel = "ZtoElecTau",
                  sample = "QCD_EMenriched_Pt20to30_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsBgEstNtupleProd, replacements = "maxEvents = -1; skipEvents = 0",
                  job = "bgEstNtupleProd", queue = "1nd", outputDirectory = outputDirectory)
for i in range(22):
    submitToBatch(configFile = "prodNtupleZtoElecTau_cfg.py", channel = "ZtoElecTau",
                  sample = "QCD_EMenriched_Pt30to80_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsBgEstNtupleProd, replacements = "maxEvents = -1; skipEvents = 0",
                  job = "bgEstNtupleProd", queue = "1nd", outputDirectory = outputDirectory)
for i in range(6):
    submitToBatch(configFile = "prodNtupleZtoElecTau_cfg.py", channel = "ZtoElecTau",
                  sample = "QCD_EMenriched_Pt80to170_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsBgEstNtupleProd, replacements = "maxEvents = -1; skipEvents = 0",
                  job = "bgEstNtupleProd", queue = "1nd", outputDirectory = outputDirectory)

# Photon + jets jobs
submitToBatch(configFile = "prodNtupleZtoElecTau_cfg.py", channel = "ZtoElecTau",
              sample = "PhotonJets_Pt15to20",
              replFunction = makeReplacementsBgEstNtupleProd, replacements = "maxEvents = -1; skipEvents = 0",
              job = "bgEstNtupleProd", queue = "1nd", outputDirectory = outputDirectory)
submitToBatch(configFile = "prodNtupleZtoElecTau_cfg.py", channel = "ZtoElecTau",
              sample = "PhotonJets_Pt20to25",
              replFunction = makeReplacementsBgEstNtupleProd, replacements = "maxEvents = -1; skipEvents = 0",
              job = "bgEstNtupleProd", queue = "1nd", outputDirectory = outputDirectory)
submitToBatch(configFile = "prodNtupleZtoElecTau_cfg.py", channel = "ZtoElecTau",
              sample = "PhotonJets_Pt25to30",
              replFunction = makeReplacementsBgEstNtupleProd, replacements = "maxEvents = -1; skipEvents = 0",
              job = "bgEstNtupleProd", queue = "1nd", outputDirectory = outputDirectory)
submitToBatch(configFile = "prodNtupleZtoElecTau_cfg.py", channel = "ZtoElecTau",
              sample = "PhotonJets_Pt30to35",
              replFunction = makeReplacementsBgEstNtupleProd, replacements = "maxEvents = -1; skipEvents = 0",
              job = "bgEstNtupleProd", queue = "1nd", outputDirectory = outputDirectory)
submitToBatch(configFile = "prodNtupleZtoElecTau_cfg.py", channel = "ZtoElecTau",
              sample = "PhotonJets_PtGt35",
              replFunction = makeReplacementsBgEstNtupleProd, replacements = "maxEvents = -1; skipEvents = 0",
              job = "bgEstNtupleProd", queue = "1nd", outputDirectory = outputDirectory)

# W/Z + jets jobs
for i in range(4):
    submitToBatch(configFile = "prodNtupleZtoElecTau_cfg.py", channel = "ZtoElecTau",
                  sample = "WplusJets_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsBgEstNtupleProd, replacements = "maxEvents = -1; skipEvents = 0",
                  job = "bgEstNtupleProd", queue = "1nd", outputDirectory = outputDirectory)

for i in range(3):    
    submitToBatch(configFile = "prodNtupleZtoElecTau_cfg.py", channel = "ZtoElecTau",
                  sample = "ZeePlusJets_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsBgEstNtupleProd, replacements = "maxEvents = -1; skipEvents = 0",
                  job = "bgEstNtupleProd", queue = "1nd", outputDirectory = outputDirectory)
for i in range(3):
    submitToBatch(configFile = "prodNtupleZtoElecTau_cfg.py", channel = "ZtoElecTau",
                  sample = "ZtautauPlusJets_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsBgEstNtupleProd, replacements = "maxEvents = -1; skipEvents = 0",
                  job = "bgEstNtupleProd", queue = "1nd", outputDirectory = outputDirectory)

# TT + jets jobs
for i in range(7):
    submitToBatch(configFile = "prodNtupleZtoElecTau_cfg.py", channel = "ZtoElecTau",
                  sample = "TTplusJets_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsBgEstNtupleProd, replacements = "maxEvents = -1; skipEvents = 0",
                  job = "bgEstNtupleProd", queue = "1nd", outputDirectory = outputDirectory)
