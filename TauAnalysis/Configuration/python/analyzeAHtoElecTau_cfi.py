import FWCore.ParameterSet.Config as cms
import copy

#----------------------------------------------------------
# filter and analyzer configurations -- to be included in analysis sequence
#----------------------------------------------------------

firstAnalyzers = cms.PSet(
    analyzers = cms.vstring(
        'genPhaseSpaceEventInfoHistManager',
        'electronHistManager',
        'tauHistManager',
        'pfMEtHistManager',
        'vertexHistManager',
        'triggerHistManagerForElecTau'
    )
)
#
genAHtoElecTauCutFilter = cms.PSet(
    filter = cms.string('evtSelGenAHtoElecTau'),
    title = cms.string('gen. A/H -> e + tau'),
    saveRunLumiSectionEventNumbers = cms.vstring('')
)
genPhaseSpaceCutFilter = cms.PSet(
    filter = cms.string('genPhaseSpaceCut'),
    title = cms.string('gen. Phase-Space'),
    saveRunLumiSectionEventNumbers = cms.vstring('')
)

triggerAnalyzer = cms.PSet(
    analyzers = cms.vstring(
        'triggerHistManagerForElecTau'
    )
)
# trigger selection
triggerFilter = cms.PSet(
    filter = cms.string('evtSelTrigger'),
    title = cms.string('Trigger'),
    saveRunLumiSectionEventNumbers = cms.vstring('')
)

# data quality control
dataQualityFilter = cms.PSet(
    filter = cms.string('evtSelDataQuality'),
    title = cms.string('Data Quality'),
    saveRunLumiSectionEventNumbers = cms.vstring('')
)

primaryEventSelectionFilterAnalyzers = cms.VPSet(
    firstAnalyzers,
    genPhaseSpaceCutFilter,
    #genAHtoElecTauCutFilter,
    triggerAnalyzer,
    triggerFilter,
    dataQualityFilter
)
primaryEventSelectionFilters = cms.VPSet(
    genPhaseSpaceCutFilter,
    #genAHtoElecTauCutFilter,
    triggerFilter,
    dataQualityFilter
)

# primary event vertex selection
vertexQualityAnalyzer = cms.PSet(
    analyzers = cms.vstring(
        'vertexHistManager'
    ),
    replace = cms.vstring('vertexHistManager.vertexSource = offlinePrimaryVerticesWithBS')
)
vertexQualityFilter = cms.PSet(
    filter = cms.string('evtSelPrimaryEventVertexQuality'),
    title = cms.string('p(chi2Vertex) > 0.01'),
    saveRunLumiSectionEventNumbers = cms.vstring('')
)
vertexPositionAnalyzer = cms.PSet(
    analyzers = cms.vstring(
        'vertexHistManager'
    ),
    replace = cms.vstring('vertexHistManager.vertexSource = selectedPrimaryVertexQuality')
)
vertexPositionFilter = cms.PSet(
    filter = cms.string('evtSelPrimaryEventVertexPosition'),
    title = cms.string('-25 < zVertex < +25 cm'),
    saveRunLumiSectionEventNumbers = cms.vstring('')
)
vertexSelectionFilterAnalyzers = cms.VPSet(
    vertexQualityAnalyzer,
    vertexQualityFilter,
    vertexPositionAnalyzer,
    vertexPositionFilter
)
vertexSelectionFilters = cms.VPSet(
    vertexQualityFilter,
    vertexPositionFilter
)

# electron acceptance cuts
electronIdAnalyzer = cms.PSet(
    analyzers = cms.vstring('electronHistManager')
)
electronIdFilter = cms.PSet(
    filter = cms.string('evtSelElectronId'),
    title = cms.string('Electron ID'),
    saveRunLumiSectionEventNumbers = cms.vstring('')
)
electronEtaAnalyzer = cms.PSet(
    analyzers = cms.vstring('electronHistManager'),
    replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauIdCumulative')
)
electronAntiCrackFilter = cms.PSet(
    filter = cms.string('evtSelElectronAntiCrack'),
    title = cms.string('Electron crack-Veto'),
    saveRunLumiSectionEventNumbers = cms.vstring('')
)
electronEtaFilter = cms.PSet(
    filter = cms.string('evtSelElectronEta'),
    title = cms.string('-2.5 < eta(Electron) < +2.5'),
    saveRunLumiSectionEventNumbers = cms.vstring('')
)
electronPtAnalyzer = cms.PSet(
    analyzers = cms.vstring('electronHistManager'),
    replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauEtaCumulative')
)
electronPtFilter = cms.PSet(
    filter = cms.string('evtSelElectronPt'),
    title = cms.string('Pt(Electron) > 20 GeV'),
    saveRunLumiSectionEventNumbers = cms.vstring('')
)

