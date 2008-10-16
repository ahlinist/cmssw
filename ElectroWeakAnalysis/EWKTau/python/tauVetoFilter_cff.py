import FWCore.ParameterSet.Config as cms

tauVetoFilter = cms.EDFilter("TauVetoFilter",
PfTauTag = cms.InputTag(""),
CaloTauTag = cms.InputTag(""),
PatTauTag = cms.InputTag(""),
MinEtCut = cms.double(20.),
MaxEtCut = cms.double(5.)
)     

