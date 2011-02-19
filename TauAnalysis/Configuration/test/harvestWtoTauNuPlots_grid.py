#!/usr/bin/env python

from TauAnalysis.Configuration.recoSampleDefinitionsWtoTauNu_grid_cfi import recoSampleDefinitionsWtoTauNu_grid
from TauAnalysis.Configuration.harvestAnalysisResults_grid import harvestAnalysisResults
from TauAnalysis.Configuration.userRegistry import getAnalysisFilePath, getHarvestingFilePath, getJobId, getTmpFilePath

channel = 'WtoTauNu'
analysisFilePath = getAnalysisFilePath(channel)
harvestingFilePath = getHarvestingFilePath(channel)
jobId = getJobId(channel)
tmpFilePath = getTmpFilePath(channel)

harvestAnalysisResults(channel = channel, samples = recoSampleDefinitionsWtoTauNu_grid,
                                              inputFilePath = analysisFilePath, outputFilePath = harvestingFilePath, jobId = jobId,
                                              tmpFilePath = tmpFilePath, ana_defs = None, plot_defs = None, plotters = None,
                                              use_job_report = True)
