import FWCore.ParameterSet.Config as cms

from TauAnalysis.RecoTools.tools.eventSelFlagProdConfigurator import *

# vertex selection
cfgPrimaryEventVertexBoosted = cms.PSet(
    pluginName = cms.string('primaryEventVertexBoosted'),
    pluginType = cms.string('VertexMinEventSelector'),
    src = cms.InputTag('selectedPrimaryVertexHighestPtTrackSum'),
    minNumber = cms.uint32(1)
    )
cfgPrimaryEventVertexQualityBoosted = cms.PSet(
    pluginName = cms.string('primaryEventVertexQualityBoosted'),
    pluginType = cms.string('VertexMinEventSelector'),
    src = cms.InputTag('selectedPrimaryVertexQuality'),
    minNumber = cms.uint32(1)
    )
cfgPrimaryEventVertexPositionBoosted = cms.PSet(
    pluginName = cms.string('primaryEventVertexPositionBoosted'),
    pluginType = cms.string('VertexMinEventSelector'),
    src = cms.InputTag('selectedPrimaryVertexPosition'),
    minNumber = cms.uint32(1)
    )

# tau selection
cfgTauEtaCutBoosted = cms.PSet(
    pluginName = cms.string('tauEtaCutBoosted'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1TausForWTauNuEta21Cumulative'),
    src_individual = cms.InputTag('selectedLayer1TausForWTauNuEta21Individual'),
    minNumber = cms.uint32(1)
    )
cfgTauPtCutBoosted = cms.PSet(
    pluginName = cms.string('tauPtCutBoosted'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1TausForWTauNuPt20Cumulative'),
    src_individual = cms.InputTag('selectedLayer1TausForWTauNuPt20Individual'),
    minNumber = cms.uint32(1)
    )

cfgMetPtBoosted = cms.PSet(
    pluginName = cms.string('metPtCutBoosted'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('selectedLayer1METsPt25'),
    minNumber = cms.uint32(1)
    )

cfgPFMetPtBoosted = cms.PSet(
    pluginName = cms.string('PFmetPtCutBoosted'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('selectedLayer1PFMETsPt40'),
    minNumber = cms.uint32(1)
    )

cfgTauLeadTrkBoosted = cms.PSet(
    pluginName = cms.string('tauLeadTrkCutBoosted'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1TausForWTauNuLeadTrkCumulative'),
    src_individual = cms.InputTag('selectedLayer1TausForWTauNuLeadTrkIndividual'),
    minNumber = cms.uint32(1)
    )

cfgTauLeadTrkPtBoosted = cms.PSet(
    pluginName = cms.string('tauLeadTrkPtCutBoosted'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1TausForWTauNuLeadTrkPtCumulative'),
    src_individual = cms.InputTag('selectedLayer1TausForWTauNuLeadTrkPtIndividual'),
    minNumber = cms.uint32(1)
    )

cfgTauEcalIsoBoosted = cms.PSet(
    pluginName = cms.string('tauEcalIsoBoosted'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1TausForWTauNuEcalIsoCumulative'),
    src_individual = cms.InputTag('selectedLayer1TausForWTauNuEcalIsoIndividual'),
    minNumber = cms.uint32(1)
    )

cfgTauTrkIsoBoosted = cms.PSet(
    pluginName = cms.string('tauTrkIsoBoosted'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1TausForWTauNuTrkIsoCumulative'),
    src_individual = cms.InputTag('selectedLayer1TausForWTauNuTrkIsoIndividual'),
    minNumber = cms.uint32(1)
    )

cfgTauMuonVetoBoosted = cms.PSet(
    pluginName = cms.string('tauMuonVetoBoosted'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1TausForWTauNuMuonVetoCumulative'),
    src_individual = cms.InputTag('selectedLayer1TausForWTauNuMuonVetoIndividual'),
    minNumber = cms.uint32(1)
    )

cfgTauElectronVetoBoosted = cms.PSet(
    pluginName = cms.string('tauElectronVetoBoosted'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1TausForWTauNuElectronVetoCumulative'),
    src_individual = cms.InputTag('selectedLayer1TausForWTauNuElectronVetoIndividual'),
    minNumber = cms.uint32(1)
    )

cfgTauEcalCrackVetoBoosted = cms.PSet(
    pluginName = cms.string('tauEcalCrackVetoBoosted'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1TausForWTauNuEcalCrackVetoCumulative'),
    src_individual = cms.InputTag('selectedLayer1TausForWTauNuEcalCrackVetoIndividual'),
    minNumber = cms.uint32(1)
    )

cfgTauProngCutBoosted = cms.PSet(
    pluginName = cms.string('tauProngCutBoosted'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1TausForWTauNuProngCumulative'),
    src_individual = cms.InputTag('selectedLayer1TausForWTauNuProngIndividual'),
    minNumber = cms.uint32(1)
    )

cfgTauChargeCutBoosted = cms.PSet(
    pluginName = cms.string('tauChargeCutBoosted'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedLayer1TausForWTauNuChargeCumulative'),
    src_individual = cms.InputTag('selectedLayer1TausForWTauNuChargeIndividual'),
    minNumber = cms.uint32(1)
    )

cfgRecoilJetBoosted = cms.PSet(
    pluginName = cms.string('recoilJetBoosted'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('selectedLayer1JetsEt15ForWTauNuCumulative'),
    minNumber = cms.uint32(1)
    )

# veto events containing additional central jets with Et > 10 GeV
cfgCentralJetVetoBoosted = cms.PSet(
    pluginName = cms.string('centralJetVetoBoosted'),
    pluginType = cms.string('PATCandViewMaxEventSelector'),
    src = cms.InputTag('selectedLayer1JetsEt15ForWTauNuCumulative'),
    maxNumber = cms.uint32(1)
    )
cfgPhiMetTauBoosted = cms.PSet(
    pluginName = cms.string('phiMetTauBoosted'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedTauNuPairsDPhiMetTauCumulative'),
    src_individual = cms.InputTag('selectedTauNuPairsDPhiMetTauIndividual'),
    minNumber = cms.uint32(1)
)

cfgMetTopologyCutBoosted = cms.PSet(
    pluginName = cms.string('metTopologyCutBoosted'),
    pluginType = cms.string('MEtTopologyMinEventSelector'),
    src = cms.InputTag('selectedMEtTopology04'),
    minNumber = cms.uint32(1)
)

cfgMetSignificanceCutBoosted = cms.PSet(
    pluginName = cms.string('metSignificanceCutBoosted'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('selectedMetSignificance10'),
    minNumber = cms.uint32(1)
    )

wToTauNuBoostedEventSelConfigurator = eventSelFlagProdConfigurator(
    [cfgPrimaryEventVertexBoosted,
     cfgPrimaryEventVertexQualityBoosted,
     cfgPrimaryEventVertexPositionBoosted,
     cfgTauEtaCutBoosted,
     cfgTauPtCutBoosted,
     cfgPFMetPtBoosted,
     cfgMetPtBoosted,
     cfgTauLeadTrkBoosted,
     cfgTauLeadTrkPtBoosted,
     cfgTauEcalIsoBoosted,
     cfgTauTrkIsoBoosted,
     cfgTauProngCutBoosted,
     cfgTauChargeCutBoosted,
     cfgTauMuonVetoBoosted,
     cfgTauElectronVetoBoosted,
     cfgTauEcalCrackVetoBoosted,
     cfgRecoilJetBoosted,
     cfgCentralJetVetoBoosted,
     cfgPhiMetTauBoosted,
     cfgMetTopologyCutBoosted,
     cfgMetSignificanceCutBoosted
     ],
    boolEventSelFlagProducer = "BoolEventSelFlagProducer",
    pyModuleName = __name__
    )

selectWtoTauNuEventsBoosted = wToTauNuBoostedEventSelConfigurator.configure()
