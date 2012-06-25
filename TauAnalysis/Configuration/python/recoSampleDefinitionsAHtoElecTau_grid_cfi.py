import FWCore.ParameterSet.Config as cms
import copy
import itertools

import TauAnalysis.Configuration.plotterProcessDefinitions_cfi as plotter
import TauAnalysis.DQMTools.plotterStyleDefinitions_cfi as styles

import TauAnalysis.Configuration.recoSampleDefinitionsZtoElecTau_grid_cfi as ZtoElecTau

_USE_BARI_XSEC = True
TAN_BETA = 20

# List of samples to run in the analysis
SAMPLES_TO_ANALYZE = copy.deepcopy(ZtoElecTau.SAMPLES_TO_ANALYZE)
SAMPLES_TO_ANALYZE.extend([
    'A100_skim', 'bbA100_skim',
    'A120_skim', 'bbA120_skim',
    'A130_skim', 'bbA130_skim',
    'A140_skim', 'bbA140_skim',
    'A160_skim', 'bbA160_skim',
    'A180_skim', 'bbA180_skim',
    'A200_skim', 'bbA200_skim',
    'A250_skim', 'bbA250_skim',
    'A300_skim', 'bbA300_skim',
    'A350_skim', 'bbA350_skim',
    'A400_skim', 'bbA400_skim',
    'A450_skim', 'bbA450_skim',
    'A500_skim', 'bbA500_skim',
    'A600_skim', 'bbA600_skim',
    'A700_skim', 'bbA700_skim',
    'A800_skim', 'bbA800_skim',
    'A900_skim', 'bbA900_skim',
    'A1000_skim', 'bbA1000_skim',
    'h95_skim', 'VBFh95_skim',
    'h100_skim', 'VBFh100_skim',
    'h105_skim', 'VBFh105_skim',
    'h110_skim', 'VBFh110_skim',
    'h115_skim', 'VBFh115_skim',
    'h120_skim', 'VBFh120_skim',
    'h125_skim', 'VBFh125_skim',
    'h130_skim', 'VBFh130_skim',
    'h135_skim', 'VBFh135_skim',
    'h140_skim', 'VBFh140_skim',
    'h145_skim', 'VBFh145_skim',
    'h160_skim',
    'h180_skim',
    'h200_skim',
    'h250_skim',
    'h300_skim',
    'h350_skim',
    'h400_skim',
    'h450_skim',
    'h500_skim',
])

# List of samples to include in the final level plots.  May include selections
# from the MERGE_SAMPLES defined at the bottom.
SAMPLES_TO_PLOT = [
    'VVsum',
    'TTplusJets_madgraph_skim',
    'WplusJets_madgraph_skim',
    'ZeeSum',
    #'EWsum',
    'ZtautauSum',
    'data'
]

SAMPLES_TO_PLOT.append('h125Sum')
SAMPLES_TO_PLOT.append('A140Sum')

SAMPLES_TO_PRINT = copy.copy(SAMPLES_TO_PLOT)

SAMPLE_DEFAULTS = ZtoElecTau.SAMPLE_DEFAULTS

#--------------------------------------------------------------------------------
# NOTE: cross-sections and branching ratios for MSSM Higgs production and decay
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
# cross-sections and branching ratios for SM Higgs production and decay
#  are taken from the Handbook of LHC Higgs Cross Section Observables (arXiv:1101.0593v3)
#
#--------------------------------------------------------------------------------

# Conversions to pico barns
_femtobarns = 1.0e-3

