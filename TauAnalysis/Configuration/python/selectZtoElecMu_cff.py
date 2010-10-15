import FWCore.ParameterSet.Config as cms

from TauAnalysis.RecoTools.tools.eventSelFlagProdConfigurator import *

#--------------------------------------------------------------------------------
# define event selection criteria for Z --> e + mu channel
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

# electron candidate selection
cfgElectronIdMinCut = cms.PSet(
    pluginName = cms.string('electronIdMinCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatElectronsIdCumulative'),
    src_individual = cms.InputTag('selectedPatElectronsIdIndividual'),
    minNumber = cms.uint32(1)
)
cfgElectronIdMaxCut = cms.PSet(
    pluginName = cms.string('electronIdMaxCut'),
    pluginType = cms.string('PATCandViewMaxEventSelector'),
    src_cumulative = cms.InputTag('selectedPatElectronsIdCumulative'),
    src_individual = cms.InputTag('selectedPatElectronsIdIndividual'),
    maxNumber = cms.uint32(1)
)
cfgElectronAntiCrackCut = cms.PSet(
    pluginName = cms.string('electronAntiCrackCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatElectronsAntiCrackCutCumulative'),
    src_individual = cms.InputTag('selectedPatElectronsAntiCrackCutIndividual'),
    minNumber = cms.uint32(1)
)
cfgElectronEtaCut = cms.PSet(
    pluginName = cms.string('electronEtaCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatElectronsEtaCumulative'),
    src_individual = cms.InputTag('selectedPatElectronsEtaIndividual'),
    minNumber = cms.uint32(1)
)
cfgElectronPtCut = cms.PSet(
    pluginName = cms.string('electronPtCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatElectronsPtCumulative'),
    src_individual = cms.InputTag('selectedPatElectronsPtIndividual'),
    minNumber = cms.uint32(1)
)
cfgElectronIsoCut = cms.PSet(
    pluginName = cms.string('electronIsoCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatElectronsIsoCumulative'),
    src_individual = cms.InputTag('selectedPatElectronsIsoIndividual'),
    minNumber = cms.uint32(1)
)
cfgElectronTrkCut = cms.PSet(
    pluginName = cms.string('electronTrkCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatElectronsTrkCumulative'),
    src_individual = cms.InputTag('selectedPatElectronsTrkIndividual'),
    minNumber = cms.uint32(1)
)
cfgElectronTrkIPCut = cms.PSet(
    pluginName = cms.string('electronTrkIPCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatElectronsTrkIPcumulative'),
    src_individual = cms.InputTag('selectedPatElectronsTrkIPindividual'),
    minNumber = cms.uint32(1)
)

# muon candidate selection
cfgGlobalMuonMinCut = cms.PSet(
    pluginName = cms.string('globalMuonMinCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatMuonsGlobalCumulative'),
    src_individual = cms.InputTag('selectedPatMuonsGlobalIndividual'),
    minNumber = cms.uint32(1)
)
cfgGlobalMuonMaxCut = cms.PSet(
    pluginName = cms.string('globalMuonMaxCut'),
    pluginType = cms.string('PATCandViewMaxEventSelector'),
    src_cumulative = cms.InputTag('selectedPatMuonsGlobalCumulative'),
    src_individual = cms.InputTag('selectedPatMuonsGlobalIndividual'),
    maxNumber = cms.uint32(1)
)
cfgMuonEtaCut = cms.PSet(
    pluginName = cms.string('muonEtaCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatMuonsEtaCumulative'),
    src_individual = cms.InputTag('selectedPatMuonsEtaIndividual'),
    minNumber = cms.uint32(1)
)
cfgMuonPtCut = cms.PSet(
    pluginName = cms.string('muonPtCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatMuonsPtCumulative'),
    src_individual = cms.InputTag('selectedPatMuonsPtIndividual'),
    minNumber = cms.uint32(1)
)
cfgMuonIsoCut = cms.PSet(
    pluginName = cms.string('muonIsoCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatMuonsIsoCumulative'),
    src_individual = cms.InputTag('selectedPatMuonsIsoIndividual'),
    minNumber = cms.uint32(1)
)
cfgMuonAntiPionCut = cms.PSet(
    pluginName = cms.string('muonAntiPionCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatMuonsPionVetoCumulative'),
    src_individual = cms.InputTag('selectedPatMuonsPionVetoIndividual'),
    minNumber = cms.uint32(1)
)
cfgMuonTrkIPCut = cms.PSet(
    pluginName = cms.string('muonTrkIPCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatMuonsTrkIPcumulative'),
    src_individual = cms.InputTag('selectedPatMuonsTrkIPindividual'),
    minNumber = cms.uint32(1)
)

