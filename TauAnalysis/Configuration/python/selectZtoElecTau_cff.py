import FWCore.ParameterSet.Config as cms

from TauAnalysis.RecoTools.eventSelFlagProdConfigurator import *

#--------------------------------------------------------------------------------
# define event selection criteria for Z --> e + tau-jet channel
#--------------------------------------------------------------------------------

# trigger selection
#cfgTrigger = cms.PSet(
#    pluginName = cms.string('Trigger'),
#    pluginType = cms.string('TriggerResultEventSelector'),
#    src = cms.InputTag('TriggerResults::HLT'),
#    triggerPaths = cms.vstring('HLT_IsoEle15_L1I')
#)

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
cfgTightElectronIdCut = cms.PSet(
    pluginName = cms.string('tightElectronIdCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1ElectronsTightIdCumulative'),
    src_individual = cms.InputTag('selectedLayer1ElectronsTightIdIndividual'),
    minNumber = cms.uint32(1)
)
cfgElectronAntiCrackCut = cms.PSet(
    pluginName = cms.string('electronAntiCrackCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1ElectronsAntiCrackCutCumulative'),
    src_individual = cms.InputTag('selectedLayer1ElectronsAntiCrackCutIndividual'),
    minNumber = cms.uint32(1)
)
cfgElectronEtaCut = cms.PSet(
    pluginName = cms.string('electronEtaCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1ElectronsEta21Cumulative'),
    src_individual = cms.InputTag('selectedLayer1ElectronsEta21Individual'),
    minNumber = cms.uint32(1)
)
cfgElectronPtCut = cms.PSet(
    pluginName = cms.string('electronPtCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1ElectronsPt15Cumulative'),
    src_individual = cms.InputTag('selectedLayer1ElectronsPt15Individual'),
    minNumber = cms.uint32(1)
)
cfgElectronTrkIsoCut = cms.PSet(
    pluginName = cms.string('electronTrkIsoCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1ElectronsTrkIsoCumulative'),
    src_individual = cms.InputTag('selectedLayer1ElectronsTrkIsoIndividual'),
    minNumber = cms.uint32(1)
)
cfgElectronEcalIsoCut = cms.PSet(
    pluginName = cms.string('electronEcalIsoCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1ElectronsEcalIsoCumulative'),
    src_individual = cms.InputTag('selectedLayer1ElectronsEcalIsoIndividual'),
    minNumber = cms.uint32(1)
)
cfgElectronTrkCut = cms.PSet(
    pluginName = cms.string('electronTrkCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1ElectronsTrkCumulative'),
    src_individual = cms.InputTag('selectedLayer1ElectronsTrkIndividual'),
    minNumber = cms.uint32(1)
)
cfgElectronTrkIPcut = cms.PSet(
    pluginName = cms.string('electronTrkIPcut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1ElectronsTrkIPcumulative'),
    src_individual = cms.InputTag('selectedLayer1ElectronsTrkIPindividual'),
    minNumber = cms.uint32(1)
)

