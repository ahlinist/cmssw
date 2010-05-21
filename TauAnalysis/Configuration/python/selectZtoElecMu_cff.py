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
    triggerPaths = cms.vstring('HLT_Ele15_SW_EleId_L1R', 'HLT_Ele15_SW_LooseTrackIso_L1R', 'HLT_Mu9')
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
cfgElectronAntiOverlapWithMuonsVeto = cms.PSet(
    pluginName = cms.string('electronAntiOverlapWithMuonsVeto'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatElectronsForElecMuAntiOverlapWithMuonsVetoCumulative'),
    src_individual = cms.InputTag('selectedPatElectronsForElecMuAntiOverlapWithMuonsVetoIndividual'),
    minNumber = cms.uint32(1)
)
cfgTightElectronIdCut = cms.PSet(
    pluginName = cms.string('tightElectronIdCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatElectronsForElecMuTightIdCumulative'),
    src_individual = cms.InputTag('selectedPatElectronsForElecMuTightIdIndividual'),
    minNumber = cms.uint32(1)
)
cfgElectronAntiCrackCut = cms.PSet(
    pluginName = cms.string('electronAntiCrackCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatElectronsForElecMuAntiCrackCutCumulative'),
    src_individual = cms.InputTag('selectedPatElectronsForElecMuAntiCrackCutIndividual'),
    minNumber = cms.uint32(1)
)
cfgElectronEtaCut = cms.PSet(
    pluginName = cms.string('electronEtaCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatElectronsForElecMuEta21Cumulative'),
    src_individual = cms.InputTag('selectedPatElectronsForElecMuEta21Individual'),
    minNumber = cms.uint32(1)
)
cfgElectronPtCut = cms.PSet(
    pluginName = cms.string('electronPtCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatElectronsForElecMuPt15Cumulative'),
    src_individual = cms.InputTag('selectedPatElectronsForElecMuPt15Individual'),
    minNumber = cms.uint32(1)
)
cfgElectronTrkIsoCut = cms.PSet(
    pluginName = cms.string('electronTrkIsoCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatElectronsForElecMuTrkIsoCumulative'),
    src_individual = cms.InputTag('selectedPatElectronsForElecMuTrkIsoIndividual'),
    minNumber = cms.uint32(1)
)
cfgElectronEcalIsoCut = cms.PSet(
    pluginName = cms.string('electronEcalIsoCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatElectronsForElecMuEcalIsoCumulative'),
    src_individual = cms.InputTag('selectedPatElectronsForElecMuEcalIsoIndividual'),
    minNumber = cms.uint32(1)
)
cfgElectronTrkCut = cms.PSet(
    pluginName = cms.string('electronTrkCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatElectronsForElecMuTrkCumulative'),
    src_individual = cms.InputTag('selectedPatElectronsForElecMuTrkIndividual'),
    minNumber = cms.uint32(1)
)
cfgElectronTrkIPcut = cms.PSet(
    pluginName = cms.string('electronTrkIPcut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatElectronsForElecMuTrkIPcumulative'),
    src_individual = cms.InputTag('selectedPatElectronsForElecMuTrkIPindividual'),
    minNumber = cms.uint32(1)
)

# muon candidate selection
cfgGlobalMuonCut = cms.PSet(
    pluginName = cms.string('globalMuonCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatMuonsGlobalCumulative'),
    src_individual = cms.InputTag('selectedPatMuonsGlobalIndividual'),
    minNumber = cms.uint32(1)
)
cfgMuonEtaCut = cms.PSet(
    pluginName = cms.string('muonEtaCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatMuonsEta21Cumulative'),
    src_individual = cms.InputTag('selectedPatMuonsEta21Individual'),
    minNumber = cms.uint32(1)
)
cfgMuonPtCut = cms.PSet(
    pluginName = cms.string('muonPtCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatMuonsPt15Cumulative'),
    src_individual = cms.InputTag('selectedPatMuonsPt15Individual'),
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
cfgMuonAntiPionCut = cms.PSet(
    pluginName = cms.string('muonAntiPionCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatMuonsPionVetoCumulative'),
    src_individual = cms.InputTag('selectedPatMuonsPionVetoIndividual'),
    minNumber = cms.uint32(1)
)
cfgMuonTrkIPcut = cms.PSet(
    pluginName = cms.string('muonTrkIPcut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatMuonsTrkIPcumulative'),
    src_individual = cms.InputTag('selectedPatMuonsTrkIPindividual'),
    minNumber = cms.uint32(1)
)

# di-tau candidate selection
cfgDiTauCandidateForElecMuAntiOverlapVeto = cms.PSet(
    pluginName = cms.string('diTauCandidateForElecMuAntiOverlapVeto'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedElecMuPairsAntiOverlapVetoCumulative'),
    src_individual = cms.InputTag('selectedElecMuPairsAntiOverlapVetoIndividual'),
    minNumber = cms.uint32(1)
)
cfgDiTauCandidateForElecMuZeroChargeCut = cms.PSet(
    pluginName = cms.string('diTauCandidateForElecMuZeroChargeCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedElecMuPairsZeroChargeCumulative'),
    src_individual = cms.InputTag('selectedElecMuPairsZeroChargeIndividual'),
    minNumber = cms.uint32(1)
)
cfgDiTauCandidateForElecMuAcoplanarity12Cut = cms.PSet(
    pluginName = cms.string('diTauCandidateForElecMuAcoplanarity12Cut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedElecMuPairsAcoplanarity12Cumulative'),
    src_individual = cms.InputTag('selectedElecMuPairsAcoplanarity12Individual'),
    minNumber = cms.uint32(1)
)
cfgDiTauCandidateForElecMuMt1METcut = cms.PSet(
    pluginName = cms.string('diTauCandidateForElecMuMt1METcut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedElecMuPairsMt1METcumulative'),
    src_individual = cms.InputTag('selectedElecMuPairsMt1METindividual'),
    minNumber = cms.uint32(1)
)
cfgDiTauCandidateForElecMuMt2METcut = cms.PSet(
    pluginName = cms.string('diTauCandidateForElecMuMt2METcut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedElecMuPairsMt2METcumulative'),
    src_individual = cms.InputTag('selectedElecMuPairsMt2METindividual'),
    minNumber = cms.uint32(1)
)
cfgDiTauCandidateForElecMuPzetaDiffCut = cms.PSet(
    pluginName = cms.string('diTauCandidateForElecMuPzetaDiffCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedElecMuPairsPzetaDiffCumulative'),
    src_individual = cms.InputTag('selectedElecMuPairsPzetaDiffIndividual'),
    minNumber = cms.uint32(1)
)

zToElecMuEventSelConfigurator = eventSelFlagProdConfigurator(
    [ cfgTrigger,
      cfgPrimaryEventVertex,
      cfgPrimaryEventVertexQuality,
      cfgPrimaryEventVertexPosition,
      cfgGlobalMuonCut,
      cfgMuonEtaCut,
      cfgMuonPtCut,
      cfgElectronAntiOverlapWithMuonsVeto,
      cfgTightElectronIdCut,
      cfgElectronAntiCrackCut,
      cfgElectronEtaCut,
      cfgElectronPtCut,
      cfgMuonTrkIsoCut,
      cfgMuonEcalIsoCut,
      cfgMuonAntiPionCut,
      cfgMuonTrkIPcut,
      cfgElectronTrkIsoCut,
      cfgElectronEcalIsoCut,
      cfgElectronTrkCut,
      cfgElectronTrkIPcut,
      cfgDiTauCandidateForElecMuAntiOverlapVeto,      
      cfgDiTauCandidateForElecMuZeroChargeCut,
      cfgDiTauCandidateForElecMuAcoplanarity12Cut,
      cfgDiTauCandidateForElecMuMt1METcut,
      cfgDiTauCandidateForElecMuMt2METcut,
      cfgDiTauCandidateForElecMuPzetaDiffCut ],
    boolEventSelFlagProducer = "BoolEventSelFlagProducer",
    pyModuleName = __name__
)

selectZtoElecMuEvents = zToElecMuEventSelConfigurator.configure()
