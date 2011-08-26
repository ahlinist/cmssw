#!/usr/bin/env python

from TauAnalysis.Configuration.recoSampleDefinitionsZtoElecTau_grid_cfi import recoSampleDefinitionsZtoElecTau
from TauAnalysis.Configuration.recoSampleDefinitionsAHtoElecTau_grid_cfi import recoSampleDefinitionsAHtoElecTau

from TauAnalysis.Configuration.harvestAnalysisResults_grid import harvestAnalysisResults
from TauAnalysis.Configuration.userRegistry import getAnalysisFilePath, getHarvestingFilePath, getJobId, getTmpFilePath

channel = 'ZtoElecTau_bgEstTemplate'
analysisFilePath = getAnalysisFilePath(channel)
harvestingFilePath = getHarvestingFilePath(channel)
jobId = getJobId(channel)
tmpFilePath = getTmpFilePath(channel)


harvestAnalysisResults(channel = channel, samples = recoSampleDefinitionsAHtoElecTau,
                       inputFilePath = analysisFilePath, outputFilePath = harvestingFilePath, jobId = jobId, tmpFilePath = tmpFilePath, use_job_report = True,
                       useCastor = False)
