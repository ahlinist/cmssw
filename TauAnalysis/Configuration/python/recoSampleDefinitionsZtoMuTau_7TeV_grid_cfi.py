import FWCore.ParameterSet.Config as cms
import copy

import TauAnalysis.Configuration.plotterProcessDefinitions_cfi as plotter
import TauAnalysis.DQMTools.plotterStyleDefinitions_cfi as styles

# List of samples to run in the analysis
SAMPLES_TO_ANALYZE = [
    'data_Mu_Run2010A_Nov4ReReco',
    'data_Mu_Run2010B_Nov4ReReco',
    ##'Ztautau', 'qqZtautau',
    'ZtautauPU156bx',
    'qqZtautau',
    ##'qqZtautauPU156bx', # EK - not at any site as of Dec 6
    'Zmumu',
    'Zee_PU156BX',
    #'InclusivePPmuX',
    'PPmuXptGt20Mu10', 'PPmuXptGt20Mu15',
    'Wenu', 'Wmunu', 'Wtaunu',
    'TTplusJets'
    'TTplusJetsNoPU'
]

# List of samples to include in the final level plots.  May include selections
# from the MERGE_SAMPLES defined at the bottom.
SAMPLES_TO_PLOT = [
    'data',
    'qcdSum',
    'WplusJets',
    'TTplusJets',
    'Zmumu',
    'ZtautauSum'
]

SAMPLES_TO_PRINT = copy.copy(SAMPLES_TO_PLOT)
SAMPLES_TO_PRINT.append('smBgSum')
SAMPLES_TO_PRINT.append('smSum')
#SAMPLES_TO_PRINT.append('data_Mu_Run2010A_Sep17ReReco')
#SAMPLES_TO_PRINT.append('data_Mu_Run2010B_Prompt')

