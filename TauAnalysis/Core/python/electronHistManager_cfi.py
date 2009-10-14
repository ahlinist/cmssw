import FWCore.ParameterSet.Config as cms

electronHistManager = cms.PSet(
    pluginName = cms.string('electronHistManager'),
    pluginType = cms.string('ElectronHistManager'),
      
    electronSource = cms.InputTag('cleanLayer1Electrons'),
    vertexSource = cms.InputTag('selectedPrimaryVertexPosition'),
    jetSource = cms.InputTag('selectedLayer1JetsEt20Cumulative'),
    genParticleSource = cms.InputTag('genParticles'),

    dqmDirectory_store = cms.string('ElectronQuantities'),

    #requireGenElectronMatch = cms.bool(True),
    requireGenElectronMatch = cms.bool(False),

    #normalization = cms.string("electrons"),
    normalization = cms.string("events"),

    makeIsoPtCtrlHistograms = cms.bool(False),
    makeIsoPtConeSizeDepHistograms = cms.bool(False)
)
