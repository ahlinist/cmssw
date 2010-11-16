#/usr/bin/env python

samples = {
    "REDIGI38X": """
        /DYToEE_M-20_TuneZ2_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO
        /DYToMuMu_M-20_TuneZ2_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO
        /DYToTauTau_M-20_TuneZ2_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO
        /G_Pt_0to15_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO
        /G_Pt_120to170_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO
        /G_Pt_15to3000_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO
        /G_Pt_15to30_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO
        /G_Pt_170to300_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO
        /G_Pt_300to470_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO
        /G_Pt_30to50_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO
        /G_Pt_50to80_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO
        /G_Pt_80to120_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO
        /QCD_Pt-120to150_MuPt5Enriched_TuneZ2_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO
        /QCD_Pt-150_MuPt5Enriched_TuneZ2_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO
        /QCD_Pt-15to20_MuPt5Enriched_TuneZ2_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO
        /QCD_Pt-20to30_MuPt5Enriched_TuneZ2_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO
        /QCD_Pt-30to50_MuPt5Enriched_TuneZ2_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO
        /QCD_Pt-50to80_MuPt5Enriched_TuneZ2_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO
        /QCD_Pt-80to120_MuPt5Enriched_TuneZ2_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO
        /QCD_Pt_120to170_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO
        /QCD_Pt_15to3000_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO
        /QCD_Pt_15to30_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO
        /QCD_Pt_170to300_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO
        /QCD_Pt_300to470_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO
        /QCD_Pt_30to50_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO
        /QCD_Pt_50to80_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO
        /QCD_Pt_5to15_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO
        /QCD_Pt_80to120_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO
        /WGtoLNuG_TuneZ2_7TeV-pythia6-tauola/Fall10-START38_V12-v1/GEN-SIM-RECO
        /WToENu_TuneZ2_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO
        /WToMuNu_TuneZ2_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO
        /WToTauNu_TuneZ2_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO
        /WWtoAnything_TuneZ2_7TeV-pythia6-tauola/Fall10-START38_V12-v1
        /WZtoAnything_TuneZ2_7TeV-pythia6-tauola/Fall10-START38_V12-v1
        /ZGtoLLG_TuneZ2_7TeV-pythia6-tauola/Fall10-START38_V12-v1/GEN-SIM-RECO
        /ZZtoAnything_TuneZ2_7TeV-pythia6-tauola/Fall10-START38_V12-v1
        """.split(),
    "HLT": """
        /DYJetsToLL_TuneD6T_M-50_7TeV-madgraph-tauola/Fall10-START38_V12-v2/GEN-SIM-RECO
        /DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall10-START38_V12-v2/GEN-SIM-RECO
        /TTJets_TuneD6T_7TeV-madgraph-tauola/Fall10-START38_V12-v2/GEN-SIM-RECO
        /TTJets_TuneZ2_7TeV-madgraph-tauola/Fall10-START38_V12-v2/GEN-SIM-RECO
        /VQQJetsToLL_TuneD6T_7TeV-madgraph-tauola/Fall10-START38_V12-v2/GEN-SIM-RECO
        """.split()
    }

for hltProcessName, datasets in samples.items():
    for dataset in datasets:
        name = dataset.split("/")[1].replace("_TuneZ2", "").replace("_7TeV", "").replace("-pythia6", "").replace("_pythia6", "") + "-Fall10"
        print """
[%s]
CMSSW.datasetpath = %s
CMSSW.pycfg_params = crabOptions= skimType=ElectronPhoton,globalTag=START38_V13::All,hltProcessName=%s,isRealData=False,wantSummary=True,print
CMSSW.total_number_of_events = -1
CMSSW.number_of_jobs = 100
USER.publish_data_name = ElectronPhotonVGammaSkim""" % (name, dataset, hltProcessName)
        print 
    
