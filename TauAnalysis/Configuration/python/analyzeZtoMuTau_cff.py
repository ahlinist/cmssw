import FWCore.ParameterSet.Config as cms

# import config for event selection, event print-out and analysis sequence
from TauAnalysis.Configuration.analyzeZtoMuTau_cfi import *

# import definitions of systematic uncertainties
from TauAnalysis.CandidateTools.sysErrDefinitions_cfi import *

# define auxiliary service
# for handling of systematic uncertainties
from TauAnalysis.CandidateTools.sysErrDefinitions_cfi import *
SysUncertaintyService = cms.Service("SysUncertaintyService",
    config = getSysUncertaintyParameterSets(
        [ muonSystematics,
          tauSystematics,
          theorySystematics ]
    )
)

analyzeZtoMuTauEvents = cms.EDAnalyzer("GenericAnalyzer",
  
    name = cms.string('zMuTauAnalyzer'), 
                            
    filters = cms.VPSet(
        # generator level phase-space selection
        genPhaseSpaceCut,
    
        # generator level selection of Z --> mu + tau-jet events
        # passing basic acceptance and kinematic cuts
        # (NOTE: to be used for efficiency studies only !!)
        #genMuonCut,
        #genTauCut,

        # trigger selection
        evtSelTrigger,

        # primary event vertex selection
        evtSelPrimaryEventVertex,
        evtSelPrimaryEventVertexQuality,
        evtSelPrimaryEventVertexPosition,

        # muon candidate selection
        evtSelGlobalMuon,
        evtSelMuonEta,
        evtSelMuonPt,
        evtSelMuonTrkIso,
        evtSelMuonEcalIso,
        evtSelMuonAntiPion,
        evtSelMuonTrkIP,

        # tau candidate selection
        evtSelTauAntiOverlapWithMuonsVeto,
        evtSelTauEta,
        evtSelTauPt,
        evtSelTauLeadTrk,
        evtSelTauLeadTrkPt,
        evtSelTauTrkIso,
        evtSelTauEcalIso,
        evtSelTauProng,
        evtSelTauCharge,
        evtSelTauMuonVeto,

        # di-tau candidate selection
        evtSelDiTauCandidateForMuTauAntiOverlapVeto,
        evtSelDiTauCandidateForMuTauZeroCharge,
        evtSelDiTauCandidateForMuTauAcoplanarity12,
        evtSelDiTauCandidateForMuTauMt1MET,
        evtSelDiTauCandidateForMuTauPzetaDiff,

        # Z --> mu+ mu- hypothesis veto (based on combinations of muon pairs)
        evtSelDiMuPairZmumuHypothesisVeto
    ),
  
    analyzers = cms.VPSet(
        genPhaseSpaceEventInfoHistManager,
        eventWeightHistManager,
        muonHistManager,
        tauHistManager,
        diTauCandidateHistManagerForMuTau,
        diTauCandidateCollinearApproxHistManagerForMuTau,
        diTauCandidateCollinearApproxHistManagerBinnedForMuTau,
        diTauCandidateCollinearApproxBinningForMuTau,
        diTauCandidateZmumuHypothesisHistManagerForMuTau,
        muPairHistManager,
        jetHistManager,
        caloMEtHistManager,
        pfMEtHistManager,
        particleMultiplicityHistManager,
        vertexHistManager,
        triggerHistManagerForMuTau,
        sysUncertaintyBinnerForMuTau
    ),

    eventDumps = cms.VPSet(
        muTauEventDump
    ),
   
    analysisSequence = muTauAnalysisSequence,

    systematics = cms.vstring(
        getSysUncertaintyNames(
            [ muonSystematics,
              tauSystematics,
              theorySystematics ]
        )
    )
)
