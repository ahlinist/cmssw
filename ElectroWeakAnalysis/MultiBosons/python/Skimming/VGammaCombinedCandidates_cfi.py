import FWCore.ParameterSet.Config as cms

dimuons = cms.EDProducer("CandViewShallowClonePtrCombiner",
    checkCharge = cms.bool(False),
    cut = cms.string("mass > 0"), ## dummy cut, real one specified below
    decay = cms.string("cleanPatMuonsTriggerMatch@+ cleanPatMuonsTriggerMatch@-"),
    roles = cms.vstring("1", "2") ## dummy roles, real ones are specified below
)

## Define all the other combined candidates
dielectrons      = dimuons.clone(decay = "cleanPatElectronsTriggerMatch@+ cleanPatElectronsTriggerMatch@-")
electronPlusMETs = dimuons.clone(decay = "cleanPatElectronsTriggerMatch patMETsPFTriggerMatch")
muonPlusMETs     = dimuons.clone(decay = "cleanPatMuonsTriggerMatch patMETsPFTriggerMatch")

WENuGammaCands  = dimuons.clone(decay = "electronPlusMETs cleanPatPhotonsTriggerMatch")
WMuNuGammaCands = dimuons.clone(decay = "muonPlusMETs cleanPatPhotonsTriggerMatch")
ZEEGammaCands   = dimuons.clone(decay = "dielectrons cleanPatPhotonsTriggerMatch")
ZMuMuGammaCands = dimuons.clone(decay = "dimuons cleanPatPhotonsTriggerMatch")
ZInvisibleGammaCands = dimuons.clone(decay = "patMETsPFTriggerMatch cleanPatPhotonsTriggerMatch")

## Specify the dauther roles
electronPlusMETs.roles = ["lepton", "MET"]
muonPlusMETs.roles     = ["lepton", "MET"]
dielectrons.roles      = ["lepton1", "lepton2"]
dimuons.roles          = ["lepton1", "lepton2"]

## Specify the cuts
electronPlusMETs.cut = "daughter('lepton').pt > 15"
muonPlusMETs.cut     = "daughter('lepton').pt > 15"
dielectrons.cut = "daughter('lepton1').pt > 15 | daughter('lepton2').pt > 15"
dimuons.cut     = "daughter('lepton1').pt > 15 | daughter('lepton2').pt > 15"

WENuGammaCands.roles  = ["leptonPlusMET", "photon"]
WMuNuGammaCands.roles = ["leptonPlusMET", "photon"]
ZEEGammaCands.roles   = ["dilepton", "photon"]
ZMuMuGammaCands.roles = ["dilepton", "photon"]
ZInvisibleGammaCands.roles = ["MET", "photon"]

WENuGammaCands.cut  = "daughter('photon').pt > 10"
WMuNuGammaCands.cut = "daughter('photon').pt > 10"
ZEEGammaCands.cut   = "daughter('photon').pt > 10"
ZMuMuGammaCands.cut = "daughter('photon').pt > 10"
ZInvisibleGammaCands.cut = """
  daughter('photon').pt > 30 &
  daughter('MET').pt > 30
  """
