import FWCore.ParameterSet.Config as cms

dimuons = cms.EDFilter("CandViewShallowClonePtrCombiner",
    checkCharge = cms.bool(False),
    cut = cms.string("mass > 0"),
    decay = cms.string("cleanPatMuons@+ cleanPatMuons@-"),
    roles = cms.vstring("1", "2") # dummy roles, real ones are specified below
)

## Define all the other combined candidates
dielectrons      = dimuons.clone(decay = "cleanPatElectrons@+ cleanPatElectrons@-")
electronPlusMETs = dimuons.clone(decay = "cleanPatElectrons patMETsPF")
muonPlusMETs     = dimuons.clone(decay = "cleanPatMuons patMETsPF")

WENuGammaCands  = dimuons.clone(decay = "electronPlusMETs cleanPatPhotons")
WMuNuGammaCands = dimuons.clone(decay = "muonPlusMETs cleanPatPhotons")
ZEEGammaCands   = dimuons.clone(decay = "dielectrons cleanPatPhotons")
ZMuMuGammaCands = dimuons.clone(decay = "dimuons cleanPatPhotons")
ZInvisibleGammaCands = dimuons.clone(decay = "patMETsPF cleanPatPhotons")

## Specify the dauther roles
dimuons.roles          = ["lepton1", "lepton2"]
dielectrons.roles      = ["lepton1", "lepton2"]
electronPlusMETs.roles = ["lepton", "MET"]
muonPlusMETs.roles     = ["lepton", "MET"]

WENuGammaCands.roles  = ["leptonPlusMET", "photon"]
WMuNuGammaCands.roles = ["leptonPlusMET", "photon"]
ZEEGammaCands.roles   = ["dilepton", "photon"]
ZMuMuGammaCands.roles = ["dilepton", "photon"]
ZInvisibleGammaCands.roles = ["MET", "photon"]
