import FWCore.ParameterSet.Config as cms
import copy

import TauAnalysis.Configuration.plotterProcessDefinitions_cfi as plotter
import TauAnalysis.DQMTools.plotterStyleDefinitions_cfi as styles

# List of samples to run in the analysis
SAMPLES_TO_ANALYZE = [
    'data_Mu_Run2010A_Nov4ReReco',
    'data_Mu_Run2010B_Nov4ReReco',
    'ZtautauPU156bx',
    'DYtautauM10to20PU156bx_pythiaZ2', 'ZtautauPU156bx_pythiaZ2',
    #'Zmumu_pythia', 'Zee_pythia',
    'qqZll',
    'qqZllPU156bx', # EK: not at any site as of Dec 23
    'bbZll', 'ccZll',
    'Ztautau_powheg', 'Zmumu_powheg', 'Zee_powheg',
    'DYmumuM2to10_pythia', 'DYmumuM10to20_pythia', 'Zmumu_pythia',
    'Zee_pythia',
    #'InclusivePPmuX',
    'PPmuXptGt20Mu10', 'PPmuXptGt20Mu15',
    #'PPmuXptGt20Mu10Workaround',
    #'Wenu_pythia', 'Wmunu_pythia', 'Wtaunu_pythia',
    #'WePlus_powheg', 'WeMinus_powheg', 'WmuMinus_powheg', 'WmuPlus_powheg', 'WtauMinus_powheg', 'WtauPlus_powheg',
    'WplusJets_madgraph',
    'WW', 'WZ', 'ZZ',
    #'TTplusJets_pythia',
    'TTplusJets_madgraph'
]

# List of samples to include in the final level plots.  May include selections
# from the MERGE_SAMPLES defined at the bottom.
SAMPLES_TO_PLOT = [
    'data',
    'VVsum',
    'TTplusJets_madgraph',
    #'Zee_powheg',
    'Zmumu_powheg',
    #'WplusJetsSum_pythia',
    'WplusJets_madgraph',
    'qcdSum',
    'ZtautauSum'
]

SAMPLES_TO_PRINT = copy.copy(SAMPLES_TO_PLOT)
SAMPLES_TO_PRINT.append('smBgSum')
SAMPLES_TO_PRINT.append('smSum')
#SAMPLES_TO_PRINT.append('DYtautauM10to20PU156bx_pythiaZ2')
#SAMPLES_TO_PRINT.append('ZtautauPU156bx')
#SAMPLES_TO_PRINT.append('ZtautauPU156bx_pythiaZ2')
#SAMPLES_TO_PRINT.append('Ztautau_powheg')
SAMPLES_TO_PRINT.append('DYmumuM2to10_pythia')
SAMPLES_TO_PRINT.append('DYmumuM10to20_pythia')

SAMPLE_DEFAULTS = {
    'dbs_url' : "http://cmsdbsprod.cern.ch/cms_dbs_ph_analysis_02/servlet/DBSServlet",
    ##'conditions' : 'startup', # CV: temporarily disabled, because JEC uncertainties need 'START38_V14' tag
                                #     while default tag for CMSSW_3_8_7 is 'START38_V13'
    'conditions' : 'START38_V14::All',
    'genPhaseSpaceCut' : '',
    'factorize' : False,
    'applyZrecoilCorrection' : False,
    'applyElectronTriggerEfficiencyCorrection' : False, # not be used in mu + tau-jet channel
    'applyMuonTriggerEfficiencyCorrection' : False,   # to be used for MC only
    'applyMuonIsolationEfficiencyCorrection' : False, # to be used for MC only
    'applyVertexMultiplicityReweighting' : False,     # to be used for MC with pile-up only
    'enableSysUncertainties' : True,
    'lumi_mask' : '',
    'runselection' : '',
    'hlt_paths' : [ 'HLT_Mu9' ],
    'SE_white_list' : '',
    'SE_black_list' : '',
    'disableDuplicateCheck' : False,
    'enableFakeRates' : False,
    'disableDuplicateCheck' : False,
}

# Conversions to pico barns
_millibarns = 1.0e+9
_microbarns = 1.0e+6
_nanobarns  = 1.0e+3
_picobarns =  1.0
_femtobarns = 1.0e-3

