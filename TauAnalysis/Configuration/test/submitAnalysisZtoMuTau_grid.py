#!/usr/bin/env python

from TauAnalysis.Configuration.submitToGrid import submitToGrid
from TauAnalysis.Configuration.makeReplacementsCrab import makeReplacementsAnalysis

from TauAnalysis.Configuration.recoSampleDefinitionsZtoMuTau_cfi import *

# name of the directory (either on afs area or castor)
# to which all .root files produced by the cmsRun job will be copied
outputDirectory = "/castor/cern.ch/user/v/veelken/plots/ZtoMuTau_grid/"
#outputDirectory = "/castor/cern.ch/user/l/lusito/ZMuTauAnalysis/"

# small cmsRun job for testing purposes...
submitToGrid(configFile = "runZtoElecMu_cfg.py", channel = "ZtoElecMu", sample = "Ztautau",
             dbs_name = "/Ztautau/sunil-Summer08_Skim-411de0fd9a45f2e9090c88c36bde639d/USER",
             dbs_url = "http://cmsdbsprod.cern.ch/cms_dbs_prod_local_09/servlet/DBSServlet",             
             replFunction = makeReplacementsAnalysis, replacements = "maxEvents = 100; eventsPerJob = 20000; applyFactorization = false",
             outputFiles = [ plotsOutputFileNameZtautau ], outputDirectory = outputDirectory)
