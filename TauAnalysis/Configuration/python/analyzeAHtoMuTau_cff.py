import FWCore.ParameterSet.Config as cms

# import config for event selection, event print-out and analysis sequence
from TauAnalysis.Configuration.analyzeAHtoMuTau_cfi import *

analyzeAHtoMuTauEvents_centralJetVeto = cms.EDAnalyzer("GenericAnalyzer",
  
    name = cms.string('ahMuTauAnalyzer_centralJetVeto'), 
                            
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
        evtSelDiTauCandidateForAHtoMuTauNonBackToBack,
        evtSelDiTauCandidateForAHtoMuTauValidCollinearApprox,

        # Z --> mu+ mu- hypothesis veto (based on combinations of muon pairs)
        evtSelDiMuPairZmumuHypothesisVeto,

        # central jet veto/b-jet candidate selection
        evtSelCentralJetVeto,
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
        diTauCandidateZmumuHypothesisHistManagerForMuTau,
        muPairHistManager,
        jetHistManager,
        caloMEtHistManager,
        pfMEtHistManager,
        particleMultiplicityHistManager,
        vertexHistManager,
        triggerHistManagerForMuTau,
        dataBinner
    ),

    eventDumps = cms.VPSet(
        muTauEventDump.clone(
            diTauCandidateSource = cms.InputTag('selectedMuTauPairsForAHtoMuTauPzetaDiffCumulative'),
            muTauZmumuHypothesisSource = cms.InputTag('muTauPairZmumuHypothesesForAHtoMuTauCentralJetVeto'),
            triggerConditions = cms.vstring("evtSelCentralJetVeto: passed_cumulative")
        )
    ),
   
    analysisSequence = muTauAnalysisSequence_centralJetVeto
)

analyzeAHtoMuTauEvents_centralJetBtag = analyzeAHtoMuTauEvents_centralJetVeto.clone(
  
    name = cms.string('ahMuTauAnalyzer_centralJetBtag'), 

    eventDumps = cms.VPSet(
        muTauEventDump.clone(
            diTauCandidateSource = cms.InputTag('selectedMuTauPairsForAHtoMuTauValidCollinearApproxCumulative'),
            muTauZmumuHypothesisSource = cms.InputTag('muTauPairZmumuHypothesesForAHtoMuTauCentralJetBtag'),
            triggerConditions = cms.vstring("evtSelCentralJetEt20bTag: passed_cumulative")
        )
    ),
   
    analysisSequence = muTauAnalysisSequence_centralJetBtag
)

analyzeAHtoMuTauEvents = cms.Sequence(analyzeAHtoMuTauEvents_centralJetVeto * analyzeAHtoMuTauEvents_centralJetBtag)
