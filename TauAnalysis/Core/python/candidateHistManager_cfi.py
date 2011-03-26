import FWCore.ParameterSet.Config as cms
import copy

candidateHistManager = cms.PSet(
  pluginName = cms.string('candidateHistManager'),
  pluginType = cms.string('CandidateHistManager'),
      
  candidateSource = cms.InputTag(''),

  dqmDirectory_store = cms.string('CandidateQuantities'),

  #normalization = cms.string("candidates")
  normalization = cms.string("events")
)
