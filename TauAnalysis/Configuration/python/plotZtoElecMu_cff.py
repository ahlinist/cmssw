import FWCore.ParameterSet.Config as cms
import copy

#--------------------------------------------------------------------------------
#
# Plot histograms for Z --> e + mu channel
#
# Author: Christian Veelken, UC Davis
#
#--------------------------------------------------------------------------------

from TauAnalysis.Configuration.plotZtoElecMu_processes_cfi import *
from TauAnalysis.Configuration.plotZtoElecMu_drawJobs_cfi import *
from TauAnalysis.DQMTools.plotterStyleDefinitions_cfi import *

loadZtoElecMu = cms.EDAnalyzer("DQMFileLoader",
    Ztautau = copy.deepcopy(processZtoElecMu_Ztautau.config_dqmFileLoader),
    Zee = copy.deepcopy(processZtoElecMu_Zee.config_dqmFileLoader),
    Zmumu = copy.deepcopy(processZtoElecMu_Zmumu.config_dqmFileLoader),                         
    WplusJets = copy.deepcopy(processZtoElecMu_WplusJets.config_dqmFileLoader),
    QCD_BCtoE_Pt20to30 = copy.deepcopy(processZtoElecMu_QCD_BCtoE_Pt20to30.config_dqmFileLoader),
    QCD_BCtoE_Pt30to80 = copy.deepcopy(processZtoElecMu_QCD_BCtoE_Pt30to80.config_dqmFileLoader),
    InclusivePPmuX = copy.deepcopy(processZtoElecMu_InclusivePPmuX.config_dqmFileLoader),
    PPmuXptGt20 = copy.deepcopy(processZtoElecMu_PPmuXptGt20.config_dqmFileLoader)                
)

addZtoElecMu_qcdSum = cms.EDAnalyzer("DQMHistAdder",
    qcdSum = cms.PSet(
        dqmDirectories_input = cms.vstring( 'QCD_BCtoE_Pt20to30',
                                            'QCD_BCtoE_Pt30to80' ),
        dqmDirectory_output = cms.string('qcdSum')
    )
    #qcdSum = cms.PSet(
    #    dqmDirectories_input = cms.vstring('InclusivePPmuX',
    #                                       'PPmuXptGt20'),
    #    dqmDirectory_output = cms.string('qcdSum')
    #)                          
)

addZtoElecMu_smSum = cms.EDAnalyzer("DQMHistAdder",
    smSum = cms.PSet(
        dqmDirectories_input = cms.vstring( 'Ztautau',
                                            'Zee',
                                            'Zmumu',
                                            'WplusJets',
                                            'qcdSum' ),
        dqmDirectory_output = cms.string('smSum')
    )
)

addZtoElecMu = cms.Sequence(addZtoElecMu_qcdSum + addZtoElecMu_smSum)