# tau candidate selection
cfgTauAntiOverlapWithElectronsVeto = cms.PSet(
    pluginName = cms.string('tauAntiOverlapWithElectronsVeto'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1TausForElecTauAntiOverlapWithElectronsVetoCumulative'),
    src_individual = cms.InputTag('selectedLayer1TausForElecTauAntiOverlapWithElectronsVetoIndividual'),
    minNumber = cms.uint32(1)
)
cfgTauEtaCut = cms.PSet(
    pluginName = cms.string('tauEtaCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1TausForElecTauEta21Cumulative'),
    src_individual = cms.InputTag('selectedLayer1TausEta21Individual'),
    minNumber = cms.uint32(1)
)
cfgTauPtCut = cms.PSet(
    pluginName = cms.string('tauPtCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1TausForElecTauPt20Cumulative'),
    src_individual = cms.InputTag('selectedLayer1TausPt20Individual'),
    minNumber = cms.uint32(1)
)
cfgTauLeadTrkCut = cms.PSet(
    pluginName = cms.string('tauLeadTrkCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1TausForElecTauLeadTrkCumulative'),
    src_individual = cms.InputTag('selectedLayer1TausLeadTrkIndividual'),
    minNumber = cms.uint32(1)
)
cfgTauLeadTrkPtCut = cms.PSet(
    pluginName = cms.string('tauLeadTrkPtCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1TausForElecTauLeadTrkPtCumulative'),
    src_individual = cms.InputTag('selectedLayer1TausLeadTrkPtIndividual'),
    minNumber = cms.uint32(1)
)
cfgTauTrkIsoCut = cms.PSet(
    pluginName = cms.string('tauTrkIsoCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1TausForElecTauTrkIsoCumulative'),
    src_individual = cms.InputTag('selectedLayer1TausTrkIsoIndividual'),
    minNumber = cms.uint32(1)
)
cfgTauEcalIsoCut = cms.PSet(
    pluginName = cms.string('tauEcalIsoCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1TausForElecTauEcalIsoCumulative'),
    src_individual = cms.InputTag('selectedLayer1TausEcalIsoIndividual'),
    minNumber = cms.uint32(1)
)
cfgTauProngCut = cms.PSet(
    pluginName = cms.string('tauProngCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1TausForElecTauProngCumulative'),
    src_individual = cms.InputTag('selectedLayer1TausProngIndividual'),
    minNumber = cms.uint32(1)
)
cfgTauElectronVeto = cms.PSet(
    pluginName = cms.string('tauElectronVeto'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1TausForElecTauElectronVetoCumulative'),
    src_individual = cms.InputTag('selectedLayer1TausElectronVetoIndividual'),
    minNumber = cms.uint32(1)
)
cfgTauEcalCrackVeto = cms.PSet(
    pluginName = cms.string('tauEcalCrackVeto'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1TausForElecTauEcalCrackVetoCumulative'),
    src_individual = cms.InputTag('selectedLayer1TausEcalCrackVetoIndividual'),
    minNumber = cms.uint32(1)
)

# di-tau candidate selection
cfgDiTauCandidateForElecTauAntiOverlapVeto = cms.PSet(
    pluginName = cms.string('diTauCandidateForElecTauAntiOverlapVeto'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedElecTauPairsAntiOverlapVetoCumulative'),
    src_individual = cms.InputTag('selectedElecTauPairsAntiOverlapVetoIndividual'),
    minNumber = cms.uint32(1)
)
cfgDiTauCandidateForElecTauZeroChargeCut = cms.PSet(
    pluginName = cms.string('diTauCandidateForElecTauZeroChargeCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedElecTauPairsZeroChargeCumulative'),
    src_individual = cms.InputTag('selectedElecTauPairsZeroChargeIndividual'),
    minNumber = cms.uint32(1)
)
cfgDiTauCandidateForElecTauAcoplanarity12Cut = cms.PSet(
    pluginName = cms.string('diTauCandidateForElecTauAcoplanarity12Cut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedElecTauPairsAcoplanarity12Cumulative'),
    src_individual = cms.InputTag('selectedElecTauPairsAcoplanarity12Individual'),
    minNumber = cms.uint32(1)
)
cfgDiTauCandidateForElecTauMt1METCut = cms.PSet(
    pluginName = cms.string('diTauCandidateForElecTauMt1METCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedElecTauPairsMt1METcumulative'),
    src_individual = cms.InputTag('selectedElecTauPairsMt1METindividual'),
    minNumber = cms.uint32(1)
)
cfgDiTauCandidateForElecTauPzetaDiffCut = cms.PSet(
    pluginName = cms.string('diTauCandidateForElecTauPzetaDiffCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedElecTauPairsPzetaDiffCumulative'),
    src_individual = cms.InputTag('selectedElecTauPairsPzetaDiffIndividual'),
    minNumber = cms.uint32(1)
)

# veto events compatible with Z --> e+ e- hypothesis
# (based on reconstructed (visible) invariant mass of e + tau-jet pair)
cfgElecTauPairZeeHypothesisVeto = cms.PSet(
    pluginName = cms.string('elecTauPairZeeHypothesisVeto'),
    pluginType = cms.string('PATElecTauPairZeeHypothesisMaxEventSelector'),
    src = cms.InputTag('selectedElecTauPairZeeHypotheses'),
    maxNumber = cms.uint32(0)
)

zToElecTauEventSelConfigurator = eventSelFlagProdConfigurator(
    [ #cfgTrigger,
      cfgPrimaryEventVertex,
      cfgPrimaryEventVertexQuality,
      cfgPrimaryEventVertexPosition,
      cfgTightElectronIdCut,
      cfgElectronAntiCrackCut,
      cfgElectronEtaCut,
      cfgElectronPtCut,
      cfgElectronTrkIsoCut,
      cfgElectronEcalIsoCut,
      cfgElectronTrkCut,
      cfgElectronTrkIPcut,
      cfgTauAntiOverlapWithElectronsVeto,
      cfgTauEtaCut,
      cfgTauPtCut,
      cfgTauLeadTrkCut,
      cfgTauLeadTrkPtCut,
      cfgTauTrkIsoCut,
      cfgTauEcalIsoCut,
      cfgTauProngCut,
      cfgTauElectronVeto,
      cfgTauEcalCrackVeto,
      cfgDiTauCandidateForElecTauAntiOverlapVeto,
      cfgDiTauCandidateForElecTauZeroChargeCut,
      cfgDiTauCandidateForElecTauAcoplanarity12Cut,
      cfgDiTauCandidateForElecTauMt1METCut,
      cfgDiTauCandidateForElecTauPzetaDiffCut,
      cfgElecTauPairZeeHypothesisVeto ],
    boolEventSelFlagProducer = "BoolEventSelFlagProducer",
    pyModuleName = __name__
)

selectZtoElecTauEvents = zToElecTauEventSelConfigurator.configure()
