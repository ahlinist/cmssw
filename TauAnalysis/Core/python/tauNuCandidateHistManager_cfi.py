import FWCore.ParameterSet.Config as cms

tauNuCandidateHistManager = cms.PSet(    
  pluginName = cms.string('tauNuCandidateHistManager'),
  pluginType = cms.string('PATTauNuHistManager'),
      
  tauNuCandidateSource = cms.InputTag('allTauNuPairs'),

  dqmDirectory_store = cms.string('TauNuCandidateQuantities'),

  #requireGenMatch = cms.bool(True),
  requireGenMatch = cms.bool(False),

  #normalization = cms.string("tauNuCandidates")
  normalization = cms.string("events")
)
