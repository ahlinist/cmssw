
import FWCore.ParameterSet.Config as cms
import copy

import TauAnalysis.Configuration.plotterProcessDefinitions_cfi as plotter
import TauAnalysis.DQMTools.plotterStyleDefinitions_cfi as styles

# List of samples to run in the analysis
SAMPLES_TO_ANALYZE = [
#	'data',
#	'Wtaunu',
#	'Wenu',
#	'Wmunu',
#	'Ztautau',
	'QCD'
	]

SAMPLES_TO_PLOT = [
	'data',
	'data2',
	'Wmunu',
	'Wenu',
	'Ztautau',
#	'QCD',
	'Wtaunu'
]

SAMPLES_TO_PRINT = copy.copy(SAMPLES_TO_PLOT)
SAMPLES_TO_PRINT.append('dataSum')
SAMPLES_TO_PRINT.append('smSum')
SAMPLES_TO_PRINT.append('ewkSum')

SAMPLE_DEFAULTS = {
#    'dbs_url' : "http://cmsdbsprod.cern.ch/cms_dbs_ph_analysis_02/servlet/DBSServlet",
    'dbs_url' : "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
    'conditions' : 'START41_V0::All',
    'genPhaseSpaceCut' : '',
    'factorize' : False,
    'enableSysUncertainties' : True,
    'lumi_mask' : '',
    'runselection' : '',
    'hlt_paths' :  [ 'HLT_SingleIsoTau20_Trk15_MET25_v3','HLT_SingleIsoTau20_Trk15_MET25_v4' ],
    'SE_white_list' : '',
    'SE_black_list' : 'T2_EE',
    'disableDuplicateCheck' : True,
    'applyZrecoilCorrection' : False,
    'applyMuonTriggerEfficiencyCorrection' : False,
    'applyElectronTriggerEfficiencyCorrection' : False,
    'applyTauMetTriggerEfficiencyCorrection' : True,
    'applyVertexMultiplicityReweighting' : True,
#    'applyVertexMultiplicityReweighting' : False,
    'applyElectronIsolationEfficiencyCorrection' : False
    }

# Conversions to pico barns
_millibarns = 1.0e+9
_microbarns = 1.0e+6
_nanobarns  = 1.0e+3
_picobarns =  1.0
_femtobarns = 1.0e-3

# Integrated luminosity to normalize
#TARGET_LUMI = 17.8/_picobarns # for runs 132440 - 149442
TARGET_LUMI = 30.82

