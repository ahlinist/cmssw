#!/usr/bin/env python

from TauAnalysis.Configuration.recoSampleDefinitionsAHtoElecTau_grid_cfi import recoSampleDefinitionsAHtoElecTau
from TauAnalysis.Configuration.submitAnalysisToGrid import submitAnalysisToGrid
import TauAnalysis.Configuration.userRegistry as reg

channel = 'AHtoElecTau'
##reg.overrideJobId(channel,"Run25")
configFile = 'runAHtoElecTau_cfg.py'
analysisFilePath = reg.getAnalysisFilePath(channel)
jobId = reg.getJobId(channel)

samplesToAnalyze = [
    'Ztautau_powheg_T3',
    'Zee_powheg_T3',
    'WplusJets_madgraph_T3',
    'TTplusJets_madgraph_T3',
    'PhotonPlusJets_Pt15to30_T3','PhotonPlusJets_Pt30to50_T3','PhotonPlusJets_Pt50to80_T3',
    'QCD_BCtoE_Pt20to30_T3','QCD_BCtoE_Pt30to80_T3','QCD_BCtoE_Pt80to170_T3',
    'QCD_EM_Pt20to30_T3','QCD_EM_Pt30to80_T3','QCD_EM_Pt80to170_T3',
    'WW_T3','WZ_T3','ZZ_T3',
    'DYtautauM10to20_powheg_T3',
    'DYeeM10to20_pythia_T3',
    'qqZll_T3',
    'A90_T3',  'bbA90_T3',
    'A100_T3', 'bbA100_T3',
    'A120_T3', 'bbA120_T3',
    'A130_T3', 'bbA130_T3',
    'A140_T3', 'bbA140_T3',
    'A160_T3', 'bbA160_T3',
    'A180_T3', 'bbA180_T3',
    'A200_T3', 'bbA200_T3',
    'A250_T3', 'bbA250_T3',
    'A300_T3', 'bbA300_T3',
    'A350_T3', 'bbA350_T3',
    'A400_T3', 'bbA400_T3',
    'A450_T3', 'bbA450_T3',
    'A500_T3', 'bbA500_T3',
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

