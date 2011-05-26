#!/usr/bin/env python


from TauAnalysis.Configuration.recoSampleDefinitionsZtoElecTau_grid_cfi import recoSampleDefinitionsZtoElecTau
from TauAnalysis.Configuration.submitAnalysisToGrid import submitAnalysisToGrid
from TauAnalysis.Configuration.userRegistry import getAnalysisFilePath, getJobId

channel = 'ZtoElecTau_bgEstTemplate'
configFile = 'runBgEstPatTupleProductionZtoElecTau_cfg.py'
outputFilePath = getAnalysisFilePath(channel)
jobId = getJobId(channel)



## samplesToAnalyze = [
##     'data_TauPlusElec_Run2011A_PR_T3'
##     'Ztautau_powheg_T3',
##     'DYtautauM10to20_powheg_T3',
##     'Zee_powheg_T3',
##     'DYeeM10to20_pythia_T3',
##     'PhotonPlusJets_Pt15to30_T3','PhotonPlusJets_Pt30to50_T3','PhotonPlusJets_Pt50to80_T3',
##     'QCD_BCtoE_Pt20to30_T3','QCD_BCtoE_Pt30to80_T3','QCD_BCtoE_Pt80to170_T3',
##     'QCD_EM_Pt20to30_T3','QCD_EM_Pt30to80_T3','QCD_EM_Pt80to170_T3',
##     'TTplusJets_madgraph_T3',
##     'WplusJets_madgraph_T3',
##     'WW_T3','WZ_T3','ZZ_T3',
## ]

# Submit analysis jobs to the grid;
# disable estimation of systematic uncertainties for all samples except Ztautau
submitAnalysisToGrid(configFile = configFile, channel = channel,
                     samples = recoSampleDefinitionsZtoElecTau, outputFilePath = outputFilePath, jobId = jobId,
                    # samplesToAnalyze = samplesToAnalyze,
                     outsideCERN = True,
                     useCastor = False,
                     disableFactorization = True, disableSysUncertainties = True, disableZrecoilCorrections = True)
