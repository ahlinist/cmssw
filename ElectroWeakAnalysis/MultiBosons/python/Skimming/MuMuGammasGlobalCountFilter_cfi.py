import FWCore.ParameterSet.Config as cms

MuMuGammasGlobalCountFilter = cms.EDFilter("CandViewCountFilter",
    src = cms.InputTag("MuMuGammasGlobal"),
    minNumber = cms.uint32(1)
)

