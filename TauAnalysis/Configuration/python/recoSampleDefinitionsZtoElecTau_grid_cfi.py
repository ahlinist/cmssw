import FWCore.ParameterSet.Config as cms
import copy

import TauAnalysis.Configuration.plotterProcessDefinitions_cfi as plotter
import TauAnalysis.DQMTools.plotterStyleDefinitions_cfi as styles

# List of samples to run in the analysis
SAMPLES_TO_ANALYZE_SKIM = [

#'Ztautau_embedded_PR_v6_skim',
#'Ztautau_embedded_May10ReReco_skim',
## 'Ztautau_embedded_PR_v4_skim',
## 'Ztautau_embedded_05AugReReco_skim',

        'data_TauPlusX_Run2011A_May10ReReco_skim',
        'data_TauPlusX_Run2011A_PR_v4_skim',
        'data_TauPlusX_Run2011A_05AugReReco_skim',
        'data_TauPlusX_Run2011A_03OctReReco_skim',
        'data_TauPlusX_Run2011B_PR_v1_skim',
        'Ztautau_powheg_skim',
        'DYtautauM10to20_powheg_skim',
        'Zee_powheg_skim',
        'DYeeM10to20_pythia_skim',
        'TTplusJets_madgraph_skim',
        'WplusJets_madgraph_skim',
        
        #'PhotonPlusJets_Pt15to30_skim','PhotonPlusJets_Pt30to50_skim','PhotonPlusJets_Pt50to80_skim',
        #'QCD_BCtoE_Pt20to30_skim','QCD_BCtoE_Pt30to80_skim','QCD_BCtoE_Pt80to170_skim',
        #'QCD_EM_Pt20to30_skim','QCD_EM_Pt30to80_skim','QCD_EM_Pt80to170_skim',

        #'WW_skim','WZ_skim',
] 

SAMPLES_TO_ANALYZE_PAT = [
    #'data_TauPlusX_Run2011A_03OctReReco_pat',
    #'data_TauPlusX_Run2011A_PR_v4_pat',
    #'data_TauPlusX_Run2011B_PR_v1_pat'
    'data_TauPlusX_Run2011A_May10ReReco_pat',
    'data_TauPlusX_Run2011A_05AugReReco_pat', 
    ## 'Ztautau_powheg_pat',
    'Zee_powheg_pat',
    ## 'WplusJets_madgraph_pat',
    'TTplusJets_madgraph_pat'
] 

# set final analysis samples here
#SAMPLES_TO_ANALYZE = SAMPLES_TO_ANALYZE_SKIM
SAMPLES_TO_ANALYZE = SAMPLES_TO_ANALYZE_PAT


# List of samples to include in the final level plots.  May include selections
# from the MERGE_SAMPLES defined at the bottom.
SAMPLES_TO_PLOT = [
    'data', 
    'WplusJets_madgraph_pat',
    'TTplusJets_madgraph_pat',
    'Ztautau_powheg_pat',
    'Zee_powheg_pat',    
]

#SAMPLES_TO_PLOT = SAMPLES_TO_ANALYZE_PAT


SAMPLES_TO_PRINT = copy.copy(SAMPLES_TO_PLOT)

SAMPLE_DEFAULTS = {
    'dbs_url' : "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
    'conditions' : 'START42_V13::All',
    'genPhaseSpaceCut' : '',
    'genFinalStateFilter': False,
    'factorize' : False,
    'enableSysUncertainties' : True,
    'lumi_mask' : '',
    'runselection' : '',
    'inputFileType' : 'AOD',
    'hlt_paths' : [ 'HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v2'],   
    'SE_white_list' : '',
    'SE_black_list' : '',
    'disableDuplicateCheck' : False,
    'applyZrecoilCorrection' : False,
    'applyElectronTriggerEfficiencyCorrection' : False,
    'applyElectronIsolationEfficiencyCorrection' : False,
    'applyMuonTriggerEfficiencyCorrection' : False,
    'applyVertexMultiplicityReweighting' : False,
    'applyRhoNeutralReweighting': True,
    'applyTauMetTriggerEfficiencyCorrection' : False,
    'hlt' : cms.InputTag("TriggerResults", "", "HLT"),
    'noRunLumiEventSave' : True,
    'absoluteNormalization' : -1
}

# Conversions to pico barns
_millibarns = 1.0e+9
_microbarns = 1.0e+6
_nanobarns  = 1.0e+3
_picobarns =  1.0
_femtobarns = 1.0e-3

# Integrated luminosity to normalize
# May10ReReco + PR_v4 + 05AugReReco + PR_v6 (-172802): Lepton-Photon dataset
TARGET_LUMI = (1610)/_picobarns 

#--------------------------------------------------------------------------------
# NOTE: cross-sections for W and Z production are scaled to next-to-leading order values
#       documented on the wiki
#        https://alcaraz.web.cern.ch/alcaraz/CROSS_SECTIONS.txt
#--------------------------------------------------------------------------------

