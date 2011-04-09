import FWCore.ParameterSet.Config as cms

cutFlowHistManager = cms.PSet(
    pluginName = cms.string('cutFlowHistManager'),
    pluginType = cms.string('CutFlowHistManager'),
      
    srcEventSelFlags = cms.VInputTag(
        cms.InputTag('Trigger'),
        cms.InputTag('dataQualityCutsPassed'),
        cms.InputTag('globalMuonCut', 'cumulative'),
        cms.InputTag('muonEtaCut', 'cumulative'),
        cms.InputTag('muonPtCut', 'cumulative'),
        cms.InputTag('tauAntiOverlapWithMuonsVeto', 'cumulative'),
        cms.InputTag('tauEtaCut', 'cumulative'),
        cms.InputTag('tauPtCut', 'cumulative'),        
        cms.InputTag('muonVbTfIdCut', 'cumulative'),
        cms.InputTag('muonPFRelIsoCut', 'cumulative'),
        cms.InputTag('muonTrkIPcut', 'cumulative'),
        cms.InputTag('tauLeadTrkCut', 'cumulative'),
        cms.InputTag('tauLeadTrkPtCut', 'cumulative'),
        cms.InputTag('tauTaNCdiscrCut', 'cumulative'),
        cms.InputTag('tauProngCut', 'cumulative'),
        cms.InputTag('tauChargeCut', 'cumulative'),
        cms.InputTag('tauMuonVeto', 'cumulative'),
        cms.InputTag('tauElectronVeto', 'cumulative'),
        cms.InputTag('diTauCandidateForMuTauAntiOverlapVeto', 'cumulative'),
        cms.InputTag('primaryEventVertexForMuTau'),
        cms.InputTag('primaryEventVertexQualityForMuTau'),
        cms.InputTag('primaryEventVertexPositionForMuTau'),
        cms.InputTag('diTauCandidateForMuTauMt1METcut', 'cumulative'),
        cms.InputTag('diTauCandidateForMuTauPzetaDiffCut', 'cumulative'),
        cms.InputTag('diMuPairZmumuHypothesisVetoByLooseIsolation'),
        cms.InputTag('diMuPairDYmumuHypothesisVeto'),
        cms.InputTag('diTauCandidateForMuTauZeroChargeCut', 'cumulative')
    ),
    
    srcGenPileUp = cms.InputTag('addPileupInfo'),
    srcRecVertices = cms.InputTag('offlinePrimaryVerticesWithBS'),

    firstBin = cms.uint32(0),
    lastBin = cms.uint32(20),

    dqmDirectory_store = cms.string('CutFlowStatistics')
)
