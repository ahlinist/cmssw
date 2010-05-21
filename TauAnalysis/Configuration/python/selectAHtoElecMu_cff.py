import FWCore.ParameterSet.Config as cms

from TauAnalysis.RecoTools.tools.eventSelFlagProdConfigurator import *

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
cfgElectronTrkIsoCut = cms.PSet(
    pluginName = cms.string('electronTrkIsoCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatElectronsTrkIsoCumulative'),
    src_individual = cms.InputTag('selectedPatElectronsTrkIsoIndividual'),
    minNumber = cms.uint32(1)
)
cfgElectronEcalIsoCut = cms.PSet(
    pluginName = cms.string('electronEcalIsoCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatElectronsEcalIsoCumulative'),
    src_individual = cms.InputTag('selectedPatElectronsEcalIsoIndividual'),
    minNumber = cms.uint32(1)
)
cfgElectronHcalIsoCut = cms.PSet(
    pluginName = cms.string('electronHcalIsoCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatElectronsHcalIsoCumulative'),
    src_individual = cms.InputTag('selectedPatElectronsHcalIsoIndividual'),
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
cfgMuonTrkIsoCut = cms.PSet(
    pluginName = cms.string('muonTrkIsoCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatMuonsTrkIsoCumulative'),
    src_individual = cms.InputTag('selectedPatMuonsTrkIsoIndividual'),
    minNumber = cms.uint32(1)
)
cfgMuonEcalIsoCut = cms.PSet(
    pluginName = cms.string('muonEcalIsoCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatMuonsEcalIsoCumulative'),
    src_individual = cms.InputTag('selectedPatMuonsEcalIsoIndividual'),
    minNumber = cms.uint32(1)
)
cfgMuonHcalIsoCut = cms.PSet(
    pluginName = cms.string('muonHcalIsoCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatMuonsHcalIsoCumulative'),
    src_individual = cms.InputTag('selectedPatMuonsHcalIsoIndividual'),
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
    src = cms.InputTag('selectedPatJetsEtaCumulative'),
    minNumber = cms.uint32(0)
)
cfgJetMaxCut = cms.PSet(
    pluginName = cms.string('jetMaxCut'),
    pluginType = cms.string('PATCandViewMaxEventSelector'),
    src = cms.InputTag('selectedPatJetsEtaCumulative'),
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

# tune iso cut
cfgElectronIso005Cut = cms.PSet(
    pluginName = cms.string('electronIso005Cut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatElectronsIso005Cumulative'),
    src_individual = cms.InputTag('selectedPatElectronsIso005Individual'),
    minNumber = cms.uint32(1)
)
cfgElectronIso010Cut = cms.PSet(
    pluginName = cms.string('electronIso010Cut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatElectronsIso010Cumulative'),
    src_individual = cms.InputTag('selectedPatElectronsIso010Individual'),
    minNumber = cms.uint32(1)
)
cfgElectronIso015Cut = cms.PSet(
    pluginName = cms.string('electronIso015Cut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatElectronsIso015Cumulative'),
    src_individual = cms.InputTag('selectedPatElectronsIso015Individual'),
    minNumber = cms.uint32(1)
)
cfgElectronIso020Cut = cms.PSet(
    pluginName = cms.string('electronIso020Cut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatElectronsIso020Cumulative'),
    src_individual = cms.InputTag('selectedPatElectronsIso020Individual'),
    minNumber = cms.uint32(1)
)
cfgElectronIso025Cut = cms.PSet(
    pluginName = cms.string('electronIso025Cut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatElectronsIso025Cumulative'),
    src_individual = cms.InputTag('selectedPatElectronsIso025Individual'),
    minNumber = cms.uint32(1)
)
cfgElectronIso030Cut = cms.PSet(
    pluginName = cms.string('electronIso030Cut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatElectronsIso030Cumulative'),
    src_individual = cms.InputTag('selectedPatElectronsIso030Individual'),
    minNumber = cms.uint32(1)
)
cfgMuonIso005Cut = cms.PSet(
    pluginName = cms.string('muonIso005Cut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatMuonsIso005Cumulative'),
    src_individual = cms.InputTag('selectedPatMuonsIso005Individual'),
    minNumber = cms.uint32(1)
)
cfgMuonIso010Cut = cms.PSet(
    pluginName = cms.string('muonIso010Cut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatMuonsIso010Cumulative'),
    src_individual = cms.InputTag('selectedPatMuonsIso010Individual'),
    minNumber = cms.uint32(1)
)
cfgMuonIso015Cut = cms.PSet(
    pluginName = cms.string('muonIso015Cut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatMuonsIso015Cumulative'),
    src_individual = cms.InputTag('selectedPatMuonsIso015Individual'),
    minNumber = cms.uint32(1)
)
cfgMuonIso020Cut = cms.PSet(
    pluginName = cms.string('muonIso020Cut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatMuonsIso020Cumulative'),
    src_individual = cms.InputTag('selectedPatMuonsIso020Individual'),
    minNumber = cms.uint32(1)
)
cfgMuonIso025Cut = cms.PSet(
    pluginName = cms.string('muonIso025Cut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatMuonsIso025Cumulative'),
    src_individual = cms.InputTag('selectedPatMuonsIso025Individual'),
    minNumber = cms.uint32(1)
)
cfgMuonIso030Cut = cms.PSet(
    pluginName = cms.string('muonIso030Cut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatMuonsIso030Cumulative'),
    src_individual = cms.InputTag('selectedPatMuonsIso030Individual'),
    minNumber = cms.uint32(1)
)

# tune pt cut
cfgElectronPt10Cut = cms.PSet(
    pluginName = cms.string('electronPt10Cut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatElectronsPt10Cumulative'),
    src_individual = cms.InputTag('selectedPatElectronsPt10Individual'),
    minNumber = cms.uint32(1)
)
cfgElectronPt11Cut = cms.PSet(
    pluginName = cms.string('electronPt11Cut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatElectronsPt11Cumulative'),
    src_individual = cms.InputTag('selectedPatElectronsPt11Individual'),
    minNumber = cms.uint32(1)
)
cfgElectronPt12Cut = cms.PSet(
    pluginName = cms.string('electronPt12Cut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatElectronsPt12Cumulative'),
    src_individual = cms.InputTag('selectedPatElectronsPt12Individual'),
    minNumber = cms.uint32(1)
)
cfgElectronPt13Cut = cms.PSet(
    pluginName = cms.string('electronPt13Cut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatElectronsPt13Cumulative'),
    src_individual = cms.InputTag('selectedPatElectronsPt13Individual'),
    minNumber = cms.uint32(1)
)
cfgElectronPt14Cut = cms.PSet(
    pluginName = cms.string('electronPt14Cut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatElectronsPt14Cumulative'),
    src_individual = cms.InputTag('selectedPatElectronsPt14Individual'),
    minNumber = cms.uint32(1)
)
cfgElectronPt15Cut = cms.PSet(
    pluginName = cms.string('electronPt15Cut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatElectronsPt15Cumulative'),
    src_individual = cms.InputTag('selectedPatElectronsPt15Individual'),
    minNumber = cms.uint32(1)
)
cfgElectronPt16Cut = cms.PSet(
    pluginName = cms.string('electronPt16Cut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatElectronsPt16Cumulative'),
    src_individual = cms.InputTag('selectedPatElectronsPt16Individual'),
    minNumber = cms.uint32(1)
)
cfgElectronPt17Cut = cms.PSet(
    pluginName = cms.string('electronPt17Cut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatElectronsPt17Cumulative'),
    src_individual = cms.InputTag('selectedPatElectronsPt17Individual'),
    minNumber = cms.uint32(1)
)
cfgElectronPt18Cut = cms.PSet(
    pluginName = cms.string('electronPt18Cut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatElectronsPt18Cumulative'),
    src_individual = cms.InputTag('selectedPatElectronsPt18Individual'),
    minNumber = cms.uint32(1)
)
cfgElectronPt19Cut = cms.PSet(
    pluginName = cms.string('electronPt19Cut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatElectronsPt19Cumulative'),
    src_individual = cms.InputTag('selectedPatElectronsPt19Individual'),
    minNumber = cms.uint32(1)
)
cfgElectronPt20Cut = cms.PSet(
    pluginName = cms.string('electronPt20Cut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatElectronsPt20Cumulative'),
    src_individual = cms.InputTag('selectedPatElectronsPt20Individual'),
    minNumber = cms.uint32(1)
)
cfgMuonPt10Cut = cms.PSet(
    pluginName = cms.string('muonPt10Cut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatMuonsPt10Cumulative'),
    src_individual = cms.InputTag('selectedPatMuonsPt10Individual'),
    minNumber = cms.uint32(1)
)
cfgMuonPt11Cut = cms.PSet(
    pluginName = cms.string('muonPt11Cut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatMuonsPt11Cumulative'),
    src_individual = cms.InputTag('selectedPatMuonsPt11Individual'),
    minNumber = cms.uint32(1)
)
cfgMuonPt12Cut = cms.PSet(
    pluginName = cms.string('muonPt12Cut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatMuonsPt12Cumulative'),
    src_individual = cms.InputTag('selectedPatMuonsPt12Individual'),
    minNumber = cms.uint32(1)
)
cfgMuonPt13Cut = cms.PSet(
    pluginName = cms.string('muonPt13Cut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatMuonsPt13Cumulative'),
    src_individual = cms.InputTag('selectedPatMuonsPt13Individual'),
    minNumber = cms.uint32(1)
)
cfgMuonPt14Cut = cms.PSet(
    pluginName = cms.string('muonPt14Cut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatMuonsPt14Cumulative'),
    src_individual = cms.InputTag('selectedPatMuonsPt14Individual'),
    minNumber = cms.uint32(1)
)
cfgMuonPt15Cut = cms.PSet(
    pluginName = cms.string('muonPt15Cut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatMuonsPt15Cumulative'),
    src_individual = cms.InputTag('selectedPatMuonsPt15Individual'),
    minNumber = cms.uint32(1)
)
cfgMuonPt16Cut = cms.PSet(
    pluginName = cms.string('muonPt16Cut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatMuonsPt16Cumulative'),
    src_individual = cms.InputTag('selectedPatMuonsPt16Individual'),
    minNumber = cms.uint32(1)
)
cfgMuonPt17Cut = cms.PSet(
    pluginName = cms.string('muonPt17Cut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatMuonsPt17Cumulative'),
    src_individual = cms.InputTag('selectedPatMuonsPt17Individual'),
    minNumber = cms.uint32(1)
)
cfgMuonPt18Cut = cms.PSet(
    pluginName = cms.string('muonPt18Cut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatMuonsPt18Cumulative'),
    src_individual = cms.InputTag('selectedPatMuonsPt18Individual'),
    minNumber = cms.uint32(1)
)
cfgMuonPt19Cut = cms.PSet(
    pluginName = cms.string('muonPt19Cut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatMuonsPt19Cumulative'),
    src_individual = cms.InputTag('selectedPatMuonsPt19Individual'),
    minNumber = cms.uint32(1)
)
cfgMuonPt20Cut = cms.PSet(
    pluginName = cms.string('muonPt20Cut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatMuonsPt20Cumulative'),
    src_individual = cms.InputTag('selectedPatMuonsPt20Individual'),
    minNumber = cms.uint32(1)
)

# tune electron id
cfgElectronLooseIdMinCut = cms.PSet(
    pluginName = cms.string('electronLooseIdMinCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatElectronsLooseIdCumulative'),
    src_individual = cms.InputTag('selectedPatElectronsLooseIdIndividual'),
    minNumber = cms.uint32(1)
)
cfgElectronTightIdMinCut = cms.PSet(
    pluginName = cms.string('electronTightIdMinCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatElectronsTightIdCumulative'),
    src_individual = cms.InputTag('selectedPatElectronsTightIdIndividual'),
    minNumber = cms.uint32(1)
)
cfgElectronRobustIdMinCut = cms.PSet(
    pluginName = cms.string('electronRobustIdMinCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatElectronsRobustIdCumulative'),
    src_individual = cms.InputTag('selectedPatElectronsRobustIdIndividual'),
    minNumber = cms.uint32(1)
)

## # tune one leg pt cut
cfgDiTauCandidateForElecMuOneLegPt18Cut = cms.PSet(
    pluginName = cms.string('diTauCandidateForElecMuOneLegPt18Cut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedElecMuPairsOneLegPt18Cumulative'),
    src_individual = cms.InputTag('selectedElecMuPairsOneLegPt18Individual'),
    minNumber = cms.uint32(1)
)
cfgDiTauCandidateForElecMuOneLegPt20Cut = cms.PSet(
    pluginName = cms.string('diTauCandidateForElecMuOneLegPt20Cut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedElecMuPairsOneLegPt20Cumulative'),
    src_individual = cms.InputTag('selectedElecMuPairsOneLegPt20Individual'),
    minNumber = cms.uint32(1)
)
cfgDiTauCandidateForElecMuOneLegPt22Cut = cms.PSet(
    pluginName = cms.string('diTauCandidateForElecMuOneLegPt22Cut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedElecMuPairsOneLegPt22Cumulative'),
    src_individual = cms.InputTag('selectedElecMuPairsOneLegPt22Individual'),
    minNumber = cms.uint32(1)
)
cfgDiTauCandidateForElecMuOneLegPt24Cut = cms.PSet(
    pluginName = cms.string('diTauCandidateForElecMuOneLegPt24Cut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedElecMuPairsOneLegPt24Cumulative'),
    src_individual = cms.InputTag('selectedElecMuPairsOneLegPt24Individual'),
    minNumber = cms.uint32(1)
)
cfgDiTauCandidateForElecMuOneLegPt26Cut = cms.PSet(
    pluginName = cms.string('diTauCandidateForElecMuOneLegPt26Cut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedElecMuPairsOneLegPt26Cumulative'),
    src_individual = cms.InputTag('selectedElecMuPairsOneLegPt26Individual'),
    minNumber = cms.uint32(1)
)
cfgDiTauCandidateForElecMuOneLegPt28Cut = cms.PSet(
    pluginName = cms.string('diTauCandidateForElecMuOneLegPt28Cut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedElecMuPairsOneLegPt28Cumulative'),
    src_individual = cms.InputTag('selectedElecMuPairsOneLegPt28Individual'),
    minNumber = cms.uint32(1)
)
cfgDiTauCandidateForElecMuOneLegPt30Cut = cms.PSet(
    pluginName = cms.string('diTauCandidateForElecMuOneLegPt30Cut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedElecMuPairsOneLegPt30Cumulative'),
    src_individual = cms.InputTag('selectedElecMuPairsOneLegPt30Individual'),
    minNumber = cms.uint32(1)
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
      #cfgDiTauCandidateForElecMuAcoplanarityCut,
      cfgDiTauCandidateForElecMuDPhiCut,
      #cfgDiTauCandidateForElecMuImpParamSigCut,
      cfgDiTauCandidateForElecMuOneLegPtCut,
      cfgMETMaxCut,
      cfgJetMinCut,
      cfgJetMaxCut,
      cfgJetBtagMinCut,
      cfgJetBtagMaxCut
##       cfgElectronIso005Cut,
##       cfgElectronIso010Cut,
##       cfgElectronIso015Cut,
##       cfgElectronIso020Cut,
##       cfgElectronIso025Cut,
##       cfgElectronIso030Cut,
##       cfgMuonIso005Cut,
##       cfgMuonIso010Cut,
##       cfgMuonIso015Cut,
##       cfgMuonIso020Cut,
##       cfgMuonIso025Cut,
##       cfgMuonIso030Cut,
##       cfgElectronPt10Cut,
##       cfgElectronPt11Cut,
##       cfgElectronPt12Cut,
##       cfgElectronPt13Cut,
##       cfgElectronPt14Cut,
##       cfgElectronPt15Cut,
##       cfgElectronPt16Cut,
##       cfgElectronPt17Cut,
##       cfgElectronPt18Cut,
##       cfgElectronPt19Cut,
##       cfgElectronPt20Cut,
##       cfgMuonPt10Cut,
##       cfgMuonPt11Cut,
##       cfgMuonPt12Cut,
##       cfgMuonPt13Cut,
##       cfgMuonPt14Cut,
##       cfgMuonPt15Cut,
##       cfgMuonPt16Cut,
##       cfgMuonPt17Cut,
##       cfgMuonPt18Cut,
##       cfgMuonPt19Cut,
##       cfgMuonPt20Cut,
##       cfgElectronLooseIdMinCut,
##       cfgElectronTightIdMinCut,
##       cfgElectronRobustIdMinCut,
##       cfgDiTauCandidateForElecMuOneLegPt18Cut,
##       cfgDiTauCandidateForElecMuOneLegPt20Cut,
##       cfgDiTauCandidateForElecMuOneLegPt22Cut,
##       cfgDiTauCandidateForElecMuOneLegPt24Cut,
##       cfgDiTauCandidateForElecMuOneLegPt26Cut,
##       cfgDiTauCandidateForElecMuOneLegPt28Cut,
##       cfgDiTauCandidateForElecMuOneLegPt30Cut
      ],
    boolEventSelFlagProducer = "BoolEventSelFlagProducer",
    pyModuleName = __name__
)

selectAHtoElecMuEvents = ahToElecMuEventSelConfigurator.configure()

