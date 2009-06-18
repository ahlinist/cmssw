#!/usr/bin/env python

from TauAnalysis.Configuration.submitToBatch import submitToBatch
from TauAnalysis.Skimming.makeReplacementsSkimByRunEventNumbers import makeReplacementsSkimByRunEventNumbers

# name of the directory (either on afs area or castor)
# to which all .root files produced by the cmsRun job will be copied
outputDirectory = "/castor/cern.ch/user/v/veelken/CMSSW_2_2_3/"

#--------------------------------------------------------------------------------
#
# NOTE: The jobs get submitted to the '1nd' queue,
#       which allows for an execution time of the cmsRun jobs of up to 24 hours
#       (the queues are {'1nh' (1 hour), '1nd' (24 hours) and '1nw' (1 week execution time limit);
#        see https://twiki.cern.ch/twiki/bin/view/CMS/CMSUKCMSSWBatch for details about the CERN batch system)           
#
#--------------------------------------------------------------------------------

# Z --> e e + jets jobs
for i in range(13):
    submitToBatch(configFile = "skimByRunEventNumbers_cfg.py", channel = "ZtoElecTau", sample = "ZeePlusJets_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsSkimByRunEventNumbers,
                  replacements = "maxEvents = -1; runEventNumberFileName = /afs/cern.ch/user/v/veelken/scratch0/CMSSW_2_2_10/src/TauAnalysis/DQMTools/test/selEvents_ZtoElecTau_ZeePlusJets.txt",
                  job = "skimByRunEventNumbers", queue = "1nd", outputDirectory = outputDirectory)
