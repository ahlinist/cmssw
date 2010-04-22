import FWCore.ParameterSet.Config as cms

MuMuGammas = cms.EDFilter("CandViewShallowCloneCombiner",
    checkCharge = cms.bool(False),
    cut = cms.string('mass > 0'),
    decay = cms.string('dimuons selectedPatPhotons')
)
