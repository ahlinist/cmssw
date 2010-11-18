import FWCore.ParameterSet.Config as cms
import copy
import itertools

import TauAnalysis.Configuration.plotterProcessDefinitions_cfi as plotter
import TauAnalysis.DQMTools.plotterStyleDefinitions_cfi as styles

import TauAnalysis.Configuration.recoSampleDefinitionsZtoMuTau_7TeV_grid_cfi as ZtoMuTau

_USE_BARI_XSEC = True
TAN_BETA = 30

# List of samples to run in the analysis
SAMPLES_TO_ANALYZE = copy.deepcopy(ZtoMuTau.SAMPLES_TO_ANALYZE)
SAMPLES_TO_ANALYZE.extend([
    'A90',  'bbA90',
    'A100', 'bbA100',
    'A120', 'bbA120',
    'A130', 'bbA130',
    'A140', 'bbA140',
    'A160', 'bbA160',
    'A180', 'bbA180',
    'A200', 'bbA200',
    'A250', 'bbA250',
    'A300', 'bbA300',
    'A350', 'bbA350',
])

# List of samples to include in the final level plots.  May include selections
# from the MERGE_SAMPLES defined at the bottom.
SAMPLES_TO_PLOT = [
    #'data',
    ##'A90Sum',
    #'A100Sum',
    ##'A120Sum',
    #'A130Sum',
    ##'A140Sum',
    ##'A160Sum',
    #'A180Sum',
    ##'A200Sum',
    ##'A300Sum',
    #'qcdSum',
    #'WplusJets',
    ##'TTplusJets',
    #'Zmumu',
    ##'Ztautau'
    #'ZtautauPU156bx',
]

SAMPLES_TO_PRINT = copy.copy(SAMPLES_TO_PLOT)
#SAMPLES_TO_PRINT.append('A90Sum')
#SAMPLES_TO_PRINT.append('A100Sum')
#SAMPLES_TO_PRINT.append('A120Sum')
#SAMPLES_TO_PRINT.append('A140Sum')
#SAMPLES_TO_PRINT.append('A160Sum')
#SAMPLES_TO_PRINT.append('A180Sum')
SAMPLES_TO_PRINT.append('A200Sum')
SAMPLES_TO_PRINT.append('A250Sum')
#SAMPLES_TO_PRINT.append('A250Sum')
#SAMPLES_TO_PRINT.append('A350Sum')
#SAMPLES_TO_PRINT.append('smBgSum')

SAMPLE_DEFAULTS = ZtoMuTau.SAMPLE_DEFAULTS

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

_femtobarns = 1.0e-3

