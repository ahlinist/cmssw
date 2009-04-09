import FWCore.ParameterSet.Config as cms

muonHistManager = cms.PSet(
  name = cms.string('muonHistManager'),
  type = cms.string('MuonHistManager'),
      
  muonSource = cms.InputTag('cleanLayer1MuonsSel'),
  vertexSource = cms.InputTag('selectedPrimaryVertexPosition'),

  dqmDirectory_store = cms.string('MuonQuantities'),

  #requireGenMuonMatch = cms.bool(True)
  requireGenMuonMatch = cms.bool(False)
)
