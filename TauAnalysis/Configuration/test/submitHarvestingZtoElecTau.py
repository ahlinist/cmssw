#!/usr/bin/env python

from TauAnalysis.Configuration.submitToBatch import submitToBatch
from TauAnalysis.Configuration.makeReplacementsHarvesting import makeReplacementsHarvesting

# name of the directory (either on afs area or castor)
# to which all .root files produced by the cmsRun job will be copied
outputDirectory = "/castor/cern.ch/user/j/jkolb/elecTauAnalysis/hists_factorized/"

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
              replFunction = makeReplacementsHarvesting, replacements = "",
              job = "harvesting", queue = "1nh", outputDirectory = outputDirectory)

# harvest Z --> e e
submitToBatch(configFile = "harvestZtoElecTauPlots_cfg.py", channel = "ZtoElecTau", sample = "Zee",
              replFunction = makeReplacementsHarvesting, replacements = "",
              job = "harvesting", queue = "1nh", outputDirectory = outputDirectory)

# harvest QCD_BCtoE 
submitToBatch(configFile = "harvestZtoElecTauPlots_cfg.py", channel = "ZtoElecTau", sample = "QCD_BCtoE_Pt20to30",
              replFunction = makeReplacementsHarvesting, replacements = "",
              job = "harvesting", queue = "1nh", outputDirectory = outputDirectory)
submitToBatch(configFile = "harvestZtoElecTauPlots_cfg.py", channel = "ZtoElecTau", sample = "QCD_BCtoE_Pt30to80",
              replFunction = makeReplacementsHarvesting, replacements = "",
              job = "harvesting", queue = "1nh", outputDirectory = outputDirectory)
submitToBatch(configFile = "harvestZtoElecTauPlots_cfg.py", channel = "ZtoElecTau", sample = "QCD_BCtoE_Pt80to170",
              replFunction = makeReplacementsHarvesting, replacements = "",
              job = "harvesting", queue = "1nh", outputDirectory = outputDirectory)

# harvest QCD_EMenriched
submitToBatch(configFile = "harvestZtoElecTauPlots_cfg.py", channel = "ZtoElecTau", sample = "QCD_EMenriched_Pt20to30",
              replFunction = makeReplacementsHarvesting, replacements = "",
              job = "harvesting", queue = "1nh", outputDirectory = outputDirectory)
submitToBatch(configFile = "harvestZtoElecTauPlots_cfg.py", channel = "ZtoElecTau", sample = "QCD_EMenriched_Pt30to80",
              replFunction = makeReplacementsHarvesting, replacements = "",
              job = "harvesting", queue = "1nh", outputDirectory = outputDirectory)
submitToBatch(configFile = "harvestZtoElecTauPlots_cfg.py", channel = "ZtoElecTau", sample = "QCD_EMenriched_Pt80to170",
              replFunction = makeReplacementsHarvesting, replacements = "",
              job = "harvesting", queue = "1nh", outputDirectory = outputDirectory)

# harvest W/Z + jets
submitToBatch(configFile = "harvestZtoElecTauPlots_cfg.py", channel = "ZtoElecTau", sample = "WplusJets",
              replFunction = makeReplacementsHarvesting, replacements = "",
              job = "harvesting", queue = "1nh", outputDirectory = outputDirectory)

submitToBatch(configFile = "harvestZtoElecTauPlots_cfg.py", channel = "ZtoElecTau", sample = "ZtautauPlusJets",
              replFunction = makeReplacementsHarvesting, replacements = "",
              job = "harvesting", queue = "1nh", outputDirectory = outputDirectory)
submitToBatch(configFile = "harvestZtoElecTauPlots_cfg.py", channel = "ZtoElecTau", sample = "ZeePlusJets",
              replFunction = makeReplacementsHarvesting, replacements = "",
              job = "harvesting", queue = "1nh", outputDirectory = outputDirectory)

# harvest TT + jets
submitToBatch(configFile = "harvestZtoElecTauPlots_cfg.py", channel = "ZtoElecTau", sample = "TTplusJets",
              replFunction = makeReplacementsHarvesting, replacements = "",
              job = "harvesting", queue = "1nh", outputDirectory = outputDirectory)