RECO_SAMPLES = copy.deepcopy(ZtoMuTau.RECO_SAMPLES)
AHtoMuTauSpecific_RECO_SAMPLES = {
    'A90' : {
        # Not skimmed
        'datasetpath' : "/SUSYGluGluToHToTauTau_M-90_7TeV-pythia6-tauola/Fall10-START38_V12-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 110000,
        'number_of_jobs' : 25,
        'skim_eff' : 1.0,
        'x_sec' : (
             174296.*_femtobarns*0.120567 # (gg -> h0)*(h0->tautau)
           + 191745.*_femtobarns*0.119765 # (gg -> A0)*(A0->tautau)
        ),
        # Feynhiggs v2.7.1 input to calculate xsec*br -  mhmax, 7TeV
        'legendEntry' : 'ggA(90) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
        'enableSysUncertainties' : True,
        'applyZrecoilCorrection' : True
    },
    'bbA90' : {
        # Not skimmed
        'datasetpath' : "/SUSYBBHToTauTau_M-90_7TeV-pythia6-tauola/Fall10-START38_V12-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 110000,
        'number_of_jobs' : 25,
        'skim_eff' : 1.0,
        'x_sec' : (
             319247.*_femtobarns*0.120567 # (bb -> h0)*(h0->tautau)
           + 320295.*_femtobarns*0.119765 # (bb -> A0)*(A0->tautau)
        ),
        # Feynhiggs v2.7.1 input to calculate xsec*br -  mhmax, 7TeV
        'legendEntry' : 'bbA(90) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
        'enableSysUncertainties' : True,
        'applyZrecoilCorrection' : True
    },
    'A100' : {
        # Not skimmed
        'datasetpath' : "/SUSYGluGluToHToTauTau_M-100_7TeV-pythia6-tauola/Fall10-START38_V12-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 110000,
        'number_of_jobs' : 25,
        'skim_eff' : 1.0,
        'x_sec' : (
             102490.*_femtobarns*0.122705 # (gg -> h0)*(h0->tautau)
           + 113695.*_femtobarns*0.121864 # (gg -> A0)*(A0->tautau)
        ),
        # Feynhiggs v2.7.1 input to calculate xsec*br -  mhmax, 7TeV
        'legendEntry' : 'ggA(100) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
        'enableSysUncertainties' : True,
        'applyZrecoilCorrection' : True
    },
    'bbA100' : {
        # Not skimmed
        'datasetpath' : "/SUSYBBHToTauTau_M-100_7TeV-pythia6-tauola/Fall10-START38_V12-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 110000,
        'number_of_jobs' : 25,
        'skim_eff' : 1.0,
        'x_sec' : (
             234065.*_femtobarns*0.122705 # (bb -> h0)*(h0->tautau)
           + 235007.*_femtobarns*0.121864 # (bb -> A0)*(A0->tautau)
        ),
        # Feynhiggs v2.7.1 input to calculate xsec*br -  mhmax, 7TeV
        'legendEntry' : 'bbA(100) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
        'enableSysUncertainties' : True,
        'applyZrecoilCorrection' : True
    },
    'A120' : {
        # Not skimmed
        'datasetpath' : "/SUSYGluGluToHToTauTau_M-120_7TeV-pythia6-tauola/Fall10-START38_V12-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 110000,
        'number_of_jobs' : 25,
        'skim_eff' : 1.0,
        'x_sec' : (
              36542.*_femtobarns*0.126386 # (gg -> h0)*(h0->tautau)
           +  48111.*_femtobarns*0.125494 # (gg -> A0)*(A0->tautau)
           +  20248.*_femtobarns*0.128313 # (gg -> H0)*(H0->tautau)
        ),
        # Feynhiggs v2.7.1 input to calculate xsec*br -  mhmax, 7TeV
        'legendEntry' : 'ggA(120) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
        'enableSysUncertainties' : True,
        'applyZrecoilCorrection' : True
    },
    'bbA120' : {
        # Not skimmed
        'datasetpath' : "/SUSYBBHToTauTau_M-120_7TeV-pythia6-tauola/Fall10-START38_V12-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 110000,
        'number_of_jobs' : 25,
        'skim_eff' : 1.0,
        'x_sec' : (
             126929.*_femtobarns*0.126386 # (bb -> h0)*(h0->tautau)
           + 133125.*_femtobarns*0.125494 # (bb -> A0)*(A0->tautau)
           +   5801.*_femtobarns*0.129316 # (bb -> H0)*(H0->tautau)
        ),
        # Feynhiggs v2.7.1 input to calculate xsec*br -  mhmax, 7TeV
        'legendEntry' : 'bbA(120) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
        'enableSysUncertainties' : True,
        'applyZrecoilCorrection' : True
    },
    'A130' : {
        # Not skimmed
        'datasetpath' : "/SUSYGluGluToHToTauTau_M-130_7TeV-pythia6-tauola/Fall10-START38_V12-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 110000,
        'number_of_jobs' : 25,
        'skim_eff' : 1.0,
        'x_sec' : (
               6595.*_femtobarns*0.126317 # (gg -> h0)*(h0->tautau)
           +  32421.*_femtobarns*0.127088 # (gg -> A0)*(A0->tautau)
           +  33870.*_femtobarns*0.128313 # (gg -> H0)*(H0->tautau)
        ),
        # Feynhiggs v2.7.1 input to calculate xsec*br -  mhmax, 7TeV
        'legendEntry' : 'ggA(130) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_lightBlue_separate,
        'enableSysUncertainties' : True,
        'applyZrecoilCorrection' : True
    },
    'bbA130' : {
        # Not skimmed
        'datasetpath' : "/SUSYBBHToTauTau_M-130_7TeV-pythia6-tauola/Fall10-START38_V12-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 110000,
        'number_of_jobs' : 25,
        'skim_eff' : 1.0,
        'x_sec' : (
              37150.*_femtobarns*0.126317 # (bb -> h0)*(h0->tautau)
           + 102366.*_femtobarns*0.127088 # (bb -> A0)*(A0->tautau)
           +  65807.*_femtobarns*0.128313 # (bb -> H0)*(H0->tautau)
        ),
        # Feynhiggs v2.7.1 input to calculate xsec*br -  mhmax, 7TeV
        'legendEntry' : 'bbA(130) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
        'enableSysUncertainties' : True,
        'applyZrecoilCorrection' : True
    },
    'A140' : {
        # Not skimmed
        'datasetpath' : "/SUSYGluGluToHToTauTau_M-1400_7TeV-pythia6-tauola/Fall10-START38_V12-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 110000,
        'number_of_jobs' : 25,
        'skim_eff' : 1.0,
        'x_sec' : (
               8877.*_femtobarns*0.121431 # (gg -> h0)*(h0->tautau)
           +  22263.*_femtobarns*0.128566 # (gg -> A0)*(A0->tautau)
           +  24328.*_femtobarns*0.129151 # (gg -> H0)*(H0->tautau)
        ),
        # Feynhiggs v2.7.1 input to calculate xsec*br -  mhmax, 7TeV
        'legendEntry' : 'ggA(140) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
        'enableSysUncertainties' : True,
        'applyZrecoilCorrection' : True
    },
    'bbA140' : {
        # Not skimmed
        'datasetpath' : "/SUSYBBHToTauTau_M-140_7TeV-pythia6-tauola/Fall10-START38_V12-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 110000,
        'number_of_jobs' : 25,
        'skim_eff' : 1.0,
        'x_sec' : (
               4948.*_femtobarns*0.121432 # (bb -> h0)*(h0->tautau)
           +  79702.*_femtobarns*0.128566 # (bb -> A0)*(A0->tautau)
           +  75246.*_femtobarns*0.129151 # (bb -> H0)*(H0->tautau)
        ),
        # Feynhiggs v2.7.1 input to calculate xsec*br -  mhmax, 7TeV
        'legendEntry' : 'bbA(140) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
        'enableSysUncertainties' : True,
        'applyZrecoilCorrection' : True
    },
    'A160' : {
        # Not skimmed
        'datasetpath' : "/SUSYGluGluToHToTauTau_M-160_7TeV-pythia6-tauola/Fall10-START38_V12-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 110000,
        'number_of_jobs' : 25,
        'skim_eff' : 1.0,
        'x_sec' : (
              11094.*_femtobarns*0.131234 # (gg -> A0)*(A0->tautau)
           +  11977.*_femtobarns*0.131575 # (gg -> H0)*(H0->tautau)
        ),
        # Feynhiggs v2.7.1 input to calculate xsec*br -  mhmax, 7TeV
        'legendEntry' : 'ggA(160) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
        'enableSysUncertainties' : True,
        'applyZrecoilCorrection' : True
    },
    'bbA160' : {
        # Not skimmed
        'datasetpath' : "/SUSYBBHToTauTau_M-160_7TeV-pythia6-tauola/Fall10-START38_V12-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 110000,
        'number_of_jobs' : 25,
        'skim_eff' : 1.0,
        'x_sec' : (
               1118.*_femtobarns*0.131234 # (bb -> A0)*(A0->tautau)
           +  50997.*_femtobarns*0.131575 # (bb -> H0)*(H0->tautau)
        ),
        # Feynhiggs v2.7.1 input to calculate xsec*br -  mhmax, 7TeV
        'legendEntry' : 'bbA(160) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
        'enableSysUncertainties' : True,
        'applyZrecoilCorrection' : True
    },
    'A180' : {
        # Not skimmed
        'datasetpath' : "/SUSYGluGluToHToTauTau_M-180_7TeV-pythia6-tauola/Fall10-START38_V12-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 110000,
        'number_of_jobs' : 25,
        'skim_eff' : 1.0,
        'x_sec' : (
               5907.*_femtobarns*0.133061 # (gg -> A0)*(A0->tautau)
           +   6388.*_femtobarns*0.133826 # (gg -> H0)*(H0->tautau)
        ),
        # Feynhiggs v2.7.1 input to calculate xsec*br -  mhmax, 7TeV
        'legendEntry' : 'ggA(180) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_violett_separate,
        'enableSysUncertainties' : True,
        'applyZrecoilCorrection' : True
    },
    'bbA180' : {
        # Not skimmed
        'datasetpath' : "/SUSYBBHToTauTau_M-180_7TeV-pythia6-tauola/Fall10-START38_V12-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 110000,
        'number_of_jobs' : 25,
        'skim_eff' : 1.0,
        'x_sec' : (
              32261.*_femtobarns*0.133061 # (bb -> A0)*(A0->tautau)
           +  32063.*_femtobarns*0.133826 # (bb -> H0)*(H0->tautau)
        ),
        # Feynhiggs v2.7.1 input to calculate xsec*br -  mhmax, 7TeV
        'legendEntry' : 'bbA(180) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
        'enableSysUncertainties' : True,
        'applyZrecoilCorrection' : True
    },
    'A200' : {
        # Not skimmed
        'datasetpath' : "/SUSYGluGluToHToTauTau_M-200_7TeV-pythia6-tauola/Fall10-START38_V12-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 110000,
        'number_of_jobs' : 25,
        'skim_eff' : 1.0,
        'x_sec' : (
               3318.*_femtobarns*0.134295 # (gg -> A0)*(A0->tautau)
           +   3665.*_femtobarns*0.135591 # (gg -> H0)*(H0->tautau)
        ),
        # Feynhiggs v2.7.1 input to calculate xsec*br -  mhmax, 7TeV
        'legendEntry' : 'ggA(200) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
        'enableSysUncertainties' : True,
        'applyZrecoilCorrection' : True
    },
    'bbA200' : {
        # Not skimmed
        'datasetpath' : "/SUSYBBHToTauTau_M-200_7TeV-pythia6-tauola/Fall10-START38_V12-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 110000,
        'number_of_jobs' : 25,
        'skim_eff' : 1.0,
        'x_sec' : (
              21202.*_femtobarns*0.134295 # (bb -> A0)*(A0->tautau)
           +  21128.*_femtobarns*0.135591 # (bb -> H0)*(H0->tautau)
        ),
        # Feynhiggs v2.7.1 input to calculate xsec*br -  mhmax, 7TeV
        'legendEntry' : 'bbA(200) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
        'enableSysUncertainties' : True,
        'applyZrecoilCorrection' : True
    },
    'A250' : {
        # Not skimmed
        'datasetpath' : "/SUSYGluGluToHToTauTau_M-250_7TeV-pythia6-tauola/Fall10-START38_V12-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 110000,
        'number_of_jobs' : 25,
        'skim_eff' : 1.0,
        'x_sec' : (
                923.*_femtobarns*0.135441 # (gg -> A0)*(A0->tautau)
           +   1101.*_femtobarns*0.139024 # (gg -> H0)*(H0->tautau)
        ),
        # Feynhiggs v2.7.1 input to calculate xsec*br -  mhmax, 7TeV
        'legendEntry' : 'ggA(250) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
        'enableSysUncertainties' : True,
        'applyZrecoilCorrection' : True
    },
    'bbA250' : {
        # Not skimmed
        'datasetpath' : "/SUSYBBHToTauTau_M-250_7TeV-pythia6-tauola/Fall10-START38_V12-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 110000,
        'number_of_jobs' : 25,
        'skim_eff' : 1.0,
        'x_sec' : (
               8432.*_femtobarns*0.135441 # (bb -> A0)*(A0->tautau)
           +   8414.*_femtobarns*0.139024 # (bb -> H0)*(H0->tautau)
        ),
        # Feynhiggs v2.7.1 input to calculate xsec*br -  mhmax, 7TeV
        'legendEntry' : 'bbA(250) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
        'enableSysUncertainties' : True,
        'applyZrecoilCorrection' : True
    },
    'A300' : {
        # Not skimmed
        'datasetpath' : "/SUSYGluGluToHToTauTau_M-300_7TeV-pythia6-tauola/Fall10-START38_V12-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 110000,
        'number_of_jobs' : 25,
        'skim_eff' : 1.0,
        'x_sec' : (
                286.*_femtobarns*0.131076 # (gg -> A0)*(A0->tautau)
           +    387.*_femtobarns*0.139620 # (gg -> H0)*(H0->tautau)
        ),
        # Feynhiggs v2.7.1 input to calculate xsec*br -  mhmax, 7TeV
        'legendEntry' : 'ggA(300) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
        'enableSysUncertainties' : True,
        'applyZrecoilCorrection' : True
    },
    'bbA300' : {
        # Not skimmed
        'datasetpath' : "/SUSYBBHToTauTau_M-300_7TeV-pythia6-tauola/Fall10-START38_V12-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 110000,
        'number_of_jobs' : 25,
        'skim_eff' : 1.0,
        'x_sec' : (
               3752.*_femtobarns*0.131076 # (bb -> A0)*(A0->tautau)
           +   3736.*_femtobarns*0.139620 # (bb -> H0)*(H0->tautau)
        ),
        # Feynhiggs v2.7.1 input to calculate xsec*br -  mhmax, 7TeV
        'legendEntry' : 'bbA(300) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
        'enableSysUncertainties' : True,
        'applyZrecoilCorrection' : True
    },
    'A350' : {
        # Not skimmed
        'datasetpath' : "/SUSYGluGluToHToTauTau_M-350_7TeV-pythia6-tauola/Fall10-START38_V12-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 110000,
        'number_of_jobs' : 25,
        'skim_eff' : 1.0,
        'x_sec' : (
                 84.*_femtobarns*0.117009 # (gg -> A0)*(A0->tautau)
           +    121.*_femtobarns*0.133089 # (gg -> H0)*(H0->tautau)
        ),
        # Feynhiggs v2.7.1 input to calculate xsec*br -  mhmax, 7TeV
        'legendEntry' : 'ggA(350) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
        'enableSysUncertainties' : True,
        'applyZrecoilCorrection' : True
    },
    'bbA350' : {
        # Not skimmed
        'datasetpath' : "/SUSYBBHToTauTau_M-350_7TeV-pythia6-tauola/Fall10-START38_V12-v1/GEN-SIM-RECO",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 110000,
        'number_of_jobs' : 25,
        'skim_eff' : 1.0,
        'x_sec' : (
               1830.*_femtobarns*0.117009 # (bb -> A0)*(A0->tautau)
           +   1829.*_femtobarns*0.133089 # (bb -> H0)*(H0->tautau)
        ),
        # Feynhiggs v2.7.1 input to calculate xsec*br -  mhmax, 7TeV
        'legendEntry' : 'bbA(350) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
        'enableSysUncertainties' : True,
        'applyZrecoilCorrection' : True
    }
}
RECO_SAMPLES.update(AHtoMuTauSpecific_RECO_SAMPLES)

