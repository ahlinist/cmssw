import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.Configuration.analyzeZtoMuTau_cfi import *
from TauAnalysis.DQMTools.drawJobConfigurator import *

# define template for all kinds of plots
# (specific to Z --> mu + tau-jet analysis)
plots_ZtoMuTau = cms.PSet(
    plots = cms.PSet(  
        dqmMonitorElements = cms.vstring(''),
        processes = cms.vstring(
            'ZmumuPlusJets', 
            #'Zmumu',
            'WplusJets',
            'qcdSum',
            #'ZtautauPlusJets',
            'Ztautau',
        )
    ),
    xAxis = cms.string('unlabeled'),
    yAxis = cms.string('numEntries_linear'),
    #yAxis = cms.string('numEntries_log'),
    legend = cms.string('regular'),
    labels = cms.vstring('mcNormScale'),                   
    drawOptionSet = cms.string('default'),
    stack = cms.vstring(
        'ZmumuPlusJets',
        #'Zmumu',
        'WplusJets',
        'qcdSum',
        #'ZtautauPlusJets'
        'Ztautau'
    )
)

drawJobConfigurator_ZtoMuTau = drawJobConfigurator(
    template = plots_ZtoMuTau,
    dqmDirectory = '#PROCESSDIR#/zMuTauAnalyzer/'
)

#--------------------------------------------------------------------------------
# define cut-flow control plots;
# show distribution of each quantity used in event selection
# (**before** quantity is cutted on)
#--------------------------------------------------------------------------------

drawJobConfigurator_ZtoMuTau.add(
    afterCut = evtSelPrimaryEventVertex,
    beforeCut = evtSelPrimaryEventVertexQuality,
    plot = drawJobConfigEntry(
        meName = 'VertexQuantities/VertexChi2Prob',
        title = "P(#Chi^{2}_{vtx} (after primary Event Vertex Cut)",
        xAxis = 'prob',
        name = "cutFlowControlPlots_vertexChi2Prob_afterPrimaryEventVertex"
    )
)

drawJobConfigurator_ZtoMuTau.add(
    afterCut = evtSelPrimaryEventVertexQuality,
    beforeCut = evtSelPrimaryEventVertexPosition,
    plot = drawJobConfigEntry(
        meName = 'VertexQuantities/VertexZ',
        title = "z_{vtx} (after primary Event Vertex quality Cut)",
        xAxis = 'posZ',
        name = "cutFlowControlPlots_vertexZ_afterPrimaryEventVertexQuality"
    )
)

drawJobConfigurator_ZtoMuTau.add(
    afterCut = evtSelPrimaryEventVertexPosition,
    beforeCut = evtSelGlobalMuon,
    plot = drawJobConfigEntry(
        meName = 'MuonQuantities/Muon#PAR#',
        PAR = [ 'Pt', 'Eta', 'Phi' ],
        title = "Muon (after primary Event Vertex position Cut)",
        xAxis = '#PAR#',
        name = "cutFlowControlPlots_muon_afterPrimaryEventVertexPosition"
    )
)    

drawJobConfigurator_ZtoMuTau.add(
    afterCut = evtSelGlobalMuon,
    beforeCut = evtSelMuonEta,
    plot = drawJobConfigEntry(
        meName = 'MuonQuantities/Muon#PAR#',
        PAR = [ 'Pt', 'Eta', 'Phi' ],
        title = "Muon (after global Muon Cut)",
        xAxis = '#PAR#',
        name = "cutFlowControlPlots_muon_afterGlobalMuon"
    )
)

drawJobConfigurator_ZtoMuTau.add(
    afterCut = evtSelMuonEta,
    beforeCut = evtSelMuonPt,
    plot = drawJobConfigEntry(
        meName = 'MuonQuantities/Muon#PAR#',
        PAR = [ 'Pt', 'Eta', 'Phi' ],
        title = "Muon (after Muon #eta Cut)",
        xAxis = '#PAR#',
        name = "cutFlowControlPlots_muon_afterMuonEta"
    )
)    

drawJobConfigurator_ZtoMuTau.add(
    afterCut = evtSelMuonPt,
    beforeCut = evtSelTauAntiOverlapWithMuonsVeto,
    plot = drawJobConfigEntry(
        meName = 'TauQuantities/Tau#PAR#',
        PAR = [ 'Pt', 'Eta', 'Phi' ],
        title = "Tau (after Muon P_{T} Cut)",
        xAxis = '#PAR#',
        name = "cutFlowControlPlots_tau_afterMuonPt"
    )
)

