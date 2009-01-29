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
    tauMuonVeto

    # require common event vertex for muon and tau-jet candidate
    #muTauEventVertex,
            
    # acoplanarity cut between muon and tau-jet candidate
    # ( dPhi(mu,tau) < 2.4 rad; not implemented yet)
  ),
  
  histManagers = cms.VPSet(
    muonHistManager,
    tauHistManager
  ),

  eventDumps = cms.VPSet(
    muTauEventDump
  ),
   
  analysisSequence = muTauAnalysisSequence
)
