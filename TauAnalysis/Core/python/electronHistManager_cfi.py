import FWCore.ParameterSet.Config as cms

from TauAnalysis.RecoTools.patLeptonSelection_cff import selectedPatElectronsForElecTauIso

electronHistManager = cms.PSet(
    pluginName = cms.string('electronHistManager'),
    pluginType = cms.string('ElectronHistManager'),
      
    electronSource = cms.InputTag('cleanPatElectrons'),
    vertexSource = cms.InputTag('selectedPrimaryVertexHighestPtTrackSum'),
    jetSource = cms.InputTag('selectedPatJetsEt20Cumulative'),
    genParticleSource = cms.InputTag('genParticles'),

    dqmDirectory_store = cms.string('ElectronQuantities'),

    #requireGenElectronMatch = cms.bool(True),
    requireGenElectronMatch = cms.bool(False),

    skipPdgIdsGenParticleMatch = cms.vint32( 12, 14, 16 ),

	pfCombIsoExtractor = cms.PSet(
		chargedHadronIso = selectedPatElectronsForElecTauIso.chargedHadronIso,
		neutralHadronIso = selectedPatElectronsForElecTauIso.neutralHadronIso,
		photonIso = selectedPatElectronsForElecTauIso.photonIso
	),
	pfChargedHadronIsoExtractor = cms.PSet(
		chargedHadronIso = selectedPatElectronsForElecTauIso.chargedHadronIso
	),
	pfNeutralHadronIsoExtractor = cms.PSet(
		neutralHadronIso = selectedPatElectronsForElecTauIso.neutralHadronIso
	),
	pfPhotonIsoExtractor = cms.PSet(
		photonIso = selectedPatElectronsForElecTauIso.photonIso
	),

	pfCandidateSource = selectedPatElectronsForElecTauIso.pfCandidateSource,

    #normalization = cms.string("electrons"),
    normalization = cms.string("events"),

    makeIsoPtCtrlHistograms = cms.bool(False),
    makeIsoPtConeSizeDepHistograms = cms.bool(False)
)
