import FWCore.ParameterSet.Config as cms

vgMuons = cms.EDProducer("ZMuMuAdapter",
  src = cms.InputTag("dimuons")
)

vgMuonsGlobal = cms.EDProducer("ZMuMuAdapter",
  src = cms.InputTag("dimuonsGlobal")
)

vgMuonsOneStandAloneMuon = cms.EDProducer("ZMuMuAdapter",
  src = cms.InputTag("dimuonsOneStandAloneMuon")
)

