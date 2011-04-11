import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.Configuration.analyzeWtoTauNu_cfi import *
from TauAnalysis.DQMTools.tools.drawJobConfigurator import *

# define template for all kinds of plots
# (specific to W --> tau nu analysis)
plots_WtoTauNu = cms.PSet(
    plots = cms.PSet(  
        dqmMonitorElements = cms.vstring(''),
        processes = cms.vstring(
            'data',
            'ZplusJets',
            'Wmunu',
            'Wenu',
            'qcd_W',   
            'Wtaunu',
            )
    ),
    xAxis = cms.string('unlabeled'),
    yAxis = cms.string('numEntries_linear'),
    #yAxis = cms.string('numEntries_log'),
    legend = cms.string('regular'),
    labels = cms.vstring('mcNormScale'),                   
    drawOptionSet = cms.string('default'),
    stack = cms.vstring(
#        'ZplusJets',
#        'Wmunu',
#        'Wenu',
#        'qcd_W',                
#        'Wtaunu'
        )
    )

drawJobConfigurator_WtoTauNu = drawJobConfigurator(
    template = plots_WtoTauNu,
    dqmDirectory = '#PROCESSDIR#/wTauNuAnalyzer/'
)

#--------------------------------------------------------------------------------
# define cut-flow control plots;
# show distribution of each quantity used in event selection
# (**before** quantity is cutted on)
#--------------------------------------------------------------------------------

drawJobConfigurator_WtoTauNu.add(
    afterCut = evtSelPrimaryEventVertex,
    beforeCut = evtSelPrimaryEventVertexQuality,
    plot = drawJobConfigEntry(
        meName = 'VertexQuantities/VertexChi2Prob',
        title = "P(#Chi^{2}_{vtx} (after primary Event Vertex Cut)",
        xAxis = 'prob',
        name = "cutFlowControlPlots_vertexChi2Prob_afterPrimaryEventVertex"
    )
)

drawJobConfigurator_WtoTauNu.add(
    afterCut = evtSelPrimaryEventVertexQuality,
    beforeCut = evtSelPrimaryEventVertexPosition,
    plot = drawJobConfigEntry(
        meName = 'VertexQuantities/VertexZ',
        title = "z_{vtx} (after primary Event Vertex quality Cut)",
        xAxis = 'posZ',
        name = "cutFlowControlPlots_vertexZ_afterPrimaryEventVertexQuality"
    )
)

drawJobConfigurator_WtoTauNu.add(
    afterCut = evtSelTauPt,
    beforeCut = evtSelPFMetPt,
    plots = [
        drawJobConfigEntry(
            meName = 'PFMEtQuantities/MEtPt',
            title = "PF-MET (after tau p_T cut)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_PFMET_afterTauPtcut"
            ),
        drawJobConfigEntry(
            meName = 'MEtTopologyQuantities/Vratio',
            title = "met-topology (after tau p_T cut)",
            xAxis = 'prob',
            name = "cutFlowControlPlots_MetTopology_afterTauPtcut"
            ),
        drawJobConfigEntry(
            meName = 'TauRecoilEnergyFromCaloTowersQuantities/EtSum',
            title = "tau recoil energy from calo towers (after tau p_T cut)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_TauRecoilEnergyFromCaloTowers_afterTauPtcut"
            ),
        drawJobConfigEntry(
            meName = 'TauNuCandidateQuantities/NuTauCandidateMt',
            title = "m_{T} (tau + nu) (after tau pt cut)",
            xAxis = 'Mt',
            name = "cutFlowControlPlots_NuTauCandidateMt_afterTauPtcut"
            ),
        drawJobConfigEntry(
            meName = 'TauNuCandidateQuantities/NuTauCandidateDPhi',
            title = "#Delta #Phi (after tau pt cut)",
            xAxis = 'Phi',
            name = "cutFlowControlPlots_NuTauCandidateDPhi_afterTauPtcut"
            ),
        drawJobConfigEntry(
            meName = 'TauQuantities/Tau#PAR#',
            PAR = [ 'Pt', 'Eta', 'Phi'],
            title = "Tau (after tau p_{T} cut)",
            xAxis = '#PAR#',
            name = "cutFlowControlPlots_tau_afterTauPtcut"
            ),
 #       drawJobConfigEntry(
 #           meName = 'TauQuantities/TauDiscriminatorByIsolation',
 #           title = "Tau isolation (after tau p_{T} cut)",
 #           xAxis = 'unlabeled',
  #          name = "cutFlowControlPlots_tauIsolation_afterTauPtcut"
  #          ),
        drawJobConfigEntry(
            meName = 'TauQuantities/TauLeadTrkPt',
            title = "Tau lead. Track (after tau p_{T} cut)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_tauLeadTrkPt_afterTauPtcut"
            )
        ]
    )


