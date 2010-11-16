#!/usr/bin/env python

from TauAnalysis.Configuration.makeYieldPlot import makeYieldPlot
from TauAnalysis.Configuration.userRegistry import getHarvestingFilePath

import os

#inputFileName = getHarvestingFilePath('AHtoMuTau') + '/' + "plotsAHtoMuTau_all.root"
inputFileName = "/data1/friis/Run17/plotsAHtoMuTau_all.root"
inputFileName.replace('//', '/')

dqmDirectory = 'harvested/data/ahMuTauAnalyzer_woBtag/FilterStatistics/evtSelNonCentralJetEt20bTag/events_passed_cumulative/'

makeYieldPlot(inputFileName = inputFileName, dqmDirectory = dqmDirectory, outputFileName = "plotAHtoMuTau_woBtag_yield.png")
