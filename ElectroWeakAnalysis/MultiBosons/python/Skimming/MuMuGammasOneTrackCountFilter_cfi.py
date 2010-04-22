import FWCore.ParameterSet.Config as cms

MuMuGammasOneTrackCountFilter = cms.EDFilter("CandViewCountFilter",
    src = cms.InputTag("MuMuGammasOneTrack"),
    minNumber = cms.uint32(1)
)

