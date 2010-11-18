import FWCore.ParameterSet.Config as cms

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

    #normalization = cms.string("electrons"),
    normalization = cms.string("events"),

    makeIsoPtCtrlHistograms = cms.bool(False),
    makeIsoPtConeSizeDepHistograms = cms.bool(False)
)
