import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.Configuration.analyzeZtoMuTau_cfi import *
from TauAnalysis.DQMTools.tools.drawJobConfigurator import *

# define template for all kinds of plots
# (specific to Z --> mu + tau-jet analysis)
plots_ZtoMuTau = cms.PSet(
    plots = cms.PSet(
        dqmMonitorElements = cms.vstring(''),
        processes = cms.vstring(
            'TTplusJets',
            'Zmumu',
            'WplusJets',
            'qcdSum',            
            'ZtautauSum'
        )
    ),
    xAxis = cms.string('unlabeled'),
    yAxis = cms.string('numEntries_linear'),
    #yAxis = cms.string('numEntries_log'),
    legend = cms.string('regular'),
    labels = cms.vstring('mcNormScale'),
    drawOptionSet = cms.string('default'),
    stack = cms.vstring(
        'TTplusJets',
        'Zmumu',
        'WplusJets',
        'qcdSum',
        'ZtautauSum'
    )
)

drawJobConfigurator_ZtoMuTauOS = drawJobConfigurator(
    template = plots_ZtoMuTau,
    dqmDirectory = '#PROCESSDIR#/zMuTauAnalyzerOS/'
)

#--------------------------------------------------------------------------------
# define cut-flow control plots;
# show distribution of each quantity used in event selection
# (**before** quantity is cutted on)
#--------------------------------------------------------------------------------

drawJobConfigurator_ZtoMuTauOS.add(
    afterCut = evtSelPrimaryEventVertex,
    beforeCut = evtSelPrimaryEventVertexQuality,
    plot = drawJobConfigEntry(
        meName = 'VertexQuantities/VertexChi2Prob',
        title = "P(#Chi^{2}_{vtx} (after primary Event Vertex Cut)",
        xAxis = 'prob',
        name = "cutFlowControlPlots_vertexChi2Prob_afterPrimaryEventVertex"
    )
)

drawJobConfigurator_ZtoMuTauOS.add(
    afterCut = evtSelPrimaryEventVertexQuality,
    beforeCut = evtSelPrimaryEventVertexPosition,
    plot = drawJobConfigEntry(
        meName = 'VertexQuantities/VertexZ',
        title = "z_{vtx} (after primary Event Vertex quality Cut)",
        xAxis = 'posZ',
        name = "cutFlowControlPlots_vertexZ_afterPrimaryEventVertexQuality"
    )
)

