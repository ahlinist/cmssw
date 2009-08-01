import FWCore.ParameterSet.Config as cms

electronHistManager = cms.PSet(
    pluginName = cms.string('electronHistManager'),
    pluginType = cms.string('ElectronHistManager'),
      
    electronSource = cms.InputTag('cleanLayer1ElectronsSel'),
    vertexSource = cms.InputTag('selectedPrimaryVertexPosition'),
    genParticleSource = cms.InputTag('genParticles'),

    dqmDirectory_store = cms.string('ElectronQuantities'),

    #requireGenElectronMatch = cms.bool(True)
    requireGenElectronMatch = cms.bool(False)
)
