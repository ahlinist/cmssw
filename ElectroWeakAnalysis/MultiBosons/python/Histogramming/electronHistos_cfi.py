import FWCore.ParameterSet.Config as cms

from ElectroWeakAnalysis.MultiBosons.Histogramming.leafKineHistos_cff import *
from ElectroWeakAnalysis.MultiBosons.Histogramming.isoHistos_cff import *
from ElectroWeakAnalysis.MultiBosons.Histogramming.electronIdHistos_cff import *
electronHistos = cms.EDAnalyzer("PatMuonHistoAnalyzer",
#photonHistos = cms.EDAnalyzer("LazyCandViewHistoAnalyzer",
  cms.PSet(histograms = (leafKineHistos + isoHistos + electronIdHistos).copy() ),
  src = cms.InputTag("cleanPatMuons")
)

from ElectroWeakAnalysis.MultiBosons.Histogramming.histoTools_cff import *
setItemsToPlot(electronHistos, 5, titlePrefix = "%d. hardest electron")

#makeMuonHistos = cms.Sequence(muonHistos)