electronSelectionFilterAnalyzers = cms.VPSet(
    electronIdAnalyzer,
    electronIdFilter,
    electronEtaAnalyzer,
    electronAntiCrackFilter,
    electronEtaFilter,
    electronPtAnalyzer,
    electronPtFilter
)
electronSelectionFilters = cms.VPSet(
    electronIdFilter,
    electronAntiCrackFilter,
    electronEtaFilter,
    electronPtFilter
)

# tau acceptance cuts
tauAntiOverlapFilter = cms.PSet(
    filter = cms.string('evtSelTauAntiOverlapWithElectronsVeto'),
    title = cms.string('Tau not overlapping with Elec.'),
    saveRunLumiSectionEventNumbers = cms.vstring('')
)
elecTauEtaAnalyzer = cms.PSet(
    analyzers = cms.vstring(
        'electronHistManager',
        'tauHistManager'
    ),
    replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauPtCumulative',
                          'tauHistManager.tauSource = selectedPatTausForElecTauAntiOverlapWithElectronsVetoCumulative')
)
tauEtaFilter = cms.PSet(
    filter = cms.string('evtSelTauEta'),
    title = cms.string('-2.3 < eta(Tau) < +2.3'),
    saveRunLumiSectionEventNumbers = cms.vstring('')
)
tauPtAnalyzer = cms.PSet(
    analyzers = cms.vstring(
        'tauHistManager'
    ),
    replace = cms.vstring('tauHistManager.tauSource = selectedPatTausForElecTauEtaCumulative')
)
tauPtFilter = cms.PSet(
    filter = cms.string('evtSelTauPt'),
    title = cms.string('Pt(Tau) > 20 GeV'),
    saveRunLumiSectionEventNumbers = cms.vstring('')
)
tauSelectionFilterAnalyzers = cms.VPSet(
    tauAntiOverlapFilter,
    elecTauEtaAnalyzer,
    tauEtaFilter,
    tauPtAnalyzer,
    tauPtFilter
)
tauSelectionFilters = cms.VPSet(
    tauAntiOverlapFilter,
    tauEtaFilter,
    tauPtFilter
)

# electron iso
tauElecIsoAnalyzer = cms.PSet(
    analyzers = cms.vstring(
        'electronHistManager',
        'tauHistManager'
    ),
    replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauPtCumulative',
                          'tauHistManager.tauSource = selectedPatTausForElecTauPtCumulative')
)
electronIsoFilter = cms.PSet(
    filter = cms.string('evtSelElectronIso'),
    title = cms.string('Electron PF iso. (rel.)'),
    saveRunLumiSectionEventNumbers = cms.vstring('')
)
electronConversionVetoAnalyzer = cms.PSet(
    analyzers = cms.vstring('electronHistManager'),
    replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauIsoCumulative')
)
electronConversionVetoFilter = cms.PSet(
    filter = cms.string('evtSelElectronConversionVeto'),
    title = cms.string('Electron Track conv. veto'),
    saveRunLumiSectionEventNumbers = cms.vstring('')
)
electronTrkIpAnalyzer = cms.PSet(
    analyzers = cms.vstring('electronHistManager'),
    replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauConversionVetoCumulative')
)
electronTrkIpFilter = cms.PSet(
    filter = cms.string('evtSelElectronTrkIP'),
    title = cms.string('Electron Track IP'),
    saveRunLumiSectionEventNumbers = cms.vstring('')
)
electronIsoFilterAnalyzers = cms.VPSet(
    tauElecIsoAnalyzer,
    electronIsoFilter,
    electronConversionVetoAnalyzer,
    electronConversionVetoFilter,
    electronTrkIpAnalyzer,
    electronTrkIpFilter
)
electronIsoFilters = cms.VPSet(
    electronIsoFilter,
    electronConversionVetoFilter,
    electronTrkIpFilter
)

