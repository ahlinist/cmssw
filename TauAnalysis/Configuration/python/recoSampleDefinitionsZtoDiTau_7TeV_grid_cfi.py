import FWCore.ParameterSet.Config as cms
import copy

import TauAnalysis.Configuration.plotterProcessDefinitions_cfi as plotter
import TauAnalysis.DQMTools.plotterStyleDefinitions_cfi as styles

# List of samples to run in the analysis
SAMPLES_TO_ANALYZE = [
    'data_JetMETTau_132440_135802_May27ReReco',
    'data_JetMETTau_135821_137436_Jun14ReReco',
    'data_JetMETTau_137437_139558_Prompt',
    'data_JetMETTau_139559_140159_Jul16ReReco',
    'data_BTau_140160_999999_Prompt',
    'Ztautau',
    'Zee',
    'Zmumu',
    'qcdDiJet', 
    'WplusJets',
    'TTplusJets'
] 

# List of samples to include in the final level plots.  May include selections
# from the MERGE_SAMPLES defined at the bottom.
SAMPLES_TO_PLOT = [
    'qcdSum',
    'WplusJets',
    'TTplusJets',
    'Zee',
    'Zmumu',
    'Ztautau'
]

SAMPLES_TO_PRINT = copy.copy(SAMPLES_TO_PLOT)
SAMPLES_TO_PRINT.append('smBgSum')
SAMPLES_TO_PRINT.append('smSum')

SAMPLE_DEFAULTS = {
    'dbs_url' : "http://cmsdbsprod.cern.ch/cms_dbs_ph_analysis_02/servlet/DBSServlet",
    'conditions' : 'startup',
    'genPhaseSpaceCut' : '',
    'factorize' : False,
    'lumi_mask' : '',
    'runselection' : ''
}

# Conversions to pico barns
_femtobarns = 1.0e-3
_millibarns = 1.0e9
_picobarns = 1.0
_nanobarns = 1000.0
_microbarns = 1.0e6

# Integrated luminosity to normalize 
TARGET_LUMI = (200.0)/_picobarns
##TARGET_LUMI = (
##     0.005 # data_JetMETTau_132440_137436_Jun14ReReco
##   + 0.060 # data_JetMETTau_137437_139558_Prompt
##   + 0.110 # data_JetMETTau_139559_140159_Jul16ReReco
##)/_picobarns

