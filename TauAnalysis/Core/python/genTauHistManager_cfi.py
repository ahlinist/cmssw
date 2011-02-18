import FWCore.ParameterSet.Config as cms

genTauHistManager = cms.PSet(    
    pluginName = cms.string('genTauHistManager'),
    pluginType = cms.string('GenTauHistManager'),
      
    genParticleSource = cms.InputTag('genParticles'),

    dqmDirectory_store = cms.string('GenTauQuantities'),
)
