import FWCore.ParameterSet.Config as cms

loopersMask = cms.EDProducer("LooperClusterRemover",
                             pixelClusters = cms.InputTag("siPixelClusters"),
                             stripClusters = cms.InputTag("siStripClusters"),
                             #algo = cms.string("ReadFileIn"),
                             algo = cms.string("LooperMethod"),
                             pixelRecHits = cms.InputTag("siPixelRecHits"),
                             stripRecHits = cms.InputTag("siStripMatchedRecHits","matchedRecHit"),
                             ##   for reading a masking file in
                             maskFile = cms.string("evt1701test1.txt"),
                             withDetId = cms.bool(True),
                             fraction = cms.double(0.5),
                             epsilon = cms.double(0.01), #in cm
                             ##   for method with skipping every N
                             everyNPixel = cms.uint32(5),
                             ##   for the method with looper analysis in 2D histogramming
                             makeTrackCandidates = cms.bool(False),
                             collector = cms.PSet(
                                xAxis = cms.vdouble(34, 1/65., 1/1.5),
                                invertX = cms.bool(True), 
                                nPhi = cms.uint32(60),
                                peakAbove= cms.uint32(4),
                                RBound=cms.double(30), #maximum radius of looper's helix
                                linkPoints=cms.bool(True),
                                annularCut = cms.double(2),
                                symetryTopologySelection = cms.uint32(2),
                                maxZForTruncation = cms.double(20),
                                offEdge = cms.int32(0),
                                deltaSlopeCut = cms.double(0.1),
                                phiSlopeEpsilon = cms.double(0.05)
                                )
                             )

