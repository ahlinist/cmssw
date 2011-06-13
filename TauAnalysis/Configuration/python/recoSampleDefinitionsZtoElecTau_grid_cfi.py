import FWCore.ParameterSet.Config as cms
import copy

import TauAnalysis.Configuration.plotterProcessDefinitions_cfi as plotter
import TauAnalysis.DQMTools.plotterStyleDefinitions_cfi as styles

# List of samples to run in the analysis
SAMPLES_TO_ANALYZE = [
    #'data_TauPlusX_Run2011A_May10ReReco_T3',
    'data_TauPlusX_Run2011A_May10ReReco',
    'data_TauPlusX_Run2011A_PR_v4',
##     'data_TauPlusX_Run2011A_PR_v1',
##     'data_TauPlusX_Run2011A_PR_v2',
##     'data_TauPlusX_Run2011A_PR_nonGolden',
    'Ztautau_powheg_T3',
##     'DYtautauM10to20_powheg_T3',
##     'qqZll',
##     'Zee_powheg_T3',
##     'DYeeM10to20_pythia_T3',
##     'PhotonPlusJets_Pt15to30_T3','PhotonPlusJets_Pt30to50_T3','PhotonPlusJets_Pt50to80_T3',
##     'QCD_BCtoE_Pt20to30_T3','QCD_BCtoE_Pt30to80_T3','QCD_BCtoE_Pt80to170_T3',
##     'QCD_EM_Pt20to30_T3','QCD_EM_Pt30to80_T3','QCD_EM_Pt80to170_T3',
##     'TTplusJets_madgraph_T3',
##     'WplusJets_madgraph_T3',
##     'WW_T3','WZ_T3','ZZ_T3',
] 

##SAMPLES_TO_ANALYZE = [
##     'data_TauPlusElec_Run2011A_PR_Pat',
##     'Ztautau_powheg_Pat',
##     'DYtautauM10to20_powheg_Pat',
##     'Zee_powheg_Pat',
##     'DYeeM10to20_pythia_Pat',
##     'PhotonPlusJets_Pt15to30_Pat','PhotonPlusJets_Pt30to50_Pat','PhotonPlusJets_Pt50to80_Pat',
##     'QCD_BCtoE_Pt20to30_Pat','QCD_BCtoE_Pt30to80_Pat','QCD_BCtoE_Pt80to170_Pat',
##     'QCD_EM_Pt20to30_Pat','QCD_EM_Pt30to80_Pat','QCD_EM_Pt80to170_Pat',
##     'WplusJets_madgraph_Pat',
##]


# List of samples to include in the final level plots.  May include selections
# from the MERGE_SAMPLES defined at the bottom.
SAMPLES_TO_PLOT = [
    'data', 
    'qcdSum', 
    'photonPlusJetsSum',
    'WplusJets_madgraph_Pat',
    #'TTplusJets_madgraph_Pat',
    #'VVsum',    
    'ZeeSum',
    'ZtautauSum',
    'smBgSum',
    'smSum'

]

SAMPLES_TO_PRINT = copy.copy(SAMPLES_TO_PLOT)

SAMPLE_DEFAULTS = {
    'dbs_url' : "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
    'conditions' : 'START311_V2::All',
    'genPhaseSpaceCut' : '',
    'factorize' : False,
    'enableSysUncertainties' : True,
    'lumi_mask' : '',
    'runselection' : '',
    'inputFileType' : 'AOD',
    ##'hlt_paths' : [ 'HLT_IsoEle12_PFTau15_v3','HLT_Ele12_SW_TighterEleId_L1R_v2'],  ## for 4_1_X, spring11
    'hlt_paths' : [ 'HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v2'],    ## for 4_2_X, summer11
    'SE_white_list' : '',
    'SE_black_list' : 'msu.ru',
    'disableDuplicateCheck' : False,
    'applyZrecoilCorrection' : False,
    'applyElectronTriggerEfficiencyCorrection' : False,
    'applyElectronIsolationEfficiencyCorrection' : False,
    'applyMuonTriggerEfficiencyCorrection' : False,
    'applyVertexMultiplicityReweighting' : False,
    'hlt' : cms.InputTag("TriggerResults", "", "HLT")
}

# Conversions to pico barns
_millibarns = 1.0e+9
_microbarns = 1.0e+6
_nanobarns  = 1.0e+3
_picobarns =  1.0
_femtobarns = 1.0e-3

# Integrated luminosity to normalize
# 204 May10ReReco + 294 PromptReco
TARGET_LUMI = (498)/_picobarns 

#--------------------------------------------------------------------------------
# NOTE: cross-sections for W and Z production are scaled to next-to-leading order values
#       documented on the wiki
#        https://alcaraz.web.cern.ch/alcaraz/CROSS_SECTIONS.txt
#--------------------------------------------------------------------------------

