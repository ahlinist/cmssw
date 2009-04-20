import FWCore.ParameterSet.Config as cms

# import config for event selection, event print-out and analysis sequence
from TauAnalysis.Configuration.analyzeAHtoElecMu_cfi import *

analyzeAHtoElecMuEvents = cms.EDAnalyzer("GenericAnalyzer",

  name = cms.string('ahElecMuAnalyzer'),

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
    tightElectronIdMinCut,
    tightElectronIdMaxCut,
    electronAntiCrackCut,
    electronEtaCut, #default 2.1, switch to 2.4
    electronPtCut, #now 15, too low?
    electronTrkIsoCut,
    electronEcalIsoCut,
    #electron hcal iso?
    electronTrkCut,
    ##electronTrkIPcut, #mmm... we used to cut on combined d0 significance!

    # muon candidate selection
    globalMuonMinCut,
    globalMuonMaxCut,
    muonEtaCut, #default 2.1, switch to 2.4
    muonPtCut, #now 15, too low?
    muonTrkIsoCut,
    muonEcalIsoCut,
    #muonHcalIsoCut, #why not?
    muonAntiPionCut,
    ##muonTrkIPcut, #mmm... we used to cut on combined d0 significance!

    # di-tau candidate selection
    ##diTauCandidateForElecMuAcoplanarityCut, #use it in case of no collinear approximation?
    diTauCandidateForElecMuZeroChargeCut,
    diTauCandidateForElecMuDPhiCut,
    diTauCandidateForElecMuImpParamSigCut,

    # jet candidate selection...
    jetMinCut, #default eta<2.1 too tight, need 2.4 for b-tagging
    jetMaxCut,    
    # b-tagging candidate selection...
    jetBtagMinCut,
    jetBtagMaxCut
  ),

  histManagers = cms.VPSet(
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

