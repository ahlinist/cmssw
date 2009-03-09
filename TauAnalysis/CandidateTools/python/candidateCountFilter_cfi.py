import FWCore.ParameterSet.Config as cms

candidateCountFilter = cms.EDFilter(
    "CandViewCountFilter",
    src = cms.InputTag(''),
    minNumber = cms.uint32(0)
)
