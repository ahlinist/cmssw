import FWCore.ParameterSet.Config as cms

def setItemsToPlot(histoAnalyzer, itemsToPlot,
                   namePrefix="", titlePrefix="%d. hardest candidate ",
                   namePostfix="_%d", titlePostfix=""):
  for histo in histoAnalyzer.histograms:
    histo.itemsToPlot = itemsToPlot
    histo.name = namePrefix + histo.name.value() + namePostfix
    histo.description = titlePrefix + histo.description.value() + titlePostfix

def makeHistoAnalyzer(analyzerName, histos, srcName, itemsToPlot=-1):
  histoAnalyzer = cms.EDAnalyzer(analyzerName,
    cms.PSet(histograms = histos.copy() ),
    src = cms.InputTag(srcName)
  )
  if itemsToPlot > 0:
    setItemsToPlot(histoAnalyzer, itemsToPlot)
  return histoAnalyzer

## Add tab-completion during the inspection
if __name__ == "__main__":
  import user


