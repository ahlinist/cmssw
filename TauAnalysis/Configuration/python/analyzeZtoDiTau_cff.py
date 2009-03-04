import FWCore.ParameterSet.Config as cms

# import config for event selection, event print-out and analysis sequence
from TauAnalysis.Configuration.analyzeZtoDiTau_cfi import *

analyzeZtoDiTau = cms.EDAnalyzer("GenericAnalyzer",
  
  name = cms.string('zDiTauAnalyzer'), 
                            
  eventSelection = cms.VPSet(
    # generator level phase-space selection
    # (NOTE: (1) to be used in case of Monte Carlo samples
    #            overlapping in simulated phase-space only !!
    #        (2) genPhaseSpaceCut needs to be **always** the first entry in the list of cuts
    #           - otherwise the script submitToBatch.csh for submission of cmsRun jobs
    #            to the CERN batch system will not work !!)
    genPhaseSpaceCut,
    
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
    diTauCandidateForDiTauZeroChargeCut,

    # veto events containing additional central jets with Et > 20 GeV
    centralJetVeto
  ),
  
  histManagers = cms.VPSet(
    genPhaseSpaceEventInfoHistManager,
    tauHistManager,
    diTauCandidateHistManagerForDiTau,
    metHistManager,
    jetHistManager,
    vertexHistManager,
    triggerHistManager
  ),

  eventDumps = cms.VPSet(
    diTauEventDump
  ),
   
  analysisSequence = diTauAnalysisSequence
)
