#!/usr/bin/env python

from TauAnalysis.Configuration.submitToBatch import submitToBatch
from TauAnalysis.Configuration.makeReplacementsHarvesting import makeReplacementsHarvesting

# name of the directory (either on afs area or castor)
# to which all .root files produced by the cmsRun job will be copied
outputFilePath = "/castor/cern.ch/user/v/veelken/plots/ZtoElecTau/"

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

# harvest Z --> tau tau 
submitToBatch(configFile = "harvestZtoElecTauPlots_cfg.py", channel = "ZtoElecTau", sample = "Ztautau",
              replFunction = makeReplacementsHarvesting, replacements = "inputFilePath = " + inputFilePath,
              job = "harvesting", queue = "1nh", outputFilePath = outputFilePath)

# harvest Z --> e e
submitToBatch(configFile = "harvestZtoElecTauPlots_cfg.py", channel = "ZtoElecTau", sample = "Zee",
              replFunction = makeReplacementsHarvesting, replacements = "inputFilePath = " + inputFilePath,
              job = "harvesting", queue = "1nh", outputFilePath = outputFilePath)

# harvest QCD_BCtoE 
submitToBatch(configFile = "harvestZtoElecTauPlots_cfg.py", channel = "ZtoElecTau", sample = "QCD_BCtoE_Pt20to30",
              replFunction = makeReplacementsHarvesting, replacements = "inputFilePath = " + inputFilePath,
              job = "harvesting", queue = "1nh", outputFilePath = outputFilePath)
submitToBatch(configFile = "harvestZtoElecTauPlots_cfg.py", channel = "ZtoElecTau", sample = "QCD_BCtoE_Pt30to80",
              replFunction = makeReplacementsHarvesting, replacements = "inputFilePath = " + inputFilePath,
              job = "harvesting", queue = "1nh", outputFilePath = outputFilePath)
submitToBatch(configFile = "harvestZtoElecTauPlots_cfg.py", channel = "ZtoElecTau", sample = "QCD_BCtoE_Pt80to170",
              replFunction = makeReplacementsHarvesting, replacements = "inputFilePath = " + inputFilePath,
              job = "harvesting", queue = "1nh", outputFilePath = outputFilePath)

# harvest QCD_EMenriched
submitToBatch(configFile = "harvestZtoElecTauPlots_cfg.py", channel = "ZtoElecTau", sample = "QCD_EMenriched_Pt20to30",
              replFunction = makeReplacementsHarvesting, replacements = "inputFilePath = " + inputFilePath,
              job = "harvesting", queue = "1nh", outputFilePath = outputFilePath)
for i in range(2):
    submitToBatch(configFile = "harvestZtoElecTauPlots_cfg.py", channel = "ZtoElecTau", sample = "QCD_EMenriched_Pt30to80_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsHarvesting, replacements = "inputFilePath = " + inputFilePath,
                  job = "harvesting", queue = "1nh", outputFilePath = outputFilePath)
submitToBatch(configFile = "harvestZtoElecTauPlots_cfg.py", channel = "ZtoElecTau", sample = "QCD_EMenriched_Pt80to170",
              replFunction = makeReplacementsHarvesting, replacements = "inputFilePath = " + inputFilePath,
              job = "harvesting", queue = "1nh", outputFilePath = outputFilePath)

# harvest W/Z + jets
submitToBatch(configFile = "harvestZtoElecTauPlots_cfg.py", channel = "ZtoElecTau", sample = "WplusJets",
              replFunction = makeReplacementsHarvesting, replacements = "inputFilePath = " + inputFilePath,
              job = "harvesting", queue = "1nh", outputFilePath = outputFilePath)

submitToBatch(configFile = "harvestZtoElecTauPlots_cfg.py", channel = "ZtoElecTau", sample = "ZtautauPlusJets",
              replFunction = makeReplacementsHarvesting, replacements = "inputFilePath = " + inputFilePath,
              job = "harvesting", queue = "1nh", outputFilePath = outputFilePath)
submitToBatch(configFile = "harvestZtoElecTauPlots_cfg.py", channel = "ZtoElecTau", sample = "ZeePlusJets",
              replFunction = makeReplacementsHarvesting, replacements = "inputFilePath = " + inputFilePath,
              job = "harvesting", queue = "1nh", outputFilePath = outputFilePath)

# harvest TT + jets
submitToBatch(configFile = "harvestZtoElecTauPlots_cfg.py", channel = "ZtoElecTau", sample = "TTplusJets",
              replFunction = makeReplacementsHarvesting, replacements = "inputFilePath = " + inputFilePath,
              job = "harvesting", queue = "1nh", outputFilePath = outputFilePath)