RECO_SAMPLES = {
	'data':
		{
		'datasetpath' : '/BTau/Run2010B-Apr21ReReco_v1/AOD',
		'lumi_mask' : "/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions10/7TeV//Reprocessing/Cert_136033-149442_7TeV_Apr21ReReco_Collisions10_JSON.txt",
		'runselection' : "148822 - 149294",
		'number_of_jobs' : 100,
		'conditions' : 'GR_R_42_V19::All',
		'enableSysUncertainties' : False,
		'events_processed' : -1,
		'skim_eff' : 1.0,
		'legendEntry' : 'DATA',
		'type' : 'Data',
		'drawOption' : styles.drawOption_Data,
		'hlt_paths' : [ 'HLT_SingleIsoTau20_Trk15_MET25_v3','HLT_SingleIsoTau20_Trk15_MET25_v4' ]
		#{
		#	'HLT_SingleIsoTau20_Trk15_MET20': '146428:MIN - 148058:MAX',
		#	['HLT_SingleIsoTau20_Trk15_MET25_v3','HLT_SingleIsoTau20_Trk15_MET25_v4']: '148822:MIN - 149294:MAX'
		#	}
		},
	'data2':
	{
	'datasetpath' : '/BTau/Run2010B-Apr21ReReco-v1/AOD',
	           'lumi_mask' : "/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions10/7TeV//Reprocessing/Cert_136033-149442_7TeV_Apr21ReReco_Collisions10_JSON.txt",
	           'runselection' : "146428 - 148058",
	           'number_of_jobs' : 100,
	           'conditions' : 'GR_R_42_V19::All',
	           'enableSysUncertainties' : False,
	           'events_processed' : -1,
	           'skim_eff' : 1.0,
	           'legendEntry' : 'DATA',
	           'type' : 'Data',
	           'drawOption' : styles.drawOption_Data,
	           'hlt_paths' : {
	           'HLT_SingleIsoTau20_Trk15_MET20': '146428:MIN - 148058:MAX'
		}
	},
	'Wtaunu' : {
		'datasetpath' : "/WToTauNu_TuneZ2_7TeV-pythia6-tauola/Spring11-PU_S1_START311_V1G1-v1/AODSIM",
#	        'datasetpath' : "/WToTauNu_TuneZ2_7TeV-pythia6-tauola/Winter10-E7TeV_ProbDist_2010Data_BX156_START39_V8-v1/AODSIM",
		'events_processed' : 5000000,
		'skim_eff' : 1.0,
		'x_sec' : 10438*_picobarns, # NLO
		'legendEntry' : plotter.process_Wtaunu.config_dqmHistPlotter.legendEntry.value(),
		'type' : plotter.process_Wtaunu.config_dqmHistPlotter.type.value(),
		'drawOption' : styles.drawOption_Wtaunu,
		'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X"),
	#	'hlt' : cms.InputTag("TriggerResults","","REDIGI39X"),
#		'SE_white_list' : 'T2_EE_Estonia'
		},
	'Wmunu' : {
		'datasetpath' :"/WToMuNu_TuneZ2_7TeV-pythia6/Spring11-PU_S1_START311_V1G1-v1/AODSIM",
		'events_processed' : 5000000,
		'skim_eff' : 1.0,
		'x_sec' : 10438*_picobarns, # NLO
		'legendEntry' : plotter.process_Wmunu.config_dqmHistPlotter.legendEntry.value(),
		'type' : plotter.process_Wmunu.config_dqmHistPlotter.type.value(),
		'drawOption' : styles.drawOption_Wmunu,
		'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X"),
	#	'SE_white_list' : 'T2_EE_Estonia'
		},
	'Wenu' : {
		'datasetpath' : "/WToENu_TuneZ2_7TeV-pythia6/Spring11-PU_S1_START311_V1G1-v1/AODSIM",
		'events_processed' : 1000000,
		'skim_eff' : 1.0,
		'x_sec' : 10438*_picobarns, # NLO
		'legendEntry' : plotter.process_Wenu.config_dqmHistPlotter.legendEntry.value(),
		'type' : plotter.process_Wenu.config_dqmHistPlotter.type.value(),
		'drawOption' : styles.drawOption_Wenu, 
#		'drawOption' : styles.drawOption_Wenu_separate,
		'hlt' : cms.InputTag("TriggerResults","","REDIGI311X"),
#		'SE_white_list' : 'T2_EE_Estonia'
		},
	'Ztautau' : {
		'datasetpath' : "/DYToTauTau_M-20_TuneZ2_7TeV-pythia6-tauola/Spring11-PU_S1_START311_V1G1-v1/AODSIM",
		'events_processed' : 4000000,
		'skim_eff' : 1.0,
		'x_sec' : 1.282*1300*_picobarns, # NLO
		'legendEntry' : plotter.process_Ztautau.config_dqmHistPlotter.legendEntry.value(),
		'type' : plotter.process_Ztautau.config_dqmHistPlotter.type.value(),
		'drawOption' : styles.drawOption_yellow_stacked,
		'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
		}, 
	'QCD' : {
	'datasetpath' : "/QCD_Pt-15_TauBiased_TuneZ2_7TeV-pythia6/Spring11-PU_S1_START311_V1G1-v1/AODSIM",
		'events_processed' : 15000000,
		'skim_eff' : 1.0,
		'x_sec' : 31540000*_picobarns, # NLO
		'legendEntry' : plotter.process_qcd_W.config_dqmHistPlotter.legendEntry.value(),
		'type' : plotter.process_qcd_W.config_dqmHistPlotter.type.value(),
		'drawOption' : styles.drawOption_QCD,
		'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
		}
	
	}

#Define samples that get merged together
MERGE_SAMPLES = {
	'dataSum' : {
        	'samples' : [
	                 'data', 
			 'data2'
			 ],
		'legendEntry' : 'DATA',
		'type' : 'Data',
		'drawOption' : styles.drawOption_Data
		},
	'ewkSum' : {
		'samples' : [
			'Wenu',
			'Wmunu',
			'Ztautau'
			'Wtaunu'
			],
		'legendEntry' : 'ewk-Bg',
		'type' : 'smMC',
		'drawOption' : styles.drawOption_Wtaunu
		},
	'smSum' : {
	        'samples' : [
	              'Wtaunu',
	              'Wenu',
		      'Wmunu',
		      'Ztautau'
#		      'QCD'
	        ],
		'legendEntry' : 'SM',
		'type' : 'smSumMC',
		'drawOption' : styles.drawOption_QCD
	}
}

#List of all subsamples used in any plot job.
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
        
recoSampleDefinitionsWtoTauNu_grid = {
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
