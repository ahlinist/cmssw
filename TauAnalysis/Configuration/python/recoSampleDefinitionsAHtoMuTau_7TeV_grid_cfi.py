import FWCore.ParameterSet.Config as cms
import TauAnalysis.Configuration.plotterProcessDefinitions_cfi as plotter
import TauAnalysis.DQMTools.plotterStyleDefinitions_cfi as styles
import copy

# List of samples to run in the analysis
SAMPLES_TO_ANALYZE = ['A120', 'A130', 'A200', 'Ztautau', 'Ztautau09', 'Zmumu',
                      'InclusivePPmuX', 'PPmuXptGt20', 'WplusJets',
                      'TTplusJets' ] 

# List of samples to include in the final level plots.  May include selections
# from the MERGED_SAMPLES defined at the bottom.
SAMPLES_TO_PLOT = ['A120', 'A130', 'A200', 
                   'Ztautau', 'qcdSum', 'WplusJets', 'Zmumu', 'TTplusJets' ]


SAMPLE_DEFAULTS = {
    'dbs_url' : "http://cmsdbsprod.cern.ch/cms_dbs_ph_analysis_02/servlet/DBSServlet",
    'conditions' : 'startup',
    'isData' : False,
    'genPhaseSpaceCut' : '',
}

# Conversions to pico barns
_femtobarns = 1.0e-3
_millibarns = 1.0e9
_picobarns = 1.0

RECO_SAMPLES = {
    'A120' : {
        # Not skimmed
        'datasetpath' : "/Atautau_M120/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 110670,
        'skim_eff' : 1.0,
        # Configuration/GenProduction/python/PYTHIA6_Tauola_gg_H120_tautau_7TeV_cff.py
        'x_sec' : 51947.8*_femtobarns*0.122789,
        # Feynhiggs input to calculate xsec*br
        #MT 173.1
        #TB 30
        #MA0 120
        #Abs(MUE) 200
        #Arg(MUE) 0
        #MSusy 1000
        #Abs(Xt) 2000
        #Arg(Xt) 0
        #Abs(M_2) 200
        #Arg(M_2) 0
        #Abs(M_3) 800.0
        #Arg(M_3) 0
        #prodSqrts 7
        # 
        'legendEntry' : 'A(120) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'A130' : {
        # Not skimmed
        'datasetpath' : "/Atautau_M130/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 115290,
        'skim_eff' : 1.0,
        # Configuration/GenProduction/python/PYTHIA6_Tauola_gg_H130_tautau_7TeV_cff.py
        'x_sec' : 35094.3*_femtobarns*0.124340,
        # Feynhiggs input to calculate xsec*br
        #MT 173.1
        #TB 30
        #MA0 130
        #Abs(MUE) 200
        #Arg(MUE) 0
        #MSusy 1000
        #Abs(Xt) 2000
        #Arg(Xt) 0
        #Abs(M_2) 200
        #Arg(M_2) 0
        #Abs(M_3) 800.0
        #Arg(M_3) 0
        #prodSqrts 7
        'legendEntry' : 'A(130) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_green_separate,
    },
    'A200' : {
        # Not skimmed
        'datasetpath' : "/Atautau_M200/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 111100,
        'skim_eff' : 1.0,
        # Configuration/GenProduction/python/PYTHIA6_Tauola_gg_H200_tautau_7TeV_cff.py
        'x_sec' : 3482.30*_femtobarns*0.131356, 
        # Feynhiggs input to calculate xsec*br
        #MT 173.1
        #TB 30
        #MA0 200
        #Abs(MUE) 200
        #Arg(MUE) 0
        #MSusy 1000
        #Abs(Xt) 2000
        #Arg(Xt) 0
        #Abs(M_2) 200
        #Arg(M_2) 0
        #Abs(M_3) 800.0
        #Arg(M_3) 0
        #prodSqrts 7
        'legendEntry' : 'A(200) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkGray_stacked,
    },
    'Ztautau' : {
        'datasetpath' : "/Ztautau/akalinow-SkimTauTau_356_pass1-0a3d3891f015a95324f94837322fb8aa-muTauSkim/USER",
        'events_processed' : 2195255,
        'skim_eff' : 0.100,
        'x_sec' : 1300*_picobarns, # pb
        'legendEntry' : plotter.process_Ztautau.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Ztautau.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Ztautau,
    },
    # Spring 09 Ztautau with MC conditions
    'Ztautau09' : {
        'datasetpath' : "/Ztautau/Summer09-MC_31X_V3_7TeV-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'conditions' : 'mc',
        'events_processed' : 2193025,
        'skim_eff' : 1.0, # not skimmed
        'x_sec' : 1300*_picobarns, # pb
        'legendEntry' : plotter.process_Ztautau.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Ztautau.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Ztautau,
    },
    'Zmumu' : {
        'datasetpath' : "/Zmumu/akalinow-SkimTauTau_356_pass1-0a3d3891f015a95324f94837322fb8aa-muTauSkim/USER",
        'events_processed' : 2015268,
        'skim_eff' : 0.739,
        'x_sec' : 1300*_picobarns,
        'legendEntry' : plotter.process_Zmumu.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Zmumu.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Zmumu,
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
    },
    'PPmuXptGt20' : {
        'datasetpath' : "/QCD_Pt-20_MuEnrichedPt10_7TeV-pythia6/akalinow-SkimTauTau_356_pass1-af7269240adb80f379a817aa7b511727-muTauSkim/USER",
        'events_processed' : 6342864, 
        'skim_eff' : 0.836,
        'x_sec' : 296500000*_picobarns*0.00116, # xsec (pb) * gen filter efficiency
        'legendEntry' : plotter.process_PPmuXptGt20.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_PPmuXptGt20.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_QCD,
    },
    'WplusJets' : {
        'datasetpath' : "/WJets-madgraph/akalinow-SkimTauTau_356_pass1-0a3d3891f015a95324f94837322fb8aa-muTauSkim/USER",
        'events_processed' : 9008895,
        'skim_eff' : 0.260,
        'x_sec' : 24170*_picobarns, # 24170 from ProductionReProcessingSpring10, "Cross section (pb), including matching efficiency", matching Eff = 0.45
        'legendEntry' : plotter.process_WplusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_WplusJets.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_WplusJets,
    },
    'TTplusJets' : {
        'datasetpath' : "/TTbarJets_Tauola-madgraph/akalinow-SkimTauTau_356_pass1-0a3d3891f015a95324f94837322fb8aa-muTauSkim/USER",
        'events_processed' : 1412199,
        'skim_eff' : 0.399,
        'x_sec' : 95*_picobarns, # Again, same message as above.  Gives matching eff. as 0.35
        'legendEntry' : plotter.process_TTplusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_TTplusJets.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_TTplusJets,
    },
}

# Define samples that get merged together
MERGE_SAMPLES = {
    'qcdSum' : {
        'samples' : ['InclusivePPmuX', 'PPmuXptGt20'],
        'legendEntry' : 'QCD',
        'type' : 'smMC', 
        'drawOption' : styles.drawOption_QCD,
    },
    'smBgSum' : {
        'samples' : ['Ztautau', 'Zmumu', 'InclusivePPmuX', 
                     'PPmuXptGt20', 'WplusJets', 'TTplusJets'],
        'legendEntry' : 'SM',
        'type' : 'smSumMC',
        'drawOption' : styles.drawOption_QCD,
    }
}

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

