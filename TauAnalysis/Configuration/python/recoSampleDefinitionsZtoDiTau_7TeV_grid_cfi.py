import FWCore.ParameterSet.Config as cms
import TauAnalysis.Configuration.plotterProcessDefinitions_cfi as plotter
import TauAnalysis.DQMTools.plotterStyleDefinitions_cfi as styles
import copy

# List of samples to run in the analysis
SAMPLES_TO_ANALYZE = [
    'Ztautau',
    'Zee',
    'Zmumu',
    'qcdDiJet', 
    'WplusJets',
    'TTplusJets',
] 

# List of samples to include in the final level plots.  May include selections
# from the MERGE_SAMPLES defined at the bottom.
SAMPLES_TO_PLOT = [
    'qcdDiJet',
    'WplusJets',
    'Zee',
    'Zmumu',
    'TTplusJets',
    'Ztautau'
]

SAMPLES_TO_PRINT = copy.copy(SAMPLES_TO_PLOT)
SAMPLES_TO_PRINT.append('smBgSum')

SAMPLE_DEFAULTS = {
    'dbs_url' : "http://cmsdbsprod.cern.ch/cms_dbs_ph_analysis_02/servlet/DBSServlet",
    'conditions' : 'startup',
    'genPhaseSpaceCut' : '',
    'factorize' : False,
    'lumi_mask' : '',
    'runselection' : '',
}

# Conversions to pico barns
_femtobarns = 1.0e-3
_millibarns = 1.0e9
_picobarns = 1.0
_nanobarns = 1000.0
_microbarns = 1.0e6

# Integrated luminosity to normalize 
TARGET_LUMI = (200.0)/_picobarns

RECO_SAMPLES = {
    'Ztautau' : {
        'datasetpath' : "/Ztautau/akalinow-SkimTauTau_356_pass1-0a3d3891f015a95324f94837322fb8aa-diTauSkim/USER",
        'events_processed' : 2195255,
        'skim_eff' : 0.321,
        'x_sec' : 1300*_picobarns,
        'legendEntry' : plotter.process_Ztautau.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Ztautau.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Ztautau,
    },
    'Zee' : {
        'datasetpath' : "/Zee/akalinow-SkimTauTau_356_pass1-0a3d3891f015a95324f94837322fb8aa-diTauSkim/USER",
        'events_processed' : 2421575,
        'skim_eff' : 0.600,
        'x_sec' : 1300*_picobarns,
        'legendEntry' : plotter.process_Zmumu.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Zmumu.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Zmumu,
    },
    'Zmumu' : {
        'datasetpath' : "/Zmumu/akalinow-SkimTauTau_356_pass1-0a3d3891f015a95324f94837322fb8aa-diTauSkim/USER",
        'events_processed' : 2111268,
        'skim_eff' : 0.649,
        'x_sec' : 1300*_picobarns,
        'legendEntry' : plotter.process_Zmumu.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Zmumu.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Zmumu,
    },
    'qcdDiJet' : {
        'datasetpath' : "/QCD_Pt15_BiasedTau/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO",
        'events_processed' : 6020758,
        'skim_eff' : 1.000, # unskimmed sample
        'x_sec' : 87.4*_millibarns*0.0338, # 48.44 mb * gen filter efficiency
        'legendEntry' : plotter.process_qcd_W.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_qcd_W.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_QCD,
        'factorize' : True,
    },
    'WplusJets' : {
        'datasetpath' : "/WJets-madgraph/akalinow-SkimTauTau_356_pass1-0a3d3891f015a95324f94837322fb8aa-diTauSkim/USER",
        'events_processed' : 9008895,
        'skim_eff' : 0.537,
        'x_sec' : 24170*_picobarns,
        'legendEntry' : plotter.process_WplusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_WplusJets.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_WplusJets,
    },
    'TTplusJets' : {
        'datasetpath' : "/TTbarJets_Tauola-madgraph/akalinow-SkimTauTau_356_pass1-0a3d3891f015a95324f94837322fb8aa-diTauSkim/USER",
        'events_processed' : 1412199,
        'skim_eff' : 0.949,
        'x_sec' : 95*_picobarns,
        'legendEntry' : plotter.process_TTplusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_TTplusJets.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_TTplusJets,
    },
}

# Define samples that get merged together
MERGE_SAMPLES = {
    'smBgSum' : {
        'samples' : [ 'Zmumu', 'Zee', 'qcdDiJet', 'WplusJets', 'TTplusJets' ],
        'legendEntry' : 'SM',
        'type' : 'smSumMC',
        'drawOption' : styles.drawOption_QCD,
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

