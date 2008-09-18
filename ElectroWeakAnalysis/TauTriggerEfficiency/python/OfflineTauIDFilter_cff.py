import FWCore.ParameterSet.Config as cms

IdentifiedTauFilter = cms.EDFilter("OfflineTauIDFilter",
                                   PfTauSource = cms.InputTag("IdentifiedTaus"),
                                   CaloTauSource = cms.InputTag("IdentifiedTaus")
)

