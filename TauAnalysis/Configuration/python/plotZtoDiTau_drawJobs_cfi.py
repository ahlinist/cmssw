import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.Configuration.analyzeZtoDiTau_cfi import *
from TauAnalysis.DQMTools.tools.drawJobConfigurator import *

# define template for all kinds of plots
# (specific to Z --> tau-jet + tau-jet analysis)
plots_ZtoDiTau = cms.PSet(
    plots = cms.PSet(  
        dqmMonitorElements = cms.vstring(''),
        processes = cms.vstring(
            ##'Zee',
            'Zmumu',
            'WplusJets',
            'TTplusJets',
            'Ztautau',
            'qcdDiJet'
            ##'Ztautau'
        )
    ),
    xAxis = cms.string('unlabeled'),
    yAxis = cms.string('numEntries_linear'),
    #yAxis = cms.string('numEntries_log'),
    legend = cms.string('regular'),
    labels = cms.vstring('mcNormScale'),                   
    drawOptionSet = cms.string('default'),
    stack = cms.vstring(
        ##'Zee',
        'Zmumu',
        'WplusJets',
        'TTplusJets',
        'Ztautau',
        'qcdDiJet'
        ##'Ztautau'
    )
)

drawJobConfigurator_ZtoDiTau = drawJobConfigurator(
    template = plots_ZtoDiTau,
    dqmDirectory = '#PROCESSDIR#/zDiTauAnalyzer/'
)

#--------------------------------------------------------------------------------
# define cut-flow control plots;
# show distribution of each quantity used in event selection
# (**before** quantity is cutted on)
#--------------------------------------------------------------------------------

drawJobConfigurator_ZtoDiTau.add(
    afterCut = evtSelPrimaryEventVertex,
    beforeCut = evtSelPrimaryEventVertexQuality,
    plot = drawJobConfigEntry(
        meName = 'VertexQuantities/VertexChi2Prob',
        title = "P(#Chi^{2}_{vtx} (after primary Event Vertex Cut)",
        xAxis = 'prob',
        name = "cutFlowControlPlots_vertexChi2Prob_afterPrimaryEventVertex"
    )
)

drawJobConfigurator_ZtoDiTau.add(
    afterCut = evtSelPrimaryEventVertexQuality,
    beforeCut = evtSelPrimaryEventVertexPosition,
    plot = drawJobConfigEntry(
        meName = 'VertexQuantities/VertexZ',
        title = "z_{vtx} (after primary Event Vertex quality Cut)",
        xAxis = 'posZ',
        name = "cutFlowControlPlots_vertexZ_afterPrimaryEventVertexQuality"
    )
)

drawJobConfigurator_ZtoDiTau.add(
    afterCut = evtSelPrimaryEventVertexPosition,
    beforeCut = evtSelFirstTauEta,
    plot = drawJobConfigEntry(
        meName = 'TauQuantities1/Tau#PAR#',
        PAR = [ 'Pt', 'Eta', 'Phi' ],
        title = "Tau_{1} (after primary Event Vertex position Cut)",
        xAxis = '#PAR#',
        name = "cutFlowControlPlots_tau1_afterPrimaryEventVertexPosition"
    )
)    

drawJobConfigurator_ZtoDiTau.add(
    afterCut = evtSelFirstTauEta,
    beforeCut = evtSelFirstTauPt,
    plot = drawJobConfigEntry(
        meName = 'TauQuantities1/Tau#PAR#',
        PAR = [ 'Pt', 'Eta', 'Phi' ],
        title = "Tau_{1} (after Tau_{1} #eta Cut)",
        xAxis = '#PAR#',
        name = "cutFlowControlPlots_tau1_afterTau1Eta"
    )
)

drawJobConfigurator_ZtoDiTau.add(
    afterCut = evtSelFirstTauPt,
    beforeCut = evtSelSecondTauEta,
    plot = drawJobConfigEntry(
        meName = 'TauQuantities2/Tau#PAR#',
        PAR = [ 'Pt', 'Eta', 'Phi' ],
        title = "Tau_{2} (after Tau_{1} P_{T} Cut)",
        xAxis = '#PAR#',
        name = "cutFlowControlPlots_tau2_afterTau1Pt"
    )
)    

