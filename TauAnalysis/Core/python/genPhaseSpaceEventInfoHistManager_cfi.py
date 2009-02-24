import FWCore.ParameterSet.Config as cms

genPhaseSpaceEventInfoHistManager = cms.PSet(
  name = cms.string('genPhaseSpaceEventInfoHistManager'),
  type = cms.string('GenPhaseSpaceEventInfoHistManager'),
      
  source = cms.InputTag('genPhaseSpaceEventInfo'),

  dqmDirectory_store = cms.string('GenPhaseSpaceEventInfoQuantities')
)
