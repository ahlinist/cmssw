import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.RecoTools.eventSelFlagProdConfigurator import *

#--------------------------------------------------------------------------------
# define event selection criteria for Z --> mu + tau-jet channel
#--------------------------------------------------------------------------------

# trigger selection
Trigger = cms.PSet(
    pluginName = cms.string('Trigger'),
    pluginType = cms.string('TriggerResultEventSelector'),
    src = cms.InputTag('TriggerResults::HLT'),
    triggerPaths = cms.vstring('HLT_Mu15', 'HLT_IsoMu11')
)

# primary event vertex selection
primaryEventVertex = cms.PSet(
    pluginName = cms.string('primaryEventVertex'),
    pluginType = cms.string('VertexMinEventSelector'),
    src = cms.InputTag('selectedPrimaryVertexHighestPtTrackSum'),
    minNumber = cms.uint32(1)
)
primaryEventVertexQuality = cms.PSet(
    pluginName = cms.string('primaryEventVertexQuality'),
    pluginType = cms.string('VertexMinEventSelector'),
    src = cms.InputTag('selectedPrimaryVertexQuality'),
    minNumber = cms.uint32(1)
)
primaryEventVertexPosition = cms.PSet(
    pluginName = cms.string('primaryEventVertexPosition'),
    pluginType = cms.string('VertexMinEventSelector'),
    src = cms.InputTag('selectedPrimaryVertexPosition'),
    minNumber = cms.uint32(1)
)

# muon candidate selection
globalMuonCut = cms.PSet(
    pluginName = cms.string('globalMuonCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1MuonsGlobalCumulative'),
    src_individual = cms.InputTag('selectedLayer1MuonsGlobalIndividual'),
    minNumber = cms.uint32(1)
)
muonEtaCut = cms.PSet(
    pluginName = cms.string('muonEtaCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1MuonsEta21Cumulative'),
    src_individual = cms.InputTag('selectedLayer1MuonsEta21Individual'),
    minNumber = cms.uint32(1)
)
muonPtCut = cms.PSet(
    pluginName = cms.string('muonPtCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1MuonsPt15Cumulative'),
    src_individual = cms.InputTag('selectedLayer1MuonsPt15Individual'),
    minNumber = cms.uint32(1)
)
muonTrkIsoCut = cms.PSet(
    pluginName = cms.string('muonTrkIsoCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1MuonsTrkIsoCumulative'),
    src_individual = cms.InputTag('selectedLayer1MuonsTrkIsoIndividual'),
    minNumber = cms.uint32(1)
)
muonEcalIsoCut = cms.PSet(
    pluginName = cms.string('muonEcalIsoCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1MuonsEcalIsoCumulative'),
    src_individual = cms.InputTag('selectedLayer1MuonsEcalIsoIndividual'),
    minNumber = cms.uint32(1)
)
muonAntiPionCut = cms.PSet(
    pluginName = cms.string('muonAntiPionCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1MuonsPionVetoCumulative'),
    src_individual = cms.InputTag('selectedLayer1MuonsPionVetoIndividual'),
    minNumber = cms.uint32(1)
)
muonTrkIPcut = cms.PSet(
    pluginName = cms.string('muonTrkIPcut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1MuonsTrkIPcumulative'),
    src_individual = cms.InputTag('selectedLayer1MuonsTrkIPindividual'),
    minNumber = cms.uint32(1)
)

