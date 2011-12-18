#/usr/bin/env python

scheduler = 'glite'
# scheduler = 'condor'

samples = {
    "Dimuon": """
        /DYToMuMu_M-20_CT10_TuneZ2_7TeV-powheg-pythia/Fall11-PU_S6_START42_V14B-v1/AODSIM
        /DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM
        /QCD_Pt-20_MuEnrichedPt-15_TuneZ2_7TeV-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM
        /TTJets_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v2/AODSIM
        /TT_TuneZ2_7TeV-powheg-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM
        /WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM
        /ZGToMuMuG_TuneZ2_7TeV-madgraph/Fall11-PU_S6_START42_V14B-v1/AODSIM
        """.split(),
        #/DYToMuMu_M-20_CT10_TuneZ2_7TeV-powheg-pythia/Fall11-PU_S6_START42_V14B-v1/GEN-SIM-RECO
        #/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/GEN-SIM-RECO
        #/QCD_Pt-20_MuEnrichedPt-15_TuneZ2_7TeV-pythia6/Fall11-PU_S6_START42_V14B-v1/GEN-SIM-RECO
        #/TTJets_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v2/GEN-SIM-RECO
        #/TT_TuneZ2_7TeV-powheg-tauola/Fall11-PU_S6_START42_V14B-v1/GEN-SIM-RECO
        #/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/GEN-SIM-RECO
        #/ZGToMuMuG_TuneZ2_7TeV-madgraph/Fall11-PU_S6_START42_V14B-v1/GEN-SIM-RECO
    
    #"Diphoton": """
        #/DYToEE_M-20_CT10_TuneZ2_7TeV-powheg-pythia/Fall11-PU_S6_START42_V14B-v1/AODSIM
        #/WGToENuG_TuneZ2_7TeV-madgraph/Fall11-PU_S6_START42_V14B-v1/AODSIM
        #/WGToMuNuG_TuneZ2_7TeV-madgraph/Fall11-PU_S6_START42_V14B-v1/AODSIM
        #/WGToTauNuG_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM
        #/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM
        #/ZGToEEG_TuneZ2_7TeV-madgraph/Fall11-PU_S6_START42_V14B-v1/AODSIM
        #/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM
        #/ZGToTauTauG_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM
        #""".split(),
    #"Inclusive": """
        #/GJet_Pt-20_doubleEMEnriched_TuneZ2_7TeV-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM
        #/GluGluToHToGG_M-90_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM
        #/GluGluToHToGG_M-120_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM
        #/GluGluToHToGG_M-125_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM
        #/GluGluToHToGG_M-140_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM
        #""".split(),
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
USER.publish_data_name = {PROCESSED_NAME}_{SCHEDULER}_{SHORT_LABEL}-42X-v10"""\
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
