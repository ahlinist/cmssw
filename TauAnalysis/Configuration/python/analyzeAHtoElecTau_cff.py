import FWCore.ParameterSet.Config as cms
import copy

# import config for event selection, event print-out and analysis sequence
from TauAnalysis.Configuration.analyzeAHtoElecTau_cfi import *

# define auxiliary service
# for handling of systematic uncertainties
#
# NOTE: only systematic uncertainties handled via weights
#      (not the ones handled via shifting/smearing pat::Objects and reapplying cuts)
#       must be passed to SysUncertaintyService !!
#

from TauAnalysis.CandidateTools.sysErrDefinitions_cfi import *
SysUncertaintyService = cms.Service("SysUncertaintyService",
    weights = getSysUncertaintyParameterSets(
		[ theorySystematics ]
    ),
	sources = cms.PSet(
		isRecAHtoElecTau = cms.vstring(
			"sysElectron*", "",
            "sysTau*", "",
			"sysZllRecoilCorrection*", "",
			"sysJet*", ""
		),
		isRecAHtoElecTauBtag = cms.vstring(
			"sysElectron*", "",
            "sysTau*", "",
			"sysZllRecoilCorrection*", "",
			"sysJet*", ""
		),
		isRecAHtoElecTauBtagVeto = cms.vstring(
			"sysElectron*", "",
            "sysTau*", "",
			"sysZllRecoilCorrection*", "",
			"sysJet*", ""
		)
	)
)

# import filter definitions
from TauAnalysis.Configuration.eventSelectionForAHtoElecTau_cfi import *
filtersForElecTau = cms.VPSet(
        # generator level phase-space selection
        evtSelGenPhaseSpace,
        evtSelGenAHtoElecTau,

        # trigger selection
        evtSelTrigger,

        # data quality (scraping beam veto, etc)
        evtSelDataQuality,

        # primary event vertex selection
        evtSelPrimaryEventVertexQuality,
        evtSelPrimaryEventVertexPosition,

        # electron candidate selection
        evtSelElectronId,
        evtSelElectronAntiCrack,
        evtSelElectronEta,
        evtSelElectronPt,
        evtSelElectronIso,
        evtSelElectronConversionVeto,
        evtSelElectronTrkIP,

        # tau candidate selection
        evtSelTauAntiOverlapWithElectronsVeto,
        evtSelTauEta,
        evtSelTauPt,
        evtSelTauDecayModeFinding,
        evtSelTauLeadTrkPt,
        evtSelTauIso,
        evtSelTauElectronVeto,
        evtSelTauMuonVeto,

        # SM Higgs di-tau candidate selection
        evtSelDiTauCandidateForElecTauAntiOverlapVeto,
        evtSelDiTauCandidateForElecTauMt1MET,
        evtSelDiTauCandidateForElecTauZeroCharge,
        evtSelDiTauCandidateForElecTauNonZeroCharge,

        # MSSM Higgs di-tau candidate selection
        evtSelDiTauCandidateForAHtoElecTauAntiOverlapVeto,
        evtSelDiTauCandidateForAHtoElecTauPzetaDiff,
        evtSelDiTauCandidateForAHtoElecTauZeroCharge,
        evtSelDiTauCandidateForAHtoElecTauNonZeroCharge,

        # veto events compatible with Z --> e+ e- hypothesis
        # (based on the precense of an opposite-sign, loosely isolated electron	 
        evtSelDiElecPairZeeHypothesisVetoByLooseIsolation,

        # jet veto/b-jet candidate selection
        evtSelJetEtCut,
        evtSelBtagVeto,
        evtSelBtagCut,

        # SM jet selection
        evtSelBoostedJetTag,
        evtSelBoostedJetVeto,
        
        # VBF selection
        evtSelVBFtag,
        evtSelVBFdEta35,
        evtSelVBFmass350,
        evtSel3rdTagJetVeto
)

