import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.Configuration.analyzeAHtoElecTau_cfi import *
from TauAnalysis.DQMTools.tools.drawJobConfigurator import *

# define template for all kins of plots
# (specific to Z --> e + tau-jet analysis)
plots_AHtoElecTau = cms.PSet(
    plots = cms.PSet(  
        dqmMonitorElements = cms.vstring(''),
        processes = cms.vstring(
			'Data',
			'Zee',
			'wPlusJetsSum',
			#'TTplusJets',
			'qcdBCtoESum',
			'qcdEMenrichedSum',
			'gammaPlusJetsSum',
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
		'Zee',
		'wPlusJetsSum',
		 #'TTplusJets',
		'qcdBCtoESum',
		'qcdEMenrichedSum',
		'gammaPlusJetsSum',
		'Ztautau'
    )
)

drawJobConfigurator_AHtoElecTau = drawJobConfigurator(
    template = plots_AHtoElecTau,
    dqmDirectory = '#PROCESSDIR#/ahElecTauAnalyzer/'
)

#--------------------------------------------------------------------------------
# define cut-flow control plots;
# show distribution of each quantity used in event selection
# (**before** quantity is cutted on)
#--------------------------------------------------------------------------------

drawJobConfigurator_AHtoElecTau.add(
    afterCut = evtSelPrimaryEventVertex,
    beforeCut = evtSelPrimaryEventVertexQuality,
    plot = drawJobConfigEntry(
        meName = 'VertexQuantities/VertexChi2Prob',
        title = "P(#Chi^{2}_{vtx}) (after primary Event Vertex Cut)",
        xAxis = 'prob',
        name = "cutFlowControlPlots_vertexChi2Prob_afterPrimaryEventVertex"
    )
)

drawJobConfigurator_AHtoElecTau.add(
    afterCut = evtSelPrimaryEventVertexQuality,
    beforeCut = evtSelPrimaryEventVertexPosition,
    plot = drawJobConfigEntry(
        meName = 'VertexQuantities/VertexZ',
        title = "z_{vtx} (after primary Event Vertex quality Cut)",
        xAxis = 'posZ',
        name = "cutFlowControlPlots_vertexZ_afterPrimaryEventVertexQuality"
    )
)

drawJobConfigurator_AHtoElecTau.add(
    afterCut = evtSelPrimaryEventVertexPosition,
    beforeCut = evtSelElectronId,
    plot = drawJobConfigEntry(
        meName = 'ElectronQuantities/Electron#PAR#',
        PAR = [ 'Pt', 'Eta', 'Phi' ],
        title = "Electron (after primary Event Vertex position Cut)",
        xAxis = '#PAR#',
        name = "cutFlowControlPlots_electron_afterPrimaryEventVertexPosition"
	)
)    

drawJobConfigurator_AHtoElecTau.add(
    afterCut = evtSelElectronId,
    beforeCut = evtSelElectronAntiCrack,
    plot = drawJobConfigEntry(
        meName = 'ElectronQuantities/Electron#PAR#',
        PAR = [ 'Pt', 'Eta', 'Phi' ],
        title = "Electron (after Electron ID Cut)",
        xAxis = '#PAR#',
        name = "cutFlowControlPlots_electron_afterElectronId"
    )
)

drawJobConfigurator_AHtoElecTau.add(
    afterCut = evtSelElectronAntiCrack,
    beforeCut = evtSelElectronEta,
    plot = drawJobConfigEntry(
        meName = 'ElectronQuantities/Electron#PAR#',
        PAR = [ 'Pt', 'Eta', 'Phi' ],
        title = "Electron (after Electron anti-crack Cut)",
        xAxis = '#PAR#',
        name = "cutFlowControlPlots_electron_afterElectronAntiCrack"
    )
)

drawJobConfigurator_AHtoElecTau.add(
    afterCut = evtSelElectronEta,
    beforeCut = evtSelElectronPt,
    plot = drawJobConfigEntry(
        meName = 'ElectronQuantities/Electron#PAR#',
        PAR = [ 'Pt', 'Eta', 'Phi' ],
        title = "Electron (after Electron  #eta Cut)",
        xAxis = '#PAR#',
        name = "cutFlowControlPlots_electron_afterElectronEta"
    )
)    

drawJobConfigurator_AHtoElecTau.add(
    afterCut = evtSelElectronPt,
    beforeCut = evtSelTauAntiOverlapWithElectronsVeto,
    plot = drawJobConfigEntry(
        meName = 'TauQuantities/Tau#PAR#',
        PAR = [ 'Pt', 'Eta', 'Phi' ],
        title = "Tau (after Electron P_{T} Cut)",
        xAxis = '#PAR#',
        name = "cutFlowControlPlots_tau_afterElectronPt"
    )
)

drawJobConfigurator_AHtoElecTau.add(
    afterCut = evtSelTauAntiOverlapWithElectronsVeto,
    beforeCut = evtSelTauEta,
    plots = [
        drawJobConfigEntry(
            meName = 'TauQuantities/Tau#PAR#',
            PAR = [ 'Pt', 'Eta', 'Phi' ],
            title = "Tau (after Electron-Tau overlap Veto)",
            xAxis = '#PAR#',
            name = "cutFlowControlPlots_tau_afterTauAntiOverlapWithElectronsVeto"
        ),
        drawJobConfigEntry(
            meName = 'TauQuantities/TauLeadTrkPt',
            title = "Tau lead. Track Pt (after Electron-Tau overlap Veto)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_tauLeadTrkPt_afterTauAntiOverlapWithElectronsVeto"
            )
    ]
)

drawJobConfigurator_AHtoElecTau.add(
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
            title = "Tau lead. Track Pt (after Tau  #eta Cut)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_tauLeadTrkPt_afterTauEta"
            )
    ]
)

