#!/usr/bin/env python

from TauAnalysis.Configuration.recoSampleDefinitionsAHtoElecTau_grid_cfi import recoSampleDefinitionsAHtoElecTau
from TauAnalysis.Configuration.harvestAnalysisResults_grid import harvestAnalysisResults
import TauAnalysis.Configuration.userRegistry as reg

channel = 'AHtoElecTau'
reg.overrideJobId(channel,"Run41")
analysisFilePath = reg.getAnalysisFilePath(channel)
harvestingFilePath = reg.getHarvestingFilePath(channel)
jobId = reg.getJobId(channel)
tmpFilePath = reg.getTmpFilePath(channel)

harvestAnalysisResults(channel = channel, samples = recoSampleDefinitionsAHtoElecTau,
                       inputFilePath = analysisFilePath, outputFilePath = harvestingFilePath, jobId = jobId,
                       tmpFilePath = tmpFilePath, ana_defs = None, plot_defs = None, plotters = None,
                       use_job_report = True,
                       useCastor = False)
