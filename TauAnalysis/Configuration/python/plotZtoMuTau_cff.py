import FWCore.ParameterSet.Config as cms
import copy

#--------------------------------------------------------------------------------
#
# Plot histograms for Z --> mu + tau-jet channel
#
# Author: Christian Veelken, UC Davis
#
#--------------------------------------------------------------------------------

from TauAnalysis.Configuration.plotZtoMuTau_processes_cfi import *
from TauAnalysis.Configuration.plotZtoMuTau_drawJobs_cfi import *
from TauAnalysis.DQMTools.plotterStyleDefinitions_cfi import *

loadZtoMuTau = cms.EDAnalyzer("DQMFileLoader",
    Ztautau = copy.deepcopy(processZtoMuTau_Ztautau.config_dqmFileLoader),
    Zmumu = copy.deepcopy(processZtoMuTau_Zmumu.config_dqmFileLoader),
    WplusJets = copy.deepcopy(processZtoMuTau_WplusJets.config_dqmFileLoader),
    InclusivePPmuX = copy.deepcopy(processZtoMuTau_InclusivePPmuX.config_dqmFileLoader),
    PPmuXptGt20 = copy.deepcopy(processZtoMuTau_PPmuXptGt20.config_dqmFileLoader)                             
)

addZtoMuTau_qcdSum = cms.EDAnalyzer("DQMHistAdder",
    qcdSum = cms.PSet(
        dqmDirectories_input = cms.vstring('InclusivePPmuX',
                                           'PPmuXptGt20'),
        dqmDirectory_output = cms.string('qcdSum')
    )                          
)

addZtoMuTau_smSum = cms.EDAnalyzer("DQMHistAdder",
    smSum = cms.PSet(
        dqmDirectories_input = cms.vstring( 'Ztautau',
                                            'Zmumu',
                                            'WplusJets',
                                            'qcdSum' ),
        dqmDirectory_output = cms.string('smSum')
    )
)

addZtoMuTau = cms.Sequence(addZtoMuTau_qcdSum + addZtoMuTau_smSum)

