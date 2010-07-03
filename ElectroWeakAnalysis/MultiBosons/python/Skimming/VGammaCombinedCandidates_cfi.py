import FWCore.ParameterSet.Config as cms

dimuons = cms.EDFilter("CandViewShallowClonePtrCombiner",
    checkCharge = cms.bool(False),
    cut = cms.string("mass > 0"),
    decay = cms.string("cleanPatMuons@+ cleanPatMuons@-"),
    roles = cms.vstring("1", "2") # dummy roles to be specified below
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
dimuons.roles          = ["muon1", "muon2"]
dielectrons.roles      = ["electron1", "electron2"]
electronPlusMETs.roles = ["electron", "MET"]
muonPlusMETs.roles     = ["muon", "MET"]

WENuGammaCands.roles  = ["electronPlusMET", "photon"]
WMuNuGammaCands.roles = ["muonPlusMET", "photon"]
ZEEGammaCands.roles   = ["dielectron", "photon"]
ZMuMuGammaCands.roles = ["dimuon", "photon"]
ZInvisibleGammaCands.roles = ["MET", "photon"]
