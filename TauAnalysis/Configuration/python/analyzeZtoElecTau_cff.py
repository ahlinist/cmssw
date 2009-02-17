import FWCore.ParameterSet.Config as cms

# import config for event selection, event print-out and analysis sequence
from TauAnalysis.Configuration.analyzeZtoElecTau_cfi import *

analyzeZtoElecTau = cms.EDAnalyzer("GenericAnalyzer",
  
  name = cms.string('zElecTauAnalyzer'), 
                            
  eventSelection = cms.VPSet(
    # generator level selection of Z --> e + tau-jet events
    # passing basic acceptance and kinematic cuts
    # (NOTE: to be used for efficiency studies only !!)
    #genElectronCut,
    #genTauCut,
    
    # trigger selection
    #Trigger,

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

    # tau candidate selection
    tauAntiOverlapWithElectronsVeto,
    tauEtaCut,
    tauPtCut,
    tauLeadTrkCut,
    tauLeadTrkPtCut,
    tauTrkIsoCut,
    tauEcalIsoCut,
    tauProngCut,
    tauElectronVeto,

    # di-tau candidate selection
    diTauCandidateForElecTauAntiOverlapVeto,
    diTauCandidateForElecTauAcoplanarityCut,
    diTauCandidateForElecTauZeroChargeCut,

    # veto events containing additional central jets with Et > 20 GeV
    centralJetVeto
  ),
  
  histManagers = cms.VPSet(
    electronHistManager,
    tauHistManager,
    diTauCandidateHistManagerForElecTau,
    metHistManager,
    vertexHistManager,
    triggerHistManager
  ),

  eventDumps = cms.VPSet(
    elecTauEventDump
  ),
   
  analysisSequence = elecTauAnalysisSequence
)