plotZtoMuTau = cms.EDAnalyzer("DQMHistPlotter",
    processes = cms.PSet(
        Ztautau = copy.deepcopy(processZtoMuTau_Ztautau.config_dqmHistPlotter),
        Zmumu = copy.deepcopy(processZtoMuTau_Zmumu.config_dqmHistPlotter),
        WplusJets = copy.deepcopy(processZtoMuTau_WplusJets.config_dqmHistPlotter),
        InclusivePPmuX = copy.deepcopy(processZtoMuTau_InclusivePPmuX.config_dqmHistPlotter),
        PPmuXptGt20 = copy.deepcopy(processZtoMuTau_PPmuXptGt20.config_dqmHistPlotter),
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
            Zmumu = copy.deepcopy(drawOption_Zmumu),
            WplusJets = copy.deepcopy(drawOption_WplusJets),
            qcdSum = copy.deepcopy(drawOption_QCD)
        )
    ),

    drawJobs = cms.PSet(
        # cut-flow control plots
        cutFlowControlPlots_vertexChi2Prob_afterPrimaryEventVertex = copy.deepcopy(plots_ZtoMuTau_vertexChi2Prob_afterPrimaryEventVertex),
        cutFlowControlPlots_vertexZ_afterPrimaryEventVertexQuality = copy.deepcopy(plots_ZtoMuTau_vertexZ_afterPrimaryEventVertexQuality),
        cutFlowControlPlots_muon_afterPrimaryEventVertexPosition = copy.deepcopy(plots_ZtoMuTau_muon_afterPrimaryEventVertexPosition),
        cutFlowControlPlots_muon_afterGlobalMuon = copy.deepcopy(plots_ZtoMuTau_muon_afterGlobalMuon),
        cutFlowControlPlots_muon_afterMuonEta = copy.deepcopy(plots_ZtoMuTau_muon_afterMuonEta),
        cutFlowControlPlots_muonTrkIso_afterMuonPt = copy.deepcopy(plots_ZtoMuTau_muonTrkIso_afterMuonPt),
        cutFlowControlPlots_muonEcalIso_afterMuonTrkIso = copy.deepcopy(plots_ZtoMuTau_muonEcalIso_afterMuonTrkIso),
        cutFlowControlPlots_muonComp_afterMuonEcalIso = copy.deepcopy(plots_ZtoMuTau_muonComp_afterMuonEcalIso),
        cutFlowControlPlots_muonTrkIP_afterMuonAntiPionVeto = copy.deepcopy(plots_ZtoMuTau_muonTrkIP_afterMuonAntiPionVeto),
        cutFlowControlPlots_tau_afterMuonTrkIP = copy.deepcopy(plots_ZtoMuTau_tau_afterMuonTrkIP),
        cutFlowControlPlots_tauLeadTrkPt_afterMuonTrkIP = copy.deepcopy(plots_ZtoMuTau_tauLeadTrkPt_afterMuonTrkIP),
        cutFlowControlPlots_tau_afterTauAntiOverlapWithMuonsVeto = copy.deepcopy(plots_ZtoMuTau_tau_afterTauAntiOverlapWithMuonsVeto),
        cutFlowControlPlots_tauLeadTrkPt_afterTauAntiOverlapWithMuonsVeto = copy.deepcopy(plots_ZtoMuTau_tauLeadTrkPt_afterTauAntiOverlapWithMuonsVeto),
        cutFlowControlPlots_tau_afterTauEta = copy.deepcopy(plots_ZtoMuTau_tau_afterTauEta),
        cutFlowControlPlots_tauLeadTrkPt_afterTauEta = copy.deepcopy(plots_ZtoMuTau_tauLeadTrkPt_afterTauEta),
        cutFlowControlPlots_tau_afterTauPt = copy.deepcopy(plots_ZtoMuTau_tau_afterTauPt),
        cutFlowControlPlots_tauLeadTrkPt_afterTauPt = copy.deepcopy(plots_ZtoMuTau_tauLeadTrkPt_afterTauPt),
        cutFlowControlPlots_tau_afterTauLeadTrk = copy.deepcopy(plots_ZtoMuTau_tau_afterTauLeadTrk),
        cutFlowControlPlots_tauLeadTrkPt_afterTauLeadTrk = copy.deepcopy(plots_ZtoMuTau_tauLeadTrkPt_afterTauLeadTrk),
        cutFlowControlPlots_tau_afterTauLeadTrkPt = copy.deepcopy(plots_ZtoMuTau_tau_afterTauLeadTrkPt),
        cutFlowControlPlots_tau_afterTauTrkIso = copy.deepcopy(plots_ZtoMuTau_tau_afterTauTrkIso),
        cutFlowControlPlots_tauNumTracksSignalCone_afterTauEcalIso = copy.deepcopy(plots_ZtoMuTau_tauNumTracksSignalCone_afterTauEcalIso),
        cutFlowControlPlots_tau_afterTauProng = copy.deepcopy(plots_ZtoMuTau_tau_afterTauProng),
        cutFlowControlPlots_tauAntiMuonDiscr_afterTauProng = copy.deepcopy(plots_ZtoMuTau_tauAntiMuonDiscr_afterTauProng),
        cutFlowControlPlots_dR12_afterTauMuonVeto = copy.deepcopy(plots_ZtoMuTau_dR12_afterTauMuonVeto),
        cutFlowControlPlots_diTauCharge_afterAntiOverlapVeto = copy.deepcopy(plots_ZtoMuTau_diTauCharge_afterAntiOverlapVeto),
        cutFlowControlPlots_mtMuonMET_afterZeroCharge = copy.deepcopy(plots_ZtoMuTau_mtMuonMET_afterZeroCharge),
        
        # distributions plotted for events passing all cuts
        finalSamplePlots_muon = copy.deepcopy(plots_ZtoMuTau_muon_finalEventSample),
        finalSamplePlots_tau = copy.deepcopy(plots_ZtoMuTau_tau_finalEventSample),
        finalSamplePlots_tauLeadTrkPt = copy.deepcopy(plots_ZtoMuTau_tauLeadTrkPt_finalEventSample),
        finalSamplePlots_tauNumTracksSignalCone = copy.deepcopy(plots_ZtoMuTau_tauNumTracksSignalCone_finalEventSample),
        finalSamplePlots_met = copy.deepcopy(plots_ZtoMuTau_met_finalEventSample),
        finalSamplePlots_mtMuonMET = copy.deepcopy(plots_ZtoMuTau_mtMuonMET_finalEventSample),
        finalSamplePlots_mtTauMET = copy.deepcopy(plots_ZtoMuTau_mtTauMET_finalEventSample),
        finalSamplePlots_mtMuonTauMET = copy.deepcopy(plots_ZtoMuTau_mtMuonTauMET_finalEventSample),
        finalSamplePlots_mCDFmethod = copy.deepcopy(plots_ZtoMuTau_mCDFmethod_finalEventSample),
        finalSamplePlots_mCollApprox = copy.deepcopy(plots_ZtoMuTau_mCollApprox_finalEventSample),
        finalSamplePlots_numCentralJets = copy.deepcopy(plots_ZtoMuTau_numCentralJets_finalEventSample)
    ),

    canvasSizeX = cms.int32(800),
    canvasSizeY = cms.int32(640),                         

    outputFilePath = cms.string('./plots/'),
    #outputFileName = cms.string('plotsZtoMuTau.ps')
    indOutputFileName = cms.string('plotZtoMuTau_#PLOT#.png')
)

saveZtoMuTau = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string('plotsZtoMuTau_all.root')
)


  
