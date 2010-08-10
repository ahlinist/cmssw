#!/usr/bin/env python

from TauAnalysis.Configuration.submitToGrid import submitToGrid
from TauAnalysis.Configuration.makeReplacementsCrab import makeReplacementsCrab

from TauAnalysis.Configuration.recoSampleDefinitionsZtoMuTau_10TeV_cfi import *

# name of the directory (either on afs area or castor)
# to which all .root files produced by the cmsRun job will be copied
#
# NOTE: outputFilePath must **not** contain "/castor/cern.ch" !!
#       (internally added by crab)
#
outputFilePath = "/user/f/friis/ZtoMuTau_grid/"
#outputFilePath = "/user/l/lusito/ZMuTauAnalysis/"

# small cmsRun job for testing purposes...
submitToGrid(configFile = "runZtoMuTau_cfg.py", channel = "ZtoMuTau",
             sample = "Ztautau_10TeV", job = "03092010", type='mc',
             dbs_name = "/Ztautau/sarkar-diTauSkim_CMSSW314_Summer09-MC_31X_V3-v1_RECO-7645e7582a8ca89f38eb471c03cbf450/USER",
             dbs_url = "http://cmsdbsprod.cern.ch/cms_dbs_prod_local_09/servlet/DBSServlet",             
             replFunction = makeReplacementsCrab, replacements =
             "maxEvents = 100; eventsPerJob = 20000; applyFactorization = false; estimateSysUncertainties = false",
             outputFileNames = [ plotsOutputFileNameZtoMuTau_Ztautau_10TeV ], outputFilePath = outputFilePath)

#submitToGrid(configFile = "producePatTuple_cfg.py", channel = "ZtoMuTau",
#             sample = "Ztautau_10TeV", job = "03092010", type='mc',
#             dbs_name = "/Ztautau/sarkar-diTauSkim_CMSSW314_Summer09-MC_31X_V3-v1_RECO-7645e7582a8ca89f38eb471c03cbf450/USER",
#             dbs_url = "http://cmsdbsprod.cern.ch/cms_dbs_prod_local_09/servlet/DBSServlet",             
#             replFunction = makeReplacementsCrab, replacements =
#             "maxEvents = 100; eventsPerJob = 20000; applyFactorization = false; estimateSysUncertainties = false",
#             outputFileNames = [ patTupleOutputFileNameZtoMuTau_Ztautau_10TeV ], outputFilePath = outputFilePath)
