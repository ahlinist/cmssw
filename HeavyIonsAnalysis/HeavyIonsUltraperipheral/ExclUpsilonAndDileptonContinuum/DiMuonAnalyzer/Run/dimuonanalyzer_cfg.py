import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
     #   'rfio:/castor/cern.ch/user/k/kumarv/CMSSW214/MuMu/up1s_mumu_reco-00_root_1',
     #   'rfio:/castor/cern.ch/user/k/kumarv/CMSSW214/MuMu/up1s_mumu_reco-01_root_1',
     #   'rfio:/castor/cern.ch/user/k/kumarv/CMSSW214/MuMu/up2s_mumu_reco-00_root_1',
     #   'rfio:/castor/cern.ch/user/k/kumarv/CMSSW214/MuMu/up3s_mumu_reco-00_root_1'
         'rfio:/castor/cern.ch/user/k/kumarv/CMSSW214/MuMu/pbpb_mumucont_reco-00_root_1',
         'rfio:/castor/cern.ch/user/k/kumarv/CMSSW214/MuMu/pbpb_mumucont_reco-02_root_1',
         'rfio:/castor/cern.ch/user/k/kumarv/CMSSW214/MuMu/pbpb_mumucont_reco-04_root_1',
         'rfio:/castor/cern.ch/user/k/kumarv/CMSSW214/MuMu/pbpb_mumucont_reco-05_root_1',
         'rfio:/castor/cern.ch/user/k/kumarv/CMSSW214/MuMu/pbpb_mumucont_reco-06_root_1',
         'rfio:/castor/cern.ch/user/k/kumarv/CMSSW214/MuMu/pbpb_mumucont_reco-07_root_1',
         'rfio:/castor/cern.ch/user/k/kumarv/CMSSW214/MuMu/pbpb_mumucont_reco-08_root_1',
         'rfio:/castor/cern.ch/user/k/kumarv/CMSSW214/MuMu/pbpb_mumucont_reco-09_root_1',
         'rfio:/castor/cern.ch/user/k/kumarv/CMSSW214/MuMu/pbpb_mumucont_reco-011_root_1',
         'rfio:/castor/cern.ch/user/k/kumarv/CMSSW214/MuMu/pbpb_mumucont_reco-012_root_1',
         'rfio:/castor/cern.ch/user/k/kumarv/CMSSW214/MuMu/pbpb_mumucont_reco-014_root_1',
         'rfio:/castor/cern.ch/user/k/kumarv/CMSSW214/MuMu/pbpb_mumucont_reco-015_root_1',
         'rfio:/castor/cern.ch/user/k/kumarv/CMSSW214/MuMu/pbpb_mumucont_reco-016_root_1',
         'rfio:/castor/cern.ch/user/k/kumarv/CMSSW214/MuMu/pbpb_mumucont_reco-017_root_1',
         'rfio:/castor/cern.ch/user/k/kumarv/CMSSW214/MuMu/pbpb_mumucont_reco-018_root_1',
         'rfio:/castor/cern.ch/user/k/kumarv/CMSSW214/MuMu/pbpb_mumucont_reco-019_root_1'
          




    )
)

process.demo = cms.EDAnalyzer('DiMuonAnalyzer'
)


process.p = cms.Path(process.demo)
