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
    evtSelPFMetPt,
    evtSelMetPt,
    evtSelTauLeadTrk,
    evtSelTauLeadTrkPt,
    evtSelTauIso,
    evtSelTauTaNC,
    evtSelTauProng,
    evtSelTauCharge,
    evtSelTauMuonVeto,
    evtSelTauElectronVeto,
    evtSelTauEcalCrackVeto,
    evtSelCentralJetVeto,
    evtSelRecoilEnergyFromCaloTowers,
    evtSelMetTopology
    ),
  
    analyzers = cms.VPSet(
        tauHistManager,
        caloMEtHistManager,
        pfMEtHistManager,
        jetHistManager,
        vertexHistManager,
        tauRecoilEnergyFromCaloTowersHistManager,
#        tauRecoilEnergyFromCentralCaloTowersHistManager,
#        tauRecoilEnergyFromForwardCaloTowersHistManager,
        metTopologyHistManager,
        tauNuCandidateHistManager,
        caloEventShapeVarsHistManager,
        pfEventShapeVarsHistManager
    ),

    eventDumps = cms.VPSet(
        wTauNuEventDump
    ),
   
    analysisSequence = wTauNuAnalysisSequence
)
