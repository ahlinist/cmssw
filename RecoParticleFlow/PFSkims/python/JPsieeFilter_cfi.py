import FWCore.ParameterSet.Config as cms

jpsiupsilon_filter = cms.EDFilter("JPsieeFilter",
    motherId = cms.vint32(553, 100553, 443),
    moduleLabel = cms.untracked.string('source'),
    leptonType = cms.int32(11),
    leptonEtaMax = cms.double(3.1),
    doHistos = cms.untracked.bool(False),
    leptonEtaMin = cms.double(-3.1),
    leptonPtMin = cms.double(0.5)
)


