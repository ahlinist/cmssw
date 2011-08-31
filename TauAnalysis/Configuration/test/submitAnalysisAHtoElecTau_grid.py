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
    #'data_TauPlusX_Run2011A_May10ReReco_skim',
    #'data_TauPlusX_Run2011A_PR_v4_skim',
    #'data_TauPlusX_Run2011A_05AugReReco_skim',
    #'data_TauPlusX_Run2011A_PR_v6_skim',
    #'data_TauPlusX_Run2011A_May10ReReco_skim_qcdBgEst',
    #'Ztautau_powheg_skim',
    #'Zee_powheg_skim',
    #'WplusJets_madgraph_skim',
    #'TTplusJets_madgraph_skim',
    #'PhotonPlusJets_Pt15to30_skim','PhotonPlusJets_Pt30to50_skim','PhotonPlusJets_Pt50to80_skim',
    #'QCD_BCtoE_Pt20to30_skim','QCD_BCtoE_Pt30to80_skim','QCD_BCtoE_Pt80to170_skim',
    #'QCD_EM_Pt20to30_skim','QCD_EM_Pt30to80_skim','QCD_EM_Pt80to170_skim',
    #'WW_skim','WZ_skim','ZZ_skim',
    #'DYtautauM10to20_powheg_skim',
    #'DYeeM10to20_pythia_skim',
    #'qqZll_skim',
    #'A90_skim',  'bbA90_skim',
    #'A100_skim', 'bbA100_skim',
    #'A120_skim', 'bbA120_skim',
    #'A130_skim', 'bbA130_skim',
    #'A140_skim', 'bbA140_skim',
    #'A160_skim', 'bbA160_skim',
    #'A180_skim', 'bbA180_skim',
    #'A200_skim', 'bbA200_skim',
    #'A250_skim', 'bbA250_skim',
    #'A300_skim', 'bbA300_skim',
    #'A350_skim', 'bbA350_skim',
    #'A400_skim', 'bbA400_skim',
    #'A450_skim', 'bbA450_skim',
    #'A500_skim', 'bbA500_skim',
]

#enableSystematics = True
enableSystematics = False

submitAnalysisToGrid(configFile = configFile, channel = 'AHtoElecTau',
        samples = recoSampleDefinitionsAHtoElecTau,
        outputFilePath = analysisFilePath, jobId = jobId,
        samplesToAnalyze = samplesToAnalyze,
        disableSysUncertainties = not enableSystematics,
        outsideCERN = True,
        useCastor = False,
        create = True,
        submit = True,
        saveFinalEvents=False)

