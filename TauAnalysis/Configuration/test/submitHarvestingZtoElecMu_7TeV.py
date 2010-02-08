#!/usr/bin/env python

from TauAnalysis.Configuration.submitToBatch import submitToBatch
from TauAnalysis.Configuration.makeReplacementsHarvesting import makeReplacementsHarvesting

# name of the directory (either on afs area or castor)
# to which all .root files produced by the cmsRun job will be copied
outputFilePath = "/castor/cern.ch/user/v/veelken/CMSSW_3_1_2/plots/ZtoElecMu/7TeV/"

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
submitToBatch(configFile = "harvestZtoElecMuPlots_cfg.py", channel = "ZtoElecMu", sample = "Ztautau_7TeV",
              replFunction = makeReplacementsHarvesting, replacements =
              "inputFilePath = " + inputFilePath + "; recoSampleDefinitionsFile = TauAnalysis.Configuration.plotZtoElecMu_processes_7TeV_cfi",
              job = "harvesting", queue = "1nh", outputFilePath = outputFilePath)

# harvest Z --> mu mu
submitToBatch(configFile = "harvestZtoElecMuPlots_cfg.py", channel = "ZtoElecMu", sample = "Zmumu_7TeV",
              replFunction = makeReplacementsHarvesting, replacements =
              "inputFilePath = " + inputFilePath + "; recoSampleDefinitionsFile = TauAnalysis.Configuration.plotZtoElecMu_processes_7TeV_cfi",
              job = "harvesting", queue = "1nh", outputFilePath = outputFilePath)

# harvest InclusivePPmuX
submitToBatch(configFile = "harvestZtoElecMuPlots_cfg.py", channel = "ZtoElecMu", sample = "InclusivePPmuX_7TeV",
              replFunction = makeReplacementsHarvesting, replacements =
              "inputFilePath = " + inputFilePath + "; recoSampleDefinitionsFile = TauAnalysis.Configuration.plotZtoElecMu_processes_7TeV_cfi",
              job = "harvesting", queue = "1nh", outputFilePath = outputFilePath)

## harvest PPmuXptGt20
##    
## CV: not yet skimmed (as of 2010/02/07)
##   
##submitToBatch(configFile = "harvestZtoElecMuPlots_cfg.py", channel = "ZtoElecMu", sample = "PPmuXptGt20_7TeV",
##              replFunction = makeReplacementsHarvesting, replacements =
##              "inputFilePath = " + inputFilePath + "; recoSampleDefinitionsFile = TauAnalysis.Configuration.plotZtoElecMu_processes_7TeV_cfi",
##              job = "harvesting", queue = "1nh", outputFilePath = outputFilePath)

# harvest W + jets
submitToBatch(configFile = "harvestZtoElecMuPlots_cfg.py", channel = "ZtoElecMu", sample = "WplusJets_7TeV",
              replFunction = makeReplacementsHarvesting, replacements =
              "inputFilePath = " + inputFilePath + "; recoSampleDefinitionsFile = TauAnalysis.Configuration.plotZtoElecMu_processes_7TeV_cfi",
              job = "harvesting", queue = "1nh", outputFilePath = outputFilePath)

# harvest ttbar + jets
submitToBatch(configFile = "harvestZtoElecMuPlots_cfg.py", channel = "ZtoElecMu", sample = "TTplusJets_7TeV",
              replFunction = makeReplacementsHarvesting, replacements =
              "inputFilePath = " + inputFilePath + "; recoSampleDefinitionsFile = TauAnalysis.Configuration.plotZtoElecMu_processes_7TeV_cfi",
              job = "harvesting", queue = "1nh", outputFilePath = outputFilePath)


