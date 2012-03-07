import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.CandidateTools.sysErrDefinitions_cfi import *

#--------------------------------------------------------------------------------
# define event selection criteria 
#--------------------------------------------------------------------------------

# generator level phase-space selection
# (NOTE: to be used in case of Monte Carlo samples
#        overlapping in simulated phase-space only !!)
evtSelGenPhaseSpace = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('genPhaseSpaceCut')
)
evtSelGenAHtoElecTau = cms.PSet(
    pluginName = cms.string('genAHtoElecTauCut'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('isGenAHtoElecTau'),
)

# trigger selection
evtSelTrigger = cms.PSet(
    pluginName = cms.string('evtSelTrigger'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('Trigger')
)
# data-quality selection
evtSelDataQuality = cms.PSet(
	pluginName = cms.string('evtSelDataQuality'),
	pluginType = cms.string('BoolEventSelector'),
	src = cms.InputTag('dataQualityCutsPassed'),
	failSilent = cms.bool(True)
)

# primary event vertex selection
evtSelPrimaryEventVertexQuality = cms.PSet(
    pluginName = cms.string('evtSelPrimaryEventVertexQuality'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('primaryEventVertexQuality')
)
evtSelPrimaryEventVertexPosition = cms.PSet(
    pluginName = cms.string('evtSelPrimaryEventVertexPosition'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('primaryEventVertexPosition')
)

# electron acceptance cuts
evtSelElectronId = cms.PSet(
    pluginName = cms.string('evtSelElectronId'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('electronIdCut', 'cumulative'),
    src_individual = cms.InputTag('electronIdCut', 'individual'),
	systematics = cms.vstring(electronSystematics.keys())
)
evtSelElectronAntiCrack = cms.PSet(
    pluginName = cms.string('evtSelElectronAntiCrack'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('electronAntiCrackCut', 'cumulative'),
    src_individual = cms.InputTag('electronAntiCrackCut', 'individual'),
	systematics = cms.vstring(electronSystematics.keys())
)
evtSelElectronEta = cms.PSet(
    pluginName = cms.string('evtSelElectronEta'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('electronEtaCut', 'cumulative'),
    src_individual = cms.InputTag('electronEtaCut', 'individual'),
	systematics = cms.vstring(electronSystematics.keys())
)
evtSelElectronPt = cms.PSet(
    pluginName = cms.string('evtSelElectronPt'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('electronPtCut', 'cumulative'),
    src_individual = cms.InputTag('electronPtCut', 'individual'),
	systematics = cms.vstring(electronSystematics.keys())
)

# tau acceptance cuts
evtSelTauAntiOverlapWithElectronsVeto = cms.PSet(
    pluginName = cms.string('evtSelTauAntiOverlapWithElectronsVeto'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('tauAntiOverlapWithElectronsVeto', 'cumulative'),
    src_individual = cms.InputTag('tauAntiOverlapWithElectronsVeto', 'individual'),
	systematics = cms.vstring(tauSystematics.keys())
)
evtSelTauEta = cms.PSet(
    pluginName = cms.string('evtSelTauEta'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('tauEtaCut', 'cumulative'),
    src_individual = cms.InputTag('tauEtaCut', 'individual'),
	systematics = cms.vstring(tauSystematics.keys())
)
evtSelTauPt = cms.PSet(
    pluginName = cms.string('evtSelTauPt'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('tauPtCut', 'cumulative'),
    src_individual = cms.InputTag('tauPtCut', 'individual'),
	systematics = cms.vstring(tauSystematics.keys())
)

evtSelElectronIso = cms.PSet(
    pluginName = cms.string('evtSelElectronIso'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('electronIsoCut', 'cumulative'),
    src_individual = cms.InputTag('electronIsoCut', 'individual'),
	systematics = cms.vstring(electronSystematics.keys())
)
evtSelElectronConversionVeto = cms.PSet(
    pluginName = cms.string('evtSelElectronConversionVeto'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('electronConversionVeto', 'cumulative'),
    src_individual = cms.InputTag('electronConversionVeto', 'individual'),
	systematics = cms.vstring(electronSystematics.keys())
)
evtSelElectronTrkIP = cms.PSet(
    pluginName = cms.string('evtSelElectronTrkIP'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('electronTrkIPcut', 'cumulative'),
    src_individual = cms.InputTag('electronTrkIPcut', 'individual'),
	systematics = cms.vstring(electronSystematics.keys())
)

# tau candidate (id.) selection
evtSelTauDecayModeFinding = cms.PSet(
    pluginName = cms.string('evtSelTauDecayModeFinding'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('tauDecayModeFindingCut', 'cumulative'),
    src_individual = cms.InputTag('tauDecayModeFindingCut', 'individual'),
	systematics = cms.vstring(tauSystematics.keys())
)
evtSelTauLeadTrkPt = cms.PSet(
    pluginName = cms.string('evtSelTauLeadTrkPt'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('tauLeadTrkPtCut', 'cumulative'),
    src_individual = cms.InputTag('tauLeadTrkPtCut', 'individual'),
	systematics = cms.vstring(tauSystematics.keys())
)
evtSelTauIso = cms.PSet(
    pluginName = cms.string('evtSelTauIso'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('tauIsoCut', 'cumulative'),
    src_individual = cms.InputTag('tauIsoCut', 'individual'),
	systematics = cms.vstring(tauSystematics.keys())
)
evtSelTauElectronVeto = cms.PSet(
    pluginName = cms.string('evtSelTauElectronVeto'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('tauElectronVeto', 'cumulative'),
    src_individual = cms.InputTag('tauElectronVeto', 'individual'),
	systematics = cms.vstring(tauSystematics.keys())
)
evtSelTauMuonVeto = cms.PSet(
    pluginName = cms.string('evtSelTauMuonVeto'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('tauMuonVeto', 'cumulative'),
    src_individual = cms.InputTag('tauMuonVeto', 'individual'),
	systematics = cms.vstring(tauSystematics.keys())
)

# veto events compatible with Z --> e+ e- hypothesis
evtSelDiElecPairZeeHypothesisVetoByLooseIsolation = cms.PSet(
    pluginName = cms.string('evtSelDiElecPairZeeHypothesisVetoByLooseIsolation'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('diElecPairZeeHypothesisVetoByLooseIsolation')
)

# jet Et cut (shared by SM/MSSM categories)
evtSelJetEtCut = cms.PSet(
    pluginName = cms.string('evtSelJetEtCut'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('jetEtCut', 'cumulative'),
    src_individual = cms.InputTag('jetEtCut', 'individual'),
    systematics = cms.vstring(jetSystematics.keys())
)

#------------------------------------------------------------
# di-tau candidate selection for SM Higgs
#------------------------------------------------------------
evtSelDiTauCandidateForElecTauAntiOverlapVeto = cms.PSet(
    pluginName = cms.string('evtSelDiTauCandidateForElecTauAntiOverlapVeto'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('diTauCandidateForElecTauAntiOverlapVeto', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForElecTauAntiOverlapVeto', 'individual'),
	systematics = cms.vstring(elecTauPairSystematics.keys())
)
evtSelDiTauCandidateForElecTauMt1MET = cms.PSet(
    pluginName = cms.string('evtSelDiTauCandidateForElecTauMt1MET'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('diTauCandidateForElecTauMt1METCut', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForElecTauMt1METCut', 'individual'),
	systematics = cms.vstring(elecTauPairSystematics.keys())
)

#  final selection for opposite-sign (OS) and same-sign (SS) di-tau pairs
evtSelDiTauCandidateForElecTauZeroCharge = cms.PSet(
    pluginName = cms.string('evtSelDiTauCandidateForElecTauZeroCharge'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('diTauCandidateForElecTauZeroChargeCut', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForElecTauZeroChargeCut', 'individual'),
	systematics = cms.vstring(elecTauPairSystematics.keys())
)
evtSelDiTauCandidateForElecTauNonZeroCharge = cms.PSet(
    pluginName = cms.string('evtSelDiTauCandidateForElecTauNonZeroCharge'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('diTauCandidateForElecTauNonZeroChargeCut', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForElecTauNonZeroChargeCut', 'individual'),
	systematics = cms.vstring(elecTauPairSystematics.keys())
)

# 0/1 jet selection
evtSelBoostedJetVeto = cms.PSet(
    pluginName = cms.string('evtSelBoostedJetVeto'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('boostedJetVeto', 'cumulative'),
    src_individual = cms.InputTag('boostedJetVeto', 'individual'),
    systematics = cms.vstring(jetSystematics.keys())
)

# boosted jet selection
evtSelBoostedJetTag = cms.PSet(
    pluginName = cms.string('evtSelBoostedJetTag'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('boostedJetTag', 'cumulative'),
    src_individual = cms.InputTag('boostedJetTag', 'individual'),
    systematics = cms.vstring(jetSystematics.keys())
)

# VBF jet selection
evtSelVBFtag = cms.PSet(
    pluginName = cms.string('evtSelVBFtag'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('vbfTagCut', 'cumulative'),
    src_individual = cms.InputTag('vbfTagCut', 'individual'),
)
evtSelVBFdEta35 = cms.PSet(
    pluginName = cms.string('evtSelVBFdEta35'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('vbfDEta35Cut', 'cumulative'),
    src_individual = cms.InputTag('vbfDEta35Cut', 'individual'),
)
evtSelVBFmass350 = cms.PSet(
    pluginName = cms.string('evtSelVBFmass350'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('vbfMass350Cut', 'cumulative'),
    src_individual = cms.InputTag('vbfMass350Cut', 'individual'),
)
evtSel3rdTagJetVeto = cms.PSet(
    pluginName = cms.string('evtSel3rdTagJetVeto'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('vbf3rdTagJetVeto', 'cumulative'),
    src_individual = cms.InputTag('vbf3rdTagJetVeto', 'individual'),
)

#------------------------------------------------------------
# di-tau candidate selection for MSSM Higgs
#------------------------------------------------------------
evtSelDiTauCandidateForAHtoElecTauAntiOverlapVeto = evtSelDiTauCandidateForElecTauAntiOverlapVeto.clone(
    pluginName = cms.string('evtSelDiTauCandidateForAHtoElecTauAntiOverlapVeto'),
    src_cumulative = cms.InputTag('diTauCandidateForAHtoElecTauAntiOverlapVeto', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForAHtoElecTauAntiOverlapVeto', 'individual')
)
evtSelDiTauCandidateForAHtoElecTauPzetaDiff = cms.PSet(
    pluginType = cms.string('BoolEventSelector'),
    pluginName = cms.string('evtSelDiTauCandidateForAHtoElecTauPzetaDiff'),
    src_cumulative = cms.InputTag('diTauCandidateForAHtoElecTauPzetaDiffCut', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForAHtoElecTauPzetaDiffCut', 'individual')
)

# opposite-sign pairs
evtSelDiTauCandidateForAHtoElecTauZeroCharge = evtSelDiTauCandidateForElecTauZeroCharge.clone(
    pluginName = cms.string('evtSelDiTauCandidateForAHtoElecTauZeroCharge'),
    src_cumulative = cms.InputTag('diTauCandidateForAHtoElecTauZeroChargeCut', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForAHtoElecTauZeroChargeCut', 'individual')
)
# same-sign pairs
evtSelDiTauCandidateForAHtoElecTauNonZeroCharge = evtSelDiTauCandidateForElecTauNonZeroCharge.clone(
    pluginName = cms.string('evtSelDiTauCandidateForAHtoElecTauNonZeroCharge'),
    src_cumulative = cms.InputTag('diTauCandidateForAHtoElecTauNonZeroChargeCut', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForAHtoElecTauNonZeroChargeCut', 'individual')
)

#  jet veto/b-jet candidate selection
evtSelBtagVeto = cms.PSet(
    pluginName = cms.string('evtSelBtagVeto'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('jetBtagVeto', 'cumulative'),
    src_individual = cms.InputTag('jetBtagVeto', 'individual'),
    systematics = cms.vstring(jetSystematics.keys())
)
evtSelBtagCut = cms.PSet(
    pluginName = cms.string('evtSelBtagCut'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('jetBtagCut', 'cumulative'),
    src_individual = cms.InputTag('jetBtagCut', 'individual'),
    systematics = cms.vstring(jetSystematics.keys())
)



