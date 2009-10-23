import FWCore.ParameterSet.Config as cms

# import config for event selection, event print-out and analysis sequence
from TauAnalysis.Configuration.analyzeZtoDiTau_cfi import *

analyzeZtoDiTauEvents = cms.EDAnalyzer("GenericAnalyzer",
  
    name = cms.string('zDiTauAnalyzer'), 
                            
    filters = cms.VPSet(
        # generator level phase-space selection
        # (NOTE: (1) to be used in case of Monte Carlo samples
        #            overlapping in simulated phase-space only !!
        #        (2) genPhaseSpaceCut needs to be **always** the first entry in the list of cuts
        #           - otherwise the script submitToBatch.csh for submission of cmsRun jobs
        #            to the CERN batch system will not work !!)
        genPhaseSpaceCut,
    
        # generator level selection of pure hadronic Z --> tau-jet + tau-jet events
        # passing basic acceptance and kinematic cuts
        # (NOTE: to be used for efficiency studies only !!)
        #genDiTauCut,
      
        # trigger selection
        #evtSelTrigger,

        # primary event vertex selection
        evtSelPrimaryEventVertex,
        evtSelPrimaryEventVertexQuality,
        evtSelPrimaryEventVertexPosition,
        
        # selection of first tau-jet candidate
        evtSelFirstTauEta,
        evtSelFirstTauPt,
        evtSelFirstTauLeadTrk,
        evtSelFirstTauLeadTrkPt,
        evtSelFirstTauTrkIso,
        evtSelFirstTauEcalIso,
        evtSelFirstTauProng,
        evtSelFirstTauCharge,
        
        # selection of second tau-jet candidate
        evtSelSecondTauEta,
        evtSelSecondTauPt,
        evtSelSecondTauLeadTrk,
        evtSelSecondTauLeadTrkPt,
        evtSelSecondTauTrkIso,
        evtSelSecondTauEcalIso,
        evtSelSecondTauProng,
        evtSelSecondTauCharge,

        # di-tau candidate selection
        evtSelDiTauCandidateForDiTauAntiOverlapVeto,
        evtSelDiTauCandidateForDiTauAcoplanarity,
        evtSelDiTauCandidateForDiTauZeroCharge,
        
        # veto events containing additional central jets with Et > 20 GeV
        #evtSelCentralJetVeto
    ),
  
    analyzers = cms.VPSet(
        genPhaseSpaceEventInfoHistManager,
        tauHistManager1,
        tauHistManager2,
        diTauCandidateHistManagerForDiTau,
        metHistManager,
        jetHistManager,
        vertexHistManager,
        triggerHistManagerForDiTau
    ),

    eventDumps = cms.VPSet(
        diTauEventDump
    ),
   
    analysisSequence = diTauAnalysisSequence
)
