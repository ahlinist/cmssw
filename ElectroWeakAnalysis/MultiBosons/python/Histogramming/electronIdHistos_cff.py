import FWCore.ParameterSet.Config as cms

sigmaIetaIetaHisto = cms.PSet(
  itemsToPlot = cms.untracked.int32(-1),
  min = cms.untracked.double(0),
  max = cms.untracked.double(0.05),
  nbins = cms.untracked.int32(100),
  name = cms.untracked.string("sigmaIetaIeta"),
  description = cms.untracked.string("#sigma_{i#etai#eta}"),
  plotquantity = cms.untracked.string("sigmaIetaIeta"),
)

dbHisto = cms.PSet(
  itemsToPlot = cms.untracked.int32(-1),
  min = cms.untracked.double(0.0),
  max = cms.untracked.double(1.0),
  nbins = cms.untracked.int32(100),
  name = cms.untracked.string("dB"),
  description = cms.untracked.string("dB"),
  plotquantity = cms.untracked.string("dB")
)

deltaPhiHisto = cms.PSet(
  itemsToPlot = cms.untracked.int32(-1),
  min = cms.untracked.double(0.0),
  max = cms.untracked.double(0.1),
  nbins = cms.untracked.int32(100),
  name = cms.untracked.string("deltaPhi"),
  description = cms.untracked.string("#Delta#phi"),
  plotquantity = cms.untracked.string("deltaPhiEleClusterTrackAtCalo")
)

deltaEtaHisto = cms.PSet(
  itemsToPlot = cms.untracked.int32(-1),
  min = cms.untracked.double(0.0),
  max = cms.untracked.double(0.1),
  nbins = cms.untracked.int32(100),
  name = cms.untracked.string("deltaEta"),
  description = cms.untracked.string("#Delta#eta"),
  plotquantity = cms.untracked.string("deltaEtaEleClusterTrackAtCalo")
)

hadronicOverEmHisto = cms.PSet(
  itemsToPlot = cms.untracked.int32(-1),
  min = cms.untracked.double(0),
  max = cms.untracked.double(1),
  nbins = cms.untracked.int32(100),
  name = cms.untracked.string("hadronicOverEm"),
  description = cms.untracked.string("H/E"),
  plotquantity = cms.untracked.string("hadronicOverEm")
)

electronIdHistos = cms.VPSet(
  sigmaIetaIetaHisto,
  dbHisto,
  deltaPhiHisto,
  deltaEtaHisto,
  hadronicOverEmHisto,
)

## Add tab-completion during the inspection
if __name__ == "__main__":
  import user


