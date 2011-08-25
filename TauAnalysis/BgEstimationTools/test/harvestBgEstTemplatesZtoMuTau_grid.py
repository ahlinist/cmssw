#!/usr/bin/env python

from TauAnalysis.Configuration.recoSampleDefinitionsZtoMuTau_7TeV_grid_cfi import recoSampleDefinitionsZtoMuTau_7TeV
from TauAnalysis.Configuration.harvestAnalysisResults_grid import harvestAnalysisResults
import TauAnalysis.Configuration.userRegistry as reg 


channel = 'ZtoMuTau_bgEstTemplate'
reg.overrideJobId(channel,'Run32')
analysisFilePath = reg.getAnalysisFilePath(channel)
harvestingFilePath = reg.getHarvestingFilePath(channel)
jobId = reg.getJobId(channel)
tmpFilePath = reg.getTmpFilePath(channel)

harvestAnalysisResults(channel = channel, 
                        samples = recoSampleDefinitionsZtoMuTau_7TeV,
                       inputFilePath = analysisFilePath, outputFilePath = harvestingFilePath, jobId = jobId,
                       tmpFilePath = tmpFilePath, use_job_report = True,
					   useCastor = False)
