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
	evtSelMetPt,
	evtSelTauLeadTrk,
	evtSelTauLeadTrkPt,
	evtSelTauTaNC,
	evtSelTauMuonVeto,
	evtSelTauElectronVeto,
        evtSelTauEcalCrackVeto,
        evtSelTauProng,
        evtSelTauCharge,
        evtSelCentralJetVeto,
	evtSelRecoilEnergyFromCaloTowers
#	evtSelRecoilEnergyFromJets,
#        evtSelMetTopology
    ),
  
    analyzers = cms.VPSet(
        tauHistManager,
        metHistManager,
        jetHistManager,
        vertexHistManager,
	tauRecoilEnergyFromJetsHistManager,
        tauRecoilEnergyFromCaloTowersHistManager,
        metTopologyHistManager,
        tauNuCandidateHistManager
    ),

    eventDumps = cms.VPSet(
        wTauNuEventDump
    ),
   
    analysisSequence = wTauNuAnalysisSequence
)
