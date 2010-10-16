import FWCore.ParameterSet.Config as cms
import copy

import TauAnalysis.Configuration.plotterProcessDefinitions_cfi as plotter
import TauAnalysis.DQMTools.plotterStyleDefinitions_cfi as styles

# List of samples to run in the analysis
SAMPLES_TO_ANALYZE = [
    'data_Mu_132440-135802_Sep17ReReco',
    'data_Mu_135808_144114_Sep17ReReco',
    'data_Mu_144461_145761_Sep17ReReco',
    'data_Mu_145762_147116_Prompt',
    'A120', 'bbA120', 
    'A130', 'bbA130', 
    'A200', 'bbA200', 
    'Ztautau', 
    'Zmumu',
    'InclusivePPmuX', 
    'PPmuXptGt20Mu10', 
    'PPmuXptGt20Mu15', 
    'WplusJets',
    'TTplusJets'
] 

# List of samples to include in the final level plots.  May include selections
# from the MERGE_SAMPLES defined at the bottom.
SAMPLES_TO_PLOT = [
    'data', 
    'A120Sum', 
    'A130Sum', 
    'A200Sum', 
    'qcdSum',
    'WplusJets',
    'TTplusJets',
    'Zmumu',
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
    'runselection' : ''
}

# Conversions to pico barns
_femtobarns = 1.0e-3
_millibarns = 1.0e9
_picobarns = 1.0
_nanobarns = 1000.0
_microbarns = 1.0e6

# Integrated luminosity to normalize 
#TARGET_LUMI = (4924.3 + 61275 + 115786 + 808979)/_microbarns
#TARGET_LUMI = (4652+60315.532+107818+630241.996)/_microbarns
TARGET_LUMI = (4203+58971+89050+1048863)/_microbarns