drawJobConfigurator_AHtoElecTau.add(
    afterCut = evtSelTauPt,
    beforeCut = evtSelElectronIso,
    plots = [
        drawJobConfigEntry(
            meName = 'ElectronQuantities/ElectronParticleFlowIsoPt',
            title = "Electron PFCandidate iso. (after Tau Pt Cut)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_electronPFCandidateAbsIso_afterTauPt"
        ),
        drawJobConfigEntry(
            meName = 'ElectronQuantities/ElectronParticleFlowIsoPtRel',
            title = "Electron PFCandidate rel. iso. (after Tau Pt Cut)",
            xAxis = 'unlabeled',
            name = "cutFlowControlPlots_electronPFCandidateRelIso_afterTauPt"
        ),
        drawJobConfigEntry(
            meName = 'ElectronQuantities/ElectronPFChargedHadronIsoPt',
            title = "Electron PFChargedHadron iso. (after Tau Pt Cut)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_electronPFChargedHadronAbsIso_afterTauPt"
        ),
        drawJobConfigEntry(
            meName = 'ElectronQuantities/ElectronPFChargedHadronIsoPtRel',
            title = "Electron PFChargedHadron rel. iso. (after Tau Pt Cut)",
            xAxis = 'unlabeled',
            name = "cutFlowControlPlots_electronPFChargedHadronRelIso_afterTauPt"
        ),
        drawJobConfigEntry(
            meName = 'ElectronQuantities/ElectronPFNeutralHadronIsoPt',
            title = "Electron PFNeutralHadron iso. (after Tau Pt Cut)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_electronPFNeutralHadronAbsIso_afterTauPt"
        ),
        drawJobConfigEntry(
            meName = 'ElectronQuantities/ElectronPFNeutralHadronIsoPtRel',
            title = "Electron PFNeutralHadron rel. iso. (after Tau Pt Cut)",
            xAxis = 'unlabeled',
            name = "cutFlowControlPlots_electronPFNeutralHadronRelIso_afterTauPt"
        ),
        drawJobConfigEntry(
            meName = 'ElectronQuantities/ElectronPFGammaIsoPt',
            title = "Electron PFGamma iso. (after Tau Pt Cut)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_electronPFGammaAbsIso_afterTauPt"
        ),
        drawJobConfigEntry(
            meName = 'ElectronQuantities/ElectronPFGammaIsoPtRel',
            title = "Electron PFGamma rel. iso. (after Tau Pt Cut)",
            xAxis = 'unlabeled',
            name = "cutFlowControlPlots_electronPFGammaRelIso_afterTauPt"
        ),
        drawJobConfigEntry(
            meName = 'ElectronQuantities/ElectronPFCombIsoPtBarrel',
            title = "Barrel electron comb. PF iso. (after Tau Pt Cut)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_electronCombPFAbsIsoEB_afterTauPt"
        ),
        drawJobConfigEntry(
            meName = 'ElectronQuantities/ElectronPFCombIsoPtRelBarrel',
            title = "Barrel electron comb. PF rel. iso. (after Tau Pt Cut)",
            xAxis = 'unlabeled',
            name = "cutFlowControlPlots_electronCombPFRelIsoEB_afterTauPt"
        ),
        drawJobConfigEntry(
            meName = 'ElectronQuantities/ElectronPFCombIsoPtBarrel',
            title = "Barrel electron comb. PF iso. (after Tau Pt Cut)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_electronCombPFAbsIsoEB_afterTauPt"
        ),
        drawJobConfigEntry(
            meName = 'ElectronQuantities/ElectronPFCombIsoPtRelBarrel',
            title = "Barrel electron comb. PF rel. iso. (after Tau Pt Cut)",
            xAxis = 'unlabeled',
            name = "cutFlowControlPlots_electronCombPFRelIsoEB_afterTauPt"
        )
    ]
)

