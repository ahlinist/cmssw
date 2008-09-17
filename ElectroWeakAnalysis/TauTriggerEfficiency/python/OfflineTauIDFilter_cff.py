import FWCore.ParameterSet.Config as cms

IdentifiedTaus = cms.EDProducer("OfflineTauIDFilter",
        MatchingCone            = cms.double(0.1),
        SignalCone              = cms.double(0.07),
        IsolationCone           = cms.double(0.4),
        LeadTrack_minPt         = cms.double(20),
        Track_minPt             = cms.double(1),
        Metric                  = cms.string("DR"),
        Isolation_Tracksmaxn    = cms.int32(0)
)

