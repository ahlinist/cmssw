import FWCore.ParameterSet.Config as cms
import copy

import TauAnalysis.Configuration.plotterProcessDefinitions_cfi as plotter
import TauAnalysis.DQMTools.plotterStyleDefinitions_cfi as styles

# List of samples to run in the analysis
SAMPLES_TO_ANALYZE = [
    'data_Mu_132440-145761_Sep17ReReco',
    'data_Mu_145762_147116_Prompt',
    'data_Mu_147117_148058_Prompt',
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
    #'PPmuXptGt20Mu15',
    'WplusJets',
    'TTplusJets',
    'Zmumu',
    'Ztautau'
]

SAMPLES_TO_PRINT = copy.copy(SAMPLES_TO_PLOT)
#SAMPLES_TO_PRINT.append('smBgSum')

SAMPLE_DEFAULTS = {
    'dbs_url' : "http://cmsdbsprod.cern.ch/cms_dbs_ph_analysis_02/servlet/DBSServlet",
    'conditions' : 'startup',
    'genPhaseSpaceCut' : '',
    'factorize' : False,
    'enableSysUncertainties' : False,
    'lumi_mask' : '',
    'runselection' : '',
    'hlt_paths' : [ 'HLT_Mu9' ]
}

# Conversions to pico barns
_femtobarns = 1.0e-3
_picobarns = 1.0
_nanobarns = 1000.0
_microbarns = 1.0e6
_millibarns = 1.0e9

# Integrated luminosity to normalize
#TARGET_LUMI = (4924.3 + 61275 + 115786 + 808979)/_microbarns
#TARGET_LUMI = (4652+60315.532+107818+630241.996)/_microbarns
#TARGET_LUMI = (4203+58971+89050+1048863)/_microbarns

#Finding lumi for crab/crabdir_runAHtoMuTau_AHtoMuTau_data_Mu_132440-145761_Sep17ReReco_Run10
#Dir: crab/crabdir_runAHtoMuTau_AHtoMuTau_data_Mu_132440-145761_Sep17ReReco_Run10    LUMI:  2939136.41826
#Finding lumi for crab/crabdir_runAHtoMuTau_AHtoMuTau_data_Mu_145762_147454_Prompt_Run10
#Dir: crab/crabdir_runAHtoMuTau_AHtoMuTau_data_Mu_145762_147454_Prompt_Run10    LUMI:  4692057.60607
#TOTAL INTEGRATED LUMINOSITY:  7631194.02434  MICROBARNS
TARGET_LUMI = (15.00)/_picobarns

#--------------------------------------------------------------------------------
# NOTE:
#   (1) cross-sections for W and Z production are scaled to next-to-leading order values
#       documented on the wiki
#        http://alcaraz.web.cern.ch/alcaraz/CROSS_SECTIONS.txt
#   (2) cross-sections and branching ratios for Higgs production and decay
#       are obtained with FeynHiggs (version 2.7.4) for "maximal mixing scenario" (cf. CMS AN-09-143),
#       using the following configuration parameters settings:
#
#         MT 173.3
#         TB 30
#         MA0 120
#         Abs(MUE) 200
#         Arg(MUE) 0
#         MSusy 1000
#         Abs(Xt) 2449.5
#         Arg(Xt) 0
#         Abs(M_2) 200
#         Arg(M_2) 0
#         Abs(M_3) 800.0
#         Arg(M_3) 0
#         prodSqrts 7
#
#--------------------------------------------------------------------------------

