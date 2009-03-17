import FWCore.ParameterSet.Config as cms

# import config for event selection, event print-out and analysis sequence
from TauAnalysis.Configuration.analyzeZtoMuTau_factorized_cfi import *

analyzeZtoMuTau_factorizedWithoutMuonIsolation = cms.EDAnalyzer("GenericAnalyzer",
  
  name = cms.string('zMuTauAnalyzer_factorizedWithoutMuonIsolation'), 
                            
  eventSelection = cms.VPSet(
    # generator level phase-space selection
    # (NOTE: (1) to be used in case of Monte Carlo samples
    #            overlapping in simulated phase-space only !!
    #        (2) genPhaseSpaceCut needs to be **always** the first entry in the list of cuts
    #           - otherwise the script submitToBatch.csh for submission of cmsRun jobs
    #            to the CERN batch system will not work !!)
    genPhaseSpaceCut,
    
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
    muonTrkIsoCutLooseMuonIsolation,
    muonEcalIsoCutLooseMuonIsolation,
    muonAntiPionCutLooseMuonIsolation,
    muonTrkIPcutLooseMuonIsolation,
        
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
    diTauCandidateForMuTauAntiOverlapVetoLooseMuonIsolation,
    diTauCandidateForMuTauAcoplanarityCutLooseMuonIsolation,
    diTauCandidateForMuTauZeroChargeCutLooseMuonIsolation,
    diTauCandidateForMuTauMt1METCutLooseMuonIsolation,
  ),
  
  histManagers = cms.VPSet(
    genPhaseSpaceEventInfoHistManager,
    muonHistManager,
    tauHistManager,
    diTauCandidateHistManagerForMuTau,
    metHistManager,
    jetHistManager,
    allPFCandidateHistManager,
    pfChargedHadronHistManager,
    pfGammaHistManager,
    pfNeutralHadronHistManager,
    vertexHistManager,
    triggerHistManager
  ),

  analysisSequence = muTauAnalysisSequence_factorizedWithoutMuonIsolation
)

analyzeZtoMuTau_factorizedWithMuonIsolation = cms.EDAnalyzer("GenericAnalyzer",
  
  name = cms.string('zMuTauAnalyzer_factorizedWithMuonIsolation'), 
                            
  eventSelection = cms.VPSet(
    # generator level phase-space selection
    # (NOTE: (1) to be used in case of Monte Carlo samples
    #            overlapping in simulated phase-space only !!
    #        (2) genPhaseSpaceCut needs to be **always** the first entry in the list of cuts
    #           - otherwise the script submitToBatch.csh for submission of cmsRun jobs
    #            to the CERN batch system will not work !!)
    genPhaseSpaceCut,
    
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
    muonAntiPionCut,
    muonTrkIPcut
  ),
  
  histManagers = cms.VPSet(
    genPhaseSpaceEventInfoHistManager,
    muonHistManager,
    vertexHistManager,
    triggerHistManager
  ),

  eventDumps = cms.VPSet(
    muTauEventDump_factorizedWithMuonIsolation
  ),                                                           
   
  analysisSequence = muTauAnalysisSequence_factorizedWithMuonIsolation
)


