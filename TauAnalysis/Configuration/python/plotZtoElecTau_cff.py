import FWCore.ParameterSet.Config as cms
import copy

#--------------------------------------------------------------------------------
#
# Plot histograms for Z --> e + tau-jet channel
# N. Marinelli modified the Z -> mu + tau jet from Christian to get this one
# Author: Christian Veelken, UC Davis
# 
#--------------------------------------------------------------------------------

from TauAnalysis.Configuration.plotZtoElecTau_processes_cfi import *
from TauAnalysis.Configuration.plotZtoElecTau_drawJobs_cfi import *
from TauAnalysis.DQMTools.plotterStyleDefinitions_cfi import *

loadZtoElecTau = cms.EDAnalyzer("DQMFileLoader",
    Ztautau = copy.deepcopy(processZtoElecTau_Ztautau.config_dqmFileLoader),
    gammaPlusJets_Pt15to20 = copy.deepcopy(processZtoElecTau_gammaPlusJets_Pt15to20.config_dqmFileLoader),
    gammaPlusJets_Pt20to25 = copy.deepcopy(processZtoElecTau_gammaPlusJets_Pt20to25.config_dqmFileLoader),
    gammaPlusJets_Pt25to30 = copy.deepcopy(processZtoElecTau_gammaPlusJets_Pt25to30.config_dqmFileLoader),
    gammaPlusJets_Pt30to35 = copy.deepcopy(processZtoElecTau_gammaPlusJets_Pt30to35.config_dqmFileLoader),
    gammaPlusJets_PtGt35 = copy.deepcopy(processZtoElecTau_gammaPlusJets_PtGt35.config_dqmFileLoader),
    WplusJets = copy.deepcopy(processZtoElecTau_WplusJets.config_dqmFileLoader),
    ZplusJets = copy.deepcopy(processZtoElecTau_ZplusJets.config_dqmFileLoader),
    QCD_BCtoE_Pt20to30 = copy.deepcopy(processZtoElecTau_QCD_BCtoE_Pt20to30.config_dqmFileLoader),
    QCD_BCtoE_Pt30to80 = copy.deepcopy(processZtoElecTau_QCD_BCtoE_Pt20to30.config_dqmFileLoader),
    QCD_BCtoE_Pt80to170 = copy.deepcopy(processZtoElecTau_QCD_BCtoE_Pt20to30.config_dqmFileLoader),
    QCD_EMenriched_Pt20to30 = copy.deepcopy(processZtoElecTau_QCD_BCtoE_Pt20to30.config_dqmFileLoader),
    QCD_EMenriched_Pt30to80 = copy.deepcopy(processZtoElecTau_QCD_BCtoE_Pt20to30.config_dqmFileLoader),
    QCD_EMenriched_Pt80to170 = copy.deepcopy(processZtoElecTau_QCD_BCtoE_Pt20to30.config_dqmFileLoader)
)

addZtoElecTau_qcdSum = cms.EDAnalyzer("DQMHistAdder",
    qcdSum = cms.PSet(
        dqmDirectories_input = cms.vstring('QCD_BCtoE_Pt20to30',
		  			   'QCD_BCtoE_Pt30to80',
                                           'QCD_BCtoE_Pt80to170',
		  			   'QCD_EMenriched_Pt20to30',
   	  				   'QCD_EMenriched_Pt30to80',
 		  			   'QCD_EMenriched_Pt80to170'),
        dqmDirectory_output = cms.string('qcdSum')
    )                          
)

addZtoElecTau_photonJetsSum = cms.EDAnalyzer("DQMHistAdder",
    gammaPlusJetsSum = cms.PSet(
        dqmDirectories_input = cms.vstring('gammaPlusJets_Pt15to20',
                                           'gammaPlusJets_Pt20to25',
                                           'gammaPlusJets_Pt25to30',
                                           'gammaPlusJets_Pt30to35',
                                           'gammaPlusJets_PtGt35'),
        dqmDirectory_output = cms.string('gammaPlusJetsSum')
    )                          
)

addZtoElecTau_smSum = cms.EDAnalyzer("DQMHistAdder",
    smSum = cms.PSet(
        dqmDirectories_input = cms.vstring( 'Ztautau',
                                            'Zee'
                                            'photonJetsSum',
                                            'WplusJets',
                                            'ZplusJets',
                                            'qcdSum' 
									  ),
        dqmDirectory_output = cms.string('smSum')
    )
)

addZtoElecTau = cms.Sequence(addZtoElecTau_qcdSum + addZtoElecTau_smSum)

