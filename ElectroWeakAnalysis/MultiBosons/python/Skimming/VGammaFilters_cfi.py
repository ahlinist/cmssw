import FWCore.ParameterSet.Config as cms
## TODO: check that filters work (they seem to)
## TODO: check that lepton1 is the leading lepton

electronPlusMETFilter = cms.EDFilter("CandViewSelector",
  src = cms.InputTag("electronPlusMETs"),
  cut = cms.string("daughter('lepton').pt > 15"),
  filter = cms.bool(True),
  )

muonPlusMETFilter = electronPlusMETFilter.clone(src = "muonPlusMETs")

dielectronFilter = electronPlusMETFilter.clone(
  src = "dielectrons",
  cut = "daughter('lepton1').pt > 15 | daughter('lepton2').pt > 15"
  )

dimuonFilter = dielectronFilter.clone(src = "dimuons")

WENuGammaFilter = electronPlusMETFilter.clone(
  src = "WENuGammaCands",
  cut = "daughter('photon').pt > 10"
  )

WMuNuGammaFilter = WENuGammaFilter.clone(src = "WMuNuGammaCands")
ZEEGammaFilter   = WENuGammaFilter.clone(src = "ZEEGammaCands")
ZMuMuGammaFilter = WENuGammaFilter.clone(src = "ZMuMuGammaCands")

ZInvisibleGammaFilter = WENuGammaFilter.clone(
  src = "ZInvisibleGammaCands",
  cut = "daughter('photon').pt > 30 & daughter('MET').pt > 30"
  )
