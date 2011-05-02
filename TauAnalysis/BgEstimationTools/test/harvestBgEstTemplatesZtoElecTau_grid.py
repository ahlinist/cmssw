#!/usr/bin/env python

from TauAnalysis.Configuration.recoSampleDefinitionsZtoElecTau_grid_cfi import recoSampleDefinitionsZtoElecTau
from TauAnalysis.Configuration.harvestAnalysisResults_grid import harvestAnalysisResults
from TauAnalysis.Configuration.userRegistry import getAnalysisFilePath, getHarvestingFilePath, getJobId, getTmpFilePath

channel = 'ZtoElecTau_bgEstTemplate'
analysisFilePath = getAnalysisFilePath(channel)
harvestingFilePath = getHarvestingFilePath(channel)
jobId = getJobId(channel)
tmpFilePath = getTmpFilePath(channel)


harvestAnalysisResults(channel = channel, samples = recoSampleDefinitionsZtoElecTau,
                       inputFilePath = analysisFilePath, outputFilePath = harvestingFilePath, jobId = jobId, tmpFilePath = tmpFilePath, use_job_report = True,
                       useCastor = False)
