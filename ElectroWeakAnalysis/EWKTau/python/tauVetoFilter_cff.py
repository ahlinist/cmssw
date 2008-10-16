import FWCore.ParameterSet.Config as cms

tauVetoFilter = cms.EDFilter("TauVetoFilter",
PfTauTag = cms.InputTag(""),
CaloTauTag = cms.InputTag(""),
PatTauTag = cms.InputTag(""),
MinEtCut = cms.double(20.),
MaxNCut = cms.int32(1),
Type = cms.string("Pf")
)     

