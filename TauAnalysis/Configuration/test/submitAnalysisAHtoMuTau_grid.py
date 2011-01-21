#!/usr/bin/env python

from TauAnalysis.Configuration.recoSampleDefinitionsAHtoMuTau_7TeV_grid_cfi \
        import recoSampleDefinitionsAHtoMuTau_7TeV as samples

from TauAnalysis.Configuration.submitAnalysisToGrid import submitAnalysisToGrid
import TauAnalysis.Configuration.userRegistry as reg

channel = 'AHtoMuTau'
reg.overrideJobId(channel, 'Run47')

configFile = 'runAHtoMuTau_cfg.py'
analysisFilePath = reg.getAnalysisFilePath(channel)
jobId = reg.getJobId(channel)

fake_rate_samples = [sample for sample in samples['SAMPLES_TO_ANALYZE']
                     if samples['RECO_SAMPLES'][sample]['enableFakeRates']]

samplesToAnalyze = []
samplesToAnalyze = fake_rate_samples

#enableSystematics = True
enableSystematics = False

submitAnalysisToGrid(configFile = configFile, channel = 'AHtoMuTau',
                     samples = samples,
                     outputFilePath = analysisFilePath, jobId = jobId,
                     samplesToAnalyze = samplesToAnalyze,
                     disableSysUncertainties = not enableSystematics,
                     enableFakeRates = False,
                     submit = True, saveFinalEvents = True)
