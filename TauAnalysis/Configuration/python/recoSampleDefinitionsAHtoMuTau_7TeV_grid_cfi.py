import FWCore.ParameterSet.Config as cms
import TauAnalysis.Configuration.plotterProcessDefinitions_cfi as plotter
import TauAnalysis.DQMTools.plotterStyleDefinitions_cfi as styles
import copy

# List of samples to run in the analysis
SAMPLES_TO_ANALYZE = [
    #'dataJul16',
    'data_Mu_132440_137436_Jun14ReReco',
    'data_Mu_137437_139558_Prompt',
    'data_Mu_139559_140159_Jul16ReReco',
    'data_Mu_140160_999999_Prompt_v2',
    'A120', 'bbA120', 
    'A130', 'bbA130', 
    'A200', 'bbA200', 
    'Ztautau', 
    'Zmumu',
    'InclusivePPmuX', 
    'PPmuXptGt20', 
    'PPmuXptGt20Mu15', 
    'WplusJets',
    'TTplusJets',
] 

# List of samples to include in the final level plots.  May include selections
# from the MERGE_SAMPLES defined at the bottom.
#SAMPLES_TO_PLOT = ['A120', 'A130', 'A200', 'qcdSum', 'WplusJets', 'Zmumu', 'TTplusJets', 'Ztautau', ]
SAMPLES_TO_PLOT = [
    'data', 
    'A120Sum', 
    'A130Sum', 
    'A200Sum', 
    'qcdSum', 
    #'InclusivePPmuX',
    #'PPmuXptGt20',
    'WplusJets', 
    'Zmumu', 
    'TTplusJets',
    'Ztautau', 
]


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
TARGET_LUMI = (4924.3 + 61275 + 115786 + 808979)/_microbarns

