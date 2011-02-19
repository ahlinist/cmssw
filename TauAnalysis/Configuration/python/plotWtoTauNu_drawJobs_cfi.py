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
#            'ZplusJets',
#            'Wmunu',
#            'Wenu',
#            'qcd_W',   
#            'Wtaunu',
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
 #       'Wmunu',
 #       'Wenu',
 #       'qcd_W',                
 #       'Wtaunu',
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
            meName = 'CaloMEtQuantities/RAW_MEtPt',
            title = "calo-MET (after tau p_T cut)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_MET_afterTauPtcut"
            ),
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
        drawJobConfigEntry(
            meName = 'TauQuantities/TauDiscriminatorByIsolation',
            title = "Tau isolation (after tau p_{T} cut)",
            xAxis = 'unlabeled',
            name = "cutFlowControlPlots_tauIsolation_afterTauPtcut"
            ),
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
            ),
        drawJobConfigEntry(
            meName = 'TauQuantities/TauDiscriminatorByIsolation',
            title = "Tau isolation (after tau PF-MET cut)",
            xAxis = 'unlabeled',
            name = "cutFlowControlPlots_tauIsolation_afterPFMETcut"
            )
        ]
    )

drawJobConfigurator_WtoTauNu.add(
    afterCut = evtSelTauCharge,
    beforeCut = evtSelTauMuonVeto,
    plots = [
        drawJobConfigEntry(
            meName = 'TauQuantities/Tau#PAR#',
            PAR = [ 'Pt', 'Eta', 'Phi' ],
            title = "Tau (after tau charge cut)",
            xAxis = '#PAR#',
            name = "cutFlowControlPlots_tau_afterTauChargecut"
            ),
        drawJobConfigEntry(
            meName = 'TauQuantities/TauLeadTrkPt',
            title = "Tau lead. Track (after tau charge cut)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_tauLeadTrkPt_afterTauChargecut"
            ),
        drawJobConfigEntry(
            meName = 'PFMEtQuantities/MEtPt',
            title = "PF-MET (after tau charge cut)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_PFMET_afterTauChargecut"
            ),
        drawJobConfigEntry(
            meName = 'MEtTopologyQuantities/Vratio',
            title = "met-topology (after tau charge cut)",
            xAxis = 'prob',
            name = "cutFlowControlPlots_MetTopology_afterTauChargecut"
            ),
        drawJobConfigEntry(
            meName = 'TauRecoilEnergyFromCaloTowersQuantities/EtSum',
            title = "tau recoil energy from calo towers (after tau charge cut)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_TauRecoilEnergyFromCaloTowers_afterTauChargecut"
            ),
        drawJobConfigEntry(
            meName = 'TauNuCandidateQuantities/NuTauCandidateMt',
            title = "m_{T} (tau + nu) (after tau charge cut)",
            xAxis = 'Mt',
            name = "cutFlowControlPlots_NuTauCandidateMt_afterTauChargecut"
            ),
        drawJobConfigEntry(
            meName = 'TauNuCandidateQuantities/NuTauCandidateDPhi',
            title = "#Delta #Phi (after tau charge cut)",
            xAxis = 'Phi',
            name = "cutFlowControlPlots_NuTauCandidateDPhi_afterTauChargecut"
            ),
        drawJobConfigEntry(
            meName = 'TauQuantities/TauDiscriminatorByIsolation',
            title = "Tau isolation (after tau charge cut)",
            xAxis = 'unlabeled',
            name = "cutFlowControlPlots_tauIsolation_afterTauChargecut"
            ),
        drawJobConfigEntry(
            meName = 'TauQuantities/TauDiscriminatorAgainstMuons',
            title = "Tau muon veto (after tau charge cut)",
            xAxis = 'unlabeled',
            name = "cutFlowControlPlots_tauIsolation_afterTauChargecut"
            )
        ]
    )

drawJobConfigurator_WtoTauNu.add(
    afterCut = evtSelRecoilEnergyFromCaloTowers,
    beforeCut = evtSelMetTopology,
    plots = [
        drawJobConfigEntry(
            meName = 'TauQuantities/Tau#PAR#',
            PAR = [ 'Pt', 'Eta', 'Phi' ],
            title = "Tau (after Met topology cut)",
            xAxis = '#PAR#',
            name = "cutFlowControlPlots_tau_afterMetTopology"
            ),
        drawJobConfigEntry(
            meName = 'TauQuantities/TauLeadTrkPt',
            title = "Tau lead. Track (after Met topology cut)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_tauLeadTrkPt_afterMetTopology"
            ),
        drawJobConfigEntry(
            meName = 'CaloMEtQuantities/RAW_MEtPt',
            title = "calo-MET (after Met topology cut)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_MET_afterMetTopology"
            ), 
        drawJobConfigEntry(
            meName = 'PFMEtQuantities/MEtPt',
            title = "PFMET (after Met topology cut)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_PFMET_afterMetTopology"
            ),         
        drawJobConfigEntry(
            meName = 'TauRecoilEnergyFromCaloTowersQuantities/EtSum',
            title = "Recoil energy from Calo Towers (after Met topology cut)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_recoilEnergyFromCaloTowers_afterMetTopology"
            ),
        drawJobConfigEntry(
            meName = 'MEtTopologyQuantities/Vratio',
            title = "MET topology (after Met topology cut)",
            xAxis = 'prob',
            name = "cutFlowControlPlots_MEtTopology_afterMetTopology"
            ),
        drawJobConfigEntry(
            meName = 'TauNuCandidateQuantities/NuTauCandidateMt',
            title = "m_{T} (tau + nu) (after Met topology cut)",
            xAxis = 'Mt',
            name = "cutFlowControlPlots_NuTauCandidateMt_afterMetTopology"
            ),
        drawJobConfigEntry(
            meName = 'TauNuCandidateQuantities/NuTauCandidateDPhi',
            title = "#Delta #Phi (after Met topology cut)",
            xAxis = 'Phi',
            name = "cutFlowControlPlots_NuTauCandidateDPhi_afterMetTopology"
            )
        ]
    )

