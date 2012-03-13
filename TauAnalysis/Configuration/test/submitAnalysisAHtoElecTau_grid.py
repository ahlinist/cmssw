#!/usr/bin/env python

from TauAnalysis.Configuration.recoSampleDefinitionsAHtoElecTau_grid_cfi import recoSampleDefinitionsAHtoElecTau
from TauAnalysis.Configuration.submitAnalysisToGrid import submitAnalysisToGrid
import TauAnalysis.Configuration.userRegistry as reg

channel = 'AHtoElecTau'
reg.overrideJobId(channel,"Run41")
jobId = reg.getJobId(channel)
configFile = 'runAHtoElecTau_cfg.py'
analysisFilePath = reg.getAnalysisFilePath(channel)

samplesToAnalyze = [
    #'Ztautau_embedded_Run2011B_PR_v1_skim',
    #'Ztautau_embedded_Run2011A_03OctReReco_skim',
    #'Ztautau_embedded_Run2011A_05AugReReco_skim',
    #'Ztautau_embedded_Run2011A_May10ReReco_skim',
    #'Ztautau_embedded_Run2011A_PR_v4_skim'
    #'data_TauPlusX_Run2011A_May10ReReco_skim',
    #'data_TauPlusX_Run2011A_PR_v4_skim',
    #'data_TauPlusX_Run2011A_05AugReReco_skim',
    #'data_TauPlusX_Run2011A_03OctReReco_skim',
    #'data_TauPlusX_Run2011B_PR_v1_skim',
    #'Ztautau_powheg_skim',
    #'Zee_powheg_skim',
    #'WplusJets_madgraph_skim',
    #'TTplusJets_madgraph_skim',
    #'WW_skim','WZ_skim',
    #'WWtoLL_skim',
    #'WZto3LNu_skim',
    #'DYtautauM10to20_powheg_skim',
    #'DYeeM10to20_pythia_skim',
    #'QCD_EM_20to30_skim',
    #'QCD_EM_30to80_skim',
    #'QCD_EM_80to170_skim',
    #'QCD_BCtoE_20to30_skim',
    'QCD_BCtoE_30to80_skim',
    #'QCD_BCtoE_80to170_skim',
    #'A90_skim',  'bbA90_skim',
    #'A100_skim', 'bbA100_skim',
    #'A120_skim', 'bbA120_skim',
    #'A130_skim', 'bbA130_skim',
    #'A140_skim', 'bbA140_skim',
    #'A160_skim', 'bbA160_skim',
    #'A180_skim', 'bbA180_skim',
    #'A200_skim', 'bbA200_skim',
    #'A250_skim', 'bbA250_skim',
    #'A300_skim', 
    #'bbA300_skim',
    #'A350_skim', 'bbA350_skim',
    #'A400_skim', 'bbA400_skim',
    #'A450_skim', 'bbA450_skim',
    #'A500_skim', 'bbA500_skim',
    #'A600_skim', 'bbA600_skim',
    #'A700_skim', 'bbA700_skim',
    #'A800_skim', 'bbA800_skim',
    #'A900_skim', 'bbA900_skim',
    #'h90_skim',
    #'VBFh95_skim', 'h95_skim',
    #'VBFh100_skim', 'h100_skim',
    #'VBFh105_skim', 'h105_skim',
    #'VBFh110_skim', 'h110_skim',
    #'VBFh115_skim', 'h115_skim',
    #'VBFh120_skim', 'h120_skim',
    #'VBFh125_skim', 'h125_skim',
    #'VBFh130_skim', 'h130_skim',
    #'VBFh135_skim', 'h135_skim',
    #'VBFh140_skim', 'h140_skim',
    #'VBFh145_skim', 'h145_skim'
]

#recoSampleDefinitionsAHtoElecTau['RECO_SAMPLES']['Zee_powheg_pat']['number_of_jobs'] = 2000
#recoSampleDefinitionsAHtoElecTau['RECO_SAMPLES']['Zee_powheg_pat']['saveFinalEvents'] = True
#recoSampleDefinitionsAHtoElecTau['RECO_SAMPLES']['Zee_powheg_pat'][''] = 

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

