#!/usr/bin/env python

from TauAnalysis.Configuration.submitToBatch import submitToBatch
from TauAnalysis.Skimming.tools.makeReplacementsSkimByRunEventNumbers import makeReplacementsSkimByRunEventNumbers

# name of the directory (either on afs area or castor)
# to which all .root files produced by the cmsRun job will be copied
outputFilePath = "/castor/cern.ch/user/v/veelken/CMSSW_3_1_2/"

#--------------------------------------------------------------------------------
#
# NOTE: The jobs get submitted to the '1nd' queue,
#       which allows for an execution time of the cmsRun jobs of up to 24 hours
#       (the queues are {'1nh' (1 hour), '1nd' (24 hours) and '1nw' (1 week execution time limit);
#        see https://twiki.cern.ch/twiki/bin/view/CMS/CMSUKCMSSWBatch for details about the CERN batch system)           
#
#--------------------------------------------------------------------------------

# pp --> mu X QCD jobs
for i in range(21):
    submitToBatch(configFile = "skimByRunEventNumbers_cfg.py", channel = "ZtoMuTau",
                  sample = "InclusivePPmuX_10TeV_part%(i)02d" % {"i" : (i + 1)},
                  replFunction = makeReplacementsSkimByRunEventNumbers, replacements =
                  "maxEvents = -1; recoSampleDefinitionsFileName = recoSampleDefinitionsZtoMuTau_10TeV_cfi; runEventNumberFileName = /afs/cern.ch/user/v/veelken/scratch0/CMSSW_3_1_4/src/TauAnalysis/DQMTools/test/selEvents_ZtoTauNu_InclusivePPmuX_passed.txt",
                  job = "skimByRunEventNumbers", queue = "1nd", outputFilePath = outputFilePath)
