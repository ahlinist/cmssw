import FWCore.ParameterSet.Config as cms

WENuGammaFilter = cms.EDFilter("CandViewCountFilter",
    src = cms.InputTag("WENuGammaCands"),
    minNumber = cms.uint32(1)
)

WMuNuGammaFilter = WENuGammaFilter.clone(src = "WMuNuGammaCands")
ZEEGammaFilter   = WENuGammaFilter.clone(src = "ZEEGammaCands")
ZMuMuGammaFilter = WENuGammaFilter.clone(src = "ZMuMuGammaCands")
ZNuNuGammaFilter = WENuGammaFilter.clone(src = "ZNuNuGammaCands")
