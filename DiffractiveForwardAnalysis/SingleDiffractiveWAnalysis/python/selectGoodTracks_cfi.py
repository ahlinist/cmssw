import FWCore.ParameterSet.Config as cms

selectGoodTracks = cms.EDFilter("TrackSelector",
   src = cms.InputTag("generalTracks"),
   cut = cms.string("pt > 0.3 & numberOfValidHits > 7 & d0 <= 3.5")
)
