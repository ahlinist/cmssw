#!/usr/bin/env python

from TauAnalysis.Configuration.recoSampleDefinitionsZtoElecTau_grid_cfi import recoSampleDefinitionsZtoElecTau
from TauAnalysis.Configuration.submitAnalysisToGrid import submitAnalysisToGrid
from TauAnalysis.Configuration.userRegistry import getAnalysisFilePath, getJobId

channel = 'ZtoElecTau'
configFile = 'runZtoElecTau_cfg.py'
analysisFilePath = getAnalysisFilePath(channel)
jobId = getJobId(channel)

samplesToAnalyze = [
	'data_EG_Run2010A_Nov4ReReco_T3',
	'data_Elec_Run2010B_Nov4ReReco_T3',
	'Ztautau_PU156BX_T3',
	'Zee_PU156BX_T3',
	'PhotonPlusJets_Pt15to30_T3','PhotonPlusJets_Pt30to50_T3','PhotonPlusJets_Pt50to80_T3',
	'QCD_BCtoE_Pt20to30_PU156BX_T3',
	'QCD_BCtoE_Pt30to80_PU156BX_T3',
	'QCD_BCtoE_Pt80to170_PU156BX_T3',
	'QCD_EM_Pt20to30_PU156BX_T3',
	'QCD_EM_Pt30to80_PU156BX_T3',
	'QCD_EM_Pt80to170_PU156BX_T3',
	'TTbar_PU156BX_T3',
	'WtoENu_PU156BX_T3',
	'WtoTauNu_PU156BX_T3'
]

enableSystematics = True
#enableSystematics = False

submitAnalysisToGrid(configFile = configFile, channel = channel,
                     samples = recoSampleDefinitionsZtoElecTau, 
					 outputFilePath = analysisFilePath, jobId = jobId,
                     samplesToAnalyze = samplesToAnalyze, 
					 disableSysUncertainties = not enableSystematics,
					 submit = True, saveFinalEvents = False)
