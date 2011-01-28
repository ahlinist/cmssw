import FWCore.ParameterSet.Config as cms

from TauAnalysis.RecoTools.tools.eventSelFlagProdConfigurator import *
from TauAnalysis.CandidateTools.sysErrDefinitions_cfi import *


#--------------------------------------------------------------------------------
# define event selection criteria for A/H --> e + tau-jet channel
#--------------------------------------------------------------------------------

# get lepton config from Z analysis
from TauAnalysis.Configuration.selectZtoElecTau_cff import *

# di-tau candidate selection
cfgDiTauCandidateForElecTauAntiOverlapVeto = cms.PSet(
    pluginName = cms.string('diTauCandidateForElecTauAntiOverlapVeto'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedElecTauPairsAntiOverlapVetoCumulative'),
    src_individual = cms.InputTag('selectedElecTauPairsAntiOverlapVetoIndividual'),
	systematics = cms.vstring(elecTauPairSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgDiTauCandidateForElecTauZeroChargeCut = cms.PSet(
    pluginName = cms.string('diTauCandidateForElecTauZeroChargeCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedElecTauPairsZeroChargeCumulative'),
    src_individual = cms.InputTag('selectedElecTauPairsZeroChargeIndividual'),
	systematics = cms.vstring(elecTauPairSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgDiTauCandidateForElecTauAcoplanarity12Cut = cms.PSet(
    pluginName = cms.string('diTauCandidateForElecTauAcoplanarity12Cut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedElecTauPairsAcoplanarity12Cumulative'),
    src_individual = cms.InputTag('selectedElecTauPairsAcoplanarity12Individual'),
	systematics = cms.vstring(elecTauPairSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgDiTauCandidateForElecTauMt1METCut = cms.PSet(
    pluginName = cms.string('diTauCandidateForElecTauMt1METCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedElecTauPairsMt1METcumulative'),
    src_individual = cms.InputTag('selectedElecTauPairsMt1METindividual'),
	systematics = cms.vstring(elecTauPairSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgDiTauCandidateForElecTauPzetaDiffCut = cms.PSet(
    pluginName = cms.string('diTauCandidateForElecTauPzetaDiffCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedElecTauPairsPzetaDiffCumulative'),
    src_individual = cms.InputTag('selectedElecTauPairsPzetaDiffIndividual'),
	systematics = cms.vstring(elecTauPairSystematics.keys()),
    minNumber = cms.uint32(1)
)

# veto events compatible with Z --> e+ e- hypothesis
# (based on reconstructed (visible) invariant mass of e + tau-jet pair)
cfgElecTauPairZeeHypothesisVeto = cms.PSet(
    pluginName = cms.string('elecTauPairZeeHypothesisVeto'),
    pluginType = cms.string('ZllHypothesisElecTauMaxEventSelector'),
    src = cms.InputTag('selectedElecTauPairZeeHypotheses'),
    maxNumber = cms.uint32(0)
)

# veto events compatible with Z -> e+ e- by finding a second loosely-isolated electron
#   with opposite charge w.r.t the primary electron
cfgDiElecPairZeeHypothesisVeto = cms.PSet(
    pluginName = cms.string('diElecPairZeeHypothesisVeto'),
    pluginType = cms.string('PATCandViewMaxEventSelector'),
    src = cms.InputTag('selectedDiElecPairZeeHypotheses'),
    maxNumber = cms.uint32(0)
)
cfgDiElecPairZeeHypothesisVetoByLooseIsolation = cms.PSet(
    pluginName = cms.string('diElecPairZeeHypothesisVetoByLooseIsolation'),
    pluginType = cms.string('PATCandViewMaxEventSelector'),
    src = cms.InputTag('selectedDiElecPairZeeHypothesesByLooseIsolation'),
    maxNumber = cms.uint32(0)
)

ahToElecTauEventSelConfigurator = eventSelFlagProdConfigurator(
    [ cfgGenPhaseSpaceCut,
	  cfgTrigger,
      cfgPrimaryEventVertex,
      cfgPrimaryEventVertexQuality,
      cfgPrimaryEventVertexPosition,
      cfgElectronIdCut,
      cfgElectronAntiCrackCut,
      cfgElectronEtaCut,
      cfgElectronPtCut,
	  #cfgElectronTrkIsoCut,
	  #cfgElectronEcalIsoCut,
      cfgElectronIsoCut,
      cfgElectronConversionVeto,
      cfgElectronTrkIPcut,
      cfgTauAntiOverlapWithElectronsVeto,
      cfgTauEtaCut,
      cfgTauPtCut,
      cfgTauLeadTrkCut,
      cfgTauLeadTrkPtCut,
      cfgTauTaNCdiscrCut,
      cfgTauTrkIsoCut,
      cfgTauEcalIsoCut,
      cfgTauProngCut,
      cfgTauChargeCut,
      cfgTauElectronVeto,
      cfgTauEcalCrackVeto,
      cfgTauMuonVeto,
      cfgDiTauCandidateForElecTauAntiOverlapVeto,
      cfgDiTauCandidateForElecTauZeroChargeCut,
      cfgDiTauCandidateForElecTauAcoplanarity12Cut,
      cfgDiTauCandidateForElecTauMt1METCut,
      cfgDiTauCandidateForElecTauPzetaDiffCut,
	  cfgDiElecPairZeeHypothesisVetoByLooseIsolation ],
		#cfgElecTauPairZeeHypothesisVeto ],
    boolEventSelFlagProducer = "BoolEventSelFlagProducer",
    pyModuleName = __name__
)

produceEventSelFlagsAHtoElecTau = ahToElecTauEventSelConfigurator.configure()

isRecAHtoElecTau = cms.EDProducer("BoolEventSelFlagProducer",
    pluginName = cms.string('isRecAHtoElecTau'),
    pluginType = cms.string('MultiBoolEventSelFlagSelector'),
    flags = cms.VInputTag(
        cms.InputTag('Trigger'),
        cms.InputTag('genPhaseSpaceCut'),
        cms.InputTag('primaryEventVertexPosition'),
        cms.InputTag('electronTrkIPcut', 'cumulative'),
        cms.InputTag('tauElectronVeto', 'cumulative'),
        cms.InputTag('diTauCandidateForElecTauZeroChargeCut', 'cumulative'),
        cms.InputTag('diElecPairZeeHypothesisVetoByLooseIsolation'),
    )
)

selectAHtoElecTauEvents = cms.Sequence(
	produceEventSelFlagsAHtoElecTau
	+ isRecAHtoElecTau
)
