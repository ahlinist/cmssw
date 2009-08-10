#!/usr/bin/env python
import FWCore.ParameterSet.Config as cms

from TauAnalysis.Configuration.submitToBatch import submitToBatch
from TauAnalysis.Configuration.makeReplacementsHarvesting import makeReplacementsHarvesting

# name of the directory (either on afs area or castor)
# to which all .root files produced by the cmsRun job will be copied
outputDirectory = "/castor/cern.ch/user/g/gfball/tauanalysis/bbAHtoElecTau/"

plotDirectoryName = cms.string("rfio:/castor/cern.ch/user/g/gfball/tauanalysis/bbAHtoElecTau/")
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

samples = [
  #'AH115_tautau',
  #'AH160_tautau',
  'AH115bb_tautau',
  'AH160bb_tautau',
  'Ztautau',
  'Zee',
  #'QCD_EMenriched_Pt20to30',
  #'QCD_EMenriched_Pt30to80',
  #'QCD_EMenriched_Pt80to170',
  #'QCD_BCtoE_Pt20to30',
  #'QCD_BCtoE_Pt30to80',
  #'QCD_BCtoE_Pt80to170',
  'WplusJets',
  'ZplusJets',
  'TTplusJets',
  #'PhotonJets_Pt15to20',
  #'PhotonJets_Pt20to25',
  #'PhotonJets_Pt25to30',
  #'PhotonJets_Pt30to25',
  #'PhotonJets_PtGt35',
  'ZeePlusJets',
  'ZtautauPlusJets'
]

for s in samples:
  submitToBatch(configFile = "harvestbbAHtoElecTauPlots_cfg.py", channel = "bbAHtoElecTau", sample = s, replFunction = makeReplacementsHarvesting, replacements = "", job = "harvesting", queue = "1nh", outputDirectory = outputDirectory)
