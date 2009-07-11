import FWCore.ParameterSet.Config as cms

metTopologyHistManager = cms.PSet(    
  pluginName = cms.string('metTopologyHistManager'),
  pluginType = cms.string('MEtTopologyHistManager'),
      
  metTopologySource = cms.InputTag('metTopologies'),

  dqmDirectory_store = cms.string('MEtTopologyQuantities'),
)
