import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.Configuration.analyzeWtoTauNu_cfi import *
from TauAnalysis.DQMTools.drawJobConfigurator import *

# define template for all kinds of plots
# (specific to W --> tau nu analysis)
plots_WtoTauNu = cms.PSet(
    plots = cms.PSet(  
        dqmMonitorElements = cms.vstring(''),
        processes = cms.vstring(
            'Wtaunu',
            'Wmunu',
            'Wenu',
            'ZplusJets',
            'TTplusJets',
            'qcd_W'
            )
    ),
    xAxis = cms.string('unlabeled'),
    #yAxis = cms.string('numEntries_linear'),
    yAxis = cms.string('numEntries_log'),
    legend = cms.string('regular'),
    labels = cms.vstring('mcNormScale'),                   
    drawOptionSet = cms.string('default'),
  #  stack = cms.vstring(
  #      'Wtaunu',
  #      'Wmunu',
  #      'Wenu',
  #      'ZplusJets',
  #      'TTplusJets',
  #      'qcd_W'
  #  )
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
    afterCut = evtSelTauEta,
    beforeCut = evtSelTauPt,
    plots = [
        drawJobConfigEntry(
            meName = 'TauQuantities/Tau#PAR#',
            PAR = [ 'Pt', 'Eta', 'Phi' ],
            title = "Tau (after Tau #eta Cut)",
            xAxis = '#PAR#',
            name = "cutFlowControlPlots_tau_afterTauEta"
        ),
        drawJobConfigEntry(
            meName = 'TauQuantities/TauLeadTrkPt',
            title = "Tau lead. Track (after Tau #eta Cut)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_tauLeadTrkPt_afterTauEta"
            )
        ]
)
   
drawJobConfigurator_WtoTauNu.add(
    afterCut = evtSelTauCharge,
    beforeCut = evtSelCentralJetVeto,
    plots = [
        drawJobConfigEntry(
            meName = 'TauQuantities/Tau#PAR#',
            PAR = [ 'Pt', 'Eta', 'Phi' ],
            title = "Tau (after Tau charge cut)",
            xAxis = '#PAR#',
            name = "cutFlowControlPlots_tau_afterTauCharge"
            ),
        drawJobConfigEntry(
            meName = 'TauQuantities/TauLeadTrkPt',
            title = "Tau lead. Track (after Tau charge Cut)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_tauLeadTrkPt_afterTauCharge"
            ),
        drawJobConfigEntry(
            meName = 'TauRecoilEnergyFromCaloTowersQuantities/EtSum',
            title = "Recoil energy from Calo Towers (after Tau charge cut)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_recoilEnergyFromCaloTowers_afterTauCharge"
            ),
        drawJobConfigEntry(
            meName = 'TauRecoilEnergyFromJetsQuantities/EtSum',
            title = "Recoil energy from Jets (after Tau charge cut)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_recoilEnergyFromJets_afterTauCharge"
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
            )

        ]
)

#--------------------------------------------------------------------------------
# define distributions to be plotted
# for events passing all event selection criteria
#--------------------------------------------------------------------------------

drawJobConfigurator_WtoTauNu.add(
    afterCut = evtSelRecoilEnergyFromCaloTowers,
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
            )
        ]    
)
                