# 
# selection of tau-jet candidate (id.)
# produced in hadronic tau decay
tauDecayModeFindingAnalyzer = cms.PSet(
    analyzers = cms.vstring('tauHistManager',
        'electronHistManager'),
    replace = cms.vstring('tauHistManager.tauSource = selectedPatTausForElecTauPtCumulative',
        'electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative')
)
tauDecayModeFindingFilter = cms.PSet(
    filter = cms.string('evtSelTauDecayModeFinding'),
    title = cms.string('Tau decay mode find.'),
    saveRunLumiSectionEventNumbers = cms.vstring('')
)
tauLeadTrkPtAnalyzer = cms.PSet(
    analyzers = cms.vstring('tauHistManager'),
    replace = cms.vstring('tauHistManager.tauSource = selectedPatTausForElecTauDecayModeFindingCumulative')
)
tauLeadTrkPtFilter = cms.PSet(
    filter = cms.string('evtSelTauLeadTrkPt'),
    title = cms.string('Tau decay mode finding'),
    saveRunLumiSectionEventNumbers = cms.vstring('')
)
tauIsoAnalyzer = cms.PSet(
    analyzers = cms.vstring('tauHistManager'),
    replace = cms.vstring('tauHistManager.tauSource = selectedPatTausForElecTauLeadTrkPtCumulative')
)
tauIsoFilter = cms.PSet(
    filter = cms.string('evtSelTauIso'),
    title = cms.string('Tau ID (HPS loose)'),
    saveRunLumiSectionEventNumbers = cms.vstring('')
)
tauElectronVetoAnalyzer = cms.PSet(
    analyzers = cms.vstring('tauHistManager'),
    replace = cms.vstring('tauHistManager.tauSource = selectedPatTausForElecTauIsoCumulative')
)
tauElectronVetoFilter = cms.PSet(
    filter = cms.string('evtSelTauElectronVeto'),
    title = cms.string('Tau e-Veto'),
    saveRunLumiSectionEventNumbers = cms.vstring('')
)
tauMuonVetoAnalyzer = cms.PSet(
    analyzers = cms.vstring('tauHistManager'),
    replace = cms.vstring('tauHistManager.tauSource = selectedPatTausForElecTauElectronVetoCumulative')
)
tauMuonVetoFilter = cms.PSet(
    filter = cms.string('evtSelTauMuonVeto'),
    title = cms.string('Tau mu-Veto'),
    saveRunLumiSectionEventNumbers = cms.vstring('')
)
elecTauPostSelectionAnalyzer = cms.PSet(
    analyzers = cms.vstring(
        'tauHistManager',
        'electronHistManager'
    ),
    replace = cms.vstring(
        'tauHistManager.tauSource = selectedPatTausForElecTauMuonVetoCumulative',
        'electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative'
    )
)
tauIdIsoFilterAnalyzers = cms.VPSet(
    tauDecayModeFindingAnalyzer,
    tauDecayModeFindingFilter,
    tauLeadTrkPtAnalyzer,
    tauLeadTrkPtFilter,
    tauIsoAnalyzer,
    tauIsoFilter,
    tauElectronVetoAnalyzer,
    tauElectronVetoFilter,
    tauMuonVetoAnalyzer,
    tauMuonVetoFilter,
    elecTauPostSelectionAnalyzer
)
tauIdIsoFilters = cms.VPSet(
    tauDecayModeFindingFilter,
    tauLeadTrkPtFilter,
    tauIsoFilter,
    tauElectronVetoFilter,
    tauMuonVetoFilter
)

