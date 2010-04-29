import FWCore.ParameterSet.Config as cms

MuMuGammasOneStandAloneMuon = cms.EDFilter("CandViewShallowCloneCombiner",
    checkCharge = cms.bool(False),
    cut = cms.string('mass > 0'),
    decay = cms.string('dimuonsOneStandAloneMuon selectedPatPhotons')
)
