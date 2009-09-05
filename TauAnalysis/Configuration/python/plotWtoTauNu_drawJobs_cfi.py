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
            'qcd_W',
            'Wmunu',
            'Wenu',
            'ZplusJets',
            'TTplusJets'
            )
    ),
    xAxis = cms.string('unlabeled'),
    yAxis = cms.string('numEntries_linear'),
    #yAxis = cms.string('numEntries_log'),
    legend = cms.string('regular'),
    labels = cms.vstring('mcNormScale'),                   
    drawOptionSet = cms.string('default'),
    stack = cms.vstring(
        'Wtaunu',
        'qcd_W',
        'Wmunu',
        'Wenu',
        'ZplusJets',
        'TTplusJets'
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
    afterCut = evtSelTauProng,
    beforeCut = evtSelRecoilEnergy,
    plots = [
        drawJobConfigEntry(
            meName = 'TauQuantities/Tau#PAR#',
            PAR = [ 'Pt', 'Eta', 'Phi' ],
            title = "Tau (after Tau prong Cut)",
            xAxis = '#PAR#',
            name = "cutFlowControlPlots_tau_afterTauProng"
            ),
        drawJobConfigEntry(
            meName = 'TauQuantities/TauLeadTrkPt',
            title = "Tau lead. Track (after Tau prong Cut)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_tauLeadTrkPt_afterTauProng"
            ),
        drawJobConfigEntry(
            meName = 'TauRecoilEnergyFromCaloTowersQuantities/EtSum',
            title = "Recoil energy from Calo Towers (after Tau prong cut)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_recoilEnergyFromCaloTowers_afterTauProng"
            ),
        drawJobConfigEntry(
            meName = 'TauRecoilEnergyFromJetsQuantities/EtSum',
            title = "Recoil energy from Jets (after Tau prong cut)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_recoilEnergyFromJets_afterTauProng"
            )
        ]
)

drawJobConfigurator_WtoTauNu.add(
    afterCut = evtSelRecoilEnergy,
    beforeCut = evtSelCentralJetVeto,
    plots = [
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
            name = "cutFlowControlPlots_MEtTopology_afterRecoilEnergy"
            )
        ]
)

#--------------------------------------------------------------------------------
# define distributions to be plotted
# for events passing all event selection criteria
#--------------------------------------------------------------------------------

drawJobConfigurator_WtoTauNu.add(
    afterCut = evtSelCentralJetVeto,
    plots =  [
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
            title = "Recoil energy from Calo Towers (after recoil jet veto cut)",
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
            name = "cutFlowControlPlots_METtopology_afterJetVeto"
            )
        ]    
)
                