RECO_SAMPLES = {
    'data_Mu_132440-135802_Sep17ReReco' : {
        'datasetpath' : '/Mu/Run2010A-Sep17ReReco_v2/RECO',
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'lumi_mask' : "/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions10/7TeV/StreamExpress/Cert_132440-147116_7TeV_StreamExpress_Collisions10_JSON.txt",
        'runselection' : "132440 - 135802",
        'conditions' : 'GR_R_38X_V13A::All',
        'events_processed' : -1,
        'skim_eff' : 1.0,
        'type' : 'Data',
        'drawOption' : styles.drawOption_Data
    },
    'data_Mu_135808_144114_Sep17ReReco' : {
        'datasetpath' : '/Mu/Run2010A-Sep17ReReco_v2/RECO',
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'runselection' : '135808 - 144114',
        'conditions' : 'GR_R_38X_V13A::All',
        'lumi_mask' : "/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions10/7TeV/StreamExpress/Cert_132440-147116_7TeV_StreamExpress_Collisions10_JSON.txt",
        'events_processed' : -1,
        'skim_eff' : 1.0,
        'type' : 'Data',
        'drawOption' : styles.drawOption_Data
    },
    'data_Mu_144461_145761_Sep17ReReco' : {
        'datasetpath' : '/Mu/Run2010A-Sep17ReReco_v2/RECO',
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'lumi_mask' : "/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions10/7TeV/StreamExpress/Cert_132440-147116_7TeV_StreamExpress_Collisions10_JSON.txt",
        'runselection' : "144461 - 145761",
        'conditions' : 'GR_R_38X_V13A::All',
        'events_processed' : -1,
        'skim_eff' : 1.0,
        'type' : 'Data',
        'drawOption' : styles.drawOption_Data
    },
    'data_Mu_145762_147116_Prompt' : {
        'datasetpath' : '/Mu/Run2010B-PromptReco-v2/RECO',
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'runselection' : '145762 - 147116',
        'conditions' : 'GR_R_38X_V13::All',
        'lumi_mask' : "/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions10/7TeV/StreamExpress/Cert_132440-147116_7TeV_StreamExpress_Collisions10_JSON.txt",
        'events_processed' : -1,
        'skim_eff' : 1.0,
        'type' : 'Data',
        'drawOption' : styles.drawOption_Data
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
        'enableSysUncertainties' : True
        #'enableSysUncertainties' : False
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
        'enableSysUncertainties' : True
        #'enableSysUncertainties' : False
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
        'enableSysUncertainties' : True
        #'enableSysUncertainties' : False
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
        'enableSysUncertainties' : True
        #'enableSysUncertainties' : False
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
        'enableSysUncertainties' : True
        #'enableSysUncertainties' : False
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
        'enableSysUncertainties' : True
        #'enableSysUncertainties' : False
    },
    # Using the unskimmed dataset
    'Ztautau' : {
        'datasetpath' : "/DYToTauTau_M-20_TuneZ2_7TeV-pythia6-tauola/Fall10-START38_V12-v1/GEN-SIM-RECO",
        'events_processed' : 2057446,
        'skim_eff' : 1.0,
        'x_sec' : 1.28*1300*_picobarns, # Z + jets correction factor for NLO/LO cross-sections = 1.28
        'legendEntry' : plotter.process_Ztautau.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Ztautau.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Ztautau,
        'enableSysUncertainties' : True
        #'enableSysUncertainties' : False
    },
    # Using the unskimmed dataset
    'Zmumu' : {
        'datasetpath' : "/Zmumu/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO",
        'events_processed' : 2111268,
        'skim_eff' : 1.0,
        'x_sec' : 1.28*1300*_picobarns, # Z + jets correction factor for NLO/LO cross-sections = 1.28
        'legendEntry' : plotter.process_Zmumu.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Zmumu.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Zmumu
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
        'factorize' : True
    },
    # Using the unskimmed dataset
    'PPmuXptGt20Mu10' : {
        'datasetpath' : "/QCD_Pt-20_MuEnrichedPt10_7TeV-pythia6/Spring10-START3X_V26-v1/GEN-SIM-RECO",
        'events_processed' : 6342864, 
        'skim_eff' : 1.0,
        'x_sec' : 296500000*_picobarns*0.00116, # xsec (pb) * gen filter efficiency
        'genPhaseSpaceCut' : 'leadingGenMuon.pt < 15.',
        'legendEntry' : plotter.process_PPmuXptGt20.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_PPmuXptGt20.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_QCD,
        'factorize' : True
    },
    # Using the unskimmed dataset
    'PPmuXptGt20Mu15' : {
        'datasetpath' : "/InclusiveMu15/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO",
        'events_processed' : 4377187,
        'skim_eff' : 1.0,
        'x_sec' : 0.2969*_millibarns*2.684e-4,
        'legendEntry' : plotter.process_PPmuXptGt20.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_PPmuXptGt20.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_QCD,
        'factorize' : True
    },    
    'WplusJets' : {
        'datasetpath' : "/WJets-madgraph/akalinow-SkimTauTau_356_pass1-0a3d3891f015a95324f94837322fb8aa-muTauSkim/USER",
        'events_processed' : 9008895,
        'skim_eff' : 0.260,
        'x_sec' : 1.28*24170*_picobarns, # W + jets correction factor for NLO/LO cross-sections = 1.28
        'legendEntry' : plotter.process_WplusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_WplusJets.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_WplusJets
    },
    'TTplusJets' : {
        'datasetpath' : "/TTbarJets_Tauola-madgraph/akalinow-SkimTauTau_356_pass1-0a3d3891f015a95324f94837322fb8aa-muTauSkim/USER",
        'events_processed' : 1412199,
        'skim_eff' : 0.399,
        'x_sec' : 95*_picobarns, # Again, same message as above.  Gives matching eff. as 0.35
        'legendEntry' : plotter.process_TTplusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_TTplusJets.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_TTplusJets
    }
}

# Define samples that get merged together
MERGE_SAMPLES = {
    'data' : {
        'samples' : [
            'data_Mu_132440-135802_Sep17ReReco',
            'data_Mu_135808_144114_Sep17ReReco',
            'data_Mu_144461_145761_Sep17ReReco',
            'data_Mu_145762_147116_Prompt'
        ],
        'legendEntry' : 'DATA',
        'type' : 'Data',
        'drawOption' : styles.drawOption_Data
    },
    'qcdSum' : {
        'samples' : [
            'InclusivePPmuX',
            'PPmuXptGt20Mu10',
            'PPmuXptGt20Mu15'
        ],
        'legendEntry' : 'QCD',
        'type' : 'smMC', 
        'drawOption' : styles.drawOption_QCD,
    },
    'A120Sum' : {
        'samples' : [ 'A120', 'bbA120' ],
        'legendEntry' : 'A(120) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : RECO_SAMPLES['A120']['drawOption'],
    },
    'A130Sum' : {
        'samples' : [ 'A130', 'bbA130' ],
        'legendEntry' : 'A(130) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : RECO_SAMPLES['A130']['drawOption'],
    },
    'A200Sum' : {
        'samples' : [ 'A200', 'bbA200' ],
        'legendEntry' : 'A(200) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : RECO_SAMPLES['A200']['drawOption'],
    },
    'smBgSum' : {
        'samples' : [
            'Ztautau',
            'Zmumu',
            'qcdSum',
            'WplusJets', 
            'TTplusJets'
        ],
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

