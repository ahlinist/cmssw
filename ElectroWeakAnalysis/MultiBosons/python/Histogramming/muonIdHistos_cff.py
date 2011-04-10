import FWCore.ParameterSet.Config as cms

normChi2Histo = cms.PSet(
  itemsToPlot = cms.untracked.int32(-1),
  min = cms.untracked.double(0),
  max = cms.untracked.double(100),
  nbins = cms.untracked.int32(100),
  name = cms.untracked.string("normChi2"),
  description = cms.untracked.string("Normalized #chi^{2}"),
  plotquantity = cms.untracked.string("normChi2")
)

dbHisto = cms.PSet(
  itemsToPlot = cms.untracked.int32(-1),
  min = cms.untracked.double(0),
  max = cms.untracked.double(1),
  nbins = cms.untracked.int32(100),
  name = cms.untracked.string("dB"),
  description = cms.untracked.string("Impact Parameter w.r.t Beamline [cm];Distance [cm];N per 1 cm"),
  plotquantity = cms.untracked.string("dB")
)

edbHisto = cms.PSet(
  itemsToPlot = cms.untracked.int32(-1),
  min = cms.untracked.double(0),
  max = cms.untracked.double(1),
  nbins = cms.untracked.int32(100),
  name = cms.untracked.string("edB"),
  description = cms.untracked.string("Impact Parameter w.r.t. Vertex [cm];N per 1 cm"),
  plotquantity = cms.untracked.string("edB")
)

numberOfValidHitsHisto = cms.PSet(
  itemsToPlot = cms.untracked.int32(-1),
  min = cms.untracked.double(0),
  max = cms.untracked.double(100),
  nbins = cms.untracked.int32(100),
  name = cms.untracked.string("numberOfValidHits"),
  description = cms.untracked.string("Number of Valid Hits;N;N per Number of Hits"),
  plotquantity = cms.untracked.string("numberOfValidHits")
)

isolationR03emVetoEtHisto = cms.PSet(
  itemsToPlot = cms.untracked.int32(-1),
  min = cms.untracked.double(0),
  max = cms.untracked.double(10),
  nbins = cms.untracked.int32(100),
  name = cms.untracked.string("isolationR03emVetoEt"),
  description = cms.untracked.string("ECAL Veto [GeV];Energy [GeV];N per .1 GeV"),
  plotquantity = cms.untracked.string("isolationR03.emVetoEt")
)

isolationR03hadVetoEtHisto = cms.PSet(
  itemsToPlot = cms.untracked.int32(-1),
  min = cms.untracked.double(0),
  max = cms.untracked.double(10),
  nbins = cms.untracked.int32(100),
  name = cms.untracked.string("isolationR03hadVetoEt"),
  description = cms.untracked.string("HCAL Veto [GeV];Energy [GeV];N per .1 GeV"),
  plotquantity = cms.untracked.string("isolationR03.hadVetoEt")
)

muonIdHistos = cms.VPSet(
  normChi2Histo,
  dbHisto,
  edbHisto,
#   numberOfValidHitsHisto,
  isolationR03emVetoEtHisto,
  isolationR03hadVetoEtHisto
)

## Add tab-completion during the inspection
if __name__ == "__main__":
  import user


