import FWCore.ParameterSet.Config as cms

from TauAnalysis.RecoTools.tools.eventSelFlagProdConfigurator import *
from TauAnalysis.CandidateTools.sysErrDefinitions_cfi import *

#--------------------------------------------------------------------------------
# define event selection criteria for Z --> mu + tau-jet channel
#--------------------------------------------------------------------------------

# generator level phase-space selection
# (NOTE: to be used in case of Monte Carlo samples
#        overlapping in simulated phase-space only !!)
cfgGenPhaseSpaceCut = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)

# trigger selection
cfgTrigger = cms.PSet(
    pluginName = cms.string('Trigger'),
    pluginType = cms.string('TriggerResultEventSelector'),
    src = cms.InputTag('TriggerResults::HLT'),
    triggerPaths = cms.vstring(
      'HLT_Mu9',
      'HLT_Mu11',
      'HLT_IsoMu9'
    )
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
    src_cumulative = cms.InputTag('selectedPatMuonsGlobalCumulative'),
    src_individual = cms.InputTag('selectedPatMuonsGlobalIndividual'),
    systematics = cms.vstring(muonSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgMuonEtaCut = cms.PSet(
    pluginName = cms.string('muonEtaCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatMuonsEta21Cumulative'),
    src_individual = cms.InputTag('selectedPatMuonsEta21Individual'),
    systematics = cms.vstring(muonSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgMuonPtCut = cms.PSet(
    pluginName = cms.string('muonPtCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatMuonsPt10Cumulative'),
    src_individual = cms.InputTag('selectedPatMuonsPt10Individual'),
    systematics = cms.vstring(muonSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgMuonVbTfIdCut = cms.PSet(
    pluginName = cms.string('muonVbTfIdCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatMuonsVbTfIdCumulative'),
    src_individual = cms.InputTag('selectedPatMuonsVbTfIdIndividual'),
    systematics = cms.vstring(muonSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgMuonPFRelIsoCut = cms.PSet(
    pluginName = cms.string('muonPFRelIsoCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatMuonsPFRelIsoCumulative'),
    src_individual = cms.InputTag('selectedPatMuonsPFRelIsoIndividual'),
    systematics = cms.vstring(muonSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgMuonAntiPionCut = cms.PSet(
    pluginName = cms.string('muonAntiPionCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatMuonsPionVetoCumulative'),
    src_individual = cms.InputTag('selectedPatMuonsPionVetoIndividual'),
    systematics = cms.vstring(muonSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgMuonTrkIPcut = cms.PSet(
    pluginName = cms.string('muonTrkIPcut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatMuonsTrkIPcumulative'),
    src_individual = cms.InputTag('selectedPatMuonsTrkIPindividual'),
    systematics = cms.vstring(muonSystematics.keys()),
    minNumber = cms.uint32(1)
)

# tau candidate selection
cfgTauAntiOverlapWithMuonsVeto = cms.PSet(
    pluginName = cms.string('tauAntiOverlapWithMuonsVeto'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatTausForMuTauAntiOverlapWithMuonsVetoCumulative'),
    src_individual = cms.InputTag('selectedPatTausForMuTauAntiOverlapWithMuonsVetoIndividual'),
    systematics = cms.vstring(tauSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgTauEtaCut = cms.PSet(
    pluginName = cms.string('tauEtaCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatTausForMuTauEta21Cumulative'),
    src_individual = cms.InputTag('selectedPatTausForMuTauEta21Individual'),
    systematics = cms.vstring(tauSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgTauPtCut = cms.PSet(
    pluginName = cms.string('tauPtCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatTausForMuTauPt20Cumulative'),
    src_individual = cms.InputTag('selectedPatTausForMuTauPt20Individual'),
    systematics = cms.vstring(tauSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgTauLeadTrkCut = cms.PSet(
    pluginName = cms.string('tauLeadTrkCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatTausForMuTauLeadTrkCumulative'),
    src_individual = cms.InputTag('selectedPatTausForMuTauLeadTrkIndividual'),
    systematics = cms.vstring(tauSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgTauLeadTrkPtCut = cms.PSet(
    pluginName = cms.string('tauLeadTrkPtCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatTausForMuTauLeadTrkPtCumulative'),
    src_individual = cms.InputTag('selectedPatTausForMuTauLeadTrkPtIndividual'),
    systematics = cms.vstring(tauSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgTauTaNCdiscrCut = cms.PSet(
    pluginName = cms.string('tauTaNCdiscrCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatTausForMuTauTaNCdiscrCumulative'),
    src_individual = cms.InputTag('selectedPatTausForMuTauTaNCdiscrIndividual'),
    systematics = cms.vstring(tauSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgTauTrkIsoCut = cms.PSet(
    pluginName = cms.string('tauTrkIsoCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatTausForMuTauTrkIsoCumulative'),
    src_individual = cms.InputTag('selectedPatTausForMuTauTrkIsoIndividual'),
    systematics = cms.vstring(tauSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgTauEcalIsoCut = cms.PSet(
    pluginName = cms.string('tauEcalIsoCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatTausForMuTauEcalIsoCumulative'),
    src_individual = cms.InputTag('selectedPatTausForMuTauEcalIsoIndividual'),
    systematics = cms.vstring(tauSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgTauProngCut = cms.PSet(
    pluginName = cms.string('tauProngCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatTausForMuTauProngCumulative'),
    src_individual = cms.InputTag('selectedPatTausForMuTauProngIndividual'),
    systematics = cms.vstring(tauSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgTauChargeCut = cms.PSet(
    pluginName = cms.string('tauChargeCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatTausForMuTauChargeCumulative'),
    src_individual = cms.InputTag('selectedPatTausForMuTauChargeIndividual'),
    systematics = cms.vstring(tauSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgTauMuonVeto = cms.PSet(
    pluginName = cms.string('tauMuonVeto'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatTausForMuTauMuonVetoCumulative'),
    src_individual = cms.InputTag('selectedPatTausForMuTauMuonVetoIndividual'),
    systematics = cms.vstring(tauSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgTauElectronVeto = cms.PSet(
    pluginName = cms.string('tauElectronVeto'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatTausForMuTauElectronVetoCumulative'),
    src_individual = cms.InputTag('selectedPatTausForMuTauElectronVetoIndividual'),
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
# (two paths:two
#   o two (loosely) isolated global muons of Pt > 10 GeV and opposite charge
#   o two global muons of Pt > 10 GeV and opposite charge and invariant mass 80 < M(mu + mu) < 100 GeV)
cfgDiMuPairZmumuHypothesisVetoByLooseIsolation = cms.PSet(
    pluginName = cms.string('diMuPairZmumuHypothesisVetoByLooseIsolation'),
    pluginType = cms.string('PATCandViewMaxEventSelector'),
    src = cms.InputTag('selectedDiMuPairZmumuHypothesesByLooseIsolation'),
    #systematics = cms.vstring(muonSystematics.keys()),
    maxNumber = cms.uint32(0)
)
cfgDiMuPairZmumuHypothesisVetoByMass = cms.PSet(
    pluginName = cms.string('diMuPairZmumuHypothesisVetoByMass'),
    pluginType = cms.string('PATCandViewMaxEventSelector'),
    src = cms.InputTag('selectedDiMuPairZmumuHypothesesByMass'),
    #systematics = cms.vstring(muonSystematics.keys()),
    maxNumber = cms.uint32(0)
)

zToMuTauEventSelConfigurator = eventSelFlagProdConfigurator(
    [ cfgGenPhaseSpaceCut,
      cfgTrigger,
      cfgPrimaryEventVertex,
      cfgPrimaryEventVertexQuality,
      cfgPrimaryEventVertexPosition,
      cfgGlobalMuonCut,
      cfgMuonEtaCut,
      cfgMuonPtCut,
      cfgTauAntiOverlapWithMuonsVeto,
      cfgTauEtaCut,
      cfgTauPtCut,
      cfgMuonVbTfIdCut,
      cfgMuonPFRelIsoCut,
      cfgMuonAntiPionCut,
      cfgMuonTrkIPcut,
      cfgTauLeadTrkCut,
      cfgTauLeadTrkPtCut,
      cfgTauTaNCdiscrCut,
      cfgTauTrkIsoCut,
      cfgTauEcalIsoCut,
      cfgTauProngCut,
      cfgTauChargeCut,
      cfgTauMuonVeto,
      cfgTauElectronVeto,
      cfgDiTauCandidateForMuTauAntiOverlapVeto,
      cfgDiTauCandidateForMuTauZeroChargeCut,
      cfgDiTauCandidateForMuTauAcoplanarity12Cut,
      cfgDiTauCandidateForMuTauMt1METcut,
      cfgDiTauCandidateForMuTauPzetaDiffCut,
      cfgDiMuPairZmumuHypothesisVetoByLooseIsolation,
      cfgDiMuPairZmumuHypothesisVetoByMass ],
    boolEventSelFlagProducer = "BoolEventSelFlagProducer",
    pyModuleName = __name__
)

produceEventSelFlagsZtoMuTau = zToMuTauEventSelConfigurator.configure()

isRecZtoMuTau = cms.EDProducer("BoolEventSelFlagProducer",
    pluginName = cms.string('isRecZtoMuTau'),
    pluginType = cms.string('MultiBoolEventSelFlagSelector'),
    flags = cms.VInputTag(
        cms.InputTag('Trigger'),
        cms.InputTag('primaryEventVertexPosition'),
        cms.InputTag('muonTrkIPcut', 'cumulative'),
        cms.InputTag('tauElectronVeto', 'cumulative'),
        cms.InputTag('diTauCandidateForMuTauPzetaDiffCut', 'cumulative'),
        cms.InputTag('diMuPairZmumuHypothesisVetoByLooseIsolation'),
        cms.InputTag('diMuPairZmumuHypothesisVetoByMass')
    )
)

selectZtoMuTauEvents = cms.Sequence(
    produceEventSelFlagsZtoMuTau
   * isRecZtoMuTau
)

