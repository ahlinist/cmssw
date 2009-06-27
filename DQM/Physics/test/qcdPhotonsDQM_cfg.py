import FWCore.ParameterSet.Config as cms

process = cms.Process("QcdPhotonsDQM")

process.load("DQMServices.Core.DQM_cfg")
process.load("DQMServices.Components.DQMEnvironment_cfi")
process.DQM.collectorHost = ''

process.dqmSaver.workflow = cms.untracked.string('/My/Test/DataSet')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
    "/store/relval/CMSSW_3_1_0_pre9/RelValGammaJets_Pt_80_120/GEN-SIM-RECO/STARTUP_31X_v1/0006/6AA9BBFD-9F4E-DE11-8064-001617C3B79A.root",
    "/store/relval/CMSSW_3_1_0_pre9/RelValGammaJets_Pt_80_120/GEN-SIM-RECO/STARTUP_31X_v1/0006/3A31C266-A14E-DE11-80AB-001D09F28F11.root",
    "/store/relval/CMSSW_3_1_0_pre9/RelValGammaJets_Pt_80_120/GEN-SIM-RECO/STARTUP_31X_v1/0006/2E9E0AED-A34E-DE11-86D2-001D09F251D1.root",
    "/store/relval/CMSSW_3_1_0_pre9/RelValGammaJets_Pt_80_120/GEN-SIM-RECO/STARTUP_31X_v1/0006/1E82DB64-B44E-DE11-880E-001D09F27003.root"
                           )
)

# DQM monitor module for QCD-Photons: Photon+Jet
process.QcdPhotonsAnalyzer = cms.EDAnalyzer("QcdPhotonsDQM",
                            triggerPathToPass        = cms.string("HLT_Photon15_L1R"),
                            triggerResultsCollection = cms.InputTag("TriggerResults", "", "HLT"),
                            photonCollection         = cms.InputTag("photons"),
                            caloJetCollection        = cms.InputTag("sisCone5CaloJets"),
#                           caloJetCollection        = cms.InputTag("L2L3CorJetSC5Calo"),
)

process.p = cms.Path(process.QcdPhotonsAnalyzer+process.dqmSaver)

