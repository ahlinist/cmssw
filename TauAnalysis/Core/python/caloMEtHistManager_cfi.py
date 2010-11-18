import FWCore.ParameterSet.Config as cms

caloMEtHistManager = cms.PSet(
    pluginName = cms.string('caloMEtHistManager'),
    pluginType = cms.string('CaloMEtHistManager'),
      
    metSource = cms.InputTag('patMETs'),
    #metSignificanceSource = cms.InputTag('met'),
    metSignificanceSource = cms.InputTag('metsignificance'),
    vertexSource = cms.InputTag('selectedPrimaryVertexHighestPtTrackSum'),

    vertexPtThresholds = cms.vdouble(5., 10., 15., 20.),
    
    dqmDirectory_store = cms.string('CaloMEtQuantities')
)
