import FWCore.ParameterSet.Config as cms

patJetTag = cms.EDAnalyzer("PATValidation_Jet",
     OutputFile  = cms.untracked.string('benchmark.root'),

     recoJet     = cms.InputTag('iterativeCone5CaloJets'),
     patJet      = cms.InputTag('allLayer1Jets'),
)

patMuonTag = cms.EDAnalyzer("PATValidation_Muon",
     OutputFile  = cms.untracked.string('benchmark.root'),

     recoMuon    = cms.InputTag('muons'),
     patMuon     = cms.InputTag('allLayer1Muons'),
)

patElectronTag = cms.EDAnalyzer("PATValidation_Electron",
     OutputFile  = cms.untracked.string('benchmark.root'),

     recoElectron= cms.InputTag('pixelMatchGsfElectrons'),
     patElectron = cms.InputTag('allLayer1Electrons'),
	)

patMETTag = cms.EDAnalyzer("PATValidation_MET",
     OutputFile  = cms.untracked.string('benchmark.root'),

     recoMET     = cms.InputTag('met'),
     patMET      = cms.InputTag('allLayer1METs'),
	)

patTauTag = cms.EDAnalyzer("PATValidation_Tau",
     OutputFile  = cms.untracked.string('benchmark.root'),

    recoTau     = cms.InputTag('pfRecoTauProducer'),
     patTau      = cms.InputTag('allLayer1Taus'),

	)

patPhotonTag = cms.EDAnalyzer("PATValidation_Photon",
     OutputFile  = cms.untracked.string('benchmark.root'),

     recoPhoton  = cms.InputTag('photons'),
     patPhoton   = cms.InputTag('allLayer1Photons'),
	)
 

