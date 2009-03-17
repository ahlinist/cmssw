import FWCore.ParameterSet.Config as cms

siStripClusters = cms.EDFilter("SiStripClusterizer",
                               DigiProducersList = cms.VInputTag(
    cms.InputTag('simSiStripDigis','\0')),
                               Algorithm = cms.string('ThreeThresholdAlgorithm'),
                               ChannelThreshold = cms.double(2.0),
                               SeedThreshold = cms.double(3.0),
                               ClusterThreshold = cms.double(5.0),
                               MaxSequentialHoles = cms.uint32(0),
                               MaxSequentialBad = cms.uint32(1),
                               MaxAdjacentBad = cms.uint32(0),
                               QualityLabel = cms.string("")
                               )
