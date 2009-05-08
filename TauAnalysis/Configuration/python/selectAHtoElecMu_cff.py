import FWCore.ParameterSet.Config as cms

from TauAnalysis.RecoTools.eventSelFlagProdConfigurator import *

#--------------------------------------------------------------------------------
# define event selection criteria for AH --> e + mu channel
#--------------------------------------------------------------------------------

# trigger selection
cfgTrigger = cms.PSet(
    pluginName = cms.string('Trigger'),
    pluginType = cms.string('TriggerResultEventSelector'),
    src = cms.InputTag('TriggerResults::HLT'),
    triggerPaths = cms.vstring('HLT_IsoEle15_L1I', 'HLT_Mu15', 'HLT_IsoMu11', 'HLT_IsoEle8_IsoMu7')
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
cfgElectronIdMinCut = cms.PSet(
    pluginName = cms.string('electronIdMinCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1ElectronsTightIdCumulative'),
    src_individual = cms.InputTag('selectedLayer1ElectronsTightIdIndividual'),
    minNumber = cms.uint32(1)
)
cfgElectronIdMaxCut = cms.PSet(
    pluginName = cms.string('electronIdMaxCut'),
    pluginType = cms.string('PATCandViewMaxEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1ElectronsTightIdCumulative'),
    src_individual = cms.InputTag('selectedLayer1ElectronsTightIdIndividual'),
    maxNumber = cms.uint32(1)
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
cfgElectronIsoCut = cms.PSet(
    pluginName = cms.string('electronIsoCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1ElectronsIsoCumulative'),
    src_individual = cms.InputTag('selectedLayer1ElectronsIsoIndividual'),
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
cfgElectronHcalIsoCut = cms.PSet(
    pluginName = cms.string('electronHcalIsoCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1ElectronsHcalIsoCumulative'),
    src_individual = cms.InputTag('selectedLayer1ElectronsHcalIsoIndividual'),
    minNumber = cms.uint32(1)
)
cfgElectronTrkCut = cms.PSet(
    pluginName = cms.string('electronTrkCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1ElectronsTrkCumulative'),
    src_individual = cms.InputTag('selectedLayer1ElectronsTrkIndividual'),
    minNumber = cms.uint32(1)
)
cfgElectronTrkIPCut = cms.PSet(
    pluginName = cms.string('electronTrkIPCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1ElectronsTrkIPcumulative'),
    src_individual = cms.InputTag('selectedLayer1ElectronsTrkIPindividual'),
    minNumber = cms.uint32(1)
)

# muon candidate selection
cfgGlobalMuonMinCut = cms.PSet(
    pluginName = cms.string('globalMuonMinCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1MuonsGlobalCumulative'),
    src_individual = cms.InputTag('selectedLayer1MuonsGlobalIndividual'),
    minNumber = cms.uint32(1)
)
cfgGlobalMuonMaxCut = cms.PSet(
    pluginName = cms.string('globalMuonMaxCut'),
    pluginType = cms.string('PATCandViewMaxEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1MuonsGlobalCumulative'),
    src_individual = cms.InputTag('selectedLayer1MuonsGlobalIndividual'),
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
cfgMuonIsoCut = cms.PSet(
    pluginName = cms.string('muonIsoCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1MuonsIsoCumulative'),
    src_individual = cms.InputTag('selectedLayer1MuonsIsoIndividual'),
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
cfgMuonHcalIsoCut = cms.PSet(
    pluginName = cms.string('muonHcalIsoCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1MuonsHcalIsoCumulative'),
    src_individual = cms.InputTag('selectedLayer1MuonsHcalIsoIndividual'),
    minNumber = cms.uint32(1)
)
cfgMuonAntiPionCut = cms.PSet(
    pluginName = cms.string('muonAntiPionCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1MuonsPionVetoCumulative'),
    src_individual = cms.InputTag('selectedLayer1MuonsPionVetoIndividual'),
    minNumber = cms.uint32(1)
)
cfgMuonTrkIPCut = cms.PSet(
    pluginName = cms.string('muonTrkIPCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1MuonsTrkIPcumulative'),
    src_individual = cms.InputTag('selectedLayer1MuonsTrkIPindividual'),
    minNumber = cms.uint32(1)
)

# di-tau candidate selection
cfgDiTauCandidateForElecMuZeroChargeCut = cms.PSet(
    pluginName = cms.string('diTauCandidateForElecMuZeroChargeCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedElecMuPairsZeroChargeCumulative'),
    src_individual = cms.InputTag('selectedElecMuPairsZeroChargeIndividual'),
    minNumber = cms.uint32(1)
)
cfgDiTauCandidateForElecMuAcoplanarityCut = cms.PSet(
    pluginName = cms.string('diTauCandidateForElecMuAcoplanarityCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedElecMuPairsAcoplanarityCumulative'),
    src_individual = cms.InputTag('selectedElecMuPairsAcoplanarityIndividual'),
    minNumber = cms.uint32(1)
)
cfgDiTauCandidateForElecMuDPhiCut = cms.PSet(
    pluginName = cms.string('diTauCandidateForElecMuDPhiCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedElecMuPairsDPhiCumulative'),
    src_individual = cms.InputTag('selectedElecMuPairsDPhiIndividual'),
    minNumber = cms.uint32(1)
)
cfgDiTauCandidateForElecMuImpParamSigCut = cms.PSet(
    pluginName = cms.string('diTauCandidateForElecMuImpParamSigCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedElecMuPairsImpParamSigCumulative'),
    src_individual = cms.InputTag('selectedElecMuPairsImpParamSigIndividual'),
    minNumber = cms.uint32(1)
)

# jet candidate selection
cfgJetMinCut = cms.PSet(
    pluginName = cms.string('jetMinCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('selectedLayer1JetsEta21Cumulative'),
    minNumber = cms.uint32(0)
)
cfgJetMaxCut = cms.PSet(
    pluginName = cms.string('jetMaxCut'),
    pluginType = cms.string('PATCandViewMaxEventSelector'),
    src = cms.InputTag('selectedLayer1JetsEta21Cumulative'),
    maxNumber = cms.uint32(2)
)
cfgJetBtagMinCut = cms.PSet(
    pluginName = cms.string('jetBtagMinCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('selectedLayer1JetsBtagCumulative'),
    minNumber = cms.uint32(0)
)
cfgJetBtagMaxCut = cms.PSet(
    pluginName = cms.string('jetBtagMaxCut'),
    pluginType = cms.string('PATCandViewMaxEventSelector'),
    src = cms.InputTag('selectedLayer1JetsBtagCumulative'),
    maxNumber = cms.uint32(1)
)

ahToElecMuEventSelConfigurator = eventSelFlagProdConfigurator(
    [ cfgTrigger,
      cfgPrimaryEventVertex,
      cfgPrimaryEventVertexQuality,
      cfgPrimaryEventVertexPosition,
      cfgElectronIdMinCut,
      #cfgElectronIdMaxCut,
      cfgElectronAntiCrackCut,
      cfgElectronEtaCut,
      cfgElectronPtCut,
      cfgElectronIsoCut,
      #cfgElectronTrkIsoCut,
      #cfgElectronEcalIsoCut,
      #cfgElectronHcalIsoCut,
      cfgElectronTrkCut,
      #cfgElectronTrkIPCut,
      cfgGlobalMuonMinCut,
      #cfgGlobalMuonMaxCut,
      cfgMuonEtaCut,
      cfgMuonPtCut,
      cfgMuonIsoCut,
      #cfgMuonTrkIsoCut,
      #cfgMuonEcalIsoCut,
      #cfgMuonHcalIsoCut,
      cfgMuonAntiPionCut,
      #cfgMuonTrkIPCut,
      cfgDiTauCandidateForElecMuZeroChargeCut,
      cfgDiTauCandidateForElecMuAcoplanarityCut,
      #cfgDiTauCandidateForElecMuDPhiCut,
      #cfgDiTauCandidateForElecMuImpParamSigCut,
      cfgJetMinCut,
      cfgJetMaxCut,
      cfgJetBtagMinCut,
      cfgJetBtagMaxCut ],
    boolEventSelFlagProducer = "BoolEventSelFlagProducer",
    pyModuleName = __name__
)

selectAHtoElecMuEvents = ahToElecMuEventSelConfigurator.configure()

