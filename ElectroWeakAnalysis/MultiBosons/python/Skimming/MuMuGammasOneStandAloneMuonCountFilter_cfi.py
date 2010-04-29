import FWCore.ParameterSet.Config as cms

MuMuGammasOneStandAloneMuonCountFilter = cms.EDFilter("CandViewCountFilter",
    src = cms.InputTag("MuMuGammasOneStandAloneMuon"),
    minNumber = cms.uint32(1)
)

