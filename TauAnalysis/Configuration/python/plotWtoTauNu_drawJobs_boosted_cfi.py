import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.Configuration.analyzeWtoTauNu_boosted_cfi import *
from TauAnalysis.DQMTools.tools.drawJobConfigurator import *

# define template for all kinds of plots
# (specific to W --> tau nu analysis)
plots_WtoTauNu = cms.PSet(
    plots = cms.PSet(  
        dqmMonitorElements = cms.vstring(''),
        processes = cms.vstring(
            #'ZplusJets',
            'Wmunu',
            'Wenu',
            'qcd_W',
            'Wtaunu'
            )
    ),
    xAxis = cms.string('unlabeled'),
    yAxis = cms.string('numEntries_linear'),
    #yAxis = cms.string('numEntries_log'),
    legend = cms.string('regular'),
    labels = cms.vstring('mcNormScale'),                   
    drawOptionSet = cms.string('default'),
    stack = cms.vstring(
        #'ZplusJets',
        'Wmunu',
        'Wenu',
        'qcd_W',
        'Wtaunu'
        )
    )

drawJobConfigurator_WtoTauNuBoosted = drawJobConfigurator(
    template = plots_WtoTauNu,
    dqmDirectory = '#PROCESSDIR#/wTauNuBoostedAnalyzer/'
)

#--------------------------------------------------------------------------------
# define cut-flow control plots;
# show distribution of each quantity used in event selection
# (**before** quantity is cutted on)
#--------------------------------------------------------------------------------

drawJobConfigurator_WtoTauNuBoosted.add(
    afterCut = evtSelPrimaryEventVertexBoosted,
    beforeCut = evtSelPrimaryEventVertexQualityBoosted,
    plot = drawJobConfigEntry(
        meName = 'VertexQuantities/VertexChi2Prob',
        title = "P(#Chi^{2}_{vtx} (after primary Event Vertex Cut)",
        xAxis = 'prob',
        name = "cutFlowControlPlots_vertexChi2Prob_afterPrimaryEventVertex"
    )
)

drawJobConfigurator_WtoTauNuBoosted.add(
    afterCut = evtSelPrimaryEventVertexQualityBoosted,
    beforeCut = evtSelPrimaryEventVertexPositionBoosted,
    plot = drawJobConfigEntry(
        meName = 'VertexQuantities/VertexZ',
        title = "z_{vtx} (after primary Event Vertex quality Cut)",
        xAxis = 'posZ',
        name = "cutFlowControlPlots_vertexZ_afterPrimaryEventVertexQuality"
    )
)

drawJobConfigurator_WtoTauNuBoosted.add(
    afterCut = evtSelPFMetPtBoosted,
    beforeCut = evtSelMetPtBoosted,
    plots = [
        drawJobConfigEntry(
            meName = 'CaloMEtQuantities/RAW_MEtPt',
            title = "calo-MET (after PF-MET cut)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_MET_afterPFMETcut"
            )   
        ]
    )

drawJobConfigurator_WtoTauNuBoosted.add(
    afterCut = evtSelCentralJetVetoBoosted,
    beforeCut = evtSelPhiJetTauBoosted,
    plots = [
        drawJobConfigEntry(
            meName = 'TauQuantities/Tau#PAR#',
            PAR = [ 'Pt', 'Eta', 'Phi' ],
            title = "Tau (after jet veto cut)",
            xAxis = '#PAR#',
            name = "cutFlowControlPlots_tau_afterJetVeto"
            ),
        drawJobConfigEntry(
            meName = 'TauQuantities/TauLeadTrkPt',
            title = "Tau lead. Track (after jet veto cut)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_tauLeadTrkPt_afterJetVeto"
            ),
        drawJobConfigEntry(
            meName = 'CaloMEtQuantities/RAW_MEtPt',
            title = "calo-MET (after jet veto cut)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_MET_afterJetVeto"
            ),   
        drawJobConfigEntry(
            meName = 'PFMEtQuantities/MEtPt',
            title = "PF-MET (after jet veto cut)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_PFMET_afterJetVeto"
            ),       
        drawJobConfigEntry(
            meName = 'TauRecoilEnergyFromCaloTowersQuantities/EtSum',
            title = "Recoil energy from Calo Towers (after jet veto cut)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_recoilEnergyFromCaloTowers_afterJetVeto"
            ),
        drawJobConfigEntry(
            meName = 'TauRecoilEnergyFromJetsQuantities/EtSum',
            title = "Recoil energy from Jets (after jet veto cut)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_recoilEnergyFromJets_afterJetVeto"
            ),
        drawJobConfigEntry(
            meName = 'MEtTopologyQuantities/Vratio',
            title = "MET topology (after jet veto cut)",
            xAxis = 'prob',
            name = "cutFlowControlPlots_MEtTopology_afterJetVeto"
            ),
        drawJobConfigEntry(
            meName = 'TauNuCandidateQuantities/NuTauCandidateMt',
            title = "m_{T} (tau + nu) (after jet veto cut)",
            xAxis = 'Mt',
            name = "cutFlowControlPlots_NuTauCandidateMt_afterJetVeto"
            ),
        drawJobConfigEntry(
            meName = 'TauNuCandidateQuantities/NuTauCandidateDPhi',
            title = "#Delta #Phi(MET,tau) (after jet veto cut)",
            xAxis = 'Phi',
            name = "cutFlowControlPlots_NuTauCandidateDPhi_afterJetVeto"
            ),
        drawJobConfigEntry(
            meName = 'CaloMEtQuantities/RAW_MEtSignificance',
            title = "MET significance (after jet veto cut)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_MEtSignificance_afterJetVeto"
            ),
        drawJobConfigEntry(
            meName = 'PFEventShapeVars/Circularity',
            title = "Circularity (after jet veto cut)",
            xAxis = 'prob',
            name = "cutFlowControlPlots_Circularity_afterJetVeto"
            ),
        drawJobConfigEntry(
            meName = 'PFEventShapeVars/C',
            title = "C (after jet veto cut)",
            xAxis = 'prob',
            name = "cutFlowControlPlots_C_afterJetVeto"
            )

        ]
)

