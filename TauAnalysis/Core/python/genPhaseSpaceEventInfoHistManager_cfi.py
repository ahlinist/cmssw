import FWCore.ParameterSet.Config as cms

genPhaseSpaceEventInfoHistManager = cms.PSet(
  pluginName = cms.string('genPhaseSpaceEventInfoHistManager'),
  pluginType = cms.string('GenPhaseSpaceEventInfoHistManager'),
      
  source = cms.InputTag('genPhaseSpaceEventInfo'),

  dqmDirectory_store = cms.string('GenPhaseSpaceEventInfoQuantities')
)