RECO_SAMPLES = {
    'data_TauPlusElec_Run2011A_PR_T3' : {   # 147.8/pb 162803-163757
        'datasetpath' : '/TauPlusX/jkolb-skimElecTau_413_v2-56283d7e938fe4e229d293a654f59050/USER',
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'conditions' : 'GR_P_V16::All',
        'number_of_jobs' : 200,
        'type' : 'Data',
        'drawOption' : styles.drawOption_Data,
        'enableSysUncertainties' : False,
        'applyZrecoilCorrection' : False,
        'applyElectronTriggerEfficiencyCorrection' : False,
        'applyElectronIsolationEfficiencyCorrection' : False,
        'applyMuonTriggerEfficiencyCorrection' : False,
        'applyVertexMultiplicityReweighting' : False,        
        'hlt_paths' : {
            'HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v1'     : '160431:MIN-161016:MAX',
            'HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v2'     : '161217:MIN-163261:MAX',
            'HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v4'     : '163269:MIN-163757:MAX'
        }
    },
    'data_TauPlusElec_Run2011A_PR_Pat' : {   # 147.8/pb
        'datasetpath' : '/TauPlusX/lantonel-patSkim_413_v5-b9ca0e2cc8621659caa5eafa9d155b85/USER',
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'conditions' : 'GR_P_V16::All',
        'number_of_jobs' : 100,
        'type' : 'Data',
        'drawOption' : styles.drawOption_Data,
        'enableSysUncertainties' : False,
        'applyZrecoilCorrection' : False,
        'applyElectronTriggerEfficiencyCorrection' : False,
        'applyElectronIsolationEfficiencyCorrection' : False,
        'applyMuonTriggerEfficiencyCorrection' : False,
        'applyVertexMultiplicityReweighting' : False,          
        'hlt_paths' : {
            'HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v1'     : '160431:MIN-161016:MAX',
            'HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v2'     : '161217:MIN-163261:MAX',
            'HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v4'     : '163269:MIN-163757:MAX'
        }
    },    
    'data_TauPlusX_Run2011A_PR_v1' : {  # 5.1/pb  160431-161016
        'datasetpath' : '/TauPlusX/Run2011A-PromptReco-v1/AOD', 
        'lumi_mask' : '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/Prompt/Cert_160404-163369_7TeV_PromptReco_Collisions11_JSON.txt',
        'conditions' : 'GR_P_V17::All',
        'number_of_jobs' : 20,
        'type' : 'Data',
        'drawOption' : styles.drawOption_Data,
        'enableSysUncertainties' : False,
        'applyZrecoilCorrection' : False,
        'applyElectronTriggerEfficiencyCorrection' : False,
        'applyElectronIsolationEfficiencyCorrection' : False,
        'applyMuonTriggerEfficiencyCorrection' : False,
        'applyVertexMultiplicityReweighting' : False,  
        'hlt_paths' : {
            'HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v1'     : '160431:MIN-161016:MAX',
            'HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v2'     : '161217:MIN-163261:MAX'
            }
        },
    'data_TauPlusX_Run2011A_PR_v2' : {  # 147.7/pb  162803-163757 ---> 186/pb  162803-163869
        'datasetpath' : '/TauPlusX/Run2011A-PromptReco-v2/AOD', 
        'lumi_mask' : '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/Prompt/Cert_160404-163869_7TeV_PromptReco_Collisions11_JSON.txt',
        'conditions' : 'GR_P_V17::All',
        'number_of_jobs' : 200,
        'type' : 'Data',
        'drawOption' : styles.drawOption_Data,
        'enableSysUncertainties' : False,
        'applyZrecoilCorrection' : False,
        'applyElectronTriggerEfficiencyCorrection' : False,
        'applyElectronIsolationEfficiencyCorrection' : False,
        'applyMuonTriggerEfficiencyCorrection' : False,
        'applyVertexMultiplicityReweighting' : False,  
        'hlt_paths' : {
            'HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v1'     : '160431:MIN-161016:MAX',
            'HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v2'     : '161217:MIN-163261:MAX',
            'HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v4'     : '163269:MIN-163869:MAX'
            }
        },
    'data_TauPlusX_Run2011A_PR_nonGolden' : { # 11.9/pb 161217-161312   
        'datasetpath' : '/TauPlusX/Run2011A-PromptReco-v1/AOD',
        'lumi_mask' : '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/Prompt/Cert_161079-161352_7TeV_PromptReco_Collisions11_JSON_noESpbl_v2.txt',
        'conditions' : 'GR_P_V16::All',
        'number_of_jobs' : 30,
        'type' : 'Data',
        'drawOption' : styles.drawOption_Data,
        'enableSysUncertainties' : False,
        'applyZrecoilCorrection' : False,
        'applyElectronTriggerEfficiencyCorrection' : False,
        'applyElectronIsolationEfficiencyCorrection' : False,
        'applyMuonTriggerEfficiencyCorrection' : False,
        'applyVertexMultiplicityReweighting' : False,  
        'hlt_paths' : {
            'HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v1'     : '160431:MIN-161016:MAX',
            'HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v2'     : '161217:MIN-163261:MAX',
            'HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v4'     : '163269:MIN-163869:MAX'
        }
    },
    'data_TauPlusX_Run2011A_PR_v4' : {  # 294/pb  165071-166502
        'datasetpath' : '/TauPlusX/Run2011A-PromptReco-v4/AOD', 
        'lumi_mask' : '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification//Collisions11/7TeV/Prompt/Cert_160404-166502_7TeV_PromptReco_Collisions11_JSON.txt',
        'conditions' : 'GR_P_V20::All',
        'number_of_jobs' : 300,
        'type' : 'Data',
        'drawOption' : styles.drawOption_Data,
        'enableSysUncertainties' : False,
	    'applyVertexMultiplicityReweighting' : False,
        'applyElectronTriggerEfficiencyCorrection' : False,
        'applyElectronIsolationEfficiencyCorrection' : False,
        'applyZrecoilCorrection' : False,
        'hlt_paths' : {
            'HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v6'     : '165071:MIN-165633:MAX',
            'HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v8'     : '165970:MIN-900000:MAX'
        }
    },
    'data_TauPlusX_Run2011A_May10ReReco' : {  #204.2/pb 160329-163869
        'datasetpath' : '/TauPlusX/Run2011A-May10ReReco-v1/AOD', 
        'lumi_mask' : '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification//Collisions11/7TeV/Reprocessing/Cert_160404-163869_7TeV_May10ReReco_Collisions11_JSON.txt',
        'conditions' : 'GR_R_42_V14::All',
        'number_of_jobs' : 150,
        'type' : 'Data',
        'drawOption' : styles.drawOption_Data,
        'enableSysUncertainties' : False,
	    'applyVertexMultiplicityReweighting' : False,
        'applyElectronTriggerEfficiencyCorrection' : False,
        'applyElectronIsolationEfficiencyCorrection' : False,
        'applyZrecoilCorrection' : False,
        'hlt_paths' : {
            'HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v1'     : '160431:MIN-161016:MAX',
            'HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v2'     : '161217:MIN-163261:MAX',
            'HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v4'     : '163269:MIN-163869:MAX'
            }
    },
    'data_TauPlusX_Run2011A_May10ReReco_T3' : {  #204.2/pb 160329-163869
        'datasetpath' : '/TauPlusX/jkolb-skimElecTau_423_May10ReReco_v3-2da1106465614f2b4aae43c293e2ca66/USER', 
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'lumi_mask' : '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification//Collisions11/7TeV/Reprocessing/Cert_160404-163869_7TeV_May10ReReco_Collisions11_JSON.txt',
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
        'hlt_paths' : {
            'HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v1'     : '160431:MIN-161016:MAX',
            'HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v2'     : '161217:MIN-163261:MAX',
            'HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v4'     : '163269:MIN-163869:MAX'
            }
    },
    'DYtautauM10to20_powheg' : {
        'datasetpath' : "/DYToTauTau_M-10To20_CT10_TuneZ2_7TeV-powheg-pythia-tauola/Spring11-PU_S1_START311_V1G1-v2/AODSIM",
        'events_processed' : 1991426,
        'skim_eff' : 1.0,
        'x_sec' : 1666*_picobarns,
        'legendEntry' : plotter.process_Ztautau.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Ztautau.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Ztautau,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'DYtautauM10to20_powheg_T3' : {
        'datasetpath' : "/DYToTauTau_M-10To20_CT10_TuneZ2_7TeV-powheg-pythia-tauola/jkolb-skimElecTau_413_v1-eae887ba91c6c27e2f0c00f8aee7bf0a/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 2,
        'events_processed' : 1991426,
        'skim_eff' : 775./1991426,
        'x_sec' : 1666*_picobarns,
        'legendEntry' : plotter.process_Ztautau.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Ztautau.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Ztautau,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'DYtautauM10to20_powheg_Pat' : {
        'datasetpath' : "/DYToTauTau_M-10To20_CT10_TuneZ2_7TeV-powheg-pythia-tauola/lantonel-patSkim_413_v3-c9dbc6c7165a3d4f1d089077c1253e67/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 2,
        'events_processed' : 1991426,
        'skim_eff' : 775./1991426,
        'x_sec' : 1666*_picobarns,
        'legendEntry' : plotter.process_Ztautau.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Ztautau.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Ztautau,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },      
    'Ztautau_powheg' : {
        'datasetpath' : "/DYToTauTau_M-20_CT10_TuneZ2_7TeV-powheg-pythia-tauola/Spring11-PU_S1_START311_V1G1-v2/AODSIM",
        'events_processed' : 1994719,
        'skim_eff' : 1.0,
        'x_sec' : 1666*_picobarns,
        'legendEntry' : plotter.process_Ztautau.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Ztautau.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Ztautau,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'Ztautau_powheg_T3' : {
        'datasetpath' : "/DYToTauTau_M-20_CT10_TuneZ2_7TeV-powheg-pythia-tauola/jkolb-skimElecTau_413_v2-eae887ba91c6c27e2f0c00f8aee7bf0a/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 20,
        'events_processed' : 1994719,
        'skim_eff' : 89799./1994719,
        'x_sec' : 1666*_picobarns,
        'legendEntry' : plotter.process_Ztautau.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Ztautau.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Ztautau,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'Ztautau_powheg_Pat' : {
        'datasetpath' : "/DYToTauTau_M-20_CT10_TuneZ2_7TeV-powheg-pythia-tauola/lantonel-patSkim_413_v3-c9dbc6c7165a3d4f1d089077c1253e67/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 20,
        'events_processed' : 1994719,
        'skim_eff' : 89799./1994719,
        'x_sec' : 1666*_picobarns,
        'legendEntry' : plotter.process_Ztautau.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Ztautau.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Ztautau,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },     
    'qqZll' : {
        'datasetpath' : "/VQQJetsToLL_TuneD6T_7TeV-madgraph-tauola/Spring11-PU_S1_START311_V1G1-v1/AODSIM",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 738233,
        'skim_eff' : 1.0,
        'x_sec' : 36.*_picobarns,
        'legendEntry' : plotter.process_Ztautau.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Ztautau.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Ztautau,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    }, 
    'qqZll_T3' : {
        'datasetpath' : "",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 738233,
        'skim_eff' : 1.0,
        'x_sec' : 36.*_picobarns,
        'legendEntry' : plotter.process_Ztautau.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Ztautau.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Ztautau,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    }, 
    'DYeeM10to20_pythia' : {
        'datasetpath' : "/DYToEE_M-10To20_TuneZ2_7TeV-pythia6/Spring11-PU_S1_START311_V1G1-v1/AODSIM",
        'events_processed' : 1933000,
        'skim_eff' : 1.0,
        'x_sec' : 1.282*2659*_picobarns, # Z + jets correction factor for NLO/LO cross-sections = 1.282 (k-factor for mMuMu > 20 GeV)
        'legendEntry' : plotter.process_Zee.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Zee.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Zee,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'DYeeM10to20_pythia_T3' : {
        'datasetpath' : "/DYToEE_M-10To20_TuneZ2_7TeV-pythia6/jkolb-skimElecTau_413_v1-eae887ba91c6c27e2f0c00f8aee7bf0a/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 10,
        'events_processed' : 1933000,
        'skim_eff' : 31175./1933000,
        'x_sec' : 1.282*2659*_picobarns, # Z + jets correction factor for NLO/LO cross-sections = 1.282 (k-factor for mMuMu > 20 GeV)
        'legendEntry' : plotter.process_Zee.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Zee.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Zee,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'DYeeM10to20_pythia_Pat' : {
        'datasetpath' : "/DYToEE_M-10To20_TuneZ2_7TeV-pythia6/lantonel-patSkim_413_v3-c9dbc6c7165a3d4f1d089077c1253e67/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 10,
        'events_processed' : 1933000,
        'skim_eff' : 31175./1933000,
        'x_sec' : 1.282*2659*_picobarns, # Z + jets correction factor for NLO/LO cross-sections = 1.282 (k-factor for mMuMu > 20 GeV)
        'legendEntry' : plotter.process_Zee.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Zee.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Zee,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },    
    'Zee_powheg' : {
        'datasetpath' : "/DYToEE_M-20_CT10_TuneZ2_7TeV-powheg-pythia/Spring11-PU_S1_START311_V1G1-v1/AODSIM",
        'events_processed' : 1992276,
        'skim_eff' : 1.0,
        'x_sec' : 1666*_picobarns,
        'legendEntry' : plotter.process_Zee.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Zee.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Zee,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'Zee_powheg_T3' : {
        'datasetpath' : "/DYToEE_M-20_CT10_TuneZ2_7TeV-powheg-pythia/jkolb-skimElecTau_413_v1-eae887ba91c6c27e2f0c00f8aee7bf0a/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 50,
        'events_processed' : 1992276,
        'skim_eff' : 819311.0/1992276,
        'x_sec' : 1666*_picobarns,
        'legendEntry' : plotter.process_Zee.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Zee.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Zee,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'Zee_powheg_Pat' : {
        'datasetpath' : "/DYToEE_M-20_CT10_TuneZ2_7TeV-powheg-pythia/lantonel-patSkim_413_v5-c9dbc6c7165a3d4f1d089077c1253e67/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 50,
        'events_processed' : 1992276,
        'skim_eff' : 819311.0/1992276,
        'x_sec' : 1666*_picobarns,
        'legendEntry' : plotter.process_Zee.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Zee.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Zee,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },      
    'WplusJets_madgraph' : {
        'datasetpath' : "/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Spring11-PU_S1_START311_V1G1-v1/AODSIM",
        'events_processed' : 15110974,
        'skim_eff' : 1.0,
        'x_sec' : 31314*_picobarns,
        'legendEntry' : plotter.process_WplusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_WplusJets.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_WplusJets,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'WplusJets_madgraph_T3' : {
        'datasetpath' : "/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/jkolb-skimElecTau_413_v1-eae887ba91c6c27e2f0c00f8aee7bf0a/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 40,
        'events_processed' : 15110974,
        'skim_eff' : 738862./15110974,
        'x_sec' : 31314*_picobarns,
        'legendEntry' : plotter.process_WplusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_WplusJets.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_WplusJets,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'WplusJets_madgraph_Pat' : {
        'datasetpath' : "/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/lantonel-patSkim_413_v3-c9dbc6c7165a3d4f1d089077c1253e67/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 100,
        'events_processed' : 15110974,
        'skim_eff' : 738862./15110974,
        'x_sec' : 31314*_picobarns,
        'legendEntry' : plotter.process_WplusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_WplusJets.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_WplusJets,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },    
    'WW' : {
        'datasetpath' : "/WWtoAnything_TuneZ2_7TeV-pythia6-tauola/Spring11-PU_S1_START311_V1G1-v1/AODSIM",
        'events_processed' : 2061760,
        'skim_eff' : 1.0,
        'x_sec' : 43.0*_picobarns, # NLO cross-section from https://twiki.cern.ch/twiki/bin/viewauth/CMS/StandardModelCrossSections
        'legendEntry' : plotter.process_VV.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_VV.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_VV,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'WW_T3' : {
        'datasetpath' : "/WWtoAnything_TuneZ2_7TeV-pythia6-tauola/jkolb-skimElecTau_413_v1-eae887ba91c6c27e2f0c00f8aee7bf0a/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 20,
        'events_processed' : 2061760,
        'skim_eff' : 193655./2061760,
        'x_sec' : 43.0*_picobarns, # NLO cross-section from https://twiki.cern.ch/twiki/bin/viewauth/CMS/StandardModelCrossSections
        'legendEntry' : plotter.process_VV.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_VV.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_VV,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'WW_Pat' : {
        'datasetpath' : "/WWtoAnything_TuneZ2_7TeV-pythia6-tauola/lantonel-patSkim_413_v3-c9dbc6c7165a3d4f1d089077c1253e67/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 50,
        'events_processed' : 2061760,
        'skim_eff' : 193655./2061760,
        'x_sec' : 43.0*_picobarns, # NLO cross-section from https://twiki.cern.ch/twiki/bin/viewauth/CMS/StandardModelCrossSections
        'legendEntry' : plotter.process_VV.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_VV.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_VV,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },      
    'WZ' : {
        'datasetpath' : "/WZtoAnything_TuneZ2_7TeV-pythia6-tauola/Spring11-PU_S1_START311_V1G1-v1/AODSIM",
        'events_processed' : 2108416,
        'skim_eff' : 1.0,
        'x_sec' : 18.2*_picobarns, # NLO cross-section from https://twiki.cern.ch/twiki/bin/viewauth/CMS/StandardModelCrossSections
        'legendEntry' : plotter.process_VV.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_VV.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_VV,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'WZ_T3' : {
        'datasetpath' : "/WZtoAnything_TuneZ2_7TeV-pythia6-tauola/jkolb-skimElecTau_413_v1-eae887ba91c6c27e2f0c00f8aee7bf0a/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 20,
        'events_processed' : 2108416,
        'skim_eff' : 152293./2108416,
        'x_sec' : 18.2*_picobarns, # NLO cross-section from https://twiki.cern.ch/twiki/bin/viewauth/CMS/StandardModelCrossSections
        'legendEntry' : plotter.process_VV.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_VV.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_VV,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'WZ_Pat' : {
        'datasetpath' : "/WZtoAnything_TuneZ2_7TeV-pythia6-tauola/lantonel-patSkim_413_v3-c9dbc6c7165a3d4f1d089077c1253e67/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 50,
        'events_processed' : 2108416,
        'skim_eff' : 152293./2108416,
        'x_sec' : 18.2*_picobarns, # NLO cross-section from https://twiki.cern.ch/twiki/bin/viewauth/CMS/StandardModelCrossSections
        'legendEntry' : plotter.process_VV.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_VV.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_VV,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },     
    'ZZ' : {
        'datasetpath' : "/ZZtoAnything_TuneZ2_7TeV-pythia6-tauola/Spring11-PU_S1_START311_V1G1-v1/AODSIM",
        'events_processed' : 2108608,
        'skim_eff' : 1.0,
        'x_sec' : 5.9*_picobarns, # NLO cross-section from https://twiki.cern.ch/twiki/bin/viewauth/CMS/StandardModelCrossSections
        'legendEntry' : plotter.process_VV.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_VV.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_VV,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'ZZ_T3' : {
        'datasetpath' : "/ZZtoAnything_TuneZ2_7TeV-pythia6-tauola/jkolb-skimElecTau_413_v1-eae887ba91c6c27e2f0c00f8aee7bf0a/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 20,
        'events_processed' : 2108608,
        'skim_eff' : 151802./2108608,
        'x_sec' : 5.9*_picobarns, # NLO cross-section from https://twiki.cern.ch/twiki/bin/viewauth/CMS/StandardModelCrossSections
        'legendEntry' : plotter.process_VV.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_VV.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_VV,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'ZZ_Pat' : {
        'datasetpath' : "/ZZtoAnything_TuneZ2_7TeV-pythia6-tauola/lantonel-patSkim_413_v3-c9dbc6c7165a3d4f1d089077c1253e67/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 50,
        'events_processed' : 2108608,
        'skim_eff' : 151802./2108608,
        'x_sec' : 5.9*_picobarns, # NLO cross-section from https://twiki.cern.ch/twiki/bin/viewauth/CMS/StandardModelCrossSections
        'legendEntry' : plotter.process_VV.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_VV.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_VV,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },     
    'TTplusJets_madgraph' : {
        'datasetpath' : "/TTJets_TuneZ2_7TeV-madgraph-tauola/Spring11-PU_S1_START311_V1G1-v1/AODSIM",
        'events_processed' : 1164208,
        'skim_eff' : 1.0,
        'x_sec' : 157.5*_picobarns, # NLO cross-section from https://twiki.cern.ch/twiki/bin/viewauth/CMS/StandardModelCrossSections
        'legendEntry' : plotter.process_TTplusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_TTplusJets.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_TTplusJets,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'TTplusJets_madgraph_T3' : {
        'datasetpath' : "/TTJets_TuneZ2_7TeV-madgraph-tauola/jkolb-skimElecTau_413_v1-eae887ba91c6c27e2f0c00f8aee7bf0a/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 20,
        'events_processed' : 1164208,
        'skim_eff' : 172976./1164208,
        'x_sec' : 157.5*_picobarns, # NLO cross-section from https://twiki.cern.ch/twiki/bin/viewauth/CMS/StandardModelCrossSections
        'legendEntry' : plotter.process_TTplusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_TTplusJets.config_dqmHistPlotter.type.value(),
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'TTplusJets_madgraph_Pat' : {
        'datasetpath' : "/TTJets_TuneZ2_7TeV-madgraph-tauola/lantonel-patSkim_413_v3-c9dbc6c7165a3d4f1d089077c1253e67/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 20,
        'events_processed' : 1164208,
        'skim_eff' : 172976./1164208,
        'x_sec' : 157.5*_picobarns, # NLO cross-section from https://twiki.cern.ch/twiki/bin/viewauth/CMS/StandardModelCrossSections
        'legendEntry' : plotter.process_TTplusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_TTplusJets.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_TTplusJets,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },     
    'PhotonPlusJets_Pt15to30' : {
        'datasetpath' : "/G_Pt_15to30_TuneZ2_7TeV_pythia6/Spring11-PU_S1_START311_V1G1-v1/AODSIM",
        'events_processed' : 1025840,
        'skim_eff' : 1.0,
        'factorize' : True,
        'x_sec' : 171700*_picobarns, 
        'legendEntry' : plotter.process_gammaPlusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_gammaPlusJets.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_gammaPlusJets,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'PhotonPlusJets_Pt15to30_T3' : {
        'datasetpath' : "/G_Pt_15to30_TuneZ2_7TeV_pythia6/jkolb-skimElecTau_413_v1-eae887ba91c6c27e2f0c00f8aee7bf0a/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 10,
        'events_processed' : 1025840,
        'skim_eff' : 22531./1025840,
        'factorize' : True,
        'x_sec' : 171700*_picobarns, 
        'legendEntry' : plotter.process_gammaPlusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_gammaPlusJets.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_gammaPlusJets,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'PhotonPlusJets_Pt15to30_PAT' : {
        'datasetpath' : "/G_Pt_15to30_TuneZ2_7TeV_pythia6/lantonel-patSkim_413_v3-c9dbc6c7165a3d4f1d089077c1253e67/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'inputFileType' : 'PATTuple',
        'number_of_jobs' : 5,
        'events_processed' : 1025840,
        'skim_eff' : 22531./1025840,
        'factorize' : True,
        'x_sec' : 171700*_picobarns, 
        'legendEntry' : plotter.process_gammaPlusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_gammaPlusJets.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_gammaPlusJets,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'PhotonPlusJets_Pt30to50' : {
        'datasetpath' : "/G_Pt_30to50_TuneZ2_7TeV_pythia6/Spring11-PU_S1_START311_V1G1-v1/AODSIM",
        'events_processed' : 1025480,
        'skim_eff' : 1.0,
        'factorize' : True,
        'x_sec' : 16690*_picobarns, 
        'legendEntry' : plotter.process_gammaPlusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_gammaPlusJets.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_gammaPlusJets,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'PhotonPlusJets_Pt30to50_T3' : {
        'datasetpath' : "/G_Pt_30to50_TuneZ2_7TeV_pythia6/jkolb-skimElecTau_413_v1-eae887ba91c6c27e2f0c00f8aee7bf0a/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 10,
        'events_processed' : 1025480,
        'skim_eff' : 34061./1025480,
        'factorize' : True,
        'x_sec' : 16690*_picobarns, 
        'legendEntry' : plotter.process_gammaPlusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_gammaPlusJets.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_gammaPlusJets,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'PhotonPlusJets_Pt30to50_Pat' : {
        'datasetpath' : "/G_Pt_30to50_TuneZ2_7TeV_pythia6/lantonel-patSkim_413_v3-c9dbc6c7165a3d4f1d089077c1253e67/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 10,
        'events_processed' : 1025480,
        'skim_eff' : 34061./1025480,
        'factorize' : True,
        'x_sec' : 16690*_picobarns, 
        'legendEntry' : plotter.process_gammaPlusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_gammaPlusJets.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_gammaPlusJets,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },     
    'PhotonPlusJets_Pt50to80' : {
        'datasetpath' : "/G_Pt_50to80_TuneZ2_7TeV_pythia6/Spring11-PU_S1_START311_V1G1-v1/AODSIM",
        'events_processed' : 1024608,
        'skim_eff' : 1.0,
        'factorize' : True,
        'x_sec' : 2722*_picobarns, 
        'legendEntry' : plotter.process_gammaPlusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_gammaPlusJets.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_gammaPlusJets,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'PhotonPlusJets_Pt50to80_T3' : {
        'datasetpath' : "/G_Pt_50to80_TuneZ2_7TeV_pythia6/jkolb-skimElecTau_413_v1-eae887ba91c6c27e2f0c00f8aee7bf0a/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 10,
        'events_processed' : 1025840,
        'skim_eff' : 36691./1025840,
        'factorize' : True,
        'x_sec' : 2722*_picobarns, 
        'legendEntry' : plotter.process_gammaPlusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_gammaPlusJets.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_gammaPlusJets,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'PhotonPlusJets_Pt50to80_Pat' : {
        'datasetpath' : "/G_Pt_50to80_TuneZ2_7TeV_pythia6/lantonel-patSkim_413_v3-c9dbc6c7165a3d4f1d089077c1253e67/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 10,
        'events_processed' : 1025840,
        'skim_eff' : 36691./1025840,
        'factorize' : True,
        'x_sec' : 2722*_picobarns, 
        'legendEntry' : plotter.process_gammaPlusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_gammaPlusJets.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_gammaPlusJets,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },     
    'QCD_BCtoE_Pt20to30' : {
        'datasetpath' : "/QCD_Pt-20to30_BCtoE_TuneZ2_7TeV-pythia6/Spring11-PU_S1_START311_V1G1-v1/AODSIM",
        'events_processed' : 2243439,
        'skim_eff' : 1,
        'factorize' : True,
        'x_sec' : 236000000*0.00056*_picobarns, 
        'legendEntry' : plotter.process_QCD_BCtoE_Pt20to30.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_QCD_BCtoE_Pt20to30.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_QCD,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'QCD_BCtoE_Pt20to30_T3' : {
        'datasetpath' : "/QCD_Pt-20to30_BCtoE_TuneZ2_7TeV-pythia6/jkolb-skimElecTau_413_v1-eae887ba91c6c27e2f0c00f8aee7bf0a/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 50,
        'events_processed' : 2243439,
        'skim_eff' : 237477./2243439,
        'factorize' : True,
        'x_sec' : 236000000*0.00056*_picobarns, 
        'legendEntry' : plotter.process_QCD_BCtoE_Pt20to30.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_QCD_BCtoE_Pt20to30.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_QCD,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'QCD_BCtoE_Pt20to30_Pat' : {
        'datasetpath' : "/QCD_Pt-20to30_BCtoE_TuneZ2_7TeV-pythia6/lantonel-patSkim_413_v3-c9dbc6c7165a3d4f1d089077c1253e67/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 50,
        'events_processed' : 2243439,
        'skim_eff' : 237477./2243439,
        'factorize' : True,
        'x_sec' : 236000000*0.00056*_picobarns, 
        'legendEntry' : plotter.process_QCD_BCtoE_Pt20to30.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_QCD_BCtoE_Pt20to30.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_QCD,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },     
    'QCD_BCtoE_Pt30to80' : {
        'datasetpath' : "/QCD_Pt-30to80_BCtoE_TuneZ2_7TeV-pythia6/Spring11-PU_S1_START311_V1G1-v1/AODSIM",
        'events_processed' : 1995502,
        'skim_eff' : 1,
		'factorize' : True,
        'x_sec' : 59480000*0.0023*_picobarns, 
        'legendEntry' : plotter.process_QCD_BCtoE_Pt30to80.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_QCD_BCtoE_Pt30to80.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_QCD,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'QCD_BCtoE_Pt30to80_T3' : {
        'datasetpath' : "/QCD_Pt-30to80_BCtoE_TuneZ2_7TeV-pythia6/jkolb-skimElecTau_413_v1-eae887ba91c6c27e2f0c00f8aee7bf0a/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 50,
        'events_processed' : 1995502,
        'skim_eff' : 228156./1995502,
        'factorize' : True,
        'x_sec' : 59480000*0.0023*_picobarns, 
        'legendEntry' : plotter.process_QCD_BCtoE_Pt30to80.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_QCD_BCtoE_Pt30to80.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_QCD,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'QCD_BCtoE_Pt30to80_Pat' : {
        'datasetpath' : "/QCD_Pt-30to80_BCtoE_TuneZ2_7TeV-pythia6/lantonel-patSkim_413_v3-c9dbc6c7165a3d4f1d089077c1253e67/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 50,
        'events_processed' : 1995502,
        'skim_eff' : 228156./1995502,
        'factorize' : True,
        'x_sec' : 59480000*0.0023*_picobarns, 
        'legendEntry' : plotter.process_QCD_BCtoE_Pt30to80.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_QCD_BCtoE_Pt30to80.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_QCD,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },      
    'QCD_BCtoE_Pt80to170' : {
        'datasetpath' : "/QCD_Pt-80to170_BCtoE_TuneZ2_7TeV-pythia6/Spring11-PU_S1_START311_V1G1-v1/AODSIM",
        'events_processed' : 1043390,
        'skim_eff' : 1.0,
		'factorize' : True,
        'x_sec' : 900000*0.0104*_picobarns, 
        'legendEntry' : plotter.process_QCD_BCtoE_Pt80to170.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_QCD_BCtoE_Pt80to170.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_QCD,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'QCD_BCtoE_Pt80to170_T3' : {
        'datasetpath' : "/QCD_Pt-80to170_BCtoE_TuneZ2_7TeV-pythia6/jkolb-skimElecTau_413_v1-eae887ba91c6c27e2f0c00f8aee7bf0a/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 10,
        'events_processed' : 1043390,
        'skim_eff' : 58871./1043390,
        'factorize' : True,
        'x_sec' : 900000*0.0104*_picobarns, 
        'legendEntry' : plotter.process_QCD_BCtoE_Pt80to170.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_QCD_BCtoE_Pt80to170.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_QCD,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'QCD_BCtoE_Pt80to170_Pat' : {
        'datasetpath' : "/QCD_Pt-80to170_BCtoE_TuneZ2_7TeV-pythia6/lantonel-patSkim_413_v3-c9dbc6c7165a3d4f1d089077c1253e67/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 10,
        'events_processed' : 1043390,
        'skim_eff' : 58871./1043390,
        'factorize' : True,
        'x_sec' : 900000*0.0104*_picobarns, 
        'legendEntry' : plotter.process_QCD_BCtoE_Pt80to170.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_QCD_BCtoE_Pt80to170.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_QCD,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },    
    'QCD_EM_Pt20to30' : {
        'datasetpath' : "/QCD_Pt-20to30_EMEnriched_TuneZ2_7TeV-pythia6/Spring11-PU_S1_START311_V1G1-v1/AODSIM",
        'events_processed' : 36136246,
        'skim_eff' : 1.0,
        'factorize' : True,
        'x_sec' : 236000000*0.0104*_picobarns, 
        'legendEntry' : plotter.process_QCD_EMenriched_Pt20to30.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_QCD_EMenriched_Pt20to30.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_QCD,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'QCD_EM_Pt20to30_T3' : {
        'datasetpath' : "/QCD_Pt-20to30_EMEnriched_TuneZ2_7TeV-pythia6/jkolb-skimElecTau_413_v1-eae887ba91c6c27e2f0c00f8aee7bf0a/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 30,
        'events_processed' : 36136246,
        'skim_eff' : 434178./36136246,
        'factorize' : True,
        'x_sec' : 236000000*0.0104*_picobarns, 
        'legendEntry' : plotter.process_QCD_EMenriched_Pt20to30.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_QCD_EMenriched_Pt20to30.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_QCD,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'QCD_EM_Pt20to30_Pat' : {
        'datasetpath' : "/QCD_Pt-20to30_EMEnriched_TuneZ2_7TeV-pythia6/lantonel-patSkim_413_v3-c9dbc6c7165a3d4f1d089077c1253e67/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 30,
        'events_processed' : 36136246,
        'skim_eff' : 434178./36136246,
        'factorize' : True,
        'x_sec' : 236000000*0.0104*_picobarns, 
        'legendEntry' : plotter.process_QCD_EMenriched_Pt20to30.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_QCD_EMenriched_Pt20to30.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_QCD,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },    
    'QCD_EM_Pt30to80' : {
        'datasetpath' : "/QCD_Pt-30to80_EMEnriched_TuneZ2_7TeV-pythia6/Spring11-PU_S1_START311_V1G1-v1/AODSIM",
        'events_processed' : 70708892,
        'skim_eff' : 1.0,
        'factorize' : True,
        'x_sec' : 59480000*0.065*_picobarns, 
        'legendEntry' : plotter.process_QCD_EMenriched_Pt30to80.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_QCD_EMenriched_Pt30to80.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_QCD,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'QCD_EM_Pt30to80_T3' : {
        'datasetpath' : "/QCD_Pt-30to80_EMEnriched_TuneZ2_7TeV-pythia6/jkolb-skimElecTau_413_v1-eae887ba91c6c27e2f0c00f8aee7bf0a/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 80,
        'events_processed' : 70708892,
        'skim_eff' : 736277./70708892,
        'factorize' : True,
        'x_sec' : 59480000*0.065*_picobarns, 
        'legendEntry' : plotter.process_QCD_EMenriched_Pt30to80.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_QCD_EMenriched_Pt30to80.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_QCD,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'QCD_EM_Pt30to80_Pat' : {
        'datasetpath' : "/QCD_Pt-30to80_EMEnriched_TuneZ2_7TeV-pythia6/lantonel-patSkim_413_v3-c9dbc6c7165a3d4f1d089077c1253e67/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 80,
        'events_processed' : 70708892,
        'skim_eff' : 736277./70708892,
        'factorize' : True,
        'x_sec' : 59480000*0.065*_picobarns, 
        'legendEntry' : plotter.process_QCD_EMenriched_Pt30to80.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_QCD_EMenriched_Pt30to80.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_QCD,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },     
    'QCD_EM_Pt80to170' : {
        'datasetpath' : "/QCD_Pt-80to170_EMEnriched_TuneZ2_7TeV-pythia6/Spring11-PU_S1_START311_V1G1-v1/AODSIM",
        'events_processed' : 8069591,
        'skim_eff' : 1.0,
        'factorize' : True,
        'x_sec' : 900000*0.155*_picobarns, 
        'legendEntry' : plotter.process_QCD_EMenriched_Pt80to170.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_QCD_EMenriched_Pt80to170.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_QCD,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'QCD_EM_Pt80to170_T3' : {
        'datasetpath' : "/QCD_Pt-80to170_EMEnriched_TuneZ2_7TeV-pythia6/jkolb-skimElecTau_413_v1-eae887ba91c6c27e2f0c00f8aee7bf0a/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 20,
        'events_processed' : 8069591,
        'skim_eff' : 74205./8069591,
        'factorize' : True,
        'x_sec' : 900000*0.155*_picobarns, 
        'legendEntry' : plotter.process_QCD_EMenriched_Pt80to170.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_QCD_EMenriched_Pt80to170.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_QCD,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'QCD_EM_Pt80to170_Pat' : {
        'datasetpath' : "/QCD_Pt-80to170_EMEnriched_TuneZ2_7TeV-pythia6/lantonel-patSkim_413_v1-c9dbc6c7165a3d4f1d089077c1253e67/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 20,
        'events_processed' : 8069591,
        'skim_eff' : 74205./8069591,
        'factorize' : True,
        'x_sec' : 900000*0.155*_picobarns, 
        'legendEntry' : plotter.process_QCD_EMenriched_Pt80to170.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_QCD_EMenriched_Pt80to170.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_QCD,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    }      
}

# Define samples that get merged together
MERGE_SAMPLES = {
    'data' : {
        'samples' : [
               'data_TauPlusElec_Run2011A_PR_Pat'
##             'data_TauPlusX_Run2011A_PR_v1',
##             'data_TauPlusX_Run2011A_PR_v2',
##             'data_TauPlusX_Run2011A_PR_nonGolden'
        ],
        'legendEntry' : 'DATA',
        'type' : 'Data',
        'drawOption' : styles.drawOption_Data
    },
    'qcdSum' : {
        'samples' : [
			'QCD_BCtoE_Pt20to30_Pat',
			'QCD_BCtoE_Pt30to80_Pat',
			'QCD_BCtoE_Pt80to170_Pat',
			'QCD_EM_Pt20to30_Pat',
			'QCD_EM_Pt30to80_Pat',
			'QCD_EM_Pt80to170_Pat'
        ],
        'legendEntry' : 'QCD',
        'type' : 'smMC', 
        'drawOption' : styles.drawOption_QCD,
    },
    'photonPlusJetsSum' : {
        'samples' : [
                        'PhotonPlusJets_Pt15to30_Pat',
			'PhotonPlusJets_Pt30to50_Pat',
			'PhotonPlusJets_Pt50to80_Pat'
        ],
        'legendEntry' : plotter.process_gammaPlusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : 'smMC', 
        'drawOption' : styles.drawOption_gammaPlusJets,
    },
    'ZeeSum' : {
        'samples' : [
            'DYeeM10to20_pythia_Pat',
            'Zee_powheg_Pat'
        ],
        'legendEntry' : plotter.process_Zee.config_dqmHistPlotter.legendEntry.value(),
        'type' : 'smMC', 
        'drawOption' : styles.drawOption_Zee,
    },
    'ZtautauSum' : {
        'samples' : [
            'DYtautauM10to20_powheg_Pat',
            'Ztautau_powheg_Pat',
            #'qqZll'
        ],
        'legendEntry' : plotter.process_Ztautau.config_dqmHistPlotter.legendEntry.value(),
        'type' : 'smMC', 
        'drawOption' : styles.drawOption_Ztautau,
    },
    'VVsum' : {
        'samples' : [
            'WW_T3',
            'WZ_T3',
            'ZZ_T3'
        ],
        'legendEntry' : 'WW/WZ/ZZ',
        'type' : 'smMC',
        'drawOption' : styles.drawOption_VV
    },
##     'smBgSum' : {
##         'samples' : [
##             'ZeeSum',
##             'qcdSum',
##             'photonPlusJetsSum',
##             'WplusJets_madgraph_Pat',
##         ],
##         'legendEntry' : 'SM',
##         'type' : 'smMC',
##         'drawOption' : styles.drawOption_QCD
##     },
##     'smSum' : {
##         'samples' : [
##             'ZtautauSum',
##             'smBgSum'
##         ],
##         'legendEntry' : 'SM',
##         'type' : 'smSumMC',
##         'drawOption' : styles.drawOption_QCD
##     }
    'smBgSum' : {
        'samples' : [
            'DYeeM10to20_pythia_Pat',
            'Zee_powheg_Pat',
            'QCD_BCtoE_Pt20to30_Pat',
            'QCD_BCtoE_Pt30to80_Pat',
            'QCD_BCtoE_Pt80to170_Pat',
            'QCD_EM_Pt20to30_Pat',
            'QCD_EM_Pt30to80_Pat',
            'QCD_EM_Pt80to170_Pat',
            'PhotonPlusJets_Pt15to30_Pat',
            'PhotonPlusJets_Pt30to50_Pat',
            'PhotonPlusJets_Pt50to80_Pat',   
            'WplusJets_madgraph_Pat',
        ],
        'legendEntry' : 'SM',
        'type' : 'smMC',
        'drawOption' : styles.drawOption_QCD
    },
    'smSum' : {
        'samples' : [
            'DYtautauM10to20_powheg_Pat',
            'Ztautau_powheg_Pat',    
            'DYeeM10to20_pythia_Pat',
            'Zee_powheg_Pat',
            'QCD_BCtoE_Pt20to30_Pat',
            'QCD_BCtoE_Pt30to80_Pat',
            'QCD_BCtoE_Pt80to170_Pat',
            'QCD_EM_Pt20to30_Pat',
            'QCD_EM_Pt30to80_Pat',
            'QCD_EM_Pt80to170_Pat',
            'PhotonPlusJets_Pt15to30_Pat',
            'PhotonPlusJets_Pt30to50_Pat',
            'PhotonPlusJets_Pt50to80_Pat',   
            'WplusJets_madgraph_Pat',    
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

