import FWCore.ParameterSet.Config as cms

from ElectroWeakAnalysis.MultiBosons.Histogramming.leafKineHistos_cff import *
genHistos = leafKineHistos.copy()

pdgIdHisto = cms.PSet(
  itemsToPlot = cms.untracked.int32(-1),
  min = cms.untracked.double(-25.5),
  max = cms.untracked.double(25.5),
  nbins = cms.untracked.int32(51),
  name = cms.untracked.string("PdgId"),
  description = cms.untracked.string("PDG id"),
  plotquantity = cms.untracked.string("pdgId")
)

statusHisto = cms.PSet(
  itemsToPlot = cms.untracked.int32(-1),
  min = cms.untracked.double(0.5),
  max = cms.untracked.double(10.5),
  nbins = cms.untracked.int32(10),
  name = cms.untracked.string("Status"),
  description = cms.untracked.string("Status"),
  plotquantity = cms.untracked.string("status")
)

genHistos += [pdgIdHisto, statusHisto]

## Add tab-completion during the inspection
if __name__ == "__main__":
  import user