drawJobConfigurator_ZtoMuTau.add(
    afterCut = evtSelTauAntiOverlapWithMuonsVeto,
    beforeCut = evtSelTauEta,
    plots = [
        drawJobConfigEntry(
            meName = 'TauQuantities/Tau#PAR#',
            PAR = [ 'Pt', 'Eta', 'Phi' ],
            title = "Tau (after Muon-Tau overlap Veto)",
            xAxis = '#PAR#',
            name = "cutFlowControlPlots_tau_afterTauAntiOverlapWithMuonsVeto"
        ),
        drawJobConfigEntry(
            meName = 'TauQuantities/TauLeadTrkPt',
            title = "Tau lead. Track (after Muon-Tau overlap Veto)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_tauLeadTrkPt_afterTauAntiOverlapWithMuonsVeto"
        )
    ]
)

drawJobConfigurator_ZtoMuTau.add(
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

drawJobConfigurator_ZtoMuTau.add(
    afterCut = evtSelTauPt,
    beforeCut = evtSelMuonTrkIso,
    plot = drawJobConfigEntry(
        meName = 'MuonQuantities/MuonTrkIsoPt',
        title = "Muon Track iso. (after Tau P_{T} Cut)",
        xAxis = 'Pt',
        name = "cutFlowControlPlots_muonTrkIso_afterTauPt"
    )
)

drawJobConfigurator_ZtoMuTau.add(
    afterCut = evtSelMuonTrkIso,
    beforeCut = evtSelMuonEcalIso,
    plot = drawJobConfigEntry(
        meName = 'MuonQuantities/MuonEcalIsoPt',
        title = "Muon ECAL iso. (after Muon Track iso. Cut)",
        xAxis = 'Pt',
        name = "cutFlowControlPlots_muonEcalIso_afterMuonTrkIso"
    )
)

drawJobConfigurator_ZtoMuTau.add(
    afterCut = evtSelMuonEcalIso,
    beforeCut = evtSelMuonAntiPion,
    plot = drawJobConfigEntry(
        meName = 'MuonQuantities/Muon#PAR#Compatibility',
        PAR = [ 'Calo', 'Segment' ],
        title = "Muon #PAR# compatibility (after Muon ECAL iso. Cut)",
        xAxis = 'prob',
        name = "cutFlowControlPlots_muonComp_afterMuonEcalIso"
    )
)

drawJobConfigurator_ZtoMuTau.add(
    afterCut = evtSelMuonAntiPion,
    beforeCut = evtSelMuonTrkIP,
    plot = drawJobConfigEntry(
        meName = 'MuonQuantities/MuonTrackIP#PAR#',
        PAR = [ 'xy', 'z' ],
        title = "Muon Track IP_{#PAR#}(after Muon #pi-Veto)",
        xAxis = 'IP#PAR#',
        name = "cutFlowControlPlots_muonTrkIP_afterMuonAntiPionVeto"
    )
)

drawJobConfigurator_ZtoMuTau.add(
    afterCut = evtSelMuonTrkIP,
    beforeCut = evtSelTauLeadTrk,
    plots = [
        drawJobConfigEntry(
            meName = 'TauQuantities/Tau#PAR#',
            PAR = [ 'Pt', 'Eta', 'Phi' ],
            title = "Tau (after Muon Track IP_{xy} Cut)",
            xAxis = '#PAR#',
            name = "cutFlowControlPlots_tau_afterMuonTrkIP"
        ),
        drawJobConfigEntry(
            meName = 'TauQuantities/TauLeadTrkPt',
            title = "Tau lead. Track (after Muon Track IP_{xy} Cut)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_tauLeadTrkPt_afterMuonTrkIP"
        )
    ]
)

drawJobConfigurator_ZtoMuTau.add(
    afterCut = evtSelTauLeadTrk,
    beforeCut = evtSelTauLeadTrkPt,
    plots = [
        drawJobConfigEntry(
            meName = 'TauQuantities/Tau#PAR#',
            PAR = [ 'Pt', 'Eta', 'Phi' ],
            title = "Tau (after Tau lead. Track Cut)",
            xAxis = '#PAR#',
            name = "cutFlowControlPlots_tau_afterTauLeadTrk"
        ),
        drawJobConfigEntry(
            meName = 'TauQuantities/TauLeadTrkPt',
            title = "Tau lead. Track (after Tau lead. Track Cut)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_tauLeadTrkPt_afterTauLeadTrk"
       )
   ]
)

drawJobConfigurator_ZtoMuTau.add(
    afterCut = evtSelTauLeadTrkPt,
    beforeCut = evtSelTauTrkIso,
    plot = drawJobConfigEntry(
        meName = 'TauQuantities/TauTrkIsoPt',
        title = "Tau Track iso. (after Tau lead. Track P_{T} Cut)",
        xAxis = 'Pt',
        name = "cutFlowControlPlots_tauTrkIso_afterTauLeadTrkPt"
    )
)

drawJobConfigurator_ZtoMuTau.add(
    afterCut = evtSelTauTrkIso,
    beforeCut = evtSelTauEcalIso,
    plot = drawJobConfigEntry(
        meName = 'TauQuantities/TauEcalIsoPt',
        title = "Tau ECAL iso. (after Tau Track iso. Cut)",
        xAxis = 'Pt',
        name = "cutFlowControlPlots_tauEcalIso_afterTauTrkIso"
    )
)

drawJobConfigurator_ZtoMuTau.add(
    afterCut = evtSelTauEcalIso,
    beforeCut = evtSelTauProng,
    plot = drawJobConfigEntry(
        meName = 'TauQuantities/TauNumTracksSignalCone',
        title = "Tau Tracks in Signal Cone (after Tau ECAL iso. Cut)",
        xAxis = 'N',
        name = "cutFlowControlPlots_tauNumTracksSignalCone_afterTauEcalIso"
    )
)

drawJobConfigurator_ZtoMuTau.add(
    afterCut = evtSelTauProng,
    beforeCut = evtSelTauMuonVeto,
    plots = [
        drawJobConfigEntry(
            meName = 'TauQuantities/Tau#PAR#',
            PAR = [ 'Pt', 'Eta', 'Phi' ],
            title = "Tau (after Tau 1-Prong||3-Prong Cut)",
            xAxis = '#PAR#',
            name = "cutFlowControlPlots_tau_afterTauProng"
        ),
        drawJobConfigEntry(
            meName = 'TauQuantities/TauDiscriminatorAgainstMuons',
            title = "Tau anti-Muon Discr. (after Tau 1-Prong||3-Prong Cut)",
            xAxis = 'unlabeled',
            name = "cutFlowControlPlots_tauAntiMuonDiscr_afterTauProng"
        )
    ]
)

drawJobConfigurator_ZtoMuTau.add(
    afterCut = evtSelTauMuonVeto,
    beforeCut = evtSelDiTauCandidateForMuTauAntiOverlapVeto,
    plot = drawJobConfigEntry(
        meName = 'DiTauCandidateQuantities/DR12',
        title = "#Delta R(Muon,Tau) (after Tau #mu-Veto)",
        xAxis = 'dR',
        name = "cutFlowControlPlots_dR12_afterTauMuonVeto"
    )
)

drawJobConfigurator_ZtoMuTau.add(
    afterCut = evtSelDiTauCandidateForMuTauAntiOverlapVeto,
    beforeCut = evtSelDiTauCandidateForMuTauZeroCharge,
    plot = drawJobConfigEntry(
        meName = 'DiTauCandidateQuantities/DiTauCandidateCharge',
        title = "Charge(Muon + Tau) (after diTau anti-Overlap Veto)",
        xAxis = 'unlabeled',
        name = "cutFlowControlPlots_diTauCharge_afterAntiOverlapVeto"
    )
)

drawJobConfigurator_ZtoMuTau.add(
    afterCut = evtSelDiTauCandidateForMuTauZeroCharge,
    beforeCut = evtSelDiTauCandidateForMuTauMt1MET,
    plot = drawJobConfigEntry(
        meName = 'DiTauCandidateQuantities/Mt1MET',
        title = "M_{T}(Muon + MET) (after opposite Charge Cut)",
        xAxis = 'Mt',
        name = "cutFlowControlPlots_mtMuonMET_afterZeroCharge"
    )
)

#--------------------------------------------------------------------------------
# define distributions to be plotted
# for events passing all event selection criteria
#--------------------------------------------------------------------------------

drawJobConfigurator_ZtoMuTau.add(
    afterCut = evtSelDiTauCandidateForMuTauMt1MET,
    plots = [
        drawJobConfigEntry(
            meName = 'MuonQuantities/Muon#PAR#',
            PAR = [ 'Pt', 'Eta', 'Phi' ],
            title = "Muon (final Event sample)",
            xAxis = '#PAR#',
            name = "finalSamplePlots_muon"
        ),
        drawJobConfigEntry(
            meName = 'TauQuantities/Tau#PAR#',
            PAR = [ 'Pt', 'Eta', 'Phi' ],
            title = "Tau (final Event sample)",
            xAxis = '#PAR#',
            name = "finalSamplePlots_tau"
        ),
        drawJobConfigEntry(
            meName = 'TauQuantities/TauLeadTrkPt',
            title = "Tau lead. Track (final Event sample)",
            xAxis = 'Pt',
            name = "finalSamplePlots_tauLeadTrkPt"
        ),
        drawJobConfigEntry(
            meName = 'TauQuantities/TauNumTracksSignalCone',
            title = "Tau Tracks in Signal Cone (final Event sample)",
            xAxis = 'N',
            name = "finalSamplePlots_tauNumTracksSignalCone"
        ),
        drawJobConfigEntry(
            meName = 'TauQuantities/TauDiscriminatorTaNCfrQuarterPercent',
            title = "TaNC output (fr = 0.25%) (final Event sample)",
            xAxis = 'unlabeled',
            name = "finalSamplePlots_tauDiscrTaNCfrQuarterPercent"
        ),
        drawJobConfigEntry(
            meName = 'TauQuantities/TauCaloEOverPLead',
            title = "E_{calo} / P_{lead. Track} (final Event sample)",
            xAxis = 'unlabeled',
            name = "finalSamplePlots_tauCaloEOverPLead"
        ),
        drawJobConfigEntry(
            meName = 'MEtQuantities/RAWplusJESplusMUONplusTAU_MEtPt',
            title = "MET (final Event sample)",
            xAxis = 'Pt',
            name = "finalSamplePlots_met"
        ),
        drawJobConfigEntry(
            meName = 'DiTauCandidateQuantities/PzetaDiff',
            title = "P_{#zeta} - 1.5*P_{#zeta}^{vis} (final Event sample)",
            xAxis = 'GeV',
            name = "finalSamplePlots_PzetaDiff"
        ),
        drawJobConfigEntry(
            meName = 'DiTauCandidateQuantities/Mt1MET',
            title = "M_{T}(Muon + MET) (final Event sample)",
            xAxis = 'Mt',
            name = "finalSamplePlots_mtMuonMET"
        ),
        drawJobConfigEntry(
            meName = 'DiTauCandidateQuantities/Mt2MET',
            title = "M_{T}(Tau + MET) (final Event sample)",
            xAxis = 'Mt',
            name = "finalSamplePlots_mtTauMET"
        ),
        drawJobConfigEntry(
            meName = 'DiTauCandidateQuantities/Mt12MET',
            title = "M_{T}(Muon + Tau + MET) (final Event sample)",
            xAxis = 'Mt',
            name = "finalSamplePlots_mtMuonTauMET"
        ),
        drawJobConfigEntry(
            meName = 'DiTauCandidateQuantities/VisMass',
            title = "M_{vis}(Muon + Tau) (final Event sample)",
            xAxis = 'Mass',
            name = "finalSamplePlots_mVisible"
        ),        
        drawJobConfigEntry(
            meName = 'DiTauCandidateQuantities/CDFmethodMass',
            title = "M(Muon + Tau), CDF method (final Event sample)",
            xAxis = 'Mass',
            name = "finalSamplePlots_mCDFmethod"
        ),
        drawJobConfigEntry(
            meName = 'DiTauCandidateQuantities/CollinearApproxMass',
            title = "M(Muon + Tau), collinear Approx. (final Event sample)",
            xAxis = 'Mass',
            name = "finalSamplePlots_mCollApprox"
        ),
        drawJobConfigEntry(
            meName = 'JetQuantities/numJetsEtGt#PAR#_0EtaLt2_1AlphaGt0_3',
            PAR = [ '15', '20', '30' ],
            title = "N_{jets} with E_{T} > #PAR# GeV, |#eta| < 2.1, #alpha > 0.3 (final Event sample)",
            xAxis = 'N',
            name = "finalSamplePlots_numCentralJets"
        )
    ]
)                
