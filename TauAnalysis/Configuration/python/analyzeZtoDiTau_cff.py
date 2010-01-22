import FWCore.ParameterSet.Config as cms

# import config for event selection, event print-out and analysis sequence
from TauAnalysis.Configuration.analyzeZtoDiTau_cfi import *

# define auxiliary service
# for handling of systematic uncertainties
from TauAnalysis.CandidateTools.sysErrDefinitions_cfi import *
SysUncertaintyService = cms.Service("SysUncertaintyService",
    config = getSysUncertaintyParameterSets(
        [ tauSystematics,
          theorySystematics ]
    )
)

analyzeZtoDiTauEvents = cms.EDAnalyzer("GenericAnalyzer",
  
    name = cms.string('zDiTauAnalyzer'), 
                            
    filters = cms.VPSet(
        # generator level phase-space selection
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
        caloMEtHistManager,
        pfMEtHistManager,
        jetHistManager,
        vertexHistManager,
        triggerHistManagerForDiTau
    ),

    analyzers_systematic = cms.VPSet(),

    eventDumps = cms.VPSet(
        diTauEventDump
    ),
   
    analysisSequence = diTauAnalysisSequence,

    #estimateSysUncertainties = cms.bool(True),                                       
    estimateSysUncertainties = cms.bool(False), 
    systematics = cms.vstring(
        getSysUncertaintyNames(
            [ tauSystematics,
              theorySystematics ]
        )
    )                                       
)