drawJobConfigurator_ZtoMuTauOS.add(
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

drawJobConfigurator_ZtoMuTauOS.add(
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

drawJobConfigurator_ZtoMuTauOS.add(
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

drawJobConfigurator_ZtoMuTauOS.add(
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

drawJobConfigurator_ZtoMuTauOS.add(
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

drawJobConfigurator_ZtoMuTauOS.add(
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

drawJobConfigurator_ZtoMuTauOS.add(
    afterCut = evtSelTauPt,
    beforeCut = evtSelMuonVbTfId,
    plots = [
        drawJobConfigEntry(
            meName = 'MuonQuantities/MuonTypeTracker',
            title = "Muon type 'Tracker' (after Tau P_{T} Cut)",
            xAxis = 'unlabeled',
            name = "cutFlowControlPlots_muonTypeTracker_afterTauPt"
        ),
        drawJobConfigEntry(
            meName = 'MuonQuantities/MuonTypeTracker',
            title = "Muon global Track #Chi^{2} (after Tau P_{T} Cut)",
            xAxis = 'Chi2',
            name = "cutFlowControlPlots_muonGlobalTrackChi2_afterTauPt"
        ),
        drawJobConfigEntry(
            meName = 'MuonQuantities/MuonGlobalTrackIPxyBeamSpot',
            title = "Muon global Track IP_{xy} wrt. BeamSpot (after Tau P_{T} Cut)",
            xAxis = 'IPxy',
            name = "cutFlowControlPlots_muonGlobalTrackIPxyBeamSpot_afterTauPt"
        ),
        drawJobConfigEntry(
            meName = 'MuonQuantities/MuonNumTrackerHits',
            title = "Muon num. Tracker Hits (after Tau P_{T} Cut)",
            xAxis = 'N',
            name = "cutFlowControlPlots_muonNumTrackerHits_afterTauPt"
        ),
        drawJobConfigEntry(
            meName = 'MuonQuantities/MuonNumPixelHits',
            title = "Muon num. Pixel Hits (after Tau P_{T} Cut)",
            xAxis = 'N',
            name = "cutFlowControlPlots_muonNumPixelHits_afterTauPt"
        ),
        drawJobConfigEntry(
            meName = 'MuonQuantities/MuonNumberOfChambers',
            title = "Muon num. Chambers with Hits (after Tau P_{T} Cut)",
            xAxis = 'N',
            name = "cutFlowControlPlots_muonNumChamberHits_afterTauPt"
        ),
        drawJobConfigEntry(
            meName = 'MuonQuantities/MuonNumberOfSegments',
            title = "Muon num. Segments (after Tau P_{T} Cut)",
            xAxis = 'N',
            name = "cutFlowControlPlots_muonNumSegments_afterTauPt"
        )
    ]
)

drawJobConfigurator_ZtoMuTauOS.add(
    afterCut = evtSelMuonVbTfId,
    beforeCut = evtSelMuonPFRelIso,
    plots = [
        drawJobConfigEntry(
            meName = 'MuonQuantities/MuonParticleFlowIsoPt',
            title = "Muon PFCandidate iso. (after Muon VBTF id. Cut)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_muonPFCandidateAbsIso_afterMuonVbTfId"
        ),
        drawJobConfigEntry(
            meName = 'MuonQuantities/MuonParticleFlowIsoPtRel',
            title = "Muon PFCandidate rel. iso. (after Muon VBTF id. Cut)",
            xAxis = 'unlabeled',
            name = "cutFlowControlPlots_muonPFCandidateRelIso_afterMuonVbTfId"
        ),
        drawJobConfigEntry(
            meName = 'MuonQuantities/MuonPFChargedHadronIsoPt',
            title = "Muon PFChargedHadron iso. (after Muon VBTF id. Cut)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_muonPFChargedHadronAbsIso_afterMuonVbTfId"
        ),
        drawJobConfigEntry(
            meName = 'MuonQuantities/MuonPFChargedHadronIsoPtRel',
            title = "Muon PFChargedHadron rel. iso. (after Muon VBTF id. Cut)",
            xAxis = 'unlabeled',
            name = "cutFlowControlPlots_muonPFChargedHadronRelIso_afterMuonVbTfId"
        ),
        drawJobConfigEntry(
            meName = 'MuonQuantities/MuonPFNeutralHadronIsoPt',
            title = "Muon PFNeutralHadron iso. (after Muon VBTF id. Cut)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_muonPFNeutralHadronAbsIso_afterMuonVbTfId"
        ),
        drawJobConfigEntry(
            meName = 'MuonQuantities/MuonPFNeutralHadronIsoPtRel',
            title = "Muon PFNeutralHadron rel. iso. (after Muon VBTF id. Cut)",
            xAxis = 'unlabeled',
            name = "cutFlowControlPlots_muonPFNeutralHadronRelIso_afterMuonVbTfId"
        ),
        drawJobConfigEntry(
            meName = 'MuonQuantities/MuonPFGammaIsoPt',
            title = "Muon PFGamma iso. (after Muon VBTF id. Cut)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_muonPFGammaAbsIso_afterMuonVbTfId"
        ),
        drawJobConfigEntry(
            meName = 'MuonQuantities/MuonPFGammaIsoPtRel',
            title = "Muon PFGamma rel. iso. (after Muon VBTF id. Cut)",
            xAxis = 'unlabeled',
            name = "cutFlowControlPlots_muonPFGammaRelIso_afterMuonVbTfId"
        )
    ]
)

drawJobConfigurator_ZtoMuTauOS.add(
    afterCut = evtSelMuonPFRelIso,
    beforeCut = evtSelMuonTrkIP,
    plot = drawJobConfigEntry(
        meName = 'MuonQuantities/MuonTrackIP#PAR#',
        PAR = [ 'xy', 'z' ],
        title = "Muon Track IP_{#PAR#} (after Muon Combined iso. Cut)",
        xAxis = 'IP#PAR#',
        name = "cutFlowControlPlots_muonTrkIP_afterMuonCombIso"
    )
)

drawJobConfigurator_ZtoMuTauOS.add(
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

drawJobConfigurator_ZtoMuTauOS.add(
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

drawJobConfigurator_ZtoMuTauOS.add(
    afterCut = evtSelTauLeadTrkPt,
    beforeCut = evtSelTauTaNCdiscr,
    plot = drawJobConfigEntry(
        meName = 'TauQuantities/TauDiscriminatorTaNCfrQuarterPercent',
        title = "Tau TaNC discr. (after Tau lead. Track P_{T} Cut)",
        xAxis = 'Pt',
        name = "cutFlowControlPlots_tauTaNCdiscr_afterTauLeadTrkPt"
    )
)

drawJobConfigurator_ZtoMuTauOS.add(
    afterCut = evtSelTauTaNCdiscr,
    beforeCut = evtSelTauProng,
    plot = drawJobConfigEntry(
        meName = 'TauQuantities/TauNumTracksSignalCone',
        title = "Tau Tracks in Signal Cone (after Tau TaNC discr. Cut)",
        xAxis = 'unlabeled',
        name = "cutFlowControlPlots_tauNumTracksSignalCone_afterTauTaNCdiscr"
    )
)

drawJobConfigurator_ZtoMuTauOS.add(
    afterCut = evtSelTauProng,
    beforeCut = evtSelTauCharge,
    plot = drawJobConfigEntry(
        meName = 'TauQuantities/TauCharge',
        title = "Tau Charge (#Sigma Tracks in Signal Cone, after Tau 1-Prong||3-Prong Cut)",
        xAxis = 'unlabeled',
        name = "cutFlowControlPlots_tauCharge_afterTauProng"
    )
)

drawJobConfigurator_ZtoMuTauOS.add(
    afterCut = evtSelTauCharge,
    beforeCut = evtSelTauMuonVeto,
    plots = [
        drawJobConfigEntry(
            meName = 'TauQuantities/Tau#PAR#',
            PAR = [ 'Pt', 'Eta', 'Phi' ],
            title = "Tau (after Charge(Tau) = +/-1 Cut)",
            xAxis = '#PAR#',
            name = "cutFlowControlPlots_tau_afterTauCharge"
        ),
        drawJobConfigEntry(
            meName = 'TauQuantities/TauDiscriminatorAgainstMuons',
            title = "Tau anti-Muon Discr. (after Charge(Tau) = +/-1 Cut)",
            xAxis = 'unlabeled',
            name = "cutFlowControlPlots_tauAntiMuonDiscr_afterTauCharge"
        )
    ]
)

drawJobConfigurator_ZtoMuTauOS.add(
    afterCut = evtSelTauMuonVeto,
    beforeCut = evtSelTauElectronVeto,
    plots = [
        drawJobConfigEntry(
            meName = 'TauQuantities/Tau#PAR#',
            PAR = [ 'Pt', 'Eta', 'Phi' ],
            title = "Tau (after Tau #mu-Veto Cut)",
            xAxis = '#PAR#',
            name = "cutFlowControlPlots_tau_afterTauMuonVeto"
        ),
        drawJobConfigEntry(
            meName = 'TauQuantities/TauDiscriminatorAgainstElectrons',
            title = "Tau anti-Electron Discr. (after Tau #mu-Veto Cut)",
            xAxis = 'unlabeled',
            name = "cutFlowControlPlots_tauAntiElectronDiscr_afterTauMuonVeto"
        )
    ]
)

drawJobConfigurator_ZtoMuTauOS.add(
    afterCut = evtSelTauElectronVeto,
    beforeCut = evtSelDiTauCandidateForMuTauAntiOverlapVeto,
    plot = drawJobConfigEntry(
        meName = 'DiTauCandidateQuantities/DR12',
        title = "#Delta R(Muon,Tau) (after Tau e-Veto Cut)",
        xAxis = 'dR',
        name = "cutFlowControlPlots_dR12_afterTauElectronVeto"
    )
)

drawJobConfigurator_ZtoMuTauOS.add(
    afterCut = evtSelDiTauCandidateForMuTauAntiOverlapVeto,
    beforeCut = evtSelDiTauCandidateForMuTauMt1MET,
    plot = drawJobConfigEntry(
        meName = 'DiTauCandidateQuantities/Mt1MET',
        title = "M_{T}(Muon + MET) (after diTau anti-Overlap Veto)",
        xAxis = 'Mt',
        name = "cutFlowControlPlots_mtMuonMET_afterAntiOverlapVeto"
    )
)

drawJobConfigurator_ZtoMuTauOS.add(
    afterCut = evtSelDiTauCandidateForMuTauMt1MET,
    beforeCut = evtSelDiTauCandidateForMuTauPzetaDiff,
    plot = drawJobConfigEntry(
        meName = 'DiTauCandidateQuantities/PzetaDiff',
        title = "P_{#zeta} - 1.5*P_{#zeta}^{vis} (after transverse Mass Cut)",
        xAxis = 'GeV',
        name = "cutFlowControlPlots_PzetaDiff_afterMt1MET"
    )
)

##drawJobConfigurator_ZtoMuTauOS.add(
##    afterCut = evtSelDiTauCandidateForMuTauPzetaDiff,
##    beforeCut = evtSelDiMuPairZmumuHypothesisVetoByLooseIsolation,
##    plot = drawJobConfigEntry(
##	meName = 'DiMuZmumuHypothesisByLooseIsolationQuantities/DiTauCandidateCharge',
##        title = "Charge(iso. Muon + iso. Muon) (after P_{#zeta} Cut)",
##        xAxis = 'unlabeled',
##        name = "cutFlowControlPlots_diMuonCharge_afterPzetaDiff"
##    )
##)

drawJobConfigurator_ZtoMuTauOS.add(
    afterCut = evtSelDiMuPairZmumuHypothesisVetoByLooseIsolation,
    beforeCut = evtSelDiTauCandidateForMuTauZeroCharge,
    plot = drawJobConfigEntry(
        meName = 'DiTauCandidateQuantities/DiTauCandidateCharge',
        title = "Charge(Muon + Tau) (after Z #rightarrow #mu^{+} #mu^{-} hypothesis Veto)",
        xAxis = 'unlabeled',
        name = "cutFlowControlPlots_diTauCharge_afterZmumuHypothesisVeto"
    )
)

#--------------------------------------------------------------------------------
# define distributions to be plotted
# for events passing all event selection criteria
#--------------------------------------------------------------------------------

finalSamplePlots = [
    drawJobConfigEntry(
        meName = 'MuonQuantities/Muon#PAR#',
        PAR = [ 'Pt', 'Eta', 'Phi' ],
        title = "Muon (final Event sample)",
        xAxis = '#PAR#',
        name = "finalSamplePlots_muon"
    ),
    drawJobConfigEntry(
        meName = 'MuonQuantities/MuonCaloCompatibility',
        title = "Muon Calo. Compatibility (final Event sample)",
        xAxis = 'unlabeled',
        name = "finalSamplePlots_muonCaloCompatibility"
    ),
    drawJobConfigEntry(
        meName = 'MuonQuantities/MuonSegmentCompatibility',
        title = "Muon Segment Compatibility (final Event sample)",
        xAxis = 'unlabeled',
        name = "finalSamplePlots_muonSegmentCompatibility"
    ),
    drawJobConfigEntry(
        meName = 'MuonQuantities/MuonMatchingGenParticlePdgId',
        title = "PdgId of gen. Particle matching Muon (final Event sample)",
        xAxis = 'PdgId',
        name = "finalSamplePlots_pdgIdGenParticleMatchingMuon"
    ),
    drawJobConfigEntry(
        meName = 'TauQuantities/Tau#PAR#',
        PAR = [ 'Pt', 'Eta', 'Phi' ],
        title = "Tau (final Event sample)",
        xAxis = '#PAR#',
        name = "finalSamplePlots_tau"
    ),
    drawJobConfigEntry(
        meName = 'TauQuantities/TauMatchingGenParticlePdgId',
        title = "PdgId of gen. Particle matching Tau (final Event sample)",
        xAxis = 'PdgId',
        name = "finalSamplePlots_pdgIdGenParticleMatchingTau"
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
        xAxis = 'unlabeled',
        name = "finalSamplePlots_tauNumTracksSignalCone"
    ),
    drawJobConfigEntry(
        meName = 'TauQuantities/TauJetRadius',
        title = "Tau Jet Radius (final Event sample)",
        xAxis = 'unlabeled',
        name = "finalSamplePlots_tauJetRadius"
    ),
    drawJobConfigEntry(
        meName = 'TauQuantities/TauDiscriminatorTaNCfrQuarterPercent',
        title = "TaNC output (fr = 0.25%) (final Event sample)",
        xAxis = 'unlabeled',
        name = "finalSamplePlots_tauDiscrTaNCfrQuarterPercent"
    ),
    drawJobConfigEntry(
        meName = 'TauQuantities/TauTaNCoutputTransform',
        title = "TaNC output (transformed) (final Event sample)",
        xAxis = 'unlabeled',
        name = "finalSamplePlots_tauTaNCtransform"
    ),
    drawJobConfigEntry(
        meName = 'DiTauCandidateQuantities/DPhi12',
        title = "#Delta#phi(Muon-Tau) (final Event sample)",
        xAxis = 'dPhi',
        name = "finalSamplePlots_dPhiMuonTau"
    ),
    drawJobConfigEntry(
        meName = 'CaloMEtQuantities/RAWplusJESplusMUONplusTAU_MEtPt',
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
        meName = 'DiTauCandidateQuantities/VisMassZllCombinedHypothesis',
        title = "M_{vis}(Muon + Tau), Z #rightarrow #ell^{+} #ell^{-} combined Hypothesis (final Event sample)",
        xAxis = 'Mass',
        name = "finalSamplePlots_mVisibleZllCombinedHypothesis"
    ),
    drawJobConfigEntry(
        meName = 'DiTauCandidateQuantities/CDFmethodMass',
        title = "M(Muon + Tau + MET), CDF method (final Event sample)",
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
        meName = 'DiTauCandidateSVfitQuantities/psKine_MEt_ptBalance/Mass',
        title = "M(Muon + Tau), SVfit method (final Event sample)",
        xAxis = 'Mass',
        name = "finalSamplePlots_mSVmethod"
    ),
    drawJobConfigEntry(
        meName = 'DiTauCandidateQuantities/Ht12MET',
        title = "#Sigma H_{T}(Muon + Tau + MET) (final Event sample)",
        xAxis = 'Mass',
        name = "finalSamplePlots_ht"
    ),
    drawJobConfigEntry(
        meName = 'JetQuantities/numJetsEtGt#PAR#_0EtaLt2_1AlphaGt0_3',
        PAR = [ '15', '20', '30' ],
        title = "N_{jets} with E_{T} > #PAR# GeV, |#eta| < 2.1, #alpha > 0.3 (final Event sample)",
        xAxis = 'N',
        name = "finalSamplePlots_numCentralJets"
    )
]

drawJobConfigurator_ZtoMuTauOS.add(
    afterCut = evtSelDiTauCandidateForMuTauZeroCharge,
    plots = finalSamplePlots
)

drawJobConfigurator_ZtoMuTauSS = drawJobConfigurator(
    template = plots_ZtoMuTau,
    dqmDirectory = '#PROCESSDIR#/zMuTauAnalyzerSS/'
)

drawJobConfigurator_ZtoMuTauSS.add(
    afterCut = evtSelDiTauCandidateForMuTauNonZeroCharge,
    plots = finalSamplePlots
)
