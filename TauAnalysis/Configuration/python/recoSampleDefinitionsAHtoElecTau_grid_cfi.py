import FWCore.ParameterSet.Config as cms
import copy
import itertools

import TauAnalysis.Configuration.plotterProcessDefinitions_cfi as plotter
import TauAnalysis.DQMTools.plotterStyleDefinitions_cfi as styles

import TauAnalysis.Configuration.recoSampleDefinitionsZtoElecTau_grid_cfi as ZtoElecTau

_USE_BARI_XSEC = True
TAN_BETA = 30

# List of samples to run in the analysis
SAMPLES_TO_ANALYZE = copy.deepcopy(ZtoElecTau.SAMPLES_TO_ANALYZE)
SAMPLES_TO_ANALYZE.extend([
    'A90_T3',  'bbA90_T3',
    'A100_T3', 'bbA100_T3',
    'A120_T3', 'bbA120_T3',
    'A130_T3', 'bbA130_T3',
    'A140_T3', 'bbA140_T3',
    'A160_T3', 'bbA160_T3',
    'A180_T3', 'bbA180_T3',
    'A200_T3', 'bbA200_T3',
    'A250_T3', 'bbA250_T3',
    'A300_T3', 'bbA300_T3',
    'A350_T3', 'bbA350_T3',
    'A400_T3', 'bbA400_T3',
    'A450_T3', 'bbA450_T3',
    'A500_T3', 'bbA500_T3',
])

# List of samples to include in the final level plots.  May include selections
# from the MERGE_SAMPLES defined at the bottom.
SAMPLES_TO_PLOT = [
    'data',
    #'A90Sum',
    #'A100Sum',
    #'A120Sum',
    #'A130Sum',
    #'A140Sum',
    #'A160Sum',
	#'A180Sum',
    #'A200Sum',
    #'A300Sum',
    'qcdSum',
    'VVsum',
    'photonPlusJetsSum',
    'TTplusJets_madgraph',
    'WplusJets_madgraph_T3',
    'ZeeSum',
    'ZtautauSum'
]

SAMPLES_TO_PRINT = copy.copy(SAMPLES_TO_PLOT)
#SAMPLES_TO_PRINT.append('VVsum')
#SAMPLES_TO_PRINT.append('TTplusJets_madgraph_T3')
#SAMPLES_TO_PRINT.append('qqZll')
#SAMPLES_TO_PRINT.append('qqZtautau')
#SAMPLES_TO_PRINT.append('A90Sum')
#SAMPLES_TO_PRINT.append('A100Sum')
#SAMPLES_TO_PRINT.append('A120Sum')
#SAMPLES_TO_PRINT.append('A140Sum')
#SAMPLES_TO_PRINT.append('A160Sum')
#SAMPLES_TO_PRINT.append('A180Sum')
#SAMPLES_TO_PRINT.append('A200Sum')
#SAMPLES_TO_PRINT.append('A250Sum')
#SAMPLES_TO_PRINT.append('A250Sum')
#SAMPLES_TO_PRINT.append('A300Sum')
#SAMPLES_TO_PRINT.append('smBgSum')

SAMPLE_DEFAULTS = ZtoElecTau.SAMPLE_DEFAULTS

#--------------------------------------------------------------------------------
# NOTE: cross-sections and branching ratios for Higgs production and decay
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

# Conversions to pico barns
_femtobarns = 1.0e-3

