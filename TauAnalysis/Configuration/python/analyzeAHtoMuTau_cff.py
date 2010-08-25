import FWCore.ParameterSet.Config as cms

# import config for event selection, event print-out and analysis sequence
from TauAnalysis.Configuration.analyzeAHtoMuTau_cfi import *

analyzeAHtoMuTauEvents_woBtag = cms.EDAnalyzer("GenericAnalyzer",
  
    name = cms.string('ahMuTauAnalyzer_woBtag'), 
                            
    filters = cms.VPSet(
        # generator level phase-space selection
        genPhaseSpaceCut,

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
        evtSelTauTaNCdiscr,
        evtSelTauTrkIso,
        evtSelTauEcalIso,
        evtSelTauProng,
        evtSelTauCharge,
        evtSelTauMuonVeto,
        evtSelTauElectronVeto,

        # di-tau candidate selection
        evtSelDiTauCandidateForAHtoMuTauAntiOverlapVeto,
        evtSelDiTauCandidateForAHtoMuTauZeroCharge,
        evtSelDiTauCandidateForAHtoMuTauMt1MET,
        evtSelDiTauCandidateForAHtoMuTauPzetaDiff,
        evtSelDiTauCandidateForAHtoMuTauCollinearApproxZmassVeto,

        # Z --> mu+ mu- hypothesis veto (based on combinations of muon pairs)
        evtSelDiMuPairZmumuHypothesisVeto,

        # central jet veto/b-jet candidate selection
        evtSelNonCentralJetEt20bTag,
        evtSelCentralJetEt20,
        evtSelCentralJetEt20bTag 
    ),
  
    analyzers = cms.VPSet(
        genPhaseSpaceEventInfoHistManager,
        eventWeightHistManager,
        muonHistManager,
        tauHistManager,
        diTauCandidateHistManagerForMuTau,
        diTauCandidateCollinearApproxHistManagerForMuTau,
        diTauCandidateSVfitHistManagerForMuTau,
        diTauCandidateZmumuHypothesisHistManagerForMuTau,
        muPairHistManager,
        jetHistManager,
        caloMEtHistManager,
        pfMEtHistManager,
        particleMultiplicityHistManager,
        vertexHistManager,
        triggerHistManagerForMuTau,
        dataBinner,
        chi2mZbinGridHistManager
    ),

    eventDumps = cms.VPSet(
        muTauEventDump.clone(
            diTauCandidateSource = cms.InputTag('selectedMuTauPairsForAHtoMuTauCollinearApproxZmassVetoCumulative'),
            muTauZmumuHypothesisSource = cms.InputTag('muTauPairZmumuHypothesesForAHtoMuTau'),
            triggerConditions = cms.vstring("evtSelNonCentralJetEt20bTag: passed_cumulative")
        )
    ),
   
    analysisSequence = muTauAnalysisSequence_woBtag
)

analyzeAHtoMuTauEvents_wBtag = analyzeAHtoMuTauEvents_woBtag.clone(
  
    name = cms.string('ahMuTauAnalyzer_wBtag'), 

    eventDumps = cms.VPSet(
        muTauEventDump.clone(
            diTauCandidateSource = cms.InputTag('selectedMuTauPairsForAHtoMuTauCollinearApproxZmassVetoCumulative'),
            muTauZmumuHypothesisSource = cms.InputTag('muTauPairZmumuHypothesesForAHtoMuTau'),
            triggerConditions = cms.vstring("evtSelCentralJetEt20bTag: passed_cumulative")
        )
    ),
   
    analysisSequence = muTauAnalysisSequence_wBtag
)

analyzeAHtoMuTauEvents = cms.Sequence(analyzeAHtoMuTauEvents_woBtag * analyzeAHtoMuTauEvents_wBtag)
