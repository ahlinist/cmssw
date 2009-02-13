import FWCore.ParameterSet.Config as cms

# import config for event selection, event print-out and analysis sequence
from TauAnalysis.Configuration.analyzeZtoDiTau_cfi import *

analyzeZtoDiTau = cms.EDAnalyzer("GenericAnalyzer",
  
  name = cms.string('zDiTauAnalyzer'), 
                            
  eventSelection = cms.VPSet(
    # generator level selection of pure hadronic Z --> tau-jet + tau-jet events
    # passing basic acceptance and kinematic cuts
    # (NOTE: to be used for efficiency studies only !!)
    #genDiTauCut,
    
    # trigger selection
    #Trigger,

    # primary event vertex selection
    primaryEventVertex,
    primaryEventVertexQuality,
    primaryEventVertexPosition,

    # selection of first tau-jet candidate
    firstTauEtaCut,
    firstTauPtCut,
    firstTauLeadTrkCut,
    firstTauLeadTrkPtCut,
    firstTauTrkIsoCut,
    firstTauEcalIsoCut,
    firstTauProngCut,

    # selection of second tau-jet candidate
    secondTauEtaCut,
    secondTauPtCut,
    secondTauLeadTrkCut,
    secondTauLeadTrkPtCut,
    secondTauTrkIsoCut,
    secondTauEcalIsoCut,
    secondTauProngCut,

    # di-tau candidate selection
    diTauCandidateForDiTauAntiOverlapVeto,
    diTauCandidateForDiTauAcoplanarityCut,
    diTauCandidateForDiTauZeroChargeCut
  ),
  
  histManagers = cms.VPSet(
    tauHistManager,
    diTauCandidateHistManagerForDiTau,
    metHistManager,
    vertexHistManager,
    triggerHistManager
  ),

  eventDumps = cms.VPSet(
    diTauEventDump
  ),
   
  analysisSequence = diTauAnalysisSequence
)
