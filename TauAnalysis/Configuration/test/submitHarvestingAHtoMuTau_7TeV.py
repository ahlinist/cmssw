#!/usr/bin/env python

from TauAnalysis.Configuration.submitToBatch import submitToBatch
from TauAnalysis.Configuration.makeReplacementsHarvesting import makeReplacementsHarvesting

# name of the directory (either on afs area or castor)
# to which all .root files produced by the cmsRun job will be copied
outputFilePath = "/castor/cern.ch/user/v/veelken/CMSSW_3_3_x/plots/AHtoMuTau/7TeVrelIsoV/"

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

# harvest MSSM Higgs A/H --> tau+ tau-
submitToBatch(configFile = "harvestZtoMuTauPlots_cfg.py", channel = "AHtoMuTau", sample = "AH120_tautau",
              replFunction = makeReplacementsHarvesting, replacements =
              "inputFilePath = " + inputFilePath + "; recoSampleDefinitionsFile = TauAnalysis.Configuration.plotAHtoMuTau_processes_7TeV_cfi",
              job = "harvesting", queue = "1nd", outputFilePath = outputFilePath)
submitToBatch(configFile = "harvestZtoMuTauPlots_cfg.py", channel = "AHtoMuTau", sample = "AHbb120_tautau",
              replFunction = makeReplacementsHarvesting, replacements =
              "inputFilePath = " + inputFilePath + "; recoSampleDefinitionsFile = TauAnalysis.Configuration.plotAHtoMuTau_processes_7TeV_cfi",
              job = "harvesting", queue = "1nd", outputFilePath = outputFilePath)

# harvest Z --> tau+ tau-
submitToBatch(configFile = "harvestZtoMuTauPlots_cfg.py", channel = "AHtoMuTau", sample = "Ztautau",
              replFunction = makeReplacementsHarvesting, replacements =
              "inputFilePath = " + inputFilePath + "; recoSampleDefinitionsFile = TauAnalysis.Configuration.plotAHtoMuTau_processes_7TeV_cfi",
              job = "harvesting", queue = "1nd", outputFilePath = outputFilePath)

# harvest Z --> mu+ mu-
submitToBatch(configFile = "harvestZtoMuTauPlots_cfg.py", channel = "AHtoMuTau", sample = "Zmumu",
              replFunction = makeReplacementsHarvesting, replacements =
              "inputFilePath = " + inputFilePath + "; recoSampleDefinitionsFile = TauAnalysis.Configuration.plotAHtoMuTau_processes_7TeV_cfi",
              job = "harvesting", queue = "1nd", outputFilePath = outputFilePath)

# harvest Z + jets
submitToBatch(configFile = "harvestZtoMuTauPlots_cfg.py", channel = "AHtoMuTau", sample = "ZplusJets",
              replFunction = makeReplacementsHarvesting, replacements =
              "inputFilePath = " + inputFilePath + "; recoSampleDefinitionsFile = TauAnalysis.Configuration.plotAHtoMuTau_processes_7TeV_cfi",
              job = "harvesting", queue = "1nd", outputFilePath = outputFilePath)

# harvest W + jets
submitToBatch(configFile = "harvestZtoMuTauPlots_cfg.py", channel = "AHtoMuTau", sample = "WplusJets",
              replFunction = makeReplacementsHarvesting, replacements =
              "inputFilePath = " + inputFilePath + "; recoSampleDefinitionsFile = TauAnalysis.Configuration.plotAHtoMuTau_processes_7TeV_cfi",
              job = "harvesting", queue = "1nd", outputFilePath = outputFilePath)

# harvest W/Z + c cbar/b bbar
submitToBatch(configFile = "harvestZtoMuTauPlots_cfg.py", channel = "AHtoMuTau", sample = "Vqq",
              replFunction = makeReplacementsHarvesting, replacements =
              "inputFilePath = " + inputFilePath + "; recoSampleDefinitionsFile = TauAnalysis.Configuration.plotAHtoMuTau_processes_7TeV_cfi",
              job = "harvesting", queue = "1nd", outputFilePath = outputFilePath)

# harvest ttbar + jets
submitToBatch(configFile = "harvestZtoMuTauPlots_cfg.py", channel = "AHtoMuTau", sample = "TTplusJets",
              replFunction = makeReplacementsHarvesting, replacements =
              "inputFilePath = " + inputFilePath + "; recoSampleDefinitionsFile = TauAnalysis.Configuration.plotAHtoMuTau_processes_7TeV_cfi",
              job = "harvesting", queue = "1nd", outputFilePath = outputFilePath)

# harvest InclusivePPmuX
submitToBatch(configFile = "harvestZtoMuTauPlots_cfg.py", channel = "AHtoMuTau", sample = "InclusivePPmuX",
              replFunction = makeReplacementsHarvesting, replacements =
              "inputFilePath = " + inputFilePath + "; recoSampleDefinitionsFile = TauAnalysis.Configuration.plotAHtoMuTau_processes_7TeV_cfi",
              job = "harvesting", queue = "1nd", outputFilePath = outputFilePath)

# harvest PPmuXptGt20
for i in range(3):
    submitToBatch(configFile = "harvestZtoMuTauPlots_cfg.py", channel = "AHtoMuTau", sample = "PPmuXptGt20_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsHarvesting, replacements =
                  "inputFilePath = " + inputFilePath + "; recoSampleDefinitionsFile = TauAnalysis.Configuration.plotAHtoMuTau_processes_7TeV_cfi",
                  job = "harvesting", queue = "1nd", outputFilePath = outputFilePath)

