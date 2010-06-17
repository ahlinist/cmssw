#!/usr/bin/env python

from TauAnalysis.Configuration.submitToGrid import submitToGrid
from TauAnalysis.Configuration.makeReplacementsCrab import makeReplacementsCrab

from TauAnalysis.Configuration.recoSampleDefinitionsZtoElecTau_cfi import *

# name of the directory (either on afs area or castor)
# to which all .root files produced by the cmsRun job will be copied
#
# NOTE: outputFilePath must **not** contain "/castor/cern.ch" !!
#       (internally added by crab)
#
outputFilePath = "/user/j/jkolb/elecTauPatTuples/spring10/"


submitToGrid(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau",
		sample = "Ztautau_7TeV", job = "Ztautau_spring10_3",
		dbs_name = "/Ztautau/akalinow-SkimTauTau_356_pass1-0a3d3891f015a95324f94837322fb8aa-elecTauSkim/USER",
		dbs_url = "http://cmsdbsprod.cern.ch/cms_dbs_ph_analysis_02/servlet/DBSServlet",             
		replFunction = makeReplacementsCrab, replacements =
		"maxEvents = -1; eventsPerJob = 21000; applyFactorization = false; estimateSysUncertainties = false; globalTag = START3X_V26::All",
		type = "mc",
		outputFileNames = [ patTupleOutputFileNameZtoElecTau_Ztautau_7TeV ], 
		outputFilePath = outputFilePath, submit = "yes")

submitToGrid(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau",
		sample = "Zee_7TeV", job = "Zee_spring10_1",
		dbs_name = "/Zee/akalinow-SkimTauTau_356_pass1-0a3d3891f015a95324f94837322fb8aa-elecTauSkim/USER",
		dbs_url = "http://cmsdbsprod.cern.ch/cms_dbs_ph_analysis_02/servlet/DBSServlet",             
		replFunction = makeReplacementsCrab, replacements =
		"maxEvents = -1; eventsPerJob = 20000; applyFactorization = false; estimateSysUncertainties = false; globalTag = START3X_V26::All",
		type = "mc",
		outputFileNames = [ patTupleOutputFileNameZtoElecTau_Zee_7TeV ], outputFilePath = outputFilePath, submit = "yes")

submitToGrid(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau",
		sample = "WplusJets_7TeV", job = "WplusJets_spring10_0",
		dbs_name = "/Wtaunu/akalinow-SkimTauTau_356_pass1-0a3d3891f015a95324f94837322fb8aa-elecTauSkim/USER",
		dbs_url = "http://cmsdbsprod.cern.ch/cms_dbs_ph_analysis_02/servlet/DBSServlet",             
		replFunction = makeReplacementsCrab, replacements =
		"maxEvents = -1; eventsPerJob = 20000; applyFactorization = false; estimateSysUncertainties = false; globalTag = START3X_V26::All",
		type = "mc",
		outputFileNames = [ patTupleOutputFileNameZtoElecTau_WplusJets_7TeV ], outputFilePath = outputFilePath, submit = "yes")

submitToGrid(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau",
		sample = "TTplusJets_7TeV", job = "TTplusJets_spring10_0",
		dbs_name = "/TTbarJets_Tauola-madgraph/akalinow-SkimTauTau_356_pass1-0a3d3891f015a95324f94837322fb8aa-elecTauSkim/USER",
		dbs_url = "http://cmsdbsprod.cern.ch/cms_dbs_ph_analysis_02/servlet/DBSServlet",             
		replFunction = makeReplacementsCrab, replacements =
		"maxEvents = -1; eventsPerJob = 20000; applyFactorization = false; estimateSysUncertainties = false; globalTag = START3X_V26::All",
		type = "mc",
		outputFileNames = [ patTupleOutputFileNameZtoElecTau_TTplusJets_7TeV ], outputFilePath = outputFilePath, submit = "yes")

