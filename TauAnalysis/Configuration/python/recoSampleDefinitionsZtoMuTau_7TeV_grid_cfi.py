import FWCore.ParameterSet.Config as cms
import copy

import TauAnalysis.Configuration.plotterProcessDefinitions_cfi as plotter
import TauAnalysis.DQMTools.plotterStyleDefinitions_cfi as styles
import TauAnalysis.Configuration.recoSampleDefaults_cfi as config

# List of samples to run in the analysis
SAMPLES_TO_ANALYZE = [
    'ZplusJets_madgraph2',
    'Ztautau_pythia',
    'WplusJets_madgraph',
    'PPmuXptGt20Mu15',      
    'WW',
    'WZ',
    'ZZ',
    'TTplusJets_madgraph2'
]

# List of samples to include in the final level plots.  May include selections
# from the MERGE_SAMPLES defined at the bottom.
SAMPLES_TO_PLOT = [
    'data',
    #'VVsum',
    'TTplusJets_madgraph',
    'ZmumuSum',
    'WplusJets_madgraph',
    'qcdSum',
    'ZtautauSum'
]

SAMPLES_TO_PRINT = copy.copy(SAMPLES_TO_PLOT)
SAMPLES_TO_PRINT.append('smBgSum')
SAMPLES_TO_PRINT.append('smSum')

SAMPLE_DEFAULTS = copy.copy(config.SAMPLE_DEFAULTS)
SAMPLE_DEFAULTS.update({
    'genPhaseSpaceCut' : '',
    'genFinalStateFilter': False,
    'factorize' : False,
    'applyZrecoilCorrection' : False,
    'applyElectronTriggerEfficiencyCorrection' : False, # not be used in mu + tau-jet channel
    'applyElectronIsolationEfficiencyCorrection' : False, # not be used in mu + tau-jet channel
    'applyTauMetTriggerEfficiencyCorrection' : False, # not be used in mu + tau-jet channel
    'applyMuonTriggerEfficiencyCorrection' : False,   # to be used for MC only
    'applyMuonIsolationEfficiencyCorrection' : False, # to be used for MC only
    'applyVertexMultiplicityReweighting' : False,     # to be used for MC with pile-up only
    'applyRhoNeutralReweighting' : False,     # to be used for MC with pile-up only
    'enableSysUncertainties' : True,
    'lumi_mask' : '',
    'runselection' : '',
    'hlt_paths' : [ 'HLT_Mu9' ],
    'noRunLumiEventSave' : True,
    'SE_white_list' : '',
    'SE_black_list' : '',
    'disableDuplicateCheck' : False,
    'enableFakeRates' : False,
    'disableDuplicateCheck' : False,
    'inputFileType' : 'AOD'
})

# Conversions to pico barns
_millibarns = 1.0e+9
_microbarns = 1.0e+6
_nanobarns  = 1.0e+3
_picobarns =  1.0
_femtobarns = 1.0e-3

# Integrated luminosity to normalize
TARGET_LUMI = 0.90*869.1/_picobarns # for runs 160404 - 167496 ("golden" quality) corrected for ~90% PFTau trigger efficiency

#--------------------------------------------------------------------------------
# NOTE: cross-sections for W and Z production are scaled to next-to-leading order values
#       documented on the wiki
#        http://alcaraz.web.cern.ch/alcaraz/CROSS_SECTIONS.txt
#--------------------------------------------------------------------------------

