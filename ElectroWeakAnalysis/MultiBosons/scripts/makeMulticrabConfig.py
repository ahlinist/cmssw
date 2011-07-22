#/usr/bin/env python

scheduler = 'glite'
#scheduler = 'condor'

samples = {
    "Dimuon": """
        /DoubleMu/Run2011A-ZMu-05Jul2011ReReco-ECAL-v1/RAW-RECO
        /DoubleMu/Run2011A-05Jul2011ReReco-ECAL-v1/RECO
        /DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/GEN-SIM-RECO   
        /DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM         
        /DYToMuMu_M-20_CT10_TuneZ2_7TeV-powheg-pythia/Summer11-PU_S4_START42_V11-v1/AODSIM        
        /DYToMuMu_M-20_TuneZ2_7TeV-pythia6/Summer11-HCal_PU_S4_START42_V11-v1/GEN-SIM-RECO
        /DYToMuMu_M-20_TuneZ2_7TeV-pythia6/Summer11-HCal_PU_S4_START42_V11-v1/AODSIM      
        /TTJets_TuneZ2_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v2/GEN-SIM-RECO            
        /TTJets_TuneZ2_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v2/AODSIM                  
        /TT_TuneZ2_7TeV-pythia6-tauola/Summer11-HCal_PU_S4_START42_V11-v1/GEN-SIM-RECO    
        /TT_TuneZ2_7TeV-pythia6-tauola/Summer11-HCal_PU_S4_START42_V11-v1/AODSIM          
        /WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/GEN-SIM-RECO        
        /WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM              
        /QCD_Pt-20_MuEnrichedPt-15_TuneZ2_7TeV-pythia6/Summer11-PU_S4_START42_V11-v1/GEN-SIM-RECO 
        /QCD_Pt-20_MuEnrichedPt-15_TuneZ2_7TeV-pythia6/Summer11-PU_S4_START42_V11-v1/AODSIM       
        /DYToMuMu_M-20_FSRFilter_Pt-50_TuneZ2_7TeV-pythia6/Summer11-X0Min_PU_S4_START42_V11-v1/AODSIM
        /DYToMuMu_M-20_FSRFilter_Pt-50_TuneZ2_7TeV-pythia6/Summer11-X0Max_PU_S4_START42_V11-v1/AODSIM
        /DYToMuMu_M-20_FSRFilter_Pt-50_TuneZ2_7TeV-pythia6/Summer11-PU_S4_START42_V11-v2/AODSIM
        /DYToMuMu_M-20_FSRFilter_Pt-30To50_TuneZ2_7TeV-pythia6/Summer11-X0Min_PU_S4_START42_V11-v1/AODSIM
        /DYToMuMu_M-20_FSRFilter_Pt-30To50_TuneZ2_7TeV-pythia6/Summer11-X0Max_PU_S4_START42_V11-v1/AODSIM
        /DYToMuMu_M-20_FSRFilter_Pt-30To50_TuneZ2_7TeV-pythia6/Summer11-PU_S4_START42_V11-v2/AODSIM
        /DYToMuMu_M-20_FSRFilter_Pt-20To30_TuneZ2_7TeV-pythia6/Summer11-X0Min_PU_S4_START42_V11-v1/AODSIM
        /DYToMuMu_M-20_FSRFilter_Pt-20To30_TuneZ2_7TeV-pythia6/Summer11-X0Max_PU_S4_START42_V11-v1/AODSIM
        /DYToMuMu_M-20_FSRFilter_Pt-20To30_TuneZ2_7TeV-pythia6/Summer11-PU_S4_START42_V11-v2/AODSIM
        /DYToMuMu_M-20_FSRFilter_Pt-10To20_TuneZ2_7TeV-pythia6/Summer11-X0Min_PU_S4_START42_V11-v1/AODSIM
        /DYToMuMu_M-20_FSRFilter_Pt-10To20_TuneZ2_7TeV-pythia6/Summer11-X0Max_PU_S4_START42_V11-v1/AODSIM
        /DYToMuMu_M-20_FSRFilter_Pt-10To20_TuneZ2_7TeV-pythia6/Summer11-PU_S4_START42_V11-v2/AODSIM
        /DYToMuMu_M-20_FSRFilter_Pt-50_TuneZ2_7TeV-pythia6/Summer11-X0Min_PU_S4_START42_V11-v1/GEN-SIM-RECO
        /DYToMuMu_M-20_FSRFilter_Pt-50_TuneZ2_7TeV-pythia6/Summer11-X0Max_PU_S4_START42_V11-v1/GEN-SIM-RECO
        /DYToMuMu_M-20_FSRFilter_Pt-50_TuneZ2_7TeV-pythia6/Summer11-PU_S4_START42_V11-v2/GEN-SIM-RECO
        /DYToMuMu_M-20_FSRFilter_Pt-30To50_TuneZ2_7TeV-pythia6/Summer11-X0Min_PU_S4_START42_V11-v1/GEN-SIM-RECO
        /DYToMuMu_M-20_FSRFilter_Pt-30To50_TuneZ2_7TeV-pythia6/Summer11-X0Max_PU_S4_START42_V11-v1/GEN-SIM-RECO
        /DYToMuMu_M-20_FSRFilter_Pt-30To50_TuneZ2_7TeV-pythia6/Summer11-PU_S4_START42_V11-v2/GEN-SIM-RECO
        /DYToMuMu_M-20_FSRFilter_Pt-20To30_TuneZ2_7TeV-pythia6/Summer11-X0Min_PU_S4_START42_V11-v1/GEN-SIM-RECO
        /DYToMuMu_M-20_FSRFilter_Pt-20To30_TuneZ2_7TeV-pythia6/Summer11-X0Max_PU_S4_START42_V11-v1/GEN-SIM-RECO
        /DYToMuMu_M-20_FSRFilter_Pt-20To30_TuneZ2_7TeV-pythia6/Summer11-PU_S4_START42_V11-v2/GEN-SIM-RECO
        /DYToMuMu_M-20_FSRFilter_Pt-10To20_TuneZ2_7TeV-pythia6/Summer11-X0Min_PU_S4_START42_V11-v1/GEN-SIM-RECO
        /DYToMuMu_M-20_FSRFilter_Pt-10To20_TuneZ2_7TeV-pythia6/Summer11-X0Max_PU_S4_START42_V11-v1/GEN-SIM-RECO
        /DYToMuMu_M-20_FSRFilter_Pt-10To20_TuneZ2_7TeV-pythia6/Summer11-PU_S4_START42_V11-v2/GEN-SIM-RECO
        """.split(),
    "Diphoton": """
        /Photon/Run2011A-DiPhoton-05Jul2011ReReco-ECAL-v1/RAW-RECO
        /Photon/Run2011A-ZElectron-05Jul2011ReReco-ECAL-v1/RAW-RECO
        /DoubleElectron/Run2011A-ZElectron-05Jul2011ReReco-ECAL-v1/RAW-RECO
        /DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM
        /TTJets_TuneZ2_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v2/GEN-SIM-RECO    
        /TTJets_TuneZ2_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v2/AODSIM          
        /WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/GEN-SIM-RECO
        /WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM      
        /DYToEE_M-20_CT10_TuneZ2_7TeV-powheg-pythia/Summer11-PU_S4_START42_V11-v1/AODSIM  
        /DYToEE_M-20_CT10_TuneZ2_7TeV-powheg-pythia/Summer11-PU_S4_START42_V11-v1/GEN-SIM-RECO
        """.split(),
    "Inclusive": """
        /G_Pt-80to120_TuneZ2_7TeV_pythia6/Summer11-PU_S4_START42_V11-v1/AODSIM
        /G_Pt-170to300_TuneZ2_7TeV_pythia6/Summer11-PU_S4_START42_V11-v1/AODSIM
        /G_Pt-15to3000_TuneZ2_Flat_7TeV_pythia6/Summer11-HCal_PU_S4_START42_V11-v1/AODSIM
        /GluGluToHToGG_M-90_7TeV-powheg-pythia6/Summer11-PU_S4_START42_V11-v1/AODSIM
        /GluGluToHToGG_M-95_7TeV-powheg-pythia6/Summer11-PU_S4_START42_V11-v1/AODSIM
        /GluGluToHToGG_M-100_7TeV-powheg-pythia6/Summer11-PU_S4_START42_V11-v1/AODSIM
        /GluGluToHToGG_M-105_7TeV-powheg-pythia6/Summer11-PU_S4_START42_V11-v1/AODSIM
        /GluGluToHToGG_M-110_7TeV-powheg-pythia6/Summer11-PU_S4_START42_V11-v1/AODSIM
        /GluGluToHToGG_M-115_7TeV-powheg-pythia6/Summer11-PU_S4_START42_V11-v1/AODSIM
        /GluGluToHToGG_M-120_7TeV-powheg-pythia6/Summer11-PU_S4_START42_V11-v2/AODSIM
        /GluGluToHToGG_M-121_7TeV-powheg-pythia6/Summer11-PU_S4_START42_V11-v1/AODSIM
        /GluGluToHToGG_M-123_7TeV-powheg-pythia6/Summer11-PU_S4_START42_V11-v1/AODSIM
        /GluGluToHToGG_M-125_7TeV-powheg-pythia6/Summer11-PU_S4_START42_V11-v1/AODSIM
        /GluGluToHToGG_M-130_7TeV-powheg-pythia6/Summer11-PU_S4_START42_V11-v1/AODSIM
        /GluGluToHToGG_M-135_7TeV-powheg-pythia6/Summer11-PU_S4_START42_V11-v1/AODSIM
        /GluGluToHToGG_M-140_7TeV-powheg-pythia6/Summer11-PU_S4_START42_V11-v1/AODSIM
        /DiPhotonBox_Pt-10To25_7TeV-pythia6/Summer11-PU_S4_START42_V11-v2/AODSIM
        /DiPhotonBox_Pt-25To250_7TeV-pythia6/Summer11-PU_S4_START42_V11-v2/AODSIM
        /DiPhotonBox_Pt-250_7TeV-pythia6/Summer11-PU_S4_START42_V11-v2/AODSIM
        /DiPhotonBorn_Pt-10To25_7TeV-pythia6/Summer11-PU_S4_START42_V11-v2/AODSIM
        /DiPhotonBorn_Pt-25To250_7TeV-pythia6/Summer11-PU_S4_START42_V11-v2/AODSIM
        /DiPhotonBorn_Pt-250_7TeV-pythia6/Summer11-PU_S4_START42_V11-v2/AODSIM
        /GJet_Pt-20_doubleEMEnriched_TuneZ2_7TeV-pythia6/Summer11-PU_S4_START42_V11-v1/AODSIM
        /GJet_PT_30toInf_TuneZ2_cJet_7TeV-pythia6/Summer11-PU_S4_START42_V11-v1/AODSIM
        /GJet_PT_30toInf_TuneZ2_bJet_7TeV-pythia6/Summer11-PU_S4_START42_V11-v1/AODSIM
        /GJet_PT_15toInf_TuneZ2_cJet_7TeV-pythia6/Summer11-PU_S4_START42_V11-v1/AODSIM
        /GJet_PT_15toInf_TuneZ2_bJet_7TeV-pythia6/Summer11-PU_S4_START42_V11-v1/AODSIM
        """.split(),
    }

