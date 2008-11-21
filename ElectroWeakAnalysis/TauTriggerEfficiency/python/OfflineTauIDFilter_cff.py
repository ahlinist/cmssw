import FWCore.ParameterSet.Config as cms

IdentifiedTauFilter = cms.EDFilter("OfflineTauIDFilter",
	#PfTauSource = cms.InputTag("IdentifiedTaus"),
	PfTauSource = cms.InputTag("PFTausSelected"),
	CaloTauSource = cms.InputTag("IdentifiedTaus")
)

