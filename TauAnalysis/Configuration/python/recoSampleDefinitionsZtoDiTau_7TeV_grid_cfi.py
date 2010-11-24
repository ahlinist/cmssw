import FWCore.ParameterSet.Config as cms
import copy

import TauAnalysis.Configuration.plotterProcessDefinitions_cfi as plotter
import TauAnalysis.DQMTools.plotterStyleDefinitions_cfi as styles

# List of samples to run in the analysis
SAMPLES_TO_ANALYZE = [
    'data_BTau_Run2010B_Prompt',
    'Ztautau', 'ZtautauPU156bx',
    'Zee',
    'Zmumu',
    'qcdDiJet', 
    'WplusJets',
    'TTplusJets'
] 

# List of samples to include in the final level plots.  May include selections
# from the MERGE_SAMPLES defined at the bottom.
SAMPLES_TO_PLOT = [
    'data', 	
    ##'qcdSum',
    'WplusJets',
    'TTplusJets',
    'Zee',
    'Zmumu',
    'Ztautau',
    'qcdSum'
]

SAMPLES_TO_PRINT = copy.copy(SAMPLES_TO_PLOT)
#SAMPLES_TO_PRINT.append('smBgSum')
#SAMPLES_TO_PRINT.append('smSum')
#SAMPLES_TO_PRINT.append('ZtautauPU156bx')

SAMPLE_DEFAULTS = {
    'dbs_url' : "http://cmsdbsprod.cern.ch/cms_dbs_ph_analysis_02/servlet/DBSServlet",
    'conditions' : 'startup',
    'genPhaseSpaceCut' : '',
    'factorize' : False,
    'applyZrecoilCorrection' : False,
    'applyMuonTriggerEfficiencyCorrection' : False, # to be used for MC only
    'applyVertexMultiplicityReweighting' : False,   # to be used for MC with pile-up only
    'enableSysUncertainties' : False,
    'lumi_mask' : '',
    'runselection' : '',
    'SE_white_list' : '',
    'SE_black_list' : '',
    'disableDuplicateCheck' : False
}

# Conversions to pico barns
_millibarns = 1.0e+9
_microbarns = 1.0e+6
_nanobarns  = 1.0e+3
_picobarns =  1.0
_femtobarns = 1.0e-3

# Integrated luminosity to normalize 
##TARGET_LUMI = (200.0)/_picobarns
TARGET_LUMI = (32.32)/_picobarns # for runs 146428 - 149442