# Integrated luminosity to normalize
##TARGET_LUMI = 200.0/_picobarns
TARGET_LUMI = 36.15/_picobarns # for runs 132440 - 149442

#--------------------------------------------------------------------------------
# NOTE: cross-sections for W and Z production are scaled to next-to-leading order values
#       documented on the wiki
#        http://alcaraz.web.cern.ch/alcaraz/CROSS_SECTIONS.txt
#--------------------------------------------------------------------------------

RECO_SAMPLES = {
    'data_Mu_Run2010A_Nov4ReReco' : {
        'datasetpath' : '/Mu/Run2010A-Nov4ReReco_v1/RECO',
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'lumi_mask' : "/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions10/7TeV/Reprocessing/Cert_136033-149442_7TeV_Nov4ReReco_Collisions10_JSON.txt",
        'runselection' : "136033-144114",
        'number_of_jobs' : 500,
        'conditions' : 'GR_R_38X_V15::All',
        'events_processed' : -1,
        'skim_eff' : 1.0,
        'type' : 'Data',
        'drawOption' : styles.drawOption_Data,
        'enableSysUncertainties' : False,
        'enableFakeRates' : True,
        'SE_black_list' : 'T2_IT_Legnaro'
    },
    'data_Mu_Run2010B_Nov4ReReco' : {
        'datasetpath' : '/Mu/Run2010B-Nov4ReReco_v1/RECO',
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'lumi_mask' : "/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions10/7TeV/Reprocessing/Cert_136033-149442_7TeV_Nov4ReReco_Collisions10_JSON.txt",
        'runselection' : '146428-149442',
        'number_of_jobs' : 1000,
        'conditions' : 'GR_R_38X_V15::All',
        'events_processed' : -1,
        'skim_eff' : 1.0,
        'type' : 'Data',
        'drawOption' : styles.drawOption_Data,
        'hlt_paths' : {
            'HLT_Mu9'               : '132440:MIN-147116:MAX',  # period A
            'HLT_IsoMu9'            : '147196:MIN-148058:MAX',  # period B
            'HLT_Mu11'              : '147196:MIN-148058:MAX',  # period B
            'HLT_Mu15_v1'           : '147196:MIN-149442:MAX',  # period B & C
            'HLT_IsoMu13_v3'        : '148822:MIN-149182:MAX',  # period C
            'HLT_IsoMu13_v4'        : '149291:MIN-149442:MAX',  # period C
            'HLT_IsoMu9_PFTau15_v1' : '148822:MIN-149182:MAX',  # period C
            'HLT_IsoMu9_PFTau15_v2' : '149291:MIN-149442:MAX',  # period C
            'HLT_Mu11_PFTau15_v1'   : '148822:MIN-149182:MAX',
            'HLT_Mu11_PFTau15_v2'   : '149291:MIN-149442:MAX'
        },
        'enableSysUncertainties' : False,
        'enableFakeRates' : True,
        'SE_black_list' : 'T2_RU_JINR,T2_US_Purdue,T2_US_Wisconsin,T2_IT_Bari,T3_US_UCLA'
    },
    'Ztautau_pythia' : {
        'datasetpath' : "/DYtoTauTau_M_20_TuneD6T_7TeV-pythia6-tauola/Fall10-START38_V12-v1/GEN-SIM-RECO",
        'events_processed' : 2677550,
        'skim_eff' : 1.0,
        'x_sec' : 1.282*1300*_picobarns, # Z + jets correction factor for NLO/LO cross-sections = 1.282
        'legendEntry' : plotter.process_Ztautau.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Ztautau.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Ztautau,
        'applyZrecoilCorrection' : True,
        'enableFakeRates' : True,
        'applyMuonTriggerEfficiencyCorrection' : True,
        'applyMuonIsolationEfficiencyCorrection' : True,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI38X")
    },
    'DYtautauM10to20PU156bx_pythiaZ2' : {
        'datasetpath' : "/DYToTauTau_M-10To20_TuneZ2_7TeV-pythia6-tauola/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO",
        'events_processed' : 2029050,
        'skim_eff' : 1.0,
        'x_sec' : 1666*_picobarns,
        'legendEntry' : plotter.process_Ztautau.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Ztautau.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Ztautau,
        'applyZrecoilCorrection' : True,
        'enableFakeRates' : True,
        'applyMuonTriggerEfficiencyCorrection' : True,
        'applyMuonIsolationEfficiencyCorrection' : True,
	'applyVertexMultiplicityReweighting' : True,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI38XPU")
    },
    'ZtautauPU156bx_pythiaZ2' : {
        'datasetpath' : "/DYToTauTau_M-20_TuneZ2_7TeV-pythia6-tauola/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v3/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 2098750,
        'skim_eff' : 1.0,
        'x_sec' : 1666*_picobarns,
        'legendEntry' : plotter.process_Ztautau.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Ztautau.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Ztautau,
        'applyZrecoilCorrection' : True,
        'enableFakeRates' : True,
        'applyMuonTriggerEfficiencyCorrection' : True,
        'applyMuonIsolationEfficiencyCorrection' : True,
	'applyVertexMultiplicityReweighting' : True,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI38XPU")
    },
    'Ztautau_powheg' : {
        'datasetpath' : "/DYToTauTau_M-20_CT10_TuneZ2_7TeV-powheg-pythia-tauola/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v3/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 1995369,
        'skim_eff' : 1.0,
        'x_sec' : 1666*_picobarns,
        'legendEntry' : plotter.process_Ztautau.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Ztautau.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Ztautau,
        'applyZrecoilCorrection' : True,
        'enableFakeRates' : True,
        'applyMuonTriggerEfficiencyCorrection' : True,
        'applyMuonIsolationEfficiencyCorrection' : True,
	'applyVertexMultiplicityReweighting' : True,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI38XPU")
    },
    'Ztautau_from_ZmumuEmbedding' : {
        'datasetpath' : "/Mu/fruboes-20101106_Zmumu_embed_fullRECO_runB_V2-64a622680cdd3df1201520286eed3637/USER",
        'dbs_url' : "http://cmsdbsprod.cern.ch/cms_dbs_ph_analysis_01/servlet/DBSServlet",
        'events_processed' : 132731,
        'skim_eff' : 1.0,
        'x_sec' : 1.282*1300*_picobarns, # Z + jets correction factor for NLO/LO cross-sections = 1.282
        'legendEntry' : plotter.process_Ztautau.config_dqmHistPlotter.legendEntry.value(),
        'type' : 'Data',
        'drawOption' : styles.drawOption_Ztautau,
        'disableDuplicateCheck' : True,
        'applyZrecoilCorrection' : False,
        'enableFakeRates' : True,
        'applyMuonTriggerEfficiencyCorrection' : True
    },
    'ZtautauPU156bx' : {
        'datasetpath' : "/DYtoTauTau_M_20_TuneD6T_7TeV-pythia6-tauola/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 2568490,
        'skim_eff' : 1.0,
        'x_sec' : 1.282*1300*_picobarns, # Z + jets correction factor for NLO/LO cross-sections = 1.282
        'legendEntry' : plotter.process_Ztautau.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Ztautau.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Ztautau,
        'applyZrecoilCorrection' : True,
        'enableFakeRates' : True,
        'applyMuonTriggerEfficiencyCorrection' : True,
        'applyMuonIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : True,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI38XPU")
    },
    'qqZll' : {
        'datasetpath' : "/VQQJetsToLL_TuneD6T_7TeV-madgraph-tauola/Fall10-START38_V12-v2/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 737157,
        'skim_eff' : 1.0,
        'x_sec' : 36.*_picobarns,
        'legendEntry' : plotter.process_Ztautau.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Ztautau.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Ztautau,
        'applyZrecoilCorrection' : True,
        'enableFakeRates' : True,
        'applyMuonTriggerEfficiencyCorrection' : True,
        'applyMuonIsolationEfficiencyCorrection' : True,
        'hlt' : cms.InputTag("TriggerResults", "", "HLT")
    },
    'qqZllPU156bx' : {
        'datasetpath' : "/VQQJetsToLL_TuneD6T_7TeV-madgraph-tauola/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 740488,
        'skim_eff' : 1.0,
        'x_sec' : 36.*_picobarns,
        'legendEntry' : plotter.process_Ztautau.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Ztautau.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Ztautau,
        'applyZrecoilCorrection' : True,
        'applyMuonTriggerEfficiencyCorrection' : True,
        'applyMuonIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : True,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI38XPU")
    },
    'bbZll' : {
        'datasetpath' : "/ZbbToLL_M-40_PtB1-15_TuneZ2_7TeV-madgraph-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 26837,
        'skim_eff' : 1.0,
        'x_sec' : 11.6*_picobarns,
        'legendEntry' : plotter.process_Ztautau.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Ztautau.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Ztautau,
        'applyZrecoilCorrection' : True,
        'applyMuonTriggerEfficiencyCorrection' : True,
        'applyMuonIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : False,
        'hlt' : cms.InputTag("TriggerResults", "", "HLT")
    },
    'ccZll' : {
        'datasetpath' : "/ZccToLL_M-40_PtC1-15_TuneD6T_7TeV-madgraph-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 40000,
        'skim_eff' : 1.0,
        'x_sec' : 18.74*_picobarns,
        'legendEntry' : plotter.process_Ztautau.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Ztautau.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Ztautau,
        'applyZrecoilCorrection' : True,
        'applyMuonTriggerEfficiencyCorrection' : True,
        'applyMuonIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : False,
        'hlt' : cms.InputTag("TriggerResults", "", "HLT")
    },
    'DYmumuM2to10_pythia' : {
        'datasetpath' : "/DYToMuMu_M-2To10_TuneZ2_7TeV-pythia6/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO",
        #'datasetpath' : "/DYToMuMu_M-2To10_TuneZ2_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO",  
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 1091900,
        #'events_processed' : 2178400,
        'skim_eff' : 1.0,
        'x_sec' : 1.282*72990*_picobarns, # Z + jets correction factor for NLO/LO cross-sections = 1.282 (k-factor for mMuMu > 20 GeV)
        'legendEntry' : plotter.process_Zmumu.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Zmumu.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Zmumu,
        'applyZrecoilCorrection' : True,
        'applyMuonTriggerEfficiencyCorrection' : True,
        'applyMuonIsolationEfficiencyCorrection' : True,
        'enableFakeRates' : True,
        'applyVertexMultiplicityReweighting' : True,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI38XPU")
    },   
    'DYmumuM10to20_pythia' : {
        'datasetpath' : "/DYToMuMu_M-10To20_TuneZ2_7TeV-pythia6/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 2184640,
        'skim_eff' : 1.0,
        'x_sec' : 1.282*2659*_picobarns, # Z + jets correction factor for NLO/LO cross-sections = 1.282 (k-factor for mMuMu > 20 GeV)
        'legendEntry' : plotter.process_Zmumu.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Zmumu.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Zmumu,
        'applyZrecoilCorrection' : True,
        'applyMuonTriggerEfficiencyCorrection' : True,
        'applyMuonIsolationEfficiencyCorrection' : True,
        'enableFakeRates' : True,
        'applyVertexMultiplicityReweighting' : True,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI38XPU")
    },
    'Zmumu_pythia' : {
        'datasetpath' : "/DYToMuMu_M-20_TuneZ2_7TeV-pythia6/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 2235697,
        'skim_eff' : 1.0,
        'x_sec' : 1.282*1300*_picobarns, # Z + jets correction factor for NLO/LO cross-sections = 1.282
        'legendEntry' : plotter.process_Zmumu.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Zmumu.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Zmumu,
        'applyZrecoilCorrection' : True,
        'applyMuonTriggerEfficiencyCorrection' : True,
        'applyMuonIsolationEfficiencyCorrection' : True,
        'enableFakeRates' : True,
        'applyVertexMultiplicityReweighting' : True,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI38XPU")
    },
    'Zmumu_powheg' : {
        'datasetpath' : "/DYToMuMu_M-20_CT10_TuneZ2_7TeV-powheg-pythia/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 1998931,
        'skim_eff' : 1.0,
        'x_sec' : 1666*_picobarns,
        'legendEntry' : plotter.process_Zmumu.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Zmumu.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Zmumu,
        'applyZrecoilCorrection' : True,
        'applyMuonTriggerEfficiencyCorrection' : True,
        'applyMuonIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : True,
        'enableFakeRates' : True,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI38XPU"),
        'SE_black_list' : 'T2_RU_JINR'
    },
    'Zee_pythia' : {
        'datasetpath' : "/DYToEE_M-20_TuneZ2_7TeV-pythia6/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO",
        'events_processed' : 2085586,
        'skim_eff' : 1.0,
        'x_sec' : 1.282*1300*_picobarns, # Z + jets correction factor for NLO/LO cross-sections = 1.282
        'legendEntry' : plotter.process_Zee.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Zee.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Zee,
        'applyZrecoilCorrection' : True,
        'applyMuonTriggerEfficiencyCorrection' : True,
        'applyMuonIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : True,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI38XPU")
    },
    'Zee_powheg' : {
        'datasetpath' : "/DYToEE_M-20_CT10_TuneZ2_7TeV-powheg-pythia/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO",
        'events_processed' : 1998990,
        'skim_eff' : 1.0,
        'x_sec' : 1666*_picobarns,
        'legendEntry' : plotter.process_Zee.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Zee.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Zee,
        'applyZrecoilCorrection' : True,
        'applyMuonTriggerEfficiencyCorrection' : True,
        'applyMuonIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : True,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI38XPU")
    },
    'InclusivePPmuX' : {
        'datasetpath' : "/ppMuX/akalinow-SkimTauTau_356_pass1-0a3d3891f015a95324f94837322fb8aa-muTauSkim/USER",
        'events_processed' : 9878911,
        'skim_eff' : 0.017,
        'x_sec' : 48.44*_millibarns*0.00176, # 48.44 mb * gen filter efficiency
        'genPhaseSpaceCut' : 'ptHat < 20. | leadingGenMuon.pt < 10.',
        'legendEntry' : plotter.process_InclusivePPmuX.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_InclusivePPmuX.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_QCD,
        'enableFakeRates' : True,
        'applyMuonTriggerEfficiencyCorrection' : True,
        'applyMuonIsolationEfficiencyCorrection' : True,
        'factorize' : True
    },
    'PPmuXptGt20Mu10Workaround' : {
        'datasetpath' : "/QCD_Pt-20_MuEnrichedPt-10_TuneZ2_7TeV-pythia6/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 8063288,
        'skim_eff' : 1.0,
        'x_sec' : 0.2966*_millibarns*1.18e-3, # x-sec * gen filter efficiency
        'genPhaseSpaceCut' : 'leadingGenMuon.pt < 15.',
        'legendEntry' : plotter.process_PPmuXptGt20.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_PPmuXptGt20.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_QCD,
        'enableFakeRates' : True,
        'applyMuonTriggerEfficiencyCorrection' : True,
        'applyMuonIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : True,
        'factorize' : True,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI38XPU")
    },
    'PPmuXptGt20Mu10' : {
        'datasetpath' : "/QCD_Pt-20_MuEnrichedPt-10_TuneZ2_7TeV-pythia6/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 8063288,
        'skim_eff' : 1.0,
        'x_sec' : 0.2966*_millibarns*1.18e-3, # x-sec * gen filter efficiency
        'genPhaseSpaceCut' : 'leadingGenMuon.pt < 15.',
        'legendEntry' : plotter.process_PPmuXptGt20.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_PPmuXptGt20.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_QCD,
        'enableFakeRates' : True,
        'applyMuonTriggerEfficiencyCorrection' : True,
        'applyMuonIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : True,
        'factorize' : True,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI38XPU")
    },
    'PPmuXptGt20Mu15' : {
        'datasetpath' : "/QCD_Pt-20_MuEnrichedPt-15_TuneZ2_7TeV-pythia6/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 29504866,
        'number_of_jobs' : 1000,
        'skim_eff' : 1.0,
        'x_sec' : 0.2966*_millibarns*2.855e-4, # x-sec * gen filter efficiency
        'legendEntry' : plotter.process_PPmuXptGt20.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_PPmuXptGt20.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_QCD,
        'applyMuonTriggerEfficiencyCorrection' : True,
        'applyMuonIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : True,
        'enableFakeRates' : True,
        'factorize' : True,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI38XPU")
    },
    'Wenu_pythia' : {
        'datasetpath' : "/WtoENu_TuneD6T_7TeV-pythia6/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 5495740,
        'skim_eff' : 1.0,
        'x_sec' : 1.321*7899*_picobarns, # W --> e nucorrection factor for NLO/LO cross-sections = 1.321
        'legendEntry' : plotter.process_WplusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_WplusJets.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_WplusJets,
        'applyZrecoilCorrection' : False,
        'applyMuonTriggerEfficiencyCorrection' : True,
        'applyMuonIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : True,
        #'enableFakeRates' : True,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI38XPU"),
        'SE_black_list' : 'T2_TW_Taiwan'
    },
    'WePlus_powheg' : {
        'datasetpath' : "/WPlusToENu_CT10_TuneZ2_7TeV-powheg-pythia/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 1997953,
        'skim_eff' : 1.0,
        'x_sec' : 6152*_picobarns,
        'legendEntry' : plotter.process_WplusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_WplusJets.config_dqmHistPlotter.type.value(),
        #'enableFakeRates' : True,
        'drawOption' : styles.drawOption_WplusJets,
        'applyZrecoilCorrection' : False,
        'applyMuonTriggerEfficiencyCorrection' : True,
        'applyMuonIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : True,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI38XPU"),
        'SE_black_list' : 'T2_TW_Taiwan'
    },
    'WeMinus_powheg' : {
        'datasetpath' : "/WMinusToENu_CT10_TuneZ2_7TeV-powheg-pythia/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 1996734,
        'skim_eff' : 1.0,
        'x_sec' : 4286*_picobarns,
        'legendEntry' : plotter.process_WplusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_WplusJets.config_dqmHistPlotter.type.value(),
        #'enableFakeRates' : True,
        'drawOption' : styles.drawOption_WplusJets,
        'applyZrecoilCorrection' : False,
        'applyMuonTriggerEfficiencyCorrection' : True,
        'applyMuonIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : True,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI38XPU"),
        'SE_black_list' : 'T2_TW_Taiwan'
    },
    'Wmunu_pythia' : {
        'datasetpath' : "/WtoMuNu_TuneD6T_7TeV-pythia6/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 5485510,
        'skim_eff' : 1.0,
        'x_sec' : 1.321*7899*_picobarns, # W --> mu nu correction factor for NLO/LO cross-sections = 1.321
        'legendEntry' : plotter.process_WplusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_WplusJets.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_WplusJets,
        'applyZrecoilCorrection' : False,
        'applyMuonTriggerEfficiencyCorrection' : True,
        'applyMuonIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : True,
        #'enableFakeRates' : True,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI38XPU"),
        'SE_black_list' : 'T2_TW_Taiwan'
    },
    'WmuMinus_powheg' : {
        'datasetpath' : "/WMinusToMuNu_CT10_TuneZ2_7TeV-powheg-pythia/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 1996548,
        'skim_eff' : 1.0,
        'x_sec' : 4286*_picobarns,
        'legendEntry' : plotter.process_WplusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_WplusJets.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_WplusJets,
        'applyZrecoilCorrection' : False,
        'applyMuonTriggerEfficiencyCorrection' : True,
        'applyMuonIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : True,
        #'enableFakeRates' : True,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI38XPU")
    },
    'WmuPlus_powheg' : {
        'datasetpath' : "/WPlusToMuNu_CT10_TuneZ2_7TeV-powheg-pythia/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 1997318,
        'skim_eff' : 1.0,
        'x_sec' : 6152*_picobarns,
        'legendEntry' : plotter.process_WplusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_WplusJets.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_WplusJets,
        'applyZrecoilCorrection' : False,
        'applyMuonTriggerEfficiencyCorrection' : True,
        'applyMuonIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : True,
        #'enableFakeRates' : True,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI38XPU")
    },
    'Wtaunu_pythia' : {
        'datasetpath' : "/WtoTauNu_TuneD6T_7TeV-pythia6-tauola/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 5476550,
        'skim_eff' : 1.0,
        'x_sec' : 1.321*7899*_picobarns, # W --> tau nu correction factor for NLO/LO cross-sections = 1.321
        'legendEntry' : plotter.process_WplusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_WplusJets.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_WplusJets,
        'applyZrecoilCorrection' : False,
        'applyMuonTriggerEfficiencyCorrection' : True,
        'applyMuonIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : True,
        #'enableFakeRates' : True,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI38XPU"),
        'SE_black_list' : 'T2_TW_Taiwan'
    },
    'WtauMinus_powheg' : {
        'datasetpath' : "/WMinusToTauNu_CT10_TuneZ2_7TeV-powheg-pythia-tauola/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 1994870,
        'skim_eff' : 1.0,
        'x_sec' : 4286*_picobarns,
        'legendEntry' : plotter.process_WplusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_WplusJets.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_WplusJets,
        'applyZrecoilCorrection' : False,
        'applyMuonTriggerEfficiencyCorrection' : True,
        'applyMuonIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : True,
        #'enableFakeRates' : True,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI38XPU")
    },
    'WtauPlus_powheg' : {
        'datasetpath' : "/WPlusToTauNu_CT10_TuneZ2_7TeV-powheg-pythia-tauola/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 1995871,
        'skim_eff' : 1.0,
        'x_sec' : 6152*_picobarns,
        'legendEntry' : plotter.process_WplusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_WplusJets.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_WplusJets,
        'applyZrecoilCorrection' : False,
        'applyMuonTriggerEfficiencyCorrection' : True,
        'applyMuonIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : True,
        #'enableFakeRates' : True,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI38XPU")
    },
    'WplusJets_madgraph' : {
        'datasetpath' : "/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 15168266,
        'skim_eff' : 1.0,
        'x_sec' : 31314*_picobarns,
        'legendEntry' : plotter.process_WplusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_WplusJets.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_WplusJets,
        'applyZrecoilCorrection' : False,
        'applyMuonTriggerEfficiencyCorrection' : True,
        'applyMuonIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : True,
        'enableFakeRates' : True,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI38XPU")
    },
    'WW' : {
        'datasetpath' : "/WWtoAnything_TuneZ2_7TeV-pythia6-tauola/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 2061760,
        'skim_eff' : 1.0,
        'x_sec' : 43.0*_picobarns, # NLO cross-section from https://twiki.cern.ch/twiki/bin/viewauth/CMS/StandardModelCrossSections
        'legendEntry' : plotter.process_VV.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_VV.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_VV,
        'applyZrecoilCorrection' : False,
        'applyMuonTriggerEfficiencyCorrection' : True,
        'applyMuonIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : True,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI38XPU")
    },
    'WZ' : {
        'datasetpath' : "/WZtoAnything_TuneZ2_7TeV-pythia6-tauola/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 2194752,
        'skim_eff' : 1.0,
        'x_sec' : 18.2*_picobarns, # NLO cross-section from https://twiki.cern.ch/twiki/bin/viewauth/CMS/StandardModelCrossSections
        'legendEntry' : plotter.process_VV.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_VV.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_VV,
        'applyZrecoilCorrection' : False,
        'applyMuonTriggerEfficiencyCorrection' : True,
        'applyMuonIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : True,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI38XPU")
    },
    'ZZ' : {
        'datasetpath' : "/ZZtoAnything_TuneZ2_7TeV-pythia6-tauola/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 2113368,
        'skim_eff' : 1.0,
        'x_sec' : 5.9*_picobarns, # NLO cross-section from https://twiki.cern.ch/twiki/bin/viewauth/CMS/StandardModelCrossSections
        'legendEntry' : plotter.process_VV.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_VV.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_VV,
        'applyZrecoilCorrection' : False,
        'applyMuonTriggerEfficiencyCorrection' : True,
        'applyMuonIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : True,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI38XPU")
    },
    'TTplusJets_pythia' : {
        'datasetpath' : "/TT_TuneZ2_7TeV-pythia6-tauola/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 1095950,
        'skim_eff' : 1.0,
        'x_sec' : 157.5*_picobarns, # NLO cross-section from https://twiki.cern.ch/twiki/bin/viewauth/CMS/StandardModelCrossSections
        'legendEntry' : plotter.process_TTplusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_TTplusJets.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_TTplusJets,
        'applyMuonTriggerEfficiencyCorrection' : True,
        'applyMuonIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : True,
        'enableFakeRates' : True,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI38XPU")
    },
    'TTplusJets_madgraph' : {
        'datasetpath' : "/TTJets_TuneZ2_7TeV-madgraph-tauola/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 1164640,
        'skim_eff' : 1.0,
        'x_sec' : 157.5*_picobarns, # NLO cross-section from https://twiki.cern.ch/twiki/bin/viewauth/CMS/StandardModelCrossSections
        'legendEntry' : plotter.process_TTplusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_TTplusJets.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_TTplusJets,
        'applyMuonTriggerEfficiencyCorrection' : True,
        'applyMuonIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : True,
        'enableFakeRates' : True,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI38XPU")
    }
}

