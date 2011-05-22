#/usr/bin/env python

scheduler = 'glite'
#scheduler = 'condor'

samples = {
    "Dimuon_AOD_data": """
        /Mu/Run2010A-Apr21ReReco-v1/AOD
        /Mu/Run2010B-Apr21ReReco-v1/AOD
        /SingleMu/Run2011A-May10ReReco-v1/AOD
        /DoubleMu/Run2011A-May10ReReco-v1/AOD
        """.split(),
    "Dimuon_RECO_data": """
        /Mu/Run2010A-Apr21ReReco-v1/RECO
        /Mu/Run2010B-Apr21ReReco-v1/RECO
        /Mu/Run2010A-ZMu-Apr21ReReco-v1/RAW-RECO
        /Mu/Run2010B-ZMu-Apr21ReReco-v1/RAW-RECO
        /SingleMu/Run2011A-ZMu-May10ReReco-v1/RAW-RECO
        """.split(),
    "Inclusive_AOD": """
        /VBF_HToGG_M-120_7TeV-powheg-pythia6/Summer11-PU_S3_START42_V11-v2/AODSIM
        /GluGluToHToGG_M-120_7TeV-powheg-pythia6/Summer11-PU_S3_START42_V11-v2/AODSIM
        /G_Pt-15to3000_TuneZ2_Flat_7TeV_pythia6/Summer11-PU_S3_START42_V11-v2/AODSIM
        """.split(),
    "Inclusive_RECO": """
        /RelValSingleGammaPt10/CMSSW_4_2_2-MC_42_V11-v1/GEN-SIM-RECO
        /RelValSingleGammaPt35/CMSSW_4_2_2-MC_42_V11-v1/GEN-SIM-RECO
        """.split(),
    "Dimuon_AOD": """
        /DYToMuMu_M-20_TuneZ2_7TeV-pythia6/Summer11-PU_S3_START42_V11-v2/AODSIM
        /DYToMuMu_M-20_TuneZ2_7TeV-pythia6/Summer11-PU_S3_START42_V11-v1/AODSIM
        /DYToMuMu_M-20_Tune4C_7TeV-pythia8/Summer11-PU_S3_START42_V11-v2/AODSIM
        /WToMuNu_TuneZ2_7TeV-pythia6/Summer11-PU_S3_START42_V11-v1/AODSIM
        /WToMuNu_TuneZ2_7TeV-pythia6/Summer11-PU_S3_START42_V11-v2/AODSIM
        """.split(),
    "Dimuon_RECO": """
        /DYToMuMu_M-20_TuneZ2_7TeV-pythia6/Summer11-PU_S3_START42_V11-v2/GEN-SIM-RECO
        /DYToMuMu_M-20_TuneZ2_7TeV-pythia6/Summer11-PU_S3_START42_V11-v1/GEN-SIM-RECO
        /DYToMuMu_M-20_Tune4C_7TeV-pythia8/Summer11-PU_S3_START42_V11-v2/GEN-SIM-RECO
        /WToMuNu_TuneZ2_7TeV-pythia6/Summer11-PU_S3_START42_V11-v1/GEN-SIM-RECO
        /WToMuNu_TuneZ2_7TeV-pythia6/Summer11-PU_S3_START42_V11-v2/GEN-SIM-RECO
        """.split(),
    }

for label, datasets in samples.items():
    for dataset in datasets:
        primaryName, processedName, tier = dataset.split("/")[1:]
        print """
###############################################################################
[%s_%s]
CMSSW.datasetpath = %s
CMSSW.pycfg_params = options_load=options_%s.cfg
USER.additional_input_files = options_%s.cfg
USER.publish_data_name = %s_%s_%s-42X-v4""" % (
            primaryName, label.replace("_data", ""),
            dataset,
            label,
            label,
            processedName, scheduler, label.replace("_data", ""),
            )
        if label[-4:] == "data":
            print """CMSSW.total_number_of_lumis = -1
CMSSW.lumis_per_job = 100"""
        else:
            print """CMSSW.total_number_of_events = -1
CMSSW.number_of_jobs = 100"""
