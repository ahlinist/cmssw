import FWCore.ParameterSet.Config as cms

from ElectroWeakAnalysis.MultiBosons.Histogramming.leafKineHistos_cff import *
compositeKineHistos = leafKineHistos.copy()

rapidityHisto = cms.PSet(
  itemsToPlot = cms.untracked.int32(-1),
  min = cms.untracked.double(-6.0),
  max = cms.untracked.double(6.0),
  nbins = cms.untracked.int32(120),
  name = cms.untracked.string("Rapidity"),
  description = cms.untracked.string("Rapidity:Rapidity:N per .1"),
  plotquantity = cms.untracked.string("rapidity")
)

massHisto = cms.PSet(
  itemsToPlot = cms.untracked.int32(-1),
  min = cms.untracked.double(0),
  max = cms.untracked.double(200),
  nbins = cms.untracked.int32(200),
  name = cms.untracked.string("Mass"),
  description = cms.untracked.string("Mass [GeV/c^{2}]:M [GeV/c^{2}]:N per 1 GeV/c^{2}"),
  plotquantity = cms.untracked.string("mass")
)

mtHisto = cms.PSet(
  itemsToPlot = cms.untracked.int32(-1),
  min = cms.untracked.double(0),
  max = cms.untracked.double(200),
  nbins = cms.untracked.int32(200),
  name = cms.untracked.string("Mt"),
  description = cms.untracked.string("Transverse Mass [GeV/c^{2}]:M_{T} [GeV/c^{2}]:N per 1 GeV/c^{2}"),
  plotquantity = cms.untracked.string("mt")
)

compositeKineHistos += [rapidityHisto, massHisto, mtHisto]

## Add tab-completion during the inspection
if __name__ == "__main__":
  import user
