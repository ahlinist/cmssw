import FWCore.ParameterSet.Config as cms

IdentifiedTaus = cms.EDProducer("OfflineTauIDProducer",
	PFTauProducer 			= cms.InputTag('shrinkingConePFTauProducer'),
	JetPtMin			= cms.double(10.),
        MatchingCone            	= cms.double(0.1),
        TrackerSignalConeSizeFormula  	= cms.string("5.0/ET"),
        TrackerSignalConeSize_min     	= cms.double(0.07),
        TrackerSignalConeSize_max     	= cms.double(0.15),
        IsolationCone           	= cms.double(0.4),
        LeadTrack_minPt         	= cms.double(1.0),
        Track_minPt             	= cms.double(2.),
        Metric                  	= cms.string("DR"),
        Isolation_Tracksmaxn    	= cms.int32(0)
)

