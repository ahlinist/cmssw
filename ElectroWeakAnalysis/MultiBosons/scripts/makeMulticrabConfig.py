#/usr/bin/env python

#scheduler = 'glite'
scheduler = 'condor'

samples = {
    "Dimuon": """
        /DoubleMu/Run2011A-ZMu-05Jul2011ReReco-ECAL-v1/RAW-RECO
        /DoubleMu/Run2011A-05Jul2011ReReco-ECAL-v1/RECO
        /DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM         
        /DYToMuMu_M-20_CT10_TuneZ2_7TeV-powheg-pythia/Summer11-PU_S4_START42_V11-v1/AODSIM        
        /TTJets_TuneZ2_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v2/GEN-SIM-RECO            
        /TTJets_TuneZ2_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v2/AODSIM                  
        /WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/GEN-SIM-RECO        
        /WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM              
        /QCD_Pt-20_MuEnrichedPt-15_TuneZ2_7TeV-pythia6/Summer11-PU_S4_START42_V11-v1/GEN-SIM-RECO 
        /QCD_Pt-20_MuEnrichedPt-15_TuneZ2_7TeV-pythia6/Summer11-PU_S4_START42_V11-v1/AODSIM       
        """.split(),
    "Diphoton": """
        /DoubleElectron/Run2011A-ZElectron-05Jul2011ReReco-ECAL-v1/RAW-RECO
        /DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM
        /TTJets_TuneZ2_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v2/GEN-SIM-RECO    
        /TTJets_TuneZ2_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v2/AODSIM          
        /WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/GEN-SIM-RECO
        /WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM      
        /DYToEE_M-20_CT10_TuneZ2_7TeV-powheg-pythia/Summer11-PU_S4_START42_V11-v1/AODSIM  
        """.split(),
    "Inclusive": """
        /G_Pt-80to120_TuneZ2_7TeV_pythia6/Summer11-PU_S4_START42_V11-v1/AODSIM
        /G_Pt-170to300_TuneZ2_7TeV_pythia6/Summer11-PU_S4_START42_V11-v1/AODSIM
        /G_Pt-15to3000_TuneZ2_Flat_7TeV_pythia6/Summer11-HCal_PU_S4_START42_V11-v1/AODSIM
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