#selection of electron + tau-jet combinations
mssmDiTauAntiOverlapFilter = cms.PSet(
    filter = cms.string('evtSelDiTauCandidateForAHtoElecTauAntiOverlapVeto'),
    title = cms.string('dR(Electron-Tau) > 0.3'),
    saveRunLumiSectionEventNumbers = cms.vstring('')
)
mssmDiTauAnalyzerAfterAntiOverlap = cms.PSet(
    analyzers = cms.vstring(
        'electronHistManager',
        'tauHistManager',
		'diTauCandidateHistManagerForElecTau',
        'pfMEtHistManager',
        'vertexHistManager'
    ),
    replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative',
                          'tauHistManager.tauSource = selectedPatTausForElecTauMuonVetoCumulative',
                          'diTauCandidateHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsForAHtoElecTauAntiOverlapVetoCumulative')
)
smDiTauAntiOverlapFilter = cms.PSet(
    filter = cms.string('evtSelDiTauCandidateForAHtoElecTauAntiOverlapVeto'),
    title = cms.string('dR(Electron-Tau) > 0.3'),
    saveRunLumiSectionEventNumbers = cms.vstring('')
)
smDiTauAnalyzerAfterAntiOverlap = cms.PSet(
    analyzers = cms.vstring(
        'electronHistManager',
        'tauHistManager',
		'diTauCandidateHistManagerForElecTau',
        'pfMEtHistManager',
        'vertexHistManager'
    ),
    replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative',
                          'tauHistManager.tauSource = selectedPatTausForElecTauMuonVetoCumulative',
                          'diTauCandidateHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsAntiOverlapVetoCumulative')
)
mtElecMetFilter = cms.PSet(
    filter = cms.string('evtSelDiTauCandidateForElecTauMt1MET'),
    title = cms.string('M_{T}(Electron-MET) < 40 GeV'),
    saveRunLumiSectionEventNumbers = cms.vstring('')
)
pzetaFilter = cms.PSet(
    filter = cms.string('evtSelDiTauCandidateForAHtoElecTauPzetaDiff'),
    title = cms.string('P_{#zeta} - 1.5*P_{#zeta}^{vis} > -20 GeV'),
    saveRunLumiSectionEventNumbers = cms.vstring('')
)
mssmDiTauAnalyzerAfterWcut = cms.PSet(
    analyzers = cms.vstring(
        'electronHistManager',
        'tauHistManager',
		'diTauCandidateHistManagerForElecTau',
        'pfMEtHistManager',
        'vertexHistManager',
        'diTauCandidateNSVfitHistManagerForElecTau',
        'elecPairHistManagerByLooseIsolation'
    ),
    replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative',
                          'tauHistManager.tauSource = selectedPatTausForElecTauMuonVetoCumulative',
                          'diTauCandidateHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsForAHtoElecTauPzetaDiffCumulative',
                          'diTauCandidateNSVfitHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsForAHtoElecTauPzetaDiffCumulative',
                          'elecPairHistManagerByLooseIsolation.diTauCandidateSource = allDiElecPairZeeHypothesesByLooseIsolation')
)
smDiTauAnalyzerAfterWcut = cms.PSet(
    analyzers = cms.vstring(
        'electronHistManager',
        'tauHistManager',
		'diTauCandidateHistManagerForElecTau',
        'pfMEtHistManager',
        'vertexHistManager',
        'diTauCandidateNSVfitHistManagerForElecTau',
        'elecPairHistManagerByLooseIsolation'
    ),
    replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative',
                          'tauHistManager.tauSource = selectedPatTausForElecTauMuonVetoCumulative',
                          'diTauCandidateHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsMt1METcumulative',
                          'diTauCandidateNSVfitHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsMt1METcumulative',
                          'elecPairHistManagerByLooseIsolation.diTauCandidateSource = allDiElecPairZeeHypothesesByLooseIsolation')
)
# veto events compatible with Z --> e+ e- hypothesis
# based on presence of a second, oppositely charged, loosely isolated, loosely ID-ed electron
zeeVetoFilter = cms.PSet(
    filter = cms.string('evtSelDiElecPairZeeHypothesisVetoByLooseIsolation'),
    title = cms.string('charge(isoelectron + looseIsoElectron) != 0'),
    saveRunLumiSectionEventNumbers = cms.vstring('')
)




# MSSM jet selection
mssmNumJetAnalyzer = cms.PSet( 
    analyzers = cms.vstring( 
        'electronHistManager', 
        'tauHistManager', 
        'diTauCandidateHistManagerForElecTau', 
        'diTauCandidateNSVfitHistManagerForElecTau', 
        'jetHistManager' 
    ), 
    replace = cms.vstring( 
        'electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative',
        'tauHistManager.tauSource = selectedPatTausForElecTauMuonVetoCumulative',
        'diTauCandidateHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsForAHtoElecTauPzetaDiffCumulative', 
        'diTauCandidateNSVfitHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsForAHtoElecTauPzetaDiffCumulative', 
        'jetHistManager.jetSource = selectedPatJetsForAHtoElecTauAntiOverlapWithLeptonsVetoCumulative'
    ) 
)
mssmNumJetFilter = cms.PSet(
	filter = cms.string('evtSelJetEtCut'),
	title = cms.string('N(jets with E_{T} > 30) < 2'),
	saveRunLumiSectionEventNumbers = cms.vstring('')
)
bTagAnalyzer = cms.PSet( 
    analyzers = cms.vstring( 
        'electronHistManager', 
        'tauHistManager', 
        'diTauCandidateHistManagerForElecTau', 
        'diTauCandidateNSVfitHistManagerForElecTau', 
        'jetHistManager' 
    ), 
    replace = cms.vstring( 
        'electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative',
        'tauHistManager.tauSource = selectedPatTausForElecTauMuonVetoCumulative',
        'diTauCandidateHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsForAHtoElecTauPzetaDiffCumulative', 
        'diTauCandidateNSVfitHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsForAHtoElecTauPzetaDiffCumulative', 
        'jetHistManager.jetSource = selectedPatJetsForAHtoElecTauJetTagCumulative'
    ) 
)
noBtagFilter = cms.PSet(
	filter = cms.string('evtSelBtagVeto'),
	title = cms.string('no E_{T} > 20 GeV jet with b-Tag'),
	saveRunLumiSectionEventNumbers = cms.vstring('')
)
bTagFilter = cms.PSet(
	filter = cms.string('evtSelBtagCut'),
	title = cms.string('E_{T} > 20 GeV jet with b-Tag'),
	saveRunLumiSectionEventNumbers = cms.vstring('')
)

