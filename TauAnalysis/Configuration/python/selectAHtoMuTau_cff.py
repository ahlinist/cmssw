import FWCore.ParameterSet.Config as cms

from TauAnalysis.RecoTools.tools.eventSelFlagProdConfigurator import *
from TauAnalysis.CandidateTools.sysErrDefinitions_cfi import *

#--------------------------------------------------------------------------------
# define event selection criteria for A/H --> mu + tau-jet channel
#--------------------------------------------------------------------------------

from TauAnalysis.Configuration.selectZtoMuTau_cff import *

# di-tau candidate selection
cfgDiTauCandidateForAHtoMuTauAntiOverlapVeto = cfgDiTauCandidateForMuTauAntiOverlapVeto.clone(
    pluginName = cms.string('diTauCandidateForAHtoMuTauAntiOverlapVeto'),
    src_cumulative = cms.InputTag('selectedMuTauPairsForAHtoMuTauAntiOverlapVetoCumulative'),
    src_individual = cms.InputTag('selectedMuTauPairsForAHtoMuTauAntiOverlapVetoIndividual')
)
cfgDiTauCandidateForAHtoMuTauZeroChargeCut = cfgDiTauCandidateForMuTauZeroChargeCut.clone(
    pluginName = cms.string('diTauCandidateForAHtoMuTauZeroChargeCut'),
    src_cumulative = cms.InputTag('selectedMuTauPairsForAHtoMuTauZeroChargeCumulative'),
    src_individual = cms.InputTag('selectedMuTauPairsForAHtoMuTauZeroChargeIndividual')
)
cfgDiTauCandidateForAHtoMuTauMt1METcut = cfgDiTauCandidateForMuTauMt1METcut.clone(
    pluginName = cms.string('diTauCandidateForAHtoMuTauMt1METcut'),
    src_cumulative = cms.InputTag('selectedMuTauPairsForAHtoMuTauMt1METcumulative'),
    src_individual = cms.InputTag('selectedMuTauPairsForAHtoMuTauMt1METindividual')
)
cfgDiTauCandidateForAHtoMuTauPzetaDiffCut = cfgDiTauCandidateForMuTauPzetaDiffCut.clone(
    pluginName = cms.string('diTauCandidateForAHtoMuTauPzetaDiffCut'),
    src_cumulative = cms.InputTag('selectedMuTauPairsForAHtoMuTauPzetaDiffCumulative'),
    src_individual = cms.InputTag('selectedMuTauPairsForAHtoMuTauPzetaDiffIndividual')
)

# central jet veto/b-jet candidate selection
cfgCentralJetEt20bTagVeto = cms.PSet(
    pluginName = cms.string('centralJetEt20bTagVeto'),
    pluginType = cms.string('PATCandViewMaxEventSelector'),
    src_cumulative = cms.InputTag('selectedPatJetsForAHtoMuTauBtagCumulative'),
    src_individual = cms.InputTag('selectedPatJetsForAHtoMuTauBtagIndividual'),
    systematics = cms.vstring(jetSystematics.keys()),
    maxNumber = cms.uint32(0)
)
cfgCentralJetEt20Cut = cms.PSet(
    pluginName = cms.string('centralJetEt20Cut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatJetsForAHtoMuTauAntiOverlapWithLeptonsVetoCumulative'),
    src_individual = cms.InputTag('selectedPatJetsForAHtoMuTauAntiOverlapWithLeptonsVetoIndividual'),
    systematics = cms.vstring(jetSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgCentralJetEt20bTagCut = cms.PSet(
    pluginName = cms.string('centralJetEt20bTagCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatJetsForAHtoMuTauBtagCumulative'),
    src_individual = cms.InputTag('selectedPatJetsForAHtoMuTauBtagIndividual'),
    systematics = cms.vstring(jetSystematics.keys()),
    minNumber = cms.uint32(1)
)

ahToMuTauEventSelConfigurator = eventSelFlagProdConfigurator(
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
      cfgDiTauCandidateForAHtoMuTauAntiOverlapVeto,
      cfgDiTauCandidateForAHtoMuTauZeroChargeCut,
      cfgDiTauCandidateForAHtoMuTauMt1METcut,
      cfgDiTauCandidateForAHtoMuTauPzetaDiffCut,
      cfgDiMuPairZmumuHypothesisVetoByLooseIsolation,
      cfgCentralJetEt20bTagVeto,
      cfgCentralJetEt20Cut,
      cfgCentralJetEt20bTagCut ],
    boolEventSelFlagProducer = "BoolEventSelFlagProducer",
    pyModuleName = __name__
)

produceEventSelFlagsAHtoMuTau = ahToMuTauEventSelConfigurator.configure()

isRecAHtoMuTauCentralJetVeto = cms.EDProducer("BoolEventSelFlagProducer",
    pluginName = cms.string('isRecAHtoMuTauCentralJetVeto'),
    pluginType = cms.string('MultiBoolEventSelFlagSelector'),
    flags = cms.VInputTag(
        cms.InputTag('Trigger'),
        cms.InputTag('primaryEventVertexPosition'),
        cms.InputTag('muonTrkIPcut', 'cumulative'),
        cms.InputTag('tauElectronVeto', 'cumulative'),
        cms.InputTag('diTauCandidateForAHtoMuTauPzetaDiffCut', 'cumulative'),
        cms.InputTag('diMuPairZmumuHypothesisVetoByLooseIsolation'),
        cms.InputTag('centralJetEt20bTagVeto', 'cumulative')
    )
)

isRecAHtoMuTauCentralJetBtag = cms.EDProducer("BoolEventSelFlagProducer",
    pluginName = cms.string('isRecAHtoMuTauCentralJetBtag'),
    pluginType = cms.string('MultiBoolEventSelFlagSelector'),
    flags = cms.VInputTag(
        cms.InputTag('Trigger'),
        cms.InputTag('primaryEventVertexPosition'),
        cms.InputTag('muonTrkIPcut', 'cumulative'),
        cms.InputTag('tauElectronVeto', 'cumulative'),
        cms.InputTag('diTauCandidateForAHtoMuTauPzetaDiffCut', 'cumulative'),
        cms.InputTag('diMuPairZmumuHypothesisVetoByLooseIsolation'),
        cms.InputTag('centralJetEt20bTagCut', 'cumulative')
    )
)

isRecAHtoMuTau = cms.EDProducer("BoolEventSelFlagProducer",
    pluginName = cms.string('isRecAHtoMuTauCentralJetBtag'),
    pluginType = cms.string('MultiBoolEventSelFlagSelector'),
    flags = cms.VInputTag(
        cms.InputTag('Trigger'),
        cms.InputTag('primaryEventVertexPosition'),
        cms.InputTag('muonTrkIPcut', 'cumulative'),
        cms.InputTag('tauMuonVeto', 'cumulative'),
        cms.InputTag('diTauCandidateForAHtoMuTauPzetaDiffCut', 'cumulative'),
        cms.InputTag('diMuPairZmumuHypothesisVetoByLooseIsolation'),
    )
)

selectAHtoMuTauEvents = cms.Sequence(
    produceEventSelFlagsAHtoMuTau
    * isRecAHtoMuTauCentralJetVeto
    * isRecAHtoMuTauCentralJetBtag
    * isRecAHtoMuTau
)
