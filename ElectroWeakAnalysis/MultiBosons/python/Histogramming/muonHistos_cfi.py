import FWCore.ParameterSet.Config as cms

from ElectroWeakAnalysis.MultiBosons.Histogramming.leafKineHistos_cff import *
from ElectroWeakAnalysis.MultiBosons.Histogramming.isoHistos_cff import *
muonHistos = cms.EDAnalyzer("PatPhotonHistoAnalyzer",
#photonHistos = cms.EDAnalyzer("LazyCandViewHistoAnalyzer",
  cms.PSet(histograms = (leafKineHistos + isoHistos).copy() ),
  src = cms.InputTag("cleanPatPhotons")
)

from ElectroWeakAnalysis.MultiBosons.Histogramming.histoTools_cff import *
setItemsToPlot(muonHistos, 5)

#makeMuonHistos = cms.Sequence(muonHistos)