#!/usr/bin/env python

from TauAnalysis.Configuration.submitToGrid import submitToGrid
from TauAnalysis.Configuration.makeReplacementsCrab import makeReplacementsCrab

from TauAnalysis.Configuration.recoSampleDefinitionsWtoTauNu_7TeV_cfi import *

# name of the directory (either on afs area or castor)
# to which all .root files produced by the cmsRun job will be copied
#
# NOTE: outputFilePath must **not** contain "/castor/cern.ch" !!
#       (internally added by crab)
#
outputFilePath = "/user/l/liis/wTauNuPatTuples/spring10"


submitToGrid(configFile = "producePatTuple_cfg.py", channel = "WtoTauNu",
		sample = "Wtaunu_7TeV", job = "spring10_1",
		dbs_name = "/Wtaunu/akalinow-SkimTauTau_356_pass1-67f1fcf093907f0b5dd99ccd3dc76eba-WTauNuSkim/USER",
		dbs_url = "http://cmsdbsprod.cern.ch/cms_dbs_ph_analysis_02/servlet/DBSServlet",             
		replFunction = makeReplacementsCrab, replacements =
		"maxEvents = -1; eventsPerJob = 50000; applyFactorization = false; estimateSysUncertainties = false; globalTag = START3X_V26::All",
		type = "mc",
		outputFileNames = [ patTupleOutputFileNameWtoTauNu_Wtaunu_7TeV ], 
		outputFilePath = outputFilePath, submit = "yes")

submitToGrid(configFile = "producePatTuple_cfg.py", channel = "WtoTauNu",
		sample = "qcd_7TeV", job = "spring10_1",
		dbs_name = "/QCD_Pt15_BiasedTau/akalinow-SkimTauTau_356_pass1-67f1fcf093907f0b5dd99ccd3dc76eba-WTauNuSkim/USER",
		dbs_url = "http://cmsdbsprod.cern.ch/cms_dbs_ph_analysis_02/servlet/DBSServlet",             
		replFunction = makeReplacementsCrab, replacements =
		"maxEvents = -1; eventsPerJob = 50000; applyFactorization = false; estimateSysUncertainties = false; globalTag = START3X_V26::All",
		type = "mc",
		outputFileNames = [ patTupleOutputFileNameWtoTauNu_qcd_7TeV ], 
		outputFilePath = outputFilePath, submit = "yes")

submitToGrid(configFile = "producePatTuple_cfg.py", channel = "WtoTauNu",
		sample = "Wmunu_7TeV", job = "spring10_1",
		dbs_name = "/Wmunu/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO",
		dbs_url = "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",             
		replFunction = makeReplacementsCrab, replacements =
		"maxEvents = 1000000; eventsPerJob = 50000; applyFactorization = false; estimateSysUncertainties = false; globalTag = START3X_V26::All",
		type = "mc",
		outputFileNames = [ patTupleOutputFileNameWtoTauNu_Wmunu_7TeV ], 
		outputFilePath = outputFilePath, submit = "yes") # 

submitToGrid(configFile = "producePatTuple_cfg.py", channel = "WtoTauNu",
             sample = "Wenu_7TeV", job = "spring10_1",
             dbs_name = "/Wenu/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO",
             dbs_url = "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
             replFunction = makeReplacementsCrab, replacements =
             "maxEvents = -1; eventsPerJob = 50000; applyFactorization = false; estimateSysUncertainties = false; globalTag = START3X_V26::All",
             type = "mc",
             outputFileNames = [ patTupleOutputFileNameWtoTauNu_Wenu_7TeV ],
             outputFilePath = outputFilePath, submit = "yes")

submitToGrid(configFile = "producePatTuple_cfg.py", channel = "WtoTauNu",
             sample = "ZplusJets_7TeV", job = "spring10_1", # 
             dbs_name = "/ZJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO",
             dbs_url = "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
             replFunction = makeReplacementsCrab, replacements =
             "maxEvents = -1; eventsPerJob = 5000; applyFactorization = false; estimateSysUncertainties = false; globalTag = START3X_V26::All",
             type = "mc",
             outputFileNames = [ patTupleOutputFileNameWtoTauNu_ZplusJets_7TeV ],
             outputFilePath = outputFilePath, submit = "yes")



