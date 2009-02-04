import FWCore.ParameterSet.Config as cms

tauHistManager = cms.PSet(    
  name = cms.string('tauHistManager'),
  type = cms.string('TauHistManager'),
      
  tauSource = cms.InputTag('allLayer1PFTausSelForTauAnalyses'),

  dqmDirectory_store = cms.string('TauQuantities'),

  #requireGenTauMatch = cms.bool(True)
  requireGenTauMatch = cms.bool(False)
)