drawJobConfigurator_ZtoDiTau.add(
    afterCut = evtSelSecondTauEta,
    beforeCut = evtSelSecondTauPt,
    plot = drawJobConfigEntry(
        meName = 'TauQuantities2/Tau#PAR#',
        PAR = [ 'Pt', 'Eta', 'Phi' ],
        title = "Tau_{2} (after Tau_{2} #eta Cut)",
        xAxis = '#PAR#',
        name = "cutFlowControlPlots_tau2_afterTau2Eta"
    )
)

drawJobConfigurator_ZtoDiTau.add(
    afterCut = evtSelSecondTauPt,
    beforeCut = evtSelFirstTauLeadTrk,
    plots = [
        drawJobConfigEntry(
            meName = 'TauQuantities1/Tau#PAR#',
            PAR = [ 'Pt', 'Eta', 'Phi' ],
            title = "Tau_{1} (after Tau_{2} P_{T} Cut)",
            xAxis = '#PAR#',
            name = "cutFlowControlPlots_tau1_afterTau2Pt"
        ),
        drawJobConfigEntry(
            meName = 'TauQuantities1/TauLeadTrkPt',
            title = "Tau_{1} lead. Track (after Tau_{2} P_{T} Cut)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_tau1LeadTrkPt_afterTau2Pt"
        )
    ]
)

drawJobConfigurator_ZtoDiTau.add(
    afterCut = evtSelFirstTauLeadTrk,
    beforeCut = evtSelFirstTauLeadTrkPt,
    plots = [
        drawJobConfigEntry(
            meName = 'TauQuantities1/Tau#PAR#',
            PAR = [ 'Pt', 'Eta', 'Phi' ],
            title = "Tau_{1} (after Tau_{1} lead. Track Cut)",
            xAxis = '#PAR#',
            name = "cutFlowControlPlots_tau1_afterTau1LeadTrk"
        ),
        drawJobConfigEntry(
            meName = 'TauQuantities1/TauLeadTrkPt',
            title = "Tau_{1} lead. Track (after Tau_{1} lead. Track Cut)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_tau1LeadTrkPt_afterTau1LeadTrk"
       )
   ]
)

drawJobConfigurator_ZtoDiTau.add(
    afterCut = evtSelFirstTauLeadTrkPt,
    beforeCut = evtSelFirstTauTaNCdiscr,
    plot = drawJobConfigEntry(
        meName = 'TauQuantities1/TauDiscriminatorTaNCfrQuarterPercent',
        title = "Tau_{1} TaNC output (fr = 0.25%; after Tau_{1} lead. Track P_{T} Cut)",
        xAxis = 'unlabeled',
        name = "cutFlowControlPlots_tau1DiscrTaNCfrQuarterPercent_afterTau1LeadTrkPt"
    )
)

drawJobConfigurator_ZtoDiTau.add(
    afterCut = evtSelFirstTauTaNCdiscr,
    beforeCut = evtSelFirstTauTrkIso,
    plot = drawJobConfigEntry(
        meName = 'TauQuantities1/TauTrkIsoPt',
        title = "Tau_{1} Track iso. (after Tau_{1} TaNC output (fr = 0.25%) Cut)",
        xAxis = 'Pt',
        name = "cutFlowControlPlots_tau1TrkIso_afterTau1DiscrTaNCfrQuarterPercent"
    )
)

drawJobConfigurator_ZtoDiTau.add(
    afterCut = evtSelFirstTauTrkIso,
    beforeCut = evtSelFirstTauEcalIso,
    plot = drawJobConfigEntry(
        meName = 'TauQuantities1/TauEcalIsoPt',
        title = "Tau_{1} ECAL iso. (after Tau_{1} Track iso. Cut)",
        xAxis = 'Pt',
        name = "cutFlowControlPlots_tau1EcalIso_afterTau1TrkIso"
    )
)