drawJobConfigurator_WtoTauNu.add(
    afterCut = evtSelCentralJetVeto,
    beforeCut = evtSelRecoilEnergyFromCaloTowers,
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
            meName = 'TauQuantities/TauNumTracksSignalCone',
            title = "Tau prong (after jet veto cut)",
            xAxis = 'unlabeled',
            name = "cutFlowControlPlots_tauNumTracksSignalCone_afterJetVeto"
            ),
        drawJobConfigEntry(
            meName = 'PFMEtQuantities/MEtPt',
            title = "PFMET (after jet veto cut)",
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
            title = "#Delta #Phi (after jet veto cut)",
            xAxis = 'Phi',
            name = "cutFlowControlPlots_NuTauCandidateDPhi_afterJetVeto"
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
            meName = 'JetQuantities/JetPt',
            title = "jet Pt (after ecal crack)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_jetPt_afterEcalCrack"
            ), 
        drawJobConfigEntry(
            meName = 'JetQuantities/NumJets',
            title = "num jets (after ecal crack)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_numJets_afterEcalCrack"
            ),
        drawJobConfigEntry(
            meName = 'TauQuantities/Tau#PAR#',
            PAR = [ 'Pt', 'Eta', 'Phi' ],
            title = "Tau (after ecal crack)",
            xAxis = '#PAR#',
            name = "cutFlowControlPlots_tau_afterEcalCrack"
            ),
        drawJobConfigEntry(
            meName = 'TauQuantities/TauNumTracksSignalCone',
            title = "Tau prong (after ecal crack)",
            xAxis = 'unlabeled',
            name = "cutFlowControlPlots_tauNumTracksSignalCone_afterEcalCrack"
            ),
        drawJobConfigEntry(
            meName = 'TauQuantities/TauLeadTrkPt',
            title = "Tau lead. Track (after ecal crack)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_tauLeadTrkPt_afterEcalCrack"
            ),
        drawJobConfigEntry(
            meName = 'PFMEtQuantities/MEtPt',
            title = "Pf-MET (after ecal crack)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_PFMET_afterEcalCrack"
            ),           
        drawJobConfigEntry(
            meName = 'TauRecoilEnergyFromCaloTowersQuantities/EtSum',
            title = "Recoil energy from Calo Towers (after ecal crack)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_recoilEnergyFromCaloTowers_afterEcalCrack"
            ),
        drawJobConfigEntry(
            meName = 'MEtTopologyQuantities/Vratio',
            title = "MET topology (after ecal crack)",
            xAxis = 'prob',
            name = "cutFlowControlPlots_METtopology_afterEcalCrack"
            ),
        drawJobConfigEntry(
            meName = 'TauNuCandidateQuantities/NuTauCandidateMt',
            title = "m_{T} (tau + nu) (after ecal crack)",
            xAxis = 'Mt',
            name = "cutFlowControlPlots_NuTauCandidateMt_afterEcalCrack"
            ),
        drawJobConfigEntry(
            meName = 'TauNuCandidateQuantities/NuTauCandidateDPhi',
            title = "#Delta #Phi (after ecal crack)",
            xAxis = 'Phi',
            name = "cutFlowControlPlots_NuTauCandidateDPhi_afterEcalCrack"
            ),
        drawJobConfigEntry(
            meName = 'TauQuantities/TauLeadTrkNumPixelHits',
            title = "Tau lead. Track num. pixel hits (after ecal crack)",
            xAxis = 'unlabeled',
            name = "cutFlowControlPlots_tauLeadTrkNumPixelHits_afterEcalCrack"
            ),
        drawJobConfigEntry(
            meName = 'TauQuantities/TauLeadTrkNumStripHits',
            title = "Tau lead. Track num. strip hits (after ecal crack)",
            xAxis = 'unlabeled',
            name = "cutFlowControlPlots_tauLeadTrkNumStripHits_afterEcalCrack"
            )
        ]    
    )

