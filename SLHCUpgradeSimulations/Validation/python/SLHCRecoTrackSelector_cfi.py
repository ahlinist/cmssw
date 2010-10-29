import FWCore.ParameterSet.Config as cms

SLHCRecoTrackSelector = cms.EDFilter("SLHCRecoTrackSelector",
    src = cms.InputTag("generalTracks"),
    maxChi2 = cms.double(5.0),
    tip = cms.double(0.2),
    minRapidity = cms.double(-5.0),
    lip = cms.double(17.0),
    ptMin = cms.double(1.0),
    maxRapidity = cms.double(5.0),
    quality = cms.vstring('loose'),
    algorithm = cms.vstring(),
    minHit = cms.int32(8),
    min3DHit = cms.int32(0),
    minPxHit = cms.int32(2),
    beamSpot = cms.InputTag("offlineBeamSpot")
)