drawJobConfigurator_WtoTauNuBoosted.add(
    afterCut = evtSelPhiJetMetBoosted,
    beforeCut = evtSelMetTopologyBoosted,
    plots = [
        drawJobConfigEntry(
            meName = 'TauQuantities/Tau#PAR#',
            PAR = [ 'Pt', 'Eta', 'Phi' ],
            title = "Tau (after phi(jet,MET))",
            xAxis = '#PAR#',
            name = "cutFlowControlPlots_tau_afterPhiJetMet"
            ),
        drawJobConfigEntry(
            meName = 'TauQuantities/TauLeadTrkPt',
            title = "Tau lead. Track (after phi(jet,MET))",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_tauLeadTrkPt_afterPhiJetMet"
            ),
        drawJobConfigEntry(
            meName = 'CaloMEtQuantities/RAW_MEtPt',
            title = "calo-MET (after phi(jet,MET))",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_MET_afterPhiJetMet"
            ),   
        drawJobConfigEntry(
            meName = 'PFMEtQuantities/MEtPt',
            title = "PF-MET (after phi(jet,MET))",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_PFMET_afterPhiJetMet"
            ),       
        drawJobConfigEntry(
            meName = 'TauRecoilEnergyFromCaloTowersQuantities/EtSum',
            title = "Recoil energy from Calo Towers (after phi(jet,MET))",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_recoilEnergyFromCaloTowers_afterPhiJetMet"
            ),
        drawJobConfigEntry(
            meName = 'TauRecoilEnergyFromJetsQuantities/EtSum',
            title = "Recoil energy from Jets (after phi(jet,MET))",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_recoilEnergyFromJets_afterPhiJetMet"
            ),
        drawJobConfigEntry(
            meName = 'MEtTopologyQuantities/Vratio',
            title = "MET topology (after phi(jet,MET))",
            xAxis = 'prob',
            name = "cutFlowControlPlots_MEtTopology_afterPhiJetMet"
            ),
        drawJobConfigEntry(
            meName = 'TauNuCandidateQuantities/NuTauCandidateMt',
            title = "m_{T} (tau + nu) (after phi(jet,MET))",
            xAxis = 'Mt',
            name = "cutFlowControlPlots_NuTauCandidateMt_afterPhiJetMet"
            ),
        drawJobConfigEntry(
            meName = 'TauNuCandidateQuantities/NuTauCandidateDPhi',
            title = "#Delta #Phi (after phi(jet,MET))",
            xAxis = 'Phi',
            name = "cutFlowControlPlots_NuTauCandidateDPhi_afterPhiJetMet"
            ),
        drawJobConfigEntry(
            meName = 'CaloMEtQuantities/RAW_MEtSignificance',
            title = "MET significance (after phi(jet,MET))",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_MEtSignificance_afterPhiJetMet"
            ),
        drawJobConfigEntry(
            meName = 'PFEventShapeVars/Circularity',
            title = "Circularity (after phi(jet,MET))",
            xAxis = 'prob',
            name = "cutFlowControlPlots_Circularity_afterPhiJetMet"
            ),
        drawJobConfigEntry(
            meName = 'PFEventShapeVars/C',
            title = "C (after phi(jet,MET))",
            xAxis = 'prob',
            name = "cutFlowControlPlots_C_afterPhiJetMet"
            )

        ]
)

#--------------------------------------------------------------------------------
# define distributions to be plotted
# for events passing all event selection criteria
#--------------------------------------------------------------------------------

