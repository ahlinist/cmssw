import FWCore.ParameterSet.Config as cms

from TauAnalysis.RecoTools.eventSelFlagProdConfigurator import *
from TauAnalysis.CandidateTools.sysErrDefinitions_cfi import *

#--------------------------------------------------------------------------------
# define event selection criteria for Z --> mu + tau-jet channel
#--------------------------------------------------------------------------------

# trigger selection
cfgTrigger = cms.PSet(
    pluginName = cms.string('Trigger'),
    pluginType = cms.string('TriggerResultEventSelector'),
    src = cms.InputTag('TriggerResults::HLT'),
    triggerPaths = cms.vstring('HLT_Mu9')
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
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1MuonsGlobalCumulative'),
    src_individual = cms.InputTag('selectedLayer1MuonsGlobalIndividual'),
    systematics = cms.vstring(muonSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgMuonEtaCut = cms.PSet(
    pluginName = cms.string('muonEtaCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1MuonsEta21Cumulative'),
    src_individual = cms.InputTag('selectedLayer1MuonsEta21Individual'),
    systematics = cms.vstring(muonSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgMuonPtCut = cms.PSet(
    pluginName = cms.string('muonPtCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1MuonsPt15Cumulative'),
    src_individual = cms.InputTag('selectedLayer1MuonsPt15Individual'),
    systematics = cms.vstring(muonSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgMuonTrkIsoCut = cms.PSet(
    pluginName = cms.string('muonTrkIsoCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1MuonsTrkIsoCumulative'),
    src_individual = cms.InputTag('selectedLayer1MuonsTrkIsoIndividual'),
    systematics = cms.vstring(muonSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgMuonEcalIsoCut = cms.PSet(
    pluginName = cms.string('muonEcalIsoCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1MuonsEcalIsoCumulative'),
    src_individual = cms.InputTag('selectedLayer1MuonsEcalIsoIndividual'),
    systematics = cms.vstring(muonSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgMuonAntiPionCut = cms.PSet(
    pluginName = cms.string('muonAntiPionCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1MuonsPionVetoCumulative'),
    src_individual = cms.InputTag('selectedLayer1MuonsPionVetoIndividual'),
    systematics = cms.vstring(muonSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgMuonTrkIPcut = cms.PSet(
    pluginName = cms.string('muonTrkIPcut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1MuonsTrkIPcumulative'),
    src_individual = cms.InputTag('selectedLayer1MuonsTrkIPindividual'),
    systematics = cms.vstring(muonSystematics.keys()),
    minNumber = cms.uint32(1)
)

# tau candidate selection
cfgTauAntiOverlapWithMuonsVeto = cms.PSet(
    pluginName = cms.string('tauAntiOverlapWithMuonsVeto'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1TausForMuTauAntiOverlapWithMuonsVetoCumulative'),
    src_individual = cms.InputTag('selectedLayer1TausForMuTauAntiOverlapWithMuonsVetoIndividual'),
    systematics = cms.vstring(tauSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgTauEtaCut = cms.PSet(
    pluginName = cms.string('tauEtaCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1TausForMuTauEta21Cumulative'),
    src_individual = cms.InputTag('selectedLayer1TausEta21Individual'),
    systematics = cms.vstring(tauSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgTauPtCut = cms.PSet(
    pluginName = cms.string('tauPtCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1TausForMuTauPt20Cumulative'),
    src_individual = cms.InputTag('selectedLayer1TausPt20Individual'),
    systematics = cms.vstring(tauSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgTauLeadTrkCut = cms.PSet(
    pluginName = cms.string('tauLeadTrkCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1TausForMuTauLeadTrkCumulative'),
    src_individual = cms.InputTag('selectedLayer1TausLeadTrkIndividual'),
    systematics = cms.vstring(tauSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgTauLeadTrkPtCut = cms.PSet(
    pluginName = cms.string('tauLeadTrkPtCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1TausForMuTauLeadTrkPtCumulative'),
    src_individual = cms.InputTag('selectedLayer1TausLeadTrkPtIndividual'),
    systematics = cms.vstring(tauSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgTauTrkIsoCut = cms.PSet(
    pluginName = cms.string('tauTrkIsoCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1TausForMuTauTrkIsoCumulative'),
    src_individual = cms.InputTag('selectedLayer1TausTrkIsoIndividual'),
    systematics = cms.vstring(tauSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgTauEcalIsoCut = cms.PSet(
    pluginName = cms.string('tauEcalIsoCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1TausForMuTauEcalIsoCumulative'),
    src_individual = cms.InputTag('selectedLayer1TausEcalIsoIndividual'),
    systematics = cms.vstring(tauSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgTauProngCut = cms.PSet(
    pluginName = cms.string('tauProngCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1TausForMuTauProngCumulative'),
    src_individual = cms.InputTag('selectedLayer1TausProngIndividual'),
    systematics = cms.vstring(tauSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgTauChargeCut = cms.PSet(
    pluginName = cms.string('tauChargeCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1TausForMuTauChargeCumulative'),
    src_individual = cms.InputTag('selectedLayer1TausChargeIndividual'),
    systematics = cms.vstring(tauSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgTauMuonVeto = cms.PSet(
    pluginName = cms.string('tauMuonVeto'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1TausForMuTauMuonVetoCumulative'),
    src_individual = cms.InputTag('selectedLayer1TausMuonVetoIndividual'),
    systematics = cms.vstring(tauSystematics.keys()),
    minNumber = cms.uint32(1)
)

# di-tau candidate selection
cfgDiTauCandidateForMuTauAntiOverlapVeto = cms.PSet(
    pluginName = cms.string('diTauCandidateForMuTauAntiOverlapVeto'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedMuTauPairsAntiOverlapVetoCumulative'),
    src_individual = cms.InputTag('selectedMuTauPairsAntiOverlapVetoIndividual'),
    systematics = cms.vstring(muTauPairSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgDiTauCandidateForMuTauZeroChargeCut = cms.PSet(
    pluginName = cms.string('diTauCandidateForMuTauZeroChargeCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedMuTauPairsZeroChargeCumulative'),
    src_individual = cms.InputTag('selectedMuTauPairsZeroChargeIndividual'),
    systematics = cms.vstring(muTauPairSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgDiTauCandidateForMuTauAcoplanarity12Cut = cms.PSet(
    pluginName = cms.string('diTauCandidateForMuTauAcoplanarity12Cut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedMuTauPairsAcoplanarity12Cumulative'),
    src_individual = cms.InputTag('selectedMuTauPairsAcoplanarity12Individual'),
    systematics = cms.vstring(muTauPairSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgDiTauCandidateForMuTauMt1METcut = cms.PSet(
    pluginName = cms.string('diTauCandidateForMuTauMt1METcut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedMuTauPairsMt1METcumulative'),
    src_individual = cms.InputTag('selectedMuTauPairsMt1METindividual'),
    systematics = cms.vstring(muTauPairSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgDiTauCandidateForMuTauPzetaDiffCut = cms.PSet(
    pluginName = cms.string('diTauCandidateForMuTauPzetaDiffCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedMuTauPairsPzetaDiffCumulative'),
    src_individual = cms.InputTag('selectedMuTauPairsPzetaDiffIndividual'),
    systematics = cms.vstring(muTauPairSystematics.keys()),
    minNumber = cms.uint32(1)
)

# veto events compatible with Z --> mu+ mu- hypothesis
# (based on reconstructed (visible) invariant mass of muon + muon pairs)
cfgDiMuPairZmumuHypothesisVeto = cms.PSet(
    pluginName = cms.string('diMuPairZmumuHypothesisVeto'),
    pluginType = cms.string('PATCandViewMaxEventSelector'),
    src = cms.InputTag('selectedDiMuPairZmumuHypotheses'),
    #systematics = cms.vstring(muonSystematics.keys()),
    maxNumber = cms.uint32(0)
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
      cfgTauChargeCut,
      cfgTauMuonVeto,
      cfgDiTauCandidateForMuTauAntiOverlapVeto,
      cfgDiTauCandidateForMuTauZeroChargeCut,
      cfgDiTauCandidateForMuTauAcoplanarity12Cut,
      cfgDiTauCandidateForMuTauMt1METcut,
      cfgDiTauCandidateForMuTauPzetaDiffCut,
      cfgDiMuPairZmumuHypothesisVeto ],
    boolEventSelFlagProducer = "BoolEventSelFlagProducer",
    pyModuleName = __name__
)

selectZtoMuTauEvents = zToMuTauEventSelConfigurator.configure()

isRecZtoMuTau = cms.EDProducer("BoolEventSelFlagProducer",
    pluginName = cms.string('isRecZtoMuTau'),
    pluginType = cms.string('MultiBoolEventSelFlagSelector'),
    flags = cms.VInputTag(
        cms.InputTag('Trigger'),
        cms.InputTag('primaryEventVertexPosition'),
        cms.InputTag('muonTrkIPcut', 'cumulative'),
        cms.InputTag('tauMuonVeto', 'cumulative'),
        cms.InputTag('diTauCandidateForMuTauPzetaDiffCut', 'cumulative'),
        cms.InputTag('diMuPairZmumuHypothesisVeto')
        
    )
)

selectZtoMuTauEvents._seq = selectZtoMuTauEvents._seq * isRecZtoMuTau

