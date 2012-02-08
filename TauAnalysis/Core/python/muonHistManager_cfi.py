import FWCore.ParameterSet.Config as cms

from TauAnalysis.RecoTools.patLeptonSelection_cff import selectedPatMuonsPFRelIso

muonHistManager = cms.PSet(
    pluginName = cms.string('muonHistManager'),
    pluginType = cms.string('MuonHistManager'),
      
    muonSource = cms.InputTag('cleanPatMuons'),
    vertexSource = cms.InputTag('selectedPrimaryVertexHighestPtTrackSum'),
    beamSpotSource = cms.InputTag('offlineBeamSpot'),    
    jetSource = cms.InputTag('selectedPatJetsEt20Cumulative'),
    genParticleSource = cms.InputTag('genParticles'),

    dqmDirectory_store = cms.string('MuonQuantities'),

    #requireGenMuonMatch = cms.bool(True),
    requireGenMuonMatch = cms.bool(False),

    skipPdgIdsGenParticleMatch = cms.vint32( 12, 14, 16 ),

    pfCombIsoExtractor = cms.PSet(
        chargedParticleIso = selectedPatMuonsPFRelIso.chargedParticleIso,
        neutralHadronIso = selectedPatMuonsPFRelIso.neutralHadronIso,
        photonIso = selectedPatMuonsPFRelIso.photonIso
    ),

    pfCandidateSource = selectedPatMuonsPFRelIso.pfCandidateSource,
    pfNoPileUpCandidateSource = cms.InputTag('pfNoPileUp'),
    
    #normalization = cms.string("muons"),
    normalization = cms.string("events"),

    makeIsoPtCtrlHistograms = cms.bool(False),
    makeIsoPtConeSizeDepHistograms = cms.bool(False)
)
