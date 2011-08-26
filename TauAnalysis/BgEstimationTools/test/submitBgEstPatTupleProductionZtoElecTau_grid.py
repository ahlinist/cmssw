#!/usr/bin/env python


from TauAnalysis.Configuration.recoSampleDefinitionsZtoElecTau_grid_cfi import recoSampleDefinitionsZtoElecTau
from TauAnalysis.Configuration.recoSampleDefinitionsAHtoElecTau_grid_cfi import recoSampleDefinitionsAHtoElecTau
from TauAnalysis.Configuration.submitAnalysisToGrid import submitAnalysisToGrid
from TauAnalysis.Configuration.userRegistry import getAnalysisFilePath, getJobId

channel = 'ZtoElecTau_bgEstTemplate'
configFile = 'runBgEstPatTupleProductionZtoElecTau_cfg.py'
outputFilePath = getAnalysisFilePath(channel)
jobId = getJobId(channel)




# Submit analysis jobs to the grid;
# disable estimation of systematic uncertainties for all samples except Ztautau
submitAnalysisToGrid(configFile = configFile, channel = channel,
                     samples = recoSampleDefinitionsZtoElecTau, outputFilePath = outputFilePath, jobId = jobId,
                    # samplesToAnalyze = samplesToAnalyze,
                     outsideCERN = True,
                     useCastor = False,
                     disableFactorization = True, disableSysUncertainties = True, disableZrecoilCorrections = True)
