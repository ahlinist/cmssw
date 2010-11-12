import FWCore.ParameterSet.Config as cms

# import config for event selection, event print-out and analysis sequence
from TauAnalysis.Configuration.analyzeZtoMuTau_cfi import *

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
        isRecZtoMuTau = cms.vstring(
            "sysMuon*", "",
            "sysTau*", "",
            "sysJet*", ""
        )        
    )
)

analyzeZtoMuTauEvents = cms.EDAnalyzer("GenericAnalyzer",
  
    name = cms.string('zMuTauAnalyzer'), 
                            
    filters = cms.VPSet(
        # generator level phase-space selection
        evtSelGenPhaseSpace,
    
        # generator level selection of Z --> mu + tau-jet events
        # passing basic acceptance and kinematic cuts
        # (NOTE: to be used for efficiency studies only !!)
        #genMuonCut,
        #genTauCut,

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
        evtSelDiTauCandidateForMuTauAntiOverlapVeto,
        evtSelDiTauCandidateForMuTauZeroCharge,
        evtSelDiTauCandidateForMuTauAcoplanarity12,
        evtSelDiTauCandidateForMuTauMt1MET,
        evtSelDiTauCandidateForMuTauPzetaDiff,

        # Z --> mu+ mu- hypothesis veto (based on combinations of muon pairs)
        evtSelDiMuPairZmumuHypothesisVetoByLooseIsolation,
        evtSelDiMuPairZmumuHypothesisVetoByMass
    ),
  
    analyzers = cms.VPSet(
        genPhaseSpaceEventInfoHistManager,
        eventWeightHistManager,
        muonHistManager,
        tauHistManager,
        diTauCandidateHistManagerForMuTau,
        diTauCandidateCollinearApproxHistManagerForMuTau,
        diTauCandidateCollinearApproxHistManagerBinnedForMuTau,
        diTauCandidateCollinearApproxBinnerForMuTau,
        diTauCandidateSVfitHistManagerForMuTau,
        diTauCandidateEventActivityHistManagerForMuTau,
        diTauCandidateZmumuHypothesisHistManagerForMuTau,
        muPairHistManagerByLooseIsolation,
        muPairHistManagerByMass,
        jetHistManager,
        caloMEtHistManager,
        pfMEtHistManager,
        particleMultiplicityHistManager,
        vertexHistManager,
        triggerHistManagerForMuTau,
        dataBinner,
        modelBinnerForMuTauGenTauLeptonPairAcc,
        modelBinnerForMuTauWrtGenTauLeptonPairAcc
    ),

    analyzers_systematic = cms.VPSet(
        sysUncertaintyBinnerForMuTauAcc,
	sysUncertaintyBinnerForMuTauEff
    ),

    eventDumps = cms.VPSet(
        muTauEventDump
    ),
   
    analysisSequence = muTauAnalysisSequence,

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

analyzeZtoMuTauSequence = cms.Sequence(analyzeZtoMuTauEvents)