bTagSelectionFilterAnalyzers = cms.VPSet(
    mssmDiTauAntiOverlapFilter,
    mssmDiTauAnalyzerAfterAntiOverlap,
    pzetaFilter,
    mssmDiTauAnalyzerAfterWcut,
    zeeVetoFilter,
    mssmNumJetAnalyzer,
    mssmNumJetFilter,
    bTagAnalyzer,
    bTagFilter
)
bTagSelectionFilters = cms.VPSet(
    mssmDiTauAntiOverlapFilter,
    pzetaFilter,
    zeeVetoFilter,
    mssmNumJetFilter,
    bTagFilter
)

noBtagSelectionFilterAnalyzers = cms.VPSet(
    mssmDiTauAntiOverlapFilter,
    mssmDiTauAnalyzerAfterAntiOverlap,
    pzetaFilter,
    mssmDiTauAnalyzerAfterWcut,
    zeeVetoFilter,
    mssmNumJetAnalyzer,
    mssmNumJetFilter,
    bTagAnalyzer,
    noBtagFilter
)
noBtagSelectionFilters = cms.VPSet(
    mssmDiTauAntiOverlapFilter,
    pzetaFilter,
    zeeVetoFilter,
    mssmNumJetFilter,
    noBtagFilter
)

# SM jet selection
smVBFjetEtAnalyzer = cms.PSet( 
    analyzers = cms.vstring( 
        'electronHistManager', 
        'tauHistManager', 
        'diTauCandidateHistManagerForElecTau', 
        'diTauCandidateNSVfitHistManagerForElecTau', 
        'vbfEventHistManagerForElecTau',
        'jetHistManager' 
    ), 
    replace = cms.vstring( 
        'electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative',
        'tauHistManager.tauSource = selectedPatTausForElecTauMuonVetoCumulative',
        'diTauCandidateHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsMt1METcumulative', 
        'diTauCandidateNSVfitHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsMt1METcumulative', 
        'vbfEventHistManagerForElecTau.vbfEventSource = allVBFEventHypothesesForAHtoElecTau',
        'jetHistManager.jetSource = selectedPatJetsForAHtoElecTauJetTagCumulative'
    ) 
)
smVBFjetEtFilter = cms.PSet(
    filter = cms.string('evtSelVBFtag'),
    title = cms.string('VBF: 2 jets with Et>30 GeV '),
    saveRunLumiSectionEventNumbers = cms.vstring('')
)
smVBFhemisphereAnalyzer = cms.PSet( 
    analyzers = cms.vstring( 
        'electronHistManager', 
        'tauHistManager', 
        'diTauCandidateHistManagerForElecTau', 
        'diTauCandidateNSVfitHistManagerForElecTau', 
        'vbfEventHistManagerForElecTau',
        'jetHistManager' 
    ), 
    replace = cms.vstring( 
        'electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative',
        'tauHistManager.tauSource = selectedPatTausForElecTauMuonVetoCumulative',
        'diTauCandidateHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsMt1METcumulative', 
        'diTauCandidateNSVfitHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsMt1METcumulative', 
        'vbfEventHistManagerForElecTau.vbfEventSource = selectedVBFEventHypothesesForAHtoElecTauTagJetEt30Cumulative',
        'jetHistManager.jetSource = selectedPatJetsForAHtoElecTauJetTagCumulative'
    ) 
)
smVBFhemisphereFilter = cms.PSet(
    filter = cms.string('evtSelVBFtag'),
    title = cms.string('VBF: opposite hemisphere jet pair'),
    saveRunLumiSectionEventNumbers = cms.vstring('')
)
smVBFdEta35Analyzer = cms.PSet( 
    analyzers = cms.vstring( 
        'electronHistManager', 
        'tauHistManager', 
        'diTauCandidateHistManagerForElecTau', 
        'diTauCandidateNSVfitHistManagerForElecTau', 
        'vbfEventHistManagerForElecTau',
        'jetHistManager' 
    ), 
    replace = cms.vstring( 
        'electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative',
        'tauHistManager.tauSource = selectedPatTausForElecTauMuonVetoCumulative',
        'diTauCandidateHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsMt1METcumulative', 
        'diTauCandidateNSVfitHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsMt1METcumulative', 
        'vbfEventHistManagerForElecTau.vbfEventSource = selectedVBFEventHypothesesForAHtoElecTauTagJetOpposHemisphereCumulative',
        'jetHistManager.jetSource = selectedPatJetsForAHtoElecTauJetTagCumulative'
    ) 
)
smVBFdEta35Filter = cms.PSet(
    filter = cms.string('evtSelVBFdEta35'),
    title = cms.string('dEta(tagJet1,tagJet2) > 3.5'),
    saveRunLumiSectionEventNumbers = cms.vstring('')
)
smVBFmassAnalyzer = cms.PSet( 
    analyzers = cms.vstring( 
        'electronHistManager', 
        'tauHistManager', 
        'diTauCandidateHistManagerForElecTau', 
        'diTauCandidateNSVfitHistManagerForElecTau', 
        'vbfEventHistManagerForElecTau',
        'jetHistManager' 
    ), 
    replace = cms.vstring( 
        'electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative',
        'tauHistManager.tauSource = selectedPatTausForElecTauMuonVetoCumulative',
        'diTauCandidateHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsMt1METcumulative', 
        'diTauCandidateNSVfitHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsMt1METcumulative', 
        'vbfEventHistManagerForElecTau.vbfEventSource = selectedVBFEventHypothesesForAHtoElecTauTagJetDEta35Cumulative',
        'jetHistManager.jetSource = selectedPatJetsForAHtoElecTauJetTagCumulative'
    ) 
)
smVBFmass350Filter = cms.PSet(
    filter = cms.string('evtSelVBFmass350'),
    title = cms.string('M(tagJet1,tagJet2) > 350 GeV'),
    saveRunLumiSectionEventNumbers = cms.vstring('')
)
smVBF3rdTagJetAnalyzer = cms.PSet( 
    analyzers = cms.vstring( 
        'electronHistManager', 
        'tauHistManager', 
        'diTauCandidateHistManagerForElecTau', 
        'diTauCandidateNSVfitHistManagerForElecTau', 
        'vbfEventHistManagerForElecTau',
        'jetHistManager' 
    ), 
    replace = cms.vstring( 
        'electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative',
        'tauHistManager.tauSource = selectedPatTausForElecTauMuonVetoCumulative',
        'diTauCandidateHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsMt1METcumulative', 
        'diTauCandidateNSVfitHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsMt1METcumulative', 
        'vbfEventHistManagerForElecTau.vbfEventSource = selectedVBFEventHypothesesForAHtoElecTauTagJetMass350Cumulative',
        'jetHistManager.jetSource = selectedPatJetsForAHtoElecTauJetTagCumulative'
    ) 
)
sm3rdTagJetVetoFilter = cms.PSet(
    filter = cms.string('evtSel3rdTagJetVeto'),
    title = cms.string('no 3rd E_{T} > 30 GeV Jet'),
    saveRunLumiSectionEventNumbers = cms.vstring('')
)
smVBFfinalAnalyzer = cms.PSet( 
    analyzers = cms.vstring( 
        'electronHistManager', 
        'tauHistManager', 
        'diTauCandidateHistManagerForElecTau', 
        'diTauCandidateNSVfitHistManagerForElecTau', 
        'vbfEventHistManagerForElecTau',
        'jetHistManager' 
    ), 
    replace = cms.vstring( 
        'electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative',
        'tauHistManager.tauSource = selectedPatTausForElecTauMuonVetoCumulative',
        'diTauCandidateHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsMt1METcumulative', 
        'diTauCandidateNSVfitHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsMt1METcumulative', 
        'vbfEventHistManagerForElecTau.vbfEventSource = selectedVBFEventHypothesesForAHtoElecTau3rdJetVetoCumulative',
        'jetHistManager.jetSource = selectedPatJetsForAHtoElecTauJetTagCumulative'
    ) 
)