drawJobConfigurator_WtoTauNu.add(
    afterCut = evtSelPFMetPt,
    beforeCut = evtSelMetPt,
    plots = [
          drawJobConfigEntry(
            meName = 'TauQuantities/Tau#PAR#',
            PAR = [ 'Pt', 'Eta', 'Phi' ],
            title = "Tau (after PF-MET cut)",
            xAxis = '#PAR#',
            name = "cutFlowControlPlots_tau_afterPFMETcut"
            ),
        drawJobConfigEntry(
            meName = 'TauQuantities/TauLeadTrkPt',
            title = "Tau lead. Track (after PF-MET cut)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_tauLeadTrkPt_afterPFMETcut"
            ),
        drawJobConfigEntry(
            meName = 'PFMEtQuantities/MEtPt',
            title = "PF-MET (after PF-MET cut)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_PFMET_afterPFMETcut"
            ),
        drawJobConfigEntry(
            meName = 'MEtTopologyQuantities/Vratio',
            title = "met-topology (after PF-MET cut)",
            xAxis = 'prob',
            name = "cutFlowControlPlots_MetTopology_afterPFMETcut"
            ),
        drawJobConfigEntry(
            meName = 'TauNuCandidateQuantities/NuTauCandidateMt',
            title = "m_{T} (tau + nu) (after PF-MET cut)",
            xAxis = 'Mt',
            name = "cutFlowControlPlots_NuTauCandidateMt_afterPFMETcut"
            ),
        drawJobConfigEntry(
            meName = 'TauNuCandidateQuantities/NuTauCandidateDPhi',
            title = "#Delta #Phi (after PF-MET cut)",
            xAxis = 'Phi',
            name = "cutFlowControlPlots_NuTauCandidateDPhi_afterPFMETcut"
            )
#        drawJobConfigEntry(
#            meName = 'TauQuantities/TauDiscriminatorByIsolation',
#            title = "Tau isolation (after tau PF-MET cut)",
#            xAxis = 'unlabeled',
#            name = "cutFlowControlPlots_tauIsolation_afterPFMETcut"
#            )
        ]
    )

drawJobConfigurator_WtoTauNu.add(
    afterCut = evtSelTauCharge,
    beforeCut = evtSelTauMuonVeto,
    plots = [
        drawJobConfigEntry(
            meName = 'TauQuantities/Tau#PAR#',
            PAR = [ 'Pt', 'Eta', 'Phi' ],
            title = "Tau (after tau isolation)",
            xAxis = '#PAR#',
            name = "cutFlowControlPlots_tau_afterTauIsolationcut"
            ),
        drawJobConfigEntry(
            meName = 'TauQuantities/TauLeadTrkPt',
            title = "Tau lead. Track (after tau isolation)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_tauLeadTrkPt_afterTauIsolation"
            ),
        drawJobConfigEntry(
            meName = 'PFMEtQuantities/MEtPt',
            title = "PF-MET (after tau isolation)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_PFMET_afterTauIsolation"
            ),
        drawJobConfigEntry(
            meName = 'MEtTopologyQuantities/Vratio',
            title = "met-topology (after tau isolation)",
            xAxis = 'prob',
            name = "cutFlowControlPlots_MetTopology_afterTauIsolation"
            ),        
        drawJobConfigEntry(
            meName = 'TauRecoilEnergyFromCaloTowersQuantities/EtSum',
            title = "tau recoil energy from calo towers (after tau isolation)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_TauRecoilEnergyFromCaloTowers_afterTauIsolation"
            )
        ]
    )

drawJobConfigurator_WtoTauNu.add(
    afterCut = evtSelRecoilEnergyFromCaloTowers,
    beforeCut = evtSelCentralJetVeto1,
    plots = [
        drawJobConfigEntry(
            meName = 'TauQuantities/Tau#PAR#',
            PAR = [ 'Pt', 'Eta', 'Phi' ],
            title = "Tau (after e and mu veto)",
            xAxis = '#PAR#',
            name = "cutFlowControlPlots_tau_afterMuonVeto"
            ),
        drawJobConfigEntry(
            meName = 'TauQuantities/TauLeadTrkPt',
            title = "Tau lead. Track (after e and mu veto)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_tauLeadTrkPt_afterMuonVeto"
            ),
        drawJobConfigEntry(
            meName = 'PFMEtQuantities/MEtPt',
            title = "PFMET (after e and mu veto)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_PFMET_afterMuonVeto"
            ),         
        drawJobConfigEntry(
            meName = 'TauRecoilEnergyFromCaloTowersQuantities/EtSum',
            title = "Recoil energy from Calo Towers (after e and mu veto)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_recoilEnergyFromCaloTowers_afterMuonVeto"
            ),
        drawJobConfigEntry(
            meName = 'MEtTopologyQuantities/Vratio',
            title = "MET topology (after e and mu veto)",
            xAxis = 'prob',
            name = "cutFlowControlPlots_MEtTopology_afterMuonVeto"
            ),
        drawJobConfigEntry(
            meName = 'TauNuCandidateQuantities/NuTauCandidateMt',
            title = "m_{T} (tau + nu) (after e and mu veto)",
            xAxis = 'Mt',
            name = "cutFlowControlPlots_NuTauCandidateMt_afterMuonVeto"
            ),
        drawJobConfigEntry(
            meName = 'TauNuCandidateQuantities/NuTauCandidateDPhi',
            title = "#Delta #Phi (after e and mu veto)",
            xAxis = 'Phi',
            name = "cutFlowControlPlots_NuTauCandidateDPhi_afterMuonVeto"
            )
        ]
    )
