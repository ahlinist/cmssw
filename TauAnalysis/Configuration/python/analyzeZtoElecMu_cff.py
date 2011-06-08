import FWCore.ParameterSet.Config as cms

# import config for event selection, event print-out and analysis sequence
from TauAnalysis.Configuration.analyzeZtoElecMu_cfi import *

# define auxiliary service
# for handling of systematic uncertainties
from TauAnalysis.CandidateTools.sysErrDefinitions_cfi import *
SysUncertaintyService = cms.Service("SysUncertaintyService",
    weights = getSysUncertaintyParameterSets(
        [ electronSystematics,
          muonSystematics,
          theorySystematics ]
    )
)

analyzeZtoElecMuEvents = cms.EDAnalyzer("GenericAnalyzer",

    name = cms.string('zElecMuAnalyzer'),

    filters = cms.VPSet(
        # generator level phase-space selection
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
        evtSelElectronIdMax,
        evtSelElectronAntiCrack,
        evtSelElectronEta, #default 2.1, switch to 2.4
        evtSelElectronPt, #now 10
        evtSelElectronIso,
        evtSelElectronTrk,
        #evtSelElectronTrkIP, #mmm... we used to cut on combined d0 significance!
        
        # muon candidate selection
        evtSelGlobalMuonMin,
        evtSelGlobalMuonMax,
        evtSelMuonEta, #default 2.1, switch to 2.4
        evtSelMuonPt, #now 10
        evtSelMuonIso,
        evtSelMuonAntiPion,
        #evtSelMuonTrkIP, #mmm... we used to cut on combined d0 significance!
        
        # di-tau candidate selection
        evtSelDiTauCandidateForElecMuDRmin,
        evtSelDiTauCandidateForElecMuZeroCharge,
        evtSelDiTauCandidateForElecMuMt1MET,
        evtSelDiTauCandidateForElecMuMt2MET,
        evtSelDiTauCandidateForElecMuDPhi,
        #evtSelDiTauCandidateForElecMuOneLegPt,
        
        # met selection...
        evtSelMETMax,
        
        # jet candidate selection...
        evtSelJetMin, #default eta<2.1 too tight, need 2.4 for b-tagging
        evtSelJetMax,    
        # b-tagging candidate selection...
        evtSelJetBtagMin,
        evtSelJetBtagMax
        ),
                                        
    analyzers = cms.VPSet(
        genPhaseSpaceEventInfoHistManager,
        eventWeightHistManager,
        electronHistManager,
        muonHistManager,
        tauHistManager,
        diTauCandidateHistManagerForElecMu,
        diTauCandidateNSVfitHistManagerForElecMu,
        diTauCandidateZmumuHypothesisHistManagerForElecMu,
        jetHistManager,
        caloMEtHistManager,
        pfMEtHistManager,
        particleMultiplicityHistManager,
        vertexHistManager,
        triggerHistManagerForElecMu
    ),

    analyzers_systematic = cms.VPSet(),                                    

    eventDumps = cms.VPSet(
        elecMuEventDump
    ),

    analysisSequence = elecMuAnalysisSequence,

    #estimateSysUncertainties = cms.bool(True),                                       
    estimateSysUncertainties = cms.bool(False), 
    systematics = cms.vstring(
        getSysUncertaintyNames(
            [ electronSystematics,
              muonSystematics,
              theorySystematics ]
        )
    )                                        
)

