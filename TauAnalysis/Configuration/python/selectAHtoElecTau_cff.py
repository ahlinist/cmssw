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

# central jet veto/b-jet candidate selection
cfgCentralJetEt20bTagVeto = cms.PSet(
    pluginName = cms.string('centralJetEt20bTagVeto'),
    pluginType = cms.string('PATCandViewMaxEventSelector'),
    src_cumulative = cms.InputTag('selectedPatJetsForAHtoElecTauBtagCumulative'),
    src_individual = cms.InputTag('selectedPatJetsForAHtoElecTauBtagIndividual'),
    systematics = cms.vstring(jetSystematics.keys()),
    maxNumber = cms.uint32(0)
)
cfgCentralJetEt20Cut = cms.PSet(
    pluginName = cms.string('centralJetEt20Cut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatJetsForAHtoElecTauAntiOverlapWithLeptonsVetoCumulative'),
    src_individual = cms.InputTag('selectedPatJetsForAHtoElecTauAntiOverlapWithLeptonsVetoIndividual'),
    systematics = cms.vstring(jetSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgCentralJetEt20bTagCut = cms.PSet(
    pluginName = cms.string('centralJetEt20bTagCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatJetsForAHtoElecTauBtagCumulative'),
    src_individual = cms.InputTag('selectedPatJetsForAHtoElecTauBtagIndividual'),
    systematics = cms.vstring(jetSystematics.keys()),
    minNumber = cms.uint32(1)
)

ahToElecTauEventSelConfiguratorOS = eventSelFlagProdConfigurator(
    [ cfgGenPhaseSpaceCut,
	  cfgTrigger,
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
      cfgTauEcalCrackVeto,
      cfgTauMuonVeto,
      cfgDiTauCandidateForAHtoElecTauAntiOverlapVeto,
      cfgDiTauCandidateForAHtoElecTauMt1METcut,
      cfgDiTauCandidateForAHtoElecTauPzetaDiffCut,
      cfgPrimaryEventVertexForElecTau,
      cfgPrimaryEventVertexQualityForElecTau,
      cfgPrimaryEventVertexPositionForElecTau,
	  cfgDiElecPairZeeHypothesisVetoByLooseIsolation,
	  cfgCentralJetEt20bTagVeto,
	  cfgCentralJetEt20Cut,
	  cfgCentralJetEt20bTagCut,
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
        cms.InputTag('primaryEventVertexPositionForElecTau'),
        cms.InputTag('electronTrkIPcut', 'cumulative'),
        cms.InputTag('tauMuonVeto', 'cumulative'),
        cms.InputTag('diTauCandidateForAHtoElecTauZeroChargeCut', 'cumulative'),
        cms.InputTag('diElecPairZeeHypothesisVetoByLooseIsolation'),
    )
)

isRecAHtoElecTauCentralJetBtag = cms.EDProducer("BoolEventSelFlagProducer",
    pluginName = cms.string('isRecAHtoElecTauCentralJetBtag'),
    pluginType = cms.string('MultiBoolEventSelFlagSelector'),
    flags = cms.VInputTag(
        cms.InputTag('Trigger'),
        cms.InputTag('primaryEventVertexPositionForElecTau'),
        cms.InputTag('electronTrkIPcut', 'cumulative'),
        cms.InputTag('tauMuonVeto', 'cumulative'),
        cms.InputTag('diTauCandidateForAHtoElecTauZeroChargeCut', 'cumulative'),
        cms.InputTag('diElecPairZeeHypothesisVetoByLooseIsolation'),
        cms.InputTag('centralJetEt20bTagCut', 'cumulative')
    )
)

isRecAHtoElecTauCentralJetVeto = cms.EDProducer("BoolEventSelFlagProducer",
    pluginName = cms.string('isRecAHtoElecTauCentralJetVeto'),
    pluginType = cms.string('MultiBoolEventSelFlagSelector'),
    flags = cms.VInputTag(
        cms.InputTag('Trigger'),
        cms.InputTag('primaryEventVertexPositionForElecTau'),
        cms.InputTag('electronTrkIPcut', 'cumulative'),
        cms.InputTag('tauMuonVeto', 'cumulative'),
        cms.InputTag('diTauCandidateForAHtoElecTauZeroChargeCut', 'cumulative'),
        cms.InputTag('diElecPairZeeHypothesisVetoByLooseIsolation'),
        cms.InputTag('centralJetEt20bTagVeto', 'cumulative')
    )
)

selectAHtoElecTauEvents = cms.Sequence(
	produceEventSelFlagsAHtoElecTau
	* isRecAHtoElecTauCentralJetVeto
	* isRecAHtoElecTauCentralJetBtag
	* isRecAHtoElecTau
)
