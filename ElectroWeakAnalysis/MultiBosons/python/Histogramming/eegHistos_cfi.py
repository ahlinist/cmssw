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
from ElectroWeakAnalysis.MultiBosons.Histogramming.leafKineHistos_cff import *
from ElectroWeakAnalysis.MultiBosons.Histogramming.compositeKineHistos_cff import *
import copy

e1Histos = copy.deepcopy(leafKineHistos)
for histo in e1Histos:
  histo.name = "Electron1" + histo.name.value()
  histo.description = "Electron 1 " + histo.description.value()
  histo.plotquantity = "daughter('electron1')." + histo.plotquantity.value()

e2Histos = copy.deepcopy(leafKineHistos)
for histo in e2Histos:
  histo.name = "Electron2" + histo.name.value()
  histo.description = "Electron 2 " + histo.description.value()
  histo.plotquantity = "daughter('electron2')." + histo.plotquantity.value()

eeHistos = copy.deepcopy(compositeKineHistos) + e1Histos + e2Histos
for histo in eeHistos:
  histo.name = "ee" + histo.name.value()
  histo.description = "Dielectron " + histo.description.value()
  histo.plotquantity = "daughter('dielectron')." + histo.plotquantity.value()

gHistos = copy.deepcopy(leafKineHistos)
for histo in gHistos:
  histo.name = "g" + histo.name.value()
  histo.description = "Photon " + histo.description.value()
  histo.plotquantity = "daughter('photon')." + histo.plotquantity.value()

eegHistos = makeHistoAnalyzer("CandViewHistoAnalyzer",
  histos = compositeKineHistos + [dau0MassHisto] + eeHistos + gHistos,
  srcName = "ZEEGammaCands",
)

