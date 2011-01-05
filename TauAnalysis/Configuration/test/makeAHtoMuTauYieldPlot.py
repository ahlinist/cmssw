#!/usr/bin/env python

from TauAnalysis.Configuration.makeYieldPlot import makeYieldPlot
import TauAnalysis.Configuration.userRegistry as reg
import os

reg.overrideJobId('AHtoMuTau', '2010Dec23_lxbatch')

inputFileName = os.path.join(reg.getHarvestingFilePath('AHtoMuTau'), "plotsAHtoMuTau_all.root")

dqmDirectory = '/harvested/data/ahMuTauAnalyzerOS_woBtag/FilterStatistics/' \
              + 'evtSelDiTauCandidateForAHtoMuTauZeroCharge/events_passed_cumulative/'

makeYieldPlot(inputFileName = inputFileName, dqmDirectory = dqmDirectory,
              outputFileName = "plotAHtoMuTau_woBtag_yield.png")
