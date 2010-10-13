import FWCore.ParameterSet.Config as cms

# import config for event selection, event print-out and analysis sequence
from TauAnalysis.Configuration.analyzeZtoElecMu_cfi import *

# define auxiliary service
# for handling of systematic uncertainties
from TauAnalysis.CandidateTools.sysErrDefinitions_cfi import *
SysUncertaintyService = cms.Service("SysUncertaintyService",
    weights = getSysUncertaintyParameterSets(
        [ electronSystematics,
          muonSystematics,
          theorySystematics ]
    )
)

analyzeZtoElecMuEvents = cms.EDAnalyzer("GenericAnalyzer",

    name = cms.string('zElecMuAnalyzer'),

    filters = cms.VPSet(
        # generator level phase-space selection
        genPhaseSpaceCut,
    
        # generator level selection of Z --> e + mu events
        # passing basic acceptance and kinematic cuts
        # (NOTE: to be used for efficiency studies only !!)
        #genElectronCut,
        #genMuonCut,
        
        # trigger selection
        evtSelTrigger,
        
        # primary event vertex selection
        evtSelPrimaryEventVertex,
        evtSelPrimaryEventVertexQuality,
        evtSelPrimaryEventVertexPosition,
        
        # electron candidate selection
        evtSelElectronAntiOverlapWithMuonsVeto,
        evtSelTightElectronId,
        evtSelElectronAntiCrack,
        evtSelElectronEta,
        evtSelElectronPt,
        evtSelElectronTrkIso,
        evtSelElectronEcalIso,
        evtSelElectronTrk,
        evtSelElectronTrkIP,

        # muon candidate selection
        evtSelGlobalMuon,
        evtSelMuonEta,
        evtSelMuonPt,
        evtSelMuonTrkIso,
        evtSelMuonEcalIso,
        evtSelMuonAntiPion,
        evtSelMuonTrkIP,
        
        # di-tau candidate selection
        evtSelDiTauCandidateForElecMuAntiOverlapVeto,
        evtSelDiTauCandidateForElecMuZeroCharge,
        evtSelDiTauCandidateForElecMuAcoplanarity12,
        evtSelDiTauCandidateForElecMuMt1MET,
        evtSelDiTauCandidateForElecMuMt2MET,
        evtSelDiTauCandidateForElecMuPzetaDiff
    ),

    analyzers = cms.VPSet(
        genPhaseSpaceEventInfoHistManager,
        eventWeightHistManager,
        electronHistManager,
        muonHistManager,
        diTauCandidateHistManagerForElecMu,
        diTauCandidateSVfitHistManagerForElecMu,
        diTauCandidateZmumuHypothesisHistManagerForElecMu,
        jetHistManager,
        caloMEtHistManager,
        pfMEtHistManager,
        particleMultiplicityHistManager,
        vertexHistManager,
        triggerHistManagerForElecMu
    ),

    analyzers_systematic = cms.VPSet(),                                    

    eventDumps = cms.VPSet(
        elecMuEventDump
    ),

    analysisSequence = elecMuAnalysisSequence,

    #estimateSysUncertainties = cms.bool(True),                                       
    estimateSysUncertainties = cms.bool(False), 
    systematics = cms.vstring(
        getSysUncertaintyNames(
            [ electronSystematics,
              muonSystematics,
              theorySystematics ]
        )
    )                                        
)

