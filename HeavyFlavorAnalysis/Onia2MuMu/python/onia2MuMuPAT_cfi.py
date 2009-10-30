import FWCore.ParameterSet.Config as cms

onia2MuMuPAT = cms.EDProducer('Onia2MuMuPAT',
  higherPuritySelection = cms.int32(1),
  lowerPuritySelection  = cms.int32(1)
)
