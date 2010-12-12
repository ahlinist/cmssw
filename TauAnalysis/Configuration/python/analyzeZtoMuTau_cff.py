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
            "sysZllRecoilCorrection*", "",
            "sysJet*", ""
        )        
    )
)

analyzeZtoMuTauEventsOS = cms.EDAnalyzer("GenericAnalyzer",
  
    name = cms.string('zMuTauAnalyzerOS'), 
                            
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
        evtSelMuonTrkIP,

        # tau candidate selection
        evtSelTauAntiOverlapWithMuonsVeto,
        evtSelTauEta,
        evtSelTauPt,
        evtSelTauLeadTrk,
        evtSelTauLeadTrkPt,
        evtSelTauTaNCdiscr,
        evtSelTauProng,
        evtSelTauCharge,
        evtSelTauMuonVeto,
        evtSelTauElectronVeto,

        # di-tau candidate selection
        evtSelDiTauCandidateForMuTauAntiOverlapVeto,
        evtSelDiTauCandidateForMuTauMt1MET,
        evtSelDiTauCandidateForMuTauPzetaDiff,
        evtSelDiTauCandidateForMuTauZeroCharge,
        evtSelDiTauCandidateForMuTauNonZeroCharge,

        # Z --> mu+ mu- hypothesis veto (based on combinations of muon pairs)
        evtSelDiMuPairZmumuHypothesisVetoByLooseIsolation
    ),
  
    analyzers = cms.VPSet(
        genPhaseSpaceEventInfoHistManager,
        eventWeightHistManager,
        muonHistManager,
        tauHistManager,
        diTauCandidateHistManagerForMuTau,
        diTauCandidateSVfitHistManagerForMuTau,
        diTauCandidateZmumuHypothesisHistManagerForMuTau,
        muPairHistManagerByLooseIsolation,
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
   
    analysisSequence = muTauAnalysisSequenceOS,

    estimateSysUncertainties = cms.bool(False), 
    systematics = cms.vstring(
        getSysUncertaintyNames(
            [ muonSystematics,
              tauSystematics,
              muTauPairSystematics,
              jetSystematics,
              theorySystematics ]
        )
    )                                   
)

analyzeZtoMuTauEventsSS = analyzeZtoMuTauEventsOS.clone(
    name = cms.string('zMuTauAnalyzerSS'), 
    analysisSequence = muTauAnalysisSequenceSS
)    

analyzeZtoMuTauSequence = cms.Sequence(analyzeZtoMuTauEventsOS * analyzeZtoMuTauEventsSS)