smVBFselectionFilterAnalyzers = cms.VPSet(
    smDiTauAntiOverlapFilter,
    smDiTauAnalyzerAfterAntiOverlap,
    mtElecMetFilter,
    smDiTauAnalyzerAfterWcut,
    zeeVetoFilter,
    smVBFjetEtAnalyzer,
    smVBFjetEtFilter,
    smVBFhemisphereAnalyzer,
    smVBFhemisphereFilter,
    smVBFdEta35Analyzer,
    smVBFdEta35Filter,
    smVBFmassAnalyzer,
    smVBFmass350Filter,
    smVBF3rdTagJetAnalyzer,
    sm3rdTagJetVetoFilter,
    smVBFfinalAnalyzer
)
smVBFselectionFilters = cms.VPSet(
    smDiTauAntiOverlapFilter,
    mtElecMetFilter,
    zeeVetoFilter,
    smVBFjetEtFilter,
    smVBFhemisphereFilter,
    smVBFdEta35Filter,
    smVBFmass350Filter,
    sm3rdTagJetVetoFilter
)

# same/opposite sign tau pair selection
smOppositeSignFilter = cms.PSet(
    filter = cms.string('evtSelDiTauCandidateForElecTauZeroCharge'),
    title = cms.string('Charge(Electron+Tau) = 0'),
    saveRunLumiSectionEventNumbers = cms.vstring('passed_cumulative')
)
smOppositeSignAnalyzer = cms.PSet(
    analyzers = cms.vstring(
        'genPhaseSpaceEventInfoHistManager',
        'electronHistManager',
        'tauHistManager',
        'jetHistManager',
        'vertexHistManager',
        'vbfEventHistManagerForElecTau',
        'diTauCandidateHistManagerForElecTau',
        'diTauCandidateNSVfitHistManagerForElecTau',
        'pfMEtHistManager',
        'particleMultiplicityHistManager',
        'triggerHistManagerForElecTau',
		'sysUncertaintyHistManagerForElecTau',
		'dataBinner',
		'sysUncertaintyBinnerForElecTauEff'
    ),
	replace = cms.vstring(
		'electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative',
		'tauHistManager.tauSource = selectedPatTausForElecTauMuonVetoCumulative',
        'jetHistManager.jetSource = selectedPatJetsForAHtoElecTauJetTagCumulative',
        'vertexHistManager.vertexSource = selectedPrimaryVertexHighestPtTrackSum',
        'vbfEventHistManagerForElecTau.vbfEventSource = selectedVBFEventHypothesesForAHtoElecTau3rdJetVetoCumulative',
        'diTauCandidateNSVfitHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsZeroChargeCumulative',
		'diTauCandidateHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsZeroChargeCumulative'
	)
)
smSameSignFilter = cms.PSet(
    filter = cms.string('evtSelDiTauCandidateForElecTauNonZeroCharge'),
    title = cms.string('Charge(Muon+Tau) != 0'),
    saveRunLumiSectionEventNumbers = cms.vstring('passed_cumulative')
)
smSameSignAnalyzer = cms.PSet(
    analyzers = cms.vstring(
        'genPhaseSpaceEventInfoHistManager',
        'electronHistManager',
        'tauHistManager',
        'jetHistManager',
        'vertexHistManager',
        'vbfEventHistManagerForElecTau',
        'pfMEtHistManager',
        'diTauCandidateHistManagerForElecTau',
        'diTauCandidateNSVfitHistManagerForElecTau'
    ),
	replace = cms.vstring(
		'electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative',
		'tauHistManager.tauSource = selectedPatTausForElecTauMuonVetoCumulative',
        'jetHistManager.jetSource = selectedPatJetsForAHtoElecTauJetTagCumulative',
        'vertexHistManager.vertexSource = selectedPrimaryVertexHighestPtTrackSum',
        'vbfEventHistManagerForElecTau.vbfEventSource = selectedVBFEventHypothesesForAHtoElecTau3rdJetVetoCumulative',
        'diTauCandidateNSVfitHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsNonZeroChargeCumulative',
		'diTauCandidateHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsNonZeroChargeCumulative'
	)
)

