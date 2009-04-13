import FWCore.ParameterSet.Config as cms

# import config for event selection, event print-out and analysis sequence
from TauAnalysis.Configuration.analyzeZtoMuTau_cfi import *

analyzeZtoMuTau = cms.EDAnalyzer("GenericAnalyzer",
  
  name = cms.string('zMuTauAnalyzer'), 
                            
  eventSelection = cms.VPSet(
    # generator level phase-space selection
    # (NOTE: (1) to be used in case of Monte Carlo samples
    #            overlapping in simulated phase-space only !!
    #        (2) genPhaseSpaceCut needs to be **always** the first entry in the list of cuts
    #           - otherwise the script submitToBatch.csh for submission of cmsRun jobs
    #            to the CERN batch system will not work !!)
    genPhaseSpaceCut,
    
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
    muonTrkIsoCut,
    muonEcalIsoCut,
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
    diTauCandidateForMuTauZeroChargeCut,
    diTauCandidateForMuTauMt1METCut,

    # veto events containing additional central jets with Et > 20 GeV
    #centralJetVeto
  ),
  
  histManagers = cms.VPSet(
    genPhaseSpaceEventInfoHistManager,
    muonHistManager,
    tauHistManager,
    diTauCandidateHistManagerForMuTau,
    metHistManager,
    jetHistManager,
    vertexHistManager,
    triggerHistManager
  ),

  eventDumps = cms.VPSet(
    muTauEventDump
  ),
   
  analysisSequence = muTauAnalysisSequence
)