submitToGrid(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau",
		sample = "ZeePlusJets_7TeV", job = "ZeePlusJets_spring10_0",
		dbs_name = "/ZJets-madgraph/akalinow-SkimTauTau_356_pass1-0a3d3891f015a95324f94837322fb8aa-elecTauSkim/USER",
		dbs_url = "http://cmsdbsprod.cern.ch/cms_dbs_ph_analysis_02/servlet/DBSServlet",             
		replFunction = makeReplacementsCrab, replacements =
		"maxEvents = -1; eventsPerJob = 20000; applyFactorization = false; estimateSysUncertainties = false; globalTag = START3X_V26::All",
		type = "mc",
		outputFileNames = [ patTupleOutputFileNameZtoElecTau_ZeePlusJets_7TeV ], outputFilePath = outputFilePath, submit = "yes")

submitToGrid(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau",
		sample = "ZtautauPlusJets_7TeV", job = "ZtautauPlusJets_spring10_0",
		dbs_name = "/ZJets-madgraph/akalinow-SkimTauTau_356_pass1-0a3d3891f015a95324f94837322fb8aa-elecTauSkim/USER",
		dbs_url = "http://cmsdbsprod.cern.ch/cms_dbs_ph_analysis_02/servlet/DBSServlet",             
		replFunction = makeReplacementsCrab, replacements =
		"maxEvents = -1; eventsPerJob = 20000; applyFactorization = false; estimateSysUncertainties = false; globalTag = START3X_V26::All",
		type = "mc",
		outputFileNames = [ patTupleOutputFileNameZtoElecTau_ZtautauPlusJets_7TeV ], outputFilePath = outputFilePath, submit = "yes")

submitToGrid(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau",
		sample = "QCD_EMenriched_Pt20to30_7TeV", job = "QCD_EMenriched_Pt20to30_spring10_0",
		dbs_name = "/QCD_EMEnriched_Pt20to30/akalinow-SkimTauTau_356_pass1-af7269240adb80f379a817aa7b511727-elecTauSkim/USER",
		dbs_url = "http://cmsdbsprod.cern.ch/cms_dbs_ph_analysis_02/servlet/DBSServlet",             
		replFunction = makeReplacementsCrab, replacements =
		"maxEvents = -1; eventsPerJob = 200000; applyFactorization = true; estimateSysUncertainties = false; globalTag = START3X_V26::All",
		type = "mc",
		outputFileNames = [ patTupleOutputFileNameZtoElecTau_QCD_EMenriched_Pt20to30_7TeV ], outputFilePath = outputFilePath, submit = "yes")

submitToGrid(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau",
		sample = "QCD_EMenriched_Pt30to80_7TeV", job = "QCD_EMenriched_Pt30to80_spring10_0",
		dbs_name = "/QCD_EMEnriched_Pt30to80/akalinow-SkimTauTau_356_pass1-0a3d3891f015a95324f94837322fb8aa-elecTauSkim/USER",
		dbs_url = "http://cmsdbsprod.cern.ch/cms_dbs_ph_analysis_02/servlet/DBSServlet",             
		replFunction = makeReplacementsCrab, replacements =
		"maxEvents = -1; eventsPerJob = 200000; applyFactorization = true; estimateSysUncertainties = false; globalTag = START3X_V26::All",
		type = "mc",
		outputFileNames = [ patTupleOutputFileNameZtoElecTau_QCD_EMenriched_Pt30to80_7TeV ], outputFilePath = outputFilePath, submit = "yes")