RECO_SAMPLES = {
    'data_BTau_Run2010B_Prompt' : {
        'datasetpath' : "/BTau/Run2010B-PromptReco-v2/RECO", # run-range in DBS: 145762-149442
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'lumi_mask' : "/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions10/7TeV/StreamExpress/Cert_132440-149442_7TeV_StreamExpress_Collisions10_JSON_v2.txt",
        'runselection' : '146428 - 149442',
        'number_of_jobs' : 1000,
        'conditions' : 'GR_R_38X_V13::All',
        'events_processed' : -1,
        'skim_eff' : 1.0,
        'type' : 'Data',
        'drawOption' : styles.drawOption_Data,
        'hlt_paths' : {
            'HLT_DoubleIsoTau15_OneLeg_Trk5' : '146428:MIN-147116:MAX',
            'HLT_DoubleIsoTau15_Trk5'        : '147196:MIN-148058:MAX',
            'HLT_DoubleIsoTau15_Trk5_v3'     : '148822:MIN-149182:MAX',
            'HLT_DoubleIsoTau15_Trk5_v4'     : '149291:MIN-149442:MAX'
        }
    },
    'Ztautau' : {
        'datasetpath' : "/DYToTauTau_M-20_TuneZ2_7TeV-pythia6-tauola/Fall10-START38_V12-v1/GEN-SIM-RECO",
        'events_processed' : 2057446,
        'skim_eff' : 1.0,
        'x_sec' : 1.28*1300*_picobarns, # Z + jets correction factor for NLO/LO cross-sections = 1.28
        'legendEntry' : plotter.process_Ztautau.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Ztautau.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Ztautau,
        'applyZrecoilCorrection' : True,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI38X"),
        'hlt_paths' : [ 'HLT_DoubleIsoTau15_Trk5' ]
    },
    'ZtautauPU156bx' : {
        'datasetpath' : "/DYToTauTau_M-20_TuneZ2_7TeV-pythia6-tauola/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",       
        'events_processed' : 2011186,
        'skim_eff' : 1.0,
        'x_sec' : 1.28*1300*_picobarns, # Z + jets correction factor for NLO/LO cross-sections = 1.28
        'legendEntry' : plotter.process_Ztautau.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Ztautau.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Ztautau,
        'applyZrecoilCorrection' : True,
        'applyVertexMultiplicityReweighting' : True,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI38XPU"),
        'hlt_paths' : [ 'HLT_DoubleIsoTau15_Trk5' ]
    },
    'Zee' : {
        'datasetpath' : "/DYToEE_M-20_TuneZ2_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO",
        'events_processed' : 2127607,
        'skim_eff' : 0.600,
        'x_sec' : 1.28*1300*_picobarns, # Z + jets correction factor for NLO/LO cross-sections = 1.28
        'legendEntry' : plotter.process_Zee.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Zee.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Zee,
        'applyZrecoilCorrection' : True,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI38X"),
        'hlt_paths' : [ 'HLT_DoubleIsoTau15_Trk5' ]
    },
    'Zmumu' : {
        'datasetpath' : "/DYToMuMu_M-20_TuneZ2_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO",
        'events_processed' : 2289913,
        'skim_eff' : 1.0,
        'x_sec' : 1.28*1300*_picobarns, # Z + jets correction factor for NLO/LO cross-sections = 1.28
        'legendEntry' : plotter.process_Zmumu.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Zmumu.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Zmumu,
        'applyZrecoilCorrection' : True,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI38X"),
        'hlt_paths' : [ 'HLT_DoubleIsoTau15_Trk5' ]
    },
    'qcdDiJet' : {
        'datasetpath' : "/QCD_Pt15_BiasedTau/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO",
        'events_processed' : 6020758,
        'skim_eff' : 1.000, # unskimmed sample
        'x_sec' : 0.874*_millibarns*0.0338, # 0.874 mb * gen filter efficiency
        'legendEntry' : plotter.process_qcd_W.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_qcd_W.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_QCD,
        'factorize' : True,
        'hlt' : cms.InputTag("TriggerResults", "", "HLT"),
        'hlt_paths' : [ 'HLT_DoubleLooseIsoTau15_Trk5' ],
        'SE_black_list' : 'T2_FR_CCIN2P3'
    },
    'WplusJets' : {
        'datasetpath' : "/WJets-madgraph/akalinow-SkimTauTau_356_pass1-0a3d3891f015a95324f94837322fb8aa-diTauSkim/USER",
        'events_processed' : 9008895,
        'skim_eff' : 0.537,
        'x_sec' : 1.28*24170*_picobarns, # W + jets correction factor for NLO/LO cross-sections = 1.28
        'legendEntry' : plotter.process_WplusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_WplusJets.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_WplusJets,
        'hlt' : cms.InputTag("TriggerResults", "", "HLT"),
        'hlt_paths' : [ '*' ]
    },
    'TTplusJets' : {
        'datasetpath' : "/TTbarJets_Tauola-madgraph/akalinow-SkimTauTau_356_pass1-0a3d3891f015a95324f94837322fb8aa-diTauSkim/USER",
        'events_processed' : 1412199,
        'skim_eff' : 0.949,
        'x_sec' : 95*_picobarns,
        'legendEntry' : plotter.process_TTplusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_TTplusJets.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_TTplusJets,
        'hlt' : cms.InputTag("TriggerResults", "", "HLT"),
        'hlt_paths' : [ '*' ]
    }
}

# Define samples that get merged together
MERGE_SAMPLES = {
    'data' : {
        'samples' : [
            'data_BTau_Run2010B_Prompt',
        ],
        'legendEntry' : 'DATA',
        'type' : 'Data',
        'drawOption' : styles.drawOption_Data
    },
    'qcdSum' : {
        'samples' : [
             'qcdDiJet',
        ],
        'legendEntry' : 'QCD',
        'type' : 'smMC', 
        'drawOption' : styles.drawOption_QCD
    },
    'smBgSum' : {
        'samples' : [
            'Zee',
            'Zmumu',
            'qcdSum',
            'WplusJets',
            'TTplusJets'
        ],
        'legendEntry' : 'SM',
        'type' : 'smMC',
        'drawOption' : styles.drawOption_QCD
    },
    'smSum' : {
        'samples' : [
            'Ztautau',
            'smBgSum'
        ],
        'legendEntry' : 'SM',
        'type' : 'smSumMC',
        'drawOption' : styles.drawOption_QCD
    }
}


# List of all subsamples used in any plot job.  i.e. if qcdSum is included in
# samples to plot it will be expanded to the inclusive/exclusive ppMux samples
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

recoSampleDefinitionsZtoDiTau_7TeV = {
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

