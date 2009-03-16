import FWCore.ParameterSet.Config as cms

pfCandidateHistManager = cms.PSet(
  name = cms.string('pfCandidateHistManager'),
  type = cms.string('PFCandidateHistManager'),
      
  pfCandidateSource = cms.InputTag(''),

  dqmDirectory_store = cms.string('PFCandidateQuantities')
)
