import FWCore.ParameterSet.Config as cms

ZllHypothesisHistManager = cms.PSet(    
    pluginName = cms.string('ZllHypothesisHistManager'),
    pluginType = cms.string('ZllHypothesisHistManager'),
      
    ZllHypothesisSource = cms.InputTag('ZllHypotheses'),

    dqmDirectory_store = cms.string('DiTauCandidateZllHypothesisQuantities'),

    #normalization = cms.string("diTauCandidates")
    normalization = cms.string("events")
)