drawJobConfigurator_ZtoDiTau.add(
    afterCut = evtSelFirstTauEcalIso,
    beforeCut = evtSelFirstTauProng,
    plot = drawJobConfigEntry(
        meName = 'TauQuantities1/TauNumTracksSignalCone',
        title = "Tau_{1} Tracks in Signal Cone (after Tau_{1} ECAL iso. Cut)",
        xAxis = 'unlabeled',
        name = "cutFlowControlPlots_tau1NumTracksSignalCone_afterTau1EcalIso"
    )
)

drawJobConfigurator_ZtoDiTau.add(
    afterCut = evtSelFirstTauProng,
    beforeCut = evtSelFirstTauCharge,
    plot = drawJobConfigEntry(
        meName = 'TauQuantities1/TauCharge',
        title = "Tau_{1} Charge (#Sigma Tracks in Signal Cone, after Tau_{1} 1-Prong||3-Prong Cut)",
        xAxis = 'unlabeled',
        name = "cutFlowControlPlots_tau1Charge_afterTau1Prong"
    )
)

drawJobConfigurator_ZtoDiTau.add(
    afterCut = evtSelFirstTauCharge,
    beforeCut = evtSelFirstTauMuonVeto,
    plots = [
        drawJobConfigEntry(
            meName = 'TauQuantities1/Tau#PAR#',
            PAR = [ 'Pt', 'Eta', 'Phi' ],
            title = "Tau_{1} (after Charge(Tau_{1}) = +/-1 Cut)",
            xAxis = '#PAR#',
            name = "cutFlowControlPlots_tau1_afterTau1Charge"
        ),
        drawJobConfigEntry(
            meName = 'TauQuantities1/TauDiscriminatorAgainstMuons',
            title = "Tau_{1} anti-Muon Discr. (after Charge(Tau_{1}) = +/-1 Cut)",
            xAxis = 'unlabeled',
            name = "cutFlowControlPlots_tau1AntiMuonDiscr_afterTau1Charge"
        )
    ]
)

drawJobConfigurator_ZtoDiTau.add(
    afterCut = evtSelFirstTauMuonVeto,
    beforeCut = evtSelFirstTauElectronVeto,
    plots = [
        drawJobConfigEntry(
            meName = 'TauQuantities1/Tau#PAR#',
            PAR = [ 'Pt', 'Eta', 'Phi' ],
            title = "Tau_{1} (after Tau_{1} #mu-Veto Cut)",
            xAxis = '#PAR#',
            name = "cutFlowControlPlots_tau1_afterTau1MuonVeto"
        ),
        drawJobConfigEntry(
            meName = 'TauQuantities1/TauDiscriminatorAgainstElectrons',
            title = "Tau_{1} anti-Electron Discr. (after Tau_{1} #mu-Veto Cut)",
            xAxis = 'unlabeled',
            name = "cutFlowControlPlots_tau1AntiElectronDiscr_afterTau1MuonVeto"
        )
    ]
)

drawJobConfigurator_ZtoDiTau.add(
    afterCut = evtSelFirstTauElectronVeto,
    beforeCut = evtSelSecondTauLeadTrk,
    plots = [
        drawJobConfigEntry(
            meName = 'TauQuantities2/Tau#PAR#',
            PAR = [ 'Pt', 'Eta', 'Phi' ],
            title = "Tau_{2} (after Tau_{1} e-Veto Cut)",
            xAxis = '#PAR#',
            name = "cutFlowControlPlots_tau2_afterTau1Charge"
        ),
        drawJobConfigEntry(
            meName = 'TauQuantities2/TauLeadTrkPt',
            title = "Tau_{2} lead. Track (after Tau_{1} e-Veto Cut)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_tau2LeadTrkPt_afterTau1ElectronVeto"
        )
    ]
)