from TauAnalysis.Configuration.histManagersForZtoElecTau_cfi import *
elecTauAnalyzers = cms.VPSet(
        genPhaseSpaceEventInfoHistManager,
        eventWeightHistManager,
        electronHistManager,
        tauHistManager,
        diTauCandidateHistManagerForElecTau,
        diTauCandidateNSVfitHistManagerForElecTau,                                     
        diTauCandidateZeeHypothesisHistManagerForElecTau,
        diTauLeg1ChargeBinGridHistManager,
        vbfEventHistManagerForElecTau,
        elecPairHistManagerByLooseIsolation,
        jetHistManager,
        caloMEtHistManager,
        pfMEtHistManager,
        particleMultiplicityHistManager,
        vertexHistManager,
        triggerHistManagerForElecTau,
        dataBinner
)

# create base analysis sequences
elecTauAnalysisSequence = cms.VPSet()
elecTauAnalysisSequence.extend(primaryEventSelectionFilterAnalyzers)
elecTauAnalysisSequence.extend(vertexSelectionFilterAnalyzers)
elecTauAnalysisSequence.extend(electronSelectionFilterAnalyzers)
elecTauAnalysisSequence.extend(tauSelectionFilterAnalyzers)
elecTauAnalysisSequence.extend(electronIsoFilterAnalyzers)
elecTauAnalysisSequence.extend(tauIdIsoFilterAnalyzers)

elecTauAnalysisSequence_noPlots = cms.VPSet()
elecTauAnalysisSequence_noPlots.extend(primaryEventSelectionFilters)
elecTauAnalysisSequence_noPlots.extend(vertexSelectionFilters)
elecTauAnalysisSequence_noPlots.extend(electronSelectionFilters)
elecTauAnalysisSequence_noPlots.extend(tauSelectionFilters)
elecTauAnalysisSequence_noPlots.extend(electronIsoFilters)
elecTauAnalysisSequence_noPlots.extend(tauIdIsoFilters)

analyzeAHtoElecTauEvents = cms.EDAnalyzer("GenericAnalyzer",
  
    name = cms.string('ahElecTauAnalyzer'), 
                            
    filters = filtersForElecTau,

    analyzers = elecTauAnalyzers,

    analyzers_systematic = cms.VPSet(
		sysUncertaintyHistManagerForElecTau,
		sysUncertaintyBinnerForElecTauEff
	),                                     

    eventDumps = cms.VPSet(),
   
    analysisSequence = elecTauAnalysisSequence_noPlots,

    estimateSysUncertainties = cms.bool(False), 

    systematics = cms.vstring(
        getSysUncertaintyNames(
            [ electronSystematics,
              tauSystematics,
			  jetSystematics,
			  elecTauPairSystematics,
              theorySystematics ]
        )
    )                                         
)


# Analyzers for SM Higgs categories
# -- VBF
analyzeAHtoElecTauEventsOS_VBF = analyzeAHtoElecTauEvents.clone(
    name = cms.string('ahElecTauAnalyzerOS_VBF'),
    analysisSequence = elecTauAnalysisSequence
)
analyzeAHtoElecTauEventsOS_VBF.analysisSequence.extend(smVBFselectionFilterAnalyzers)
analyzeAHtoElecTauEventsOS_VBF.analysisSequence.extend(smOppositeSignFilterAnalyzers)

analyzeAHtoElecTauEventsSS_VBF = analyzeAHtoElecTauEvents.clone(
    name = cms.string('ahElecTauAnalyzerSS_VBF')
)
analyzeAHtoElecTauEventsSS_VBF.analysisSequence.extend(smVBFselectionFilters)
analyzeAHtoElecTauEventsSS_VBF.analysisSequence.extend(smSameSignFilterAnalyzers)

# -- 0/1 jets
analyzeAHtoElecTauEventsOS_ZeroOneJets = analyzeAHtoElecTauEvents.clone(
    name = cms.string('ahElecTauAnalyzerOS_ZeroOneJets')
)
analyzeAHtoElecTauEventsOS_ZeroOneJets.analysisSequence.extend(smZeroOneJetsSelectionFilterAnalyzers)
analyzeAHtoElecTauEventsOS_ZeroOneJets.analysisSequence.extend(smOppositeSignFilterAnalyzers)

