import FWCore.ParameterSet.Config as cms

candidateCountFilter = cms.EDFilter(
    "CandCountFilter",
    src = cms.InputTag(''),
    minNumber = cms.uint32(0)
)
