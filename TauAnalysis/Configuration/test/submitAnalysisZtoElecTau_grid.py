#!/usr/bin/env python

#from TauAnalysis.Configuration.recoSampleDefinitionsZtoElecTau_grid_cfi import recoSampleDefinitionsZtoElecTau
from TauAnalysis.Configuration.recoSampleDefinitionsAHtoElecTau_grid_cfi import recoSampleDefinitionsAHtoElecTau as recoSampleDefinitionsZtoElecTau
from TauAnalysis.Configuration.submitAnalysisToGrid import submitAnalysisToGrid
from TauAnalysis.Configuration.userRegistry import getAnalysisFilePath, getJobId

channel = 'ZtoElecTau_stdCuts'
configFile = 'runZtoElecTau_stdCuts_cfg.py'
analysisFilePath = getAnalysisFilePath(channel)
jobId = getJobId(channel)

samplesToAnalyze = [
	#'data_EG_Run2010A_Nov4ReReco',
	#'data_Elec_Run2010B_Nov4ReReco',
	'Ztautau_PU156BX',
	'Ztautau_PU156BX_D6T',
	#'ZtautauEmbedded',
	'Zee_PU156BX',
	'Zee_PU156BX_D6T'
	#'PhotonPlusJets_Pt15to30','PhotonPlusJets_Pt30to50','PhotonPlusJets_Pt50to80',
	#'QCD_BCtoE_Pt20to30_PU156BX','QCD_BCtoE_Pt30to80_PU156BX','QCD_BCtoE_Pt80to170_PU156BX',
	#'QCD_BCtoE_Pt20to30',
	#'QCD_BCtoE_Pt30to80',
	#'QCD_BCtoE_Pt80to170',
	#'QCD_EM_Pt20to30',
	#'QCD_EM_Pt30to80',
	#'QCD_EM_Pt80to170'
	#'QCD_BCtoE_Pt20to30_T3',
	#'QCD_BCtoE_Pt30to80_T3',
	#'QCD_BCtoE_Pt80to170_T3',
	#'QCD_EM_Pt20to30_T3',
	#'QCD_EM_Pt30to80_T3',
	#'QCD_EM_Pt80to170_T3',
	#'QCD_EM_Pt20to30_PU156BX',
	#'QCD_EM_Pt30to80_PU156BX',
	#'QCD_EM_Pt80to170_PU156BX'
	#'TTbar',
	#'TTplusJets',
	#'WtoENu_PU156BX',
	#'WtoTauNu'
	#'A90',  'bbA90',
	#'A100', 'bbA100',
	#'A120', 'bbA120',
	#'A130', 'bbA130',
	#'A140'
	#'bbA140',
	#'A160', 'bbA160',
	#'A180', 'bbA180',
	#'A200', 'bbA200',
	#'A250', 'bbA250',
	#'A300', 'bbA300',
	#'A350', 'bbA350'
]

#enableSystematics = True
enableSystematics = False

submitAnalysisToGrid(configFile = configFile, channel = channel,
                     samples = recoSampleDefinitionsZtoElecTau, outputFilePath = analysisFilePath, jobId = jobId,
                     samplesToAnalyze = samplesToAnalyze, disableSysUncertainties = not enableSystematics,
					 create = True, submit = True, cfgdir = 'crab', inputFileMap = None, outputFileMap = None,
					 enableEventDumps = False, enableFakeRates = False, processName = None, saveFinalEvents = False)
