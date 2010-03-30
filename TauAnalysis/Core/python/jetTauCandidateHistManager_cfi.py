import FWCore.ParameterSet.Config as cms

jetTauCandidateHistManager = cms.PSet(    
  pluginName = cms.string('jetTauCandidateHistManager'),
  pluginType = cms.string('PATJetTauPairHistManager'),
     
  diTauCandidateSource = cms.InputTag('allJetTauPairs'),

  #dqmDirectory_store = cms.string('JetTauCandidateQuantities'),

  vertexSource = cms.InputTag('selectedPrimaryVertexPosition'),

  dqmDirectory_store = cms.string('DiTauCandidateQuantities'),

  #requireGenMatch = cms.bool(True),
  requireGenMatch = cms.bool(False),

  #normalization = cms.string("diTauCandidates"),
  normalization = cms.string("events"),
)
