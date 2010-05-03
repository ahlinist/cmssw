import FWCore.ParameterSet.Config as cms

dau0MassHisto = cms.PSet(
  itemsToPlot = cms.untracked.int32(-1),
  min = cms.untracked.double(0),
  max = cms.untracked.double(200),
  nbins = cms.untracked.int32(200),
  name = cms.untracked.string("dau0Mass"),
  description = cms.untracked.string("Daughter 0 Mass [GeV/c^{2}]"),
  plotquantity = cms.untracked.string("daughter(0).mass")
)

from ElectroWeakAnalysis.MultiBosons.Histogramming.histoTools_cff import *
from ElectroWeakAnalysis.MultiBosons.Histogramming.compositeKineHistos_cff import *

mmgHistos = makeHistoAnalyzer("CandViewHistoAnalyzer",
  histos = compositeKineHistos + [dau0MassHisto],
  srcName = "MuMuGammasGlobal",
)
