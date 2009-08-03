import FWCore.ParameterSet.Config as cms

from TauAnalysis.RecoTools.eventSelFlagProdConfigurator import *

#--------------------------------------------------------------------------------
# define event selection criteria for Z --> mu + tau-jet channel
#--------------------------------------------------------------------------------

# trigger selection
cfgTrigger = cms.PSet(
    pluginName = cms.string('Trigger'),
    pluginType = cms.string('TriggerResultEventSelector'),
    src = cms.InputTag('TriggerResults::HLT'),
    triggerPaths = cms.vstring('HLT_Mu15', 'HLT_IsoMu11')
)

# primary event vertex selection
cfgPrimaryEventVertex = cms.PSet(
    pluginName = cms.string('primaryEventVertex'),
    pluginType = cms.string('VertexMinEventSelector'),
    src = cms.InputTag('selectedPrimaryVertexHighestPtTrackSum'),
    minNumber = cms.uint32(1)
)
cfgPrimaryEventVertexQuality = cms.PSet(
    pluginName = cms.string('primaryEventVertexQuality'),
    pluginType = cms.string('VertexMinEventSelector'),
    src = cms.InputTag('selectedPrimaryVertexQuality'),
    minNumber = cms.uint32(1)
)
cfgPrimaryEventVertexPosition = cms.PSet(
    pluginName = cms.string('primaryEventVertexPosition'),
    pluginType = cms.string('VertexMinEventSelector'),
    src = cms.InputTag('selectedPrimaryVertexPosition'),
    minNumber = cms.uint32(1)
)

# muon candidate selection
cfgGlobalMuonCut = cms.PSet(
    pluginName = cms.string('globalMuonCut'),
    pluginType = cms.string('PATCandViewCountEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1MuonsGlobalCumulative'),
    src_individual = cms.InputTag('selectedLayer1MuonsGlobalIndividual'),
    minNumber = cms.uint32(1),
    maxNumber = cms.uint32(1)
)
cfgMuonEtaCut = cms.PSet(
    pluginName = cms.string('muonEtaCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1MuonsEta21Cumulative'),
    src_individual = cms.InputTag('selectedLayer1MuonsEta21Individual'),
    minNumber = cms.uint32(1)
)
cfgMuonPtCut = cms.PSet(
    pluginName = cms.string('muonPtCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1MuonsPt15Cumulative'),
    src_individual = cms.InputTag('selectedLayer1MuonsPt15Individual'),
    minNumber = cms.uint32(1)
)
cfgMuonTrkIsoCut = cms.PSet(
    pluginName = cms.string('muonTrkIsoCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1MuonsTrkIsoCumulative'),
    src_individual = cms.InputTag('selectedLayer1MuonsTrkIsoIndividual'),
    minNumber = cms.uint32(1)
)
cfgMuonEcalIsoCut = cms.PSet(
    pluginName = cms.string('muonEcalIsoCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1MuonsEcalIsoCumulative'),
    src_individual = cms.InputTag('selectedLayer1MuonsEcalIsoIndividual'),
    minNumber = cms.uint32(1)
)
cfgMuonAntiPionCut = cms.PSet(
    pluginName = cms.string('muonAntiPionCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1MuonsPionVetoCumulative'),
    src_individual = cms.InputTag('selectedLayer1MuonsPionVetoIndividual'),
    minNumber = cms.uint32(1)
)
cfgMuonTrkIPcut = cms.PSet(
    pluginName = cms.string('muonTrkIPcut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1MuonsTrkIPcumulative'),
    src_individual = cms.InputTag('selectedLayer1MuonsTrkIPindividual'),
    minNumber = cms.uint32(1)
)