RECO_SAMPLES = copy.deepcopy(ZtoElecTau.RECO_SAMPLES)
AHtoElecTauSpecific_RECO_SAMPLES = {
    'A90_T3' : {
        'datasetpath' : "/GluGluToHToTauTau_M-90_7TeV-powheg-pythia6/jkolb-skimElecTau_423_v1-2453a4eaae124a4a3fe9f365dc31e11f/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 199984,
        'number_of_jobs' : 10,
        'skim_eff' : 24138./199984.,
        'x_sec' : (
             174296.*_femtobarns*0.120567 # (gg -> h0)*(h0->tautau)
           + 191745.*_femtobarns*0.119765 # (gg -> A0)*(A0->tautau)
        ),
        'legendEntry' : 'ggA(90) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate
    },
    'bbA90_T3' : {
        'datasetpath' : "/SUSYBBHToTauTau_M-90_7TeV-pythia6-tauola/jkolb-skimElecTau_423_v1-2453a4eaae124a4a3fe9f365dc31e11f/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 220000,
        'number_of_jobs' : 10,
        'skim_eff' : 26287./220000,
        'x_sec' : (
             319247.*_femtobarns*0.120567 # (bb -> h0)*(h0->tautau)
           + 320295.*_femtobarns*0.119765 # (bb -> A0)*(A0->tautau)
        ),
        'legendEntry' : 'bbA(90) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'A100_T3' : {
        'datasetpath' : "/GluGluToHToTauTau_M-100_7TeV-powheg-pythia6/jkolb-skimElecTau_423_v1-2453a4eaae124a4a3fe9f365dc31e11f/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 199985,
        'number_of_jobs' : 10,
        'skim_eff' : 26667./199985.,
        'x_sec' : (
             102490.*_femtobarns*0.122705 # (gg -> h0)*(h0->tautau)
           + 113695.*_femtobarns*0.121864 # (gg -> A0)*(A0->tautau)
        ),
        'legendEntry' : 'ggA(100) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'bbA100_T3' : {
        'datasetpath' : "/SUSYBBHToTauTau_M-100_7TeV-pythia6-tauola/jkolb-skimElecTau_423_v1-2453a4eaae124a4a3fe9f365dc31e11f/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 220000,
        'number_of_jobs' : 10,
        'skim_eff' : 29173./220000,
        'x_sec' : (
             234065.*_femtobarns*0.122705 # (bb -> h0)*(h0->tautau)
           + 235007.*_femtobarns*0.121864 # (bb -> A0)*(A0->tautau)
        ),
        'legendEntry' : 'bbA(100) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'A110_T3' : {
        'datasetpath' : "/GluGluToHToTauTau_M-110_7TeV-powheg-pythia6/jkolb-skimElecTau_423_v1-2453a4eaae124a4a3fe9f365dc31e11f/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 199988,
        'number_of_jobs' : 10,
        'skim_eff' : 29358./199988,
        'x_sec' : (
              36542.*_femtobarns*0.126386 # (gg -> h0)*(h0->tautau)
           +  48111.*_femtobarns*0.125494 # (gg -> A0)*(A0->tautau)
           +  20248.*_femtobarns*0.128313 # (gg -> H0)*(H0->tautau)
        ),
        'legendEntry' : 'ggA(120) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'A120_T3' : {
        'datasetpath' : "/GluGluToHToTauTau_M-120_7TeV-powheg-pythia6/jkolb-skimElecTau_423_v1-2453a4eaae124a4a3fe9f365dc31e11f/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 199981,
        'number_of_jobs' : 10,
        'skim_eff' : 31433./199981,
        'x_sec' : (
              36542.*_femtobarns*0.126386 # (gg -> h0)*(h0->tautau)
           +  48111.*_femtobarns*0.125494 # (gg -> A0)*(A0->tautau)
           +  20248.*_femtobarns*0.128313 # (gg -> H0)*(H0->tautau)
        ),
        'legendEntry' : 'ggA(120) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'bbA120_T3' : {
        'datasetpath' : "/SUSYBBHToTauTau_M-120_7TeV-pythia6-tauola/jkolb-skimElecTau_423_v1-2453a4eaae124a4a3fe9f365dc31e11f/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 220000,
        'number_of_jobs' : 10,
        'skim_eff' : 34247./220000.,
        'x_sec' : (
             126929.*_femtobarns*0.126386 # (bb -> h0)*(h0->tautau)
           + 133125.*_femtobarns*0.125494 # (bb -> A0)*(A0->tautau)
           +   5801.*_femtobarns*0.129316 # (bb -> H0)*(H0->tautau)
        ),
        'legendEntry' : 'bbA(120) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'A130_T3' : {
        'datasetpath' : "/GluGluToHToTauTau_M-130_7TeV-powheg-pythia6/jkolb-skimElecTau_423_v1-2453a4eaae124a4a3fe9f365dc31e11f/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 199985,
        'number_of_jobs' : 20,
        'skim_eff' : 33094./199985.,
        'x_sec' : (
               6595.*_femtobarns*0.126317 # (gg -> h0)*(h0->tautau)
           +  32421.*_femtobarns*0.127088 # (gg -> A0)*(A0->tautau)
           +  33870.*_femtobarns*0.128313 # (gg -> H0)*(H0->tautau)
        ),
        'legendEntry' : 'ggA(130) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_lightBlue_separate,
    },
    'bbA130_T3' : {
        'datasetpath' : "/SUSYBBHToTauTau_M-130_7TeV-pythia6-tauola/jkolb-skimElecTau_423_v1-2453a4eaae124a4a3fe9f365dc31e11f/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 220000,
        'number_of_jobs' : 10,
        'skim_eff' : 36374./220000,
        'x_sec' : (
              37150.*_femtobarns*0.126317 # (bb -> h0)*(h0->tautau)
           + 102366.*_femtobarns*0.127088 # (bb -> A0)*(A0->tautau)
           +  65807.*_femtobarns*0.128313 # (bb -> H0)*(H0->tautau)
        ),
        'legendEntry' : 'bbA(130) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'A140_T3' : {
        'datasetpath' : "/GluGluToHToTauTau_M-140_7TeV-powheg-pythia6/jkolb-skimElecTau_423_v1-2453a4eaae124a4a3fe9f365dc31e11f/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 191549,
        'number_of_jobs' : 10,
        'skim_eff' : 33740./191549.,
        'x_sec' : (
               8877.*_femtobarns*0.121431 # (gg -> h0)*(h0->tautau)
           +  22263.*_femtobarns*0.128566 # (gg -> A0)*(A0->tautau)
           +  24328.*_femtobarns*0.129151 # (gg -> H0)*(H0->tautau)
        ),
        'legendEntry' : 'ggA(140) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'bbA140_T3' : {
        'datasetpath' : "/SUSYBBHToTauTau_M-140_7TeV-pythia6-tauola/jkolb-skimElecTau_423_v1-2453a4eaae124a4a3fe9f365dc31e11f/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 220000,
        'number_of_jobs' : 10,
        'skim_eff' : 38351./220000,
        'x_sec' : (
               4948.*_femtobarns*0.121432 # (bb -> h0)*(h0->tautau)
           +  79702.*_femtobarns*0.128566 # (bb -> A0)*(A0->tautau)
           +  75246.*_femtobarns*0.129151 # (bb -> H0)*(H0->tautau)
        ),
        'legendEntry' : 'bbA(140) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'A160_T3' : {
        'datasetpath' : "/GluGluToHToTauTau_M-160_7TeV-powheg-pythia6/jkolb-skimElecTau_423_v1-2453a4eaae124a4a3fe9f365dc31e11f/USER",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 199983,
        'number_of_jobs' : 25,
        'skim_eff' : 38111./199983.,
        'x_sec' : (
              11094.*_femtobarns*0.131234 # (gg -> A0)*(A0->tautau)
           +  11977.*_femtobarns*0.131575 # (gg -> H0)*(H0->tautau)
        ),
        'legendEntry' : 'ggA(160) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'bbA160_T3' : {
        'datasetpath' : "/SUSYBBHToTauTau_M-160_7TeV-pythia6-tauola/jkolb-skimElecTau_423_v1-2453a4eaae124a4a3fe9f365dc31e11f/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 220000,
        'number_of_jobs' : 10,
        'skim_eff' : 41816./220000,
        'x_sec' : (
               1118.*_femtobarns*0.131234 # (bb -> A0)*(A0->tautau)
           +  50997.*_femtobarns*0.131575 # (bb -> H0)*(H0->tautau)
        ),
        'legendEntry' : 'bbA(160) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'A180_T3' : {
        'datasetpath' : "/GluGluToHToTauTau_M-180_7TeV-powheg-pythia6/jkolb-skimElecTau_423_v1-2453a4eaae124a4a3fe9f365dc31e11f/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 193230,
        'number_of_jobs' : 10,
        'skim_eff' : 39640./193230,
        'x_sec' : (
               5907.*_femtobarns*0.133061 # (gg -> A0)*(A0->tautau)
           +   6388.*_femtobarns*0.133826 # (gg -> H0)*(H0->tautau)
        ),
        'legendEntry' : 'ggA(180) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_violett_separate,
    },
    'bbA180_T3' : {
        'datasetpath' : "/SUSYBBHToTauTau_M-180_7TeV-pythia6-tauola/jkolb-skimElecTau_423_v1-2453a4eaae124a4a3fe9f365dc31e11f/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 220000,
        'number_of_jobs' : 45614./220000,
        'skim_eff' : 1.0,
        'x_sec' : (
              32261.*_femtobarns*0.133061 # (bb -> A0)*(A0->tautau)
           +  32063.*_femtobarns*0.133826 # (bb -> H0)*(H0->tautau)
        ),
        'legendEntry' : 'bbA(180) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'A200_T3' : {
        'datasetpath' : "/GluGluToHToTauTau_M-200_7TeV-powheg-pythia6/jkolb-skimElecTau_423_v1-2453a4eaae124a4a3fe9f365dc31e11f/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 197673,
        'number_of_jobs' : 10,
        'skim_eff' : 43015./197673,
        'x_sec' : (
               3318.*_femtobarns*0.134295 # (gg -> A0)*(A0->tautau)
           +   3665.*_femtobarns*0.135591 # (gg -> H0)*(H0->tautau)
        ),
        'legendEntry' : 'ggA(200) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'bbA200_T3' : {
        'datasetpath' : "/SUSYBBHToTauTau_M-200_7TeV-pythia6-tauola/jkolb-skimElecTau_423_v1-2453a4eaae124a4a3fe9f365dc31e11f/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 220000,
        'number_of_jobs' : 10,
        'skim_eff' : 47809./220000,
        'x_sec' : (
              21202.*_femtobarns*0.134295 # (bb -> A0)*(A0->tautau)
           +  21128.*_femtobarns*0.135591 # (bb -> H0)*(H0->tautau)
        ),
        'legendEntry' : 'bbA(200) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'A250_T3' : {
        'datasetpath' : "/GluGluToHToTauTau_M-250_7TeV-powheg-pythia6/jkolb-skimElecTau_423_v1-2453a4eaae124a4a3fe9f365dc31e11f/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 199987,
        'number_of_jobs' : 10,
        'skim_eff' : 47724./199987,
        'x_sec' : (
                923.*_femtobarns*0.135441 # (gg -> A0)*(A0->tautau)
           +   1101.*_femtobarns*0.139024 # (gg -> H0)*(H0->tautau)
        ),
        'legendEntry' : 'ggA(250) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'bbA250_T3' : {
        'datasetpath' : "/SUSYBBHToTauTau_M-250_7TeV-pythia6-tauola/jkolb-skimElecTau_423_v1-2453a4eaae124a4a3fe9f365dc31e11f/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 220000,
        'number_of_jobs' : 10,
        'skim_eff' : 53110./220000,
        'x_sec' : (
               8432.*_femtobarns*0.135441 # (bb -> A0)*(A0->tautau)
           +   8414.*_femtobarns*0.139024 # (bb -> H0)*(H0->tautau)
        ),
        'legendEntry' : 'bbA(250) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'A300_T3' : {
        'datasetpath' : "/GluGluToHToTauTau_M-300_7TeV-powheg-pythia6/jkolb-skimElecTau_423_v1-2453a4eaae124a4a3fe9f365dc31e11f/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 196122,
        'number_of_jobs' : 10,
        'skim_eff' : 50724./196122,
        'x_sec' : (
                286.*_femtobarns*0.131076 # (gg -> A0)*(A0->tautau)
           +    387.*_femtobarns*0.139620 # (gg -> H0)*(H0->tautau)
        ),
        'legendEntry' : 'ggA(300) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'bbA300_T3' : {
        'datasetpath' : "/SUSYBBHToTauTau_M-300_7TeV-pythia6-tauola/jkolb-skimElecTau_423_v1-2453a4eaae124a4a3fe9f365dc31e11f/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 220000,
        'number_of_jobs' : 10,
        'skim_eff' : 57334./220000,
        'x_sec' : (
               3752.*_femtobarns*0.131076 # (bb -> A0)*(A0->tautau)
           +   3736.*_femtobarns*0.139620 # (bb -> H0)*(H0->tautau)
        ),
        'legendEntry' : 'bbA(300) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'A350_T3' : {
        'datasetpath' : "/GluGluToHToTauTau_M-350_7TeV-powheg-pythia6/jkolb-skimElecTau_423_v1-2453a4eaae124a4a3fe9f365dc31e11f/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 199504,
        'number_of_jobs' : 10,
        'skim_eff' : 54926./199504,
        'x_sec' : (
                 84.*_femtobarns*0.117009 # (gg -> A0)*(A0->tautau)
           +    121.*_femtobarns*0.133089 # (gg -> H0)*(H0->tautau)
        ),
        'legendEntry' : 'ggA(350) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'bbA350_T3' : {
        'datasetpath' : "/SUSYBBHToTauTau_M-350_7TeV-pythia6-tauola/jkolb-skimElecTau_423_v1-2453a4eaae124a4a3fe9f365dc31e11f/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 220000,
        'number_of_jobs' : 10,
        'skim_eff' : 60268./220000,
        'x_sec' : (
               1830.*_femtobarns*0.117009 # (bb -> A0)*(A0->tautau)
           +   1829.*_femtobarns*0.133089 # (bb -> H0)*(H0->tautau)
        ),
        'legendEntry' : 'bbA(350) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'A400_T3' : {
        'datasetpath' : "/GluGluToHToTauTau_M-400_7TeV-powheg-pythia6/jkolb-skimElecTau_423_v1-2453a4eaae124a4a3fe9f365dc31e11f/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 199984,
        'number_of_jobs' : 10,
        'skim_eff' : 57056./199984,
        'x_sec' : (                 # CV: cross-section * branching-ratio taken directly from Bari .root file
                 1.*_femtobarns*0.1 # (gg -> A0)*(A0->tautau)
           +     1.*_femtobarns*0.1 # (gg -> H0)*(H0->tautau)
        ),
        'legendEntry' : 'ggA(400) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'bbA400_T3' : {
        'datasetpath' : "/SUSYBBHToTauTau_M-400_7TeV-pythia6-tauola/jkolb-skimElecTau_423_v1-2453a4eaae124a4a3fe9f365dc31e11f/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 220000,
        'number_of_jobs' : 10,
        'skim_eff' : 62944./220000,
        'x_sec' : (               # CV: cross-section * branching-ratio taken directly from Bari .root file
               1.*_femtobarns*0.1 # (bb -> A0)*(A0->tautau)
           +   1.*_femtobarns*0.1 # (bb -> H0)*(H0->tautau)
        ),
        'legendEntry' : 'bbA(400) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'A450_T3' : {
        'datasetpath' : "/GluGluToHToTauTau_M-450_7TeV-powheg-pythia6/jkolb-skimElecTau_423_v1-2453a4eaae124a4a3fe9f365dc31e11f/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 199510,
        'number_of_jobs' : 10,
        'skim_eff' : 59076./199510,
        'x_sec' : (                 # CV: cross-section * branching-ratio taken directly from Bari .root file
                 1.*_femtobarns*0.1 # (gg -> A0)*(A0->tautau)
           +     1.*_femtobarns*0.1 # (gg -> H0)*(H0->tautau)
        ),
        'legendEntry' : 'ggA(450) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'bbA450_T3' : {
        'datasetpath' : "/SUSYBBHToTauTau_M-450_7TeV-pythia6-tauola/jkolb-skimElecTau_423_v1-2453a4eaae124a4a3fe9f365dc31e11f/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 220000,
        'number_of_jobs' : 10,
        'skim_eff' : 65606./220000,
        'x_sec' : (               # CV: cross-section * branching-ratio taken directly from Bari .root file
               1.*_femtobarns*0.1 # (bb -> A0)*(A0->tautau)
           +   1.*_femtobarns*0.1 # (bb -> H0)*(H0->tautau)
        ),
        'legendEntry' : 'bbA(450) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'A500_T3' : {
        'datasetpath' : "/GluGluToHToTauTau_M-500_7TeV-powheg-pythia6/jkolb-skimElecTau_423_v1-2453a4eaae124a4a3fe9f365dc31e11f/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 198082,
        'number_of_jobs' : 10,
        'skim_eff' : 60004./198082,
        'x_sec' : (                 # CV: cross-section * branching-ratio taken directly from Bari .root file
                 1.*_femtobarns*0.1 # (gg -> A0)*(A0->tautau)
           +     1.*_femtobarns*0.1 # (gg -> H0)*(H0->tautau)
        ),
        'legendEntry' : 'ggA(500) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'bbA500_T3' : {
        'datasetpath' : "/SUSYBBHToTauTau_M-500_7TeV-pythia6-tauola/jkolb-skimElecTau_423_v1-2453a4eaae124a4a3fe9f365dc31e11f/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 220000,
        'number_of_jobs' : 10,
        'skim_eff' : 67206./220000,
        'x_sec' : (               # CV: cross-section * branching-ratio taken directly from Bari .root file
               1.*_femtobarns*0.1 # (bb -> A0)*(A0->tautau)
           +   1.*_femtobarns*0.1 # (bb -> H0)*(H0->tautau)
        ),
        'legendEntry' : 'bbA(500) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    }
}
RECO_SAMPLES.update(AHtoElecTauSpecific_RECO_SAMPLES)

MERGE_SAMPLES = copy.deepcopy(ZtoElecTau.MERGE_SAMPLES)
AHtoElecTauSpecific_MERGE_SAMPLES = {
    'A90Sum' : {
        'samples' : [ 'A90_T3', 'bbA90_T3' ],
        'legendEntry' : 'A(90) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : RECO_SAMPLES['A90_T3']['drawOption'],
    },
    'A100Sum' : {
        'samples' : [ 'A100_T3', 'bbA100_T3' ],
        'legendEntry' : 'A(100) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : RECO_SAMPLES['A100_T3']['drawOption'],
    },
    'A120Sum' : {
        'samples' : [ 'A120_T3', 'bbA120_T3' ],
        'legendEntry' : 'A(120) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : RECO_SAMPLES['A120_T3']['drawOption'],
    },
    'A130Sum' : {
        'samples' : [ 'A130_T3', 'bbA130_T3' ],
        'legendEntry' : 'A(130) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : RECO_SAMPLES['A130_T3']['drawOption'],
    },
    'A140Sum' : {
        'samples' : [ 'A140_T3', 'bbA140_T3' ],
        'legendEntry' : 'A(140) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : RECO_SAMPLES['A140_T3']['drawOption'],
    },
    'A160Sum' : {
        'samples' : [ 'A160_T3', 'bbA160_T3' ],
        'legendEntry' : 'A(160) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : RECO_SAMPLES['A160_T3']['drawOption'],
    },
    'A180Sum' : {
        'samples' : [ 'A180_T3', 'bbA180_T3' ],
        'legendEntry' : 'A(180) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : RECO_SAMPLES['A180_T3']['drawOption'],
    },
    'A200Sum' : {
        'samples' : [ 'A200_T3', 'bbA200_T3' ],
        'legendEntry' : 'A(200) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : RECO_SAMPLES['A200_T3']['drawOption'],
    },
    'A250Sum' : {
        'samples' : [ 'A250_T3', 'bbA250_T3' ],
        'legendEntry' : 'A(250) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : RECO_SAMPLES['A250_T3']['drawOption'],
    },
    'A300Sum' : {
        'samples' : [ 'A300_T3', 'bbA300_T3' ],
        'legendEntry' : 'A(300) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : RECO_SAMPLES['A300_T3']['drawOption'],
    },
    'A350Sum' : {
        'samples' : [ 'A350_T3', 'bbA350_T3' ],
        'legendEntry' : 'A(350) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : RECO_SAMPLES['A350_T3']['drawOption'],
    },
    'A400Sum' : {
        'samples' : [ 'A400_T3', 'bbA450_T3' ],
        'legendEntry' : 'A(400) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : RECO_SAMPLES['A400_T3']['drawOption'],
    },
    'A450Sum' : {
        'samples' : [ 'A450_T3', 'bbA450_T3' ],
        'legendEntry' : 'A(450) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : RECO_SAMPLES['A450_T3']['drawOption'],
    },
    'A500Sum' : {
        'samples' : [ 'A500_T3', 'bbA500_T3' ],
        'legendEntry' : 'A(500) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : RECO_SAMPLES['A500_T3']['drawOption'],
    }
}
MERGE_SAMPLES.update(AHtoElecTauSpecific_MERGE_SAMPLES)

# Functions that determine whether or not a Higgs (h, H, A) is non-negligble
_inclusion_ranges = {
    'A' : lambda massA: True,
    'H' : lambda massA: massA > 120,
    'h' : lambda massA: massA < 140,
}

# Update higgs sample cross sections
if _USE_BARI_XSEC:
    verbose = False
    if verbose: print "Updating samples to use Bari xsections"
    import re
    import TauAnalysis.Configuration.tools.mssm_xsec as mssm_xsec
    matcher = re.compile(r"(?P<isBB>bb)*A(?P<massA>\d*)")
    higgs_samples = [ sample for sample in SAMPLES_TO_ANALYZE if
                     matcher.match(sample) ]
    higgs_lookups = {}
    for sample in higgs_samples:
        match = matcher.match(sample)
        mass = int(match.group('massA'))
        if verbose:
            print "Updating cross section for sample %s - mA: %i" % (
                sample, mass)
        # Lookup the XSec etc, if we haven't already
        mssm_info = higgs_lookups.setdefault(
            (mass, TAN_BETA), mssm_xsec.query(mass, TAN_BETA))
        # Determine if samples is bb or glu-glu
        production_mechanism = (match.group('isBB') and 'santander' or 'ggF')
        # Compute the total cross section, using multiple higgs if necessary
        total_eff_xsec = 0.0
        for higgs_type in ['H', 'A', 'h']:
            # Determine if we care about this higgs for this mA
            if _inclusion_ranges[higgs_type](mass):
                higgs_dict = mssm_info['higgses'][higgs_type]
                br = higgs_dict['BR']
                # Get the cross section in picobarns
                xsec = (higgs_dict['xsec'][production_mechanism]
                        /mssm_xsec.picobarns)
                if verbose:
                    print "--- %s contributes (BR*xsec) %0.2f * %0.2fpb = %0.2f" % (
                        higgs_type, br, xsec, br*xsec)
                total_eff_xsec += xsec*br
        if verbose:
            print "--- Total effective xsec: %0.2f pb" % total_eff_xsec
        # Convert to the local units (should not matter, but lets be safe)
        total_eff_xsec *= ZtoElecTau._picobarns
        RECO_SAMPLES[sample]['x_sec'] = total_eff_xsec

# List of all subsamples used in any plot job.  i.e. if qcdSum is included in
# samples to plot it will be expanded to the inclusive/exclusive ppMux samples
FLATTENED_SAMPLES_TO_PLOT = set([])

for sample in itertools.chain(SAMPLES_TO_PLOT, SAMPLES_TO_PRINT):
    if sample in MERGE_SAMPLES:
        for subsample in MERGE_SAMPLES[sample]['samples']:
            FLATTENED_SAMPLES_TO_PLOT.add(subsample)
    else:
        FLATTENED_SAMPLES_TO_PLOT.add(sample)

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

recoSampleDefinitionsAHtoElecTau = {
    'SAMPLES_TO_ANALYZE' : SAMPLES_TO_ANALYZE,
    'SAMPLES_TO_PLOT' : SAMPLES_TO_PLOT,
    'SAMPLES_TO_PRINT' : SAMPLES_TO_PRINT,
    'SAMPLE_DEFAULTS' : SAMPLE_DEFAULTS,
    'TARGET_LUMI' : ZtoElecTau.TARGET_LUMI,
    'RECO_SAMPLES' : RECO_SAMPLES,
    'MERGE_SAMPLES' : MERGE_SAMPLES,
    'FLATTENED_SAMPLES_TO_PLOT' : FLATTENED_SAMPLES_TO_PLOT,
    'ALL_SAMPLES' : ALL_SAMPLES
}