RECO_SAMPLES = {
    'data_JetMETTau_132440_135802_May27ReReco' : {
        'datasetpath' : "/MinimumBias/Commissioning10-May27thSkim_SD_JetMETTau-v2/RECO", # run-range in DBS: 131511-135802
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'lumi_mask' : "/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions10/7TeV/Reprocessing/Cert_132440-136119_7TeV_May27thReReco_Collisions10_JSON.txt",
        'runselection' : "132440 - 135802",
        'conditions' : 'GR_R_36X_V12A::All',
        'events_processed' : -1,
        'skim_eff' : 1.0,
        'type' : 'Data',
        'drawOption' : styles.drawOption_Data
    },
    'data_JetMETTau_135821_137436_Jun14ReReco' : {
        'datasetpath' : "/JetMETTau/Run2010A-Jun14thReReco_v2/RECO", # run-range in DBS: 135821-137772
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'lumi_mask' : "/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions10/7TeV/Reprocessing/Cert_132440-137028_7TeV_June14thReReco_Collisions10_JSON.txt",
        'runselection' : "135821 - 137436",
        'conditions' : 'GR_R_36X_V12A::All',
        'events_processed' : -1,
        'skim_eff' : 1.0,
        'type' : 'Data',
        'drawOption' : styles.drawOption_Data
    },
    'data_JetMETTau_137437_139558_Prompt' : {
        'datasetpath' : '/JetMETTau/Run2010A-PromptReco-v4/RECO', # run-range in DBS: 137437-141887
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'runselection' : '137437 - 139778',
        'conditions' : 'GR_R_36X_V12::All',
        'lumi_mask' : "/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions10/7TeV/StreamExpress/Cert_132440-144114_7TeV_StreamExpress_Collisions10_JSON.txt",
        'events_processed' : -1,
        'skim_eff' : 1.0,
        'type' : 'Data',
        'drawOption' : styles.drawOption_Data
    },
    'data_JetMETTau_139559_140159_Jul16ReReco' : {
        'datasetpath' : "/JetMETTau/Run2010A-Jul16thReReco-v1/RECO", # run-range in DBS: 139779-140160
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'lumi_mask' : "/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions10/7TeV/Reprocessing/Cert_139779-140159_7TeV_July16thReReco_Collisions10_JSON.txt",
        'runselection' : "139779 - 140159",
        'conditions' : 'GR_R_36X_V12B::All',
        'events_processed' : -1,
        'skim_eff' : 1.0,
        'type' : 'Data',
        'drawOption' : styles.drawOption_Data
    },
    'data_BTau_140160_999999_Prompt' : {
        'datasetpath' : '/BTau/Run2010A-PromptReco-v4/RECO', # run-range in DBS: 141950-144114
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'runselection' : '140160 - 144114',
        'conditions' : 'GR_R_36X_V12::All',
        'lumi_mask' : "/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions10/7TeV/StreamExpress/Cert_132440-144114_7TeV_StreamExpress_Collisions10_JSON_v2.txt",
        'events_processed' : -1,
        'skim_eff' : 1.0,
        'type' : 'Data',
        'drawOption' : styles.drawOption_Data
    },
    'Ztautau' : {
        'datasetpath' : "/Ztautau/akalinow-SkimTauTau_356_pass1-0a3d3891f015a95324f94837322fb8aa-diTauSkim/USER",
        'events_processed' : 2195255,
        'skim_eff' : 0.321,
        'x_sec' : 1.28*1300*_picobarns, # Z + jets correction factor for NLO/LO cross-sections = 1.28
        'legendEntry' : plotter.process_Ztautau.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Ztautau.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Ztautau
    },
    'Zee' : {
        'datasetpath' : "/Zee/akalinow-SkimTauTau_356_pass1-0a3d3891f015a95324f94837322fb8aa-diTauSkim/USER",
        'events_processed' : 2421575,
        'skim_eff' : 0.600,
        'x_sec' : 1.28*1300*_picobarns, # Z + jets correction factor for NLO/LO cross-sections = 1.28
        'legendEntry' : plotter.process_Zee.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Zee.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Zee
    },
    'Zmumu' : {
        'datasetpath' : "/Zmumu/akalinow-SkimTauTau_356_pass1-0a3d3891f015a95324f94837322fb8aa-diTauSkim/USER",
        'events_processed' : 2111268,
        'skim_eff' : 0.649,
        'x_sec' : 1.28*1300*_picobarns, # Z + jets correction factor for NLO/LO cross-sections = 1.28
        'legendEntry' : plotter.process_Zmumu.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Zmumu.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Zmumu
    },
    'qcdDiJet' : {
        'datasetpath' : "/QCD_Pt15_BiasedTau/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO",
        'events_processed' : 6020758,
        'skim_eff' : 1.000, # unskimmed sample
        'x_sec' : 0.874*_millibarns*0.0338, # 0.874 mb * gen filter efficiency
        'legendEntry' : plotter.process_qcd_W.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_qcd_W.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_QCD,
        'factorize' : True
    },
    'WplusJets' : {
        'datasetpath' : "/WJets-madgraph/akalinow-SkimTauTau_356_pass1-0a3d3891f015a95324f94837322fb8aa-diTauSkim/USER",
        'events_processed' : 9008895,
        'skim_eff' : 0.537,
        'x_sec' : 1.28*24170*_picobarns, # W + jets correction factor for NLO/LO cross-sections = 1.28
        'legendEntry' : plotter.process_WplusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_WplusJets.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_WplusJets
    },
    'TTplusJets' : {
        'datasetpath' : "/TTbarJets_Tauola-madgraph/akalinow-SkimTauTau_356_pass1-0a3d3891f015a95324f94837322fb8aa-diTauSkim/USER",
        'events_processed' : 1412199,
        'skim_eff' : 0.949,
        'x_sec' : 95*_picobarns,
        'legendEntry' : plotter.process_TTplusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_TTplusJets.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_TTplusJets
    },
}

# Define samples that get merged together
MERGE_SAMPLES = {
    'data' : {
        'samples' : [
            'data_JetMETTau_132440_135802_May27ReReco',
            'data_JetMETTau_132440_137436_Jun14ReReco',
            'data_JetMETTau_137437_139558_Prompt',
            'data_JetMETTau_139559_140159_Jul16ReReco',
            'data_BTau_140160_999999_Prompt'
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
            'Zmumu',
            'Zee',
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