plotZtoElecMu = cms.EDAnalyzer("DQMHistPlotter",
    processes = cms.PSet(
        Ztautau = copy.deepcopy(processZtoElecMu_Ztautau.config_dqmHistPlotter),
        Zee = copy.deepcopy(processZtoElecMu_Zee.config_dqmHistPlotter),
        Zmumu = copy.deepcopy(processZtoElecMu_Zmumu.config_dqmHistPlotter),                         
        WplusJets = copy.deepcopy(processZtoElecMu_WplusJets.config_dqmHistPlotter),
        QCD_BCtoE_Pt20to30 = copy.deepcopy(processZtoElecMu_QCD_BCtoE_Pt20to30.config_dqmHistPlotter),
        QCD_BCtoE_Pt30to80 = copy.deepcopy(processZtoElecMu_QCD_BCtoE_Pt30to80.config_dqmHistPlotter),
        InclusivePPmuX = copy.deepcopy(processZtoElecMu_InclusivePPmuX.config_dqmHistPlotter),
        PPmuXptGt20 = copy.deepcopy(processZtoElecMu_PPmuXptGt20.config_dqmHistPlotter),
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
            Zee = copy.deepcopy(drawOption_Zee),
            Zmumu = copy.deepcopy(drawOption_Zmumu),
            WplusJets = copy.deepcopy(drawOption_WplusJets),
            qcdSum = copy.deepcopy(drawOption_QCD)
        )
    ),

    drawJobs = cms.PSet(
        # cut-flow control plots
        cutFlowControlPlots_vertexChi2Prob_afterPrimaryEventVertex = copy.deepcopy(plots_ZtoElecMu_vertexChi2Prob_afterPrimaryEventVertex),
        cutFlowControlPlots_vertexZ_afterPrimaryEventVertexQuality = copy.deepcopy(plots_ZtoElecMu_vertexZ_afterPrimaryEventVertexQuality),
        cutFlowControlPlots_electron_afterPrimaryEventVertexPosition = copy.deepcopy(plots_ZtoElecMu_electron_afterPrimaryEventVertexPosition),
        cutFlowControlPlots_electron_afterTightElectronId = copy.deepcopy(plots_ZtoElecMu_electron_afterTightElectronId),
        cutFlowControlPlots_electron_afterElectronAntiCrack = copy.deepcopy(plots_ZtoElecMu_electron_afterElectronAntiCrack),
        cutFlowControlPlots_electron_afterElectronEta = copy.deepcopy(plots_ZtoElecMu_electron_afterElectronEta),
        cutFlowControlPlots_electronTrkIso_afterElectronPt = copy.deepcopy(plots_ZtoElecMu_electronTrkIso_afterElectronPt),
        cutFlowControlPlots_electronEcalIso_afterElectronTrkIso = copy.deepcopy(plots_ZtoElecMu_electronEcalIso_afterElectronTrkIso),
        cutFlowControlPlots_electron_afterElectronEcalIso = copy.deepcopy(plots_ZtoElecMu_electron_afterElectronEcalIso),
        cutFlowControlPlots_electronTrkIP_afterElectronTrk = copy.deepcopy(plots_ZtoElecMu_electronTrkIP_afterElectronTrk),
        cutFlowControlPlots_muon_afterElectronTrackIP = copy.deepcopy(plots_ZtoElecMu_muon_afterElectronTrackIP),
        cutFlowControlPlots_muon_afterGlobalMuon = copy.deepcopy(plots_ZtoElecMu_muon_afterGlobalMuon),
        cutFlowControlPlots_muon_afterMuonEta = copy.deepcopy(plots_ZtoElecMu_muon_afterMuonEta),
        cutFlowControlPlots_muonTrkIso_afterMuonPt = copy.deepcopy(plots_ZtoElecMu_muonTrkIso_afterMuonPt),
        cutFlowControlPlots_muonEcalIso_afterMuonTrkIso = copy.deepcopy(plots_ZtoElecMu_muonEcalIso_afterMuonTrkIso),
        cutFlowControlPlots_muonComp_afterMuonEcalIso = copy.deepcopy(plots_ZtoElecMu_muonComp_afterMuonEcalIso),
        cutFlowControlPlots_muonTrkIP_afterMuonAntiPionVeto = copy.deepcopy(plots_ZtoElecMu_muonTrkIP_afterMuonAntiPionVeto),
        cutFlowControlPlots_dPhi1MET_afterMuonTrkIP = copy.deepcopy(plots_ZtoElecMu_dPhi1MET_afterMuonTrkIP),
        cutFlowControlPlots_dPhi2MET_afterMuonTrkIP = copy.deepcopy(plots_ZtoElecMu_dPhi2MET_afterMuonTrkIP),
        cutFlowControlPlots_diTauCharge_afterAcoplanarity = copy.deepcopy(plots_ZtoElecMu_diTauCharge_afterAcoplanarity),
        
        # distributions plotted for events passing all cuts
        finalSamplePlots_electron = copy.deepcopy(plots_ZtoElecMu_electron_finalEventSample),
        finalSamplePlots_muon = copy.deepcopy(plots_ZtoElecMu_muon_finalEventSample),
        finalSamplePlots_met = copy.deepcopy(plots_ZtoElecMu_met_finalEventSample),
        finalSamplePlots_mtElectronMET = copy.deepcopy(plots_ZtoElecMu_mtElectronMET_finalEventSample),
        finalSamplePlots_mtMuonMET = copy.deepcopy(plots_ZtoElecMu_mtMuonMET_finalEventSample),
        finalSamplePlots_mtElectronMuonMET = copy.deepcopy(plots_ZtoElecMu_mtElectronMuonMET_finalEventSample),
        finalSamplePlots_mCDFmethod = copy.deepcopy(plots_ZtoElecMu_mCDFmethod_finalEventSample),
        finalSamplePlots_mCollApprox = copy.deepcopy(plots_ZtoElecMu_mCollApprox_finalEventSample),
        finalSamplePlots_numCentralJets = copy.deepcopy(plots_ZtoElecMu_numCentralJets_finalEventSample)
    ),

    canvasSizeX = cms.int32(800),
    canvasSizeY = cms.int32(640),                         

    outputFilePath = cms.string('./plots/'),
    #outputFileName = cms.string('plotsZtoElecMu.ps')
    indOutputFileName = cms.string('plotZtoElecMu_#PLOT#.png')
)

saveZtoElecMu = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string('plotsZtoElecMu_all.root')
)

  
