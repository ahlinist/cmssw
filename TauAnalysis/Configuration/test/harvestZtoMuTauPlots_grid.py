#!/usr/bin/env python

from TauAnalysis.Configuration.recoSampleDefinitionsZtoMuTau_7TeV_grid_cfi import recoSampleDefinitionsZtoMuTau_7TeV
from TauAnalysis.Configuration.harvestAnalysisResults_grid import harvestAnalysisResults
from TauAnalysis.Configuration.userRegistry import getAnalysisFilePath, getHarvestingFilePath, getJobId, getTmpFilePath

channel = 'ZtoMuTau'
analysisFilePath = getAnalysisFilePath(channel)
harvestingFilePath = getHarvestingFilePath(channel)
jobId = getJobId(channel)
tmpFilePath = getTmpFilePath(channel)

harvestAnalysisResults(channel = channel, samples = recoSampleDefinitionsZtoMuTau_7TeV,
                       inputFilePath = analysisFilePath, outputFilePath = harvestingFilePath, jobId = jobId,
                       tmpFilePath = tmpFilePath)