analyzeAHtoElecTauEventsSS_ZeroOneJets = analyzeAHtoElecTauEvents.clone(
    name = cms.string('ahElecTauAnalyzerSS_ZeroOneJets')
)
analyzeAHtoElecTauEventsSS_ZeroOneJets.analysisSequence.extend(smZeroOneJetsSelectionFilters)
analyzeAHtoElecTauEventsSS_ZeroOneJets.analysisSequence.extend(smSameSignFilterAnalyzers)

# -- boosted Higgs
analyzeAHtoElecTauEventsOS_Boosted = analyzeAHtoElecTauEvents.clone(
    name = cms.string('ahElecTauAnalyzerOS_Boosted')
)
analyzeAHtoElecTauEventsOS_Boosted.analysisSequence.extend(smBoostedSelectionFilterAnalyzers)
analyzeAHtoElecTauEventsOS_Boosted.analysisSequence.extend(smOppositeSignFilterAnalyzers)

analyzeAHtoElecTauEventsSS_Boosted = analyzeAHtoElecTauEvents.clone(
    name = cms.string('ahElecTauAnalyzerSS_Boosted')
)
analyzeAHtoElecTauEventsSS_Boosted.analysisSequence.extend(smBoostedSelectionFilters)
analyzeAHtoElecTauEventsSS_Boosted.analysisSequence.extend(smSameSignFilterAnalyzers)

# Analyzers for MSSM Higgs categories
# -- b-tagged
analyzeAHtoElecTauEventsOS_wBtag = analyzeAHtoElecTauEvents.clone(
    name = cms.string('ahElecTauAnalyzerOS_wBtag')
)
analyzeAHtoElecTauEventsOS_wBtag.analysisSequence.extend(bTagSelectionFilterAnalyzers)
analyzeAHtoElecTauEventsOS_wBtag.analysisSequence.extend(mssmOppositeSignFilterAnalyzers_wBtag)

analyzeAHtoElecTauEventsSS_wBtag = analyzeAHtoElecTauEvents.clone(
    name = cms.string('ahElecTauAnalyzerSS_wBtag')
)
analyzeAHtoElecTauEventsSS_wBtag.analysisSequence.extend(bTagSelectionFilters)
analyzeAHtoElecTauEventsSS_wBtag.analysisSequence.extend(mssmSameSignFilterAnalyzers_wBtag)

# -- no b-tag
analyzeAHtoElecTauEventsOS_woBtag = analyzeAHtoElecTauEvents.clone(
    name = cms.string('ahElecTauAnalyzerOS_woBtag')
)
analyzeAHtoElecTauEventsOS_woBtag.analysisSequence.extend(noBtagSelectionFilterAnalyzers)
analyzeAHtoElecTauEventsOS_woBtag.analysisSequence.extend(mssmOppositeSignFilterAnalyzers_woBtag)

analyzeAHtoElecTauEventsSS_woBtag = analyzeAHtoElecTauEvents.clone(
    name = cms.string('ahElecTauAnalyzerSS_woBtag')
)
analyzeAHtoElecTauEventsSS_woBtag.analysisSequence.extend(noBtagSelectionFilters)
analyzeAHtoElecTauEventsSS_woBtag.analysisSequence.extend(mssmSameSignFilterAnalyzers_woBtag)

analyzeAHtoElecTauSequence = cms.Sequence(
    analyzeAHtoElecTauEventsOS_woBtag 
    + analyzeAHtoElecTauEventsOS_wBtag
    + analyzeAHtoElecTauEventsSS_woBtag 
    + analyzeAHtoElecTauEventsSS_wBtag
    + analyzeAHtoElecTauEventsOS_VBF
    + analyzeAHtoElecTauEventsSS_VBF
    + analyzeAHtoElecTauEventsOS_ZeroOneJets
    + analyzeAHtoElecTauEventsSS_ZeroOneJets
    + analyzeAHtoElecTauEventsSS_Boosted
    + analyzeAHtoElecTauEventsOS_Boosted
)

