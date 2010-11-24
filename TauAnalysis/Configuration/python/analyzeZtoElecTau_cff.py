import FWCore.ParameterSet.Config as cms
import copy

# import config for event selection, event print-out and analysis sequence
from TauAnalysis.Configuration.analyzeZtoElecTau_cfi import *

# define auxiliary service
# for handling of systematic uncertainties
from TauAnalysis.CandidateTools.sysErrDefinitions_cfi import *
SysUncertaintyService = cms.Service("SysUncertaintyService",
    weights = getSysUncertaintyParameterSets(
        [ electronSystematics,
          tauSystematics,
          theorySystematics ]
    )
)

analyzeZtoElecTauEvents = cms.EDAnalyzer("GenericAnalyzer",
  
    name = cms.string('zElecTauAnalyzer'), 
                            
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
        evtSelElectronTrkIso,
        evtSelElectronEcalIso,
        evtSelElectronConversionVeto,
        evtSelElectronTrkIP,

        # tau candidate selection
        evtSelTauAntiOverlapWithElectronsVeto,
        evtSelTauEta,
        evtSelTauPt,
        evtSelTauLeadTrk,
        evtSelTauLeadTrkPt,
				evtSelTauTaNCdiscr,
        evtSelTauTrkIso,
        evtSelTauEcalIso,
        evtSelTauProng,
        evtSelTauCharge,
        evtSelTauElectronVeto,
        evtSelTauEcalCrackVeto,
        evtSelTauMuonVeto,
        
        # di-tau candidate selection
        evtSelDiTauCandidateForElecTauAntiOverlapVeto,
        evtSelDiTauCandidateForElecTauZeroCharge,
        evtSelDiTauCandidateForElecTauAcoplanarity12,
        evtSelDiTauCandidateForElecTauMt1MET,
        evtSelDiTauCandidateForElecTauPzetaDiff,

        # veto events compatible with Z --> e+ e- hypothesis
        # (based on reconstructed (visible) invariant mass of e + tau-jet pair)
        evtSelElecTauPairZeeHypothesisVeto
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
   
    analysisSequence = elecTauAnalysisSequence,

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

