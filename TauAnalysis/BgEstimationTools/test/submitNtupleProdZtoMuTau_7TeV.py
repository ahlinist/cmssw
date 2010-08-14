#!/usr/bin/env python

from TauAnalysis.Configuration.submitToBatch import submitToBatch
from TauAnalysis.BgEstimationTools.makeReplacementsBgEstNtupleProd import makeReplacementsBgEstNtupleProd

# name of the directory (either on afs area or castor)
# to which all .root files produced by the cmsRun job will be copied
outputFilePath = "/castor/cern.ch/user/v/veelken/CMSSW_3_6_x/ntuples/ZtoMuTau/7TeV/"

#--------------------------------------------------------------------------------
#
# Monte Carlo samples from Summer'09 production
# reprocessed with CMSSW_3_1_2, skimmed by Letizia and Manuel
#
# NOTE: The jobs get submitted to the '1nd' queue,
#       which allows for an execution time of the cmsRun jobs of up to 24 hours
#       (the queues are {'1nh' (1 hour), '1nd' (24 hours) and '1nw' (1 week execution time limit);
#        see https://twiki.cern.ch/twiki/bin/view/CMS/CMSUKCMSSWBatch for details about the CERN batch system)           
#
#--------------------------------------------------------------------------------

# Z --> tau+ tau- jobs
for i in range(16):
    submitToBatch(configFile = "prodNtupleZtoMuTau_cfg.py", channel = "ZtoMuTau",
                  sample = "Ztautau_7TeV_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsBgEstNtupleProd, replacements =
                  "maxEvents = -1; skipEvents = 0; intLumiData = intLumiZtoMuTau_Data_7TeV; globalTag = MC_36Y_V7A::All",
                  job = "ntupleProd", queue = "1nd", outputFilePath = outputFilePath)

# Z --> mu+ mu- jobs
for i in range(15):
    submitToBatch(configFile = "prodNtupleZtoMuTau_cfg.py", channel = "ZtoMuTau",
                  sample = "Zmumu_7TeV_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsBgEstNtupleProd, replacements =
                  "maxEvents = -1; skipEvents = 0; intLumiData = intLumiZtoMuTau_Data_7TeV; globalTag = MC_36Y_V7A::All",
                  job = "ntupleProd", queue = "1nd", outputFilePath = outputFilePath)

# pp --> mu X QCD jobs
for i in range(28):
    submitToBatch(configFile = "prodNtupleZtoMuTau_cfg.py", channel = "ZtoMuTau",
                  sample = "InclusivePPmuX_7TeV_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsBgEstNtupleProd, replacements =
                  "maxEvents = -1; skipEvents = 0; intLumiData = intLumiZtoMuTau_Data_7TeV; globalTag = MC_36Y_V7A::All",
                  job = "ntupleProd", queue = "1nd", outputFilePath = outputFilePath)

for i in range(71):
    submitToBatch(configFile = "prodNtupleZtoMuTau_cfg.py", channel = "ZtoMuTau",
                  sample = "PPmuXptGt20_7TeV_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsBgEstNtupleProd, replacements =
                  "maxEvents = -1; skipEvents = 0; intLumiData = intLumiZtoMuTau_Data_7TeV; globalTag = MC_36Y_V7A::All",
                  job = "ntupleProd", queue = "1nd", outputFilePath = outputFilePath)

# W + jets jobs
for i in range(31):
    submitToBatch(configFile = "prodNtupleZtoMuTau_cfg.py", channel = "ZtoMuTau",
                  sample = "WplusJets_7TeV_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsBgEstNtupleProd, replacements =
                  "maxEvents = -1; skipEvents = 0; intLumiData = intLumiZtoMuTau_Data_7TeV; globalTag = MC_36Y_V7A::All",
                  job = "ntupleProd", queue = "1nd", outputFilePath = outputFilePath)

# ttbar + jets jobs
for i in range(10):
    submitToBatch(configFile = "prodNtupleZtoMuTau_cfg.py", channel = "ZtoMuTau",
                  sample = "TTplusJets_7TeV_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsBgEstNtupleProd, replacements =
                  "maxEvents = -1; skipEvents = 0; intLumiData = intLumiZtoMuTau_Data_7TeV; globalTag = MC_36Y_V7A::All",
                  job = "ntupleProd", queue = "1nd", outputFilePath = outputFilePath)

