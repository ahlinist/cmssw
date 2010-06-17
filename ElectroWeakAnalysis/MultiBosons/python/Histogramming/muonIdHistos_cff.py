import FWCore.ParameterSet.Config as cms

normChi2Histo = cms.PSet(
  itemsToPlot = cms.untracked.int32(-1),
  min = cms.untracked.double(0),
  max = cms.untracked.double(100),
  nbins = cms.untracked.int32(100),
  name = cms.untracked.string("normChi2"),
  description = cms.untracked.string("normalized #chi^{2}"),
  plotquantity = cms.untracked.string("normChi2")
)

dbHisto = cms.PSet(
  itemsToPlot = cms.untracked.int32(-1),
  min = cms.untracked.double(0),
  max = cms.untracked.double(100),
  nbins = cms.untracked.int32(100),
  name = cms.untracked.string("dB"),
  description = cms.untracked.string("dB"),
  plotquantity = cms.untracked.string("dB")
)

edbHisto = cms.PSet(
  itemsToPlot = cms.untracked.int32(-1),
  min = cms.untracked.double(0),
  max = cms.untracked.double(100),
  nbins = cms.untracked.int32(100),
  name = cms.untracked.string("edB"),
  description = cms.untracked.string("edB"),
  plotquantity = cms.untracked.string("edB")
)

numberOfValidHitsHisto = cms.PSet(
  itemsToPlot = cms.untracked.int32(-1),
  min = cms.untracked.double(0),
  max = cms.untracked.double(100),
  nbins = cms.untracked.int32(100),
  name = cms.untracked.string("numberOfValidHits"),
  description = cms.untracked.string("number of valid hits"),
  plotquantity = cms.untracked.string("numberOfValidHits")
)

muonIdHistos = cms.VPSet(normChi2Histo,
  dbHisto,
  edbHisto,
  numberOfValidHitsHisto
)

## Add tab-completion during the inspection
if __name__ == "__main__":
  import user