drawJobConfigurator_AHtoElecTau.add(
    afterCut = evtSelElectronIso,
    beforeCut = evtSelElectronConversionVeto,
     plot = drawJobConfigEntry(
        meName = 'ElectronQuantities/Electron#PAR#',
        PAR = [ 'Pt', 'Eta', 'Phi' ],
        title = "#PAR# (after Electron ECAL iso. Cut)",
        xAxis = '#PAR#',
        name = "cutFlowControlPlots_electron_afterElectronEcalIso"
    )
)

drawJobConfigurator_AHtoElecTau.add(
    afterCut = evtSelElectronConversionVeto,
    beforeCut = evtSelElectronTrkIP,
     plot = drawJobConfigEntry(
        meName = 'ElectronQuantities/ElectronTrackIP#PAR#',
        PAR = [ 'xy', 'z' ],
        title = "Electron Track IP_{#PAR#} (after Electron Conversion Veto)",
        xAxis = 'IP#PAR#',
        name = "cutFlowControlPlots_electronTrkIP_afterElectronConversionVeto"
    )
)

drawJobConfigurator_AHtoElecTau.add(
    afterCut = evtSelElectronTrkIP,
    beforeCut = evtSelTauLeadTrk,
    plots = [
        drawJobConfigEntry(
        	meName = 'ElectronQuantities/Electron#PAR#',
            PAR = [ 'Pt', 'Eta', 'Phi' ],
            title = "Electron (after Electron Track IP_{xy} Cut)",
            xAxis = '#PAR#',
            name = "cutFlowControlPlots_electron_afterElectronTrkIP"
        ),
        drawJobConfigEntry(
            meName = 'TauQuantities/Tau#PAR#',
            PAR = [ 'Pt', 'Eta', 'Phi' ],
            title = "Tau (after Electron Track IP_{xy} Cut)",
            xAxis = '#PAR#',
            name = "cutFlowControlPlots_tau_afterElectronTrkIP"
        ),
        drawJobConfigEntry(
            meName = 'TauQuantities/TauLeadTrkPt',
            title = "Tau lead. Track Pt (after Electron Track IP_{xy} Cut)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_tauLeadTrkPt_afterElectronTrkIP"
        )
    ]
)

drawJobConfigurator_AHtoElecTau.add(
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
            title = "Tau lead. Track Pt (after Tau lead. Track Cut)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_tauLeadTrkPt_afterTauLeadTrk"
        )
    ]
)

drawJobConfigurator_AHtoElecTau.add(
    afterCut = evtSelTauLeadTrkPt,
    beforeCut = evtSelTauTaNCdiscr,
    plots = [
			drawJobConfigEntry(
        meName = 'TauQuantities/TauDiscriminatorTaNCloose',
        title = "TaNC output (loose criteria, after Tau lead. Track Pt Cut)",
        xAxis = 'unlabeled',
        name = "cutFlowControlPlots_tauDiscrTaNCfrQuarterPercent_afterTauLeadTrkPt"
			)
		]
)

