import FWCore.ParameterSet.Config as cms

MuMuGammasGlobal = cms.EDFilter("CandViewShallowCloneCombiner",
    checkCharge = cms.bool(False),
    cut = cms.string('mass > 0'),
    decay = cms.string('dimuonsGlobal selectedPatPhotons')
)
