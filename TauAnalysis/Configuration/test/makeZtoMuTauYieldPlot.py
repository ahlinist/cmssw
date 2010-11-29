#!/usr/bin/env python

from TauAnalysis.Configuration.makeYieldPlot import makeYieldPlot
from TauAnalysis.Configuration.userRegistry import getHarvestingFilePath

import os

inputFileName = getHarvestingFilePath('ZtoMuTau') + '/' + "plotsZtoMuTau_all.root"
inputFileName.replace('//', '/')

dqmDirectory = 'harvested/data/zMuTauAnalyzer/FilterStatistics/evtSelDiMuPairZmumuHypothesisVetoByMass/events_passed_cumulative/'

makeYieldPlot(inputFileName = inputFileName, dqmDirectory = dqmDirectory, outputFileName = "plotZtoMuTau_yield.png")
