import FWCore.ParameterSet.Config as cms

import copy

import TauAnalysis.Configuration.plotterProcessDefinitions_cfi as plotter
import TauAnalysis.DQMTools.plotterStyleDefinitions_cfi as styles
from TauAnalysis.Configuration.recoSampleDefaults_cfi import SAMPLE_DEFAULTS

# List of samples to run in the analysis
SAMPLES_TO_ANALYZE = [
    #'Data_runs190456to191859'
    #'Data_runs190456to193336',
    'Data_runs190456to193557',
    'Data_runs190456to193621',
    'Data_runs193752to194076',
    #'ZplusJets_madgraph',
    'ZplusJets_madgraph2',
    'TTplusJets_madgraph2',
    'PPmuXptGt20Mu15',
    'PPmuXptGt20Mu15v2',
    'WW',
    'WZ',
    'ZZ'
]

_millibarns = 1.0e+9
_picobarns =  1.0
_femtobarns = 1.0e-3

TARGET_LUMI = 0.1/_femtobarns # CV: estimated luminosity of first 2012 data

RECO_SAMPLES = {
    'Data_runs190456to193557' : {
        'datasetpath'      : '/DoubleMu/Run2012A-PromptReco-v1/AOD',
        'dbs_url'          : "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 5533140, # 2012/05/11
        'lumi_mask'        : "/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Prompt/Cert_190456-193557_8TeV_PromptReco_Collisions12_JSON.txt",
        'runselection'     : "190456-193557",
        'lumis_per_job'    : "25",
        'type'             : 'Data'
    },
    'Data_runs190456to193621' : {
        'datasetpath'      : '/DoubleMu/Run2012A-PromptReco-v1/AOD',
        'dbs_url'          : "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 5850263, # 2012/05/21
        'lumi_mask'        : "/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Prompt/Cert_190456-194076_8TeV_PromptReco_Collisions12_JSON.txt",
        'runselection'     : "190456-193621",
        'lumis_per_job'    : "25",
        'type'             : 'Data'
    },
    'Data_runs193752to194076' : {
        'datasetpath'      : '/DoubleMu/Run2012B-PromptReco-v1/AOD',
        'dbs_url'          : "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",        
        'events_processed' : 3061986, # 2012/05/21
        'lumi_mask'        : "/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Prompt/Cert_190456-194076_8TeV_PromptReco_Collisions12_JSON.txt",
        'runselection'     : "193752-194076",
        'lumis_per_job'    : "25",
        'type'             : 'Data'
    },    
    'ZplusJets_madgraph2' : {
        'datasetpath'      : "/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12-PU_S7_START52_V9-v2/AODSIM",
        'dbs_url'          : "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 9271272,
        'x_sec'            : 3503.71*_picobarns, # Note: Madgraph samples are generated for M > 50 GeV      
        'type'             : 'smMC'
    },
    'TTplusJets_madgraph2' : {
        'datasetpath'      : "/TTJets_TuneZ2star_8TeV-madgraph-tauola/Summer12-PU_S7_START52_V9-v1/AODSIM",
        'dbs_url'          : "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 6736135,
        'x_sec'            : 225.197*_picobarns,
        'type'             : 'smMC'
    },
    'PPmuXptGt20Mu15' : {
        'datasetpath'      : "/QCD_Pt_20_MuEnrichedPt_15_TuneZ2star_8TeV_pythia6/Summer12-PU_S7_START50_V15-v1/AODSIM",
        'dbs_url'          :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 7529312,
        'x_sec'            : 0.364*_millibarns*3.7e-4, # x-sec * gen filter efficiency,
                                                       # taken from http://cms.cern.ch/iCMS/prep/requestmanagement?campid=Summer12_DR52X
        'type'             : 'smMC'
    },
    'PPmuXptGt20Mu15v2' : {
        'datasetpath'      : "/QCD_Pt_20_MuEnrichedPt_15_TuneZ2star_8TeV_pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM",
        'dbs_url'          :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 7529312,
        'x_sec'            : 0.364*_millibarns*3.7e-4, # x-sec * gen filter efficiency,
                                                       # taken from http://cms.cern.ch/iCMS/prep/requestmanagement?campid=Summer12_DR52X
        'type'             : 'smMC'
    },
    'WW' : {
        'datasetpath'      : "/WW_TuneZ2_7TeV_pythia6_tauola/Summer11-PU_S4_START42_V11-v1/AODSIM",
        'dbs_url'          :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 4225916,
        'x_sec'            : 57.11*_picobarns, # NLO cross-section @ 8 TeV,
                                               # taken from https://twiki.cern.ch/twiki/bin/viewauth/CMS/StandardModelCrossSectionsat8TeV
        'type'             : 'smMC'
    },
    'WZ' : {
        'datasetpath'      : "/WZ_TuneZ2star_8TeV_pythia6_tauola/Summer12-PU_S8_START52_V9-v1/AODSIM",
        'dbs_url'          :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 4265243,
        'x_sec'            : 32.32*_picobarns, # NLO cross-section @ 8 TeV,
                                               # taken from https://twiki.cern.ch/twiki/bin/viewauth/CMS/StandardModelCrossSectionsat8TeV
        'type'             : 'smMC'
    },
    'ZZ' : {
        'datasetpath'      : "/ZZ_TuneZ2star_8TeV_pythia6_tauola/Summer12-PU_S8_START52_V9-v1/AODSIM",
        'dbs_url'          :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 1890152,
        'x_sec'            : 8.26*_picobarns, # NLO cross-section @ 8 TeV,
                                              # taken from https://twiki.cern.ch/twiki/bin/viewauth/CMS/StandardModelCrossSectionsat8TeV
        'type'             : 'smMC'
    }
}

# Update to use the defaults if necessary
for sample in RECO_SAMPLES.keys():
    defaults = copy.copy(SAMPLE_DEFAULTS)
    defaults.update(RECO_SAMPLES[sample])
    RECO_SAMPLES[sample] = defaults
    
recoSampleDefinitionsGoldenZmumu_7TeV = {
    'SAMPLES_TO_ANALYZE' : SAMPLES_TO_ANALYZE,
    'TARGET_LUMI'        : TARGET_LUMI,
    'RECO_SAMPLES'       : RECO_SAMPLES
}
