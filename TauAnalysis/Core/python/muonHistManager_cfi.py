import FWCore.ParameterSet.Config as cms

muonHistManager = cms.PSet(
    pluginName = cms.string('muonHistManager'),
    pluginType = cms.string('MuonHistManager'),
      
    muonSource = cms.InputTag('cleanLayer1Muons'),
    vertexSource = cms.InputTag('selectedPrimaryVertexPosition'),
    jetSource = cms.InputTag('selectedLayer1JetsEt20Cumulative'),
    genParticleSource = cms.InputTag('genParticles'),

    dqmDirectory_store = cms.string('MuonQuantities'),

    #requireGenMuonMatch = cms.bool(True),
    requireGenMuonMatch = cms.bool(False),

    #normalization = cms.string("muons"),
    normalization = cms.string("events"),

    makeIsoPtCtrlHistograms = cms.bool(False),
    makeIsoPtConeSizeDepHistograms = cms.bool(False)
)