RECO_SAMPLES = {
    'Ztautau_pythia' : {
        'datasetpath' : "/DYToTauTau_M_20_TuneZ2star_8TeV_pythia6_tauola/Summer12-PU_S8_START52_V9-v1/AODSIM",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 1987776,
        'skim_eff' : 1.0,
        'x_sec' : 1666*_picobarns,
        'legendEntry' : plotter.process_Ztautau.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Ztautau.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Ztautau,
        'applyZrecoilCorrection' : True,
        'enableFakeRates' : True,
        'applyMuonTriggerEfficiencyCorrection' : True,
        'applyMuonIsolationEfficiencyCorrection' : True,
	'applyVertexMultiplicityReweighting' : True,
        'applyRhoNeutralReweighting' : True,
        'hlt' : cms.InputTag("TriggerResults", "", "HLT")
    },
    'Zmumu_pythia' : {
        'datasetpath' : "/DYToMuMu_M_20_TuneZ2star_8TeV_pythia6/Summer12-PU_S7_START50_V15-v1/AODSIM",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 1963296,
        'skim_eff' : 1.0,
        'x_sec' : 1666*_picobarns,
        'legendEntry' : plotter.process_Zmumu.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Zmumu.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Zmumu,
        'applyZrecoilCorrection' : True,
        'applyMuonTriggerEfficiencyCorrection' : True,
        'applyMuonIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : True,
        'applyRhoNeutralReweighting' : True,
        'enableFakeRates' : True,
        'hlt' : cms.InputTag("TriggerResults", "", "HLT")
    },
    'DYmumuM10to20_pythia' : {
        'datasetpath' : "/DYToMuMu_M_10To20_TuneZ2star_8TeV_pythia6/Summer12-PU_S7_START50_V15-v1/AODSIM",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 1762560,
        'skim_eff' : 1.0,
        'x_sec' : 2659.*1.15*(1666/1300)*_picobarns, # LO cross-section taken from
                                                     #  https://twiki.cern.ch/twiki/bin/view/CMS/ProductionReProcessingSpring10
                                                     # and scaled by ratio of Drell-Yan cross-sections @ 8/7 TeV and NNLO/LO k-factor
                                                     # computed for M > 20 GeV
        'legendEntry' : plotter.process_Zmumu.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Zmumu.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Zmumu,
        'applyZrecoilCorrection' : True,
        'applyMuonTriggerEfficiencyCorrection' : True,
        'applyMuonIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : True,
        'applyRhoNeutralReweighting' : True,
        'enableFakeRates' : True,
        'hlt' : cms.InputTag("TriggerResults", "", "HLT")
    },    
    'ZplusJets_madgraph2' : {
        'datasetpath' : "/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12-PU_S7_START52_V9-v2/AODSIM",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 9271272,
        'skim_eff' : 1.0,
        'x_sec' : 3503.71*_picobarns, # Note: Madgraph samples are generated for M > 50 GeV
        'legendEntry' : plotter.process_Zmumu.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Zmumu.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Zmumu,
        'applyZrecoilCorrection' : True,
        'applyMuonTriggerEfficiencyCorrection' : True,
        'applyMuonIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : True,
        'applyRhoNeutralReweighting' : True,
        'enableFakeRates' : True,
        'hlt' : cms.InputTag("TriggerResults", "", "HLT")
    },
    'WplusJets_madgraph' : {
        'datasetpath' : "/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12-PU_S7_START52_V9-v1/AODSIM",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 18393090,
        'number_of_jobs' : 2500,
        'skim_eff' : 1.0,
        'x_sec' : 36257.2*_picobarns, # NLO cross-section @ 8 TeV,
                                      # taken from https://twiki.cern.ch/twiki/bin/viewauth/CMS/StandardModelCrossSectionsat8TeV
        'legendEntry' : plotter.process_WplusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_WplusJets.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_WplusJets,
        'applyZrecoilCorrection' : True,
        'applyMuonTriggerEfficiencyCorrection' : True,
        'applyMuonIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : True,
        'applyRhoNeutralReweighting' : False,
        'enableFakeRates' : True,
        'hlt' : cms.InputTag("TriggerResults", "", "HLT")
    },
    'PPmuXptGt20Mu15' : {
        'datasetpath' : "/QCD_Pt_20_MuEnrichedPt_15_TuneZ2star_8TeV_pythia6/Summer12-PU_S7_START50_V15-v1/AODSIM",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 7529312,
        'number_of_jobs' : 2500,
        'skim_eff' : 1.0,
        'x_sec' : 0.364*_millibarns*3.7e-4, # x-sec * gen filter efficiency,
                                            # taken from http://cms.cern.ch/iCMS/prep/requestmanagement?campid=Summer12_DR52X
        'legendEntry' : plotter.process_PPmuXptGt20.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_PPmuXptGt20.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_QCD,
        'applyMuonTriggerEfficiencyCorrection' : True,
        'applyMuonIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : True,
        'applyRhoNeutralReweighting' : True,
        'enableFakeRates' : True,
        'factorize' : True,
        'hlt' : cms.InputTag("TriggerResults", "", "HLT")
    },
    'PPmuXptGt20Mu15v2' : {
        'datasetpath' : "/QCD_Pt_20_MuEnrichedPt_15_TuneZ2star_8TeV_pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 7529312,
        'number_of_jobs' : 2500,
        'skim_eff' : 1.0,
        'x_sec' : 0.364*_millibarns*3.7e-4, # x-sec * gen filter efficiency,
                                            # taken from http://cms.cern.ch/iCMS/prep/requestmanagement?campid=Summer12_DR52X
        'legendEntry' : plotter.process_PPmuXptGt20.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_PPmuXptGt20.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_QCD,
        'applyMuonTriggerEfficiencyCorrection' : True,
        'applyMuonIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : True,
        'applyRhoNeutralReweighting' : True,
        'enableFakeRates' : True,
        'factorize' : True,
        'hlt' : cms.InputTag("TriggerResults", "", "HLT")
    },
    'WW' : {
        'datasetpath' : "/WW_TuneZ2_7TeV_pythia6_tauola/Summer11-PU_S4_START42_V11-v1/AODSIM",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 4225916,
        'skim_eff' : 1.0,
        'x_sec' : 57.11*_picobarns, # NLO cross-section @ 8 TeV,
                                    # taken from https://twiki.cern.ch/twiki/bin/viewauth/CMS/StandardModelCrossSectionsat8TeV
        'legendEntry' : plotter.process_VV.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_VV.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_VV,
        'applyZrecoilCorrection' : False,
        'applyMuonTriggerEfficiencyCorrection' : True,
        'applyMuonIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : True,
        'applyRhoNeutralReweighting' : False,
        'hlt' : cms.InputTag("TriggerResults", "", "HLT")
    },
    'WZ' : {
        'datasetpath' : "/WZ_TuneZ2star_8TeV_pythia6_tauola/Summer12-PU_S8_START52_V9-v1/AODSIM",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 4265243,
        'skim_eff' : 1.0,
        'x_sec' : 32.32*_picobarns, # NLO cross-section @ 8 TeV,
                                    # taken from https://twiki.cern.ch/twiki/bin/viewauth/CMS/StandardModelCrossSectionsat8TeV
        'legendEntry' : plotter.process_VV.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_VV.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_VV,
        'applyZrecoilCorrection' : False,
        'applyMuonTriggerEfficiencyCorrection' : True,
        'applyMuonIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : True,
        'applyRhoNeutralReweighting' : False,
        'hlt' : cms.InputTag("TriggerResults", "", "HLT")
    },
    'ZZ' : {
        'datasetpath' : "/ZZ_TuneZ2star_8TeV_pythia6_tauola/Summer12-PU_S8_START52_V9-v1/AODSIM",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 1890152,
        'skim_eff' : 1.0,
        'x_sec' : 8.26*_picobarns, # NLO cross-section @ 8 TeV,
                                   # taken from https://twiki.cern.ch/twiki/bin/viewauth/CMS/StandardModelCrossSectionsat8TeV
        'legendEntry' : plotter.process_VV.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_VV.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_VV,
        'applyZrecoilCorrection' : False,
        'applyMuonTriggerEfficiencyCorrection' : True,
        'applyMuonIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : True,
        'applyRhoNeutralReweighting' : False,
        'hlt' : cms.InputTag("TriggerResults", "", "HLT")
    },
    'TTplusJets_madgraph2' : {
        'datasetpath' : "/TTJets_TuneZ2star_8TeV-madgraph-tauola/Summer12-PU_S7_START52_V9-v1/AODSIM",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 6736135,
        'skim_eff' : 1.0,
        'x_sec' : 225.197*_picobarns, # NLO cross-section @ 8 TeV,
                                      # taken from https://twiki.cern.ch/twiki/bin/viewauth/CMS/StandardModelCrossSectionsat8TeV
        'legendEntry' : plotter.process_TTplusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_TTplusJets.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_TTplusJets,
        'applyMuonTriggerEfficiencyCorrection' : True,
        'applyMuonIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : True,
        'applyRhoNeutralReweighting' : False,
        'enableFakeRates' : True,
        'hlt' : cms.InputTag("TriggerResults", "", "HLT")
    }
}