submitToGrid(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau",
		sample = "QCD_EMenriched_Pt80to170_7TeV", job = "QCD_EMenriched_Pt80to170_spring10_0",
		dbs_name = "/QCD_EMEnriched_Pt80to170/akalinow-SkimTauTau_356_pass1-0a3d3891f015a95324f94837322fb8aa-elecTauSkim/USER",
		dbs_url = "http://cmsdbsprod.cern.ch/cms_dbs_ph_analysis_02/servlet/DBSServlet",             
		replFunction = makeReplacementsCrab, replacements =
		"maxEvents = -1; eventsPerJob = 200000; applyFactorization = true; estimateSysUncertainties = false; globalTag = START3X_V26::All",
		type = "mc",
		outputFileNames = [ patTupleOutputFileNameZtoElecTau_QCD_EMenriched_Pt80to170_7TeV ], outputFilePath = outputFilePath, submit = "yes")

submitToGrid(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau",
		sample = "QCD_BCtoE_Pt20to30_7TeV", job = "QCD_BCtoE_Pt20to30_spring10_0",
		dbs_name = "/QCD_BCtoE_Pt20to30/akalinow-SkimTauTau_356_pass1-0a3d3891f015a95324f94837322fb8aa-elecTauSkim/USER",
		dbs_url = "http://cmsdbsprod.cern.ch/cms_dbs_ph_analysis_02/servlet/DBSServlet",             
		replFunction = makeReplacementsCrab, replacements =
		"maxEvents = -1; eventsPerJob = 200000; applyFactorization = true; estimateSysUncertainties = false; globalTag = START3X_V26::All",
		type = "mc",
		outputFileNames = [ patTupleOutputFileNameZtoElecTau_QCD_BCtoE_Pt20to30_7TeV ], outputFilePath = outputFilePath, submit = "yes")

submitToGrid(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau",
		sample = "QCD_BCtoE_Pt30to80_7TeV", job = "QCD_BCtoE_Pt30to80_spring10_0",
		dbs_name = "/QCD_BCtoE_Pt30to80/akalinow-SkimTauTau_356_pass1-0a3d3891f015a95324f94837322fb8aa-elecTauSkim/USER",
		dbs_url = "http://cmsdbsprod.cern.ch/cms_dbs_ph_analysis_02/servlet/DBSServlet",             
		replFunction = makeReplacementsCrab, replacements =
		"maxEvents = -1; eventsPerJob = 200000; applyFactorization = true; estimateSysUncertainties = false; globalTag = START3X_V26::All",
		type = "mc",
		outputFileNames = [ patTupleOutputFileNameZtoElecTau_QCD_BCtoE_Pt30to80_7TeV ], outputFilePath = outputFilePath, submit = "yes")

submitToGrid(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau",
		sample = "QCD_BCtoE_Pt80to170_7TeV", job = "QCD_BCtoE_Pt80to170_spring10_0",
		dbs_name = "/QCD_BCtoE_Pt80to170/akalinow-SkimTauTau_356_pass1-0a3d3891f015a95324f94837322fb8aa-elecTauSkim/USER",
		dbs_url = "http://cmsdbsprod.cern.ch/cms_dbs_ph_analysis_02/servlet/DBSServlet",             
		replFunction = makeReplacementsCrab, replacements =
		"maxEvents = -1; eventsPerJob = 200000; applyFactorization = true; estimateSysUncertainties = false; globalTag = START3X_V26::All",
		type = "mc",
		outputFileNames = [ patTupleOutputFileNameZtoElecTau_QCD_BCtoE_Pt80to170_7TeV ], outputFilePath = outputFilePath, submit = "yes")


submitToGrid(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau",
		sample = "PhotonPlusJets_Pt15to20_7TeV", job = "PhotonPlusJets_Pt15to20_spring10_0",
		dbs_name = "/PhotonJet_Pt15to20/akalinow-SkimTauTau_356_pass1-0a3d3891f015a95324f94837322fb8aa-elecTauSkim/USER",
		dbs_url = "http://cmsdbsprod.cern.ch/cms_dbs_ph_analysis_02/servlet/DBSServlet",             
		replFunction = makeReplacementsCrab, replacements =
		"maxEvents = -1; eventsPerJob = 200000; applyFactorization = true; estimateSysUncertainties = false; globalTag = START3X_V26::All",
		type = "mc",
		outputFileNames = [ patTupleOutputFileNameZtoElecTau_PhotonPlusJets_Pt15to20_7TeV ], outputFilePath = outputFilePath, submit = "yes")

