#!/usr/bin/env python

from TauAnalysis.Configuration.submitToBatch import submitToBatch
from TauAnalysis.Configuration.makeReplacementsHarvesting import makeReplacementsHarvesting

# name of the directory (either on afs area or castor)
# to which all .root files produced by the cmsRun job will be copied
outputFilePath = "/castor/cern.ch/user/v/veelken/CMSSW_3_1_2/plots/ZtoElecMu/10TeV/"

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
submitToBatch(configFile = "harvestZtoElecMuPlots_cfg.py", channel = "ZtoElecMu", sample = "Ztautau",
              replFunction = makeReplacementsHarvesting, replacements =
              "inputFilePath = " + inputFilePath + "; recoSampleDefinitionsFile = TauAnalysis.Configuration.plotZtoElecMu_processes_10TeV_cfi",
              job = "harvesting", queue = "1nh", outputFilePath = outputFilePath)

# harvest Z --> mu mu
submitToBatch(configFile = "harvestZtoElecMuPlots_cfg.py", channel = "ZtoElecMu", sample = "Zmumu",
              replFunction = makeReplacementsHarvesting, replacements =
              "inputFilePath = " + inputFilePath + "; recoSampleDefinitionsFile = TauAnalysis.Configuration.plotZtoElecMu_processes_10TeV_cfi",
              job = "harvesting", queue = "1nh", outputFilePath = outputFilePath)

# harvest InclusivePPmuX
submitToBatch(configFile = "harvestZtoElecMuPlots_cfg.py", channel = "ZtoElecMu", sample = "InclusivePPmuX",
              replFunction = makeReplacementsHarvesting, replacements =
              "inputFilePath = " + inputFilePath + "; recoSampleDefinitionsFile = TauAnalysis.Configuration.plotZtoElecMu_processes_10TeV_cfi",
              job = "harvesting", queue = "1nh", outputFilePath = outputFilePath)

# harvest PPmuXptGt20
submitToBatch(configFile = "harvestZtoElecMuPlots_cfg.py", channel = "ZtoElecMu", sample = "PPmuXptGt20",
              replFunction = makeReplacementsHarvesting, replacements =
              "inputFilePath = " + inputFilePath + "; recoSampleDefinitionsFile = TauAnalysis.Configuration.plotZtoElecMu_processes_10TeV_cfi",
              job = "harvesting", queue = "1nh", outputFilePath = outputFilePath)

# harvest W + jets
submitToBatch(configFile = "harvestZtoElecMuPlots_cfg.py", channel = "ZtoElecMu", sample = "WplusJets",
              replFunction = makeReplacementsHarvesting, replacements =
              "inputFilePath = " + inputFilePath + "; recoSampleDefinitionsFile = TauAnalysis.Configuration.plotZtoElecMu_processes_10TeV_cfi",
              job = "harvesting", queue = "1nh", outputFilePath = outputFilePath)

# harvest ttbar + jets
submitToBatch(configFile = "harvestZtoElecMuPlots_cfg.py", channel = "ZtoElecMu", sample = "TTplusJets",
              replFunction = makeReplacementsHarvesting, replacements =
              "inputFilePath = " + inputFilePath + "; recoSampleDefinitionsFile = TauAnalysis.Configuration.plotZtoElecMu_processes_10TeV_cfi",
              job = "harvesting", queue = "1nh", outputFilePath = outputFilePath)


