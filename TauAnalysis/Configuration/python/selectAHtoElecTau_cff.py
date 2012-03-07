import FWCore.ParameterSet.Config as cms

from TauAnalysis.RecoTools.tools.eventSelFlagProdConfigurator import *
from TauAnalysis.CandidateTools.sysErrDefinitions_cfi import *


#--------------------------------------------------------------------------------
# define event selection criteria for A/H --> e + tau-jet channel
#--------------------------------------------------------------------------------

# get lepton config from Z analysis
from TauAnalysis.Configuration.selectZtoElecTau_cff import *

# di-tau candidate selection
cfgDiTauCandidateForAHtoElecTauAntiOverlapVeto = cms.PSet(
    pluginName = cms.string('diTauCandidateForAHtoElecTauAntiOverlapVeto'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedElecTauPairsForAHtoElecTauAntiOverlapVetoCumulative'),
    src_individual = cms.InputTag('selectedElecTauPairsForAHtoElecTauAntiOverlapVetoIndividual'),
	systematics = cms.vstring(elecTauPairSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgDiTauCandidateForAHtoElecTauMt1METcut = cms.PSet(
    pluginName = cms.string('diTauCandidateForAHtoElecTauMt1METcut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedElecTauPairsForAHtoElecTauMt1METcumulative'),
    src_individual = cms.InputTag('selectedElecTauPairsForAHtoElecTauMt1METindividual'),
	systematics = cms.vstring(elecTauPairSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgDiTauCandidateForAHtoElecTauPzetaDiffCut = cms.PSet(
    pluginName = cms.string('diTauCandidateForAHtoElecTauPzetaDiffCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedElecTauPairsForAHtoElecTauPzetaDiffCumulative'),
    src_individual = cms.InputTag('selectedElecTauPairsForAHtoElecTauPzetaDiffIndividual'),
	systematics = cms.vstring(elecTauPairSystematics.keys()),
    minNumber = cms.uint32(1)
)

# select opposite- and same-sign tau pairs
cfgDiTauCandidateForAHtoElecTauZeroChargeCut = cms.PSet(
    pluginName = cms.string('diTauCandidateForAHtoElecTauZeroChargeCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedElecTauPairsForAHtoElecTauZeroChargeCumulative'),
    src_individual = cms.InputTag('selectedElecTauPairsForAHtoElecTauZeroChargeIndividual'),
	systematics = cms.vstring(elecTauPairSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgDiTauCandidateForAHtoElecTauNonZeroChargeCut = cms.PSet(
    pluginName = cms.string('diTauCandidateForAHtoElecTauNonZeroChargeCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedElecTauPairsForAHtoElecTauNonZeroChargeCumulative'),
    src_individual = cms.InputTag('selectedElecTauPairsForAHtoElecTauNonZeroChargeIndividual'),
	systematics = cms.vstring(elecTauPairSystematics.keys()),
    minNumber = cms.uint32(1)
)

# jet Et selection
cfgJetEtCut = cms.PSet(
    pluginName = cms.string('jetEtCut'),
    pluginType = cms.string('PATCandViewMaxEventSelector'),
    src_cumulative = cms.InputTag('selectedPatJetsForAHtoElecTauJetTagCumulative'),
    src_individual = cms.InputTag('selectedPatJetsForAHtoElecTauJetTagIndividual'),
    systematics = cms.vstring(jetSystematics.keys()),
    maxNumber = cms.uint32(1)
)
# MSSM b-tag jet selection
cfgBtagVeto = cms.PSet(
    pluginName = cms.string('jetBtagVeto'),
    pluginType = cms.string('PATCandViewMaxEventSelector'),
    src_cumulative = cms.InputTag('selectedPatJetsForAHtoElecTauBtagCumulative'),
    src_individual = cms.InputTag('selectedPatJetsForAHtoElecTauBtagIndividual'),
    systematics = cms.vstring(jetSystematics.keys()),
    maxNumber = cms.uint32(0)
)
cfgBtagCut = cms.PSet(
    pluginName = cms.string('jetBtagCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatJetsForAHtoElecTauBtagCumulative'),
    src_individual = cms.InputTag('selectedPatJetsForAHtoElecTauBtagIndividual'),
    systematics = cms.vstring(jetSystematics.keys()),
    minNumber = cms.uint32(1)
)

# SM 0/1 jet selection
cfgBoostedJetVeto = cms.PSet(
    pluginName = cms.string('boostedJetVeto'),
    pluginType = cms.string('PATCandViewMaxEventSelector'),
    src_cumulative = cms.InputTag('selectedPatJetsForAHtoElecTauBoostedJetTagCumulative'),
    src_individual = cms.InputTag('selectedPatJetsForAHtoElecTauBoostedJetTagIndividual'),
    systematics = cms.vstring(jetSystematics.keys()),
    maxNumber = cms.uint32(0)
)
# SM boosted selection
cfgBoostedJetTag = cms.PSet(
    pluginName = cms.string('boostedJetTag'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatJetsForAHtoElecTauBoostedJetTagCumulative'),
    src_individual = cms.InputTag('selectedPatJetsForAHtoElecTauBoostedJetTagIndividual'),
    systematics = cms.vstring(jetSystematics.keys()),
    minNumber = cms.uint32(1)
)


# VBF event selection
cfgVBFEventTag = cms.PSet(
    pluginName = cms.string('vbfTagCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedVBFEventHypothesesForAHtoElecTauTagJetOpposHemisphereCumulative'),
    src_individual = cms.InputTag('selectedVBFEventHypothesesForAHtoElecTauTagJetOpposHemisphereIndividual'),
    systematics = cms.vstring(jetSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgVBFEventDEta35 = cms.PSet(
    pluginName = cms.string('vbfDEta35Cut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedVBFEventHypothesesForAHtoElecTauTagJetDEta35Cumulative'),
    src_individual = cms.InputTag('selectedVBFEventHypothesesForAHtoElecTauTagJetDEta35Individual'),
    systematics = cms.vstring(jetSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgVBFEventMass350 = cms.PSet(
    pluginName = cms.string('vbfMass350Cut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedVBFEventHypothesesForAHtoElecTauTagJetMass350Cumulative'),
    src_individual = cms.InputTag('selectedVBFEventHypothesesForAHtoElecTauTagJetMass350Individual'),
    systematics = cms.vstring(jetSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgVBF3rdTagJetVeto = cms.PSet(
    pluginName = cms.string('vbf3rdTagJetVeto'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedVBFEventHypothesesForAHtoElecTau3rdJetVetoCumulative'),
    src_individual = cms.InputTag('selectedVBFEventHypothesesForAHtoElecTau3rdJetVetoIndividual'),
    systematics = cms.vstring(jetSystematics.keys()),
    minNumber = cms.uint32(1)
)

ahToElecTauEventSelConfiguratorOS = eventSelFlagProdConfigurator(
    [ cfgGenPhaseSpaceCut,
	  cfgTrigger,
      cfgPrimaryEventVertexQuality,
      cfgPrimaryEventVertexPosition,
      cfgElectronIdCut,
      cfgElectronAntiCrackCut,
      cfgElectronEtaCut,
      cfgElectronPtCut,
      cfgElectronIsoCut,
      cfgElectronConversionVeto,
      cfgElectronTrkIPcut,
      cfgTauAntiOverlapWithElectronsVeto,
      cfgTauEtaCut,
      cfgTauPtCut,
      cfgTauDecayModeFindingCut,
      cfgTauLeadTrkPtCut,
      cfgTauIsoCut,
      cfgTauElectronVeto,
      cfgTauMuonVeto,
      cfgDiTauCandidateForAHtoElecTauAntiOverlapVeto,
      cfgDiTauCandidateForAHtoElecTauMt1METcut,
      cfgDiTauCandidateForAHtoElecTauPzetaDiffCut,
	  cfgDiElecPairZeeHypothesisVetoByLooseIsolation,
	  cfgJetEtCut,
	  cfgBtagVeto,
	  cfgBtagCut,
      cfgBoostedJetVeto,
      cfgBoostedJetTag,
      cfgVBFEventTag,
      cfgVBFEventDEta35,
      cfgVBFEventMass350,
      cfgVBF3rdTagJetVeto,
      cfgDiTauCandidateForAHtoElecTauZeroChargeCut ],
    boolEventSelFlagProducer = "BoolEventSelFlagProducer",
    pyModuleName = __name__
)

produceEventSelFlagsAHtoElecTauOS = ahToElecTauEventSelConfiguratorOS.configure()

ahToElecTauEventSelConfiguratorSS = eventSelFlagProdConfigurator(
    [ cfgDiTauCandidateForAHtoElecTauNonZeroChargeCut ],
    boolEventSelFlagProducer = "BoolEventSelFlagProducer",
    pyModuleName = __name__
)

produceEventSelFlagsAHtoElecTauSS = ahToElecTauEventSelConfiguratorSS.configure()

produceEventSelFlagsAHtoElecTau = cms.Sequence( produceEventSelFlagsAHtoElecTauOS * produceEventSelFlagsAHtoElecTauSS )

isRecAHtoElecTau = cms.EDProducer("BoolEventSelFlagProducer",
    pluginName = cms.string('isRecAHtoElecTau'),
    pluginType = cms.string('MultiBoolEventSelFlagSelector'),
    flags = cms.VInputTag(
        cms.InputTag('Trigger'),
        cms.InputTag('genPhaseSpaceCut'),
        cms.InputTag('primaryEventVertexPosition'),
        cms.InputTag('electronTrkIPcut', 'cumulative'),
        cms.InputTag('tauMuonVeto', 'cumulative'),
        cms.InputTag('diTauCandidateForAHtoElecTauZeroChargeCut', 'cumulative'),
        cms.InputTag('diElecPairZeeHypothesisVetoByLooseIsolation'),
    )
)

isRecAHtoElecTauBtag = cms.EDProducer("BoolEventSelFlagProducer",
    pluginName = cms.string('isRecAHtoElecTauBtag'),
    pluginType = cms.string('MultiBoolEventSelFlagSelector'),
    flags = cms.VInputTag(
        cms.InputTag('Trigger'),
        cms.InputTag('primaryEventVertexPosition'),
        cms.InputTag('electronTrkIPcut', 'cumulative'),
        cms.InputTag('tauMuonVeto', 'cumulative'),
        cms.InputTag('diTauCandidateForAHtoElecTauZeroChargeCut', 'cumulative'),
        cms.InputTag('diElecPairZeeHypothesisVetoByLooseIsolation'),
        cms.InputTag('jetBtagCut', 'cumulative')
    )
)

isRecAHtoElecTauBtagVeto = cms.EDProducer("BoolEventSelFlagProducer",
    pluginName = cms.string('isRecAHtoElecTauBtagVeto'),
    pluginType = cms.string('MultiBoolEventSelFlagSelector'),
    flags = cms.VInputTag(
        cms.InputTag('Trigger'),
        cms.InputTag('primaryEventVertexPosition'),
        cms.InputTag('electronTrkIPcut', 'cumulative'),
        cms.InputTag('tauMuonVeto', 'cumulative'),
        cms.InputTag('diTauCandidateForAHtoElecTauZeroChargeCut', 'cumulative'),
        cms.InputTag('diElecPairZeeHypothesisVetoByLooseIsolation'),
        cms.InputTag('jetBtagVeto', 'cumulative')
    )
)
isRecAHtoElecTauVBFtag = cms.EDProducer("BoolEventSelFlagProducer",
    pluginName = cms.string('isRecAHtoElecTauVBFtag'),
    pluginType = cms.string('MultiBoolEventSelFlagSelector'),
    flags = cms.VInputTag(
        cms.InputTag('Trigger'),
        cms.InputTag('genPhaseSpaceCut'),
        cms.InputTag('primaryEventVertexPosition'),
        cms.InputTag('electronTrkIPcut', 'cumulative'),
        cms.InputTag('tauMuonVeto', 'cumulative'),
        cms.InputTag('diTauCandidateForElecTauZeroChargeCut', 'cumulative'),
        cms.InputTag('diElecPairZeeHypothesisVetoByLooseIsolation'),
        cms.InputTag('vbfMass350Cut', 'cumulative')
    )
)

isRecAHtoElecTauZeroOneJets = cms.EDProducer("BoolEventSelFlagProducer",
    pluginName = cms.string('isRecAHtoElecTauZeroOneJets'),
    pluginType = cms.string('MultiBoolEventSelFlagSelector'),
    flags = cms.VInputTag(
        cms.InputTag('Trigger'),
        cms.InputTag('electronTrkIPcut', 'cumulative'),
        cms.InputTag('tauMuonVeto', 'cumulative'),
        cms.InputTag('diTauCandidateForElecTauZeroChargeCut', 'cumulative'),
        cms.InputTag('primaryEventVertexPosition'),
        cms.InputTag('diElecPairZeeHypothesisVetoByLooseIsolation'),
        cms.InputTag('jetEtCut', 'cumulative'),
        cms.InputTag('boostedJetVeto', 'cumulative')
    )
)

isRecAHtoElecTauBoosted = cms.EDProducer("BoolEventSelFlagProducer",
    pluginName = cms.string('isRecAHtoElecTauBoosted'),
    pluginType = cms.string('MultiBoolEventSelFlagSelector'),
    flags = cms.VInputTag(
        cms.InputTag('Trigger'),
        cms.InputTag('electronTrkIPcut', 'cumulative'),
        cms.InputTag('tauMuonVeto', 'cumulative'),
        cms.InputTag('diTauCandidateForElecTauZeroChargeCut', 'cumulative'),
        cms.InputTag('primaryEventVertexPosition'),
        cms.InputTag('diElecPairZeeHypothesisVetoByLooseIsolation'),
        cms.InputTag('jetEtCut', 'cumulative'),
        cms.InputTag('boostedJetTag', 'cumulative')
    )
)

selectAHtoElecTauEvents = cms.Sequence(
    selectZtoElecTauEvents
	* produceEventSelFlagsAHtoElecTau
    * isRecAHtoElecTauVBFtag
    * isRecAHtoElecTauZeroOneJets
    * isRecAHtoElecTauBoosted
	* isRecAHtoElecTauBtagVeto
	* isRecAHtoElecTauBtag
	* isRecAHtoElecTau
)
