import FWCore.ParameterSet.Config as cms

process = cms.Process("QcdPhotonsDQM")
process.load("DQM.Physics.qcdPhotonsDQM_cfi")

process.load("DQMServices.Core.DQM_cfg")
process.load("DQMServices.Components.DQMEnvironment_cfi")
process.DQM.collectorHost = ''

process.dqmSaver.workflow = cms.untracked.string('/My/Test/DataSet')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
     '/store/relval/CMSSW_3_1_1/RelValGammaJets_Pt_80_120/GEN-SIM-RECO/STARTUP31X_V1-v2/0002/AA75B481-D16B-DE11-A245-000423D99996.root',
     '/store/relval/CMSSW_3_1_1/RelValGammaJets_Pt_80_120/GEN-SIM-RECO/STARTUP31X_V1-v2/0002/8240F64F-D36B-DE11-ACC1-000423D6006E.root',
     '/store/relval/CMSSW_3_1_1/RelValGammaJets_Pt_80_120/GEN-SIM-RECO/STARTUP31X_V1-v2/0002/70043885-D76B-DE11-BED8-000423D98B28.root',
     '/store/relval/CMSSW_3_1_1/RelValGammaJets_Pt_80_120/GEN-SIM-RECO/STARTUP31X_V1-v2/0002/6C6D6590-D56B-DE11-AD51-001D09F24353.root',
     '/store/relval/CMSSW_3_1_1/RelValGammaJets_Pt_80_120/GEN-SIM-RECO/STARTUP31X_V1-v2/0002/421096F6-CF6B-DE11-AC03-000423D6A6F4.root'
                           )
)

process.p = cms.Path(process.qcdPhotonsDQM+process.dqmSaver)

