import FWCore.ParameterSet.Config as cms
from TauAnalysis.Configuration.analyzeWtoTauNu_cfi import *

# define auxiliary service
# for handling of systematic uncertainties
from TauAnalysis.CandidateTools.sysErrDefinitions_cfi import *
SysUncertaintyService = cms.Service("SysUncertaintyService",
    weights = getSysUncertaintyParameterSets(
        [ muonSystematics,
          tauSystematics,
          theorySystematics ]
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
    evtSelMetTopology
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
        electronHistManager
    ),

    eventDumps = cms.VPSet(
        wTauNuEventDump
    ),
   
    analysisSequence = wTauNuAnalysisSequence
)
