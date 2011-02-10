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
		isRecAHtoElecTauCentralJetVeto = cms.vstring(
			"sysElectron*", "",
            "sysTau*", "",
			"sysZllRecoilCorrection*", "",
			"sysJet*", ""
		),
		isRecAHtoElecTauCentralJetBtag = cms.vstring(
			"sysElectron*", "",
            "sysTau*", "",
			"sysZllRecoilCorrection*", "",
			"sysJet*", ""
		)
	)
)

analyzeAHtoElecTauEventsOS_woBtag = cms.EDAnalyzer("GenericAnalyzer",
  
    name = cms.string('ahElecTauAnalyzerOS_woBtag'), 
                            
    filters = cms.VPSet(
        # generator level phase-space selection
        evtSelGenPhaseSpace,
    
        # trigger selection
        evtSelTrigger,
        
        # primary event vertex selection
        evtSelPrimaryEventVertex,
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
        evtSelTauLeadTrk,
        evtSelTauLeadTrkPt,
		evtSelTauTaNCdiscr,
        evtSelTauProng,
        evtSelTauCharge,
        evtSelTauElectronVeto,
        evtSelTauEcalCrackVeto,
        evtSelTauMuonVeto,
        
        # di-tau candidate selection
        evtSelDiTauCandidateForAHtoElecTauAntiOverlapVeto,
        evtSelDiTauCandidateForAHtoElecTauMt1MET,
        evtSelDiTauCandidateForAHtoElecTauPzetaDiff,
        evtSelDiTauCandidateForAHtoElecTauZeroCharge,
        evtSelDiTauCandidateForAHtoElecTauNonZeroCharge,

		# veto events compatible with Z --> e+ e- hypothesis
		# (based on the precense of an opposite-sign, loosely isolated electron	 
		evtSelDiElecPairZeeHypothesisVetoByLooseIsolation,
        
		# central jet veto/b-jet candidate selection
		evtSelNonCentralJetEt20bTag,
		evtSelCentralJetEt20,
		evtSelCentralJetEt20bTag
    ),
  
    analyzers = cms.VPSet(
        genPhaseSpaceEventInfoHistManager,
        eventWeightHistManager,
        electronHistManager,
        tauHistManager,
        diTauCandidateHistManagerForElecTau,
        diTauCandidateSVfitHistManagerForElecTau,                                     
        diTauCandidateZeeHypothesisHistManagerForElecTau,
		diTauLeg1ChargeBinGridHistManager,
		elecPairHistManagerByLooseIsolation,
        jetHistManager,
        caloMEtHistManager,
        pfMEtHistManager,
        particleMultiplicityHistManager,
        vertexHistManager,
        triggerHistManagerForElecTau,
		dataBinner
    ),

    analyzers_systematic = cms.VPSet(
		sysUncertaintyHistManagerForElecTau,
		sysUncertaintyBinnerForElecTauEff
	),                                     

    eventDumps = cms.VPSet(
        elecTauEventDump
    ),
   
    analysisSequence = elecTauAnalysisSequenceOS_woBtag,

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
analyzeAHtoElecTauEventsOS_wBtag = analyzeAHtoElecTauEventsOS_woBtag.clone(

    name = cms.string('ahElecTauAnalyzerOS_wBtag'),
    eventDumps = cms.VPSet(
    ),

    analysisSequence = elecTauAnalysisSequenceOS_wBtag
)

analyzeAHtoElecTauEventsSS_woBtag = analyzeAHtoElecTauEventsOS_woBtag.clone(
    name = cms.string('ahElecTauAnalyzerSS_woBtag'),
    analysisSequence = elecTauAnalysisSequenceSS_woBtag
)

analyzeAHtoElecTauEventsSS_wBtag = analyzeAHtoElecTauEventsOS_wBtag.clone(
    name = cms.string('ahElecTauAnalyzerSS_wBtag'),
    analysisSequence = elecTauAnalysisSequenceSS_wBtag
)

analyzeAHtoElecTauSequence = cms.Sequence(
    analyzeAHtoElecTauEventsOS_woBtag * analyzeAHtoElecTauEventsOS_wBtag
   * analyzeAHtoElecTauEventsSS_woBtag * analyzeAHtoElecTauEventsSS_wBtag
)

