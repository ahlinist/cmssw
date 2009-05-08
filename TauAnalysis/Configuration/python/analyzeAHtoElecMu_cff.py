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
    evtSelTrigger,
    
    # primary event vertex selection
    evtSelPrimaryEventVertex,
    evtSelPrimaryEventVertexQuality,
    evtSelPrimaryEventVertexPosition,

    # electron candidate selection
    evtSelElectronIdMin,
    #evtSelElectronIdMax,
    evtSelElectronAntiCrack,
    evtSelElectronEta, #default 2.1, switch to 2.4
    evtSelElectronPt, #now 15
    evtSelElectronIso,
    #evtSelElectronTrkIso,
    #evtSelElectronEcalIso,
    #evtSelElectronHcalIso,
    evtSelElectronTrk,
    #evtSelElectronTrkIP, #mmm... we used to cut on combined d0 significance!

    # muon candidate selection
    evtSelGlobalMuonMin,
    #evtSelGlobalMuonMax,
    evtSelMuonEta, #default 2.1, switch to 2.4
    evtSelMuonPt, #now 15
    evtSelMuonIso,
    #evtSelMuonTrkIso,
    #evtSelMuonEcalIso,
    #evtSelMuonHcalIso, #why not?
    evtSelMuonAntiPion,
    #evtSelMuonTrkIP, #mmm... we used to cut on combined d0 significance!

    # di-tau candidate selection
    evtSelDiTauCandidateForElecMuZeroCharge,
    evtSelDiTauCandidateForElecMuAcoplanarity, #use it in case of no collinear approximation?
    #evtSelDiTauCandidateForElecMuDPhi,
    #evtSelDiTauCandidateForElecMuImpParamSig,

    # jet candidate selection...
    evtSelJetMin, #default eta<2.1 too tight, need 2.4 for b-tagging
    evtSelJetMax,    
    # b-tagging candidate selection...
    evtSelJetBtagMin,
    evtSelJetBtagMax
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

