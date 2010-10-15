import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.Configuration.analyzeZtoElecMu_cfi import *
from TauAnalysis.DQMTools.tools.drawJobConfigurator import *

# define template for all kinds of plots
# (specific to Z --> e + mu analysis)
plots_ZtoElecMu = cms.PSet(
    plots = cms.PSet(  
        dqmMonitorElements = cms.vstring(''),
        processes = cms.vstring(
            'ZmumuPlusJets',
            #'Zmumu',
            'ZeePlusJets',
            #'Zee',
            'WplusJets',
            'TTplusJets',
            'qcdSum',
            'ZtautauPlusJets'
            #'Ztautau'
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
        'ZeePlusJets',
        #'Zee',
        'WplusJets',
        'TTplusJets',
        'qcdSum',
        'ZtautauPlusJets'
        #'Ztautau'
    )
)

drawJobConfigurator_ZtoElecMu = drawJobConfigurator(
    template = plots_ZtoElecMu,
    dqmDirectory = '#PROCESSDIR#/zElecMuAnalyzer/'
)

#--------------------------------------------------------------------------------
# define cut-flow control plots;
# show distribution of each quantity used in event selection
# (**before** quantity is cutted on)
#--------------------------------------------------------------------------------

drawJobConfigurator_ZtoElecMu.add(
    afterCut = evtSelPrimaryEventVertex,
    beforeCut = evtSelPrimaryEventVertexQuality,
    plot = drawJobConfigEntry(
        meName = 'VertexQuantities/VertexChi2Prob',
        title = "P(#Chi^{2}_{vtx} (after primary Event Vertex Cut)",
        xAxis = 'prob',
        name = "cutFlowControlPlots_vertexChi2Prob_afterPrimaryEventVertex"
    )
)

drawJobConfigurator_ZtoElecMu.add(
    afterCut = evtSelPrimaryEventVertexQuality,
    beforeCut = evtSelPrimaryEventVertexPosition,
    plot = drawJobConfigEntry(
        meName = 'VertexQuantities/VertexZ',
        title = "z_{vtx} (after primary Event Vertex quality Cut)",
        xAxis = 'posZ',
        name = "cutFlowControlPlots_vertexZ_afterPrimaryEventVertexQuality"
    )
)

drawJobConfigurator_ZtoElecMu.add(
    afterCut = evtSelPrimaryEventVertexPosition,
    beforeCut = evtSelElectronIdMin,
    plot = drawJobConfigEntry(
        meName = 'ElectronQuantities/Electron#PAR#',
        PAR = [ 'Pt', 'Eta', 'Phi' ],
        title = "Electron (after primary Event Vertex position Cut)",
        xAxis = '#PAR#',
        name = "cutFlowControlPlots_electron_afterPrimaryEventVertexPosition"
    )
)

drawJobConfigurator_ZtoElecMu.add(
    afterCut = evtSelElectronIdMin,
    beforeCut = evtSelElectronIdMax,
    plot = drawJobConfigEntry(
        meName = 'ElectronQuantities/Electron#PAR#',
        PAR = [ 'Pt', 'Eta', 'Phi' ],
        title = "Electron (after ElectronIdMin)",
        xAxis = '#PAR#',
        name = "cutFlowControlPlots_electron_afterElectronIdMin"
    )
)

drawJobConfigurator_ZtoElecMu.add(
    afterCut = evtSelElectronIdMax,
    beforeCut = evtSelGlobalMuonMin,
    plot = drawJobConfigEntry(
        meName = 'MuonQuantities/Muon#PAR#',
        PAR = [ 'Pt', 'Eta', 'Phi' ],
        title = "Muon (after ElectronIdMax)",
        xAxis = '#PAR#',
        name = "cutFlowControlPlots_muon_afterElectronIdMax"
    )
)

drawJobConfigurator_ZtoElecMu.add(
    afterCut = evtSelGlobalMuonMin,
    beforeCut = evtSelGlobalMuonMax,
    plot = drawJobConfigEntry(
        meName = 'MuonQuantities/Muon#PAR#',
        PAR = [ 'Pt', 'Eta', 'Phi' ],
        title = "Muon (after GlobalMuonMin)",
        xAxis = '#PAR#',
        name = "cutFlowControlPlots_muon_afterGlobalMuonMin"
    )
)

drawJobConfigurator_ZtoElecMu.add(
    afterCut = evtSelGlobalMuonMax,
    beforeCut = evtSelDiTauCandidateForElecMuDRmin,
    plot = drawJobConfigEntry(
        meName = 'DiTauCandidateQuantities/DR12',
        title = "#Delta R(Electron,Muon) (after ElectronTrkIP Cut)",
        xAxis = 'dR',
        name = "cutFlowControlPlots_dR12_afterElectronTrkIP"
    )
)

drawJobConfigurator_ZtoElecMu.add(
    afterCut = evtSelDiTauCandidateForElecMuDRmin,
    beforeCut = evtSelDiTauCandidateForElecMuZeroCharge,
    plot = drawJobConfigEntry(
        meName = 'DiTauCandidateQuantities/DiTauCandidateCharge',
        title = "Charge(Electron + Muon) (after DiTauCandidateForElecMuDRmin Cut)",
        xAxis = 'unlabeled',
        name = "cutFlowControlPlots_diTauCharge_afterDiTauCandidateForElecMuDRmin"
    )
)

drawJobConfigurator_ZtoElecMu.add(
    afterCut = evtSelDiTauCandidateForElecMuZeroCharge,
    beforeCut = evtSelElectronAntiCrack,
    plot = drawJobConfigEntry(
        meName = 'ElectronQuantities/Electron#PAR#',
        PAR = [ 'Pt', 'Eta', 'Phi' ],
        title = "Electron (after DiTauCandidateForElecMuZeroCharge Cut)",
        xAxis = '#PAR#',
        name = "cutFlowControlPlots_electron_afterDiTauCandidateForElecMuZeroCharge"
    )
)   

drawJobConfigurator_ZtoElecMu.add(
    afterCut = evtSelElectronAntiCrack,
    beforeCut = evtSelElectronEta,
    plot = drawJobConfigEntry(
        meName = 'ElectronQuantities/Electron#PAR#',
        PAR = [ 'Pt', 'Eta', 'Phi' ],
        title = "Electron (after Electron anti-Crack Cut)",
        xAxis = '#PAR#',
        name = "cutFlowControlPlots_electron_afterElectronAntiCrack"
    )
) 

drawJobConfigurator_ZtoElecMu.add(
    afterCut = evtSelElectronEta,
    beforeCut = evtSelMuonEta,
    plot = drawJobConfigEntry(
        meName = 'MuonQuantities/Muon#PAR#',
        PAR = [ 'Pt', 'Eta', 'Phi' ],
        title = "Muon (after ElectronEta Cut)",
        xAxis = '#PAR#',
        name = "cutFlowControlPlots_muon_afterElectronEta"
    )
)

drawJobConfigurator_ZtoElecMu.add(
    afterCut = evtSelMuonEta,
    beforeCut = evtSelElectronPt,
    plot = drawJobConfigEntry(
        meName = 'ElectronQuantities/Electron#PAR#',
        PAR = [ 'Pt', 'Eta', 'Phi' ],
        title = "Electron (after Muon #eta Cut)",
        xAxis = '#PAR#',
        name = "cutFlowControlPlots_electron_afterMuonEta"
    )
)  

drawJobConfigurator_ZtoElecMu.add(
    afterCut = evtSelElectronPt,
    beforeCut = evtSelMuonPt,
    plot = drawJobConfigEntry(
        meName = 'MuonQuantities/Muon#PAR#',
        PAR = [ 'Pt', 'Eta', 'Phi' ],
        title = "Muon (after ElectronPt Cut)",
        xAxis = '#PAR#',
        name = "cutFlowControlPlots_muon_afterElectronPt"
    )
)

drawJobConfigurator_ZtoElecMu.add(
    afterCut = evtSelMuonPt,
    beforeCut = evtSelElectronIso,
    plot = drawJobConfigEntry(
        meName = 'ElectronQuantities/ElectronIsoSumPtRel',
        title = "Electron Track iso. (after MuonPt Cut)",
        xAxis = 'Pt',
        name = "cutFlowControlPlots_electronIso_afterMuonPt"
    )
)

drawJobConfigurator_ZtoElecMu.add(
    afterCut = evtSelElectronIso,
    beforeCut = evtSelElectronTrk,
    plot = drawJobConfigEntry(
        meName = 'ElectronQuantities/Electron#PAR#',
        PAR = [ 'Pt', 'Eta', 'Phi' ],
        title = "Electron (after Electron iso. Cut)",
        xAxis = '#PAR#',
        name = "cutFlowControlPlots_electron_afterElectronIso"
    )
)

drawJobConfigurator_ZtoElecMu.add(
    afterCut = evtSelElectronTrk,
    beforeCut = evtSelMuonIso,
    plot = drawJobConfigEntry(
        meName = 'MuonQuantities/MuonIsoSumPtRel',
        title = "Muon Track iso. (after Electron Track Cut)",
        xAxis = 'Pt',
        name = "cutFlowControlPlots_muonIso_afterElectronTrk"
    )
)

drawJobConfigurator_ZtoElecMu.add(
    afterCut = evtSelMuonIso,
    beforeCut = evtSelMuonAntiPion,
    plot = drawJobConfigEntry(
        meName = 'MuonQuantities/Muon#PAR#Compatibility',
        PAR = [ 'Calo', 'Segment' ],
        title = "Muon #PAR# compatibility (after Muon iso. Cut)",
        xAxis = 'prob',
        name = "cutFlowControlPlots_muonComp_afterMuonIso"
    )
)

drawJobConfigurator_ZtoElecMu.add(
    afterCut = evtSelMuonAntiPion,
    beforeCut = evtSelDiTauCandidateForElecMuMt1MET,
    plot = drawJobConfigEntry(
        meName = 'DiTauCandidateQuantities/Mt1MET',
        title = "M_{T}(Electron + MET) (after MuonAntiPion Cut)",
        xAxis = 'Mt',
        name = "cutFlowControlPlots_mtElectronMET_afterMuonAntiPion"
    )
)

drawJobConfigurator_ZtoElecMu.add(
    afterCut = evtSelDiTauCandidateForElecMuMt1MET,
    beforeCut = evtSelDiTauCandidateForElecMuMt2MET,
    plot = drawJobConfigEntry(
        meName = 'DiTauCandidateQuantities/Mt2MET',
        title = "M_{T}(Muon + MET) (after M_{T}(Electron + MET) Cut)",
        xAxis = 'Mt',
        name = "cutFlowControlPlots_mtMuonMET_afterMtElectronMET"
    )
)

drawJobConfigurator_ZtoElecMu.add(
    afterCut = evtSelDiTauCandidateForElecMuMt2MET,
    beforeCut = evtSelDiTauCandidateForElecMuDPhi,
    plot = drawJobConfigEntry(
        meName = 'DiTauCandidateQuantities/DPhi12',
        title = "#Delta#phi(Electron-Muon) (after DiTauCandidateForElecMuMt2MET Cut)",
        xAxis = 'dPhi',
        name = "cutFlowControlPlots_dPhiElectronMuon_afterDiTauCandidateForElecMuMt2MET"
    )
)

#--------------------------------------------------------------------------------
# define distributions to be plotted
# for events passing all event selection criteria
#--------------------------------------------------------------------------------

drawJobConfigurator_ZtoElecMu.add(
    afterCut = evtSelDiTauCandidateForElecMuDPhi,
    plots = [
        drawJobConfigEntry(
            meName = 'ElectronQuantities/Electron#PAR#',
            PAR = [ 'Pt', 'Eta', 'Phi' ],
            title = "Electron (final Event sample)",
            xAxis = '#PAR#',
            name = "finalSamplePlots_electron"
        ),
        drawJobConfigEntry(
            meName = 'ElectronQuantities/ElectronMatchingGenParticlePdgId',
            title = "PdgId of gen. Particle matching Electron (final Event sample)",
            xAxis = 'PdgId',
            name = "finalSamplePlots_pdgIdGenParticleMatchingElectron"
        ),
        drawJobConfigEntry(
            meName = 'ElectronQuantities/ElectronEcalIsoPtBarrel',
            title = "Electron ECAL iso., Barrel (final Event sample)",
            xAxis = 'Pt',
            name = "finalSamplePlots_electronEcalIsoBarrel"
        ),
        drawJobConfigEntry(
            meName = 'ElectronQuantities/ElectronEcalIsoPtEndcap',
            title = "Electron ECAL iso., Endcap (final Event sample)",
            xAxis = 'Pt',
            name = "finalSamplePlots_electronEcalIsoEndcap"
        ),
        drawJobConfigEntry(
            meName = 'MuonQuantities/Muon#PAR#',
            PAR = [ 'Pt', 'Eta', 'Phi' ],
            title = "Muon (final Event sample)",
            xAxis = '#PAR#',
            name = "finalSamplePlots_muon"
        ),
        drawJobConfigEntry(
            meName = 'MuonQuantities/MuonMatchingGenParticlePdgId',
            title = "PdgId of gen. Particle matching Muon (final Event sample)",
            xAxis = 'PdgId',
            name = "finalSamplePlots_pdgIdGenParticleMatchingMuon"
        ),
        drawJobConfigEntry(
            meName = 'DiTauCandidateQuantities/DPhi12',
            title = "#Delta#phi(Electron-Muon) (final Event sample)",
            xAxis = 'dPhi',
            name = "finalSamplePlots_dPhiElectronMuon"
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
            title = "M_{T}(Electron + MET) (final Event sample)",
            xAxis = 'Mt',
            name = "finalSamplePlots_mtElectronMET"
        ),
        drawJobConfigEntry(
            meName = 'DiTauCandidateQuantities/Mt2MET',
            title = "M_{T}(Muon + MET) (final Event sample)",
            xAxis = 'Mt',
            name = "finalSamplePlots_mtMuonMET"
        ),
        drawJobConfigEntry(
            meName = 'DiTauCandidateQuantities/Mt12MET',
            title = "M_{T}(Electron + Muon + MET) (final Event sample)",
            xAxis = 'Mt',
            name = "finalSamplePlots_mtElectronMuonMET"
        ),
        drawJobConfigEntry(
            meName = 'DiTauCandidateQuantities/VisMass',
            title = "M_{vis}(Electron + Muon) (final Event sample)",
            xAxis = 'Mass',
            name = "finalSamplePlots_mVisible"
        ),
        drawJobConfigEntry(
            meName = 'DiTauCandidateQuantities/VisMassZllCombinedHypothesis',
            title = "M_{vis}(Electron + Muon), Z #rightarrow #ell^{+} #ell^{-} combined Hypothesis (final Event sample)",
            xAxis = 'Mass',
            name = "finalSamplePlots_mVisibleZllCombinedHypothesis"
        ),
        drawJobConfigEntry(
            meName = 'DiTauCandidateQuantities/CDFmethodMass',
            title = "M(Electron + Muon), CDF method (final Event sample)",
            xAxis = 'Mass',
            name = "finalSamplePlots_mCDFmethod"
        ),
        drawJobConfigEntry(
            meName = 'DiTauCandidateQuantities/CollinearApproxMass',
            title = "M(Electron + Muon), collinear Approx. (final Event sample)",
            xAxis = 'Mass',
            name = "finalSamplePlots_mCollApprox"
        ),
        drawJobConfigEntry(
            meName = 'DiTauCandidateSVfitQuantities/psKine_MEt_ptBalance/Mass',
            title = "M(Electron + Muon), SVfit method (final Event sample)",
            xAxis = 'Mass',
            name = "finalSamplePlots_mSVmethod"
        ),
        drawJobConfigEntry(
            meName = 'DiTauCandidateQuantities/Ht12MET',
            title = "#Sigma H_{T}(Electron + Muon + MET) (final Event sample)",
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


