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
    beforeCut = evtSelTauLeadTrk,
    plot = drawJobConfigEntry(
       meName = 'TauQuantities/Tau#PAR#',
       PAR = [ 'Pt', 'Eta', 'Phi' ],
       title = "Tau (after tau p_{T})",
       xAxis = '#PAR#',
       name = "cutFlowControlPlots_tau_afterTauPt"
       )    
    )

drawJobConfigurator_WtoTauNu.add(
    afterCut = evtSelHtRatio,
#    beforeCut = evtSelMetTopology,
    plots = [
    drawJobConfigEntry(
    meName = 'TauQuantities/TauMatchingGenParticlePdgId',
                title = "Matching gen-particle Pdg-Id (after HT-ratio)",
                xAxis = 'unlabeled',
                name = "cutFlowControlPlots_jetPt_afterHtRatio"
    ),
    drawJobConfigEntry(
    meName = 'MEtTopologyQuantities/Vratio',
               title = "MET topology (after HT-ratio)",
               xAxis = 'prob',
               name = "cutFlowControlPlots_METtopology_afterHtRatio"
    ),
            drawJobConfigEntry(
               meName = 'HtRatioQuantities/Ratio',
                          title = "HT-ratio (after Ht-ratio)",
                          xAxis = 'prob',
                          name = "cutFlowControlPlots_HtRatio_afterHtRatio"
                          ),
            drawJobConfigEntry(
                meName = 'TauNuCandidateQuantities/NuTauCandidateMt',
                            title = "m_{T} (tau + nu) (after HT-ratio)",
                            xAxis = 'Mt',
                            name = "cutFlowControlPlots_NuTauCandidateMt_afterHtRatio"
                            ),
    drawJobConfigEntry(
                meName = 'TauNuCandidateQuantities/NuTauCandidateMtSVfit',
                            title = "m_{T} (tau + nu) SV-fit (after HT-ratio)",
                            xAxis = 'Mt',
                            name = "cutFlowControlPlots_NuTauCandidateMtSVfit_afterHtRatio"
                            ) 
    ]
    )
#--------------------------------------------------------------------------------
# define distributions to be plotted
# for events passing all event selection criteria
#--------------------------------------------------------------------------------
#drawJobConfigurator_WtoTauNu.add(
#    afterCut = evtSelMetTopology,
#    plots =  [
#        drawJobConfigEntry(
#            meName = 'TauQuantities/TauMatchingGenParticlePdgId',
#            title = "Matching gen-particle Pdg-Id (after MET-topology)",
#            xAxis = 'unlabeled',
#            name = "cutFlowControlPlots_jetPt_afterMetTopology"
#            ), 
#        drawJobConfigEntry(
#            meName = 'TauQuantities/Tau#PAR#',
#            PAR = [ 'Pt', 'Eta', 'Phi' ],
#            title = "Tau (after MET-topology)",
#            xAxis = '#PAR#',
#            name = "cutFlowControlPlots_tau_afterMetTopology"
#            ),
#        drawJobConfigEntry(
#            meName = 'TauQuantities/TauNumTracksSignalCone',
#            title = "Tau prong (after MET-topology)",
#            xAxis = 'unlabeled',
#            name = "cutFlowControlPlots_tauNumTracksSignalCone_afterMetTopology"
#            ),
#        drawJobConfigEntry(
#            meName = 'TauQuantities/TauLeadTrkPt',
#            title = "Tau lead. Track (after MET-topology)",
#            xAxis = 'Pt',
#            name = "cutFlowControlPlots_tauLeadTrkPt_afterMetTopology"
#            ),
 #       drawJobConfigEntry(
#            meName = 'PFMEtQuantities/MEtPt',
#            title = "PF-MET (after MET-topology)",
 #           xAxis = 'Pt',
#            name = "cutFlowControlPlots_PFMET_afterMetTopology"
#            ),           
#        drawJobConfigEntry(
#            meName = 'TauRecoilEnergyFromCaloTowersQuantities/EtSum',
#            title = "Recoil energy from Calo Towers (after MET-topology)",
#            xAxis = 'Pt',
#            name = "cutFlowControlPlots_recoilEnergyFromCaloTowers_afterMetTopology"
#            ),
#        drawJobConfigEntry(
#            meName = 'MEtTopologyQuantities/Vratio',
#            title = "MET topology (after MET-topology)",
#            xAxis = 'prob',
#            name = "cutFlowControlPlots_METtopology_afterMetTopology"
#            ),
#        drawJobConfigEntry(
#           meName = 'HtRatioQuantities/Ratio',
#           title = "HT-ratio (after MET-topology)",
#           xAxis = 'prob',
#           name = "cutFlowControlPlots_HtRatio_afterMetTopology"
#           ),
#        drawJobConfigEntry(
#            meName = 'TauNuCandidateQuantities/NuTauCandidateMt',
#            title = "m_{T} (tau + nu) (after MET-topology)",
#            xAxis = 'Mt',
#            name = "cutFlowControlPlots_NuTauCandidateMt_afterMetTopology"
#            ),
#        drawJobConfigEntry(
#            meName = 'TauNuCandidateQuantities/NuTauCandidateMtSVfit',
#            title = "m_{T} (tau + nu) SV-fit (after MET-topology)",
#            xAxis = 'Mt',
#            name = "cutFlowControlPlots_NuTauCandidateMtSVfit_afterMetTopology"
#            ),
#        drawJobConfigEntry(
#            meName = 'TauNuCandidateQuantities/NuTauCandidateDPhi',
#            title = "#Delta #Phi (after MET-topology)",
#            xAxis = 'Phi',
#            name = "cutFlowControlPlots_NuTauCandidateDPhi_afterMetTopology"
#            ),
#        drawJobConfigEntry(
#            meName = 'TauQuantities/TauLeadTrkNumPixelHits',
#            title = "Tau lead. Track num. pixel hits (after MET-topology)",
#            xAxis = 'unlabeled',
#            name = "cutFlowControlPlots_tauLeadTrkNumPixelHits_afterMetTopology"
#            ),
#    drawJobConfigEntry(
#            meName = 'TauQuantities/TauLeadTrkNumStripHits',
#            title = "Tau lead. Track num. strip hits (after MET-topology)",
#            xAxis = 'unlabeled',
#            name = "cutFlowControlPlots_tauLeadTrkNumStripHits_afterMetTopology"
#            ),
#    drawJobConfigEntry(
#            meName = 'TauQuantities/TauEmFraction',
#            title = "EM-fraction (after MET-topology)",
#            xAxis = 'prob',
#            name = "cutFlowControlPlots_tauEmFraction_afterMetTopology"     
#            )
    
 #       ]    
 #   )