# di-tau candidate selection
cfgDiTauCandidateForElecMuDRminCut = cms.PSet(
    pluginName = cms.string('diTauCandidateForElecMuDRminCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedElecMuPairsDRminCumulative'),
    src_individual = cms.InputTag('selectedElecMuPairsDRminIndividual'),
    minNumber = cms.uint32(1)
)
cfgDiTauCandidateForElecMuZeroChargeCut = cms.PSet(
    pluginName = cms.string('diTauCandidateForElecMuZeroChargeCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedElecMuPairsZeroChargeCumulative'),
    src_individual = cms.InputTag('selectedElecMuPairsZeroChargeIndividual'),
    minNumber = cms.uint32(1)
)
cfgDiTauCandidateForElecMuMt1METCut = cms.PSet(
    pluginName = cms.string('diTauCandidateForElecMuMt1METCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedElecMuPairsMt1METcumulative'),
    src_individual = cms.InputTag('selectedElecMuPairsMt1METindividual'),
    minNumber = cms.uint32(1)
)
cfgDiTauCandidateForElecMuMt2METCut = cms.PSet(
    pluginName = cms.string('diTauCandidateForElecMuMt2METCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedElecMuPairsMt2METcumulative'),
    src_individual = cms.InputTag('selectedElecMuPairsMt2METindividual'),
    minNumber = cms.uint32(1)
)
cfgDiTauCandidateForElecMuDPhiCut = cms.PSet(
    pluginName = cms.string('diTauCandidateForElecMuDPhiCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedElecMuPairsDPhiCumulative'),
    src_individual = cms.InputTag('selectedElecMuPairsDPhiIndividual'),
    minNumber = cms.uint32(1)
)
cfgDiTauCandidateForElecMuOneLegPtCut = cms.PSet(
    pluginName = cms.string('diTauCandidateForElecMuOneLegPtCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedElecMuPairsOneLegPtCumulative'),
    src_individual = cms.InputTag('selectedElecMuPairsOneLegPtIndividual'),
    minNumber = cms.uint32(1)
)

# jet candidate selection
cfgJetMinCut = cms.PSet(
    pluginName = cms.string('jetMinCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('selectedPatJetsAntiOverlapWithLeptonsVetoCumulative'),
    minNumber = cms.uint32(0)
)
cfgJetMaxCut = cms.PSet(
    pluginName = cms.string('jetMaxCut'),
    pluginType = cms.string('PATCandViewMaxEventSelector'),
    src = cms.InputTag('selectedPatJetsAntiOverlapWithLeptonsVetoCumulative'),
    maxNumber = cms.uint32(2)
)
cfgJetBtagMinCut = cms.PSet(
    pluginName = cms.string('jetBtagMinCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('selectedPatJetsBtagCumulative'),
    minNumber = cms.uint32(0)
)
cfgJetBtagMaxCut = cms.PSet(
    pluginName = cms.string('jetBtagMaxCut'),
    pluginType = cms.string('PATCandViewMaxEventSelector'),
    src = cms.InputTag('selectedPatJetsBtagCumulative'),
    maxNumber = cms.uint32(1)
)

# met selection
cfgMETMaxCut = cms.PSet(
    pluginName = cms.string('metMaxCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('selectedMETMaxCumulative'),
    minNumber = cms.uint32(1)
)

zToElecMuEventSelConfigurator = eventSelFlagProdConfigurator(
    [ cfgTrigger,
      cfgPrimaryEventVertex,
      cfgPrimaryEventVertexQuality,
      cfgPrimaryEventVertexPosition,
      cfgElectronIdMinCut,
      cfgElectronIdMaxCut,
      cfgElectronAntiCrackCut,
      cfgElectronEtaCut,
      cfgElectronPtCut,
      cfgElectronIsoCut,
      cfgElectronTrkCut,
      #cfgElectronTrkIPCut,
      cfgGlobalMuonMinCut,
      cfgGlobalMuonMaxCut,
      cfgMuonEtaCut,
      cfgMuonPtCut,
      cfgMuonIsoCut,
      cfgMuonAntiPionCut,
      #cfgMuonTrkIPCut,
      cfgDiTauCandidateForElecMuDRminCut,
      cfgDiTauCandidateForElecMuZeroChargeCut,
      cfgDiTauCandidateForElecMuMt1METCut,
      cfgDiTauCandidateForElecMuMt2METCut,
      cfgDiTauCandidateForElecMuDPhiCut,
      #cfgDiTauCandidateForElecMuOneLegPtCut,
      cfgMETMaxCut,
      cfgJetMinCut,
      cfgJetMaxCut,
      cfgJetBtagMinCut,
      cfgJetBtagMaxCut
      ],
    boolEventSelFlagProducer = "BoolEventSelFlagProducer",
    pyModuleName = __name__
)

selectZtoElecMuEvents = zToElecMuEventSelConfigurator.configure()