SAMPLE_DEFAULTS = {
    'dbs_url' : "http://cmsdbsprod.cern.ch/cms_dbs_ph_analysis_02/servlet/DBSServlet",
    'conditions' : 'startup',
    'genPhaseSpaceCut' : '',
    'factorize' : False,
    'applyZrecoilCorrection' : False,
    'applyMuonTriggerEfficiencyCorrection' : False, # to be used for MC only
    'applyVertexMultiplicityReweighting' : False,   # to be used for MC with pile-up only
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
TARGET_LUMI = 35.39/_picobarns # for runs 132440 - 149442

#--------------------------------------------------------------------------------
# NOTE: cross-sections for W and Z production are scaled to next-to-leading order values
#       documented on the wiki
#        http://alcaraz.web.cern.ch/alcaraz/CROSS_SECTIONS.txt
#--------------------------------------------------------------------------------

RECO_SAMPLES = {
    'data_Mu_Run2010A_Sep17ReReco' : {
        'datasetpath' : '/Mu/Run2010A-Sep17ReReco_v2/RECO',
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'lumi_mask' : "/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions10/7TeV/Reprocessing/Cert_132440-144114_7TeV_Sep17ReReco_Collisions10_JSON_v2.txt",
        'runselection' : "132440-136032",
        'conditions' : 'GR_R_38X_V14::All',
        'events_processed' : -1,
        'skim_eff' : 1.0,
        'type' : 'Data',
        'drawOption' : styles.drawOption_Data,
        'enableSysUncertainties' : False,
        'enableFakeRates' : True
    },
    'data_Mu_Run2010A_Nov4ReReco' : {
        'datasetpath' : '/Mu/Run2010A-Nov4ReReco_v1/RECO',
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'lumi_mask' : "/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions10/7TeV/Reprocessing/Cert_136033-149442_7TeV_Nov4ReReco_Collisions10_JSON.txt",
        'runselection' : "136033-144114",
        'number_of_jobs' : 500,
        'conditions' : 'GR_R_38X_V14::All',
        'events_processed' : -1,
        'skim_eff' : 1.0,
        'type' : 'Data',
        'drawOption' : styles.drawOption_Data,
        'enableSysUncertainties' : False,
        'enableFakeRates' : True
    },
    'data_Mu_Run2010B_Nov4ReReco' : {
        'datasetpath' : '/Mu/Run2010B-Nov4ReReco_v1/RECO',
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'lumi_mask' : "/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions10/7TeV/Reprocessing/Cert_136033-149442_7TeV_Nov4ReReco_Collisions10_JSON.txt",
        'runselection' : '146428-149442',
        'number_of_jobs' : 1000,
        'conditions' : 'GR_R_38X_V14::All',
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
        'enableFakeRates' : True
    },
    # Keep this here temporarily as the Nov4th is swamped
    'data_Mu_Run2010B_Prompt' : {
        'datasetpath' : '/Mu/Run2010B-PromptReco-v2/RECO',
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'lumi_mask' : "/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions10/7TeV/StreamExpress/Cert_132440-149442_7TeV_StreamExpress_Collisions10_JSON_v2.txt",
        'runselection' : '145762 - 149442',
        'number_of_jobs' : 1000,
        'conditions' : 'GR_R_38X_V13::All',
        'events_processed' : -1,
        'skim_eff' : 1.0,
        'type' : 'Data',
        'drawOption' : styles.drawOption_Data,
        'hlt_paths' : {
            'HLT_Mu9'               : '132440:MIN-147116:MAX',  # period A
            'HLT_IsoMu9'            : '147196:MIN-148058:MAX',  # period B
            'HLT_Mu11'              : '147196:MIN-148058:MAX',  # period B
            'HLT_IsoMu13_v3'        : '148822:MIN-149182:MAX',  # period C
            'HLT_IsoMu13_v4'        : '147196:MIN-149442:MAX',  # period B
            'HLT_Mu15_v1'           : '147196:MIN-149442:MAX',  # period B
            'HLT_IsoMu9_PFTau15_v1' : '148822:MIN-149182:MAX',  # period C
            'HLT_IsoMu9_PFTau15_v2' : '149291:MIN-149442:MAX',  # period C
            'HLT_Mu11_PFTau15_v1'   : '148822:MIN-149182:MAX',
            'HLT_Mu11_PFTau15_v2'   : '149291:MIN-149442:MAX'
        },
        'enableSysUncertainties' : False,
        'enableFakeRates' : True
    },
    'Ztautau' : {
        'datasetpath' : "/DYtoTauTau_M_20_TuneD6T_7TeV-pythia6-tauola/Fall10-START38_V12-v1/GEN-SIM-RECO",
        'events_processed' : 2677550,
        'skim_eff' : 1.0,
        'x_sec' : 1.28*1300*_picobarns, # Z + jets correction factor for NLO/LO cross-sections = 1.28
        'legendEntry' : plotter.process_Ztautau.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Ztautau.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Ztautau,
        'applyZrecoilCorrection' : True,
        'enableFakeRates' : True,
        'applyMuonTriggerEfficiencyCorrection' : True,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI38X")
    },
    'ZtautauEmbedded' : {
        'datasetpath' : "/Mu/fruboes-20101106_Zmumu_embed_fullRECO_runB_V2-64a622680cdd3df1201520286eed3637/USER",
        'dbs_url' : "http://cmsdbsprod.cern.ch/cms_dbs_ph_analysis_01/servlet/DBSServlet",
        'events_processed' : 132731,
        'skim_eff' : 1.0,
        'x_sec' : 1.28*1300*_picobarns, # Z + jets correction factor for NLO/LO cross-sections = 1.28
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
        'x_sec' : 1.28*1300*_picobarns, # Z + jets correction factor for NLO/LO cross-sections = 1.28
        'legendEntry' : plotter.process_Ztautau.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Ztautau.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Ztautau,
        'applyZrecoilCorrection' : True,
        'enableFakeRates' : True,
        'applyMuonTriggerEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : True,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI38XPU")
    },
    'qqZtautau' : {
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
        'hlt' : cms.InputTag("TriggerResults", "", "HLT")
    },
    'qqZtautauPU156bx' : {
        'datasetpath' : "/VQQJetsToLL_TuneD6T_7TeV-madgraph-tauola/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 737157,
        'skim_eff' : 1.0,
        'x_sec' : 36.*_picobarns,
        'legendEntry' : plotter.process_Ztautau.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Ztautau.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Ztautau,
        'applyZrecoilCorrection' : True,
        'applyMuonTriggerEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : True,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI38XPU")
    },
    'Zmumu' : {
        'datasetpath' : "/DYtoMuMu_M_20_TuneD6T_7TeV-pythia6/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 2558038,
        'skim_eff' : 1.0,
        'x_sec' : 1.28*1300*_picobarns, # Z + jets correction factor for NLO/LO cross-sections = 1.28
        'legendEntry' : plotter.process_Zmumu.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Zmumu.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Zmumu,
        'applyZrecoilCorrection' : True,
        'applyMuonTriggerEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : True,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI38XPU")
    },
    'Zee_PU156BX' : {
        'datasetpath' : "/DYToEE_M-20_TuneZ2_7TeV-pythia6/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO",
        'events_processed' : 2085586,
        'skim_eff' : 1.0,
        'x_sec' : 1.28*1300*_picobarns, # Z + jets correction factor for NLO/LO cross-sections = 1.28
        'legendEntry' : plotter.process_Zee.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Zee.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Zee,
        'applyZrecoilCorrection' : False,
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
        'factorize' : True
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
        'applyVertexMultiplicityReweighting' : True,
        'factorize' : True,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI38XPU")
    },
    'PPmuXptGt20Mu15' : {
        'datasetpath' : "/QCD_Pt-20_MuEnrichedPt-15_TuneZ2_7TeV-pythia6/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 29504866,
        'number_of_jobs' : 600,
        'skim_eff' : 1.0,
        'x_sec' : 0.2966*_millibarns*2.855e-4, # x-sec * gen filter efficiency
        'legendEntry' : plotter.process_PPmuXptGt20.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_PPmuXptGt20.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_QCD,
        'applyMuonTriggerEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : True,
        'factorize' : True,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI38XPU"),
        'enableFakeRates' : True,
    },
    'Wenu' : {
        'datasetpath' : "/WtoENu_TuneD6T_7TeV-pythia6/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 5495740,
        'skim_eff' : 1.0,
        'x_sec' : 1.32*7899*_picobarns, # W --> e nucorrection factor for NLO/LO cross-sections = 1.32
        'legendEntry' : plotter.process_WplusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_WplusJets.config_dqmHistPlotter.type.value(),
        'enableFakeRates' : True,
        'drawOption' : styles.drawOption_WplusJets,
        'applyZrecoilCorrection' : True,
        'applyMuonTriggerEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : True,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI38XPU"),
        'SE_black_list' : 'T2_TW_Taiwan'
    },
    'Wmunu' : {
        'datasetpath' : "/WtoMuNu_TuneD6T_7TeV-pythia6/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 5485510,
        'skim_eff' : 1.0,
        'x_sec' : 1.32*7899*_picobarns, # W --> mu nu correction factor for NLO/LO cross-sections = 1.32
        'legendEntry' : plotter.process_WplusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_WplusJets.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_WplusJets,
        'applyZrecoilCorrection' : True,
        'applyMuonTriggerEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : True,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI38XPU"),
        'SE_black_list' : 'T2_TW_Taiwan'
    },
    'Wtaunu' : {
        'datasetpath' : "/WtoTauNu_TuneD6T_7TeV-pythia6-tauola/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 5476550,
        'skim_eff' : 1.0,
        'x_sec' : 1.32*7899*_picobarns, # W --> tau nu correction factor for NLO/LO cross-sections = 1.32
        'legendEntry' : plotter.process_WplusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_WplusJets.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_WplusJets,
        'applyZrecoilCorrection' : True,
        'applyMuonTriggerEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : True,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI38XPU"),
        'SE_black_list' : 'T2_TW_Taiwan',
    },
    'TTplusJets' : {
        'datasetpath' : "/TT_TuneZ2_7TeV-pythia6-tauola/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO",
        ##'datasetpath' : "/TT_TuneZ2_7TeV-pythia6-tauola/Fall10-START38_V12-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        ##'events_processed' : 1099550,
        'events_processed' : 1095950,
        'skim_eff' : 1.0,
        'x_sec' : 165*_picobarns, # NNLO cross-section from https://twiki.cern.ch/twiki/bin/viewauth/CMS/StandardModelCrossSections
        'legendEntry' : plotter.process_TTplusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_TTplusJets.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_TTplusJets,
        'applyMuonTriggerEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : True,
        ##'hlt' : cms.InputTag("TriggerResults", "", "HLT")
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI38XPU")
    },
    'TTplusJetsNoPU' : {
        'datasetpath' : "/TT_TuneZ2_7TeV-pythia6-tauola/Fall10-START38_V12-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 1099550,
        'skim_eff' : 1.0,
        'x_sec' : 165*_picobarns, # NNLO cross-section from https://twiki.cern.ch/twiki/bin/viewauth/CMS/StandardModelCrossSections
        'legendEntry' : plotter.process_TTplusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_TTplusJets.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_TTplusJets,
        'applyMuonTriggerEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : True,
        'hlt' : cms.InputTag("TriggerResults", "", "HLT")
    },
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
    'data_preNov4' : {
        'samples' : [
            'data_Mu_Run2010A_Sep17ReReco',
            'data_Mu_Run2010B_Prompt'
        ],
        'legendEntry' : 'DATA',
        'type' : 'Data',
        'drawOption' : styles.drawOption_Data
    },
    'data_nov25workaround' : {
        'samples' : [
            'data_Mu_Run2010A_Nov4ReReco',
            'data_Mu_Run2010B_Prompt'
        ],
        'legendEntry' : 'DATA',
        'type' : 'Data',
        'drawOption' : styles.drawOption_Data
    },
    'ZtautauSum' : {
        'samples' : [
            ##'Ztautau',
            ##'qqZtautau',
            'ZtautauPU156bx',
            'qqZtautauPU156bx',
        ],
        'legendEntry' : plotter.process_Ztautau.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Ztautau.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Ztautau
    },
    'qcdSum' : {
        'samples' : [
            #'InclusivePPmuX',
            'PPmuXptGt20Mu10',
            'PPmuXptGt20Mu15'
        ],
        #'legendEntry' : plotter.process_PPmuXptGt20.config_dqmHistPlotter.legendEntry.value(),
        'legendEntry' : 'QCD',
        'type' : plotter.process_PPmuXptGt20.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_QCD
    },
    'WplusJetsSum' : {
        'samples' : [
            'Wenu',
            'Wmunu',
            'Wtaunu'
        ],
        'legendEntry' : plotter.process_WplusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_WplusJets.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_WplusJets,
    },
    'smBgSum' : {
        'samples' : [
            'Zmumu',
            'qcdSum',
            'WplusJetsSum',
            'TTplusJets'
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