mssmOppositeSignFilter = cms.PSet(
    filter = cms.string('evtSelDiTauCandidateForAHtoElecTauZeroCharge'),
    title = cms.string('Charge(Electron+Tau) = 0'),
    saveRunLumiSectionEventNumbers = cms.vstring('passed_cumulative')
)
mssmOppositeSignAnalyzer_wBtag = cms.PSet(
    analyzers = cms.vstring(
        'genPhaseSpaceEventInfoHistManager',
        'electronHistManager',
        'tauHistManager',
        'jetHistManager',
        'vertexHistManager',
        'diTauCandidateHistManagerForElecTau',
        'diTauCandidateNSVfitHistManagerForElecTau',
        'pfMEtHistManager',
        'particleMultiplicityHistManager',
        'triggerHistManagerForElecTau',
		'sysUncertaintyHistManagerForElecTau',
		'dataBinner',
		'sysUncertaintyBinnerForElecTauEff'
    ),
	replace = cms.vstring(
		'electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative',
		'tauHistManager.tauSource = selectedPatTausForElecTauMuonVetoCumulative',
        'jetHistManager.jetSource = selectedPatJetsForAHtoElecTauBtagCumulative',
        'vertexHistManager.vertexSource = selectedPrimaryVertexHighestPtTrackSum',
        'diTauCandidateNSVfitHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsForAHtoElecTauZeroChargeCumulative',
		'diTauCandidateHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsForAHtoElecTauZeroChargeCumulative'
	)
)
mssmOppositeSignAnalyzer_woBtag = cms.PSet(
    analyzers = cms.vstring(
        'genPhaseSpaceEventInfoHistManager',
        'electronHistManager',
        'tauHistManager',
        'jetHistManager',
        'vertexHistManager',
        'diTauCandidateHistManagerForElecTau',
        'diTauCandidateNSVfitHistManagerForElecTau',
        'pfMEtHistManager',
        'particleMultiplicityHistManager',
        'triggerHistManagerForElecTau',
		'sysUncertaintyHistManagerForElecTau',
		'dataBinner',
		'sysUncertaintyBinnerForElecTauEff'
    ),
	replace = cms.vstring(
		'electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative',
		'tauHistManager.tauSource = selectedPatTausForElecTauMuonVetoCumulative',
        'jetHistManager.jetSource = selectedPatJetsForAHtoElecTauJetTagCumulative',
        'vertexHistManager.vertexSource = selectedPrimaryVertexHighestPtTrackSum',
        'diTauCandidateNSVfitHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsForAHtoElecTauZeroChargeCumulative',
		'diTauCandidateHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsForAHtoElecTauZeroChargeCumulative'
	)
)
mssmSameSignFilter = cms.PSet(
    filter = cms.string('evtSelDiTauCandidateForAHtoElecTauNonZeroCharge'),
    title = cms.string('Charge(Muon+Tau) != 0'),
    saveRunLumiSectionEventNumbers = cms.vstring('passed_cumulative')
)
mssmSameSignAnalyzer_woBtag = cms.PSet(
    analyzers = cms.vstring(
        'genPhaseSpaceEventInfoHistManager',
        'electronHistManager',
        'tauHistManager',
        'vertexHistManager',
        'pfMEtHistManager',
        'diTauCandidateHistManagerForElecTau',
        'diTauCandidateNSVfitHistManagerForElecTau'
    ),
	replace = cms.vstring(
		'electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative',
		'tauHistManager.tauSource = selectedPatTausForElecTauMuonVetoCumulative',
        'jetHistManager.jetSource = selectedPatJetsForAHtoElecTauJetTagCumulative',
        'vertexHistManager.vertexSource = selectedPrimaryVertexHighestPtTrackSum',
        'diTauCandidateNSVfitHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsForAHtoElecTauNonZeroChargeCumulative',
		'diTauCandidateHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsForAHtoElecTauNonZeroChargeCumulative'
	)
)
mssmSameSignAnalyzer_wBtag = cms.PSet(
    analyzers = cms.vstring(
        'genPhaseSpaceEventInfoHistManager',
        'electronHistManager',
        'tauHistManager',
        'vertexHistManager',
        'pfMEtHistManager',
        'diTauCandidateHistManagerForElecTau',
        'diTauCandidateNSVfitHistManagerForElecTau'
    ),
	replace = cms.vstring(
		'electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative',
		'tauHistManager.tauSource = selectedPatTausForElecTauMuonVetoCumulative',
        'jetHistManager.jetSource = selectedPatJetsForAHtoElecTauBtagCumulative',
        'vertexHistManager.vertexSource = selectedPrimaryVertexHighestPtTrackSum',
        'diTauCandidateNSVfitHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsForAHtoElecTauNonZeroChargeCumulative',
		'diTauCandidateHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsForAHtoElecTauNonZeroChargeCumulative'
	)
)


smOppositeSignFilterAnalyzers = cms.VPSet(smOppositeSignFilter,smOppositeSignAnalyzer)
smSameSignFilterAnalyzers = cms.VPSet(smSameSignFilter,smSameSignAnalyzer)
mssmOppositeSignFilterAnalyzers_woBtag = cms.VPSet(mssmOppositeSignFilter,mssmOppositeSignAnalyzer_woBtag)
mssmOppositeSignFilterAnalyzers_wBtag = cms.VPSet(mssmOppositeSignFilter,mssmOppositeSignAnalyzer_wBtag)
mssmSameSignFilterAnalyzers_woBtag = cms.VPSet(mssmSameSignFilter,mssmSameSignAnalyzer_woBtag)
mssmSameSignFilterAnalyzers_wBtag = cms.VPSet(mssmSameSignFilter,mssmSameSignAnalyzer_wBtag)




