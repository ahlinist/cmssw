import FWCore.ParameterSet.Config as cms
from TauAnalysis.Configuration.analyzeWtoTauNu_cfi import *

analyzeWtoTauNuEvents = cms.EDAnalyzer("GenericAnalyzer",
  
    name = cms.string('wTauNuAnalyzer'), 
                            
    filters = cms.VPSet(
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
	evtSelRecoilEnergyFromCaloTowers
    ),
  
    analyzers = cms.VPSet(
        tauHistManager,
        metHistManager,
        jetHistManager,
        vertexHistManager,
	tauRecoilEnergyFromJetsHistManager,
        tauRecoilEnergyFromCaloTowersHistManager,
        metTopologyHistManager,
        tauNuCandidateHistManager,
        muonHistManager,
        electronHistManager,
        caloEventShapeVarsHistManager,
        pfEventShapeVarsHistManager
    ),

    eventDumps = cms.VPSet(
        wTauNuEventDump
    ),
   
    analysisSequence = wTauNuAnalysisSequence
)
