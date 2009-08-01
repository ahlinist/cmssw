import FWCore.ParameterSet.Config as cms

tauHistManager = cms.PSet(    
    pluginName = cms.string('tauHistManager'),
    pluginType = cms.string('TauHistManager'),
      
    tauSource = cms.InputTag('cleanLayer1TausSel'),
    vertexSource = cms.InputTag('selectedPrimaryVertexPosition'),
    genParticleSource = cms.InputTag('genParticles'),
  
    tauIndicesToPlot = cms.string('all'),

    dqmDirectory_store = cms.string('TauQuantities'),

    #requireGenTauMatch = cms.bool(True)
    requireGenTauMatch = cms.bool(False)
)