# Define samples that get merged together
MERGE_SAMPLES = {
    'data' : {
        'samples' : [
            'data_Mu_Run2010A_Nov4ReReco',
            'data_Mu_Run2010B_Nov4ReReco'
        ],
        'legendEntry' : 'DATA',
        'type' : 'Data',
        'drawOption' : styles.drawOption_Data
    },
    'ZtautauSum' : {
        'samples' : [
            ##'Ztautau',
            ##'qqZll',
            ##'bbZll',
            ##'ccZll',
            'ZtautauPU156bx'
            ##'qqZllPU156bx',
            ##'Ztautau_powheg'
        ],
        'legendEntry' : plotter.process_Ztautau.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Ztautau.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Ztautau
    },
    'ZmumuSum' : {
        'samples' : [            
            ##'Zmumu_powheg',            
            'Zmumu_pythia',
            'DYmumuM2to10_pythia',
            'DYmumuM10to20_pythia'
        ],
        'legendEntry' : plotter.process_Zmumu.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Zmumu.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Zmumu
    },
    'qcdSum' : {
        'samples' : [
            #'InclusivePPmuX',
            #'PPmuXptGt20Mu10Workaround',
            'PPmuXptGt20Mu10',
            'PPmuXptGt20Mu15'
        ],
        #'legendEntry' : plotter.process_PPmuXptGt20.config_dqmHistPlotter.legendEntry.value(),
        'legendEntry' : 'QCD',
        'type' : plotter.process_PPmuXptGt20.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_QCD
    },
    #'WplusJetsSum_pythia' : {
       #'samples' : [
           #'Wenu_pythia',
           #'Wmunu_pythia',
           #'Wtaunu_pythia'
       #],
       #'legendEntry' : plotter.process_WplusJets.config_dqmHistPlotter.legendEntry.value(),
       #'type' : plotter.process_WplusJets.config_dqmHistPlotter.type.value(),
       #'drawOption' : styles.drawOption_WplusJets
    #},
    ##'WplusJetsSum_powheg' : {
    ##	'samples' : [
    ##        'WePlus_powheg' ,
    ##        'WeMinus_powheg',
    ##        'WmuMinus_powheg',
    ##        'WmuPlus_powheg',
    ##        'WtauMinus_powheg',
    ##        'WtauPlus_powheg'
    ##	],
    ## 	'legendEntry' : plotter.process_WplusJets.config_dqmHistPlotter.legendEntry.value(),
    ##    'type' : plotter.process_WplusJets.config_dqmHistPlotter.type.value(),
    ##    'drawOption' : styles.drawOption_WplusJets
    ##},
    'VVsum' : {
        'samples' : [
            'WW',
            'WZ',
            'ZZ'
        ],
        'legendEntry' : 'WW/WZ/ZZ',
        'type' : 'smMC',
        'drawOption' : styles.drawOption_VV
    },
    'smBgSum' : {
        'samples' : [
            'Zmumu_powheg',
            'qcdSum',
            'WplusJets_madgraph',
            'VVsum',
            'TTplusJets_madgraph'
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
    }
}

# List of all subsamples used in any plot job.
# i.e. if qcdSum is included in samples to plot
#      it will be expanded to the inclusive/exclusive ppMux samples
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

recoSampleDefinitionsZtoMuTau_7TeV = {
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