drawJobConfigurator_WtoTauNuBoosted.add(
    afterCut = evtSelMetSignificanceBoosted,
    plots =  [
        drawJobConfigEntry(
            meName = 'TauQuantities/Tau#PAR#',
            PAR = [ 'Pt', 'Eta', 'Phi' ],
            title = "Tau (after met-significance cut)",
            xAxis = '#PAR#',
            name = "cutFlowControlPlots_tau_afterMetSignificance"
            ),
        drawJobConfigEntry(
            meName = 'TauQuantities/TauLeadTrkPt',
            title = "Tau lead. Track (after met-significance cut)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_tauLeadTrkPt_afterMetSignificance"
            ),
          drawJobConfigEntry(
            meName = 'CaloMEtQuantities/RAW_MEtPt',
            title = "calo-MET (after met-significance cut)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_MET_afterMetSignificance"
            ),     
        drawJobConfigEntry(
            meName = 'PFMEtQuantities/MEtPt',
            title = "PF-MET (after met-significance cut)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_PFMET_aferMetSignificance"
            ),   
        drawJobConfigEntry(
            meName = 'TauRecoilEnergyFromCaloTowersQuantities/EtSum',
            title = "Recoil energy from Calo Towers (after met-significance cut)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_recoilEnergyFromCaloTowers_afterMetSignificance"
            ),
        drawJobConfigEntry(
            meName = 'TauRecoilEnergyFromJetsQuantities/EtSum',
            title = "Recoil energy from Jets (after met-significance cut)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_recoilEnergyFromJets_afterMetSignificance"
            ),
        drawJobConfigEntry(
            meName = 'MEtTopologyQuantities/Vratio',
            title = "MET topology (after met-significance cut)",
            xAxis = 'prob',
            name = "cutFlowControlPlots_METtopology_afterMetSignificance"
            ),
        drawJobConfigEntry(
            meName = 'TauNuCandidateQuantities/NuTauCandidateMt',
            title = "m_{T} (tau + nu) (after met-significance cut)",
            xAxis = 'Mt',
            name = "cutFlowControlPlots_NuTauCandidateMt_afterMetSignificance"
            ),
        drawJobConfigEntry(
            meName = 'TauNuCandidateQuantities/NuTauCandidateDPhi',
            title = "#Delta #Phi(MET,tau) (after met-significance cut)",
            xAxis = 'Phi',
            name = "cutFlowControlPlots_NuTauCandidateDPhi_afterMetSignificance"
            ),
        drawJobConfigEntry(
            meName = 'CaloMEtQuantities/RAW_MEtSignificance',
            title = "MET significance (after met-significance cut)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_MEtSignificance_afterMetSignificance"
            ),
        drawJobConfigEntry(
            meName = 'PFEventShapeVars/Circularity',
            title = "Circularity (after met-significance cut)",
            xAxis = 'prob',
            name = "cutFlowControlPlots_Circularity_afterMetSignificance"
            ),
        drawJobConfigEntry(
            meName = 'PFEventShapeVars/C',
            title = "C (after met-significance cut)",
            xAxis = 'prob',
            name = "cutFlowControlPlots_C_afterMetSignificance"
            ),
        drawJobConfigEntry(
            meName = 'PFEventShapeVars/Aplanarity',
            title = "Aplanarity (after met-significance cut)",
            xAxis = 'prob',
            name = "cutFlowControlPlots_Aplanarity_afterMetSignificance"
            ),
        drawJobConfigEntry(
            meName = 'PFEventShapeVars/D',
            title = "D (after met-significance cut)",
            xAxis = 'prob',
            name = "cutFlowControlPlots_D_afterMetSignificance"
            ),
        drawJobConfigEntry(
            meName = 'PFEventShapeVars/Sphericity',
            title = "Spherisity (after met-significance cut)",
            xAxis = 'prob',
            name = "cutFlowControlPlots_Sphericity_afterMetSignificance"
            ),
        drawJobConfigEntry(
            meName = 'PFEventShapeVars/Thrust',
            title = "Thrust (after met-significance cut)",
            xAxis = 'prob',
            name = "cutFlowControlPlots_Thrust_afterMetSignificance"
            ),
        drawJobConfigEntry(
            meName = 'TauNuCandidateQuantities/Jet_Tau_DPhi',
            title = "#Delta #Phi(tau,jet) (after met-significance cut)",
            xAxis = 'Phi',
            name = "cutFlowControlPlots_mettauDPhi_afterMetSignificance"
            ),
        drawJobConfigEntry(
            meName = 'TauNuCandidateQuantities/Jet_MET_DPhi',
            title = "#Delta #Phi(jet,MET) (after met-significance cut)",
            xAxis = 'Phi',
            name = "cutFlowControlPlots_jetmeteDPhi_afterMetSignificance"
            )
        
        ]    
)
                
