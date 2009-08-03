import FWCore.ParameterSet.Config as cms

# import config for event selection, event print-out and analysis sequence
from TauAnalysis.Configuration.analyzeZtoElecTau_cfi import *

analyzeZtoElecTauEvents = cms.EDAnalyzer("GenericAnalyzer",
  
    name = cms.string('zElecTauAnalyzer'), 
                            
    filters = cms.VPSet(
        # generator level phase-space selection
        # (NOTE: (1) to be used in case of Monte Carlo samples
        #            overlapping in simulated phase-space only !!
        #        (2) genPhaseSpaceCut needs to be **always** the first entry in the list of cuts
        #           - otherwise the script submitToBatch.csh for submission of cmsRun jobs
        #            to the CERN batch system will not work !!)
        genPhaseSpaceCut,
    
        # generator level selection of Z --> e + tau-jet events
        # passing basic acceptance and kinematic cuts
        # (NOTE: to be used for efficiency studies only !!)
        #genElectronCut,
        #genTauCut,
    
        # trigger selection
        #evtSelTrigger,
        
        # primary event vertex selection
        evtSelPrimaryEventVertex,
        evtSelPrimaryEventVertexQuality,
        evtSelPrimaryEventVertexPosition,
        
        # electron candidate selection
        evtSelTightElectronId,
        evtSelElectronAntiCrack,
        evtSelElectronEta,
        evtSelElectronPt,
        evtSelElectronTrkIso,
        evtSelElectronEcalIso,
        evtSelElectronTrk,
        evtSelElectronTrkIP,

        # tau candidate selection
        evtSelTauAntiOverlapWithElectronsVeto,
        evtSelTauEta,
        evtSelTauPt,
        evtSelTauLeadTrk,
        evtSelTauLeadTrkPt,
        evtSelTauTrkIso,
        evtSelTauEcalIso,
        evtSelTauProng,
        evtSelTauElectronVeto,
        evtSelTauEcalCrackVeto,
        
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
        electronHistManager,
        tauHistManager,
        diTauCandidateHistManagerForElecTau,
        diTauCandidateZeeHypothesisHistManager,
        metHistManager,
        vertexHistManager,
        triggerHistManager
    ),

    eventDumps = cms.VPSet(
        elecTauEventDump
    ),
   
    analysisSequence = elecTauAnalysisSequence
)
