import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.Configuration.analyzeZtoElecTau_cfi import *
from TauAnalysis.DQMTools.tools.drawJobConfigurator import *

# define template for all kins of plots
# (specific to Z --> e + tau-jet analysis)
plots_ZtoElecTau = cms.PSet(
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

drawJobConfigurator_ForElecTau = drawJobConfigurator(
    template = plots_ZtoElecTau,
    dqmDirectory = '#PROCESSDIR#/zElecTauAnalyzer/'
)

#--------------------------------------------------------------------------------
# define cut-flow control plots;
# show distribution of each quantity used in event selection
# (**before** quantity is cutted on)
#--------------------------------------------------------------------------------

drawJobConfigurator_ForElecTau.add(
    afterCut = evtSelDataQuality,
    beforeCut = evtSelElectronId,
    plot = drawJobConfigEntry(
        meName = 'ElectronQuantities/Electron#PAR#',
        PAR = [ 'Pt', 'Eta', 'Phi' ],
        title = "Electron (after data quality cut)",
        xAxis = '#PAR#',
        name = "cutFlowControlPlots_electron_afterDataQuality"
	)
)    

drawJobConfigurator_ForElecTau.add(
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

drawJobConfigurator_ForElecTau.add(
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

drawJobConfigurator_ForElecTau.add(
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

drawJobConfigurator_ForElecTau.add(
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

drawJobConfigurator_ForElecTau.add(
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

drawJobConfigurator_ForElecTau.add(
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

drawJobConfigurator_ForElecTau.add(
    afterCut = evtSelTauPt,
    beforeCut = evtSelElectronIso,
    plots = [
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
            meName = 'ElectronQuantities/ElectronPFCombIsoPtEndcap',
            title = "Endcap electron comb. PF iso. (after Tau Pt Cut)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_electronCombPFAbsIsoEE_afterTauPt"
        ),
        drawJobConfigEntry(
            meName = 'ElectronQuantities/ElectronPFCombIsoPtRelEndcap',
            title = "Endcap electron comb. PF rel. iso. (after Tau Pt Cut)",
            xAxis = 'unlabeled',
            name = "cutFlowControlPlots_electronCombPFRelIsoEE_afterTauPt"
        )
    ]
)

drawJobConfigurator_ForElecTau.add(
        afterCut = evtSelElectronIso,
        beforeCut = evtSelElectronConversionVeto,
        plots = [
            drawJobConfigEntry(
                meName = 'ElectronQuantities/Electron#PAR#',
                PAR = [ 'Pt', 'Eta', 'Phi' ],
                title = "#PAR# (after Electron iso. Cut)",
                xAxis = '#PAR#',
                name = "cutFlowControlPlots_electron_afterElectronIso"
                ),
            drawJobConfigEntry(
                meName = 'ElectronQuantities/ElectronConversionRadius',
                title = "Vertex radius of best conv. pair (after Electron iso. Cut)",
                xAxis = 'posX',
                name = "cutFlowControlPlots_electronConvRadius_afterElectronIso"
                ),
            drawJobConfigEntry(
                meName = 'ElectronQuantities/ElectronConversionDoca',
                title = "DOCA of best conv. pair (after Electron iso. Cut)",
                xAxis = 'posX',
                name = "cutFlowControlPlots_electronConvDOCA_afterElectronIso"
                ),
            drawJobConfigEntry(
                meName = 'ElectronQuantities/ElectronConversionDeltaCotTheta',
                title = "#DeltaCot(#theta) of best conv. pair (after Electron iso. Cut)",
                xAxis = 'unlabeled',
                name = "cutFlowControlPlots_electronConvDeltaCotTheta_afterElectronIso"
                ),
            drawJobConfigEntry(
                meName = 'ElectronQuantities/ElectronMissingExpInnerHits',
                title = " Missing inner hits (after Electron iso. Cut)",
                xAxis = 'unlabeled',
                name = "cutFlowControlPlots_electronMissingInnerHits_afterElectronIso"
                )
            ]
)

drawJobConfigurator_ForElecTau.add(
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

drawJobConfigurator_ForElecTau.add(
    afterCut = evtSelElectronTrkIP,
    beforeCut = evtSelTauDecayModeFinding,
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
        )
    ]
)

drawJobConfigurator_ForElecTau.add(
    afterCut = evtSelTauDecayModeFinding,
    beforeCut = evtSelTauLeadTrkPt,
	plots = [
        drawJobConfigEntry(
            meName = 'TauQuantities/TauLeadTrkPt',
            title = "Tau lead. Track Pt (after Tau decay mode finding)",
            xAxis = 'Pt',
            name = "cutFlowControlPlots_tauLeadTrkPt_afterTauDecayModeFinding"
        ),
		drawJobConfigEntry(
			meName = 'TauQuantities/Tau#PAR#',
			PAR = [ 'Pt', 'Eta', 'Phi' ],
			title = "Tau (after Tau decay mode finding)",
			xAxis = '#PAR#',
			name = "cutFlowControlPlots_tau_afterTauDecayModeFinding"
		)
	]
)

drawJobConfigurator_ForElecTau.add(
    afterCut = evtSelTauLeadTrkPt,
    beforeCut = evtSelTauIso,
	plots = [
		drawJobConfigEntry(
			meName = 'TauQuantities/TauDiscriminatorHPSloose',
			title = "Tau ID output (HPS loose, after Tau lead track Pt Cut)",
			xAxis = 'unlabeled',
			name = "cutFlowControlPlots_tauDiscrHPSloose_afterTauLeadTrkPt"
			),
		drawJobConfigEntry(
			meName = 'TauQuantities/TauDiscriminatorHPSmedium',
			title = "Tau ID output (HPS medium, after Tau lead track Pt Cut)",
			xAxis = 'unlabeled',
			name = "cutFlowControlPlots_tauDiscrHPSmedium_afterTauLeadTrkPt"
			),
		drawJobConfigEntry(
			meName = 'TauQuantities/Tau#PAR#',
			PAR = [ 'Pt', 'Eta', 'Phi' ],
			title = "Tau (after Tau lead track Pt Cut)",
			xAxis = '#PAR#',
			name = "cutFlowControlPlots_tau_afterTauLeadTrkPt"
			)
	]
)

drawJobConfigurator_ForElecTau.add(
    afterCut = evtSelTauIso,
    beforeCut = evtSelTauElectronVeto,
	plots = [
		drawJobConfigEntry(
			meName = 'TauQuantities/TauDiscriminatorAgainstElectronsTight',
			title = "Tau tight anti-Electron Discr. (after Tau ID)",
			xAxis = 'unlabeled',
			name = "cutFlowControlPlots_tauAntiElectronDiscr_afterTauIso"
			),
		drawJobConfigEntry(
			meName = 'TauQuantities/Tau#PAR#',
			PAR = [ 'Pt', 'Eta', 'Phi' ],
			title = "Tau (after Tau ID)",
			xAxis = '#PAR#',
			name = "cutFlowControlPlots_tau_afterTauIso"
			)
	]
)

drawJobConfigurator_ForElecTau.add(
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

drawJobConfigurator_ForElecTau.add(
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
			meName = 'TauQuantities/TauDiscriminatorAgainstMuonsTight',
			title = "Tau anti-muon Discr. (after Tau ECAL Crack Veto)",
			xAxis = 'unlabeled',
			name = "cutFlowControlPlots_tauAntiMuonDiscr_afterTauEcalCrackVeto"
			)
		]
)

#--------------------------------------------------------------------------------
#  configure plots specific to Z -> e + tau-jet
#--------------------------------------------------------------------------------

drawJobConfigurator_ZtoElecTau = copy.deepcopy(drawJobConfigurator_ForElecTau)

drawJobConfigurator_ZtoElecTau.add(
    afterCut = evtSelPrimaryEventVertexForElecTau,
    beforeCut = evtSelPrimaryEventVertexQualityForElecTau,
    plot = drawJobConfigEntry(
        meName = 'VertexQuantities/VertexChi2Prob',
        title = "P(#Chi^{2}_{vtx}) (after Common Electron+Tau Vertex)",
        xAxis = 'prob',
        name = "cutFlowControlPlots_vertexChi2Prob_afterPrimaryEventVertexForElecTau"
    )
)

drawJobConfigurator_ZtoElecTau.add(
    afterCut = evtSelPrimaryEventVertexQualityForElecTau,
    beforeCut = evtSelPrimaryEventVertexPositionForElecTau,
    plot = drawJobConfigEntry(
        meName = 'VertexQuantities/VertexZ',
        title = "z_{vtx} (after primary Event Vertex quality Cut)",
        xAxis = 'posZ',
        name = "cutFlowControlPlots_vertexZ_afterPrimaryEventVertexQualityForElecTau"
    )
)

drawJobConfigurator_ZtoElecTau.add(
    afterCut = evtSelPrimaryEventVertexPositionForElecTau,
    beforeCut = evtSelDiTauCandidateForElecTauMt1MET,
    plot = drawJobConfigEntry(
        meName = 'DiTauCandidateQuantities/Mt1MET',
        title = "M_{T}(Electron + MET) (after primary Event Vertex position Cut)",
        xAxis = 'Mt',
        name = "cutFlowControlPlots_mtElectronMET_afterPrimaryEventVertexPositionForElecTau"
    )
)

drawJobConfigurator_ZtoElecTau.add(
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

drawJobConfigurator_ZtoElecTau.add(
    afterCut = evtSelDiTauCandidateForElecTauPzetaDiff,
    beforeCut = evtSelDiElecPairZeeHypothesisVetoByLooseIsolation,
    plots = [
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
# define distributions to be plotted
# for events passing all event selection criteria
#--------------------------------------------------------------------------------


finalSamplePlots = [
		drawJobConfigEntry(
			meName = 'ElectronQuantities/Electron#PAR#',
			PAR = [ 'Pt', 'Eta', 'Phi' ],
			title = "Electron (final event sample)",
			xAxis = '#PAR#',
			name = "finalSamplePlots_electron"
			),
		drawJobConfigEntry(
			meName = 'ElectronQuantities/ElectronMatchingGenParticlePdgId',
			title = "PdgId of gen. Particle matching Electron (final event sample)",
			xAxis = 'PdgId',
			name = "finalSamplePlots_pdgIdGenParticleMatchingElectron"
			),
		drawJobConfigEntry(
			meName = 'ElectronQuantities/ElectronMatchingFinalStateGenParticlePdgId',
			title = "PdgId of final-state gen. Particle matching Electron (final event sample)",
			xAxis = 'PdgId',
			name = "finalSamplePlots_pdgIdFinalStateGenParticleMatchingElectron"
			),
		drawJobConfigEntry(
			meName = 'ElectronQuantities/ElectronPFCombIsoPtRelBarrel',
			title = "Barrel electron comb. PF rel. iso. (final event sample)",
			xAxis = 'unlabeled',
			name = "finalSamplePlots_electronCombPFRelIsoEB"
			),
		drawJobConfigEntry(
			meName = 'ElectronQuantities/ElectronPFCombIsoPtRelEndcap',
			title = "Endcap electron comb. PF rel. iso. (final event sample)",
			xAxis = 'unlabeled',
			name = "finalSamplePlots_electronCombPFRelIsoEE"
			),
		drawJobConfigEntry(
			meName = 'ElectronQuantities/ElectronConversionRadius',
			title = "Vertex radius of best conv. pair (final event sample)",
			xAxis = 'posX',
			name = "finalSamplePlots_electronConvRadius"
			),
		drawJobConfigEntry(
			meName = 'ElectronQuantities/ElectronConversionDoca',
			title = "DOCA of best conv. pair (final event sample)",
			xAxis = 'posX',
			name = "finalSamplePlots_electronConvDOCA"
			),
		drawJobConfigEntry(
			meName = 'ElectronQuantities/ElectronConversionDeltaCotTheta',
			title = "#DeltaCot(#theta) of best conv. pair (final event sample)",
			xAxis = 'unlabeled',
			name = "finalSamplePlots_electronConvDeltaCotTheta"
			),
		drawJobConfigEntry(
			meName = 'ElectronQuantities/ElectronMissingExpInnerHits',
			title = "Electron missing inner hits (final event sample)",
			xAxis = 'unlabeled',
			name = "finalSamplePlots_electronMissingInnerHits"
			),
		drawJobConfigEntry(
			meName = 'TauQuantities/Tau#PAR#',
			PAR = [ 'Pt', 'Eta', 'Phi' ],
			title = "Tau (final event sample)",
			xAxis = '#PAR#',
			name = "finalSamplePlots_tau"
			),
		drawJobConfigEntry(
			meName = 'TauQuantities/TauMatchingGenParticlePdgId',
			title = "PdgId of gen. Particle matching Tau (final event sample)",
			xAxis = 'PdgId',
			name = "finalSamplePlots_pdgIdGenParticleMatchingTau"
			),
		drawJobConfigEntry(
			meName = 'TauQuantities/TauMatchingFinalStateGenParticlePdgId',
			title = "PdgId of final-state gen. Particle matching Tau (final event sample)",
			xAxis = 'PdgId',
			name = "finalSamplePlots_pdgIdFinalStateGenParticleMatchingTau"
			),
		drawJobConfigEntry(
				meName = 'TauQuantities/TauLeadTrkPt',
				title = "Tau lead. Track Pt (final event sample)",
				xAxis = 'Pt',
				name = "finalSamplePlots_tauLeadTrkPt"
				),
		drawJobConfigEntry(
				meName = 'TauQuantities/TauNumTracksSignalCone',
				title = "Tau Tracks in Signal Cone (final event sample)",
				xAxis = 'unlabeled',
				name = "finalSamplePlots_tauNumTracksSignalCone"
				),
		drawJobConfigEntry(
				meName = 'TauQuantities/TauJetRadius',
				title = "Tau Jet Radius (final event sample)",
				xAxis = 'unlabeled',
				name = "finalSamplePlots_tauJetRadius"
				),
		drawJobConfigEntry(
				meName = 'TauQuantities/TauDiscriminatorHPSloose',
				title = "HPS loose output (final event sample)",
				xAxis = 'unlabeled',
				name = "finalSamplePlots_tauDiscrHPSloose"
				),
		drawJobConfigEntry(
				meName = 'DiTauCandidateQuantities/DPhi12',
				title = "#Delta#phi(Electron-Tau) (final event sample)",
				xAxis = 'dPhi',
				name = "finalSamplePlots_dPhiElectronTau"
				),
		drawJobConfigEntry(
				meName = 'CaloMEtQuantities/MEtPt',
				title = "CaloMET (final Event sample)",
				xAxis = 'Pt',
				name = "finalSamplePlots_caloMEt"
				),  
		drawJobConfigEntry(
				meName = 'PFMEtQuantities/MEtPt',
				title = "PFMET (final Event sample)",
				xAxis = 'Pt',
				name = "finalSamplePlots_pfMEt"
				),
		drawJobConfigEntry(
				meName = 'DiTauCandidateQuantities/PzetaDiff',
				title = "P_{#zeta} - 1.5*P_{#zeta}^{vis} (final event sample)",
				xAxis = 'GeV',
				name = "finalSamplePlots_PzetaDiff"
				),
		drawJobConfigEntry(
				meName = 'DiTauCandidateQuantities/Mt1MET',
				title = "M_{T}(Electron + MET) (final event sample)",
				xAxis = 'Mt',
				name = "finalSamplePlots_mtElectronMET"
				),
		drawJobConfigEntry(
				meName = 'DiTauCandidateQuantities/Mt2MET',
				title = "M_{T}(Tau + MET) (final event sample)",
				xAxis = 'Mt',
				name = "finalSamplePlots_mtTauMET"
				),
		drawJobConfigEntry(
				meName = 'DiTauCandidateQuantities/Mt12MET',
				title = "M_{T}(Electron + Tau + MET) (final event sample)",
				xAxis = 'Mt',
				name = "finalSamplePlots_mtElectronTauMET"
				),
		drawJobConfigEntry(
				meName = 'DiTauCandidateQuantities/VisMass',
				title = "M_{vis}(Electron + Tau) (final event sample)",
				xAxis = 'Mass',
				name = "finalSamplePlots_mVisible"
				),
		drawJobConfigEntry(
				meName = 'DiTauCandidateQuantities/CDFmethodMass',
				title = "M(Electron + Tau), CDF method (final event sample)",
				xAxis = 'Mass',
				name = "finalSamplePlots_mCDFmethod"
				),
		drawJobConfigEntry(
				meName = 'DiTauCandidateQuantities/CollinearApproxMass',
				title = "M(Electron + Tau), collinear Approx. (final event sample)",
				xAxis = 'Mass',
				name = "finalSamplePlots_mCollApprox"
				),
		drawJobConfigEntry(
				meName = 'DiTauCandidateSVfitQuantities/psKine_MEt_ptBalance/Mass',
				title = "M(Electron + Tau), SVfit method (final event sample)",
				xAxis = 'Mass',
				name = "finalSamplePlots_mSVmethod"
				),
		drawJobConfigEntry(
				meName = 'DiTauCandidateQuantities/Ht12MET',
				title = "#Sigma H_{T}(Electron + Tau + MET) (final event sample)",
				xAxis = 'Mass',
				name = "finalSamplePlots_ht"
				)
		]



# Build draw job configurations for the opposite sign final event plots
drawJobConfigurator_ZtoElecTauOS = copy.deepcopy(drawJobConfigurator_ZtoElecTau)
drawJobConfigurator_ZtoElecTauOS.setDQMdirectory('#PROCESSDIR#/zElecTauAnalyzerOS/')

drawJobConfigurator_ZtoElecTauOS.add(
    afterCut = evtSelDiTauCandidateForElecTauZeroCharge,
    plots = finalSamplePlots
)


# Build draw job configurations for the same sign final event plots
drawJobConfigurator_ZtoElecTauSS = drawJobConfigurator(
    template = plots_ZtoElecTau,
    dqmDirectory = '#PROCESSDIR#/zElecTauAnalyzerSS/'
)

drawJobConfigurator_ZtoElecTauSS.add(
    afterCut = evtSelDiTauCandidateForElecTauNonZeroCharge,
    plots = finalSamplePlots
)

