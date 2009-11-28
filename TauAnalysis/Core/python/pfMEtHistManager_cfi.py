import FWCore.ParameterSet.Config as cms

pfMEtHistManager = cms.PSet(
    pluginName = cms.string('pfMEtHistManager'),
    pluginType = cms.string('PFMEtHistManager'),
      
    metSource = cms.InputTag('layer1PFMETs'),
    metSignificanceSource = cms.InputTag(''),
    
    dqmDirectory_store = cms.string('PFMEtQuantities')
)
