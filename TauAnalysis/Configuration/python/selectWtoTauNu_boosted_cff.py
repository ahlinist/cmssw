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
    src_cumulative = cms.InputTag('selectedPatTausForWTauNuEta21Cumulative'),
    src_individual = cms.InputTag('selectedPatTausForWTauNuEta21Individual'),
    minNumber = cms.uint32(1)
    )
cfgTauPtCutBoosted = cms.PSet(
    pluginName = cms.string('tauPtCutBoosted'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatTausForWTauNuPt20Cumulative'),
    src_individual = cms.InputTag('selectedPatTausForWTauNuPt20Individual'),
    minNumber = cms.uint32(1)
    )

cfgMetPtBoosted = cms.PSet(
    pluginName = cms.string('metPtCutBoosted'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('selectedPatMETsPt25'),
    minNumber = cms.uint32(1)
    )

cfgPFMetPtBoosted = cms.PSet(
    pluginName = cms.string('PFmetPtCutBoosted'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('selectedPatPFMETsPt40'),
    minNumber = cms.uint32(1)
    )

cfgTauLeadTrkBoosted = cms.PSet(
    pluginName = cms.string('tauLeadTrkCutBoosted'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatTausForWTauNuLeadTrkCumulative'),
    src_individual = cms.InputTag('selectedPatTausForWTauNuLeadTrkIndividual'),
    minNumber = cms.uint32(1)
    )

cfgTauLeadTrkPtBoosted = cms.PSet(
    pluginName = cms.string('tauLeadTrkPtCutBoosted'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatTausForWTauNuLeadTrkPtCumulative'),
    src_individual = cms.InputTag('selectedPatTausForWTauNuLeadTrkPtIndividual'),
    minNumber = cms.uint32(1)
    )

cfgTauEcalIsoBoosted = cms.PSet(
    pluginName = cms.string('tauEcalIsoBoosted'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatTausForWTauNuEcalIsoCumulative'),
    src_individual = cms.InputTag('selectedPatTausForWTauNuEcalIsoIndividual'),
    minNumber = cms.uint32(1)
    )

cfgTauTrkIsoBoosted = cms.PSet(
    pluginName = cms.string('tauTrkIsoBoosted'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatTausForWTauNuTrkIsoCumulative'),
    src_individual = cms.InputTag('selectedPatTausForWTauNuTrkIsoIndividual'),
    minNumber = cms.uint32(1)
    )

cfgTauMuonVetoBoosted = cms.PSet(
    pluginName = cms.string('tauMuonVetoBoosted'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatTausForWTauNuMuonVetoCumulative'),
    src_individual = cms.InputTag('selectedPatTausForWTauNuMuonVetoIndividual'),
    minNumber = cms.uint32(1)
    )

cfgTauElectronVetoBoosted = cms.PSet(
    pluginName = cms.string('tauElectronVetoBoosted'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatTausForWTauNuElectronVetoCumulative'),
    src_individual = cms.InputTag('selectedPatTausForWTauNuElectronVetoIndividual'),
    minNumber = cms.uint32(1)
    )

cfgTauEcalCrackVetoBoosted = cms.PSet(
    pluginName = cms.string('tauEcalCrackVetoBoosted'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatTausForWTauNuEcalCrackVetoCumulative'),
    src_individual = cms.InputTag('selectedPatTausForWTauNuEcalCrackVetoIndividual'),
    minNumber = cms.uint32(1)
    )

cfgTauProngCutBoosted = cms.PSet(
    pluginName = cms.string('tauProngCutBoosted'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatTausForWTauNuProngCumulative'),
    src_individual = cms.InputTag('selectedPatTausForWTauNuProngIndividual'),
    minNumber = cms.uint32(1)
    )

cfgTauChargeCutBoosted = cms.PSet(
    pluginName = cms.string('tauChargeCutBoosted'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatTausForWTauNuChargeCumulative'),
    src_individual = cms.InputTag('selectedPatTausForWTauNuChargeIndividual'),
    minNumber = cms.uint32(1)
    )

cfgRecoilJetBoosted = cms.PSet(
    pluginName = cms.string('recoilJetBoosted'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('selectedPatJetsEt15ForWTauNuCumulative'),
    minNumber = cms.uint32(1)
    )

# veto events containing additional central jets with Et > 10 GeV
cfgCentralJetVetoBoosted = cms.PSet(
    pluginName = cms.string('centralJetVetoBoosted'),
    pluginType = cms.string('PATCandViewMaxEventSelector'),
    src = cms.InputTag('selectedPatJetsEt15ForWTauNuCumulative'),
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
