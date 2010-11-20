import FWCore.ParameterSet.Config as cms

# import config for event selection, event print-out and analysis sequence
from TauAnalysis.Configuration.analyzeAHtoMuTau_cfi import *

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
        isRecAHtoMuTauCentralJetVeto = cms.vstring(
            "sysMuon*", "",
            "sysTau*", "",
            "sysJet*", ""                               
        ),
        isRecAHtoMuTauCentralJetBtag = cms.vstring(
            "sysMuon*", "",
            "sysTau*", "",
            "sysJet*", ""                               
        )                                
    )
)

analyzeAHtoMuTauEvents_woBtag = cms.EDAnalyzer("GenericAnalyzer",
  
    name = cms.string('ahMuTauAnalyzer_woBtag'), 
                            
    filters = cms.VPSet(
        # generator level phase-space selection
        evtSelGenPhaseSpace,

        # trigger selection
        evtSelTrigger,

        # data-quality selection
        evtSelDataQuality,

        # primary event vertex selection
        evtSelPrimaryEventVertex,
        evtSelPrimaryEventVertexQuality,
        evtSelPrimaryEventVertexPosition,

        # muon candidate selection
        evtSelGlobalMuon,
        evtSelMuonEta,
        evtSelMuonPt,
        evtSelMuonVbTfId,
        evtSelMuonPFRelIso,
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

        # Z --> mu+ mu- hypothesis veto (based on combinations of muon pairs)
        evtSelDiMuPairZmumuHypothesisVetoByLooseIsolation,

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
        diTauLeg1ChargeBinGridHistManager,
        muPairHistManagerByLooseIsolation,
        jetHistManager,
        caloMEtHistManager,
        pfMEtHistManager,
        particleMultiplicityHistManager,
        vertexHistManager,
        triggerHistManagerForMuTau,
        dataBinner,
        modelBinnerForMuTauGenTauLeptonPairAcc,
        modelBinnerForMuTauCentralJetVetoWrtGenTauLeptonPairAcc,
        modelBinnerForMuTauCentralJetBtagWrtGenTauLeptonPairAcc                                           
    ),

    analyzers_systematic = cms.VPSet(
	sysUncertaintyBinnerForMuTauAccCentralJetVeto,
	sysUncertaintyBinnerForMuTauAccCentralJetBtag,
	sysUncertaintyBinnerForMuTauEff,
        sysUncertaintyHistManagerForMuTau                                           
    ),                                           

    eventDumps = cms.VPSet(
        muTauEventDump_woBtag
    ),
   
    analysisSequence = muTauAnalysisSequence_woBtag,

    estimateSysUncertainties = cms.bool(False), 
    systematics = cms.vstring(
        getSysUncertaintyNames(
            [ muonSystematics,
              tauSystematics,
              jetSystematics,
              theorySystematics ]
        )
    )                                                 
)

analyzeAHtoMuTauEvents_wBtag = analyzeAHtoMuTauEvents_woBtag.clone(
  
    name = cms.string('ahMuTauAnalyzer_wBtag'), 

    eventDumps = cms.VPSet(
        muTauEventDump_wBtag
    ),
   
    analysisSequence = muTauAnalysisSequence_wBtag
)

# split analysis into b-Tag and no-b-Tag channels
analyzeAHtoMuTauEvents = cms.Sequence(analyzeAHtoMuTauEvents_woBtag * analyzeAHtoMuTauEvents_wBtag)
# disable b-Tag channel for now
#analyzeAHtoMuTauEvents = cms.Sequence(analyzeAHtoMuTauEvents_woBtag)
