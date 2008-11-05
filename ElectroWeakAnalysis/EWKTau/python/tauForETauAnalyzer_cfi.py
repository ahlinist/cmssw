import FWCore.ParameterSet.Config as cms

analyzeTauForETau = cms.EDAnalyzer("ETauTauAnalyzer",

  TauInputTags = cms.VInputTag(
                                "selectorPfTausForETauKinEff",
                                "selectorPfTausForETauLdgTrkEff",
                                "selectorPfTausForETauLdgTrkPtEff",
                                "selectorPfTausForETauEclIsodEff",
                                "selectorPfTausForETauTrkIsodEff",
                                "selectorPfTausForETauElecRejEff",
                                "selectorPfTausForETauProngEff"
                              ),
  FinalElecTag = cms.InputTag("selectIpAfterIsoElecForETau"),
  RefInputTags = cms.VInputTag(""),

  DoTauIdAfterElecId = cms.bool(True),
  DoMatching = cms.bool(False),
  DoTuning = cms.bool(True),
  EventWeight = cms.double(1.)
)