plotZtoElecTau = cms.EDAnalyzer("DQMHistPlotter",
    processes = cms.PSet(
        Ztautau = copy.deepcopy(processZtoElecTau_Ztautau.config_dqmHistPlotter),
        Zee = copy.deepcopy(processZtoElecTau_Zee.config_dqmHistPlotter),
        gammaPlusJetsSum = cms.PSet(
            dqmDirectory = cms.string('gammaPlusJetsSum'),
            legendEntry = cms.string('#gamma + Jets'),
            type = cms.string('smMC') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
        ),
        WplusJets = copy.deepcopy(processZtoElecTau_WplusJets.config_dqmHistPlotter),
        ZplusJets = copy.deepcopy(processZtoElecTau_ZplusJets.config_dqmHistPlotter),
        qcdSum = cms.PSet(
            dqmDirectory = cms.string('qcdSum'),
            legendEntry = cms.string('QCD'),
            type = cms.string('smMC') # 'Data' / 'smMC' / 'bsmMC' / 'smSumMC'
        )
    ),

    xAxes = cms.PSet(
        Pt = copy.deepcopy(xAxis_pt),
        Eta = copy.deepcopy(xAxis_eta),
        Phi = copy.deepcopy(xAxis_phi),
        IPxy = copy.deepcopy(xAxis_ipXY),
        IPz = copy.deepcopy(xAxis_ipZ),
        dPhi = copy.deepcopy(xAxis_dPhi),
        prob = copy.deepcopy(xAxis_prob),
        posZ = copy.deepcopy(xAxis_posZ),
        Mt = copy.deepcopy(xAxis_transMass),
        M = copy.deepcopy(xAxis_mass),
        N = copy.deepcopy(xAxis_num),
        unlabeled = copy.deepcopy(xAxis_unlabeled),
    ),

    yAxes = cms.PSet(                         
        numEntries_linear = copy.deepcopy(yAxis_numEntries_linear),
        numEntries_log = copy.deepcopy(yAxis_numEntries_log)
    ),

    legends = cms.PSet(
        regular = copy.deepcopy(legend_regular)
    ),

    labels = cms.PSet(
        mcNormScale = copy.deepcopy(label_mcNormScale)
    ),

    drawOptionSets = cms.PSet(
        default = cms.PSet(
            Ztautau = copy.deepcopy(drawOption_Ztautau),
            WplusJets = copy.deepcopy(drawOption_WplusJets),
            gammaPlusJetsSum = copy.deepcopy(drawOption_gammaPlusJets),
            qcdSum = copy.deepcopy(drawOption_QCD)
        )
    ),

    drawJobs = cms.PSet(
        # cut-flow control plots
    cutFlowControlPlots_vertexChi2Prob_afterPrimaryEventVertex = copy.deepcopy(plots_ZtoElecTau_vertexChi2Prob_afterPrimaryEventVertex),
    cutFlowControlPlots_vertexZ_afterPrimaryEventVertexQuality = copy.deepcopy(plots_ZtoElecTau_vertexZ_afterPrimaryEventVertexQuality),
    cutFlowControlPlots_electron_afterPrimaryEventVertexPosition = copy.deepcopy(plots_ZtoElecTau_electron_afterPrimaryEventVertexPosition),
    cutFlowControlPlots_electron_afterTightElectronId = copy.deepcopy(plots_ZtoElecTau_electron_afterTightElectronId),
    cutFlowControlPlots_electron_afterElectronAntiCrack = copy.deepcopy(plots_ZtoElecTau_electron_afterElectronAntiCrack),
    cutFlowControlPlots_electron_afterElectronEta = copy.deepcopy(plots_ZtoElecTau_electron_afterElectronEta),
    cutFlowControlPlots_electronTrkIso_afterElectronPt = copy.deepcopy(plots_ZtoElecTau_electronTrkIso_afterElectronPt),
    cutFlowControlPlots_electronEcalIso_afterElectronTrkIso = copy.deepcopy(plots_ZtoElecTau_electronEcalIso_afterElectronTrkIso),
    cutFlowControlPlots_electron_afterElectronEcalIso = copy.deepcopy(plots_ZtoElecTau_electron_afterElectronEcalIso),
    cutFlowControlPlots_electronTrkIP_afterElectronTrk = copy.deepcopy(plots_ZtoElecTau_electronTrkIP_afterElectronTrk),
    cutFlowControlPlots_tau_afterElectronTrkIP = copy.deepcopy(plots_ZtoElecTau_tau_afterElectronTrkIP),
    cutFlowControlPlots_tauLeadTrkPt_afterElectronTrkIP = copy.deepcopy(plots_ZtoElecTau_tauLeadTrkPt_afterElectronTrkIP),
    cutFlowControlPlots_tau_afterTauAntiOverlapWithElectronsVeto = copy.deepcopy(plots_ZtoElecTau_tau_afterTauAntiOverlapWithElectronsVeto),
    cutFlowControlPlots_tauLeadTrkPt_afterTauAntiOverlapWithElectronsVeto = copy.deepcopy(plots_ZtoElecTau_tauLeadTrkPt_afterTauAntiOverlapWithElectronsVeto),
    cutFlowControlPlots_tau_afterTauEta = copy.deepcopy(plots_ZtoElecTau_tau_afterTauEta),
    cutFlowControlPlots_tauLeadTrkPt_afterTauEta = copy.deepcopy(plots_ZtoElecTau_tauLeadTrkPt_afterTauEta),
    cutFlowControlPlots_tau_afterTauPt = copy.deepcopy(plots_ZtoElecTau_tau_afterTauPt),
    cutFlowControlPlots_tauLeadTrkPt_afterTauPt = copy.deepcopy(plots_ZtoElecTau_tauLeadTrkPt_afterTauPt),
    cutFlowControlPlots_tau_afterTauLeadTrk = copy.deepcopy(plots_ZtoElecTau_tau_afterTauLeadTrk),
    cutFlowControlPlots_tauLeadTrkPt_afterTauLeadTrk = copy.deepcopy(plots_ZtoElecTau_tauLeadTrkPt_afterTauLeadTrk),
    cutFlowControlPlots_tau_afterTauLeadTrkPt = copy.deepcopy(plots_ZtoElecTau_tau_afterTauLeadTrkPt),
    cutFlowControlPlots_tau_afterTauTrkIso = copy.deepcopy(plots_ZtoElecTau_tau_afterTauTrkIso),
    cutFlowControlPlots_tauNumTracksSignalCone_afterTauEcalIso = copy.deepcopy(plots_ZtoElecTau_tauNumTracksSignalCone_afterTauEcalIso),
    cutFlowControlPlots_tau_afterTauProng = copy.deepcopy(plots_ZtoElecTau_tau_afterTauProng),
    cutFlowControlPlots_tauAntiElectronDiscr_afterTauProng = copy.deepcopy(plots_ZtoElecTau_tauAntiElectronDiscr_afterTauProng),
    cutFlowControlPlots_dR12_afterTauElectronVeto = copy.deepcopy(plots_ZtoElecTau_dR12_afterTauElectronVeto),
    cutFlowControlPlots_diTauCharge_afterAntiOverlapVeto = copy.deepcopy(plots_ZtoElecTau_diTauCharge_afterAntiOverlapVeto),
    cutFlowControlPlots_mtElectronMET_afterZeroCharge = copy.deepcopy(plots_ZtoElecTau_mtElectronMET_afterZeroCharge),
        
    # distributions plotted for events passing all cuts
    finalSamplePlots_electron = copy.deepcopy(plots_ZtoElecTau_electron_finalEventSample),
    finalSamplePlots_tau = copy.deepcopy(plots_ZtoElecTau_tau_finalEventSample),
    finalSamplePlots_tauLeadTrkPt = copy.deepcopy(plots_ZtoElecTau_tauLeadTrkPt_finalEventSample),
    finalSamplePlots_tauNumTracksSignalCone = copy.deepcopy(plots_ZtoElecTau_tauNumTracksSignalCone_finalEventSample),
    finalSamplePlots_met = copy.deepcopy(plots_ZtoElecTau_met_finalEventSample),
    finalSamplePlots_mtElectronMET = copy.deepcopy(plots_ZtoElecTau_mtElectronMET_finalEventSample),
    finalSamplePlots_mtTauMET = copy.deepcopy(plots_ZtoElecTau_mtTauMET_finalEventSample),
    finalSamplePlots_mtElectronTauMET = copy.deepcopy(plots_ZtoElecTau_mtElectronTauMET_finalEventSample),
    finalSamplePlots_mCDFmethod = copy.deepcopy(plots_ZtoElecTau_mCDFmethod_finalEventSample),
    finalSamplePlots_mCollApprox = copy.deepcopy(plots_ZtoElecTau_mCollApprox_finalEventSample),
 #   finalSamplePlots_numCentralJets = copy.deepcopy(plots_ZtoElecTau_numCentralJets_finalEventSample)
    

    ),

    canvasSizeX = cms.int32(800),
    canvasSizeY = cms.int32(640),                         

    outputFilePath = cms.string('./plots/'),
    #outputFileName = cms.string('plotsZtoElecTau.ps')
#    indOutputFileName = cms.string('plotZtoElecTau_#PLOT#.png')
    indOutputFileName = cms.string('plotZtoElecTau_#PLOT#.ps')
)

saveZtoElecTau = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string('plotsZtoElecTau_all.root')
)


  
