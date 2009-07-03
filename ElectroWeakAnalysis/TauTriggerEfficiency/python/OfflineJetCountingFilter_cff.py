import FWCore.ParameterSet.Config as cms

JetCountingFilter = cms.EDFilter("OfflineJetCountingFilter",
	JetSource       = cms.InputTag("iterativeCone5CaloJets"),
        RawJetEtMin     = cms.double( 20. ),
        RawJetEtaMax    = cms.double( 2.5 ),
        MaxNumberOfJets = cms.int32(1)
)
