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
            "sysZllRecoilCorrection*", "",
            "sysJet*", ""
        ),
        isRecAHtoMuTauCentralJetBtag = cms.vstring(
            "sysMuon*", "",
            "sysTau*", "",
            "sysZllRecoilCorrection*", "",
            "sysJet*", ""
        ),
        isRecAHtoMuTauVBFtag = cms.vstring(
            "sysMuon*", "",
            "sysTau*", "",
            "sysZllRecoilCorrection*", "",
            "sysJet*", ""
        )                                
    )
)

analyzeAHtoMuTauEventsOS_woBtag = cms.EDAnalyzer("GenericAnalyzer",

    name = cms.string('ahMuTauAnalyzerOS_woBtag'),

    filters = cms.VPSet(
        # generator level phase-space selection
        evtSelGenPhaseSpace,

        # trigger selection
        evtSelTrigger,

        # data-quality selection
        evtSelDataQuality,

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
        evtSelDiTauCandidateForAHtoMuTauAntiOverlapVeto,
        evtSelDiTauCandidateForAHtoMuTauMt1MET,
        evtSelDiTauCandidateForAHtoMuTauPzetaDiff,
        evtSelDiTauCandidateForAHtoMuTauZeroCharge,
        evtSelDiTauCandidateForAHtoMuTauNonZeroCharge,

        # primary event vertex selection
        evtSelPrimaryEventVertexForMuTau,
        evtSelPrimaryEventVertexQualityForMuTau,
        evtSelPrimaryEventVertexPositionForMuTau,

        # Z/gamma* --> mu+ mu- hypothesis veto (based on combinations of muon pairs)
        evtSelDiMuPairZmumuHypothesisVetoByLooseIsolation,
        evtSelDiMuPairDYmumuHypothesisVeto,                                             

        # central jet veto/b-jet candidate selection
        evtSelNonCentralJetEt20bTag,
        evtSelCentralJetEt20,
        evtSelCentralJetEt20bTag,

        # VBF event selection
        evtSelVBFtag,
        evtSelVBFdEta35,
        evtSelVBFmass350,
        evtSel3rdTagJetVeto
    ),

    analyzers = cms.VPSet(
        genPhaseSpaceEventInfoHistManager,
        eventWeightHistManager,
        muonHistManager,
        tauHistManager,
        diTauCandidateHistManagerForMuTau,
        diTauCandidateNSVfitHistManagerForMuTau,
        diTauCandidateZmumuHypothesisHistManagerForMuTau,
        diTauLeg1ChargeBinGridHistManager,
        muPairHistManagerZmumuHypothesesByLooseIsolation,
        muPairHistManagerDYmumuHypotheses,
        jetHistManager,
        vbfEventHistManagerForMuTau,                                             
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
        muTauEventDump_woBtag,
        patTauDebugEventDump
    ),

    analysisSequence = muTauAnalysisSequenceOS_woBtag,

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

analyzeAHtoMuTauEventsOS_wBtag = analyzeAHtoMuTauEventsOS_woBtag.clone(

    name = cms.string('ahMuTauAnalyzerOS_wBtag'),

    eventDumps = cms.VPSet(
        muTauEventDump_wBtag
    ),

    analysisSequence = muTauAnalysisSequenceOS_wBtag
)

analyzeAHtoMuTauEventsOS_wVBFtag = analyzeAHtoMuTauEventsOS_woBtag.clone(

    name = cms.string('ahMuTauAnalyzerOS_wVBFtag'),

    eventDumps = cms.VPSet(
        muTauEventDump_wVBFtag
    ),

    analysisSequence = muTauAnalysisSequenceOS_wVBFtag,

    systematics = cms.vstring(
        getSysUncertaintyNames(
            [ muonSystematics,
              tauSystematics,
              muTauPairSystematics,
              muTauPairVBFEventSystematics,
              jetSystematics,
              theorySystematics ]
        )
    )
)

analyzeAHtoMuTauEventsSS_woBtag = analyzeAHtoMuTauEventsOS_woBtag.clone(
    name = cms.string('ahMuTauAnalyzerSS_woBtag'),
    analysisSequence = muTauAnalysisSequenceSS_woBtag
)

analyzeAHtoMuTauEventsSS_wBtag = analyzeAHtoMuTauEventsOS_wBtag.clone(
    name = cms.string('ahMuTauAnalyzerSS_wBtag'),
    analysisSequence = muTauAnalysisSequenceSS_wBtag
)

analyzeAHtoMuTauSequence = cms.Sequence(
    analyzeAHtoMuTauEventsOS_woBtag * analyzeAHtoMuTauEventsOS_wBtag
   * analyzeAHtoMuTauEventsSS_woBtag * analyzeAHtoMuTauEventsSS_wBtag
)
