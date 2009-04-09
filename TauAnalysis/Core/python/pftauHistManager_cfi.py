import FWCore.ParameterSet.Config as cms

tauHistManager = cms.PSet(    
  name = cms.string('tauHistManager'),
  type = cms.string('TauHistManager'),
      
  tauSource = cms.InputTag('cleanLayer1TausSel'),
  vertexSource = cms.InputTag('selectedPrimaryVertexPosition'),

  tauIndicesToPlot = cms.string('all'),

  dqmDirectory_store = cms.string('TauQuantities'),

  #requireGenTauMatch = cms.bool(True)
  requireGenTauMatch = cms.bool(False)
)