RECO_SAMPLES = {
    'data_Mu_132440-145761_Sep17ReReco' : {
        'datasetpath' : '/Mu/Run2010A-Sep17ReReco_v2/RECO',
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'lumi_mask' : "/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions10/7TeV/StreamExpress/Cert_132440-148058_7TeV_StreamExpress_Collisions10_JSON.txt",
        'runselection' : "132440 - 145761",
        'conditions' : 'GR_R_38X_V13A::All',
        'events_processed' : -1,
        'skim_eff' : 1.0,
        'type' : 'Data',
        'drawOption' : styles.drawOption_Data
    },
    'data_Mu_145762_147116_Prompt' : {
        'datasetpath' : '/Mu/Run2010B-PromptReco-v2/RECO',
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'lumi_mask' : "/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions10/7TeV/StreamExpress/Cert_132440-148058_7TeV_StreamExpress_Collisions10_JSON.txt",
        'runselection' : '145762 - 147116',
        'conditions' : 'GR_R_38X_V13::All',
        'events_processed' : -1,
        'skim_eff' : 1.0,
        'type' : 'Data',
        'drawOption' : styles.drawOption_Data,
    },
    'data_Mu_147117_148058_Prompt' : {
        'datasetpath' : '/Mu/Run2010B-PromptReco-v2/RECO',
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'lumi_mask' : "/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions10/7TeV/StreamExpress/Cert_132440-148058_7TeV_StreamExpress_Collisions10_JSON.txt",
        'runselection' : '147117 - 148058',
        'conditions' : 'GR_R_38X_V13::All',
        'events_processed' : -1,
        'skim_eff' : 1.0,
        'type' : 'Data',
        'drawOption' : styles.drawOption_Data,
        'hlt_paths' : [ 'HLT_Mu11', 'HLT_IsoMu9' ],
    },
    'A120' : {
        # Not skimmed
        'datasetpath' : "/Atautau_M120/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 110670,
        'number_of_jobs' : 25,
        'skim_eff' : 1.0,
        # Configuration/GenProduction/python/PYTHIA6_Tauola_gg_H120_tautau_7TeV_cff.py
        'x_sec' : (
              48110.*_femtobarns*0.122789 # (gg -> A0)*(A0->tautau)
           +  36542.*_femtobarns*0.123462 # (gg -> h0)*(h0->tautau)
           +  20247.*_femtobarns*0.125908 # (gg -> H0)*(H0->tautau)
        ),
        # Feynhiggs v2.7.1 input to calculate xsec*br -  mhmax, 7TeV
        'legendEntry' : 'ggA(120) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
        'enableSysUncertainties' : True
    },
    'bbA120' : {
        'datasetpath' : "/bbAtautau_M120/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 99470,
        'number_of_jobs' : 25,
        'skim_eff' : 1.0,
        'x_sec' : (
             133125.*_femtobarns*0.122789 # (bb -> A0)*(A0->tautau)
           + 126929.*_femtobarns*0.123462 # (bb -> h0)*(h0->tautau)
           +   5801.*_femtobarns*0.125908 # (bb -> H0)*(H0->tautau)
        ),
        'legendEntry' : 'bbA(120) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
        'enableSysUncertainties' : True
    },
    'A130' : {
        # Not skimmed
        'datasetpath' : "/Atautau_M130/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 115290,
        'number_of_jobs' : 25,
        'skim_eff' : 1.0,
        # Configuration/GenProduction/python/PYTHIA6_Tauola_gg_H130_tautau_7TeV_cff.py
        'x_sec' : (
              32421.*_femtobarns*0.124340 # (gg -> A0)*(A0->tautau)
           +   6595.*_femtobarns*0.124263 # (gg -> h0)*(h0->tautau)
           +  33870.*_femtobarns*0.125984 # (gg -> H0)*(H0->tautau)
        ),
        # Feynhiggs v2.7.1 input to calculate xsec*br -  mhmax, 7TeV
        'legendEntry' : 'ggA(130) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_green_separate,
        'enableSysUncertainties' : True
    },
    'bbA130' : {
        # Not skimmed
        'datasetpath' : "/bbAtautau_M130/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 101900,
        'number_of_jobs' : 25,
        'skim_eff' : 1.0,
        'x_sec' : (
             102366.*_femtobarns*0.124340 # (bb -> A0)*(A0->tautau)
           +  37150.*_femtobarns*0.124263 # (bb -> h0)*(h0->tautau)
           +  65807.*_femtobarns*0.125984 # (bb -> H0)*(H0->tautau)
        ),
        # Feynhiggs v2.7.1 input to calculate xsec*br -  mhmax, 7TeV
        'legendEntry' : 'bbA(130) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_green_separate,
        'enableSysUncertainties' : True
    },
    'A200' : {
        # Not skimmed
        'datasetpath' : "/Atautau_M200/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 111100,
        'number_of_jobs' : 25,
        'skim_eff' : 1.0,
        # Configuration/GenProduction/python/PYTHIA6_Tauola_gg_H200_tautau_7TeV_cff.py
        'x_sec' : (
               # CV, EK: (gg -> h0)*(h0->tautau) cross-section not included, as h0 mass is very different from mA
               3318.*_femtobarns*0.131356 # (gg -> A0)*(A0->tautau)
           +   3665.*_femtobarns*0.132576 # (gg -> H0)*(H0->tautau)
        ),
        'legendEntry' : 'ggA(200) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkGray_separate,
        'enableSysUncertainties' : True
    },
    'bbA200' : {
        # Not skimmed
        'datasetpath' : "/bbAtautau_M200/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 101360,
        'number_of_jobs' : 25,
        'skim_eff' : 1.0,
        # Configuration/GenProduction/python/PYTHIA6_Tauola_gg_H200_tautau_7TeV_cff.py
        'x_sec' : (
              # CV, EK: (bb -> h0)*(h0->tautau) cross-section not included, as h0 mass is very different from mA
              21202.*_femtobarns*0.131356 # (bb -> A0)*(A0->tautau)
           +  21128.*_femtobarns*0.132576 # (bb -> HH)*(H0->tautau)
        ),
        'legendEntry' : 'bbA(200) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkGray_separate,
        'enableSysUncertainties' : True
    },
    # Using the unskimmed dataset
    'Ztautau' : {
        ##'datasetpath' : "/Ztautau/akalinow-SkimTauTau_356_pass1-0a3d3891f015a95324f94837322fb8aa-muTauSkim/USER",
        ##'events_processed' : 2195255,
        ##'skim_eff' : 0.100,
        'datasetpath' : "/DYToTauTau_M-20_TuneZ2_7TeV-pythia6-tauola/Fall10-START38_V12-v1/GEN-SIM-RECO",
        'dbs_url' : "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 2057446,
        'number_of_jobs' : 500,
        'skim_eff' : 1.0,
        'x_sec' : 1.28*1300*_picobarns, # Z + jets correction factor for NLO/LO cross-sections = 1.28
        'legendEntry' : plotter.process_Ztautau.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Ztautau.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Ztautau,
        'enableSysUncertainties' : True,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI38X")
    },
    # Using the unskimmed dataset
    'Zmumu' : {
        'datasetpath' : "/Zmumu/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO",
        'events_processed' : 2111268,
        'number_of_jobs' : 500,
        'skim_eff' : 1.0,
        'x_sec' : 1.28*1300*_picobarns, # Z + jets correction factor for NLO/LO cross-sections = 1.28
        'legendEntry' : plotter.process_Zmumu.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Zmumu.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Zmumu
    },
    'InclusivePPmuX' : {
        'datasetpath' : "/ppMuX/akalinow-SkimTauTau_356_pass1-0a3d3891f015a95324f94837322fb8aa-muTauSkim/USER",
        'events_processed' : 9878911,
        'number_of_jobs' : 50,
        'skim_eff' : 0.017,
        'x_sec' : 48.44*_millibarns*0.00176, # 48.44 mb * gen filter efficiency
        'genPhaseSpaceCut' : 'ptHat < 20. | leadingGenMuon.pt < 10.',
        'legendEntry' : plotter.process_InclusivePPmuX.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_InclusivePPmuX.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_QCD,
        'enableSysUncertainties' : True,
        'factorize' : True
    },
    # Using the unskimmed dataset
    'PPmuXptGt20Mu10' : {
        'datasetpath' : "/QCD_Pt-20_MuEnrichedPt10_7TeV-pythia6/Spring10-START3X_V26-v1/GEN-SIM-RECO",
        'events_processed' : 6342864,
        'number_of_jobs' : 25,
        'skim_eff' : 1.0,
        'x_sec' : 296500000*_picobarns*0.00116, # xsec (pb) * gen filter efficiency
        'genPhaseSpaceCut' : 'leadingGenMuon.pt < 15.',
        'legendEntry' : plotter.process_PPmuXptGt20.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_PPmuXptGt20.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_QCD,
        'enableSysUncertainties' : True,
        'factorize' : True
    },
    # Using the unskimmed dataset
    'PPmuXptGt20Mu15' : {
        'datasetpath' : "/InclusiveMu15/Summer10-START36_V9_S09-v1/GEN-SIM-RECO",
        'events_processed' : 5120334,
        'number_of_jobs' : 1000,
        'skim_eff' : 1.0,
        'x_sec' : 0.2969*_millibarns*2.684e-4,
        'legendEntry' : plotter.process_PPmuXptGt20.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_PPmuXptGt20.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_QCD,
        'enableSysUncertainties' : True,
        'factorize' : True,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI36X")
    },
    'WplusJets' : {
        'datasetpath' : "/WJets-madgraph/akalinow-SkimTauTau_356_pass1-0a3d3891f015a95324f94837322fb8aa-muTauSkim/USER",
        'events_processed' : 9008895,
        'number_of_jobs' : 500,
        'skim_eff' : 0.260,
        'x_sec' : 1.28*24170*_picobarns, # W + jets correction factor for NLO/LO cross-sections = 1.28
        'legendEntry' : plotter.process_WplusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_WplusJets.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_WplusJets,
        'enableSysUncertainties' : True
    },
    'TTplusJets' : {
        'datasetpath' : "/TTbarJets_Tauola-madgraph/akalinow-SkimTauTau_356_pass1-0a3d3891f015a95324f94837322fb8aa-muTauSkim/USER",
        'events_processed' : 1412199,
        'number_of_jobs' : 125,
        'skim_eff' : 0.399,
        'x_sec' : 95*_picobarns, # Again, same message as above.  Gives matching eff. as 0.35
        'legendEntry' : plotter.process_TTplusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_TTplusJets.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_TTplusJets,
        'enableSysUncertainties' : True
    }
}

# Define samples that get merged together
MERGE_SAMPLES = {
    'data' : {
        'samples' : [
            'data_Mu_132440-145761_Sep17ReReco',
            'data_Mu_145762_147116_Prompt',
            'data_Mu_147117_148058_Prompt'
        ],
        'legendEntry' : 'DATA',
        'type' : 'Data',
        'drawOption' : styles.drawOption_Data
    },
    'qcdSum' : {
        'samples' : [
            #EK: need to change the list of samples entering smBgSum if you edit this!
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
    #'smBgSum' : {
        #'samples' : [
            #'Ztautau',
            #'Zmumu',
            #'qcdSum',
            #'InclusivePPmuX',
            #'PPmuXptGt20Mu10',
            #'PPmuXptGt20Mu15',
            #'WplusJets',
            #'TTplusJets'
        #],
        #'legendEntry' : 'SM',
        #'type' : 'smSumMC',
        #'drawOption' : styles.drawOption_QCD,
    #}
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

recoSampleDefinitionsAHtoMuTau_7TeV = {
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
