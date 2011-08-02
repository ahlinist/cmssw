#!/usr/bin/env python

from TauAnalysis.Configuration.recoSampleDefinitionsZtoMuTau_7TeV_grid_cfi import recoSampleDefinitionsZtoMuTau_7TeV
from TauAnalysis.Configuration.submitAnalysisToGrid import submitAnalysisToGrid
from TauAnalysis.Configuration.userRegistry import getAnalysisFilePath, getJobId

channel = 'ZtoMuTau_bgEstTemplate'
## rename cfg so crabdir name and user registry entry match 
configFile = 'run' + channel + '_cfg.py'
#configFile = 'runBgEstTemplateProductionZtoMuTau_cfg.py'
outputFilePath = getAnalysisFilePath(channel)
jobId = getJobId(channel)

samplesToAnalyze = [
    # modify in case you want to submit crab jobs for some of the samples only...
    #'data_SingleMu_Run2011A_PromptReco_v1_T3',
    'data_SingleMu_Run2011A_PromptReco_v2_T3',
    #'DYtautauM10to20_powheg',
    #'Ztautau_powheg_T3',
    #'qqZll',
    #'DYmumuM10to20_pythia',
    #'Zmumu_powheg_T3',
    #'PPmuXptGt20Mu15_T3',
    #'WplusJets_madgraph_T3',
    #'WW',
    #'WZ',
    #'ZZ'
    #'TTplusJets_madgraph_T3'
]

# Submit analysis jobs to the grid;
# disable estimation of systematic uncertainties for all samples except Ztautau
submitAnalysisToGrid(configFile = configFile, channel = channel,
                     samples = recoSampleDefinitionsZtoMuTau_7TeV, outputFilePath = outputFilePath, jobId = jobId,
                     samplesToAnalyze = samplesToAnalyze, samplesToSkip = ['Ztautau'],
                     disableFactorization = True, disableSysUncertainties = True,
                     create = True,
                     submit = True)
if samplesToAnalyze.count('Ztautau') > 0 or len(samplesToAnalyze) == 0:
    submitAnalysisToGrid(configFile = configFile, channel = channel,
                         samples = recoSampleDefinitionsZtoMuTau_7TeV, outputFilePath = outputFilePath, jobId = jobId,
                         samplesToAnalyze = ['Ztautau'], samplesToSkip = None, disableSysUncertainties = False,
                         create = True,
                         submit = True)
    
