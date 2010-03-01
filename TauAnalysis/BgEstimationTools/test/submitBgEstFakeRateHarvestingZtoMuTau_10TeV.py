#!/usr/bin/env python

from TauAnalysis.Configuration.submitToBatch import submitToBatch
from TauAnalysis.Configuration.makeReplacementsHarvesting import makeReplacementsHarvesting

# name of the directory (either on afs area or castor)
# to which all .root files produced by the cmsRun job will be copied
#outputFilePath = "/castor/cern.ch/user/v/veelken/CMSSW_3_3_x/bgEstPlots/ZtoMuTau_frCDF/10TeV/"
outputFilePath = "/castor/cern.ch/user/v/veelken/CMSSW_3_3_x/bgEstPlots/ZtoMuTau_frSimple/10TeV/"

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
submitToBatch(configFile = "../../Configuration/test/harvestZtoMuTauPlots_cfg.py", channel = "ZtoMuTau", sample = "Ztautau",
              replFunction = makeReplacementsHarvesting, replacements =
              "inputFilePath = " + inputFilePath + "; recoSampleDefinitionsFile = TauAnalysis.Configuration.plotZtoMuTau_processes_10TeV_cfi",
              job = "frHarvesting", queue = "1nh", outputFilePath = outputFilePath)

# harvest Z --> mu mu
submitToBatch(configFile = "../../Configuration/test/harvestZtoMuTauPlots_cfg.py", channel = "ZtoMuTau", sample = "Zmumu",
              replFunction = makeReplacementsHarvesting, replacements =
              "inputFilePath = " + inputFilePath + "; recoSampleDefinitionsFile = TauAnalysis.Configuration.plotZtoMuTau_processes_10TeV_cfi",
              job = "frHarvesting", queue = "1nh", outputFilePath = outputFilePath)

# harvest InclusivePPmuX
submitToBatch(configFile = "../../Configuration/test/harvestZtoMuTauPlots_cfg.py", channel = "ZtoMuTau", sample = "InclusivePPmuX",
              replFunction = makeReplacementsHarvesting, replacements =
              "inputFilePath = " + inputFilePath + "; recoSampleDefinitionsFile = TauAnalysis.Configuration.plotZtoMuTau_processes_10TeV_cfi",
              job = "frHarvesting", queue = "1nh", outputFilePath = outputFilePath)

# harvest PPmuXptGt20
for i in range(7):
    submitToBatch(configFile = "../../Configuration/test/harvestZtoMuTauPlots_cfg.py", channel = "ZtoMuTau", sample = "PPmuXptGt20_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsHarvesting, replacements =
                  "inputFilePath = " + inputFilePath + "; recoSampleDefinitionsFile = TauAnalysis.Configuration.plotZtoMuTau_processes_10TeV_cfi",
                  job = "frHarvesting", queue = "1nh", outputFilePath = outputFilePath)

# harvest W + jets
submitToBatch(configFile = "../../Configuration/test/harvestZtoMuTauPlots_cfg.py", channel = "ZtoMuTau", sample = "WplusJets",
              replFunction = makeReplacementsHarvesting, replacements =
              "inputFilePath = " + inputFilePath + "; recoSampleDefinitionsFile = TauAnalysis.Configuration.plotZtoMuTau_processes_10TeV_cfi",
              job = "frHarvesting", queue = "1nh", outputFilePath = outputFilePath)

# harvest ttbar + jets
submitToBatch(configFile = "../../Configuration/test/harvestZtoMuTauPlots_cfg.py", channel = "ZtoMuTau", sample = "TTplusJets",
              replFunction = makeReplacementsHarvesting, replacements =
              "inputFilePath = " + inputFilePath + "; recoSampleDefinitionsFile = TauAnalysis.Configuration.plotZtoMuTau_processes_10TeV_cfi",
              job = "frHarvesting", queue = "1nh", outputFilePath = outputFilePath)


