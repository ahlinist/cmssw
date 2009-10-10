#!/usr/bin/env python

from TauAnalysis.Configuration.submitToGrid import submitToGrid
from TauAnalysis.Configuration.makeReplacementsAnalysis import makeReplacementsAnalysis

from TauAnalysis.Configuration.recoSampleDefinitionsZtoMuTau_cfi import *

# name of the directory (either on afs area or castor)
# to which all .root files produced by the cmsRun job will be copied
outputDirectory = "/castor/cern.ch/user/v/veelken/plots/ZtoMuTau/"
#outputDirectory = "/castor/cern.ch/user/l/lusito/ZMuTauAnalysis/"

# small cmsRun job for testing purposes...
submitToGrid(configFile = "runZtoMuTau_cfg.py", channel = "ZtoMuTau", sample = "Ztautau",
             dbs_name = "/Ztautau/lusito-CMSSW_2_2_3_SkimMuTauZTT02_GEN_SIM_DIGI_L1_DIGI2RAW_HLT-5c432a197f799755d712fe824b01cf85/USER",
             dbs_url = "https://cmsdbsprod.cern.ch:8443/cms_dbs_prod_local_09_writer/servlet/DBSServlet",
             replFunction = makeReplacementsAnalysis, replacements = "maxEvents = 100; eventsPerJob = 20000; applyFactorization = false",
             outputFiles = [ plotsOutputFileNameZtautau ], outputDirectory = outputDirectory)
