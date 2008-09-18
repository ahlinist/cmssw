import FWCore.ParameterSet.Config as cms

IdentifiedTaus = cms.EDProducer("OfflineTauIDProducer",
        MatchingCone            = cms.double(0.1),
        TrackerSignalConeSizeFormula  = cms.string("0.07"),
        TrackerSignalConeSize_min     = cms.double(0.07),
        TrackerSignalConeSize_max     = cms.double(0.15),
        IsolationCone           = cms.double(0.4),
        LeadTrack_minPt         = cms.double(20),
        Track_minPt             = cms.double(1),
        Metric                  = cms.string("DR"),
        Isolation_Tracksmaxn    = cms.int32(0)
)

