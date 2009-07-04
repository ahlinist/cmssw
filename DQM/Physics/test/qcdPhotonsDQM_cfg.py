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
      '/store/relval/CMSSW_3_1_0/RelValGammaJets_Pt_80_120/GEN-SIM-RECO/STARTUP31X_V1-v1/0002/2AAA6789-DF66-DE11-AE21-001D09F24024.root',
      '/store/relval/CMSSW_3_1_0/RelValGammaJets_Pt_80_120/GEN-SIM-RECO/STARTUP31X_V1-v1/0001/E62A1021-4F66-DE11-9AC2-001D09F29533.root',
      '/store/relval/CMSSW_3_1_0/RelValGammaJets_Pt_80_120/GEN-SIM-RECO/STARTUP31X_V1-v1/0001/522DD728-4E66-DE11-8738-001D09F2924F.root',
      '/store/relval/CMSSW_3_1_0/RelValGammaJets_Pt_80_120/GEN-SIM-RECO/STARTUP31X_V1-v1/0001/3CF955C7-4D66-DE11-962A-001D09F29146.root',
      '/store/relval/CMSSW_3_1_0/RelValGammaJets_Pt_80_120/GEN-SIM-RECO/STARTUP31X_V1-v1/0001/1883AC54-4F66-DE11-B02A-001D09F24D8A.root'
                           )
)

process.p = cms.Path(process.qcdPhotonsDQM+process.dqmSaver)

