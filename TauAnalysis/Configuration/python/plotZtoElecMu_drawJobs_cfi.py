import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.Configuration.analyzeZtoElecMu_cfi import *
from TauAnalysis.DQMTools.drawJobConfigurator import *

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
            #'ZtautauPlusJets'
            'Ztautau'
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
        #'ZtautauPlusJets'
        'Ztautau'
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
    beforeCut = evtSelTightElectronId,
    plot = drawJobConfigEntry(
        meName = 'ElectronQuantities/Electron#PAR#',
        PAR = [ 'Pt', 'Eta', 'Phi' ],
        title = "Electron (after primary Event Vertex position Cut)",
        xAxis = '#PAR#',
        name = "cutFlowControlPlots_electron_afterPrimaryEventVertexPosition"
    )
)

drawJobConfigurator_ZtoElecMu.add(
    afterCut = evtSelTightElectronId,
    beforeCut = evtSelElectronAntiCrack,
    plot = drawJobConfigEntry(
        meName = 'ElectronQuantities/Electron#PAR#',
        PAR = [ 'Pt', 'Eta', 'Phi' ],
        title = "Electron (after Electron id. Cut)",
        xAxis = '#PAR#',
        name = "cutFlowControlPlots_electron_afterTightElectronId"
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
    beforeCut = evtSelElectronPt,
    plot = drawJobConfigEntry(
        meName = 'ElectronQuantities/Electron#PAR#',
        PAR = [ 'Pt', 'Eta', 'Phi' ],
        title = "Electron (after Electron #eta Cut)",
        xAxis = '#PAR#',
        name = "cutFlowControlPlots_electron_afterElectronEta"
    )
)  

drawJobConfigurator_ZtoElecMu.add(
    afterCut = evtSelElectronPt,
    beforeCut = evtSelGlobalMuon,
    plot = drawJobConfigEntry(
        meName = 'MuonQuantities/Muon#PAR#',
        PAR = [ 'Pt', 'Eta', 'Phi' ],
        title = "Muon (after Electron P_{T} Cut)",
        xAxis = '#PAR#',
        name = "cutFlowControlPlots_muon_afterElectronPt"
    )
)

drawJobConfigurator_ZtoElecMu.add(
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

drawJobConfigurator_ZtoElecMu.add(
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

drawJobConfigurator_ZtoElecMu.add(
    afterCut = evtSelMuonPt,
    beforeCut = evtSelElectronTrkIso,
    plot = drawJobConfigEntry(
        meName = 'ElectronQuantities/ElectronTrkIsoPt',
        title = "Electron Track iso. (after Muon P_{T} Cut)",
        xAxis = 'Pt',
        name = "cutFlowControlPlots_electronTrkIso_afterMuonPt"
    )
)

drawJobConfigurator_ZtoElecMu.add(
    afterCut = evtSelElectronTrkIso,
    beforeCut = evtSelElectronEcalIso,
    plot = drawJobConfigEntry(
        meName = 'ElectronQuantities/ElectronEcalIsoPt',
        title = "Electron ECAL iso. (after Electron Track iso. Cut)",
        xAxis = 'Pt',
        name = "cutFlowControlPlots_electronEcalIso_afterElectronTrkIso"
    )
)

drawJobConfigurator_ZtoElecMu.add(
    afterCut = evtSelElectronEcalIso,
    beforeCut = evtSelElectronTrk,
    plot = drawJobConfigEntry(
        meName = 'ElectronQuantities/Electron#PAR#',
        PAR = [ 'Pt', 'Eta', 'Phi' ],
        title = "Electron (after Electron ECAL iso. Cut)",
        xAxis = '#PAR#',
        name = "cutFlowControlPlots_electron_afterElectronEcalIso"
    )
)

drawJobConfigurator_ZtoElecMu.add(
    afterCut = evtSelElectronTrk,
    beforeCut = evtSelElectronTrkIP,
    plot = drawJobConfigEntry(
        meName = 'ElectronQuantities/ElectronTrackIP#PAR#',
        PAR = [ 'xy', 'z' ],
        title = "Electron Track IP_{#PAR#}(after Electron Track Cut)",
        xAxis = 'IP#PAR#',
        name = "cutFlowControlPlots_electronTrkIP_afterElectronTrk"
    )
)

drawJobConfigurator_ZtoElecMu.add(
    afterCut = evtSelElectronTrkIP,
    beforeCut = evtSelMuonTrkIso,
    plot = drawJobConfigEntry(
        meName = 'MuonQuantities/MuonTrkIsoPt',
        title = "Muon Track iso. (after Electron Track IP_{xy} Cut)",
        xAxis = 'Pt',
        name = "cutFlowControlPlots_muonTrkIso_afterElectronTrackIP"
    )
)

drawJobConfigurator_ZtoElecMu.add(
    afterCut = evtSelMuonTrkIso,
    beforeCut = evtSelMuonEcalIso,
    plot = drawJobConfigEntry(
        meName = 'MuonQuantities/MuonEcalIsoPt',
        title = "Muon ECAL iso. (after Muon Track iso. Cut)",
        xAxis = 'Pt',
        name = "cutFlowControlPlots_muonEcalIso_afterMuonTrkIso"
    )
)

drawJobConfigurator_ZtoElecMu.add(
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

drawJobConfigurator_ZtoElecMu.add(
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

drawJobConfigurator_ZtoElecMu.add(
    afterCut = evtSelMuonTrkIP,
    beforeCut = evtSelDiTauCandidateForElecMuAntiOverlapVeto,
    plot = drawJobConfigEntry(
        meName = 'DiTauCandidateQuantities/DR12',
        title = "#Delta R(Electron,Muon) (after Muon Track IP_{xy} Cut)",
        xAxis = 'dR',
        name = "cutFlowControlPlots_dR12_afterMuonTrkIPcut"
    )
)

drawJobConfigurator_ZtoElecMu.add(
    afterCut = evtSelDiTauCandidateForElecMuAntiOverlapVeto,
    beforeCut = evtSelDiTauCandidateForElecMuZeroCharge,
    plot = drawJobConfigEntry(
        meName = 'DiTauCandidateQuantities/DiTauCandidateCharge',
        title = "Charge(Electron + Muon) (after diTau anti-Overlap Veto)",
        xAxis = 'unlabeled',
        name = "cutFlowControlPlots_diTauCharge_afterAntiOverlapVeto"
    )
)

drawJobConfigurator_ZtoElecMu.add(
    afterCut = evtSelDiTauCandidateForElecMuZeroCharge,
    beforeCut = evtSelDiTauCandidateForElecMuAcoplanarity12,
    plot = drawJobConfigEntry(
        meName = 'DiTauCandidateQuantities/DPhi12',
        title = "#Delta#phi(Electron-Muon) (after opposite Charge Cut)",
        xAxis = 'dPhi',
        name = "cutFlowControlPlots_dPhiElectronMuon_afterZeroCharge"
    )
)

drawJobConfigurator_ZtoElecMu.add(
    afterCut = evtSelDiTauCandidateForElecMuAcoplanarity12,
    beforeCut = evtSelDiTauCandidateForElecMuMt1MET,
    plot = drawJobConfigEntry(
        meName = 'DiTauCandidateQuantities/Mt1MET',
        title = "M_{T}(Electron + MET) (after Acoplanarity(Electron-Muon) Cut)",
        xAxis = 'Mt',
        name = "cutFlowControlPlots_mtElectronMET_afterAcoplanarityElectronMuon"
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
    beforeCut = evtSelDiTauCandidateForElecMuPzetaDiff,
    plot = drawJobConfigEntry(
        meName = 'DiTauCandidateQuantities/PzetaDiff',
        title = "P_{#zeta} - 1.5*P_{#zeta}^{vis} (after M_{T}(Muon + MET) Cut)",
        xAxis = 'GeV',
        name = "cutFlowControlPlots_PzetaDiff_afterMtMuonMET"
    )
)

#--------------------------------------------------------------------------------
# define distributions to be plotted
# for events passing all event selection criteria
#--------------------------------------------------------------------------------

drawJobConfigurator_ZtoElecMu.add(
    afterCut = evtSelDiTauCandidateForElecMuPzetaDiff,
    plots = [
        drawJobConfigEntry(
            meName = 'ElectronQuantities/Electron#PAR#',
            PAR = [ 'Pt', 'Eta', 'Phi' ],
            title = "Electron (final Event sample)",
            xAxis = '#PAR#',
            name = "finalSamplePlots_electron"
        ),
        drawJobConfigEntry(
            meName = 'ElectronQuantities/hElectronMatchingGenParticlePdgId',
            title = "PdgId of gen. Particle matching Electron (final Event sample)",
            xAxis = 'PdgId',
            name = "finalSamplePlots_pdgIdGenParticleMatchingElectron"
        ),
        drawJobConfigEntry(
            meName = 'MuonQuantities/Muon#PAR#',
            PAR = [ 'Pt', 'Eta', 'Phi' ],
            title = "Muon (final Event sample)",
            xAxis = '#PAR#',
            name = "finalSamplePlots_muon"
        ),
        drawJobConfigEntry(
            meName = 'MuonQuantities/hMuonMatchingGenParticlePdgId',
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
            meName = 'JetQuantities/numJetsEtGt#PAR#_0EtaLt2_1AlphaGt0_3',
            PAR = [ '15', '20', '30' ],
            title = "N_{jets} with E_{T} > #PAR# GeV, |#eta| < 2.1, #alpha > 0.3 (final Event sample)",
            xAxis = 'N',
            name = "finalSamplePlots_numCentralJets"
        )
    ]
)                


