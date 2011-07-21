import FWCore.ParameterSet.Config as cms
from TauAnalysis.Configuration.analyzeWtoTauNu_cfi import *

from TauAnalysis.CandidateTools.sysErrDefinitions_cfi import *
sysUncertaintyService = cms.Service("SysUncertaintyService",
                                   weights = getSysUncertaintyParameterSets(
    [ theorySystematics ]
    ),
       sources = cms.PSet(
       isRecWtoTauNu = cms.vstring(
           "sysTau*","",
           "sysJet*",""
           )
       )
                                    )
analyzeWtoTauNuEvents = cms.EDAnalyzer("GenericAnalyzer",
  
    name = cms.string('wTauNuAnalyzer'), 
                            
    filters = cms.VPSet(
    #trigger selection
    evtSelTrigger,
    
    #vertex selection
    evtSelPrimaryEventVertex,
    evtSelPrimaryEventVertexQuality,
    evtSelPrimaryEventVertexPosition,
    
    #wtaunu specific selection  
    evtSelTauEta,
    evtSelTauPt,
    evtSelTrkVertex,
    evtSelTauLeadTrk,
    evtSelTauLeadTrkPt,
    evtSelTauMuonVeto,
    evtSelMuonVeto,
    evtSelTauElectronVeto,
    evtSelTauEmFraction,
    evtSelElectronVeto,
    evtSelTauIso,
    evtSelTauProng,
    evtSelTauCharge,
    evtSelTauEcalCrackVeto,
    evtSelPFMetPt,
    evtSelHtRatio,

    evtSelTrigger2
#    evtSelMetTopology
    ),
  
    analyzers = cms.VPSet(
        tauHistManager,
        pfMEtHistManager,
        jetHistManager,
        vertexHistManager,
        tauRecoilEnergyFromCaloTowersHistManager,
        htRatioHistManager,
        metTopologyHistManager,
        tauNuCandidateHistManager,
        muonHistManager,
        electronHistManager,
        dataBinner,
        modelBinnerForWTauNuGenTauNuPairAcc,
        modelBinnerForWTauNuWrtGenTauNuPairAcc
    ),

    analyzers_systematic = cms.VPSet(
        sysUncertaintyBinnerForWTauNuAcc,
        sysUncertaintyBinnerForWTauNuEff,
        sysUncertaintyHistManagerForWTauNu
    ),
    eventDumps = cms.VPSet(
        wTauNuEventDump
    ),
   
    analysisSequence = wTauNuAnalysisSequence,
    estimateSysUncertainties = cms.bool(False),
    systematics = cms.vstring(
       getSysUncertaintyNames(
              [ tauSystematics,
                jetSystematics,
                metSystematicsForWtoTauNu,
                htRatioSystematics,
                tauNuPairSystematics,
                metTopologySystematics,
                theorySystematics
                ]
              )
       )
                                       ) 
