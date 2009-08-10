#!/usr/bin/env python

from TauAnalysis.Configuration.submitToBatch import submitToBatch
from TauAnalysis.Configuration.makeReplacementsAnalysis import makeReplacementsAnalysis

# name of the directory (either on afs area or castor)
# to which all .root files produced by the cmsRun job will be copied
outputDirectory = "/castor/cern.ch/user/g/gfball/tauanalysis/bbAHtoElecTau/"


# small cmsRun job for testing purposes...
#submitToBatch(configFile = "runbbAHtoElecTau_cfg.py", channel = "bbAHtoElecTau", sample = "Ztautau",
#              replFunction = makeReplacementsAnalysis, replacements = "maxEvents = 100; applyFactorization = false",
#              job = "analysis", queue = "1nh", outputDirectory = outputDirectory)

#--------------------------------------------------------------------------------
#
# Monte Carlo samples from Summer'08 production
# reprocessed with CMSSW_2_2_3, skimmed by Sunil and Giuseppe
#
# NOTE: The jobs get submitted to the '1nd' queue,
#       which allows for an execution time of the cmsRun jobs of up to 24 hours
#       (the queues are {'1nh' (1 hour), '1nd' (24 hours) and '1nw' (1 week execution time limit);
#        see https://twiki.cern.ch/twiki/bin/view/CMS/CMSUKCMSSWBatch for details about the CERN batch system)           
#
#--------------------------------------------------------------------------------
#submitToBatch(configFile = "runbbAHtoElecTau_cfg.py", channel = "bbAHtoElecTau", sample = 'Ztautau_part01',                  replFunction = makeReplacementsAnalysis, replacements = "maxEvents = 100; applyFactorization = false",job = "analysis", queue = "1nh", outputDirectory = outputDirectory)


samples = [
  ('AH115bb_tautau',7),
  ('AH160bb_tautau',7),
  ('Ztautau',5),
  ('Zee',28),
#  ('QCD_EMenriched_Pt20to30',16),
#  ('QCD_EMenriched_Pt30to80',83),
#  ('QCD_EMenriched_Pt80to170',30),
#  ('QCD_BCtoE_Pt20to30',24),
#  ('QCD_BCtoE_Pt30to80',27),
#  ('QCD_BCtoE_Pt80to170',15),
  ('WplusJets',18),
  ('ZplusJets',13),
  ('TTplusJets',18),
#  ('PhotonJets_Pt15to20',-1),
#  ('PhotonJets_Pt20to25',-1),
#  ('PhotonJets_Pt25to30',-1),
#  ('PhotonJets_Pt30to25',-1),
#  ('PhotonJets_PtGt35',-1)
  ('ZeePlusJets',13),
  ('ZtautauPlusJets',13),
]

for s in samples:
  if len(s)==2:
    s = (s[0],s[1],-1)
  if s[1]==-1:
    submitToBatch(configFile = "runbbAHtoElecTau_cfg.py", channel = "bbAHtoElecTau", sample = s[0],
                  replFunction = makeReplacementsAnalysis, replacements = "maxEvents = %d; applyFactorization = false"%s[2],
                  job = "analysis", queue = "8nh", outputDirectory = outputDirectory)
  else:
    for p in range(1,s[1]+1):
      submitToBatch(configFile = "runbbAHtoElecTau_cfg.py", channel = "bbAHtoElecTau", sample = "%s_part%02d"%(s[0],p),
                  replFunction = makeReplacementsAnalysis, replacements = "maxEvents = %d; applyFactorization = false"%s[2],
                  job = "analysis", queue = "8nh", outputDirectory = outputDirectory)

