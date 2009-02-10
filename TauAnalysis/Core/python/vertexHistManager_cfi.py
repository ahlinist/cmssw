import FWCore.ParameterSet.Config as cms

vertexHistManager = cms.PSet(
  name = cms.string('vertexHistManager'),
  type = cms.string('VertexHistManager'),
      
  vertexSource = cms.InputTag('selectedPrimaryVertexPosition'),

  dqmDirectory_store = cms.string('VertexQuantities')
)
