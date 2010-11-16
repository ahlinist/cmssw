import FWCore.ParameterSet.Config as cms

from TauAnalysis.RecoTools.patLeptonSelection_cff import selectedPatMuonsPFRelIso

muonHistManager = cms.PSet(
    pluginName = cms.string('muonHistManager'),
    pluginType = cms.string('MuonHistManager'),
      
    muonSource = cms.InputTag('cleanPatMuons'),
    vertexSource = cms.InputTag('selectedPrimaryVertexPosition'),
    beamSpotSource = cms.InputTag('offlineBeamSpot'),    
    jetSource = cms.InputTag('selectedPatJetsEt20Cumulative'),
    genParticleSource = cms.InputTag('genParticles'),

    dqmDirectory_store = cms.string('MuonQuantities'),

    #requireGenMuonMatch = cms.bool(True),
    requireGenMuonMatch = cms.bool(False),

    skipPdgIdsGenParticleMatch = cms.vint32( 12, 14, 16 ),

    pfCombIsoExtractor = cms.PSet(
        chargedHadronIso = selectedPatMuonsPFRelIso.chargedHadronIso,
        neutralHadronIso = selectedPatMuonsPFRelIso.neutralHadronIso,
        photonIso = selectedPatMuonsPFRelIso.photonIso
    ),

    pfCandidateSource = selectedPatMuonsPFRelIso.pfCandidateSource,
    
    #normalization = cms.string("muons"),
    normalization = cms.string("events"),

    makeIsoPtCtrlHistograms = cms.bool(False),
    makeIsoPtConeSizeDepHistograms = cms.bool(False)
)
