#!/usr/bin/env python

from TauAnalysis.Configuration.submitToGrid import submitToGrid
from TauAnalysis.Configuration.makeReplacementsCrab import makeReplacementsAnalysis

from TauAnalysis.Configuration.recoSampleDefinitionsZtoMuTau_10TeV_cfi import *

# name of the directory (either on afs area or castor)
# to which all .root files produced by the cmsRun job will be copied
outputFilePath = "/castor/cern.ch/user/v/veelken/plots/ZtoMuTau_grid/"
#outputFilePath = "/castor/cern.ch/user/l/lusito/ZMuTauAnalysis/"

# small cmsRun job for testing purposes...
submitToGrid(configFile = "runZtoMuTau_cfg.py", channel = "ZtoMuTau",
             sample = "Ztautau_10TeV",
             dbs_name = "/Ztautau/sarkar-diTauSkim_CMSSW314_Summer09-MC_31X_V3-v1_RECO-7645e7582a8ca89f38eb471c03cbf450/USER",
             dbs_url = "http://cmsdbsprod.cern.ch/cms_dbs_prod_local_09/servlet/DBSServlet",             
             replFunction = makeReplacementsAnalysis, replacements =
             "maxEvents = 100; eventsPerJob = 20000; applyFactorization = false; estimateSysUncertainties = false",
             outputFileNames = [ plotsOutputFileNameZtoMuTau_Ztautau_10TeV ], outputFilePath = outputFilePath)