MERGE_SAMPLES = copy.deepcopy(ZtoMuTau.MERGE_SAMPLES)
AHtoMutauSpecific_MERGE_SAMPLES = {
    'A90Sum' : {
        'samples' : [ 'A90', 'bbA90' ],
        'legendEntry' : 'A(90) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : RECO_SAMPLES['A90']['drawOption'],
    },
    'A100Sum' : {
        'samples' : [ 'A100', 'bbA100' ],
        'legendEntry' : 'A(100) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : RECO_SAMPLES['A100']['drawOption'],
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
    'A140Sum' : {
        'samples' : [ 'A140', 'bbA140' ],
        'legendEntry' : 'A(140) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : RECO_SAMPLES['A140']['drawOption'],
    },
    'A160Sum' : {
        'samples' : [ 'A160', 'bbA160' ],
        'legendEntry' : 'A(160) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : RECO_SAMPLES['A160']['drawOption'],
    },
    'A180Sum' : {
        'samples' : [ 'A180', 'bbA180' ],
        'legendEntry' : 'A(180) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : RECO_SAMPLES['A180']['drawOption'],
    },
    'A200Sum' : {
        'samples' : [ 'A200', 'bbA200' ],
        'legendEntry' : 'A(200) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : RECO_SAMPLES['A200']['drawOption'],
    },
    'A250Sum' : {
        'samples' : [ 'A250', 'bbA250' ],
        'legendEntry' : 'A(250) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : RECO_SAMPLES['A250']['drawOption'],
    },
    'A300Sum' : {
        'samples' : [ 'A300', 'bbA300' ],
        'legendEntry' : 'A(300) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : RECO_SAMPLES['A300']['drawOption'],
    },
    'A350Sum' : {
        'samples' : [ 'A350', 'bbA350' ],
        'legendEntry' : 'A(350) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : RECO_SAMPLES['A350']['drawOption'],
    }
}
MERGE_SAMPLES.update(AHtoMutauSpecific_MERGE_SAMPLES)

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
        production_mechanism = (match.group('isBB') and 'bbH' or 'ggF')
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
        total_eff_xsec *= ZtoMuTau._picobarns
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

recoSampleDefinitionsAHtoMuTau_7TeV = {
    'SAMPLES_TO_ANALYZE' : SAMPLES_TO_ANALYZE,
    'SAMPLES_TO_PLOT' : SAMPLES_TO_PLOT,
    'SAMPLES_TO_PRINT' : SAMPLES_TO_PRINT,
    'SAMPLE_DEFAULTS' : SAMPLE_DEFAULTS,
    #'TARGET_LUMI' : ZtoMuTau.TARGET_LUMI,
    'TARGET_LUMI' : (3061964.0/ZtoMuTau._microbarns +
                     32264363.944/ZtoMuTau._microbarns),
    'RECO_SAMPLES' : RECO_SAMPLES,
    'MERGE_SAMPLES' : MERGE_SAMPLES,
    'FLATTENED_SAMPLES_TO_PLOT' : FLATTENED_SAMPLES_TO_PLOT,
    'ALL_SAMPLES' : ALL_SAMPLES
}