drawJobConfigurator_AHtoElecTau.add(
    afterCut = evtSelTauTaNCdiscr,
    beforeCut = evtSelTauProng,
    plots = [
			drawJobConfigEntry(
        meName = 'TauQuantities/TauNumIsoPFChargedHadrons',
        title = "Num PF charged hadrons in Tau iso. region (after Tau TaNC cut)",
        xAxis = 'unlabeled',
        name = "cutFlowControlPlots_tauTrkIso_afterTauDiscrTaNCloose"
			)
		]
)

drawJobConfigurator_AHtoElecTau.add(
    afterCut = evtSelTauProng,
    beforeCut = evtSelTauCharge,
    plots = [
			drawJobConfigEntry(
        meName = 'TauQuantities/TauCharge',
        title = "Tau Charge (#Sigma Track Charge in Signal Cone, after Tau 1-Prong||3-Prong Cut)",
        xAxis = 'unlabeled',
        name = "cutFlowControlPlots_tauCharge_afterTauProng"
			)
		]
)

drawJobConfigurator_AHtoElecTau.add(
    afterCut = evtSelTauCharge,
    beforeCut = evtSelTauElectronVeto,
    plots = [
        drawJobConfigEntry(
            meName = 'TauQuantities/TauDiscriminatorAgainstElectrons',
            title = "Tau anti-Electron Discr. (after Charge(Tau) = +/-1 Cut)",
            xAxis = 'unlabeled',
            name = "cutFlowControlPlots_tauAntiElectronDiscr_afterTauCharge"
        )
    ]
)

drawJobConfigurator_AHtoElecTau.add(
    afterCut = evtSelTauElectronVeto,
    beforeCut = evtSelTauEcalCrackVeto,
    plots = [
			drawJobConfigEntry(
        meName = 'TauQuantities/Tau#PAR#',
        PAR = [ 'Pt', 'Eta', 'Phi' ],
        title = "Tau (after Tau anti-Electron Veto)",
        xAxis = '#PAR#',
        name = "cutFlowControlPlots_tau_afterTauElectronVeto"
			)
		]
)

drawJobConfigurator_AHtoElecTau.add(
    afterCut = evtSelTauEcalCrackVeto,
    beforeCut = evtSelTauMuonVeto,
    plots = [
		drawJobConfigEntry(
			meName = 'TauQuantities/Tau#PAR#',
			PAR = [ 'Pt', 'Eta', 'Phi' ],
			title = "Tau (after Tau ECAL Crack Veto)",
			xAxis = '#PAR#',
			name = "cutFlowControlPlots_tau_afterTauEcalCrackVeto"
			),
		drawJobConfigEntry(
			meName = 'TauQuantities/TauDiscriminatorAgainstMuons',
			title = "Tau anti-muon Discr. (after Tau ECAL Crack Veto)",
			xAxis = 'unlabeled',
			name = "cutFlowControlPlots_tauAntiMuonDiscr_afterTauEcalCrackVeto"
			)
		]
)

drawJobConfigurator_AHtoElecTau.add(
    afterCut = evtSelTauMuonVeto,
    beforeCut = evtSelDiTauCandidateForElecTauAntiOverlapVeto,
	plots = [
		drawJobConfigEntry(
			meName = 'DiTauCandidateQuantities/DR12',
			title = "#Delta R(Electron,Tau) (after Tau anti-Muon Veto)",
			xAxis = 'dR',
			name = "cutFlowControlPlots_dR12_afterTauMuonVeto"
			)
		]
)

#drawJobConfigurator_AHtoElecTau.add(
#    afterCut = evtSelDiTauCandidateForElecTauAntiOverlapVeto,
#    beforeCut = evtSelDiTauCandidateForElecTauZeroCharge,
#    plots = [
#			drawJobConfigEntry(
#        meName = 'DiTauCandidateQuantities/DiTauCandidateCharge',
#        title = "Charge(Electron + Tau) (after diTau anti-Overlap Veto)",
#        xAxis = 'unlabeled',
#        name = "cutFlowControlPlots_diTauCharge_afterAntiOverlapVeto"
#			),
#      drawJobConfigEntry(
#            meName = 'DiTauCandidateQuantities/VisMass',
#            title = "M_{vis}(Electron + Tau) (after diTau anti-Overlap Veto)",
#            xAxis = 'Mass',
#            name = "cutFlowControlPlots_visibleMass_afterAntiOverlapVeto"
#      )
#		]
#)