tierAndSourceMap = {
    'AOD'         : 'AOD_data',
    'RECO'        : 'RECO_data',
    'RAW-RECO'    : 'RECO_data',
    'AODSIM'      : 'AOD',
    'GEN-SIM-RECO': 'RECO',
}

class SampleData:
    pass

sampleData = {}
order = []

for label, datasets in samples.items():
    for dataset in datasets:
        data = SampleData()
        data.label   = label
        data.dataset = dataset
        data.primaryName, \
            data.processedName, \
            data.tier =   dataset.split("/")[1:]
        data.label += "_" + tierAndSourceMap[data.tier]
        data.shortLabel = data.label.replace('_data', '')
        data.isRealData = data.label[-4:] == "data"

        name = '%s_%s_%s' % ( data.shortLabel,
                              data.primaryName,
                              data.processedName, )
        if name in sampleData.keys():
            print "WARNING! %s and %s share the same name `%s'" % (
                sampleData[name].dataset,
                dataset,
                name,
            )
        else:
            sampleData[name] = data
        order.append(name)

for name in order:
    data = sampleData[name]
    print """
###############################################################################
[{NAME}]
CMSSW.datasetpath = {DATASET}
CMSSW.pycfg_params = options_load=options_{LABEL}.cfg
USER.additional_input_files = options_{LABEL}.cfg
USER.publish_data_name = {PROCESSED_NAME}_{SCHEDULER}_{SHORT_LABEL}-42X-v9"""\
    .format( NAME = name,
             DATASET = data.dataset,
             LABEL = data.label,
             PROCESSED_NAME = data.processedName,
             SCHEDULER = scheduler,
             SHORT_LABEL = data.shortLabel )
    if data.isRealData:
        print """CMSSW.total_number_of_lumis = -1
CMSSW.lumis_per_job = 100"""
    else:
        print """CMSSW.total_number_of_events = -1
CMSSW.number_of_jobs = 100"""
