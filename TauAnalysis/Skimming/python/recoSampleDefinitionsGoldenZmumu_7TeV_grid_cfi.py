import FWCore.ParameterSet.Config as cms

import copy

import TauAnalysis.Configuration.plotterProcessDefinitions_cfi as plotter
import TauAnalysis.DQMTools.plotterStyleDefinitions_cfi as styles
from TauAnalysis.Configuration.recoSampleDefaults_cfi import SAMPLE_DEFAULTS

# List of samples to run in the analysis
SAMPLES_TO_ANALYZE = [
    #'Data_runs190456to190688'
    'Data_runs190456to191276'
    #'ZplusJets_madgraph',
    #'TTplusJets_madgraph'
]

_millibarns = 1.0e+9
_picobarns =  1.0
_femtobarns = 1.0e-3

TARGET_LUMI = 0.1/_femtobarns # CV: estimated luminosity of first 2012 data

RECO_SAMPLES = {
    'Data_runs190456to190688' : {
        'datasetpath'      : '/DoubleMu/Run2012A-PromptReco-v1/AOD',
        'dbs_url'          : "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 1387525, # 2012/04/14
        'lumi_mask'        : "/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Prompt/Cert_190456-190688_8TeV_PromptReco_Collisions12_JSON.txt",        
        'runselection'     : "190456-190688",
        'lumis_per_job'    : "25",
        'type'             : 'Data'
    },
    'Data_runs190456to191276' : {
       'datasetpath'      : '/DoubleMu/Run2012A-PromptReco-v1/AOD',
        'dbs_url'          : "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 3934826, # 2012/04/20
        'lumi_mask'        : "/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Prompt/Cert_190456-191276_8TeV_PromptReco_Collisions12_JSON.txt",
        'runselection'     : "190456-191276",
        'lumis_per_job'    : "25",
        'type'             : 'Data'
    }, 
    'ZplusJets_madgraph' : {
        'datasetpath'      : "/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12-PU_S7_START52_V5-v2/AODSIM",
        'dbs_url'          : "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 1082838,
        'x_sec'            : 3503.71*_picobarns, # Note: Madgraph samples are generated for M > 50 GeV      
        'type'             : 'smMC'
    },
    'TTplusJets_madgraph' : {
        'datasetpath'      : "/TTJets_TuneZ2star_8TeV-madgraph-tauola/Summer12-PU_S7_START52_V5-v1/AODSIM",
        'dbs_url'          : "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 1203373,
        'x_sec'            : 225.197*_picobarns,
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