submitToGrid(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau",
		sample = "PhotonPlusJets_Pt20to30_7TeV", job = "PhotonPlusJets_Pt20to30_spring10_0",
		dbs_name = "/PhotonJet_Pt20to30/akalinow-SkimTauTau_356_pass1-0a3d3891f015a95324f94837322fb8aa-elecMuSkim/USER",
		dbs_url = "http://cmsdbsprod.cern.ch/cms_dbs_ph_analysis_02/servlet/DBSServlet",             
		replFunction = makeReplacementsCrab, replacements =
		"maxEvents = -1; eventsPerJob = 200000; applyFactorization = true; estimateSysUncertainties = false; globalTag = START3X_V26::All",
		type = "mc",
		outputFileNames = [ patTupleOutputFileNameZtoElecTau_PhotonPlusJets_Pt20to30_7TeV ], outputFilePath = outputFilePath, submit = "yes")

submitToGrid(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau",
		sample = "PhotonPlusJets_Pt30to50_7TeV", job = "PhotonPlusJets_Pt30to50_spring10_0",
		dbs_name = "/PhotonJet_Pt30to50/akalinow-SkimTauTau_356_pass1-0a3d3891f015a95324f94837322fb8aa-elecMuSkim/USER",
		dbs_url = "http://cmsdbsprod.cern.ch/cms_dbs_ph_analysis_02/servlet/DBSServlet",             
		replFunction = makeReplacementsCrab, replacements =
		"maxEvents = -1; eventsPerJob = 200000; applyFactorization = true; estimateSysUncertainties = false; globalTag = START3X_V26::All",
		type = "mc",
		outputFileNames = [ patTupleOutputFileNameZtoElecTau_PhotonPlusJets_Pt30to50_7TeV ], outputFilePath = outputFilePath, submit = "yes")

submitToGrid(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau",
		sample = "PhotonPlusJets_Pt50to80_7TeV", job = "PhotonPlusJets_Pt50to80_spring10_0",
		dbs_name = "/PhotonJet_Pt50to80/akalinow-SkimTauTau_356_pass1-0a3d3891f015a95324f94837322fb8aa-elecMuSkim/USER",
		dbs_url = "http://cmsdbsprod.cern.ch/cms_dbs_ph_analysis_02/servlet/DBSServlet",             
		replFunction = makeReplacementsCrab, replacements =
		"maxEvents = -1; eventsPerJob = 200000; applyFactorization = true; estimateSysUncertainties = false; globalTag = START3X_V26::All",
		type = "mc",
		outputFileNames = [ patTupleOutputFileNameZtoElecTau_PhotonPlusJets_Pt50to80_7TeV ], outputFilePath = outputFilePath, submit = "yes")

submitToGrid(configFile = "producePatTuple_cfg.py", channel = "ZtoElecTau",
		sample = "PhotonPlusJets_Pt80to120_7TeV", job = "PhotonPlusJets_Pt80to120_spring10_0",
		dbs_name = "/PhotonJet_Pt80to120/akalinow-SkimTauTau_356_pass1-0a3d3891f015a95324f94837322fb8aa-elecMuSkim/USER",
		dbs_url = "http://cmsdbsprod.cern.ch/cms_dbs_ph_analysis_02/servlet/DBSServlet",             
		replFunction = makeReplacementsCrab, replacements =
		"maxEvents = -1; eventsPerJob = 200000; applyFactorization = true; estimateSysUncertainties = false; globalTag = START3X_V26::All",
		type = "mc",
		outputFileNames = [ patTupleOutputFileNameZtoElecTau_PhotonPlusJets_Pt80to120_7TeV ], outputFilePath = outputFilePath, submit = "yes")



