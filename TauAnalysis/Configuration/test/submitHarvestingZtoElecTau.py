#!/usr/bin/env python

from TauAnalysis.Configuration.submitToBatch import submitToBatch
from TauAnalysis.Configuration.makeReplacementsHarvesting import makeReplacementsHarvesting

# name of the directory (either on afs area or castor)
# to which all .root files produced by the cmsRun job will be copied
outputFilePath = "/castor/cern.ch/user/j/jkolb/elecTauAnalysis/fall10/current/"

inputFilePath = "rfio:" + outputFilePath

#--------------------------------------------------------------------------------
#
# Add histograms, numbers in FilterStatisticsTables and run + event numbers
# stored as DQM MonitorElements in different ROOT files
#
# NOTE: The jobs get submitted to the '1nh' queue,
#       which allows for an execution time of the cmsRun jobs of up to 1 hour
#       (the queues are {'1nh' (1 hour), '1nd' (24 hours) and '1nw' (1 week execution time limit);
#        see https://twiki.cern.ch/twiki/bin/view/CMS/CMSUKCMSSWBatch for details about the CERN batch system)           
#
#--------------------------------------------------------------------------------

# 7TeV samples

# harvest 2010A data
for i in range(9):
	submitToBatch(configFile = "harvestZtoElecTauPlots_cfg.py", channel = "ZtoElecTau", 
							sample = "Data_7TeV" + "_part%(i)02d" % {"i" : (i + 1)},
              replFunction = makeReplacementsHarvesting, replacements = "inputFilePath = " + inputFilePath,
              job = "harvesting", queue = "8nh", outputFilePath = outputFilePath, type = "data")

# harvest 2010B data
for i in range(7):
	submitToBatch(configFile = "harvestZtoElecTauPlots_cfg.py", channel = "ZtoElecTau", 
							sample = "Data2010B" + "_part%(i)02d" % {"i" : (i + 1)},
              replFunction = makeReplacementsHarvesting, replacements = "inputFilePath = " + inputFilePath,
              job = "harvesting", queue = "8nh", outputFilePath = outputFilePath, type = "data")

# harvest Z --> tau tau 
submitToBatch(configFile = "harvestZtoElecTauPlots_cfg.py", channel = "ZtoElecTau", sample = "Ztautau_7TeV",
              replFunction = makeReplacementsHarvesting, replacements = "inputFilePath = " + inputFilePath,
              job = "harvesting", queue = "1nh", outputFilePath = outputFilePath, type = "mc")

# harvest Z --> e e
for i in range(2):
	submitToBatch(configFile = "harvestZtoElecTauPlots_cfg.py", channel = "ZtoElecTau", 
		sample = "Zee_7TeV" + "_part%(i)02d" % {"i" : (i + 1)},
    replFunction = makeReplacementsHarvesting, replacements = "inputFilePath = " + inputFilePath,
    job = "harvesting", queue = "1nh", outputFilePath = outputFilePath, type = "mc")

# harvest gamma + jets - PT30 inclus
submitToBatch(configFile = "harvestZtoElecTauPlots_cfg.py", channel = "ZtoElecTau", sample = "gammaPlusJets_Pt30",
              replFunction = makeReplacementsHarvesting, replacements = "inputFilePath = " + inputFilePath,
              job = "harvesting", queue = "1nh", outputFilePath = outputFilePath, type = "mc")

# harvest QCD_BCtoE 
for i in range(3):
	submitToBatch(configFile = "harvestZtoElecTauPlots_cfg.py", channel = "ZtoElecTau", 
		sample = "QCD_BCtoE_Pt20to30_7TeV" + "_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsHarvesting, replacements = "inputFilePath = " + inputFilePath,
		job = "harvesting", queue = "8nh", outputFilePath = outputFilePath, type = "mc")
for i in range(2):
	submitToBatch(configFile = "harvestZtoElecTauPlots_cfg.py", channel = "ZtoElecTau", 
		sample = "QCD_BCtoE_Pt30to80_7TeV" + "_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsHarvesting, replacements = "inputFilePath = " + inputFilePath,
		job = "harvesting", queue = "8nh", outputFilePath = outputFilePath, type = "mc")
for i in range(2):
	submitToBatch(configFile = "harvestZtoElecTauPlots_cfg.py", channel = "ZtoElecTau", 
		sample = "QCD_BCtoE_Pt80to170_7TeV" + "_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsHarvesting, replacements = "inputFilePath = " + inputFilePath,
		job = "harvesting", queue = "8nh", outputFilePath = outputFilePath, type = "mc")

# harvest QCD_EMenriched
for i in range(3):
	submitToBatch(configFile = "harvestZtoElecTauPlots_cfg.py", channel = "ZtoElecTau", 
		sample = "QCD_EMenriched_Pt20to30_7TeV" + "_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsHarvesting, replacements = "inputFilePath = " + inputFilePath,
		job = "harvesting", queue = "8nh", outputFilePath = outputFilePath, type = "mc")
for i in range(20):
	submitToBatch(configFile = "harvestZtoElecTauPlots_cfg.py", channel = "ZtoElecTau", 
		sample = "QCD_EMenriched_Pt30to80_7TeV" + "_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsHarvesting, replacements = "inputFilePath = " + inputFilePath,
		job = "harvesting", queue = "8nh", outputFilePath = outputFilePath, type = "mc")
for i in range(3):
	submitToBatch(configFile = "harvestZtoElecTauPlots_cfg.py", channel = "ZtoElecTau", 
		sample = "QCD_EMenriched_Pt80to170_7TeV" + "_part%(i)02d" % {"i" : (i + 1)},
		replFunction = makeReplacementsHarvesting, replacements = "inputFilePath = " + inputFilePath,
		job = "harvesting", queue = "8nh", outputFilePath = outputFilePath, type = "mc")

# harvest W/Z + jets
for i in range(2):
	submitToBatch(configFile = "harvestZtoElecTauPlots_cfg.py", channel = "ZtoElecTau", 
			sample = "WplusJets_7TeV" + "_part%(i)02d" % {"i" : (i + 1)},
			replFunction = makeReplacementsHarvesting, replacements = "inputFilePath = " + inputFilePath,
			job = "harvesting", queue = "1nh", outputFilePath = outputFilePath, type = "mc")

submitToBatch(configFile = "harvestZtoElecTauPlots_cfg.py", channel = "ZtoElecTau", sample = "ZtautauPlusJets_7TeV",
              replFunction = makeReplacementsHarvesting, replacements = "inputFilePath = " + inputFilePath,
              job = "harvesting", queue = "1nh", outputFilePath = outputFilePath, type = "mc")
submitToBatch(configFile = "harvestZtoElecTauPlots_cfg.py", channel = "ZtoElecTau", sample = "ZeePlusJets_7TeV",
              replFunction = makeReplacementsHarvesting, replacements = "inputFilePath = " + inputFilePath,
              job = "harvesting", queue = "1nh", outputFilePath = outputFilePath, type = "mc")

# harvest TT + jets
submitToBatch(configFile = "harvestZtoElecTauPlots_cfg.py", channel = "ZtoElecTau", sample = "TTplusJets_7TeV",
              replFunction = makeReplacementsHarvesting, replacements = "inputFilePath = " + inputFilePath,
              job = "harvesting", queue = "1nh", outputFilePath = outputFilePath, type = "mc")

