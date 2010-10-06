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

jetSkimFilterSequence = cms.Sequence(
  superClusterMerger *
  superClusterCands *
  goodSuperClusters
  )