drawJobConfigurator_ZtoDiTau.add(
    afterCut = evtSelSecondTauLeadTrk,
    beforeCut = evtSelSecondTauLeadTrkPt,
    plots = [
        drawJobConfigEntry(
            meName = 'TauQuantities2/Tau#PAR#',
            PAR = [ 'Pt', 'Eta', 'Phi' ],
            title = "Tau_{2} (after Tau_{2} lead. Track Cut)",
            xAxis = '#PAR#',
            name = "cutFlowControlPlots_tau2_afterTau2LeadTrk"
        ),
        drawJobConfigEntry(
            meName = 'TauQuantities2/TauLeadTrkPt',
            title = "Tau_{2} lead. Track (after Tau_{2} lead. Track Cut)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_tau2LeadTrkPt_afterTau2LeadTrk"
       )
   ]
)

drawJobConfigurator_ZtoDiTau.add(
    afterCut = evtSelSecondTauLeadTrkPt,
    beforeCut = evtSelSecondTauTaNCdiscr,
    plot = drawJobConfigEntry(
        meName = 'TauQuantities2/TauDiscriminatorTaNCfrQuarterPercent',
        title = "Tau_{2} TaNC output (fr = 0.25%; after Tau_{2} lead. Track P_{T} Cut)",
        xAxis = 'unlabeled',
        name = "cutFlowControlPlots_tau2DiscrTaNCfrQuarterPercent_afterTau2LeadTrkPt"
    )
)

drawJobConfigurator_ZtoDiTau.add(
    afterCut = evtSelSecondTauTaNCdiscr,
    beforeCut = evtSelSecondTauTrkIso,
    plot = drawJobConfigEntry(
        meName = 'TauQuantities2/TauTrkIsoPt',
        title = "Tau_{2} Track iso. (after Tau_{2} TaNC output (fr = 0.25%) Cut)",
        xAxis = 'Pt',
        name = "cutFlowControlPlots_tau2TrkIso_afterTau2DiscrTaNCfrQuarterPercent"
    )
)

drawJobConfigurator_ZtoDiTau.add(
    afterCut = evtSelSecondTauTrkIso,
    beforeCut = evtSelSecondTauEcalIso,
    plot = drawJobConfigEntry(
        meName = 'TauQuantities2/TauEcalIsoPt',
        title = "Tau_{2} ECAL iso. (after Tau_{2} Track iso. Cut)",
        xAxis = 'Pt',
        name = "cutFlowControlPlots_tau2EcalIso_afterTau2TrkIso"
    )
)

drawJobConfigurator_ZtoDiTau.add(
    afterCut = evtSelSecondTauEcalIso,
    beforeCut = evtSelSecondTauProng,
    plot = drawJobConfigEntry(
        meName = 'TauQuantities2/TauNumTracksSignalCone',
        title = "Tau_{2} Tracks in Signal Cone (after Tau_{2} ECAL iso. Cut)",
        xAxis = 'unlabeled',
        name = "cutFlowControlPlots_tau2NumTracksSignalCone_afterTau2EcalIso"
    )
)

drawJobConfigurator_ZtoDiTau.add(
    afterCut = evtSelSecondTauProng,
    beforeCut = evtSelSecondTauCharge,
    plot = drawJobConfigEntry(
        meName = 'TauQuantities2/TauCharge',
        title = "Tau_{2} Charge (#Sigma Tracks in Signal Cone, after Tau_{2} 1-Prong||3-Prong Cut)",
        xAxis = 'unlabeled',
        name = "cutFlowControlPlots_tau2Charge_afterTau2Prong"
    )
)

