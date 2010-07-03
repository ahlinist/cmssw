import FWCore.ParameterSet.Config as cms

dimuons = cms.EDFilter("CandViewShallowClonePtrCombiner",
    checkCharge = cms.bool(False),
    cut = cms.string("mass > 0"),
    decay = cms.string("cleanPatMuons@+ cleanPatMuons@-")
)

dielectrons = dimuons.clone(decay = "cleanPatElectrons@+ cleanPatElectrons@-")
electronPlusMETs = dimuons.clone(decay = "patMETs cleanPatElectrons")
muonPlusMETs     = dimuons.clone(decay = "patMETs cleanPatMuons")

WENuGammaCands  = dimuons.clone(decay = "cleanPatPhotons electronPlusMETs")
WMuNuGammaCands = dimuons.clone(decay = "cleanPatPhotons muonPlusMETs")
ZEEGammaCands   = dimuons.clone(decay = "cleanPatPhotons dielectrons")
ZMuMuGammaCands = dimuons.clone(decay = "cleanPatPhotons dimuons")
ZNuNuGammaCands = dimuons.clone(decay = "cleanPatPhotons patMETs")

