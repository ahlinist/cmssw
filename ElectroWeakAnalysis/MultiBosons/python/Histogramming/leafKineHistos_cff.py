import FWCore.ParameterSet.Config as cms

ptHisto = cms.PSet(
  itemsToPlot = cms.untracked.int32(-1),
  min = cms.untracked.double(0),
  max = cms.untracked.double(300),
  nbins = cms.untracked.int32(300),
  name = cms.untracked.string("Pt"),
  description = cms.untracked.string("P_{#perp}   [GeV/c]"),
  plotquantity = cms.untracked.string("pt")
)

etaHisto = cms.PSet(
  itemsToPlot = cms.untracked.int32(-1),
  min = cms.untracked.double(-10.0),
  max = cms.untracked.double(10.0),
  nbins = cms.untracked.int32(200),
  name = cms.untracked.string("Eta"),
  description = cms.untracked.string("#eta"),
  plotquantity = cms.untracked.string("eta")
)

phiHisto = cms.PSet(
  itemsToPlot = cms.untracked.int32(-1),
  min = cms.untracked.double(-4.0),
  max = cms.untracked.double(4.0),
  nbins = cms.untracked.int32(80),
  name = cms.untracked.string("Phi"),
  description = cms.untracked.string("#phi"),
  plotquantity = cms.untracked.string("phi")
)

leafKineHistos = cms.VPSet(ptHisto, etaHisto, phiHisto)

## Add tab-completion during the inspection
if __name__ == "__main__":
  import user