drawJobConfigurator_ZtoDiTau.add(
    afterCut = evtSelSecondTauCharge,
    beforeCut = evtSelSecondTauMuonVeto,
    plots = [
        drawJobConfigEntry(
            meName = 'TauQuantities2/Tau#PAR#',
            PAR = [ 'Pt', 'Eta', 'Phi' ],
            title = "Tau_{2} (after Charge(Tau_{2}) = +/-1 Cut)",
            xAxis = '#PAR#',
            name = "cutFlowControlPlots_tau2_afterTau2Charge"
        ),
        drawJobConfigEntry(
            meName = 'TauQuantities2/TauDiscriminatorAgainstMuons',
            title = "Tau_{2} anti-Muon Discr. (after Charge(Tau_{2}) = +/-1 Cut)",
            xAxis = 'unlabeled',
            name = "cutFlowControlPlots_tau2AntiMuonDiscr_afterTau2Charge"
        )
    ]
)

drawJobConfigurator_ZtoDiTau.add(
    afterCut = evtSelSecondTauMuonVeto,
    beforeCut = evtSelSecondTauElectronVeto,
    plots = [
        drawJobConfigEntry(
            meName = 'TauQuantities2/Tau#PAR#',
            PAR = [ 'Pt', 'Eta', 'Phi' ],
            title = "Tau_{2} (after Tau_{2} #mu-Veto Cut)",
            xAxis = '#PAR#',
            name = "cutFlowControlPlots_tau2_afterTau2MuonVeto"
        ),
        drawJobConfigEntry(
            meName = 'TauQuantities2/TauDiscriminatorAgainstElectrons',
            title = "Tau_{2} anti-Electron Discr. (after Tau_{2} #mu-Veto Cut)",
            xAxis = 'unlabeled',
            name = "cutFlowControlPlots_tau2AntiElectronDiscr_afterTau2MuonVeto"
        )
    ]
)

##drawJobConfigurator_ZtoDiTau.add(
##    afterCut = evtSelSecondTauElectronVeto,
##    beforeCut = evtSelDiTauCandidateForDiTauAntiOverlapVeto,
##    plot = drawJobConfigEntry(
##        meName = 'DiTauCandidateQuantities/DR12',
##        title = "#Delta R(Tau_{1},Tau_{2}) (after Tau_{2} e-Veto Cut)",
##        xAxis = 'dR',
##        name = "cutFlowControlPlots_dR12_afterTau2ElectronVeto"
##    )
##)

drawJobConfigurator_ZtoDiTau.add(
    afterCut = evtSelDiTauCandidateForDiTauAntiOverlapVeto,
    beforeCut = evtSelDiTauCandidateForDiTauZeroCharge,
    plot = drawJobConfigEntry(
        meName = 'DiTauCandidateQuantities/DiTauCandidateCharge',
        title = "Charge(Tau_{1} + Tau+{2}) (after diTau anti-Overlap Veto)",
        xAxis = 'unlabeled',
        name = "cutFlowControlPlots_diTauCharge_afterAntiOverlapVeto"
    )
)

drawJobConfigurator_ZtoDiTau.add(
    afterCut = evtSelDiTauCandidateForDiTauZeroCharge,
    beforeCut = evtSelDiTauCandidateForDiTauAcoplanarity,
    plots = [
        drawJobConfigEntry(
            meName = 'DiTauCandidateQuantities/DPhi12',
            title = "#Delta#phi(Tau_{1}-Tau_{2}) (after opposite Charge Cut)",
            xAxis = 'dPhi',
            name = "cutFlowControlPlots_dPhiTau1Tau2_afterZeroCharge"
        ),
        drawJobConfigEntry(
            meName = 'DiTauCandidateQuantities/DPhi1MET',
            title = "#Delta#phi(Tau_{1}-MET) (after opposite Charge Cut)",
            xAxis = 'dPhi',
            name = "cutFlowControlPlots_dPhiTau1MET_afterZeroCharge"
        ),
        drawJobConfigEntry(
            meName = 'DiTauCandidateQuantities/DPhi2MET',
            title = "#Delta#phi(Tau_{2}-MET) (after opposite Charge Cut)",
            xAxis = 'dPhi',
            name = "cutFlowControlPlots_dPhiTau2MET_afterZeroCharge"
        )
    ]
)

