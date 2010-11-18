import FWCore.ParameterSet.Config as cms

pfMEtHistManager = cms.PSet(
    pluginName = cms.string('pfMEtHistManager'),
    pluginType = cms.string('PFMEtHistManager'),
      
    metSource = cms.InputTag('patPFMETs'),
    metSignificanceSource = cms.InputTag(''),
    vertexSource = cms.InputTag('selectedPrimaryVertexHighestPtTrackSum'),

    vertexPtThresholds = cms.vdouble(5., 10., 15., 20.),
    
    dqmDirectory_store = cms.string('PFMEtQuantities')
)
