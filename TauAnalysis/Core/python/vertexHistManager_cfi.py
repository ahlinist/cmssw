import FWCore.ParameterSet.Config as cms

vertexHistManager = cms.PSet(
  pluginName = cms.string('vertexHistManager'),
  pluginType = cms.string('VertexHistManager'),
      
  vertexSource = cms.InputTag('selectedPrimaryVertexPosition'),

  dqmDirectory_store = cms.string('VertexQuantities'),

  vertexPtThresholds = cms.vdouble(0., 5., 10., 15., 20.),

  makeVertexXvsYhistogram = cms.bool(False)
)
