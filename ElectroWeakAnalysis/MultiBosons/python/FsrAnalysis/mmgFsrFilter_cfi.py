import FWCore.ParameterSet.Config as cms

mmgFsrFilter = cms.EDFilter("MmgFsrFilter",
    photonSrc   = cms.untracked.InputTag("cleanPatPhotonsTriggerMatch"),
    muonSrc     = cms.untracked.InputTag("cleanPatMuonsTriggerMatch"),
    beamSpotSrc = cms.untracked.InputTag("offlineBeamSpot"),
    ebRecHitSrc = cms.untracked.InputTag("reducedEcalRecHitsEB"),
    eeRecHitSrc = cms.untracked.InputTag("reducedEcalRecHitsEE"),
    requireFsr  = cms.untracked.bool(False),
    vetoFsr     = cms.untracked.bool(False),
    verbosity   = cms.untracked.int32(1),
)
