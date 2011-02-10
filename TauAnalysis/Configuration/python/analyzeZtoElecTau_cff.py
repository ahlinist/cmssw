import FWCore.ParameterSet.Config as cms
import copy

# import config for event selection, event print-out and analysis sequence
from TauAnalysis.Configuration.analyzeZtoElecTau_cfi import *

# define auxiliary service
# for handling of systematic uncertainties
from TauAnalysis.CandidateTools.sysErrDefinitions_cfi import *
SysUncertaintyService = cms.Service("SysUncertaintyService",
    weights = getSysUncertaintyParameterSets(
        [ theorySystematics ]
    ),
	sources = cms.PSet(
        isRecZtoElecTau = cms.vstring(
            "sysElectron*", "",
            "sysTau*", "",
            "sysZllRecoilCorrection*", "",
            "sysJet*", ""
        )
    )
)

analyzeZtoElecTauEventsOS = cms.EDAnalyzer("GenericAnalyzer",
  
    name = cms.string('zElecTauAnalyzerOS'), 
                            
    filters = cms.VPSet(
        # generator level phase-space selection
        evtSelGenPhaseSpace,
    
        # generator level selection of Z --> e + tau-jet events
        # passing basic acceptance and kinematic cuts
        # (NOTE: to be used for efficiency studies only !!)
				#genElectronCut,
        #genTauCut,
    
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
        evtSelDiTauCandidateForElecTauAntiOverlapVeto,
        evtSelDiTauCandidateForElecTauMt1MET,
        evtSelDiTauCandidateForElecTauPzetaDiff,
        evtSelDiTauCandidateForElecTauZeroCharge,
        evtSelDiTauCandidateForElecTauNonZeroCharge,

        # veto events compatible with Z --> e+ e- hypothesis
        evtSelDiElecPairZeeHypothesisVetoByLooseIsolation
    ),
  
    analyzers = cms.VPSet(
        genPhaseSpaceEventInfoHistManager,
        eventWeightHistManager,
        electronHistManager,
        tauHistManager,
        diTauCandidateHistManagerForElecTau,
        diTauCandidateSVfitHistManagerForElecTau,                                     
        diTauCandidateZeeHypothesisHistManagerForElecTau,
        jetHistManager,
        caloMEtHistManager,
        pfMEtHistManager,
        particleMultiplicityHistManager,
        vertexHistManager,
        triggerHistManagerForElecTau
    ),

    analyzers_systematic = cms.VPSet(),                                     

    eventDumps = cms.VPSet(
        elecTauEventDump
    ),
   
    analysisSequence = elecTauAnalysisSequenceOS,

    #estimateSysUncertainties = cms.bool(True),                                       
    estimateSysUncertainties = cms.bool(False), 
    systematics = cms.vstring(
        getSysUncertaintyNames(
            [ electronSystematics,
              tauSystematics,
              theorySystematics ]
        )
    )                                         
)

analyzeZtoElecTauEventsSS = analyzeZtoElecTauEventsOS.clone(
	name = cms.string('zElecTauAnalyzerSS'),
	analysisSequence = elecTauAnalysisSequenceSS
)

analyzeZtoElecTauEvents = cms.Sequence( analyzeZtoElecTauEventsOS * analyzeZtoElecTauEventsSS )

