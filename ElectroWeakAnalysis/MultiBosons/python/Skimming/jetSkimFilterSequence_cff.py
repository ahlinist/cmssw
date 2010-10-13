import FWCore.ParameterSet.Config as cms

superClusterMerger =  cms.EDFilter("EgammaSuperClusterMerger",
    src = cms.VInputTag(
        cms.InputTag('correctedHybridSuperClusters'),
        cms.InputTag('correctedMulti5x5SuperClustersWithPreshower')
        )
    )

superClusterCands = cms.EDProducer("ConcreteEcalCandidateProducer",
    src = cms.InputTag("superClusterMerger"),
    particleType = cms.string('gamma')
    )

## TODO: separated from the HLT-matched good quality jet by dR > 1.0
goodSuperClusters = cms.EDFilter("CandViewRefSelector",
    filter = cms.bool(True),
    src = cms.InputTag("superClusterCands"),
    cut = cms.string('et > 10.')
)

goodJets = cms.EDFilter("CaloJetRefSelector",
    filter = cms.bool(True),
    src = cms.InputTag("ak5CaloJets"),
    cut = cms.string('(abs(eta) >= 2.6 | emEnergyFraction() > 0.01) & energyFractionHadronic() < 0.98 & n90() > 1')
)



jetSkimFilterSequence = cms.Sequence(
  superClusterMerger *
  superClusterCands *
  goodSuperClusters
  )