# tau candidate selection
tauAntiOverlapWithMuonsVeto = cms.PSet(
    pluginName = cms.string('tauAntiOverlapWithMuonsVeto'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1TausForMuTauAntiOverlapWithMuonsVetoCumulative'),
    src_individual = cms.InputTag('selectedLayer1TausForMuTauAntiOverlapWithMuonsVetoIndividual'),
    minNumber = cms.uint32(1)
)
tauEtaCut = cms.PSet(
    pluginName = cms.string('tauEtaCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1TausForMuTauEta21Cumulative'),
    src_individual = cms.InputTag('selectedLayer1TausEta21Individual'),
    minNumber = cms.uint32(1)
)
tauPtCut = cms.PSet(
    pluginName = cms.string('tauPtCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1TausForMuTauPt20Cumulative'),
    src_individual = cms.InputTag('selectedLayer1TausPt20Individual'),
    minNumber = cms.uint32(1)
)
tauLeadTrkCut = cms.PSet(
    pluginName = cms.string('tauLeadTrkCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1TausForMuTauLeadTrkCumulative'),
    src_individual = cms.InputTag('selectedLayer1TausLeadTrkIndividual'),
    minNumber = cms.uint32(1)
)
tauLeadTrkPtCut = cms.PSet(
    pluginName = cms.string('tauLeadTrkPtCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1TausForMuTauLeadTrkPtCumulative'),
    src_individual = cms.InputTag('selectedLayer1TausLeadTrkPtIndividual'),
    minNumber = cms.uint32(1)
)
tauTrkIsoCut = cms.PSet(
    pluginName = cms.string('tauTrkIsoCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1TausForMuTauTrkIsoCumulative'),
    src_individual = cms.InputTag('selectedLayer1TausTrkIsoIndividual'),
    minNumber = cms.uint32(1)
)
tauEcalIsoCut = cms.PSet(
    pluginName = cms.string('tauEcalIsoCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1TausForMuTauEcalIsoCumulative'),
    src_individual = cms.InputTag('selectedLayer1TausEcalIsoIndividual'),
    minNumber = cms.uint32(1)
)
tauProngCut = cms.PSet(
    pluginName = cms.string('tauProngCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1TausForMuTauProngCumulative'),
    src_individual = cms.InputTag('selectedLayer1TausProngIndividual'),
    minNumber = cms.uint32(1)
)
tauMuonVeto = cms.PSet(
    pluginName = cms.string('tauMuonVeto'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1TausForMuTauMuonVetoCumulative'),
    src_individual = cms.InputTag('selectedLayer1TausMuonVetoIndividual'),
    minNumber = cms.uint32(1)
)

# di-tau candidate selection
diTauCandidateForMuTauAntiOverlapVeto = cms.PSet(
    pluginName = cms.string('diTauCandidateForMuTauAntiOverlapVeto'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedMuTauPairsAntiOverlapVetoCumulative'),
    src_individual = cms.InputTag('selectedMuTauPairsAntiOverlapVetoIndividual'),
    minNumber = cms.uint32(1)
)
diTauCandidateForMuTauZeroChargeCut = cms.PSet(
    pluginName = cms.string('diTauCandidateForMuTauZeroChargeCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedMuTauPairsZeroChargeCumulative'),
    src_individual = cms.InputTag('selectedMuTauPairsZeroChargeIndividual'),
    minNumber = cms.uint32(1)
)
diTauCandidateForMuTauMt1METcut = cms.PSet(
    pluginName = cms.string('diTauCandidateForMuTauMt1METcut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedMuTauPairsMt1METcumulative'),
    src_individual = cms.InputTag('selectedMuTauPairsMt1METindividual'),
    minNumber = cms.uint32(1)
)

# veto events containing additional central jets with Et > 20 GeV
#centralJetVeto = cms.PSet(
#    pluginName = cms.string('centralJetVeto'),
#    pluginType = cms.string('PATCandViewMaxEventSelector'),
#    src = cms.InputTag('selectedLayer1JetsEt20'),
#    maxNumber = cms.uint32(0)
#)

zToMuTauEventSelConfigurator = eventSelFlagProdConfigurator(
    [ Trigger,
      primaryEventVertex,
      primaryEventVertexQuality,
      primaryEventVertexPosition,
      globalMuonCut,
      muonEtaCut,
      muonPtCut,
      muonTrkIsoCut,
      muonEcalIsoCut,
      muonAntiPionCut,
      muonTrkIPcut,
      tauAntiOverlapWithMuonsVeto,
      tauEtaCut,
      tauPtCut,
      tauLeadTrkCut,
      tauLeadTrkPtCut,
      tauTrkIsoCut,
      tauEcalIsoCut,
      tauProngCut,
      tauMuonVeto,
      diTauCandidateForMuTauAntiOverlapVeto,
      diTauCandidateForMuTauZeroChargeCut,
      diTauCandidateForMuTauMt1METcut ],
    boolEventSelFlagProducer = "BoolEventSelFlagProducer",
    pyModuleName = __name__
)

selectZtoMuTauEvents = zToMuTauEventSelConfigurator.configure()
