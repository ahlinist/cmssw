import FWCore.ParameterSet.Config as cms

# import config for event selection, event print-out and analysis sequence
from TauAnalysis.Configuration.analyzeZtoDiTau_cfi import *

# define auxiliary service
# for handling of systematic uncertainties
from TauAnalysis.CandidateTools.sysErrDefinitions_cfi import *
SysUncertaintyService = cms.Service("SysUncertaintyService",
    weights = getSysUncertaintyParameterSets(
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
        evtSelFirstTauTaNCdiscr,
        evtSelFirstTauTrkIso,
        evtSelFirstTauEcalIso,
        evtSelFirstTauProng,
        evtSelFirstTauCharge,
        evtSelFirstTauMuonVeto,
        evtSelFirstTauElectronVeto,
        
        # selection of second tau-jet candidate
        evtSelSecondTauEta,
        evtSelSecondTauPt,
        evtSelSecondTauLeadTrk,
        evtSelSecondTauLeadTrkPt,
        evtSelSecondTauTaNCdiscr,
        evtSelSecondTauTrkIso,
        evtSelSecondTauEcalIso,
        evtSelSecondTauProng,
        evtSelSecondTauCharge,
        evtSelSecondTauMuonVeto,
        evtSelSecondTauElectronVeto,

        # di-tau candidate selection
        evtSelDiTauCandidateForDiTauAntiOverlapVeto,
        evtSelDiTauCandidateForDiTauZeroCharge,
        evtSelDiTauCandidateForDiTauAcoplanarity,
        evtSelDiTauCandidateForDiTauPzetaDiff
        
        # veto events containing additional central jets with Et > 20 GeV
        #evtSelCentralJetVeto
    ),
  
    analyzers = cms.VPSet(
        genPhaseSpaceEventInfoHistManager,
        eventWeightHistManager,
        tauHistManager1,
        tauHistManager2,
        diTauCandidateHistManagerForDiTau,
        diTauCandidateCollinearApproxHistManagerForDiTau,
        diTauCandidateSVfitHistManagerForDiTau,
        jetHistManagerForDiTau,
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
