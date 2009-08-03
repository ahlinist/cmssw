import FWCore.ParameterSet.Config as cms

from TauAnalysis.RecoTools.eventSelFlagProdConfigurator import *

#--------------------------------------------------------------------------------
# define event selection criteria for Z --> e + mu channel
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

# muon candidate selection
cfgGlobalMuonCut = cms.PSet(
    pluginName = cms.string('globalMuonCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1MuonsGlobalCumulative'),
    src_individual = cms.InputTag('selectedLayer1MuonsGlobalIndividual'),
    minNumber = cms.uint32(1)
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
cfgMuonAntiPionCut = cms.PSet(
    pluginName = cms.string('muonAntiPionCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1MuonsPionVetoCumulative'),
    src_individual = cms.InputTag('selectedLayer1MuonsPionVetoIndividual'),
    minNumber = cms.uint32(1)
)
cfgMuonTrkIPcut = cms.PSet(
    pluginName = cms.string('muonTrkIPcut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1MuonsTrkIPcumulative'),
    src_individual = cms.InputTag('selectedLayer1MuonsTrkIPindividual'),
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
      cfgTightElectronIdCut,
      cfgElectronAntiCrackCut,
      cfgElectronEtaCut,
      cfgElectronPtCut,
      cfgGlobalMuonCut,
      cfgMuonEtaCut,
      cfgMuonPtCut,
      cfgElectronTrkIsoCut,
      cfgElectronEcalIsoCut,
      cfgElectronTrkCut,
      cfgElectronTrkIPcut,
      cfgMuonTrkIsoCut,
      cfgMuonEcalIsoCut,
      cfgMuonAntiPionCut,
      cfgMuonTrkIPcut,
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
