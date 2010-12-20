#!/usr/bin/env python

from TauAnalysis.Configuration.recoSampleDefinitionsAHtoMuTau_7TeV_grid_cfi \
        import recoSampleDefinitionsAHtoMuTau_7TeV as samples

from TauAnalysis.Configuration.submitAnalysisToGrid import submitAnalysisToGrid
import TauAnalysis.Configuration.userRegistry as reg

channel = 'AHtoMuTau'
reg.overrideJobId(channel, 'Run35')

configFile = 'runAHtoMuTau_cfg.py'
analysisFilePath = reg.getAnalysisFilePath(channel)
jobId = reg.getJobId(channel)

samplesToAnalyze = [
    # modify in case you want to submit crab jobs for some of the samples only...
]

enableSystematics = True
#enableSystematics = False

submitAnalysisToGrid(configFile = configFile, channel = 'AHtoMuTau',
                     samples = samples,
                     outputFilePath = analysisFilePath, jobId = jobId,
                     samplesToAnalyze = samplesToAnalyze,
                     disableSysUncertainties = not enableSystematics,
                     submit = True, saveFinalEvents = True)
