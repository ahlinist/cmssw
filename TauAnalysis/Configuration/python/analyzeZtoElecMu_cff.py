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
    #genTauCut,

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

    # require common event vertex for electron and tau-jet candidate
    #elecTauEventVertex,

    # acoplanarity cut between electron and tau-jet candidate
    # ( dPhi(elec,tau) < 2.4 rad; not implemented yet)
  ),

  histManagers = cms.VPSet(
    muonHistManager,
    metHistManager,
    electronHistManager,
    vertexHistManager,
    triggerHistManager
 
  ),

  eventDumps = cms.VPSet(
    elecMuEventDump
  ),

  analysisSequence = elecMuAnalysisSequence
)

