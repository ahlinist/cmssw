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
            'Wmunu',
            'Wenu',
            'ZplusJets',
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
    'Wmunu',
    'Wenu',
    'ZplusJets',
    'qcd_W',
    'Wtaunu'
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
    afterCut = evtSelPFMetPt,
    beforeCut = evtSelMetPt,
    plots = [
        drawJobConfigEntry(
            meName = 'CaloMEtQuantities/RAW_MEtPt',
            title = "calo-MET (after PF-MET cut)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_MET_afterPFMETcut"
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
            meName = 'CaloMEtQuantities/RAW_MEtPt',
            title = "calo-MET (after jet veto cut)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_MET_afterJetVeto"
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
            title = "#Delta #Phi (after jet veto cut)",
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

#--------------------------------------------------------------------------------
# define distributions to be plotted
# for events passing all event selection criteria
#--------------------------------------------------------------------------------
drawJobConfigurator_WtoTauNu.add(
    afterCut = evtSelTauEcalCrackVeto,
    beforeCut = evtSelCentralJetVeto,
    plots =  [
        drawJobConfigEntry(
            meName = 'TauQuantities/Tau#PAR#',
            PAR = [ 'Pt', 'Eta', 'Phi' ],
            title = "Tau (after ecal crack)",
            xAxis = '#PAR#',
            name = "cutFlowControlPlots_tau_afterEcalCrack"
            ),
        drawJobConfigEntry(
            meName = 'TauQuantities/TauLeadTrkPt',
            title = "Tau lead. Track (after ecal crack)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_tauLeadTrkPt_afterEcalCrack"
            ),
          drawJobConfigEntry(
            meName = 'CaloMEtQuantities/RAW_MEtPt',
            title = "calo-MET (after ecal crack)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_MET_afterEcalCrack"
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
            meName = 'TauRecoilEnergyFromJetsQuantities/EtSum',
            title = "Recoil energy from Jets (after ecal crack)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_recoilEnergyFromJets_afterEcalCrack"
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
            meName = 'CaloMEtQuantities/RAW_MEtSignificance',
            title = "MET significance (after ecal crack)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_MEtSignificance_afterEcalCrack"
            ),
        drawJobConfigEntry(
            meName = 'PFEventShapeVars/Circularity',
            title = "Circularity (after ecal crack)",
            xAxis = 'prob',
            name = "cutFlowControlPlots_Circularity_afterEcalCrack"
            ),
  drawJobConfigEntry(
            meName = 'PFEventShapeVars/C',
            title = "C (after ecal crack)",
            xAxis = 'prob',
            name = "cutFlowControlPlots_C_afterEcalCrack"
            ),
        drawJobConfigEntry(
            meName = 'PFEventShapeVars/Aplanarity',
            title = "AplanarityCircularity (after ecal crack)",
            xAxis = 'prob',
            name = "cutFlowControlPlots_Aplanarity_afterEcalCrack"
            ),
        drawJobConfigEntry(
            meName = 'PFEventShapeVars/D',
            title = "D (after ecal crack)",
            xAxis = 'prob',
            name = "cutFlowControlPlots_D_afterEcalCrack"
            ),
        drawJobConfigEntry(
            meName = 'PFEventShapeVars/Sphericity',
            title = "Spherisity (after ecal crack)",
            xAxis = 'prob',
            name = "cutFlowControlPlots_Sphericity_afterEcalCrack"
            ),
        drawJobConfigEntry(
            meName = 'PFEventShapeVars/Thrust',
            title = "Thrust (after ecal crack)",
            xAxis = 'prob',
            name = "cutFlowControlPlots_Thrust_afterEcalCrack"
            )
        
        ]    
)


drawJobConfigurator_WtoTauNu.add(
    afterCut = evtSelRecoilEnergyFromCaloTowers,
    beforeCut = evtSelMetTopology,
    plots =  [
        drawJobConfigEntry(
            meName = 'TauQuantities/Tau#PAR#',
            PAR = [ 'Pt', 'Eta', 'Phi' ],
            title = "Tau (after recoil energy cut)",
            xAxis = '#PAR#',
            name = "cutFlowControlPlots_tau_afterRecoilEnergy"
            ),
        drawJobConfigEntry(
            meName = 'TauQuantities/TauLeadTrkPt',
            title = "Tau lead. Track (after recoil energy cut)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_tauLeadTrkPt_afterRecoilEnergy"
            ),
          drawJobConfigEntry(
            meName = 'CaloMEtQuantities/RAW_MEtPt',
            title = "calo-MET (after recoil energy cut)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_MET_afterRecoilEnergy"
            ), 
        drawJobConfigEntry(
            meName = 'PFMEtQuantities/MEtPt',
            title = "Pf-MET (after recoil energy cut)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_PFMET_afterRecoilEnergy"
            ),           
        drawJobConfigEntry(
            meName = 'TauRecoilEnergyFromCaloTowersQuantities/EtSum',
            title = "Recoil energy from Calo Towers (after recoil energy cut)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_recoilEnergyFromCaloTowers_afterRecoilEnergy"
            ),
        drawJobConfigEntry(
            meName = 'TauRecoilEnergyFromJetsQuantities/EtSum',
            title = "Recoil energy from Jets (after recoil energy cut)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_recoilEnergyFromJets_afterRecoilEnergy"
            ),
        drawJobConfigEntry(
            meName = 'MEtTopologyQuantities/Vratio',
            title = "MET topology (after recoil energy cut)",
            xAxis = 'prob',
            name = "cutFlowControlPlots_METtopology_afterRecoilEnergy"
            ),
        drawJobConfigEntry(
            meName = 'TauNuCandidateQuantities/NuTauCandidateMt',
            title = "m_{T} (tau + nu) (after recoil energy cut)",
            xAxis = 'Mt',
            name = "cutFlowControlPlots_NuTauCandidateMt_afterRecoilEnergy"
            ),
        drawJobConfigEntry(
            meName = 'TauNuCandidateQuantities/NuTauCandidateDPhi',
            title = "#Delta #Phi (after recoil energy cut)",
            xAxis = 'Phi',
            name = "cutFlowControlPlots_NuTauCandidateDPhi_afterRecoilEnergy"
            ),
        drawJobConfigEntry(
            meName = 'CaloMEtQuantities/RAW_MEtSignificance',
            title = "MET significance (after recoil energy cut)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_MEtSignificance_afterRecoilEnergy"
            ),
        drawJobConfigEntry(
            meName = 'PFEventShapeVars/Circularity',
            title = "Circularity (after recoil energy cut)",
            xAxis = 'prob',
            name = "cutFlowControlPlots_Circularity_afterRecoilEnergy"
            ),
  drawJobConfigEntry(
            meName = 'PFEventShapeVars/C',
            title = "C (after recoil energy cut)",
            xAxis = 'prob',
            name = "cutFlowControlPlots_C_afterRecoilEnergy"
            ),
        drawJobConfigEntry(
            meName = 'PFEventShapeVars/Aplanarity',
            title = "AplanarityCircularity (after recoil energy cut)",
            xAxis = 'prob',
            name = "cutFlowControlPlots_Aplanarity_afterRecoilEnergy"
            ),
        drawJobConfigEntry(
            meName = 'PFEventShapeVars/D',
            title = "D (after recoil energy cut)",
            xAxis = 'prob',
            name = "cutFlowControlPlots_D_afterRecoilEnergy"
            ),
        drawJobConfigEntry(
            meName = 'PFEventShapeVars/Sphericity',
            title = "Spherisity (after recoil energy cut)",
            xAxis = 'prob',
            name = "cutFlowControlPlots_Sphericity_afterRecoilEnergy"
            ),
        drawJobConfigEntry(
            meName = 'PFEventShapeVars/Thrust',
            title = "Thrust (after recoil energy cut)",
            xAxis = 'prob',
            name = "cutFlowControlPlots_Thrust_afterRecoilEnergy"
            )
        
        ]    
)

drawJobConfigurator_WtoTauNu.add(
    afterCut = evtSelMetTopology,
    plots =  [
        drawJobConfigEntry(
            meName = 'TauQuantities/Tau#PAR#',
            PAR = [ 'Pt', 'Eta', 'Phi' ],
            title = "Tau (after met topology cut)",
            xAxis = '#PAR#',
            name = "cutFlowControlPlots_tau_afterMetTopoloty"
            ),
        drawJobConfigEntry(
            meName = 'TauQuantities/TauLeadTrkPt',
            title = "Tau lead. Track (after met topology cut)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_tauLeadTrkPt_afterMetTopoloty"
            ),
          drawJobConfigEntry(
            meName = 'CaloMEtQuantities/RAW_MEtPt',
            title = "calo-MET (after met topology cut)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_MET_afterMetTopoloty"
            ), 
        drawJobConfigEntry(
            meName = 'PFMEtQuantities/MEtPt',
            title = "Pf-MET (after met topology cut)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_PFMET_afterMetTopoloty"
            ),           
        drawJobConfigEntry(
            meName = 'TauRecoilEnergyFromCaloTowersQuantities/EtSum',
            title = "Recoil energy from Calo Towers (after met topology cut)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_recoilEnergyFromCaloTowers_afterMetTopoloty"
            ),
        drawJobConfigEntry(
            meName = 'TauRecoilEnergyFromJetsQuantities/EtSum',
            title = "Recoil energy from Jets (after met topology cut)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_recoilEnergyFromJets_afterMetTopoloty"
            ),
        drawJobConfigEntry(
            meName = 'MEtTopologyQuantities/Vratio',
            title = "MET topology (after met topology cut)",
            xAxis = 'prob',
            name = "cutFlowControlPlots_METtopology_afterMetTopoloty"
            ),
        drawJobConfigEntry(
            meName = 'TauNuCandidateQuantities/NuTauCandidateMt',
            title = "m_{T} (tau + nu) (after met topology cut)",
            xAxis = 'Mt',
            name = "cutFlowControlPlots_NuTauCandidateMt_afterMetTopoloty"
            ),
        drawJobConfigEntry(
            meName = 'TauNuCandidateQuantities/NuTauCandidateDPhi',
            title = "#Delta #Phi (after met topology cut)",
            xAxis = 'Phi',
            name = "cutFlowControlPlots_NuTauCandidateDPhi_afterMetTopoloty"
            ),
        drawJobConfigEntry(
            meName = 'CaloMEtQuantities/RAW_MEtSignificance',
            title = "MET significance (after met topology cut)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_MEtSignificance_afterMetTopoloty"
            ),
        drawJobConfigEntry(
            meName = 'PFEventShapeVars/Circularity',
            title = "Circularity (after met topology cut)",
            xAxis = 'prob',
            name = "cutFlowControlPlots_Circularity_afterMetTopoloty"
            ),
  drawJobConfigEntry(
            meName = 'PFEventShapeVars/C',
            title = "C (after met topology cut)",
            xAxis = 'prob',
            name = "cutFlowControlPlots_C_afterMetTopoloty"
            ),
        drawJobConfigEntry(
            meName = 'PFEventShapeVars/Aplanarity',
            title = "AplanarityCircularity (after met topology cut)",
            xAxis = 'prob',
            name = "cutFlowControlPlots_Aplanarity_afterMetTopoloty"
            ),
        drawJobConfigEntry(
            meName = 'PFEventShapeVars/D',
            title = "D (after met topology cut)",
            xAxis = 'prob',
            name = "cutFlowControlPlots_D_afterMetTopoloty"
            ),
        drawJobConfigEntry(
            meName = 'PFEventShapeVars/Sphericity',
            title = "Spherisity (after met topology cut)",
            xAxis = 'prob',
            name = "cutFlowControlPlots_Sphericity_afterMetTopoloty"
            ),
        drawJobConfigEntry(
            meName = 'PFEventShapeVars/Thrust',
            title = "Thrust (after met topology cut)",
            xAxis = 'prob',
            name = "cutFlowControlPlots_Thrust_afterMetTopoloty"
            )
        ]
    )
                
