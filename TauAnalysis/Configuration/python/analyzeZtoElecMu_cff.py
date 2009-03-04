import FWCore.ParameterSet.Config as cms

# import config for event selection, event print-out and analysis sequence
from TauAnalysis.Configuration.analyzeZtoElecMu_cfi import *

analyzeZtoElecMu = cms.EDAnalyzer("GenericAnalyzer",

  name = cms.string('zElecMuAnalyzer'),

  eventSelection = cms.VPSet(
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
    Trigger,
    
    # primary event vertex selection
    primaryEventVertex,
    primaryEventVertexQuality,
    primaryEventVertexPosition,

    # electron candidate selection
    tightElectronIdCut,
    electronAntiCrackCut,
    electronEtaCut,
    electronPtCut,
    electronHLTmatchCut,
    electronTrkIsoCut,
    electronEcalIsoCut,
    electronTrkCut,
    electronTrkIPcut,

    # muon candidate selection
    globalMuonCut,
    muonEtaCut,
    muonPtCut,
    muonHLTmatchCut,
    muonTrkIsoCut,
    muonEcalIsoCut,
    muonAntiPionCut,
    muonTrkIPcut,

    # di-tau candidate selection
    diTauCandidateForElecMuAcoplanarityCut,
    diTauCandidateForElecMuZeroChargeCut,

    # veto events containing additional central jets with Et > 20 GeV
    centralJetVeto
  ),

  histManagers = cms.VPSet(
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