RECO_SAMPLES = {
    'data_Mu_132440_137436_Jun14ReReco' : {
        'datasetpath' : "/Mu/Run2010A-Jun14thReReco_v1/RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'lumi_mask' : "/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions10/7TeV/Reprocessing/Cert_132440-137028_7TeV_June14thReReco_Collisions10_JSON.txt",
        'runselection' : "132440 - 137436",
        'conditions' : 'GR_R_36X_V12B::All',
        'events_processed' : -1,
        'skim_eff' : 1.0,
        'type' : 'Data',
        'drawOption' : styles.drawOption_Data,
    },
    'data_Mu_137437_139558_Prompt' : {
        'datasetpath' : '/Mu/Run2010A-PromptReco-v4/RECO',
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'runselection' : '137437 - 139558',
        'conditions' : 'GR_R_36X_V12::All',
        'lumi_mask' : "/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions10/7TeV/StreamExpress/Cert_132440-143179_7TeV_StreamExpress_Collisions10_JSON.txt",
        'events_processed' : -1,
        'skim_eff' : 1.0,
        'type' : 'Data',
    },
    'data_Mu_139559_140159_Jul16ReReco' : {
        'datasetpath' : "/Mu/Run2010A-Jul16thReReco-v1/RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'lumi_mask' : "/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions10/7TeV/Reprocessing/Cert_139779-140159_7TeV_July16thReReco_Collisions10_JSON.txt",
        'runselection' : "139559 - 140159",
        'conditions' : 'GR_R_36X_V12B::All',
        'events_processed' : -1,
        'skim_eff' : 1.0,
        'type' : 'Data',
        'drawOption' : styles.drawOption_Data,
    },
    'data_Mu_140160_999999_Prompt_v2' : {
        'datasetpath' : '/Mu/Run2010A-PromptReco-v4/RECO',
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'runselection' : '140160 - 999999',
        'conditions' : 'GR_R_36X_V12::All',
        'lumi_mask' : "/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions10/7TeV/StreamExpress/Cert_132440-143179_7TeV_StreamExpress_Collisions10_JSON.txt",
        'events_processed' : -1,
        'skim_eff' : 1.0,
        'type' : 'Data',
    },
    'A120' : {
        # Not skimmed
        'datasetpath' : "/Atautau_M120/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 110670,
        'skim_eff' : 1.0,
        # Configuration/GenProduction/python/PYTHIA6_Tauola_gg_H120_tautau_7TeV_cff.py
        'x_sec' : (
            51947.8*_femtobarns*0.122789 + # (gg -> A0)*(A0->tautau)
            39445.6*_femtobarns*0.123462 + # (gg -> h0)*(h0->tautau)
            20753.1*_femtobarns*0.125908   # (gg -> HH)*(HH->tautau)
        ),
        # Feynhiggs v2.7.1 input to calculate xsec*br -  mhmax, 7TeV
        'legendEntry' : 'ggA(120) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'bbA120' : {
        'datasetpath' : "/bbAtautau_M120/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 99470,
        'skim_eff' : 1.0,
        'x_sec' : (
            64468.1*_femtobarns*0.122789 + # (bb -> A0)*(A0->tautau)
            60252.8*_femtobarns*0.123462 + # (bb -> h0)*(h0->tautau)
            5133.30*_femtobarns*0.125908   # (bb -> HH)*(HH->tautau)
        ),
        'legendEntry' : 'bbA(120) #rightarrow #tau^{+} #tau^{-}',
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
        'x_sec' : (
            35094.3*_femtobarns*0.124340 + # (gg -> A0)*(A0->tautau)
            12089.4*_femtobarns*0.124263 + # (gg -> h0)*(h0->tautau)
            30851.8*_femtobarns*0.125984   # (gg -> HH)*(HH->tautau)
        ),
        # Feynhiggs v2.7.1 input to calculate xsec*br -  mhmax, 7TeV
        'legendEntry' : 'ggA(130) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_green_separate,
    },
    'bbA130' : {
        # Not skimmed
        'datasetpath' : "/bbAtautau_M130/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 101900,
        'skim_eff' : 1.0,
        'x_sec' : (
            104316*_femtobarns*0.124340 + # (bb -> A0)*(A0->tautau)
            65638.3*_femtobarns*0.124263 + # (bb -> h0)*(h0->tautau)
            39203.8*_femtobarns*0.125984   # (bb -> HH)*(HH->tautau)
        ),
        # Feynhiggs v2.7.1 input to calculate xsec*br -  mhmax, 7TeV
        'legendEntry' : 'bbA(130) #rightarrow #tau^{+} #tau^{-}',
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
        'x_sec' : (
            3482.30*_femtobarns*0.131356 + # (gg -> A0)*(A0->tautau)
            #12232.9*_femtobarns*9.501279E-02 + # (gg -> h0)*(h0->tautau)
            3902.27*_femtobarns*0.132576   # (gg -> HH)*(HH->tautau)
        ),
        'legendEntry' : 'ggA(200) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkGray_separate,
    },
    'bbA200' : {
        # Not skimmed
        'datasetpath' : "/bbAtautau_M200/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 101360,
        'skim_eff' : 1.0,
        # Configuration/GenProduction/python/PYTHIA6_Tauola_gg_H200_tautau_7TeV_cff.py
        'x_sec' : (
            21620.8*_femtobarns*0.131356 + # (bb -> A0)*(A0->tautau)
            #556.482*_femtobarns*9.501279E-02 + # (bb -> h0)*(h0->tautau)
            21467.6*_femtobarns*0.132576   # (bb -> HH)*(HH->tautau)
        ), 
        'legendEntry' : 'bbA(200) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkGray_separate,
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
    #    Using the unskimmed dataset
    'Zmumu' : {
        'datasetpath' : "/Zmumu/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO",
        'events_processed' : 2111268,
        'skim_eff' : 1.0,
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
        'factorize' : True,
    },
    'PPmuXptGt20Mu15' : {
        'datasetpath' : "/InclusiveMu15/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO",
        'events_processed' : 4377187,
        'skim_eff' : 1.0,
        'x_sec' : 0.2969*_millibarns*2.684e-4,
        'legendEntry' : plotter.process_PPmuXptGt20.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_PPmuXptGt20.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_QCD,
        'factorize' : True,
    },
    #Using the unskimmed dataset
    'PPmuXptGt20' : {
        'datasetpath' : "/QCD_Pt-20_MuEnrichedPt10_7TeV-pythia6/Spring10-START3X_V26-v1/GEN-SIM-RECO",
        'events_processed' : 6342864, 
        'skim_eff' : 1.0,
        'x_sec' : 296500000*_picobarns*0.00116, # xsec (pb) * gen filter efficiency
        'genPhaseSpaceCut' : 'leadingGenMuon.pt < 15.',
        'legendEntry' : plotter.process_PPmuXptGt20.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_PPmuXptGt20.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_QCD,
        'factorize' : True,
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
    'data' : {
        'samples' : [
            'data_Mu_132440_137436_Jun14ReReco',
            'data_Mu_137437_139558_Prompt',
            'data_Mu_139559_140159_Jul16ReReco',
            'data_Mu_140160_999999_Prompt_v2',
        ],
        'legendEntry' : 'DATA',
        'type' : 'Data',
        'drawOption' : styles.drawOption_Data
    },
    'qcdSum' : {
        #'samples' : ['InclusivePPmuX', 'PPmuXptGt20', 'PPmuXptGt20Mu15'],
        'samples' : ['InclusivePPmuX', 'PPmuXptGt20'],
        'legendEntry' : 'QCD',
        'type' : 'smMC', 
        'drawOption' : styles.drawOption_QCD,
    },
    'A120Sum' : {
        'samples' : ['A120', 'bbA120'],
        'legendEntry' : 'A(120) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : RECO_SAMPLES['A120']['drawOption'],
    },
    'A130Sum' : {
        'samples' : ['A130', 'bbA130'],
        'legendEntry' : 'A(130) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : RECO_SAMPLES['A130']['drawOption'],
    },
    'A200Sum' : {
        'samples' : ['A200', 'bbA200'],
        'legendEntry' : 'A(200) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : RECO_SAMPLES['A200']['drawOption'],
    },
    'smBgSum' : {
        'samples' : ['Ztautau', 'Zmumu', 'InclusivePPmuX', 
                     'PPmuXptGt20', 'WplusJets', 'TTplusJets'],
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



#    'dataJul16' : {
#        'datasetpath' : "/JetMETTau/Run2010A-Jul16thReReco-v1/RECO",
#        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
#        'lumi_mask' : "/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions10/7TeV/Reprocessing/Cert_139779-140159_7TeV_July16thReReco_Collisions10_JSON.txt",
#        'conditions' : 'GR_R_36X_V12B::All',
#        'events_processed' : 3413445,
#        'skim_eff' : 1.0,
#        'legendEntry' : 'DATA',
#        'type' : 'Data',
#        'drawOption' : styles.drawOption_Data,
#    },
#