RECO_SAMPLES = copy.deepcopy(ZtoElecTau.RECO_SAMPLES)
AHtoElecTauSpecific_RECO_SAMPLES = {
    'h90_skim' : {
        'datasetpath' : "/GluGluToHToTauTau_M-90_7TeV-powheg-pythia6/jkolb-skimElecTau_428_Fall11_v1-6aa5d932edddb97c8f87b85a020d9993/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 199984,
        'number_of_jobs' : 20,
        'skim_eff' : 23903./199984,
        'x_sec' : 29480.*_femtobarns*0.084,
        'legendEntry' : 'h(90) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'VBFh95_skim' : {
        'datasetpath' : "/VBF_HToTauTau_M-95_7TeV-powheg-pythia6-tauola/jkolb-skimElecTau_428_Fall11_v1-6aa5d932edddb97c8f87b85a020d9993/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 195888,
        'number_of_jobs' : 50,
        'skim_eff' : 1./195888,
        'x_sec' : 1598.*_femtobarns*0.084,
        'legendEntry' : 'VBF h(95) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'h95_skim' : {
        'datasetpath' : "/GluGluToHToTauTau_M-95_7TeV-powheg-pythia6/jkolb-skimElecTau_428_Fall11_v1-6aa5d932edddb97c8f87b85a020d9993/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 197290,
        'number_of_jobs' : 50,
        'skim_eff' : 24786./197290,
        'x_sec' : 26480.*_femtobarns*0.084,
        'legendEntry' : 'h(95) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'A100_skim' : {
        'datasetpath' : "/SUSYGluGluToHToTauTau_M-100_7TeV-pythia6-tauola/jkolb-skimElecTau_428_Fall11_SUSYGluGluToHToTauTau_M-100_7TeV-pythia6-tauola-93adcd4a77296f6fbf0e8af6d73f09f9/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 220000,
        'number_of_jobs' : 50,
        'skim_eff' : 32203./220000,
        'x_sec' : 1, # take cross sections from root file
        'legendEntry' : 'ggA(100) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'bbA100_skim' : {
        'datasetpath' : "/SUSYBBHToTauTau_M-100_7TeV-pythia6-tauola/jkolb-skimElecTau_428_Fall11_SUSYBBHToTauTau_M-100_7TeV-pythia6-tauola-93adcd4a77296f6fbf0e8af6d73f09f9/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 220000,
        'number_of_jobs' : 50,
        'skim_eff' : 32519./220000,
        'x_sec' : 1, # take cross sections from root file
        'legendEntry' : 'bbA(100) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'VBFh100_skim' : {
        'datasetpath' : "/VBF_HToTauTau_M-100_7TeV-powheg-pythia6-tauola/jkolb-skimElecTau_428_Fall11_v1-6aa5d932edddb97c8f87b85a020d9993/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 199166,
        'number_of_jobs' : 50,
        'skim_eff' : 32067./199166,
        'x_sec' : 1530.*_femtobarns*0.0836,
        'legendEntry' : 'VBF h(100) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'h100_skim' : {
        'datasetpath' : "/GluGluToHToTauTau_M-100_7TeV-powheg-pythia6/jkolb-skimElecTau_428_Fall11_v1-6aa5d932edddb97c8f87b85a020d9993/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 199985,
        'number_of_jobs' : 50,
        'skim_eff' : 26192./199985,
        'x_sec' : 23970.*_femtobarns*0.0836,
        'legendEntry' : 'h(100) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'VBFh105_skim' : {
        'datasetpath' : "/VBF_HToTauTau_M-105_7TeV-powheg-pythia6-tauola/jkolb-skimElecTau_428_Fall11_v1-6aa5d932edddb97c8f87b85a020d9993/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 199784,
        'number_of_jobs' : 50,
        'skim_eff' : 32911./199784,
        'x_sec' : 1445.*_femtobarns*0.0825,
        'legendEntry' : 'VBF h(105) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'h105_skim' : {
        'datasetpath' : "/GluGluToHToTauTau_M-105_7TeV-powheg-pythia6/jkolb-skimElecTau_428_Fall11_v1-6aa5d932edddb97c8f87b85a020d9993/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 199982,
        'number_of_jobs' : 50,
        'skim_eff' : 27422./199982,
        'x_sec' : 21740.*_femtobarns*0.0825,
        'legendEntry' : 'h(105) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'VBFh110_skim' : {
        'datasetpath' : "/VBF_HToTauTau_M-110_7TeV-powheg-pythia6-tauola/jkolb-skimElecTau_428_Fall11_v1-6aa5d932edddb97c8f87b85a020d9993/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 198435,
        'number_of_jobs' : 50,
        'skim_eff' : 33919./198435,
        'x_sec' : 1385.*_femtobarns*0.0803,
        'legendEntry' : 'VBF h(110) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'h110_skim' : {
        'datasetpath' : "/GluGluToHToTauTau_M-110_7TeV-powheg-pythia6/jkolb-skimElecTau_428_Fall11_v1-6aa5d932edddb97c8f87b85a020d9993/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 199988,
        'number_of_jobs' : 50,
        'skim_eff' : 28962./199988,
        'x_sec' : 19810.*_femtobarns*0.0803,
        'legendEntry' : 'h(110) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'VBFh115_skim' : {
        'datasetpath' : "/VBF_HToTauTau_M-115_7TeV-powheg-pythia6-tauola/jkolb-skimElecTau_428_Fall11_v1-6aa5d932edddb97c8f87b85a020d9993/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 198051,
        'number_of_jobs' : 50,
        'skim_eff' : 34795./198051,
        'x_sec' : 1312.*_femtobarns*0.0765,
        'legendEntry' : 'VBF h(115) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'h115_skim' : {
        'datasetpath' : "/GluGluToHToTauTau_M-115_7TeV-powheg-pythia6/jkolb-skimElecTau_428_Fall11_v1-6aa5d932edddb97c8f87b85a020d9993/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 196002,
        'number_of_jobs' : 50,
        'skim_eff' : 29310./196002,
        'x_sec' : 18120.*_femtobarns*0.0765,
        'legendEntry' : 'h(115) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'A120_skim' : {
        'datasetpath' : "/SUSYGluGluToHToTauTau_M-120_7TeV-pythia6-tauola/jkolb-skimElecTau_428_Fall11_SUSYGluGluToHToTauTau_M-120_7TeV-pythia6-tauola-7f65814f89474459e41f353668c9096b/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 2196560,
        'number_of_jobs' : 200,
        'skim_eff' : 366414./2196560,
        'x_sec' : (
              36542.*_femtobarns*0.126386 # (gg -> h0)*(h0->tautau)
           +  48111.*_femtobarns*0.125494 # (gg -> A0)*(A0->tautau)
           +  20248.*_femtobarns*0.128313 # (gg -> H0)*(H0->tautau)
        ),
        'legendEntry' : 'ggA(120) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
        'noRunLumiEventSave' : False
    },
    'bbA120_skim' : {
        'datasetpath' : "/SUSYBBHToTauTau_M-120_7TeV-pythia6-tauola/jkolb-skimElecTau_428_Fall11_SUSYBBHToTauTau_M-120_7TeV-pythia6-tauola-93adcd4a77296f6fbf0e8af6d73f09f9/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 2197624,
        'number_of_jobs' : 200,
        'skim_eff' : 378113./2197624,
        'x_sec' : (
             126929.*_femtobarns*0.126386 # (bb -> h0)*(h0->tautau)
           + 133125.*_femtobarns*0.125494 # (bb -> A0)*(A0->tautau)
           +   5801.*_femtobarns*0.129316 # (bb -> H0)*(H0->tautau)
        ),
        'legendEntry' : 'bbA(120) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'VBFh120_skim' : {
        'datasetpath' : "/VBF_HToTauTau_M-120_7TeV-powheg-pythia6-tauola/jkolb-skimElecTau_428_Fall11_v1-6aa5d932edddb97c8f87b85a020d9993/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 198632,
        'number_of_jobs' : 50,
        'skim_eff' : 36014./198632,
        'x_sec' : 1257.*_femtobarns*0.0711,
        'legendEntry' : 'VBF h(120) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'h120_skim' : {
        'datasetpath' : "/GluGluToHToTauTau_M-120_7TeV-powheg-pythia6/jkolb-skimElecTau_428_Fall11_v1-6aa5d932edddb97c8f87b85a020d9993/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 199981,
        'number_of_jobs' : 50,
        'skim_eff' : 31054./199981,
        'x_sec' : 16630.*_femtobarns*0.0711,
        'legendEntry' : 'h(120) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'VBFh125_skim' : {
        'datasetpath' : "/VBF_HToTauTau_M-125_7TeV-powheg-pythia6-tauola/jkolb-skimElecTau_428_Fall11_v1-6aa5d932edddb97c8f87b85a020d9993/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 198612,
        'number_of_jobs' : 80,
        'skim_eff' : 36194./198612,
        'x_sec' : 1193.*_femtobarns*0.0637,
        'legendEntry' : 'VBF h(125) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'h125_skim' : {
        'datasetpath' : "/GluGluToHToTauTau_M-125_7TeV-powheg-pythia6/jkolb-skimElecTau_428_Fall11_v1-6aa5d932edddb97c8f87b85a020d9993/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 199986,
        'number_of_jobs' : 50,
        'skim_eff' : 31691./199986,
        'x_sec' : 15310.*_femtobarns*0.0637,
        'legendEntry' : 'h(125) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'A130_skim' : {
        'datasetpath' : "/SUSYGluGluToHToTauTau_M-130_7TeV-pythia6-tauola/jkolb-skimElecTau_428_Fall11_SUSYGluGluToHToTauTau_M-130_7TeV-pythia6-tauola-93adcd4a77296f6fbf0e8af6d73f09f9/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 220000,
        'number_of_jobs' : 50,
        'skim_eff' : 39010./220000,
        'x_sec' : (
               6595.*_femtobarns*0.126317 # (gg -> h0)*(h0->tautau)
           +  32421.*_femtobarns*0.127088 # (gg -> A0)*(A0->tautau)
           +  33870.*_femtobarns*0.128313 # (gg -> H0)*(H0->tautau)
        ),
        'legendEntry' : 'ggA(130) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_lightBlue_separate,
    },
    'bbA130_skim' : {
        'datasetpath' : "/SUSYBBHToTauTau_M-130_7TeV-pythia6-tauola/jkolb-skimElecTau_428_Fall11_SUSYBBHToTauTau_M-130_7TeV-pythia6-tauola-93adcd4a77296f6fbf0e8af6d73f09f9/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 220000,
        'number_of_jobs' : 50,
        'skim_eff' : 40000./220000,
        'x_sec' : (
              37150.*_femtobarns*0.126317 # (bb -> h0)*(h0->tautau)
           + 102366.*_femtobarns*0.127088 # (bb -> A0)*(A0->tautau)
           +  65807.*_femtobarns*0.128313 # (bb -> H0)*(H0->tautau)
        ),
        'legendEntry' : 'bbA(130) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'VBFh130_skim' : {
        'datasetpath' : "/VBF_HToTauTau_M-130_7TeV-powheg-pythia6-tauola/jkolb-skimElecTau_428_Fall11_v1-6aa5d932edddb97c8f87b85a020d9993/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 197236,
        'number_of_jobs' : 50,
        'skim_eff' : 36899./197236,
        'x_sec' : 1144.*_femtobarns*0.0549,
        'legendEntry' : 'VBF h(130) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'h130_skim' : {
        'datasetpath' : "/GluGluToHToTauTau_M-130_7TeV-powheg-pythia6/jkolb-skimElecTau_428_Fall11_v1-6aa5d932edddb97c8f87b85a020d9993/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 199985,
        'number_of_jobs' : 50,
        'skim_eff' :32604./199985,
        'x_sec' : 14130.*_femtobarns*0.0549,
        'legendEntry' : 'h(130) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'VBFh135_skim' : {
        'datasetpath' : "/VBF_HToTauTau_M-135_7TeV-powheg-pythia6-tauola/jkolb-skimElecTau_428_Fall11_v1-6aa5d932edddb97c8f87b85a020d9993/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 198682,
        'number_of_jobs' : 50,
        'skim_eff' : 38360./198682,
        'x_sec' : 1087.*_femtobarns*0.0452,
        'legendEntry' : 'VBF h(135) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'h135_skim' : {
        'datasetpath' : "/GluGluToHToTauTau_M-135_7TeV-powheg-pythia6/jkolb-skimElecTau_428_Fall11_v1-7425ae7dad1d99a5db224df3689e983a/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 198571,
        'number_of_jobs' : 50,
        'skim_eff' : 33486./198571,
        'x_sec' : 13080.*_femtobarns*0.0452,
        'legendEntry' : 'h(135) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'A140_skim' : {
        'datasetpath' : "/SUSYGluGluToHToTauTau_M-140_7TeV-pythia6-tauola/jkolb-skimElecTau_428_Fall11_SUSYGluGluToHToTauTau_M-140_7TeV-pythia6-tauola-93adcd4a77296f6fbf0e8af6d73f09f9/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 220000,
        'number_of_jobs' : 80,
        'skim_eff' : 41286./220000,
        'x_sec' : 1,
        'legendEntry' : 'ggA(140) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'bbA140_skim' : {
        'datasetpath' : "/SUSYBBHToTauTau_M-140_7TeV-pythia6-tauola/jkolb-skimElecTau_428_Fall11_SUSYBBHToTauTau_M-140_7TeV-pythia6-tauola-93adcd4a77296f6fbf0e8af6d73f09f9/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 220000,
        'number_of_jobs' : 80,
        'skim_eff' : 41944./220000,
        'x_sec' : 1,
        'legendEntry' : 'bbA(140) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'VBFh140_skim' : {
        'datasetpath' : "/VBF_HToTauTau_M-140_7TeV-powheg-pythia6-tauola/jkolb-skimElecTau_428_Fall11_v1-6aa5d932edddb97c8f87b85a020d9993/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 193846,
        'number_of_jobs' : 50,
        'skim_eff' : 37513./193846,
        'x_sec' : 1042.*_femtobarns*0.0354,
        'legendEntry' : 'VBF h(140) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'h140_skim' : {
        'datasetpath' : "/GluGluToHToTauTau_M-140_7TeV-powheg-pythia6/jkolb-skimElecTau_428_Fall11_v1-6aa5d932edddb97c8f87b85a020d9993/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 191549,
        'number_of_jobs' : 50,
        'skim_eff' : 33374./191549,
        'x_sec' : 12140.*_femtobarns*0.0354,
        'legendEntry' : 'h(140) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'VBFh145_skim' : {
        'datasetpath' : "/VBF_HToTauTau_M-145_7TeV-powheg-pythia6-tauola/jkolb-skimElecTau_428_Fall11_v1-6aa5d932edddb97c8f87b85a020d9993/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 197095,
        'number_of_jobs' : 50,
        'skim_eff' : 38958./197095,
        'x_sec' : 992.*_femtobarns*0.0262,
        'legendEntry' : 'VBF h(145) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'h145_skim' : {
        'datasetpath' : "/GluGluToHToTauTau_M-145_7TeV-powheg-pythia6/jkolb-skimElecTau_428_Fall11_v1-6aa5d932edddb97c8f87b85a020d9993/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 199981,
        'number_of_jobs' : 50,
        'skim_eff' : 35335./199981,
        'x_sec' : 11290.*_femtobarns*0.0262,
        'legendEntry' : 'h(145) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'A160_skim' : {
        'datasetpath' : "/SUSYGluGluToHToTauTau_M-160_7TeV-pythia6-tauola/jkolb-skimElecTau_428_Fall11_SUSYGluGluToHToTauTau_M-160_7TeV-pythia6-tauola-93adcd4a77296f6fbf0e8af6d73f09f9/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 220000,
        'number_of_jobs' : 80,
        'skim_eff' : 44422./220000,
        'x_sec' : 1,
        'legendEntry' : 'ggA(160) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'bbA160_skim' : {
        'datasetpath' : "/SUSYBBHToTauTau_M-160_7TeV-pythia6-tauola/jkolb-skimElecTau_428_Fall11_SUSYBBHToTauTau_M-160_7TeV-pythia6-tauola-93adcd4a77296f6fbf0e8af6d73f09f9/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 220000,
        'number_of_jobs' : 80,
        'skim_eff' : 44422./220000,
        'x_sec' : 1,
        'legendEntry' : 'bbA(160) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'A180_skim' : {
        'datasetpath' : "/SUSYGluGluToHToTauTau_M-180_7TeV-pythia6-tauola/jkolb-skimElecTau_428_Fall11_SUSYGluGluToHToTauTau_M-180_7TeV-pythia6-tauola-93adcd4a77296f6fbf0e8af6d73f09f9/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 220000,
        'number_of_jobs' : 80,
        'skim_eff' : 47729./220000,
        'x_sec' : 1,
        'legendEntry' : 'ggA(180) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_violett_separate,
    },
    'bbA180_skim' : {
        'datasetpath' : "/SUSYBBHToTauTau_M-180_7TeV-pythia6-tauola/jkolb-skimElecTau_428_Fall11_SUSYBBHToTauTau_M-180_7TeV-pythia6-tauola-93adcd4a77296f6fbf0e8af6d73f09f9/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 220000,
        'number_of_jobs' : 80,
        'skim_eff' : 49022./220000,
        'x_sec' : 1,
        'legendEntry' : 'bbA(180) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'A200_skim' : {
        'datasetpath' : "/SUSYGluGluToHToTauTau_M-200_7TeV-pythia6-tauola/jkolb-skimElecTau_428_Fall11_SUSYGluGluToHToTauTau_M-200_7TeV-pythia6-tauola-93adcd4a77296f6fbf0e8af6d73f09f9/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 220000,
        'number_of_jobs' : 80,
        'skim_eff' : 50427./220000,
        'x_sec' : 1,
        'legendEntry' : 'ggA(200) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'bbA200_skim' : {
        'datasetpath' : "/SUSYBBHToTauTau_M-200_7TeV-pythia6-tauola/jkolb-skimElecTau_428_Fall11_SUSYBBHToTauTau_M-200_7TeV-pythia6-tauola-93adcd4a77296f6fbf0e8af6d73f09f9/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 220000,
        'number_of_jobs' : 80,
        'skim_eff' : 51416./220000,
        'x_sec' : 1,
        'legendEntry' : 'bbA(200) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'A250_skim' : {
        'datasetpath' : "/SUSYGluGluToHToTauTau_M-250_7TeV-pythia6-tauola/jkolb-skimElecTau_428_Fall11_SUSYGluGluToHToTauTau_M-250_7TeV-pythia6-tauola-93adcd4a77296f6fbf0e8af6d73f09f9/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 220000,
        'number_of_jobs' : 80,
        'skim_eff' : 55502./220000,
        'x_sec' : 1,
        'legendEntry' : 'ggA(250) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'bbA250_skim' : {
        'datasetpath' : "/SUSYBBHToTauTau_M-250_7TeV-pythia6-tauola/jkolb-skimElecTau_428_Fall11_SUSYBBHToTauTau_M-250_7TeV-pythia6-tauola-93adcd4a77296f6fbf0e8af6d73f09f9/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 220000,
        'number_of_jobs' : 80,
        'skim_eff' : 56624./220000,
        'x_sec' : 1,
        'legendEntry' : 'bbA(250) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'A300_skim' : {
        'datasetpath' : "/SUSYGluGluToHToTauTau_M-300_7TeV-pythia6-tauola/jkolb-skimElecTau_428_Fall11_SUSYGluGluToHToTauTau_M-300_7TeV-pythia6-tauola-93adcd4a77296f6fbf0e8af6d73f09f9/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 2198534,
        'number_of_jobs' : 200,
        'skim_eff' : 590153./2198534,
        'x_sec' : 1,
        'legendEntry' : 'ggA(300) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'bbA300_skim' : {
        'datasetpath' : "/SUSYBBHToTauTau_M-300_7TeV-pythia6-tauola/jkolb-skimElecTau_428_Fall11_SUSYBBHToTauTau_M-300_7TeV-pythia6-tauola-93adcd4a77296f6fbf0e8af6d73f09f9/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 2194344,
        'number_of_jobs' : 200,
        'skim_eff' : 605452./2194344,
        'x_sec' : 1,
        'legendEntry' : 'bbA(300) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'A350_skim' : {
        'datasetpath' : "/SUSYGluGluToHToTauTau_M-350_7TeV-pythia6-tauola/jkolb-skimElecTau_428_Fall11_SUSYGluGluToHToTauTau_M-350_7TeV-pythia6-tauola-93adcd4a77296f6fbf0e8af6d73f09f9/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 220000,
        'number_of_jobs' : 80,
        'skim_eff' : 60423./220000,
        'x_sec' : 1,
        'legendEntry' : 'ggA(350) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'bbA350_skim' : {
        'datasetpath' : "/SUSYBBHToTauTau_M-350_7TeV-pythia6-tauola/jkolb-skimElecTau_428_Fall11_SUSYBBHToTauTau_M-350_7TeV-pythia6-tauola-93adcd4a77296f6fbf0e8af6d73f09f9/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 220000,
        'number_of_jobs' : 80,
        'skim_eff' : 63984./220000,
        'x_sec' : 1,
        'legendEntry' : 'bbA(350) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'A400_skim' : {
        'datasetpath' : "/SUSYGluGluToHToTauTau_M-400_7TeV-pythia6-tauola/jkolb-skimElecTau_428_Fall11_SUSYGluGluToHToTauTau_M-400_7TeV-pythia6-tauola-93adcd4a77296f6fbf0e8af6d73f09f9/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 220000,
        'number_of_jobs' : 80,
        'skim_eff' : 62508./220000,
        'x_sec' : 1,                 # CV: cross-section * branching-ratio taken directly from Bari .root file
        'legendEntry' : 'ggA(400) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'bbA400_skim' : {
        'datasetpath' : "/SUSYBBHToTauTau_M-400_7TeV-pythia6-tauola/jkolb-skimElecTau_428_Fall11_SUSYBBHToTauTau_M-400_7TeV-pythia6-tauola-93adcd4a77296f6fbf0e8af6d73f09f9/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 220000,
        'number_of_jobs' : 80,
        'skim_eff' : 66376./220000,
        'x_sec' : (               # CV: cross-section * branching-ratio taken directly from Bari .root file
               1.*_femtobarns*0.1 # (bb -> A0)*(A0->tautau)
           +   1.*_femtobarns*0.1 # (bb -> H0)*(H0->tautau)
        ),
        'legendEntry' : 'bbA(400) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'A450_skim' : {
        'datasetpath' : "/SUSYGluGluToHToTauTau_M-450_7TeV-pythia6-tauola/jkolb-skimElecTau_428_Fall11_SUSYGluGluToHToTauTau_M-450_7TeV-pythia6-tauola-93adcd4a77296f6fbf0e8af6d73f09f9/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 220000,
        'number_of_jobs' : 80,
        'skim_eff' : 62414./220000,
        'x_sec' : (                 # CV: cross-section * branching-ratio taken directly from Bari .root file
                 1.*_femtobarns*0.1 # (gg -> A0)*(A0->tautau)
           +     1.*_femtobarns*0.1 # (gg -> H0)*(H0->tautau)
        ),
        'legendEntry' : 'ggA(450) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'bbA450_skim' : {
        'datasetpath' : "/SUSYBBHToTauTau_M-450_7TeV-pythia6-tauola/jkolb-skimElecTau_428_Fall11_SUSYBBHToTauTau_M-450_7TeV-pythia6-tauola-93adcd4a77296f6fbf0e8af6d73f09f9/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 220000,
        'number_of_jobs' : 80,
        'skim_eff' : 69164./220000,
        'x_sec' : (               # CV: cross-section * branching-ratio taken directly from Bari .root file
               1.*_femtobarns*0.1 # (bb -> A0)*(A0->tautau)
           +   1.*_femtobarns*0.1 # (bb -> H0)*(H0->tautau)
        ),
        'legendEntry' : 'bbA(450) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'A500_skim' : {
        'datasetpath' : "/SUSYGluGluToHToTauTau_M-500_7TeV-pythia6-tauola/jkolb-skimElecTau_428_Fall11_SUSYGluGluToHToTauTau_M-500_7TeV-pythia6-tauola-93adcd4a77296f6fbf0e8af6d73f09f9/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 220000,
        'number_of_jobs' : 80,
        'skim_eff' : 59886./220000,
        'x_sec' : (                 # CV: cross-section * branching-ratio taken directly from Bari .root file
                 1.*_femtobarns*0.1 # (gg -> A0)*(A0->tautau)
           +     1.*_femtobarns*0.1 # (gg -> H0)*(H0->tautau)
        ),
        'legendEntry' : 'ggA(500) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'bbA500_skim' : {
        'datasetpath' : "/SUSYBBHToTauTau_M-500_7TeV-pythia6-tauola/jkolb-skimElecTau_428_Fall11_SUSYBBHToTauTau_M-500_7TeV-pythia6-tauola-93adcd4a77296f6fbf0e8af6d73f09f9/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 220000,
        'number_of_jobs' : 80,
        'skim_eff' : 70794./220000,
        'x_sec' : (               # CV: cross-section * branching-ratio taken directly from Bari .root file
               1.*_femtobarns*0.1 # (bb -> A0)*(A0->tautau)
           +   1.*_femtobarns*0.1 # (bb -> H0)*(H0->tautau)
        ),
        'legendEntry' : 'bbA(500) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'A600_skim' : {
        'datasetpath' : "/SUSYGluGluToHToTauTau_M-600_7TeV-pythia6-tauola/jkolb-skimElecTau_428_Fall11_SUSYGluGluToHToTauTau_M-600_7TeV-pythia6-tauola-93adcd4a77296f6fbf0e8af6d73f09f9/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 216625,
        'number_of_jobs' : 80,
        'skim_eff' : 31608./216625,
        'x_sec' : (                 # CV: cross-section * branching-ratio taken directly from Bari .root file
                 1.*_femtobarns*0.1 # (gg -> A0)*(A0->tautau)
           +     1.*_femtobarns*0.1 # (gg -> H0)*(H0->tautau)
        ),
        'legendEntry' : 'ggA(600) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'bbA600_skim' : {
        'datasetpath' : "/SUSYBBHToTauTau_M-600_7TeV-pythia6-tauola/jkolb-skimElecTau_428_Fall11_SUSYBBHToTauTau_M-600_7TeV-pythia6-tauola-93adcd4a77296f6fbf0e8af6d73f09f9/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 217345,
        'number_of_jobs' : 80,
        'skim_eff' : 72294./217345,
        'x_sec' : (               # CV: cross-section * branching-ratio taken directly from Bari .root file
               1.*_femtobarns*0.1 # (bb -> A0)*(A0->tautau)
           +   1.*_femtobarns*0.1 # (bb -> H0)*(H0->tautau)
        ),
        'legendEntry' : 'bbA(600) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'A700_skim' : {
        'datasetpath' : "/SUSYGluGluToHToTauTau_M-700_7TeV-pythia6-tauola/jkolb-skimElecTau_428_Fall11_SUSYGluGluToHToTauTau_M-700_7TeV-pythia6-tauola-93adcd4a77296f6fbf0e8af6d73f09f9/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 218678,
        'number_of_jobs' : 80,
        'skim_eff' : 26450./218678,
        'x_sec' : (                 # CV: cross-section * branching-ratio taken directly from Bari .root file
                 1.*_femtobarns*0.1 # (gg -> A0)*(A0->tautau)
           +     1.*_femtobarns*0.1 # (gg -> H0)*(H0->tautau)
        ),
        'legendEntry' : 'ggA(700) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'bbA700_skim' : {
        'datasetpath' : "/SUSYBBHToTauTau_M-700_7TeV-pythia6-tauola/jkolb-skimElecTau_428_Fall11_SUSYBBHToTauTau_M-700_7TeV-pythia6-tauola-93adcd4a77296f6fbf0e8af6d73f09f9/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 212314,
        'number_of_jobs' : 80,
        'skim_eff' : 73028./212314,
        'x_sec' : (               # CV: cross-section * branching-ratio taken directly from Bari .root file
               1.*_femtobarns*0.1 # (bb -> A0)*(A0->tautau)
           +   1.*_femtobarns*0.1 # (bb -> H0)*(H0->tautau)
        ),
        'legendEntry' : 'bbA(700) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'A800_skim' : {
        'datasetpath' : "/SUSYGluGluToHToTauTau_M-800_7TeV-pythia6-tauola/jkolb-skimElecTau_428_Fall11_SUSYGluGluToHToTauTau_M-800_7TeV-pythia6-tauola-93adcd4a77296f6fbf0e8af6d73f09f9/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 219229,
        'number_of_jobs' : 80,
        'skim_eff' : 22495./219229,
        'x_sec' : (                 # CV: cross-section * branching-ratio taken directly from Bari .root file
                 1.*_femtobarns*0.1 # (gg -> A0)*(A0->tautau)
           +     1.*_femtobarns*0.1 # (gg -> H0)*(H0->tautau)
        ),
        'legendEntry' : 'ggA(800) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'bbA800_skim' : {
        'datasetpath' : "/SUSYBBHToTauTau_M-800_7TeV-pythia6-tauola/jkolb-skimElecTau_428_Fall11_SUSYBBHToTauTau_M-800_7TeV-pythia6-tauola-93adcd4a77296f6fbf0e8af6d73f09f9/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 213214,
        'number_of_jobs' : 80,
        'skim_eff' : 74163./213214,
        'x_sec' : (               # CV: cross-section * branching-ratio taken directly from Bari .root file
               1.*_femtobarns*0.1 # (bb -> A0)*(A0->tautau)
           +   1.*_femtobarns*0.1 # (bb -> H0)*(H0->tautau)
        ),
        'legendEntry' : 'bbA(800) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'A900_skim' : {
        'datasetpath' : "/SUSYGluGluToHToTauTau_M-900_7TeV-pythia6-tauola/jkolb-skimElecTau_428_Fall11_SUSYGluGluToHToTauTau_M-900_7TeV-pythia6-tauola-93adcd4a77296f6fbf0e8af6d73f09f9/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 215275,
        'number_of_jobs' : 80,
        'skim_eff' : 19204./215275,
        'x_sec' : (                 # CV: cross-section * branching-ratio taken directly from Bari .root file
                 1.*_femtobarns*0.1 # (gg -> A0)*(A0->tautau)
           +     1.*_femtobarns*0.1 # (gg -> H0)*(H0->tautau)
        ),
        'legendEntry' : 'ggA(900) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'bbA900_skim' : {
        'datasetpath' : "/SUSYBBHToTauTau_M-900_7TeV-pythia6-tauola/jkolb-skimElecTau_428_Fall11_SUSYBBHToTauTau_M-900_7TeV-pythia6-tauola-93adcd4a77296f6fbf0e8af6d73f09f9/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 213214,
        'number_of_jobs' : 80,
        'skim_eff' : 74209./213214,
        'x_sec' : (               # CV: cross-section * branching-ratio taken directly from Bari .root file
               1.*_femtobarns*0.1 # (bb -> A0)*(A0->tautau)
           +   1.*_femtobarns*0.1 # (bb -> H0)*(H0->tautau)
        ),
        'legendEntry' : 'bbA(900) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'A1000_skim' : {
        'datasetpath' : "/SUSYGluGluToHToTauTau_M-1000_7TeV-pythia6-tauola/jkolb-skimElecTau_428_Fall11_SUSYGluGluToHToTauTau_M-1000_7TeV-pythia6-tauola-93adcd4a77296f6fbf0e8af6d73f09f9/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 211904,
        'number_of_jobs' : 80,
        'skim_eff' : 16516./211904,
        'x_sec' : (                 # CV: cross-section * branching-ratio taken directly from Bari .root file
                 1.*_femtobarns*0.1 # (gg -> A0)*(A0->tautau)
           +     1.*_femtobarns*0.1 # (gg -> H0)*(H0->tautau)
        ),
        'legendEntry' : 'ggA(1000) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'bbA1000_skim' : {
        'datasetpath' : "/SUSYBBHToTauTau_M-1000_7TeV-pythia6-tauola/jkolb-skimElecTau_428_Fall11_SUSYBBHToTauTau_M-1000_7TeV-pythia6-tauola-93adcd4a77296f6fbf0e8af6d73f09f9/USER",
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'events_processed' : 213526,
        'number_of_jobs' : 80,
        'skim_eff' : 74420./213526,
        'x_sec' : (               # CV: cross-section * branching-ratio taken directly from Bari .root file
               1.*_femtobarns*0.1 # (bb -> A0)*(A0->tautau)
           +   1.*_femtobarns*0.1 # (bb -> H0)*(H0->tautau)
        ),
        'legendEntry' : 'bbA(1000) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    }
}
RECO_SAMPLES.update(AHtoElecTauSpecific_RECO_SAMPLES)

MERGE_SAMPLES = copy.deepcopy(ZtoElecTau.MERGE_SAMPLES)
AHtoElecTauSpecific_MERGE_SAMPLES = {
    'h95Sum' : {
        'samples' : [ 'h95_skim', 'VBFh95_skim' ],
        'legendEntry' : 'h(95) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : RECO_SAMPLES['h95_skim']['drawOption'],
    },
    'h100Sum' : {
        'samples' : [ 'h100_skim', 'VBFh100_skim' ],
        'legendEntry' : 'h(100) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : RECO_SAMPLES['h100_skim']['drawOption'],
    },
    'h105Sum' : {
        'samples' : [ 'h105_skim', 'VBFh105_skim' ],
        'legendEntry' : 'h(105) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : RECO_SAMPLES['h105_skim']['drawOption'],
    },
    'h110Sum' : {
        'samples' : [ 'h110_skim', 'VBFh110_skim' ],
        'legendEntry' : 'h(110) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : RECO_SAMPLES['h110_skim']['drawOption'],
    },
    'h115Sum' : {
        'samples' : [ 'h115_skim', 'VBFh115_skim' ],
        'legendEntry' : 'h(115) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : RECO_SAMPLES['h115_skim']['drawOption'],
    },
    'h120Sum' : {
        'samples' : [ 'h120_skim', 'VBFh120_skim' ],
        'legendEntry' : 'h(120) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : RECO_SAMPLES['h120_skim']['drawOption'],
    },
    'h125Sum' : {
        'samples' : [ 'h125_skim', 'VBFh125_skim' ],
        'legendEntry' : 'h(125) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : RECO_SAMPLES['h125_skim']['drawOption'],
    },
    'h130Sum' : {
        'samples' : [ 'h130_skim', 'VBFh130_skim' ],
        'legendEntry' : 'h(130) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : RECO_SAMPLES['h130_skim']['drawOption'],
    },
    'h135Sum' : {
        'samples' : [ 'h135_skim', 'VBFh135_skim' ],
        'legendEntry' : 'h(135) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : RECO_SAMPLES['h135_skim']['drawOption'],
    },
    'h140Sum' : {
        'samples' : [ 'h140_skim', 'VBFh140_skim' ],
        'legendEntry' : 'h(140) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : RECO_SAMPLES['h140_skim']['drawOption'],
    },
    'h145Sum' : {
        'samples' : [ 'h145_skim', 'VBFh145_skim' ],
        'legendEntry' : 'h(145) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : RECO_SAMPLES['h145_skim']['drawOption'],
    },
    'A100Sum' : {
        'samples' : [ 'A100_skim', 'bbA100_skim' ],
        'legendEntry' : 'A(100) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : RECO_SAMPLES['A100_skim']['drawOption'],
    },
    'A120Sum' : {
        'samples' : [ 'A120_skim', 'bbA120_skim' ],
        'legendEntry' : 'A(120) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : RECO_SAMPLES['A120_skim']['drawOption'],
    },
    'A130Sum' : {
        'samples' : [ 'A130_skim', 'bbA130_skim' ],
        'legendEntry' : 'A(130) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : RECO_SAMPLES['A130_skim']['drawOption'],
    },
    'A140Sum' : {
        'samples' : [ 'A140_skim', 'bbA140_skim' ],
        'legendEntry' : 'A(140) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : RECO_SAMPLES['A140_skim']['drawOption'],
    },
    'A160Sum' : {
        'samples' : [ 'A160_skim', 'bbA160_skim' ],
        'legendEntry' : 'A(160) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : RECO_SAMPLES['A160_skim']['drawOption'],
    },
    'A180Sum' : {
        'samples' : [ 'A180_skim', 'bbA180_skim' ],
        'legendEntry' : 'A(180) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : RECO_SAMPLES['A180_skim']['drawOption'],
    },
    'A200Sum' : {
        'samples' : [ 'A200_skim', 'bbA200_skim' ],
        'legendEntry' : 'A(200) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : RECO_SAMPLES['A200_skim']['drawOption'],
    },
    'A250Sum' : {
        'samples' : [ 'A250_skim', 'bbA250_skim' ],
        'legendEntry' : 'A(250) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : RECO_SAMPLES['A250_skim']['drawOption'],
    },
    'A300Sum' : {
        'samples' : [ 'A300_skim', 'bbA300_skim' ],
        'legendEntry' : 'A(300) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : RECO_SAMPLES['A300_skim']['drawOption'],
    },
    'A350Sum' : {
        'samples' : [ 'A350_skim', 'bbA350_skim' ],
        'legendEntry' : 'A(350) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : RECO_SAMPLES['A350_skim']['drawOption'],
    },
    'A400Sum' : {
        'samples' : [ 'A400_skim', 'bbA400_skim' ],
        'legendEntry' : 'A(400) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : RECO_SAMPLES['A400_skim']['drawOption'],
    },
    'A450Sum' : {
        'samples' : [ 'A450_skim', 'bbA450_skim' ],
        'legendEntry' : 'A(450) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : RECO_SAMPLES['A450_skim']['drawOption'],
    },
    'A500Sum' : {
        'samples' : [ 'A500_skim', 'bbA500_skim' ],
        'legendEntry' : 'A(500) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : RECO_SAMPLES['A500_skim']['drawOption'],
    },
    'A600Sum' : {
        'samples' : [ 'A600_skim', 'bbA600_skim' ],
        'legendEntry' : 'A(600) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : RECO_SAMPLES['A600_skim']['drawOption'],
    },
    'A700Sum' : {
        'samples' : [ 'A700_skim', 'bbA700_skim' ],
        'legendEntry' : 'A(700) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : RECO_SAMPLES['A700_skim']['drawOption'],
    },
    'A800Sum' : {
        'samples' : [ 'A800_skim', 'bbA800_skim' ],
        'legendEntry' : 'A(800) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : RECO_SAMPLES['A800_skim']['drawOption'],
    },
    'A900Sum' : {
        'samples' : [ 'A900_skim', 'bbA900_skim' ],
        'legendEntry' : 'A(900) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : RECO_SAMPLES['A900_skim']['drawOption'],
    },
    'A1000Sum' : {
        'samples' : [ 'A1000_skim', 'bbA1000_skim' ],
        'legendEntry' : 'A(1000) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : RECO_SAMPLES['A1000_skim']['drawOption'],
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
    import os
    import re
    from HiggsAnalysis.HiggsToTauTau.tools.mssm_xsec_tools import mssm_xsec_tools
    _FILE_NAME = os.path.join(os.environ['CMSSW_BASE'], 'src/HiggsAnalysis/HiggsToTauTau/data', 'out.mhmax_7_nnlo.root')
    helper = mssm_xsec_tools(_FILE_NAME)
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
            (mass, TAN_BETA), helper.query(mass, TAN_BETA))
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
                        /helper.unit_pb)
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