drawJobConfigurator_AHtoElecTau.add(
    afterCut = evtSelDiTauCandidateForElecTauAntiOverlapVeto,
    beforeCut = evtSelDiTauCandidateForElecTauMt1MET,
    plot = drawJobConfigEntry(
        meName = 'DiTauCandidateQuantities/Mt1MET',
        title = "M_{T}(Electron + MET) (after diTau anti-Overlap Veto)",
        xAxis = 'Mt',
        name = "cutFlowControlPlots_mtElectronMET_afterAntiOverlapVeto"
    )
)

drawJobConfigurator_AHtoElecTau.add(
    afterCut = evtSelDiTauCandidateForElecTauMt1MET,
    beforeCut = evtSelDiTauCandidateForElecTauPzetaDiff,
    plots = [
			drawJobConfigEntry(
        meName = 'DiTauCandidateQuantities/PzetaDiff',
        title = "P_{#zeta} - 1.5*P_{#zeta}^{vis} (after transverse Mass Cut)",
        xAxis = 'GeV',
        name = "cutFlowControlPlots_PzetaDiff_afterMt1MET"
			),
      drawJobConfigEntry(
            meName = 'DiTauCandidateQuantities/VisMass',
            title = "M_{vis}(Electron + Tau) (after transverse Mass Cut)",
            xAxis = 'Mass',
            name = "cutFlowControlPlots_visibleMass_afterMt1MET"
       )
		]
)

drawJobConfigurator_AHtoElecTau.add(
    afterCut = evtSelDiTauCandidateForElecTauPzetaDiff,
    beforeCut = evtSelDiElecPairZeeHypothesisVetoByLooseIsolation,
    plots = [
		drawJobConfigEntry(
			meName = 'DiTauCandidateZeeHypothesisQuantities/VisMassBestMach',
			title = "M_{vis}(Electron + Tau, Z #rightarrow e^{+} e^{-} Mass hypothesis) (after P_{#zeta} Cut)",
			xAxis = 'Mass',
			name = "cutFlowControlPlots_mVisibleZeeHypothesis_afterPzetaDiff"
		),
		drawJobConfigEntry(
            meName = 'DiTauCandidateQuantities/VisMass',
            title = "M_{vis}(Electron + Tau) (after P_{#zeta} Cut)",
            xAxis = 'Mass',
            name = "cutFlowControlPlots_visibleMass_afterPzetaDiff"
		),
		drawJobConfigEntry(
			meName = 'DiElecZeeHypothesisByLooseIsolationQuantities/DiTauCandidateCharge',
			title = "Charge(iso. Elec. + iso. Elec.) (after P_{#zeta} Cut)",
			xAxis = 'unlabeled',
			name = "cutFlowControlPlots_diElectronCharge_afterPzetaDiff"
		)
	]
)
#--------------------------------------------------------------------------------
# define cut-flow control plots specific to "non-b-tag" analysis path
#--------------------------------------------------------------------------------

drawJobConfigurator_AHtoElecTau_woBtag = copy.deepcopy(drawJobConfigurator_AHtoElecTau)
drawJobConfigurator_AHtoElecTau_woBtag.setDQMdirectory('#PROCESSDIR#/ahElecTauAnalyzerOS_woBtag/')

drawJobConfigurator_AHtoElecTau_woBtag.add(
    afterCut = evtSelDiElecPairZeeHypothesisVetoByLooseIsolation,
    beforeCut = evtSelNonCentralJetEt20bTag,
    plot = drawJobConfigEntry(
    meName = 'JetQuantities/Jet#PAR#',
        PAR = [ 'Pt', 'Eta', 'Phi' ],
        title = "Jet (after Z #rightarrow #e^{+} #e^{-} hypothesis Veto)",
        xAxis = '#PAR#',
        name = "cutFlowControlPlots_jet_afterZeeHypothesisVeto"
    )
)

