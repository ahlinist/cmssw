#!/usr/bin/env python

from TauAnalysis.Configuration.submitToBatch import submitToBatch
from TauAnalysis.Configuration.makeReplacementsHarvesting import makeReplacementsHarvesting

# name of the directory (either on afs area or castor) to which all .root files produced by the Harvesting job will be copied
#outputDirectory = "/afs/cern.ch/user/l/liis/scratch0/CMSSW_2_2_13/src/TauAnalysis/Configuration/test/TESTING"
outputDirectory = "/castor/cern.ch/user/l/liis/WTauNuPlots/"


#--------------------------------------------------------------------------------
# Add histograms, numbers in FilterStatisticsTables and run + event numbers
# stored as DQM MonitorElements in different ROOT files
#
# NOTE: The jobs get submitted to the '1nh' queue,
#       which allows for an execution time of the cmsRun jobs of up to 1 hour
#       (the queues are {'1nh' (1 hour), '1nd' (24 hours) and '1nw' (1 week execution time limit);
#        see https://twiki.cern.ch/twiki/bin/view/CMS/CMSUKCMSSWBatch for details about the CERN batch system)           
#--------------------------------------------------------------------------------

# harvest W --> tau nu
submitToBatch(configFile = "harvestWtoTauNuPlots_cfg.py", channel = "WtoTauNu", sample = "Wtaunu",
              replFunction = makeReplacementsHarvesting, replacements = "",
              job = "harvesting", queue = "1nh", outputDirectory = outputDirectory)

# harvest qcd
submitToBatch(configFile = "harvestWtoTauNuPlots_cfg.py", channel = "WtoTauNu", sample = "qcd_W",
              replFunction = makeReplacementsHarvesting, replacements = "",
              job = "harvesting", queue = "1nh", outputDirectory = outputDirectory)

# harvest W --> mu nu
submitToBatch(configFile = "harvestWtoTauNuPlots_cfg.py", channel = "WtoTauNu", sample = "Wmunu",
              replFunction = makeReplacementsHarvesting, replacements = "",
              job = "harvesting", queue = "1nh", outputDirectory = outputDirectory)

# harvest W --> e nu
submitToBatch(configFile = "harvestWtoTauNuPlots_cfg.py", channel = "WtoTauNu", sample = "Wenu",
              replFunction = makeReplacementsHarvesting, replacements = "",
              job = "harvesting", queue = "1nh", outputDirectory = outputDirectory)

# harvest Zjets
submitToBatch(configFile = "harvestWtoTauNuPlots_cfg.py", channel = "WtoTauNu", sample = "ZplusJets",
              replFunction = makeReplacementsHarvesting, replacements = "",
              job = "harvesting", queue = "1nh", outputDirectory = outputDirectory)

#harvest Ttbar
submitToBatch(configFile = "harvestWtoTauNuPlots_cfg.py", channel = "WtoTauNu", sample = "TTplusJets",
              replFunction = makeReplacementsHarvesting, replacements = "",
              job = "harvesting", queue = "1nh", outputDirectory = outputDirectory)

