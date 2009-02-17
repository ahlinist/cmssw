import FWCore.ParameterSet.Config as cms

# import config for event selection, event print-out and analysis sequence
from TauAnalysis.Configuration.analyzeZtoElecMu_cfi import *

analyzeZtoElecMu = cms.EDAnalyzer("GenericAnalyzer",

  name = cms.string('zElecMuAnalyzer'),

  eventSelection = cms.VPSet(
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
    muonHcalIsoCut,
    muonAntiPionCut,
    muonTrkIPcut,

    # di-tau candidate selection
    diTauCandidateForElecMuAcoplanarityCut,
    diTauCandidateForElecMuZeroChargeCut,

    # veto events containing additional central jets with Et > 20 GeV
    centralJetVeto
  ),

  histManagers = cms.VPSet(
    electronHistManager,
    muonHistManager,
    diTauCandidateHistManagerForElecMu,
    metHistManager,
    vertexHistManager,
    triggerHistManager 
  ),

  eventDumps = cms.VPSet(
    elecMuEventDump
  ),

  analysisSequence = elecMuAnalysisSequence
)

