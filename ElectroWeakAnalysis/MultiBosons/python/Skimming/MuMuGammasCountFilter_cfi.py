import FWCore.ParameterSet.Config as cms

MuMuGammasCountFilter = cms.EDFilter("CandViewCountFilter",
    src = cms.InputTag("MuMuGammas"),
    minNumber = cms.uint32(1)
)

