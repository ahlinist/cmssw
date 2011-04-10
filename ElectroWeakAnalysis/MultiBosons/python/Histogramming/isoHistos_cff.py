import FWCore.ParameterSet.Config as cms

trackIsoHisto = cms.PSet(
  itemsToPlot = cms.untracked.int32(-1),
  min = cms.untracked.double(0),
  max = cms.untracked.double(100),
  nbins = cms.untracked.int32(100),
  name = cms.untracked.string("trackIso"),
  description = cms.untracked.string("Tracker Isolation [GeV];#Sigma p_{T} [GeV];N per 1 GeV"),
  plotquantity = cms.untracked.string("trackIso")
)

relativeTrackIsoHisto = cms.PSet(
  itemsToPlot = cms.untracked.int32(-1),
  min = cms.untracked.double(0),
  max = cms.untracked.double(2),
  nbins = cms.untracked.int32(100),
  name = cms.untracked.string("relativeTrackIso"),
  description = cms.untracked.string("Relative Tracker Isolation;(#Sigma p_{T})/p_{T};N per .02"),
  plotquantity = cms.untracked.string("trackIso/pt")
)

ecalIsoHisto = cms.PSet(
  itemsToPlot = cms.untracked.int32(-1),
  min = cms.untracked.double(0),
  max = cms.untracked.double(100),
  nbins = cms.untracked.int32(100),
  name = cms.untracked.string("ecalIso"),
  description = cms.untracked.string("ECAL Isolation [GeV];#Sigma E_{T} [GeV];N per 1 GeV"),
  plotquantity = cms.untracked.string("ecalIso")
)

relativeEcalIsoHisto = cms.PSet(
  itemsToPlot = cms.untracked.int32(-1),
  min = cms.untracked.double(0),
  max = cms.untracked.double(2),
  nbins = cms.untracked.int32(100),
  name = cms.untracked.string("relativeEcalIso"),
  description = cms.untracked.string("Relative ECAL Isolation;(#Sigma E_{T})/E_{T};N per .02"),
  plotquantity = cms.untracked.string("ecalIso/pt")
)

hcalIsoHisto = cms.PSet(
  itemsToPlot = cms.untracked.int32(-1),
  min = cms.untracked.double(0),
  max = cms.untracked.double(100),
  nbins = cms.untracked.int32(100),
  name = cms.untracked.string("hcalIso"),
  description = cms.untracked.string("HCAL Isolation [GeV];#Sigma E_{T} [GeV];N per 1 GeV"),
  plotquantity = cms.untracked.string("hcalIso")
)

relativeHcalIsoHisto = cms.PSet(
  itemsToPlot = cms.untracked.int32(-1),
  min = cms.untracked.double(0),
  max = cms.untracked.double(2),
  nbins = cms.untracked.int32(100),
  name = cms.untracked.string("relativeHcalIso"),
  description = cms.untracked.string("Relative HCAL Isolation;(#Sigma E_{T})/E_{T};N per .02"),
  plotquantity = cms.untracked.string("hcalIso/pt")
)

combinedIsoHisto = cms.PSet(
  itemsToPlot = cms.untracked.int32(-1),
  min = cms.untracked.double(0),
  max = cms.untracked.double(100),
  nbins = cms.untracked.int32(100),
  name = cms.untracked.string("combinedIso"),
  description = cms.untracked.string("Combined Isolation [GeV];#Sigma E_{T} [GeV];N per 1 GeV"),
  plotquantity = cms.untracked.string("trackIso + ecalIso + hcalIso")
)

relativeCombinedIsoHisto = cms.PSet(
  itemsToPlot = cms.untracked.int32(-1),
  min = cms.untracked.double(0),
  max = cms.untracked.double(2),
  nbins = cms.untracked.int32(100),
  name = cms.untracked.string("combinedRelativeIso"),
  description = cms.untracked.string("Combined Relative Isolation;(#Sigma E_{T})/E_{T};N per .02"),
  plotquantity = cms.untracked.string("(trackIso + ecalIso + hcalIso) / pt")
)

isoHistos = cms.VPSet(
  trackIsoHisto, ecalIsoHisto, hcalIsoHisto,
  relativeTrackIsoHisto, relativeEcalIsoHisto, relativeHcalIsoHisto,
  combinedIsoHisto, relativeCombinedIsoHisto
)

## Add tab-completion during the inspection
if __name__ == "__main__":
  import user