drawJobConfigurator_WtoTauNu.add(
    afterCut = evtSelCentralJetVeto2,
    beforeCut = evtSelMetTopology,
    plots = [
        drawJobConfigEntry(
            meName = 'MEtTopologyQuantities/Vratio',
            title = 'MET topology (after HT cut)',
            xAxis = 'prob',
            name = "cutFlowControlPlots_MetTopology_afterHTratio"
            )
        ]
    )
    
#--------------------------------------------------------------------------------
# define distributions to be plotted
# for events passing all event selection criteria
#--------------------------------------------------------------------------------
drawJobConfigurator_WtoTauNu.add(
    afterCut = evtSelMetTopology,
    plots =  [
        drawJobConfigEntry(
            meName = 'TauQuantities/TauMatchingGenParticlePdgId',
            title = "Matching gen-particle Pdg-Id (after MET-topology)",
            xAxis = 'unlabeled',
            name = "cutFlowControlPlots_jetPt_afterMetTopology"
            ), 
        drawJobConfigEntry(
            meName = 'TauQuantities/Tau#PAR#',
            PAR = [ 'Pt', 'Eta', 'Phi' ],
            title = "Tau (after MET-topology)",
            xAxis = '#PAR#',
            name = "cutFlowControlPlots_tau_afterMetTopology"
            ),
        drawJobConfigEntry(
            meName = 'TauQuantities/TauNumTracksSignalCone',
            title = "Tau prong (after MET-topology)",
            xAxis = 'unlabeled',
            name = "cutFlowControlPlots_tauNumTracksSignalCone_afterMetTopology"
            ),
        drawJobConfigEntry(
            meName = 'TauQuantities/TauLeadTrkPt',
            title = "Tau lead. Track (after MET-topology)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_tauLeadTrkPt_afterMetTopology"
            ),
        drawJobConfigEntry(
            meName = 'PFMEtQuantities/MEtPt',
            title = "PF-MET (after MET-topology)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_PFMET_afterMetTopology"
            ),           
        drawJobConfigEntry(
            meName = 'TauRecoilEnergyFromCaloTowersQuantities/EtSum',
            title = "Recoil energy from Calo Towers (after MET-topology)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_recoilEnergyFromCaloTowers_afterMetTopology"
            ),
        drawJobConfigEntry(
            meName = 'MEtTopologyQuantities/Vratio',
            title = "MET topology (after MET-topology)",
            xAxis = 'prob',
            name = "cutFlowControlPlots_METtopology_afterMetTopology"
            ),
        drawJobConfigEntry(
            meName = 'TauNuCandidateQuantities/NuTauCandidateMt',
            title = "m_{T} (tau + nu) (after MET-topology)",
            xAxis = 'Mt',
            name = "cutFlowControlPlots_NuTauCandidateMt_afterMetTopology"
            ),
        drawJobConfigEntry(
            meName = 'TauNuCandidateQuantities/NuTauCandidateDPhi',
            title = "#Delta #Phi (after MET-topology)",
            xAxis = 'Phi',
            name = "cutFlowControlPlots_NuTauCandidateDPhi_afterMetTopology"
            ),
        drawJobConfigEntry(
            meName = 'TauQuantities/TauLeadTrkNumPixelHits',
            title = "Tau lead. Track num. pixel hits (after MET-topology)",
            xAxis = 'unlabeled',
            name = "cutFlowControlPlots_tauLeadTrkNumPixelHits_afterMetTopology"
            ),
    drawJobConfigEntry(
            meName = 'TauQuantities/TauLeadTrkNumStripHits',
            title = "Tau lead. Track num. strip hits (after MET-topology)",
            xAxis = 'unlabeled',
            name = "cutFlowControlPlots_tauLeadTrkNumStripHits_afterMetTopology"
            ),
    drawJobConfigEntry(
            meName = 'TauQuantities/TauEmFraction',
            title = "EM-fraction (after MET-topology)",
            xAxis = 'unlabeled',
            name = "cutFlowControlPlots_tauLeadTrkNumStripHits_afterMetTopology"     
            )
    
        ]    
    )

