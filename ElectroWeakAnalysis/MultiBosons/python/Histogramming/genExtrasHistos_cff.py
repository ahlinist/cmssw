import FWCore.ParameterSet.Config as cms

from ElectroWeakAnalysis.MultiBosons.Histogramming.genHistos_cff import *
genExtrasHistos = genHistos.copy()

motherCountHisto = cms.PSet(
  itemsToPlot = cms.untracked.int32(-1),
  min = cms.untracked.double(-0.5),
  max = cms.untracked.double(50.5),
  nbins = cms.untracked.int32(51),
  name = cms.untracked.string("MotherCount"),
  description = cms.untracked.string("number of mothers"),
  plotquantity = cms.untracked.string("numberOfMothers")
)

motherPdgIdHisto = cms.PSet(
  itemsToPlot = cms.untracked.int32(-1),
  min = cms.untracked.double(-100.5),
  max = cms.untracked.double(100.5),
  nbins = cms.untracked.int32(201),
  name = cms.untracked.string("MotherPdgId"),
  description = cms.untracked.string("mother PDG id"),
  plotquantity = cms.untracked.string("mother(0).pdgId")
)

motherStatusHisto = cms.PSet(
  itemsToPlot = cms.untracked.int32(-1),
  min = cms.untracked.double(0.5),
  max = cms.untracked.double(10.5),
  nbins = cms.untracked.int32(10),
  name = cms.untracked.string("MotherStatus"),
  description = cms.untracked.string("Mother Status"),
  plotquantity = cms.untracked.string("mother(0).status")
)

grandmotherCountHisto = cms.PSet(
  itemsToPlot = cms.untracked.int32(-1),
  min = cms.untracked.double(-0.5),
  max = cms.untracked.double(50.5),
  nbins = cms.untracked.int32(51),
  name = cms.untracked.string("GrandmotherCount"),
  description = cms.untracked.string("number of grandmothers"),
  plotquantity = cms.untracked.string("mother(0).numberOfMothers")
)

grandmotherStatusHisto = cms.PSet(
  itemsToPlot = cms.untracked.int32(-1),
  min = cms.untracked.double(0.5),
  max = cms.untracked.double(10.5),
  nbins = cms.untracked.int32(10),
  name = cms.untracked.string("GrandmotherStatus"),
  description = cms.untracked.string("Gandmother Status"),
  plotquantity = cms.untracked.string("mother(0).mother(0).status")
)

grandmotherPdgIdHisto = cms.PSet(
  itemsToPlot = cms.untracked.int32(-1),
  min = cms.untracked.double(-100.5),
  max = cms.untracked.double(100.5),
  nbins = cms.untracked.int32(201),
  name = cms.untracked.string("GrandmotherPdgId"),
  description = cms.untracked.string("grandmother PDG id"),
  plotquantity = cms.untracked.string("mother(0).mother(0).pdgId")
)

sisterCountHisto = cms.PSet(
  itemsToPlot = cms.untracked.int32(-1),
  min = cms.untracked.double(-0.5),
  max = cms.untracked.double(50.5),
  nbins = cms.untracked.int32(51),
  name = cms.untracked.string("SistersCount"),
  description = cms.untracked.string("number of sisters"),
  plotquantity = cms.untracked.string("mother(0).numberOfDaughters")
)

sisterPdgIdHisto = cms.PSet(
  itemsToPlot = cms.untracked.int32(-1),
  min = cms.untracked.double(-100.5),
  max = cms.untracked.double(100.5),
  nbins = cms.untracked.int32(201),
  name = cms.untracked.string("SisterPdgId"),
  description = cms.untracked.string("sister PDG id"),
  plotquantity = cms.untracked.string("mother(0).daughter(0).pdgId")
)

genExtrasHistos += [
    motherCountHisto, motherPdgIdHisto, motherStatusHisto,
    grandmotherCountHisto, grandmotherPdgIdHisto, grandmotherStatusHisto,
    sisterCountHisto, sisterPdgIdHisto
]

## Add tab-completion during the inspection
if __name__ == "__main__":
  import user
