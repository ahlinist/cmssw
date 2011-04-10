import FWCore.ParameterSet.Config as cms

ptHisto = cms.PSet(
  itemsToPlot = cms.untracked.int32(-1),
  min = cms.untracked.double(0),
  max = cms.untracked.double(300),
  nbins = cms.untracked.int32(300),
  name = cms.untracked.string("Pt"),
  description = cms.untracked.string("p_{T} [GeV/c];p_{T} [GeV/c];N per 1 GeV/c"),
  plotquantity = cms.untracked.string("pt")
)

etaHisto = cms.PSet(
  itemsToPlot = cms.untracked.int32(-1),
  min = cms.untracked.double(-10.0),
  max = cms.untracked.double(10.0),
  nbins = cms.untracked.int32(100),
  name = cms.untracked.string("Eta"),
  description = cms.untracked.string("#eta;Pseudorapidity;N per 0.2"),
  plotquantity = cms.untracked.string("eta")
)

import math
phiHisto = cms.PSet(
  itemsToPlot = cms.untracked.int32(-1),
  min = cms.untracked.double(-math.pi),
  max = cms.untracked.double(math.pi),
  nbins = cms.untracked.int32(100),
  name = cms.untracked.string("Phi"),
  description = cms.untracked.string("#phi;Azimuthal Angle [radians];N per 0.063 radians"),
  plotquantity = cms.untracked.string("phi")
)

leafKineHistos = cms.VPSet(ptHisto, etaHisto, phiHisto)

## Add tab-completion during the inspection
if __name__ == "__main__":
  import user
