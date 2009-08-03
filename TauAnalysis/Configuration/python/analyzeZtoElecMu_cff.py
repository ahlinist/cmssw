import FWCore.ParameterSet.Config as cms

# import config for event selection, event print-out and analysis sequence
from TauAnalysis.Configuration.analyzeZtoElecMu_cfi import *

analyzeZtoElecMuEvents = cms.EDAnalyzer("GenericAnalyzer",

    name = cms.string('zElecMuAnalyzer'),

    filters = cms.VPSet(
        # generator level phase-space selection
        # (NOTE: (1) to be used in case of Monte Carlo samples
        #            overlapping in simulated phase-space only !!
        #        (2) genPhaseSpaceCut needs to be **always** the first entry in the list of cuts
        #           - otherwise the script submitToBatch.csh for submission of cmsRun jobs
        #            to the CERN batch system will not work !!)
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
        electronHistManager,
        muonHistManager,
        diTauCandidateHistManagerForElecMu,
        metHistManager,
        jetHistManager,
        vertexHistManager,
        triggerHistManager 
    ),

    eventDumps = cms.VPSet(
        elecMuEventDump
    ),

    analysisSequence = elecMuAnalysisSequence
)

