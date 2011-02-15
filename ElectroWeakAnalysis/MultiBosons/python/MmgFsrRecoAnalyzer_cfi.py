import FWCore.ParameterSet.Config as cms

analyzeMmgFsr= cms.EDAnalyzer("MmgFsrRecoAnalyzer",
  photonSrc   = cms.untracked.InputTag("photons"),
  muonSrc     = cms.untracked.InputTag("muons"),
  beamSpotSrc = cms.untracked.InputTag("offlineBeamSpot")
)
