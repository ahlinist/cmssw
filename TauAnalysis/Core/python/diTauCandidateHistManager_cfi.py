import FWCore.ParameterSet.Config as cms

diTauCandidateHistManager = cms.PSet(    
  pluginName = cms.string('diTauCandidateHistManager'),
  pluginType = cms.string('DiCandidatePairHistManager'),
      
  diTauCandidateSource = cms.InputTag(''),

  vertexSource = cms.InputTag('selectedPrimaryVertexPosition'),

  dqmDirectory_store = cms.string('DiTauCandidateQuantities'),

  #requireGenMatch = cms.bool(True)
  requireGenMatch = cms.bool(False)
)
