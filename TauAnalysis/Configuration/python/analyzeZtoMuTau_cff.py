import FWCore.ParameterSet.Config as cms

# import config for event selection, event print-out and analysis sequence
from TauAnalysis.Configuration.analyzeZtoMuTau_cfi import *

analyzeZtoMuTau = cms.EDAnalyzer("GenericAnalyzer",
  
  name = cms.string('zMuTauAnalyzer'), 
                            
  eventSelection = cms.VPSet(
    # generator level selection of Z --> mu + tau-jet events
    # passing basic acceptance and kinematic cuts
    # (NOTE: to be used for efficiency studies only !!)
    #genMuonCut,
    #genTauCut,
    
    # trigger selection
    Trigger,

    # primary event vertex selection
    primaryEventVertex,
    primaryEventVertexQuality,
    primaryEventVertexPosition,
    
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
        
    # tau candidate selection
    tauAntiOverlapWithMuonsVeto,
    tauEtaCut,
    tauPtCut,
    tauLeadTrkCut,
    tauLeadTrkPtCut,
    tauTrkIsoCut,
    tauEcalIsoCut,
    tauProngCut,
    tauMuonVeto,

    # di-tau candidate selection
    diTauCandidateForMuTauAntiOverlapVeto,
    diTauCandidateForMuTauAcoplanarityCut,
    diTauCandidateForMuTauZeroChargeCut
  ),
  
  histManagers = cms.VPSet(
    muonHistManager,
    tauHistManager,
    diTauCandidateHistManagerForMuTau
  ),

  eventDumps = cms.VPSet(
    muTauEventDump
  ),
   
  analysisSequence = muTauAnalysisSequence
)