# Define samples that get merged together
MERGE_SAMPLES = {
    'ZtautauSum' : {
        'samples' : [
            'Ztautau_pythia'
        ],
        'legendEntry' : plotter.process_Ztautau.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Ztautau.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Ztautau
    },
    'ZmumuSum' : {
        'samples' : [
            'Zmumu_pythia',
            'DYmumuM10to20_pythia'
        ],
        'legendEntry' : plotter.process_Zmumu.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Zmumu.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Zmumu
    },
    'ZplusJetsSum' : {
        'samples' : [
            'ZplusJets_madgraph2'
        ],
        'legendEntry' : plotter.process_ZplusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_ZplusJets.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_ZplusJets
    },
    'WplusJetsSum' : {
        'samples' : [
            'WplusJets_madgraph'
        ],
        'legendEntry' : plotter.process_WplusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_WplusJets.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_WplusJets
    },
    'qcdSum' : {
        'samples' : [
            'PPmuXptGt20Mu15'
        ],
        'legendEntry' : 'QCD',
        'type' : plotter.process_PPmuXptGt20.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_QCD
    },
    'TTplusJetsSum' : {
        'samples' : [
            'TTplusJets_madgraph2'
        ],
        'legendEntry' : plotter.process_TTplusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_TTplusJets.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_TTplusJets
    },
    'VVsum' : {
        'samples' : [
            'WW',
            'WZ',
            'ZZ'
        ],
        'legendEntry' : 'WW/WZ/ZZ',
        'type' : 'smMC',
        'drawOption' : styles.drawOption_VV
    },
    'smBgSum' : {
        'samples' : [
            'ZmumuSum',
            'qcdSum',
            'WplusJetsSum',
            'VVsum',
            'TTplusJetsSum'
        ],
        'legendEntry' : 'SM',
        'type' : 'smMC',
        'drawOption' : styles.drawOption_QCD
    },
    'smSum' : {
        'samples' : [
            'ZtautauSum',
            'smBgSum'
        ],
        'legendEntry' : 'SM',
        'type' : 'smSumMC',
        'drawOption' : styles.drawOption_QCD
    }
}
MERGE_SAMPLES.update({
    'A130Sum' : {
        'samples' : [ 'A130', 'bbA130' ],
        'legendEntry' : 'A(130) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'A200Sum' : {
        'samples' : [ 'A200', 'bbA200' ],
        'legendEntry' : 'A(200) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'A300Sum' : {
        'samples' : [ 'A300', 'bbA300' ],
        'legendEntry' : 'A(300) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    },
    'A450Sum' : {
        'samples' : [ 'A450', 'bbA450' ],
        'legendEntry' : 'A(450) #rightarrow #tau^{+} #tau^{-}',
        'type' : 'bsmMC',
        'drawOption' : styles.drawOption_darkBlue_separate,
    }
})

# List of all subsamples used in any plot job.
# i.e. if qcdSum is included in samples to plot
#      it will be expanded to the inclusive/exclusive ppMux samples
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

recoSampleDefinitionsZtoMuTau_7TeV = {
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

