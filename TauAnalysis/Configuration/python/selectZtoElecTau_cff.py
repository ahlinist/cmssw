import FWCore.ParameterSet.Config as cms

from TauAnalysis.RecoTools.tools.eventSelFlagProdConfigurator import *
from TauAnalysis.CandidateTools.sysErrDefinitions_cfi import *

#--------------------------------------------------------------------------------
# define event selection criteria for Z --> e + tau-jet channel
#--------------------------------------------------------------------------------

# 
cfgGenPhaseSpaceCut = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)

# trigger selection
cfgTrigger = cms.PSet(
    pluginName = cms.string('Trigger'),
	#pluginType = cms.string('TriggerResultEventSelector'),
    pluginType = cms.string('PATTriggerEventSelector'),
	#src = cms.InputTag('TriggerResults::HLT'),
    src = cms.InputTag('patTriggerEvent'),
	hltAcceptPaths = cms.vstring('HLT_Ele10_SW_L1R','HLT_Ele12_SW_TightEleIdIsol_L1R')
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

# electron candidate selection
cfgElectronIdCut = cms.PSet(
    pluginName = cms.string('electronIdCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatElectronsForElecTauIdCumulative'),
    src_individual = cms.InputTag('selectedPatElectronsForElecTauIdIndividual'),
	systematics = cms.vstring(electronSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgElectronAntiCrackCut = cms.PSet(
    pluginName = cms.string('electronAntiCrackCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatElectronsForElecTauAntiCrackCutCumulative'),
    src_individual = cms.InputTag('selectedPatElectronsForElecTauAntiCrackCutIndividual'),
	systematics = cms.vstring(electronSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgElectronEtaCut = cms.PSet(
    pluginName = cms.string('electronEtaCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatElectronsForElecTauEta21Cumulative'),
    src_individual = cms.InputTag('selectedPatElectronsForElecTauEta21Individual'),
	systematics = cms.vstring(electronSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgElectronPtCut = cms.PSet(
    pluginName = cms.string('electronPtCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatElectronsForElecTauPt15Cumulative'),
    src_individual = cms.InputTag('selectedPatElectronsForElecTauPt15Individual'),
	systematics = cms.vstring(electronSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgElectronTrkIsoCut = cms.PSet(
    pluginName = cms.string('electronTrkIsoCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatElectronsForElecTauTrkIsoCumulative'),
    src_individual = cms.InputTag('selectedPatElectronsForElecTauTrkIsoIndividual'),
	systematics = cms.vstring(electronSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgElectronEcalIsoCut = cms.PSet(
    pluginName = cms.string('electronEcalIsoCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatElectronsForElecTauEcalIsoCumulative'),
    src_individual = cms.InputTag('selectedPatElectronsForElecTauEcalIsoIndividual'),
	systematics = cms.vstring(electronSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgElectronIsoCut = cms.PSet(
    pluginName = cms.string('electronIsoCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatElectronsForElecTauIsoCumulative'),
    src_individual = cms.InputTag('selectedPatElectronsForElecTauIsoIndividual'),
    systematics = cms.vstring(electronSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgElectronConversionVeto = cms.PSet(
    pluginName = cms.string('electronConversionVeto'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatElectronsForElecTauConversionVetoCumulative'),
    src_individual = cms.InputTag('selectedPatElectronsForElecTauConversionVetoIndividual'),
	systematics = cms.vstring(electronSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgElectronTrkIPcut = cms.PSet(
    pluginName = cms.string('electronTrkIPcut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatElectronsForElecTauTrkIPcumulative'),
    src_individual = cms.InputTag('selectedPatElectronsForElecTauTrkIPindividual'),
	systematics = cms.vstring(electronSystematics.keys()),
    minNumber = cms.uint32(1)
)

# tau candidate selection
cfgTauAntiOverlapWithElectronsVeto = cms.PSet(
    pluginName = cms.string('tauAntiOverlapWithElectronsVeto'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatTausForElecTauAntiOverlapWithElectronsVetoCumulative'),
    src_individual = cms.InputTag('selectedPatTausForElecTauAntiOverlapWithElectronsVetoIndividual'),
	systematics = cms.vstring(tauSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgTauEtaCut = cms.PSet(
    pluginName = cms.string('tauEtaCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatTausForElecTauEta23Cumulative'),
    src_individual = cms.InputTag('selectedPatTausEta23Individual'),
	systematics = cms.vstring(tauSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgTauPtCut = cms.PSet(
    pluginName = cms.string('tauPtCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatTausForElecTauPt20Cumulative'),
    src_individual = cms.InputTag('selectedPatTausPt20Individual'),
	systematics = cms.vstring(tauSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgTauLeadTrkCut = cms.PSet(
    pluginName = cms.string('tauLeadTrkCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatTausForElecTauLeadTrkCumulative'),
    src_individual = cms.InputTag('selectedPatTausLeadTrkIndividual'),
	systematics = cms.vstring(tauSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgTauLeadTrkPtCut = cms.PSet(
    pluginName = cms.string('tauLeadTrkPtCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatTausForElecTauLeadTrkPtCumulative'),
    src_individual = cms.InputTag('selectedPatTausLeadTrkPtIndividual'),
	systematics = cms.vstring(tauSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgTauTaNCdiscrCut = cms.PSet(
    pluginName = cms.string('tauTaNCdiscrCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatTausForElecTauTaNCdiscrCumulative'),
    src_individual = cms.InputTag('selectedPatTausForElecTauTaNCdiscrIndividual'),
	systematics = cms.vstring(tauSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgTauTrkIsoCut = cms.PSet(
    pluginName = cms.string('tauTrkIsoCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatTausForElecTauTrkIsoCumulative'),
    src_individual = cms.InputTag('selectedPatTausForElecTauTrkIsoIndividual'),
	systematics = cms.vstring(tauSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgTauEcalIsoCut = cms.PSet(
    pluginName = cms.string('tauEcalIsoCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatTausForElecTauEcalIsoCumulative'),
    src_individual = cms.InputTag('selectedPatTausForElecTauEcalIsoIndividual'),
	systematics = cms.vstring(tauSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgTauProngCut = cms.PSet(
    pluginName = cms.string('tauProngCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatTausForElecTauProngCumulative'),
    src_individual = cms.InputTag('selectedPatTausForElecTauProngIndividual'),
	systematics = cms.vstring(tauSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgTauChargeCut = cms.PSet(
    pluginName = cms.string('tauChargeCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatTausForElecTauChargeCumulative'),
    src_individual = cms.InputTag('selectedPatTausForElecTauChargeIndividual'),
	systematics = cms.vstring(tauSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgTauElectronVeto = cms.PSet(
    pluginName = cms.string('tauElectronVeto'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatTausForElecTauElectronVetoCumulative'),
    src_individual = cms.InputTag('selectedPatTausForElecTauElectronVetoIndividual'),
	systematics = cms.vstring(tauSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgTauEcalCrackVeto = cms.PSet(
    pluginName = cms.string('tauEcalCrackVeto'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatTausForElecTauEcalCrackVetoCumulative'),
    src_individual = cms.InputTag('selectedPatTausForElecTauEcalCrackVetoIndividual'),
	systematics = cms.vstring(tauSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgTauMuonVeto = cms.PSet(
    pluginName = cms.string('tauMuonVeto'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatTausForElecTauMuonVetoCumulative'),
    src_individual = cms.InputTag('selectedPatTausForElecTauMuonVetoIndividual'),
	systematics = cms.vstring(tauSystematics.keys()),
    minNumber = cms.uint32(1)
)


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
cfgDiElecPairZeeHypothesisVetoByLooseIsolation = cms.PSet(
    pluginName = cms.string('diElecPairZeeHypothesisVetoByLooseIsolation'),
    pluginType = cms.string('PATCandViewMaxEventSelector'),
    src = cms.InputTag('selectedDiElecPairZeeHypothesesByLooseIsolation'),
    maxNumber = cms.uint32(0)
)

zToElecTauEventSelConfigurator = eventSelFlagProdConfigurator(
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
      cfgElecTauPairZeeHypothesisVeto ],
    boolEventSelFlagProducer = "BoolEventSelFlagProducer",
    pyModuleName = __name__
)

produceEventSelFlagsZtoElecTau = zToElecTauEventSelConfigurator.configure()

isRecZtoElecTau = cms.EDProducer("BoolEventSelFlagProducer",
    pluginName = cms.string('isRecZtoElecTau'),
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

selectZtoElecTauEvents = cms.Sequence(
    produceEventSelFlagsZtoElecTau
    + isRecZtoElecTau
)

