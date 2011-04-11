#!/usr/bin/env python

from TauAnalysis.Configuration.recoSampleDefinitionsAHtoMuTau_7TeV_grid_cfi \
        import recoSampleDefinitionsAHtoMuTau_7TeV as samples

from TauAnalysis.Configuration.submitAnalysisToGrid import submitAnalysisToGrid
import TauAnalysis.Configuration.userRegistry as reg

channel = 'AHtoMuTau'
reg.overrideJobId(channel, 'RunSVTestApr01')

configFile = 'runAHtoMuTau_cfg.py'
analysisFilePath = reg.getAnalysisFilePath(channel)
jobId = reg.getJobId(channel)

fake_rate_samples = [sample for sample in samples['SAMPLES_TO_ANALYZE']
                     if samples['RECO_SAMPLES'][sample]['enableFakeRates']]

samplesToAnalyze = [
    'ZtautauPU156bx_pythiaZ2',
    'A90',  'bbA90',
    'A100', 'bbA100',
    'A120', 'bbA120',
    'A130', 'bbA130',
    'A140', 'bbA140',
    'A160', 'bbA160',
    'A180', 'bbA180',
    'A200', 'bbA200',
    'A250', 'bbA250',
    'A300', 'bbA300',
    'A350', 'bbA350',
]

enableSystematics = False

submitAnalysisToGrid(configFile = configFile, channel = 'AHtoMuTau',
                     samples = samples,
                     outputFilePath = analysisFilePath, jobId = jobId,
                     samplesToAnalyze = samplesToAnalyze,
                     disableSysUncertainties = not enableSystematics,
                     enableFakeRates = False,
                     submit = True, saveFinalEvents = True)
