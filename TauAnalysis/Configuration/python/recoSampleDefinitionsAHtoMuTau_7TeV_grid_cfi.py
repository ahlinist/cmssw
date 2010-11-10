import FWCore.ParameterSet.Config as cms
import copy

import TauAnalysis.Configuration.plotterProcessDefinitions_cfi as plotter
import TauAnalysis.DQMTools.plotterStyleDefinitions_cfi as styles

# List of samples to run in the analysis
SAMPLES_TO_ANALYZE = [
    'data_Mu_132440-145761_Sep17ReReco',
    'data_Mu_145762_147116_Prompt',
    'data_Mu_147117_149442_Prompt',
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
    'A130Sum',
    'A160Sum',
    'A200Sum',
    'A300Sum',
    'qcdSum',
    #'PPmuXptGt20Mu15',
    'WplusJets',
    'TTplusJets',
    'Zmumu',
    'Ztautau'
]

SAMPLES_TO_PRINT = copy.copy(SAMPLES_TO_PLOT)
SAMPLES_TO_PRINT.append('A90Sum')
SAMPLES_TO_PRINT.append('A100Sum')
SAMPLES_TO_PRINT.append('A120Sum')
SAMPLES_TO_PRINT.append('A140Sum')
SAMPLES_TO_PRINT.append('A180Sum')
SAMPLES_TO_PRINT.append('A250Sum')
SAMPLES_TO_PRINT.append('A350Sum')
#SAMPLES_TO_PRINT.append('smBgSum')

SAMPLE_DEFAULTS = {
    'dbs_url' : "http://cmsdbsprod.cern.ch/cms_dbs_ph_analysis_02/servlet/DBSServlet",
    'conditions' : 'startup',
    'genPhaseSpaceCut' : '',
    'factorize' : False,
    'applyZrecoilCorrection' : False,
    'enableSysUncertainties' : False,
    'lumi_mask' : '',
    'runselection' : '',
    'hlt_paths' : [ 'HLT_Mu9' ],
    'SE_white_list' : '',
    'SE_black_list' : ''
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
TARGET_LUMI = (34.85)/_picobarns

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
        'lumi_mask' : "/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions10/7TeV/StreamExpress/Cert_132440-149442_7TeV_StreamExpress_Collisions10_JSON.txt",
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
        'lumi_mask' : "/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions10/7TeV/StreamExpress/Cert_132440-149442_7TeV_StreamExpress_Collisions10_JSON.txt",
        'runselection' : '145762 - 147116',
        'conditions' : 'GR_R_38X_V13::All',
        'events_processed' : -1,
        'skim_eff' : 1.0,
        'type' : 'Data',
        'drawOption' : styles.drawOption_Data,
    },
    'data_Mu_147117_149442_Prompt' : {
        'datasetpath' : '/Mu/Run2010B-PromptReco-v2/RECO',
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'lumi_mask' : "/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions10/7TeV/StreamExpress/Cert_132440-149442_7TeV_StreamExpress_Collisions10_JSON.txt",
        'runselection' : '147117 - 149442',
        'conditions' : 'GR_R_38X_V13::All',
        'events_processed' : -1,
        'skim_eff' : 1.0,
        'type' : 'Data',
        'drawOption' : styles.drawOption_Data,
        'hlt_paths' : [ 'HLT_Mu11', 'HLT_Mu15', 'HLT_IsoMu9' ],
    },
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
        'drawOption' : styles.drawOption_darkBlue_separate,
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
        'drawOption' : styles.drawOption_darkBlue_separate,
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
        'applyZrecoilCorrection' : True,
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
            'data_Mu_147117_149442_Prompt'
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
