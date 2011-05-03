import FWCore.ParameterSet.Config as cms

htRatioHistManager = cms.PSet(    
  pluginName = cms.string('htRatioHistManager'),
  pluginType = cms.string('HtRatioHistManager'),
      
  htRatioSource = cms.InputTag('htRatios'),

  dqmDirectory_store = cms.string('HtRatioQuantities'),
)
