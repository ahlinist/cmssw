import FWCore.ParameterSet.Config as cms

vbfDiTauCandidateEventHistManager = cms.PSet(    
    pluginName = cms.string('vbfDiTauCandidateEventHistManager'),
    pluginType = cms.string('DiCandidatePairVBFEventHistManager'),
      
    vbfEventSource = cms.InputTag('vbfEventHypotheses'),

    dqmDirectory_store = cms.string('VBFDiTauCandidateEventQuantities'),

    #requireGenMatch = cms.bool(True),
    requireGenMatch = cms.bool(False),

    #normalization = cms.string("diTauCandidates"),
    normalization = cms.string("events")
)
