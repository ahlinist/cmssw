import FWCore.ParameterSet.Config as cms

onia2MuMuPAT = cms.EDProducer('Onia2MuMuPAT',
  muons = cms.InputTag("patMuons"),
  beamSpotTag = cms.InputTag("offlineBeamSpot"),
  primaryVertexTag = cms.InputTag("offlinePrimaryVertices"),
  higherPuritySelection = cms.int32(1),
  lowerPuritySelection  = cms.int32(1),
  addCommonVertex = cms.bool(True), ## Embed the full reco::Vertex out of the common vertex fit
  addMCTruth = cms.bool(True),      ## Add the common MC mother of the two muons, if any
)
