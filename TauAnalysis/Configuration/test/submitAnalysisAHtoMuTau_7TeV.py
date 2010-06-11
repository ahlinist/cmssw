#!/usr/bin/env python

from TauAnalysis.Configuration.submitToBatch import submitToBatch
from TauAnalysis.Configuration.makeReplacementsAnalysis import makeReplacementsAnalysis

# name of the directory (either on afs area or castor)
# to which all .root files produced by the cmsRun job will be copied
outputFilePath = "/castor/cern.ch/user/v/veelken/CMSSW_3_6_x/plots/AHtoMuTau/7TeV/"

inputFileType = "RECO/AOD"
#inputFileType = "PATTuple"

inputFilePath = "/castor/cern.ch/user/v/veelken/CMSSW_3_1_2/patTuples/AHtoMuTau/"

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

# MSSM Higgs A/H --> tau+ tau- jobs
for i in range(8):
    submitToBatch(configFile = "runAHtoMuTau_cfg.py", channel = "AHtoMuTau",
                  sample = "AH120_tautau_7TeV_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsAnalysis, replacements =
                  "maxEvents = -1; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = false; estimateSysUncertainties = false; disableEventDump = true; globalTag = MC_36Y_V7A::All",
                  job = "analysis", queue = "1nd", outputFilePath = outputFilePath)
for i in range(4):
    submitToBatch(configFile = "runAHtoMuTau_cfg.py", channel = "AHtoMuTau",
                  sample = "AHbb120_tautau_7TeV_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsAnalysis, replacements =
                  "maxEvents = -1; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = false; estimateSysUncertainties = false; disableEventDump = true; globalTag = MC_36Y_V7A::All",
                  job = "analysis", queue = "1nd", outputFilePath = outputFilePath)
    
# Z --> tau+ tau- jobs
for i in range(16):
    submitToBatch(configFile = "runAHtoMuTau_cfg.py", channel = "AHtoMuTau",
                  sample = "Ztautau_7TeV_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsAnalysis, replacements =
                  "maxEvents = -1; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = false; estimateSysUncertainties = false; disableEventDump = true; globalTag = MC_36Y_V7A::All",
                  job = "analysis", queue = "1nd", outputFilePath = outputFilePath)
# Z --> mu+ mu- jobs
for i in range(15):
    submitToBatch(configFile = "runAHtoMuTau_cfg.py", channel = "AHtoMuTau",
                  sample = "Zmumu_7TeV_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsAnalysis, replacements =
                  "maxEvents = -1; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = false; estimateSysUncertainties = false; disableEventDump = true; globalTag = MC_36Y_V7A::All",
                  job = "analysis", queue = "1nd", outputFilePath = outputFilePath)

# Z + jets jobs
for i in range(8):
    submitToBatch(configFile = "runAHtoMuTau_cfg.py", channel = "AHtoMuTau",
                  sample = "ZplusJets_7TeV_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsAnalysis, replacements =
                  "maxEvents = -1; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = false; estimateSysUncertainties = false; disableEventDump = true; globalTag = MC_36Y_V7A::All",
                  job = "analysis", queue = "1nd", outputFilePath = outputFilePath)

# W + jets jobs
for i in range(31):
    submitToBatch(configFile = "runAHtoMuTau_cfg.py", channel = "AHtoMuTau",
                  sample = "WplusJets_7TeV_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsAnalysis, replacements =
                  "maxEvents = -1; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = false; estimateSysUncertainties = false; disableEventDump = true; globalTag = MC_36Y_V7A::All",
                  job = "analysis", queue = "1nd", outputFilePath = outputFilePath)

# W/Z + c cbar/b bbar jobs
for i in range(23):
    submitToBatch(configFile = "runAHtoMuTau_cfg.py", channel = "AHtoMuTau",
                  sample = "Vqq_7TeV_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsAnalysis, replacements =
                  "maxEvents = -1; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = false; estimateSysUncertainties = false; disableEventDump = true; globalTag = MC_36Y_V7A::All",
                  job = "analysis", queue = "1nd", outputFilePath = outputFilePath)

# ttbar + jets jobs
for i in range(10):
    submitToBatch(configFile = "runAHtoMuTau_cfg.py", channel = "AHtoMuTau",
                  sample = "TTplusJets_7TeV_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsAnalysis, replacements =
                  "maxEvents = -1; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = false; estimateSysUncertainties = false; disableEventDump = true; globalTag = MC_36Y_V7A::All",
                  job = "analysis", queue = "1nd", outputFilePath = outputFilePath)

# pp --> mu X QCD jobs
for i in range(28):
    submitToBatch(configFile = "runAHtoMuTau_cfg.py", channel = "AHtoMuTau",
                  sample = "InclusivePPmuX_7TeV_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsAnalysis, replacements =
                  "maxEvents = -1; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = true; estimateSysUncertainties = false; disableEventDump = true; globalTag = MC_36Y_V7A::All",
                  job = "analysis", queue = "1nd", outputFilePath = outputFilePath)

for i in range(71):
    submitToBatch(configFile = "runAHtoMuTau_cfg.py", channel = "AHtoMuTau",
                  sample = "PPmuXptGt20_7TeV_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsAnalysis, replacements =
                  "maxEvents = -1; inputFileType = " + inputFileType + "; inputFilePath = " + inputFilePath + "; applyFactorization = true; estimateSysUncertainties = false; disableEventDump = true; globalTag = MC_36Y_V7A::All",
                  job = "analysis", queue = "1nd", outputFilePath = outputFilePath)