drawJobConfigurator_AHtoElecTau_woBtag.add(
    afterCut = evtSelNonCentralJetEt20bTag,
    beforeCut = evtSelDiTauCandidateForAHtoElecTauZeroCharge,
    plot = drawJobConfigEntry(
        meName = 'DiTauCandidateQuantities/DiTauCandidateCharge',
        title = "Charge(Electron + Tau) (after Jet no b-tag Cut)",
        xAxis = 'unlabeled',
        name = "cutFlowControlPlots_diTauCharge_afterJetNoBtag"
    )
)

#--------------------------------------------------------------------------------
# define cut-flow control plots specific to "b-tag" analysis path
#--------------------------------------------------------------------------------

drawJobConfigurator_AHtoElecTau_wBtag = copy.deepcopy(drawJobConfigurator_AHtoElecTau)
drawJobConfigurator_AHtoElecTau_wBtag.setDQMdirectory('#PROCESSDIR#/ahElecTauAnalyzerOS_wBtag/')

drawJobConfigurator_AHtoElecTau_wBtag.add(
    afterCut = evtSelDiElecPairZeeHypothesisVetoByLooseIsolation,
    beforeCut = evtSelCentralJetEt20,
    plot = drawJobConfigEntry(
    meName = 'JetQuantities/Jet#PAR#',
        PAR = [ 'Pt', 'Eta', 'Phi' ],
        title = "Jet (after Z #rightarrow #e^{+} #e^{-} hypothesis Veto)",
        xAxis = '#PAR#',
        name = "cutFlowControlPlots_jet_afterZeeHypothesisVeto"
    )
)

drawJobConfigurator_AHtoElecTau_wBtag.add(
    afterCut = evtSelCentralJetEt20,
    beforeCut = evtSelCentralJetEt20bTag,
    plots = [
        drawJobConfigEntry(
            meName = 'JetQuantities/Jet#PAR#',
            PAR = [ 'Pt', 'Eta', 'Phi' ],
            title = "Jet (after Jet P{T} and #eta Cuts)",
            xAxis = '#PAR#',
            name = "cutFlowControlPlots_jet_afterJetPtAndEta"
        ),
        drawJobConfigEntry(
            meName = 'JetQuantities/BtagDisc_trackCountingHighEffBJetTags',
            title = "Jet b-Tag Discr. (after Jet P{T} and #eta Cuts)",
            xAxis = 'unlabeled',
            name = "cutFlowControlPlots_jetBtagDiscr_afterJetPtAndEta"
        ),
        drawJobConfigEntry(
            meName = 'JetQuantities/NumBtags_trackCountingHighEffBJetTags',
            title = "Num. Jets with b-Tag (after Jet P{T} and #eta Cuts)",
            xAxis = 'unlabeled',
            name = "cutFlowControlPlots_numBtagJets_afterJetPtAndEta"
        )
    ]

)

drawJobConfigurator_AHtoElecTau_wBtag.add(
    afterCut = evtSelCentralJetEt20bTag,
    beforeCut = evtSelDiTauCandidateForAHtoElecTauZeroCharge,
    plot = drawJobConfigEntry(
        meName = 'DiTauCandidateQuantities/DiTauCandidateCharge',
        title = "Charge(Electron + Tau) (after Jet b-tag Cut)",
        xAxis = 'unlabeled',
        name = "cutFlowControlPlots_diTauCharge_afterJetBtag"
    )
)


#--------------------------------------------------------------------------------
# define distributions to be plotted
# for events passing all event selection criteria
#--------------------------------------------------------------------------------


