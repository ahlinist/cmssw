import FWCore.ParameterSet.Config as cms

dimuonFilter   = cms.EDFilter("CandViewCountFilter",
  src = cms.InputTag("dimuons"),
  minNumber = cms.uint32(1)
  )

electronPlusMETFilter = dimuonFilter.clone(src = "electronPlusMETs")
muonPlusMETFilter     = dimuonFilter.clone(src = "muonPlusMETs")
dielectronFilter      = dimuonFilter.clone(src = "dielectrons")

WENuGammaFilter       = dimuonFilter.clone(src = "WENuGammaCands")
WMuNuGammaFilter      = dimuonFilter.clone(src = "WMuNuGammaCands")
ZEEGammaFilter        = dimuonFilter.clone(src = "ZEEGammaCands")
ZMuMuGammaFilter      = dimuonFilter.clone(src = "ZMuMuGammaCands")
ZInvisibleGammaFilter = dimuonFilter.clone(src = "ZInvisibleGammaCands")
