#!/usr/bin/env python

from TauAnalysis.Configuration.recoSampleDefinitionsAHtoMuTau_7TeV_grid_cfi import recoSampleDefinitionsAHtoMuTau_7TeV
from TauAnalysis.Configuration.harvestAnalysisResults_grid import harvestAnalysisResults
import TauAnalysis.Configuration.userRegistry as reg

channel = 'AHtoMuTau'
#reg.overrideJobId(channel, 'Run33')
analysisFilePath = reg.getAnalysisFilePath(channel)
harvestingFilePath = reg.getHarvestingFilePath(channel)
jobId = reg.getJobId(channel)
tmpFilePath = reg.getTmpFilePath(channel)

harvestAnalysisResults(channel = channel, samples = recoSampleDefinitionsAHtoMuTau_7TeV,
                       inputFilePath = analysisFilePath, outputFilePath = harvestingFilePath, jobId = jobId,
                       tmpFilePath = tmpFilePath)