# tau candidate selection
cfgTauAntiOverlapWithMuonsVeto = cms.PSet(
    pluginName = cms.string('tauAntiOverlapWithMuonsVeto'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1TausForMuTauAntiOverlapWithMuonsVetoCumulative'),
    src_individual = cms.InputTag('selectedLayer1TausForMuTauAntiOverlapWithMuonsVetoIndividual'),
    minNumber = cms.uint32(1)
)
cfgTauEtaCut = cms.PSet(
    pluginName = cms.string('tauEtaCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1TausForMuTauEta21Cumulative'),
    src_individual = cms.InputTag('selectedLayer1TausEta21Individual'),
    minNumber = cms.uint32(1)
)
cfgTauPtCut = cms.PSet(
    pluginName = cms.string('tauPtCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1TausForMuTauPt20Cumulative'),
    src_individual = cms.InputTag('selectedLayer1TausPt20Individual'),
    minNumber = cms.uint32(1)
)
cfgTauLeadTrkCut = cms.PSet(
    pluginName = cms.string('tauLeadTrkCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1TausForMuTauLeadTrkCumulative'),
    src_individual = cms.InputTag('selectedLayer1TausLeadTrkIndividual'),
    minNumber = cms.uint32(1)
)
cfgTauLeadTrkPtCut = cms.PSet(
    pluginName = cms.string('tauLeadTrkPtCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1TausForMuTauLeadTrkPtCumulative'),
    src_individual = cms.InputTag('selectedLayer1TausLeadTrkPtIndividual'),
    minNumber = cms.uint32(1)
)
cfgTauTrkIsoCut = cms.PSet(
    pluginName = cms.string('tauTrkIsoCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1TausForMuTauTrkIsoCumulative'),
    src_individual = cms.InputTag('selectedLayer1TausTrkIsoIndividual'),
    minNumber = cms.uint32(1)
)
cfgTauEcalIsoCut = cms.PSet(
    pluginName = cms.string('tauEcalIsoCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1TausForMuTauEcalIsoCumulative'),
    src_individual = cms.InputTag('selectedLayer1TausEcalIsoIndividual'),
    minNumber = cms.uint32(1)
)
cfgTauProngCut = cms.PSet(
    pluginName = cms.string('tauProngCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1TausForMuTauProngCumulative'),
    src_individual = cms.InputTag('selectedLayer1TausProngIndividual'),
    minNumber = cms.uint32(1)
)
cfgTauMuonVeto = cms.PSet(
    pluginName = cms.string('tauMuonVeto'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1TausForMuTauMuonVetoCumulative'),
    src_individual = cms.InputTag('selectedLayer1TausMuonVetoIndividual'),
    minNumber = cms.uint32(1)
)

# di-tau candidate selection
cfgDiTauCandidateForMuTauAntiOverlapVeto = cms.PSet(
    pluginName = cms.string('diTauCandidateForMuTauAntiOverlapVeto'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedMuTauPairsAntiOverlapVetoCumulative'),
    src_individual = cms.InputTag('selectedMuTauPairsAntiOverlapVetoIndividual'),
    minNumber = cms.uint32(1)
)
cfgDiTauCandidateForMuTauZeroChargeCut = cms.PSet(
    pluginName = cms.string('diTauCandidateForMuTauZeroChargeCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedMuTauPairsZeroChargeCumulative'),
    src_individual = cms.InputTag('selectedMuTauPairsZeroChargeIndividual'),
    minNumber = cms.uint32(1)
)
cfgDiTauCandidateForMuTauAcoplanarity12Cut = cms.PSet(
    pluginName = cms.string('diTauCandidateForMuTauAcoplanarity12Cut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedMuTauPairsAcoplanarity12Cumulative'),
    src_individual = cms.InputTag('selectedMuTauPairsAcoplanarity12Individual'),
    minNumber = cms.uint32(1)
)
cfgDiTauCandidateForMuTauMt1METcut = cms.PSet(
    pluginName = cms.string('diTauCandidateForMuTauMt1METcut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedMuTauPairsMt1METcumulative'),
    src_individual = cms.InputTag('selectedMuTauPairsMt1METindividual'),
    minNumber = cms.uint32(1)
)
cfgDiTauCandidateForMuTauPzetaDiffCut = cms.PSet(
    pluginName = cms.string('diTauCandidateForMuTauPzetaDiffCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedMuTauPairsPzetaDiffCumulative'),
    src_individual = cms.InputTag('selectedMuTauPairsPzetaDiffIndividual'),
    minNumber = cms.uint32(1)
)

zToMuTauEventSelConfigurator = eventSelFlagProdConfigurator(
    [ cfgTrigger,
      cfgPrimaryEventVertex,
      cfgPrimaryEventVertexQuality,
      cfgPrimaryEventVertexPosition,
      cfgGlobalMuonCut,
      cfgMuonEtaCut,
      cfgMuonPtCut,
      cfgTauAntiOverlapWithMuonsVeto,
      cfgTauEtaCut,
      cfgTauPtCut,
      cfgMuonTrkIsoCut,
      cfgMuonEcalIsoCut,
      cfgMuonAntiPionCut,
      cfgMuonTrkIPcut,
      cfgTauLeadTrkCut,
      cfgTauLeadTrkPtCut,
      cfgTauTrkIsoCut,
      cfgTauEcalIsoCut,
      cfgTauProngCut,
      cfgTauMuonVeto,
      cfgDiTauCandidateForMuTauAntiOverlapVeto,
      cfgDiTauCandidateForMuTauZeroChargeCut,
      cfgDiTauCandidateForMuTauAcoplanarity12Cut,
      cfgDiTauCandidateForMuTauMt1METcut,
      cfgDiTauCandidateForMuTauPzetaDiffCut ],
    boolEventSelFlagProducer = "BoolEventSelFlagProducer",
    pyModuleName = __name__
)

selectZtoMuTauEvents = zToMuTauEventSelConfigurator.configure()
