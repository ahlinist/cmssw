#!/usr/bin/env python

from TauAnalysis.Configuration.recoSampleDefinitionsAHtoElecTau_grid_cfi import recoSampleDefinitionsAHtoElecTau
from TauAnalysis.Configuration.submitAnalysisToGrid import submitAnalysisToGrid
import TauAnalysis.Configuration.userRegistry as reg

channel = 'AHtoElecTau'
reg.overrideJobId(channel,"Run24")
configFile = 'runAHtoElecTau_cfg.py'
analysisFilePath = reg.getAnalysisFilePath(channel)
jobId = reg.getJobId(channel)

samplesToAnalyze = [
    #'data_TauPlusX_Run2011A_PR_v1',
    'data_TauPlusX_Run2011A_PR_v2',
    #'data_TauPlusX_Run2011A_PR_nonGolden',
    #'Ztautau_powheg_T3','DYtautauM10to20_powheg_T3',
    #'qqZll',
    #'DYeeM10to20_pythia_T3','Zee_powheg_T3',
    #'WplusJets_madgraph_T3',
    #'TTplusJets_madgraph_T3',
    #'WW_T3','WZ_T3','ZZ_T3',
    #'PhotonPlusJets_Pt15to30_T3','PhotonPlusJets_Pt30to50_T3','PhotonPlusJets_Pt50to80_T3',
    #'QCD_BCtoE_Pt20to30_T3','QCD_BCtoE_Pt30to80_T3','QCD_BCtoE_Pt80to170_T3',
    #'QCD_EM_Pt20to30_T3','QCD_EM_Pt30to80_T3','QCD_EM_Pt80to170_T3',
    #'A90',  'bbA90',
    #'A100', 'bbA100',
    #'A120', 'bbA120',
    #'A130_T3', 'bbA130_T3',
    #'A140', 'bbA140',
    #'A160', 'bbA160',
    #'A180', 'bbA180',
    #'A200', 'bbA200',
    #'A250', 'bbA250',
    #'A300', 'bbA300',
    #'A350', 'bbA350',
    #'A400', 'bbA400',
    #'A450', 'bbA450',
    #'A500', 'bbA500',
]

#enableSystematics = True
enableSystematics = False

submitAnalysisToGrid(configFile = configFile, channel = 'AHtoElecTau',
                     samples = recoSampleDefinitionsAHtoElecTau,
                     outputFilePath = analysisFilePath, jobId = jobId,
                     samplesToAnalyze = samplesToAnalyze,
                     disableSysUncertainties = not enableSystematics,
                     create = True,
                     submit = True,
                     saveFinalEvents=False)

