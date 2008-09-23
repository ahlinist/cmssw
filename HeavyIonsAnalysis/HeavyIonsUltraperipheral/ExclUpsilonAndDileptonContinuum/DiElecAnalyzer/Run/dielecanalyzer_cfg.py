import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        'rfio:/castor/cern.ch/user/k/kumarv/CMSSW214/Elec/u1s_pbpb_ee_reco-00_root_1',
        'rfio:/castor/cern.ch/user/k/kumarv/CMSSW214/Elec/u1s_pbpb_ee_reco-01_root_1',
        'rfio:/castor/cern.ch/user/k/kumarv/CMSSW214/Elec/u2s_pbpb_ee_reco_root_1',
        'rfio:/castor/cern.ch/user/k/kumarv/CMSSW214/Elec/u3s_pbpb_ee_reco_root_1'
#    'rfio:/castor/cern.ch/user/k/kumarv/CMSSW214/Elec/pbpb-eecont-reco-01_root_1',
#   'rfio:/castor/cern.ch/user/k/kumarv/CMSSW214/Elec/pbpb-eecont-reco-03_root_1',
#    'rfio:/castor/cern.ch/user/k/kumarv/CMSSW214/Elec/pbpb-eecont-reco-04_root_1',
#    'rfio:/castor/cern.ch/user/k/kumarv/CMSSW214/Elec/pbpb-eecont-reco-05_root_1',
#    'rfio:/castor/cern.ch/user/k/kumarv/CMSSW214/Elec/pbpb-eecont-reco-06_root_1',
#    'rfio:/castor/cern.ch/user/k/kumarv/CMSSW214/Elec/pbpb-eecont-reco-07_root_1',
#    'rfio:/castor/cern.ch/user/k/kumarv/CMSSW214/Elec/pbpb-eecont-reco-08_root_1',
#    'rfio:/castor/cern.ch/user/k/kumarv/CMSSW214/Elec/pbpb-eecont-reco-09_root_1',
#    'rfio:/castor/cern.ch/user/k/kumarv/CMSSW214/Elec/pbpb-eecont-reco-010_root_1',
#    'rfio:/castor/cern.ch/user/k/kumarv/CMSSW214/Elec/pbpb-eecont-reco-011_root_1',
#    'rfio:/castor/cern.ch/user/k/kumarv/CMSSW214/Elec/pbpb-eecont-reco-012_root_1',
#    'rfio:/castor/cern.ch/user/k/kumarv/CMSSW214/Elec/pbpb-eecont-reco-013_root_1',
#    'rfio:/castor/cern.ch/user/k/kumarv/CMSSW214/Elec/pbpb-eecont-reco-014_root_1',
#    'rfio:/castor/cern.ch/user/k/kumarv/CMSSW214/Elec/pbpb-eecont-reco-015_root_1',
#    'rfio:/castor/cern.ch/user/k/kumarv/CMSSW214/Elec/pbpb-eecont-reco-016_root_1',
#    'rfio:/castor/cern.ch/user/k/kumarv/CMSSW214/Elec/pbpb-eecont-reco-017_root_1',
#    'rfio:/castor/cern.ch/user/k/kumarv/CMSSW214/Elec/pbpb-eecont-reco-018_root_1',
#    'rfio:/castor/cern.ch/user/k/kumarv/CMSSW214/Elec/pbpb-eecont-reco-019_root_1'









    )
)

process.demo = cms.EDAnalyzer('DiElecAnalyzer'
)


process.p = cms.Path(process.demo)