RECO_SAMPLES = {
        'data_TauPlusX_Run2011A_May10ReReco_skim' : {  # 160/pb, 160432-163869, xxxxx events: run =< 163261 have bad triggers
        'datasetpath' : '/TauPlusX/jkolb-skimElecTau_423_May10ReReco_v3-2da1106465614f2b4aae43c293e2ca66/USER', 
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'lumi_mask' : '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/Reprocessing/Cert_160404-163869_7TeV_May10ReReco_Collisions11_JSON_v3.txt',
        'conditions' : 'GR_R_42_V14::All',
        'runselection' : '163262-163869',
        'number_of_jobs' : 150,
        'type' : 'Data',
        'drawOption' : styles.drawOption_Data,
        'enableSysUncertainties' : False,
        'applyVertexMultiplicityReweighting' : False,
        'applyElectronTriggerEfficiencyCorrection' : False,
        'applyElectronIsolationEfficiencyCorrection' : False,
        'applyZrecoilCorrection' : False,
        'noRunLumiEventSave' : False,
        'hlt_paths' : [
            'HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v1',
            'HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v2',
            'HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v4'
        ]
    },
    'data_TauPlusX_Run2011A_May10ReReco_pat' : {#done
        'datasetpath' : '/TauPlusX/lantonel-patSkim_428_v3-d9c77f81737e7a088cf162cb68efea10/USER', 
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'lumi_mask' : '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/Reprocessing/Cert_160404-163869_7TeV_May10ReReco_Collisions11_JSON_v2.txt',
        'skim_eff' : 1407306./12485249.,
        'conditions' : 'GR_R_42_V14::All',
        'number_of_jobs' : 50,
        'type' : 'Data',
        'drawOption' : styles.drawOption_Data,
        'enableSysUncertainties' : False,
        'applyVertexMultiplicityReweighting' : False,
        'applyElectronTriggerEfficiencyCorrection' : False,
        'applyElectronIsolationEfficiencyCorrection' : False,
        'applyZrecoilCorrection' : False,
        'applyRhoNeutralReweighting': False,
        'inputFileType' : 'PATTuple',
        'legendEntry' : 'DATA',        
        'hlt_paths' : [
            'HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v1',
            'HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v2',
            'HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v4'
        ]
    },    
    'data_TauPlusX_Run2011A_05AugReReco_skim' : {  #400.3/pb, runs 170249-172619, 728380 events
        'datasetpath' : '/TauPlusX/jkolb-Run2011A-05Aug2011-v1_skimElecTau_v1-982c87f3521a6471fb16318d08f703d0/USER', 
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'lumi_mask' : '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/Reprocessing/Cert_170249-172619_7TeV_ReReco5Aug_Collisions11_JSON_v3.txt',
        'conditions' : 'GR_R_42_V20::All',
        'number_of_jobs' : 300,
        'type' : 'Data',
        'drawOption' : styles.drawOption_Data,
        'enableSysUncertainties' : False,
	'applyVertexMultiplicityReweighting' : False,
        'applyElectronTriggerEfficiencyCorrection' : False,
        'applyElectronIsolationEfficiencyCorrection' : False,
        'applyZrecoilCorrection' : False,
        'noRunLumiEventSave' : False,
        'hlt_paths' : [
            'HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v6',
            'HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v8',
            'HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v9',
            'HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TightIsoPFTau20_v2',
            'HLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TightIsoPFTau20_v2',
            'HLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v1'
        ]
    },
    'data_TauPlusX_Run2011A_05AugReReco_pat' : {#done
        'datasetpath' : '/TauPlusX/lantonel-patSkim_428_v3-da7956cf664e460ea98716c7f3391601/USER', 
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'skim_eff' : 728380./3727491,
        'conditions' : 'GR_R_42_V20::All',
        'number_of_jobs' : 300,
        'type' : 'Data',
        'drawOption' : styles.drawOption_Data,
        'enableSysUncertainties' : False,
	'applyVertexMultiplicityReweighting' : False,
        'applyElectronTriggerEfficiencyCorrection' : False,
        'applyElectronIsolationEfficiencyCorrection' : False,
        'applyZrecoilCorrection' : False,
        'applyRhoNeutralReweighting': False,
        'inputFileType' : 'PATTuple',
        'legendEntry' : 'DATA',        
        'hlt_paths' : [
            'HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v6',
            'HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v8',
            'HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v9',
            'HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TightIsoPFTau20_v2',
            'HLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TightIsoPFTau20_v2',
            'HLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v1'
        ]
    },    
    'data_TauPlusX_Run2011A_03OctReReco_skim' : { # 705/pb 172620-173692, 1217831 events
        'datasetpath' : '/TauPlusX/jkolb-Run2011A-03OctReReco_skimElecTau_v1-6aa5d932edddb97c8f87b85a020d9993/USER', 
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'conditions' : 'GR_P_V22::All',
        'number_of_jobs' : 500,
        'type' : 'Data',
        'drawOption' : styles.drawOption_Data,
        'enableSysUncertainties' : False,
	'applyVertexMultiplicityReweighting' : False,
        'applyElectronTriggerEfficiencyCorrection' : False,
        'applyElectronIsolationEfficiencyCorrection' : False,
        'applyZrecoilCorrection' : False,
        'hlt_paths' : [
            'HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TightIsoPFTau20_v2',
            'HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TightIsoPFTau20_v2',
            'HLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v1'
        ]
    },
    'data_TauPlusX_Run2011A_03OctReReco_pat' : { # 705/pb 172620-173692, 1217831 events  #done
        'datasetpath' : '/TauPlusX/lantonel-patSkim_428_v3-b13ce57e4a1e9e520b27a0250d4dfe14/USER', 
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'conditions' : 'GR_P_V22::All',
        'number_of_jobs' : 500,
        'runselection' : '172620-173692',        
        'type' : 'Data',
        'drawOption' : styles.drawOption_Data,
        'enableSysUncertainties' : False,
	'applyVertexMultiplicityReweighting' : False,
        'applyElectronTriggerEfficiencyCorrection' : False,
        'applyElectronIsolationEfficiencyCorrection' : False,
        'applyZrecoilCorrection' : False,
        'applyZrecoilCorrection' : False,
        'applyRhoNeutralReweighting': False,
        'inputFileType' : 'PATTuple',
        'legendEntry' : 'DATA',        
        'hlt_paths' : [
            'HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TightIsoPFTau20_v2',
            'HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TightIsoPFTau20_v2',
            'HLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v1'
        ]
    },        

        
    'data_TauPlusX_Run2011A_PR_v4_skim' : { # 887/pb 165071-167913, 5942864 events--> current lumi_mask restricts analysis to Lepton-Photon dataset
        'datasetpath' : '/TauPlusX/jkolb-skimElecTau_424_v1-982c87f3521a6471fb16318d08f703d0/USER', 
        'lumi_mask' : '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification//Collisions11/7TeV/Prompt/Cert_160404-172802_7TeV_PromptReco_Collisions11_JSON_v4.txt',
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'conditions' : 'GR_P_V22::All',
        'number_of_jobs' : 300,
        'type' : 'Data',
        'drawOption' : styles.drawOption_Data,
        'enableSysUncertainties' : False,
	'applyVertexMultiplicityReweighting' : False,
        'applyElectronTriggerEfficiencyCorrection' : False,
        'applyElectronIsolationEfficiencyCorrection' : False,
        'applyZrecoilCorrection' : False,
        'noRunLumiEventSave' : False,
        'hlt_paths' : [
            'HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v6',
            'HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v8',
            'HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v9'
        ]
    },
    'data_TauPlusX_Run2011A_PR_v4_pat' : {#done
        'datasetpath' : '/TauPlusX/lantonel-patSkim_428_v3-b13ce57e4a1e9e520b27a0250d4dfe14/USER', 
        'lumi_mask' : '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification//Collisions11/7TeV/Prompt/Cert_160404-172802_7TeV_PromptReco_Collisions11_JSON_v4.txt',
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'conditions' : 'GR_P_V22::All',
        'number_of_jobs' : 200,
        'runselection' : '165071-167913',           
        'type' : 'Data',
        'drawOption' : styles.drawOption_Data,
        'enableSysUncertainties' : False,
	'applyVertexMultiplicityReweighting' : False,
        'applyElectronTriggerEfficiencyCorrection' : False,
        'applyElectronIsolationEfficiencyCorrection' : False,
        'applyZrecoilCorrection' : False,
        'inputFileType' : 'PATTuple',
        'legendEntry' : 'DATA',        
        'hlt_paths' : [
            'HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v6',
            'HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v8',
            'HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v9'
        ]
    },   
    ####   PR_v6 has been replaced by 03Oct rereco  ######
    'data_TauPlusX_Run2011A_PR_v6_skim' : {  # 641.9/pb, runs 172620-173692, 1160206 events --> limited to LP dataset: 120/pb, runs 172620-172802
        'datasetpath' : '/TauPlusX/jkolb-Run2011A-PromptReco-v6_skimElecTau_v3-982c87f3521a6471fb16318d08f703d0/USER', 
        'lumi_mask' : '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification//Collisions11/7TeV/Prompt/Cert_160404-172802_7TeV_PromptReco_Collisions11_JSON_v4.txt', # LP
        #'lumi_mask' : '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification//Collisions11/7TeV/Prompt/Cert_160404-173692_7TeV_PromptReco_Collisions11_JSON.txt', # full v6 PR
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'conditions' : 'GR_P_V22::All',
        'number_of_jobs' : 300,
        'type' : 'Data',
        'drawOption' : styles.drawOption_Data,
        'enableSysUncertainties' : False,
	    'applyVertexMultiplicityReweighting' : False,
        'applyElectronTriggerEfficiencyCorrection' : False,
        'applyElectronIsolationEfficiencyCorrection' : False,
        'applyZrecoilCorrection' : False,
        'noRunLumiEventSave' : False,
        'hlt_paths' : [
            'HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TightIsoPFTau20_v2',
            'HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TightIsoPFTau20_v2',
            'HLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v1'
        ]
    },
    'data_TauPlusX_Run2011B_PR_v1_skim' : {  # 2190/pb, runs 175860-179411, 3585094 events 
        'datasetpath' : '/TauPlusX/jkolb-Run2011B-PromptReco-v1_skimElecTau_v4-982c87f3521a6471fb16318d08f703d0/USER', 
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'conditions' : 'GR_P_V22::All',
        'number_of_jobs' : 300,
        'type' : 'Data',
        'drawOption' : styles.drawOption_Data,
        'enableSysUncertainties' : False,
	    'applyVertexMultiplicityReweighting' : False,
        'applyElectronTriggerEfficiencyCorrection' : False,
        'applyElectronIsolationEfficiencyCorrection' : False,
        'applyZrecoilCorrection' : False,
        'noRunLumiEventSave' : False,
        'hlt_paths' : [
            'HLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v1',
            'HLT_Ele20_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v5',
            'HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau25_v4'
        ]
    },
    'data_TauPlusX_Run2011B_PR_v1_pat' : {#done
        'datasetpath' : '/TauPlusX/lantonel-patSkim_428_v3-b13ce57e4a1e9e520b27a0250d4dfe14/USER', 
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'conditions' : 'GR_P_V22::All',
        'number_of_jobs' : 300,
        'runselection' : '175860-179411',           
        'type' : 'Data',
        'drawOption' : styles.drawOption_Data,
        'enableSysUncertainties' : False,
        'applyVertexMultiplicityReweighting' : False,
        'applyElectronTriggerEfficiencyCorrection' : False,
        'applyElectronIsolationEfficiencyCorrection' : False,
        'applyZrecoilCorrection' : False,
        'noRunLumiEventSave' : False,
        'inputFileType' : 'PATTuple',
        'legendEntry' : 'DATA',        
        'hlt_paths' : [
            'HLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v1',
            'HLT_Ele20_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v5',
            'HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau25_v4'
        ]
    },
    'DYtautauM10to20_powheg_skim' : {
        'datasetpath' : "/DYToTauTau_M-10To20_TuneZ2_7TeV-pythia6-tauola/jkolb-skimElecTau_423_v1-8e1616e5e84b94400800aa9b2edac84c/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 5,
        'events_processed' : 2200000,
        'skim_eff' : 1410./2200000,
        'x_sec' : 1666*_picobarns,
        'legendEntry' : plotter.process_Ztautau.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Ztautau.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Ztautau,
    },
    'DYtautauM10to20_powheg_pat' : {#done
        'datasetpath' : "/DYToTauTau_M-10To20_TuneZ2_7TeV-pythia6-tauola/lantonel-patSkim_428_v3-fb0e7ea3046b2efaed62c2520cf554fb/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 5,
        'events_processed' : 2200000,
        'skim_eff' : 1410./2200000,
        'x_sec' : 1.28*2659*_picobarns,
        'legendEntry' : plotter.process_Ztautau.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Ztautau.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Ztautau,
        'enableSysUncertainties' : False,
        'applyZrecoilCorrection' : True,
        'applyRhoNeutralReweighting' : True,
        'applyVertexMultiplicityReweighting' : False,        
        'inputFileType' : 'PATTuple',
    },    
    'Ztautau_powheg_skim' : {
        'datasetpath' : "/DYToTauTau_M-20_CT10_TuneZ2_7TeV-powheg-pythia-tauola/jkolb-skimElecTau_v4_Summer11-982c87f3521a6471fb16318d08f703d0/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 300,
        #'applyZrecoilCorrection' : False,
        'events_processed' : 19937479,
        'skim_eff' : 1012582./19937479,
        'x_sec' : 1666*_picobarns,
        'legendEntry' : plotter.process_Ztautau.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Ztautau.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Ztautau,
        'applyRhoNeutralReweighting' : True,
        'applyVertexMultiplicityReweighting' : False,
        
        #'absoluteNormalization' : 5294./29702
    },
    'Ztautau_powheg_pat' : {#done
        'datasetpath' : "/DYToTauTau_M-20_CT10_TuneZ2_7TeV-powheg-pythia-tauola/lantonel-patSkim_428_v3-fb0e7ea3046b2efaed62c2520cf554fb/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 300,
        'events_processed' : 19937479,
        'skim_eff' : 1012582./19937479,
        'x_sec' : 1666*_picobarns,
        'legendEntry' : plotter.process_Ztautau.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Ztautau.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Ztautau,
        'enableSysUncertainties' : False,
        'applyZrecoilCorrection' : True,
        'applyRhoNeutralReweighting' : True,
        'applyVertexMultiplicityReweighting' : False,
        'inputFileType' : 'PATTuple',        
    },    
    'Ztautau_pythia_skim' : { 
        'datasetpath' : "/DYToTauTau_M-20_TuneZ2_7TeV-pythia6-tauola/jkolb-skimElecTau_423_v1-2453a4eaae124a4a3fe9f365dc31e11f/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 40,
        'events_processed' : 2032536,
        'skim_eff' : 102161./2032536,
        'x_sec' : 1666*_picobarns,
        'legendEntry' : plotter.process_Ztautau.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Ztautau.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Ztautau,
    },
    'DYeeM10to20_pythia_skim' : {
        'datasetpath' : "/DYToEE_M-10To20_TuneZ2_7TeV-pythia6/jkolb-skimElecTau_423_v1-8e1616e5e84b94400800aa9b2edac84c/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 10,
        'events_processed' : 2121872,
        'skim_eff' : 53901./2121872,
        'x_sec' : 1666*_picobarns,
        'legendEntry' : plotter.process_Zee.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Zee.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Zee,
    },
    'DYeeM10to20_pythia_pat' : {#done
        'datasetpath' : "/DYToEE_M-10To20_TuneZ2_7TeV-pythia6/lantonel-patSkim_428_v3-fb0e7ea3046b2efaed62c2520cf554fb/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 10,
        'events_processed' : 2121872,
        'skim_eff' : 53901./2121872,
        'x_sec' : 1666*_picobarns,
        'legendEntry' : plotter.process_Zee.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Zee.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Zee,
        'enableSysUncertainties' : False,
        'applyZrecoilCorrection' : True,
        'applyRhoNeutralReweighting' : True,
        'applyVertexMultiplicityReweighting' : False,
        'inputFileType' : 'PATTuple',        
    },    
    'Zee_powheg_skim' : { 
        'datasetpath' : "/DYToEE_M-20_CT10_TuneZ2_7TeV-powheg-pythia/jkolb-skimElecTau_v3_Summer11-982c87f3521a6471fb16318d08f703d0/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 900,
        'events_processed' : 29497207,
        'skim_eff' : 13672814./29497207,
        'x_sec' : 1666*_picobarns,
        'legendEntry' : plotter.process_Zee.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Zee.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Zee,
        #'absoluteNormalization' : 2308./2551
    },
    'Zee_powheg_pat' : {#done
        'datasetpath' : "/DYToEE_M-20_CT10_TuneZ2_7TeV-powheg-pythia/lantonel-patSkim_428_v3-fb0e7ea3046b2efaed62c2520cf554fb/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 900,
        'events_processed' : 29497207,
        'skim_eff' : 10330560./29497207,
        'x_sec' : 1666*_picobarns,
        'legendEntry' : plotter.process_Zee.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Zee.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Zee,
        'enableSysUncertainties' : False,
        'applyZrecoilCorrection' : True,
        'applyRhoNeutralReweighting' : True,
        'applyVertexMultiplicityReweighting' : False,
        'inputFileType' : 'PATTuple',          
    },    
    'WplusJets_madgraph_skim' : {
        'datasetpath' : "/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/jkolb-skimElecTau_424_v5-982c87f3521a6471fb16318d08f703d0/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 800,
        'events_processed' : 81352581,
        'skim_eff' : 8165915./81352581,
        'x_sec' : 31314*_picobarns,
        'legendEntry' : plotter.process_WplusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_WplusJets.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_WplusJets,
        'enableSysUncertainties' : False,
        'absoluteNormalization' : 2155./538
    },
    'WplusJets_madgraph_pat' : {#done
        'datasetpath' : "/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/lantonel-patSkim_428_v3-fb0e7ea3046b2efaed62c2520cf554fb/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 800,
        'events_processed' : 81352581,
        'skim_eff' : 8165915./81352581,
        'x_sec' : 31314*_picobarns,
        'enableSysUncertainties' : False,
        'legendEntry' : plotter.process_WplusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_WplusJets.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_WplusJets,
        'enableSysUncertainties' : False,
        'applyZrecoilCorrection' : True,
        'applyRhoNeutralReweighting' : True,
        'applyVertexMultiplicityReweighting' : False,
        'inputFileType' : 'PATTuple',          
    },    
    'WW_skim' : {
        'datasetpath' : "/WW_TuneZ2_7TeV_pythia6_tauola/jkolb-skimElecTau_423_v1-2453a4eaae124a4a3fe9f365dc31e11f/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 20,
        'events_processed' : 4225916,
        'skim_eff' : 512235./4225916,
        'x_sec' : 27.83*_picobarns, 
        'legendEntry' : plotter.process_VV.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_VV.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_VV,
    },
##     'WW_pat' : {
##         'datasetpath' : "/WW_TuneZ2_7TeV_pythia6_tauola/lantonel-patSkim_428_v1-0fa068f9514ddfc0e3f25d602948dee0/USER",
##         'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
##         'number_of_jobs' : 20,
##         'events_processed' : 4225916,
##         'skim_eff' : 512235./4225916,
##         'x_sec' : 27.83*_picobarns, 
##         'legendEntry' : plotter.process_VV.config_dqmHistPlotter.legendEntry.value(),
##         'type' : plotter.process_VV.config_dqmHistPlotter.type.value(),
##         'drawOption' : styles.drawOption_VV,
##     },
        
    'WZ_skim' : {
        'datasetpath' : "/WZ_TuneZ2_7TeV_pythia6_tauola/jkolb-skimElecTau_423_v1-2453a4eaae124a4a3fe9f365dc31e11f/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 20,
        'events_processed' : 4265243,
        'skim_eff' : 357637./4265243,
        'x_sec' : 10.5*_picobarns, 
        'legendEntry' : plotter.process_VV.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_VV.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_VV,
    },
##     'WZ_pat' : {
##         'datasetpath' : "/WZ_TuneZ2_7TeV_pythia6_tauola/lantonel-patSkim_428_v1-0fa068f9514ddfc0e3f25d602948dee0/USER",
##         'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
##         'number_of_jobs' : 20,
##         'events_processed' : 4265243,
##         'skim_eff' : 357637./4265243,
##         'x_sec' : 10.5*_picobarns, 
##         'legendEntry' : plotter.process_VV.config_dqmHistPlotter.legendEntry.value(),
##         'type' : plotter.process_VV.config_dqmHistPlotter.type.value(),
##         'drawOption' : styles.drawOption_VV,
##     },   
        
    'ZZ' : {  ##  no datset yet for Summer11
        'datasetpath' : "",
        'events_processed' : 4,
        'x_sec' : 5.9*_picobarns,
        'legendEntry' : plotter.process_VV.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_VV.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_VV,
    },
    'ZZ_skim' : {
        'datasetpath' : "",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 20,
        'events_processed' : 4191045,
        'skim_eff' : 151802./4191045,
        'x_sec' : 5.9*_picobarns, 
        'legendEntry' : plotter.process_VV.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_VV.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_VV,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'TTplusJets_madgraph_skim' : {
        'datasetpath' : "/TTJets_TuneZ2_7TeV-madgraph-tauola/jkolb-skimElecTau_423_v2-2453a4eaae124a4a3fe9f365dc31e11f/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 300,
        'events_processed' : 3701947,
        'skim_eff' : 725953./3701947,
        'x_sec' : 157.5*_picobarns, # NLO cross-section from https://twiki.cern.ch/twiki/bin/viewauth/CMS/StandardModelCrossSections
        'legendEntry' : plotter.process_TTplusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_TTplusJets.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_TTplusJets,
    },
    'TTplusJets_madgraph_pat' : {#done
        'datasetpath' : "/TTJets_TuneZ2_7TeV-madgraph-tauola/lantonel-patSkim_428_v3-fb0e7ea3046b2efaed62c2520cf554fb/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 100,
        'events_processed' : 3701947,
        'skim_eff' : 725953./3701947,
        'x_sec' : 157.5*_picobarns, # NLO cross-section from https://twiki.cern.ch/twiki/bin/viewauth/CMS/StandardModelCrossSections
        'legendEntry' : plotter.process_TTplusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_TTplusJets.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_TTplusJets,
        'enableSysUncertainties' : False,
        'applyZrecoilCorrection' : True,
        'applyRhoNeutralReweighting' : True,
        'applyVertexMultiplicityReweighting' : False,
        'inputFileType' : 'PATTuple',         
    },     
    'PhotonPlusJets_Pt15to30_skim' : {  ## jobs mostly done
        'datasetpath' : "/G_Pt-15to30_TuneZ2_7TeV_pythia6/jkolb-skimElecTau_423_v1-8e1616e5e84b94400800aa9b2edac84c/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 20,
        'events_processed' : 2046119,
        'skim_eff' : 72643./2046119,
        'factorize' : True,
        'x_sec' : 171700*_picobarns, 
        'legendEntry' : plotter.process_gammaPlusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_gammaPlusJets.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_gammaPlusJets,
    },
##     'PhotonPlusJets_Pt15to30_pat' : {  
##         'datasetpath' : "/G_Pt-15to30_TuneZ2_7TeV_pythia6/lantonel-patSkim_428_v1-0fa068f9514ddfc0e3f25d602948dee0/USER",
##         'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
##         'number_of_jobs' : 20,
##         'events_processed' : 2046119,
##         'skim_eff' : 72643./2046119,
##         'factorize' : True,
##         'x_sec' : 171700*_picobarns, 
##         'legendEntry' : plotter.process_gammaPlusJets.config_dqmHistPlotter.legendEntry.value(),
##         'type' : plotter.process_gammaPlusJets.config_dqmHistPlotter.type.value(),
##         'drawOption' : styles.drawOption_gammaPlusJets,
##     }, 
        
    'PhotonPlusJets_Pt30to50_skim' : { ## jobs mostly done
        'datasetpath' : "/G_Pt-30to50_TuneZ2_7TeV_pythia6/jkolb-skimElecTau_423_v1-8e1616e5e84b94400800aa9b2edac84c/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 30,
        'events_processed' : 2187260,
        'skim_eff' : 125184./2187260,
        'factorize' : True,
        'x_sec' : 16690*_picobarns, 
        'legendEntry' : plotter.process_gammaPlusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_gammaPlusJets.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_gammaPlusJets,
    },
##     'PhotonPlusJets_Pt30to50_pat' : { 
##         'datasetpath' : "/G_Pt-30to50_TuneZ2_7TeV_pythia6/lantonel-patSkim_428_v1-0fa068f9514ddfc0e3f25d602948dee0/USER",
##         'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
##         'number_of_jobs' : 30,
##         'events_processed' : 2187260,
##         'skim_eff' : 125184./2187260,
##         'factorize' : True,
##         'x_sec' : 16690*_picobarns, 
##         'legendEntry' : plotter.process_gammaPlusJets.config_dqmHistPlotter.legendEntry.value(),
##         'type' : plotter.process_gammaPlusJets.config_dqmHistPlotter.type.value(),
##         'drawOption' : styles.drawOption_gammaPlusJets,
##     }, 
        
    'PhotonPlusJets_Pt50to80_skim' : {  ## jobs mostly done
        'datasetpath' : "/G_Pt-50to80_TuneZ2_7TeV_pythia6/jkolb-skimElecTau_423_v1-8e1616e5e84b94400800aa9b2edac84c/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 30,
        'events_processed' : 2036704,
        'skim_eff' : 112937./2036704,
        'factorize' : True,
        'x_sec' : 2722*_picobarns, 
        'legendEntry' : plotter.process_gammaPlusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_gammaPlusJets.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_gammaPlusJets,
    },
##     'PhotonPlusJets_Pt50to80_pat' : {  
##         'datasetpath' : "/G_Pt-50to80_TuneZ2_7TeV_pythia6/lantonel-patSkim_428_v1-0fa068f9514ddfc0e3f25d602948dee0/USER",
##         'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
##         'number_of_jobs' : 30,
##         'events_processed' : 2036704,
##         'skim_eff' : 112937./2036704,
##         'factorize' : True,
##         'x_sec' : 2722*_picobarns, 
##         'legendEntry' : plotter.process_gammaPlusJets.config_dqmHistPlotter.legendEntry.value(),
##         'type' : plotter.process_gammaPlusJets.config_dqmHistPlotter.type.value(),
##         'drawOption' : styles.drawOption_gammaPlusJets,
##     },  
        
    'QCD_BCtoE_Pt20to30_skim' : { ## 2 jobs not finished
        'datasetpath' : "/QCD_Pt-20to30_BCtoE_TuneZ2_7TeV-pythia6/jkolb-skimElecTau_423_v1-8e1616e5e84b94400800aa9b2edac84c/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 80,
        'events_processed' : 2081560,
        'skim_eff' : 430412./2081560,
        'factorize' : True,
        'x_sec' : 236000000*0.00056*_picobarns, 
        'legendEntry' : plotter.process_QCD_BCtoE_Pt20to30.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_QCD_BCtoE_Pt20to30.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_QCD,
    },
##     'QCD_BCtoE_Pt20to30_pat' : {
##         'datasetpath' : "/QCD_Pt-20to30_BCtoE_TuneZ2_7TeV-pythia6/lantonel-patSkim_428_v1-0fa068f9514ddfc0e3f25d602948dee0/USER",
##         'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
##         'number_of_jobs' : 50,
##         'events_processed' : 2081560,
##         'skim_eff' : 430412./2081560,
##         'factorize' : True,
##         'x_sec' : 236000000*0.00056*_picobarns, 
##         'legendEntry' : plotter.process_QCD_BCtoE_Pt20to30.config_dqmHistPlotter.legendEntry.value(),
##         'type' : plotter.process_QCD_BCtoE_Pt20to30.config_dqmHistPlotter.type.value(),
##         'drawOption' : styles.drawOption_QCD,
##     }, 
        
    'QCD_BCtoE_Pt30to80_skim' : {  ## 5/201 skim jobs not finished
        'datasetpath' : "/QCD_Pt-30to80_BCtoE_TuneZ2_7TeV-pythia6/jkolb-skimElecTau_423_v1-8e1616e5e84b94400800aa9b2edac84c/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 80,
        'events_processed' : 2030033,
        'skim_eff' : 457229./2030033,
        'factorize' : True,
        'x_sec' : 59480000*0.0023*_picobarns, 
        'legendEntry' : plotter.process_QCD_BCtoE_Pt30to80.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_QCD_BCtoE_Pt30to80.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_QCD,
    },
##     'QCD_BCtoE_Pt30to80_pat' : {
##         'datasetpath' : "/QCD_Pt-30to80_BCtoE_TuneZ2_7TeV-pythia6/lantonel-patSkim_428_v1-0fa068f9514ddfc0e3f25d602948dee0/USER",
##         'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
##         'number_of_jobs' : 50,
##         'events_processed' : 2030033,
##         'skim_eff' : 457229./2030033,
##         'factorize' : True,
##         'x_sec' : 59480000*0.0023*_picobarns, 
##         'legendEntry' : plotter.process_QCD_BCtoE_Pt30to80.config_dqmHistPlotter.legendEntry.value(),
##         'type' : plotter.process_QCD_BCtoE_Pt30to80.config_dqmHistPlotter.type.value(),
##         'drawOption' : styles.drawOption_QCD,
##     }, 
        
    'QCD_BCtoE_Pt80to170_skim' : { # all jobs finished
        'datasetpath' : "/QCD_Pt-80to170_BCtoE_TuneZ2_7TeV-pythia/jkolb-skimElecTau_423_v1-8e1616e5e84b94400800aa9b2edac84c/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 30,
        'events_processed' : 1082691,
        'skim_eff' : 144314./1082691,
        'factorize' : True,
        'x_sec' : 900000*0.0104*_picobarns, 
        'legendEntry' : plotter.process_QCD_BCtoE_Pt80to170.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_QCD_BCtoE_Pt80to170.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_QCD,
    },
##     'QCD_BCtoE_Pt80to170_pat' : {
##         'datasetpath' : "/QCD_Pt-80to170_BCtoE_TuneZ2_7TeV-pythia/lantonel-patSkim_428_v1-0fa068f9514ddfc0e3f25d602948dee0/USER",
##         'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
##         'number_of_jobs' : 15,
##         'events_processed' : 1082691,
##         'skim_eff' : 144314./1082691,
##         'factorize' : True,
##         'x_sec' : 900000*0.0104*_picobarns, 
##         'legendEntry' : plotter.process_QCD_BCtoE_Pt80to170.config_dqmHistPlotter.legendEntry.value(),
##         'type' : plotter.process_QCD_BCtoE_Pt80to170.config_dqmHistPlotter.type.value(),
##         'drawOption' : styles.drawOption_QCD,
##     }, 
        
    'QCD_EM_Pt20to30_skim' : { # 62/408 jobs not done
        'datasetpath' : "/QCD_Pt-20to30_EMEnriched_TuneZ2_7TeV-pythia6/jkolb-skimElecTau_423_v1-8e1616e5e84b94400800aa9b2edac84c/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 100,
        'events_processed' : 22529376,
        'skim_eff' : 574785./22529376,
        'factorize' : True,
        'x_sec' : 236000000*0.0104*_picobarns, 
        'legendEntry' : plotter.process_QCD_EMenriched_Pt20to30.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_QCD_EMenriched_Pt20to30.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_QCD,
    },
##     'QCD_EM_Pt30to80_skim' : {
##         'datasetpath' : "/QCD_Pt-30to80_EMEnriched_TuneZ2_7TeV-pythia/jkolb-skimElecTau_423_v3-6bdd841a52c69818ca2e9691e30868a4/USER",
##         'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
##         'number_of_jobs' : 200,
##         'events_processed' : 70392060,
##         'skim_eff' : 1169446./70392060,
##         'factorize' : True,
##         'x_sec' : 59480000*0.065*_picobarns, 
##         'legendEntry' : plotter.process_QCD_EMenriched_Pt30to80.config_dqmHistPlotter.legendEntry.value(),
##         'type' : plotter.process_QCD_EMenriched_Pt30to80.config_dqmHistPlotter.type.value(),
##         'drawOption' : styles.drawOption_QCD,
##     },
    'QCD_EM_Pt80to170_skim' : {
        'datasetpath' : "/QCD_Pt-80to170_EMEnriched_TuneZ2_7TeV-pythia6/jkolb-skimElecTau_423_v1-8e1616e5e84b94400800aa9b2edac84c/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 50,
        'events_processed' : 8150672,
        'skim_eff' : 40284./8150672,
        'factorize' : True,
        'x_sec' : 900000*0.155*_picobarns, 
        'legendEntry' : plotter.process_QCD_EMenriched_Pt80to170.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_QCD_EMenriched_Pt80to170.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_QCD,
    },
##     'QCD_EM_Pt80to170_pat' : {
##         'datasetpath' : "/QCD_Pt-80to170_EMEnriched_TuneZ2_7TeV-pythia6/lantonel-patSkim_428_v1-0fa068f9514ddfc0e3f25d602948dee0/USER",
##         'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
##         'number_of_jobs' : 20,
##         'events_processed' : 8150672,
##         'skim_eff' : 40284./8150672,
##         'factorize' : True,
##         'x_sec' : 900000*0.155*_picobarns, 
##         'legendEntry' : plotter.process_QCD_EMenriched_Pt80to170.config_dqmHistPlotter.legendEntry.value(),
##         'type' : plotter.process_QCD_EMenriched_Pt80to170.config_dqmHistPlotter.type.value(),
##         'drawOption' : styles.drawOption_QCD,
##     },


#####embedded skims

##     'Ztautau_embedded_Run2011A_May10ReReco_skim' : {
##         'datasetpath' : "",
##         'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
##         'number_of_jobs' : 20,
##         'events_processed' : ,
##         'skim_eff' : ,
##         'type' : 'Data',
##         'hlt_paths' : ['*'],         
##         'factorize' : True,
##         'enableSysUncertainties' : False,
##         'applyVertexMultiplicityReweighting' : False,
##         'applyElectronTriggerEfficiencyCorrection' : False,
##         'applyElectronIsolationEfficiencyCorrection' : False,
##         'applyZrecoilCorrection' : False,
##         'legendEntry' : plotter.process_Ztautau.config_dqmHistPlotter.legendEntry.value(),
##         'type' : plotter.process_Ztautau.config_dqmHistPlotter.type.value(),
##         'drawOption' : styles.drawOption_Ztautau,        
##     }, 
##     'Ztautau_embedded_Run2011A_PR_v4_skim' : {
##         'datasetpath' : "",
##         'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
##         'number_of_jobs' : 100,
##         'events_processed' : ,
##         'skim_eff' : ,
##         'type' : 'Data',
##         'hlt_paths' : ['*'],         
##         'factorize' : True,
##         'enableSysUncertainties' : False,
##         'applyVertexMultiplicityReweighting' : False,
##         'applyElectronTriggerEfficiencyCorrection' : False,
##         'applyElectronIsolationEfficiencyCorrection' : False,
##         'applyZrecoilCorrection' : False,
##         'legendEntry' : plotter.process_Ztautau.config_dqmHistPlotter.legendEntry.value(),
##         'type' : plotter.process_Ztautau.config_dqmHistPlotter.type.value(),
##         'drawOption' : styles.drawOption_Ztautau,        
##     },
##     'Ztautau_embedded_Run2011A_05AugReReco_skim' : {
##         'datasetpath' : "",
##         'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
##         'number_of_jobs' : 50,
##         'events_processed' : ,
##         'skim_eff' : ,
##         'type' : 'Data',
##         'hlt_paths' : ['*'],         
##         'factorize' : True,
##         'enableSysUncertainties' : False,
##         'applyVertexMultiplicityReweighting' : False,
##         'applyElectronTriggerEfficiencyCorrection' : False,
##         'applyElectronIsolationEfficiencyCorrection' : False,
##         'applyZrecoilCorrection' : False,
##         'legendEntry' : plotter.process_Ztautau.config_dqmHistPlotter.legendEntry.value(),
##         'type' : plotter.process_Ztautau.config_dqmHistPlotter.type.value(),
##         'drawOption' : styles.drawOption_Ztautau,        
##     },
##     'Ztautau_embedded_Run2011A_03OctReReco_skim' : {
##         'datasetpath' : "",
##         'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
##         'number_of_jobs' : 50,
##         'events_processed' : ,
##         'skim_eff' : ,
##         'type' : 'Data',
##         'hlt_paths' : ['*'],         
##         'factorize' : True,
##         'enableSysUncertainties' : False,
##         'applyVertexMultiplicityReweighting' : False,
##         'applyElectronTriggerEfficiencyCorrection' : False,
##         'applyElectronIsolationEfficiencyCorrection' : False,
##         'applyZrecoilCorrection' : False,
##         'legendEntry' : plotter.process_Ztautau.config_dqmHistPlotter.legendEntry.value(),
##         'type' : plotter.process_Ztautau.config_dqmHistPlotter.type.value(),
##         'drawOption' : styles.drawOption_Ztautau,        
##     },    
##     'Ztautau_embedded_Run2011B_PR_v1_skim' : {
##         'datasetpath' : "",
##         'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
##         'number_of_jobs' : 50,
##         'events_processed' : ,
##         'skim_eff' : ,
##         'type' : 'Data',
##         'hlt_paths' : ['*'],         
##         'factorize' : True,
##         'enableSysUncertainties' : False,
##         'applyVertexMultiplicityReweighting' : False,
##         'applyElectronTriggerEfficiencyCorrection' : False,
##         'applyElectronIsolationEfficiencyCorrection' : False,
##         'applyZrecoilCorrection' : False,
##         'legendEntry' : plotter.process_Ztautau.config_dqmHistPlotter.legendEntry.value(),
##         'type' : plotter.process_Ztautau.config_dqmHistPlotter.type.value(),
##         'drawOption' : styles.drawOption_Ztautau,        
##     },    





    
}

# Define samples that get merged together
MERGE_SAMPLES = {
    'data' : {
        'samples' : [
   
    'data_TauPlusX_Run2011A_May10ReReco_pat',
    'data_TauPlusX_Run2011A_05AugReReco_pat',     



        ],
        'legendEntry' : 'DATA',
        'type' : 'Data',
        'drawOption' : styles.drawOption_Data
    },
    'qcdSum' : {
        'samples' : [
			'QCD_BCtoE_Pt20to30_skim',
			'QCD_BCtoE_Pt30to80_skim',
			'QCD_BCtoE_Pt80to170_skim',
			'QCD_EM_Pt20to30_skim',
			'QCD_EM_Pt30to80_skim',
			'QCD_EM_Pt80to170_skim'
        ],
        'legendEntry' : 'QCD',
        'type' : 'smMC', 
        'drawOption' : styles.drawOption_QCD,
    },
    'photonPlusJetsSum' : {
        'samples' : [
            'PhotonPlusJets_Pt15to30_skim',
			'PhotonPlusJets_Pt30to50_skim',
			'PhotonPlusJets_Pt50to80_skim'
        ],
        'legendEntry' : plotter.process_gammaPlusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : 'smMC', 
        'drawOption' : styles.drawOption_gammaPlusJets,
    },
    'ZeeSum' : {
        'samples' : [
            'DYeeM10to20_pythia_skim',
            'Zee_pythia_skim'
        ],
        'legendEntry' : plotter.process_Zee.config_dqmHistPlotter.legendEntry.value(),
        'type' : 'smMC', 
        'drawOption' : styles.drawOption_Zee,
    },
    'ZtautauSum' : {
        'samples' : [
            'DYtautauM10to20_powheg_skim',
            'Ztautau_powheg_skim',
            #'qqZll'
        ],
        'legendEntry' : plotter.process_Ztautau.config_dqmHistPlotter.legendEntry.value(),
        'type' : 'smMC', 
        'drawOption' : styles.drawOption_Ztautau,
    },
    'VVsum' : {
        'samples' : [
            'WW_skim',
            'WZ_skim',
            #'ZZ_skim'
        ],
        'legendEntry' : 'WW/WZ/ZZ',
        'type' : 'smMC',
        'drawOption' : styles.drawOption_VV
    },
    'smBgSum' : {
         'samples' : [
             'ZeeSum',
             'qcdSum',
             'photonPlusJetsSum',
             'WplusJets_madgraph_skim',
         ],
         'legendEntry' : 'SM',
         'type' : 'smMC',
         'drawOption' : styles.drawOption_QCD
    },
    'smSum' : {
         'samples' : [
             'ZtautauSum',
             'smBgSum'
         ],
         'legendEntry' : 'SM',
         'type' : 'smSumMC',
         'drawOption' : styles.drawOption_QCD
    },
    'smBgSum' : {
        'samples' : [
            'DYeeM10to20_pythia_skim',
            'Zee_pythia_skim',
            'QCD_BCtoE_Pt20to30_skim',
            'QCD_BCtoE_Pt30to80_skim',
            'QCD_BCtoE_Pt80to170_skim',
            'QCD_EM_Pt20to30_skim',
            'QCD_EM_Pt30to80_skim',
            'QCD_EM_Pt80to170_skim',
            #'PhotonPlusJets_Pt15to30_skim',
            #'PhotonPlusJets_Pt30to50_skim',
            #'PhotonPlusJets_Pt50to80_skim',   
            'WplusJets_madgraph_skim',
        ],
        'legendEntry' : 'SM',
        'type' : 'smMC',
        'drawOption' : styles.drawOption_QCD
    },
    'smSum' : {
        'samples' : [
            'DYtautauM10to20_powheg_skim',
            'Ztautau_powheg_skim',    
            'DYeeM10to20_pythia_skim',
            'Zee_pythia_skim',
            'QCD_BCtoE_Pt20to30_skim',
            'QCD_BCtoE_Pt30to80_skim',
            'QCD_BCtoE_Pt80to170_skim',
            'QCD_EM_Pt20to30_skim',
            'QCD_EM_Pt30to80_skim',
            'QCD_EM_Pt80to170_skim',
            #'PhotonPlusJets_Pt15to30_skim',
            #'PhotonPlusJets_Pt30to50_skim',
            #'PhotonPlusJets_Pt50to80_skim',   
            'WplusJets_madgraph_skim',    
        ],
        'legendEntry' : 'SM',
        'type' : 'smSumMC',
        'drawOption' : styles.drawOption_QCD
    }    
}

# List of all subsamples used in any plot job.
# i.e. if qcdSum is included in samples to plot
FLATTENED_SAMPLES_TO_PLOT = []
for sample in SAMPLES_TO_PLOT:
    if sample in MERGE_SAMPLES:
        for subsample in MERGE_SAMPLES[sample]['samples']:
            FLATTENED_SAMPLES_TO_PLOT.append(subsample)
    else:
        FLATTENED_SAMPLES_TO_PLOT.append(sample)

ALL_SAMPLES = {}
# Update to use the defaults if necessary
for sample in RECO_SAMPLES.keys():
    defaults = copy.copy(SAMPLE_DEFAULTS)
    defaults.update(RECO_SAMPLES[sample])
    RECO_SAMPLES[sample] = defaults
    # Combine MERGE and RECO samples in ALL samples
    # for simple access 
    ALL_SAMPLES.update(MERGE_SAMPLES)
    ALL_SAMPLES.update(RECO_SAMPLES)

recoSampleDefinitionsZtoElecTau = {
    'SAMPLES_TO_ANALYZE' : SAMPLES_TO_ANALYZE,
    'SAMPLES_TO_PLOT' : SAMPLES_TO_PLOT,
    'SAMPLES_TO_PRINT' : SAMPLES_TO_PRINT,
    'SAMPLE_DEFAULTS' : SAMPLE_DEFAULTS,
    'TARGET_LUMI' : TARGET_LUMI,
    'RECO_SAMPLES' : RECO_SAMPLES,
    'MERGE_SAMPLES' : MERGE_SAMPLES,
    'FLATTENED_SAMPLES_TO_PLOT' : FLATTENED_SAMPLES_TO_PLOT,
    'ALL_SAMPLES' : ALL_SAMPLES
}

