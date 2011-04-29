#!/usr/bin/env python


from TauAnalysis.Configuration.recoSampleDefinitionsZtoElecTau_grid_cfi import recoSampleDefinitionsZtoElecTau
from TauAnalysis.Configuration.submitAnalysisToGrid import submitAnalysisToGrid
from TauAnalysis.Configuration.userRegistry import getAnalysisFilePath, getJobId

channel = 'ZtoElecTau_bgEstTemplate'
configFile = 'runBgEstTemplateProductionZtoElecTau_cfg.py'
outputFilePath = getAnalysisFilePath(channel)
jobId = getJobId(channel)


## samplesToAnalyze = [
##     # modify in case you want to submit crab jobs for some of the samples only...
##     'data_EG_Run2010A_Nov4ReReco_T3'

    
##     #'Ztautau_PU156BX_T3','Zee_PU156BX_T3',
## ]

# Submit analysis jobs to the grid;
# disable estimation of systematic uncertainties for all samples except Ztautau
submitAnalysisToGrid(configFile = configFile, channel = channel,
                     samples = recoSampleDefinitionsZtoElecTau, outputFilePath = outputFilePath, jobId = jobId,
                     #samplesToAnalyze = samplesToAnalyze,
                     outsideCERN = True,
                     useCastor = False, 
                     disableFactorization = True, disableSysUncertainties = True)