drawJobConfigurator_ZtoDiTau.add(
    afterCut = evtSelDiTauCandidateForDiTauAcoplanarity,
    beforeCut = evtSelDiTauCandidateForDiTauPzetaDiff,
    plot = drawJobConfigEntry(
        meName = 'DiTauCandidateQuantities/PzetaDiff',
        title = "P_{#zeta} - 1.5*P_{#zeta}^{vis} (after Acoplanarity Cut)",
        xAxis = 'GeV',
        name = "cutFlowControlPlots_PzetaDiff_afterAcoplanarity"
    )
)    

#--------------------------------------------------------------------------------
# define distributions to be plotted
# for events passing all event selection criteria
#--------------------------------------------------------------------------------

drawJobConfigurator_ZtoDiTau.add(
    afterCut = evtSelDiTauCandidateForDiTauPzetaDiff,
    plots = [
        drawJobConfigEntry(
            meName = 'TauQuantities1/Tau#PAR#',
            PAR = [ 'Pt', 'Eta', 'Phi' ],
            title = "Tau_{1} (final Event sample)",
            xAxis = '#PAR#',
            name = "finalSamplePlots_tau1"
        ),
        drawJobConfigEntry(
            meName = 'TauQuantities1/TauMatchingGenParticlePdgId',
            title = "PdgId of gen. Particle matching Tau_{1} (final Event sample)",
            xAxis = 'PdgId',
            name = "finalSamplePlots_pdgIdGenParticleMatchingTau1"
        ),
        drawJobConfigEntry(
            meName = 'TauQuantities1/TauLeadTrkPt',
            title = "Tau_{1} lead. Track (final Event sample)",
            xAxis = 'Pt',
            name = "finalSamplePlots_tau1LeadTrkPt"
        ),
        drawJobConfigEntry(
            meName = 'TauQuantities1/TauNumTracksSignalCone',
            title = "Tau_{1} Tracks in Signal Cone (final Event sample)",
            xAxis = 'unlabeled',
            name = "finalSamplePlots_tau1NumTracksSignalCone"
        ),
        drawJobConfigEntry(
            meName = 'TauQuantities1/TauJetRadius',
            title = "Tau_{1} Jet Radius (final Event sample)",
            xAxis = 'unlabeled',
            name = "finalSamplePlots_tau1JetRadius"
        ),
        drawJobConfigEntry(
            meName = 'TauQuantities1/TauDiscriminatorTaNCfrTenthPercent',
            title = "Tau_{1} TaNC output (fr = 0.10%) (final Event sample)",
            xAxis = 'unlabeled',
            name = "finalSamplePlots_tau1DiscrTaNCfrTenthPercent"
        ),
        drawJobConfigEntry(
            meName = 'TauQuantities1/TauTaNCoutputTransform',
            title = "Tau_{1} TaNC output (transformed) (final Event sample)",
            xAxis = 'unlabeled',
            name = "finalSamplePlots_tau1TaNCtransform"
        ),
        drawJobConfigEntry(
            meName = 'TauQuantities2/Tau#PAR#',
            PAR = [ 'Pt', 'Eta', 'Phi' ],
            title = "Tau_{2} (final Event sample)",
            xAxis = '#PAR#',
            name = "finalSamplePlots_tau2"
        ),
        drawJobConfigEntry(
            meName = 'TauQuantities2/TauMatchingGenParticlePdgId',
            title = "PdgId of gen. Particle matching Tau_{2} (final Event sample)",
            xAxis = 'PdgId',
            name = "finalSamplePlots_pdgIdGenParticleMatchingTau2"
        ),
        drawJobConfigEntry(
            meName = 'TauQuantities2/TauLeadTrkPt',
            title = "Tau_{2} lead. Track (final Event sample)",
            xAxis = 'Pt',
            name = "finalSamplePlots_tau2LeadTrkPt"
        ),
        drawJobConfigEntry(
            meName = 'TauQuantities2/TauNumTracksSignalCone',
            title = "Tau_{2} Tracks in Signal Cone (final Event sample)",
            xAxis = 'unlabeled',
            name = "finalSamplePlots_tau2NumTracksSignalCone"
        ),
        drawJobConfigEntry(
            meName = 'TauQuantities2/TauJetRadius',
            title = "Tau_{2} Jet Radius (final Event sample)",
            xAxis = 'unlabeled',
            name = "finalSamplePlots_tau2JetRadius"
        ),
        drawJobConfigEntry(
            meName = 'TauQuantities2/TauDiscriminatorTaNCfrTenthPercent',
            title = "Tau_{2} TaNC output (fr = 0.10%) (final Event sample)",
            xAxis = 'unlabeled',
            name = "finalSamplePlots_tau2DiscrTaNCfrTenthPercent"
        ),
        drawJobConfigEntry(
            meName = 'TauQuantities2/TauTaNCoutputTransform',
            title = "Tau_{2} TaNC output (transformed) (final Event sample)",
            xAxis = 'unlabeled',
            name = "finalSamplePlots_tau2TaNCtransform"
        ),
        drawJobConfigEntry(
            meName = 'DiTauCandidateQuantities/DPhi12',
            title = "#Delta#phi(Tau_{1}-Tau_{2}) (final Event sample)",
            xAxis = 'dPhi',
            name = "finalSamplePlots_dPhiTau1Tau2"
        ),
        drawJobConfigEntry(
            meName = 'DiTauCandidateQuantities/DPhi1MET',
            title = "#Delta#phi(Tau_{1}-MET) (final Event sample)",
            xAxis = 'dPhi',
            name = "finalSamplePlots_dPhiTau1MET"
        ),
        drawJobConfigEntry(
            meName = 'DiTauCandidateQuantities/DPhi2MET',
            title = "#Delta#phi(Tau_{2}-MET) (final Event sample)",
            xAxis = 'dPhi',
            name = "finalSamplePlots_dPhiTau2MET"
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
            title = "M_{T}(Tau_{1} + MET) (final Event sample)",
            xAxis = 'Mt',
            name = "finalSamplePlots_mtTau1MET"
        ),
        drawJobConfigEntry(
            meName = 'DiTauCandidateQuantities/Mt2MET',
            title = "M_{T}(Tau_{2} + MET) (final Event sample)",
            xAxis = 'Mt',
            name = "finalSamplePlots_mtTau2MET"
        ),        
        drawJobConfigEntry(
            meName = 'DiTauCandidateQuantities/Mt12MET',
            title = "M_{T}(Tau_{1} + Tau_{2} + MET) (final Event sample)",
            xAxis = 'Mt',
            name = "finalSamplePlots_mtTau1Tau2MET"
        ),
	drawJobConfigEntry(
            meName = 'DiTauCandidateQuantities/VisMass',
            title = "M_{vis}(Tau_{1} + Tau_{2}) (final Event sample)",
            xAxis = 'Mass',
            name = "finalSamplePlots_mVisible"
        ),
        drawJobConfigEntry(
            meName = 'DiTauCandidateQuantities/CDFmethodMass',
            title = "M(Tau_{1} + Tau_{2}), CDF method (final Event sample)",
            xAxis = 'Mass',
            name = "finalSamplePlots_mCDFmethod"
        ),
        drawJobConfigEntry(
            meName = 'DiTauCandidateQuantities/CollinearApproxMass',
            title = "M(Tau_{1} + Tau_{2}), collinear Approx. (final Event sample)",
            xAxis = 'Mass',
            name = "finalSamplePlots_mCollApprox"
        ),
        drawJobConfigEntry(
            meName = 'DiTauCandidateSVfitQuantities/psKine_MEt_ptBalance/Mass',
            title = "M(Tau_{1} + Tau_{2}), SVfit method (final Event sample)",
            xAxis = 'Mass',
            name = "finalSamplePlots_mSVmethod"
        ),
        drawJobConfigEntry(
            meName = 'DiTauCandidateQuantities/Ht12MET',
            title = "#Sigma H_{T}(Tau_{1} + Tau_{2} + MET) (final Event sample)",
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
)                
