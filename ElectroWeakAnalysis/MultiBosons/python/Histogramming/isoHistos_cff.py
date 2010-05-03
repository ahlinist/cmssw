import FWCore.ParameterSet.Config as cms

trackIsoHisto = cms.PSet(
  itemsToPlot = cms.untracked.int32(-1),
  min = cms.untracked.double(0),
  max = cms.untracked.double(100),
  nbins = cms.untracked.int32(100),
  name = cms.untracked.string("trackIso"),
  description = cms.untracked.string("tracker isolation [GeV]"),
  plotquantity = cms.untracked.string("trackIso")
)

ecalIsoHisto = cms.PSet(
  itemsToPlot = cms.untracked.int32(-1),
  min = cms.untracked.double(0),
  max = cms.untracked.double(100),
  nbins = cms.untracked.int32(100),
  name = cms.untracked.string("ecalIso"),
  description = cms.untracked.string("ECAL isolation [GeV]"),
  plotquantity = cms.untracked.string("ecalIso")
)

hcalIsoHisto = cms.PSet(
  itemsToPlot = cms.untracked.int32(-1),
  min = cms.untracked.double(0),
  max = cms.untracked.double(100),
  nbins = cms.untracked.int32(100),
  name = cms.untracked.string("hcalIso"),
  description = cms.untracked.string("HCAL isolation [GeV]"),
  plotquantity = cms.untracked.string("hcalIso")
)

isoHistos = cms.VPSet(trackIsoHisto, ecalIsoHisto, hcalIsoHisto)

## Add tab-completion during the inspection
if __name__ == "__main__":
  import user