finalSamplePlots = [
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
			meName = 'ElectronQuantities/ElectronMatchingFinalStateGenParticlePdgId',
			title = "PdgId of final-state gen. Particle matching Electron (final Event sample)",
			xAxis = 'PdgId',
			name = "finalSamplePlots_pdgIdFinalStateGenParticleMatchingElectron"
		),
		drawJobConfigEntry(
			meName = 'ElectronQuantities/ElectronParticleFlowIsoPtRel',
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
			meName = 'TauQuantities/TauMatchingFinalStateGenParticlePdgId',
			title = "PdgId of final-state gen. Particle matching Tau (final Event sample)",
			xAxis = 'PdgId',
			name = "finalSamplePlots_pdgIdFinalStateGenParticleMatchingTau"
		),
		drawJobConfigEntry(
			meName = 'TauQuantities/TauLeadTrkPt',
			title = "Tau lead. Track Pt (final Event sample)",
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
			meName = 'DiTauCandidateQuantities/DPhi12',
			title = "#Delta#phi(Electron-Tau) (final Event sample)",
			xAxis = 'dPhi',
			name = "finalSamplePlots_dPhiElectronTau"
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
			title = "M_{T}(Tau + MET) (final Event sample)",
			xAxis = 'Mt',
			name = "finalSamplePlots_mtTauMET"
		),
		drawJobConfigEntry(
			meName = 'DiTauCandidateQuantities/Mt12MET',
			title = "M_{T}(Electron + Tau + MET) (final Event sample)",
			xAxis = 'Mt',
			name = "finalSamplePlots_mtElectronTauMET"
		),
		drawJobConfigEntry(
				meName = 'DiTauCandidateQuantities/VisMass',
				title = "M_{vis}(Electron + Tau) (final Event sample)",
				xAxis = 'Mass',
				name = "finalSamplePlots_mVisible"
				),
		drawJobConfigEntry(
				meName = 'DiTauCandidateQuantities/CDFmethodMass',
				title = "M(Electron + Tau), CDF method (final Event sample)",
				xAxis = 'Mass',
				name = "finalSamplePlots_mCDFmethod"
				),
		drawJobConfigEntry(
				meName = 'DiTauCandidateQuantities/CollinearApproxMass',
				title = "M(Electron + Tau), collinear Approx. (final Event sample)",
				xAxis = 'Mass',
				name = "finalSamplePlots_mCollApprox"
				),
		drawJobConfigEntry(
				meName = 'DiTauCandidateSVfitQuantities/psKine_MEt_ptBalance/Mass',
				title = "M(Electron + Tau), SVfit method (final Event sample)",
				xAxis = 'Mass',
				name = "finalSamplePlots_mSVmethod"
				),
		drawJobConfigEntry(
				meName = 'DiTauCandidateQuantities/Ht12MET',
				title = "#Sigma H_{T}(Electron + Tau + MET) (final Event sample)",
				xAxis = 'Mass',
				name = "finalSamplePlots_ht"
				),
		drawJobConfigEntry(
				meName = 'JetQuantities/BtagDisc_trackCountingHighEffBJetTags',
				title = "Jet b-Tag Discr. (final Event sample)",
				xAxis = 'unlabeled',
				name = "finalSamplePlots_jetBtagDiscr"
				),
		drawJobConfigEntry(
				meName = 'JetQuantities/NumJets',
				title = "Num. Jets with b-Tag (final Event sample)",
				xAxis = 'unlabeled',
				name = "finalSamplePlots_numBtagJets"
				)
	]

drawJobConfigurator_AHtoElecTau_woBtag.add(
    afterCut = evtSelDiTauCandidateForAHtoElecTauZeroCharge,
    plots = finalSamplePlots
)

drawJobConfigurator_AHtoElecTau_wBtag.add(
    afterCut = evtSelDiTauCandidateForAHtoElecTauZeroCharge,
    plots = finalSamplePlots
)

# Build draw job configurations for the same sign final event plots
drawJobConfigurator_AHtoElecTau_woBtagSS = drawJobConfigurator(
    template = plots_AHtoElecTau,
    dqmDirectory = ''
)
drawJobConfigurator_AHtoElecTau_wBtagSS = drawJobConfigurator(
    template = plots_AHtoElecTau,
    dqmDirectory = ''
)
drawJobConfigurator_AHtoElecTau_woBtagSS.setDQMdirectory('#PROCESSDIR#/ahElecTauAnalyzerSS_woBtag/')
drawJobConfigurator_AHtoElecTau_wBtagSS.setDQMdirectory('#PROCESSDIR#/ahElecTauAnalyzerSS_wBtag/')

drawJobConfigurator_AHtoElecTau_woBtagSS.add(
    afterCut = evtSelDiTauCandidateForAHtoElecTauNonZeroCharge,
    plots = finalSamplePlots
)
drawJobConfigurator_AHtoElecTau_wBtagSS.add(
    afterCut = evtSelDiTauCandidateForAHtoElecTauNonZeroCharge,
    plots = finalSamplePlots
)

