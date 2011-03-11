#/usr/bin/env python

samples = {
    "REDIGI38X": """
        /DYToMuMu_M-20_CT10_TuneZ2_7TeV-powheg-pythia/Winter10-E7TeV_ProbDist_2010Data_BX156_START39_V8-v1/GEN-SIM-RECO
        /DYToMuMu_M-20_CT10_TuneZ2_7TeV-powheg-pythia/Winter10-E7TeV_ProbDist_2010Data_BX156_START39_V8-v2/GEN-SIM-RECO
        /TT_TuneZ2_7TeV-pythia6-tauola/Winter10-E7TeV_ProbDist_2010Data_BX156_START39_V8-v1/GEN-SIM-RECO
        /TTJets_TuneZ2_7TeV-madgraph-tauola/Winter10-E7TeV_ProbDist_2010Data_BX156_START39_V8-v1/GEN-SIM-RECO
        /WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Winter10-E7TeV_ProbDist_2010Data_BX156_START39_V8-v2/GEN-SIM-RECO
        /QCD_Pt-20_MuEnrichedPt-15_TuneZ2_7TeV-pythia6/Winter10-E7TeV_ProbDist_2010Data_BX156_START39_V8-v1/GEN-SIM-RECO
        /ZGToMuMuG_TuneD6T_7TeV-madgraph/Winter10-E7TeV_ProbDist_2010Data_BX156_START39_V8-v1/GEN-SIM-RECO
        /GJets_TuneD6T_HT-20To40_7TeV-madgraph/Winter10-E7TeV_ProbDist_2010Data_BX156_START39_V8-v1/GEN-SIM-RECO
        /GJets_TuneD6T_HT-40To100_7TeV-madgraph/Winter10-E7TeV_ProbDist_2010Data_BX156_START39_V8-v1/GEN-SIM-RECO
        /GJets_TuneD6T_HT-100To200_7TeV-madgraph/Winter10-E7TeV_ProbDist_2010Data_BX156_START39_V8-v1/GEN-SIM-RECO
        /GJets_TuneD6T_HT-200_7TeV-madgraph/Winter10-E7TeV_ProbDist_2010Data_BX156_START39_V8-v1/GEN-SIM-RECO
        /GJet_Pt-20_doubleEMEnriched_TuneZ2_7TeV-pythia6/Winter10-E7TeV_ProbDist_2010Data_BX156_START39_V8-v1/GEN-SIM-RECO
        /GluGluToHToGG_M-120_7TeV-powheg-pythia6/Winter10-E7TeV_ProbDist_2010Data_BX156_START39_V8-v1/GEN-SIM-RECO
        /ZGtoLLG_TuneZ2_7TeV-pythia6-tauola/Winter10-E7TeV_ProbDist_2010Data_BX156_START39_V8-v1/GEN-SIM-RECO
        """.split(),
    }

for hltProcessName, datasets in samples.items():
    for dataset in datasets:
        name, tier = dataset.split("/")[1:3]

        print """
[%s]
CMSSW.datasetpath = %s
CMSSW.pycfg_params = crabOptions=skimType=Dimuon,globalTag=START39_V8::All,reportEvery=1000,isRealData=False,wantSummary=True,hltProcessName=REDIGI39X,print
CMSSW.total_number_of_events = -1
CMSSW.number_of_jobs = 100
USER.publish_data_name = %s-DimuonVGammaSkim_v4""" % (name, dataset, tier)
        print


