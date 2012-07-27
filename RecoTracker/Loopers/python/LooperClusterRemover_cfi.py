import FWCore.ParameterSet.Config as cms

loopersMask = cms.EDProducer("LooperClusterRemover",
                             pixelClusters = cms.InputTag("siPixelClusters"),
                             stripClusters = cms.InputTag("siStripClusters"),
                             #algo = cms.string("ReadFileIn"),
                             algo = cms.string("LooperMethod"),
                             pixelRecHits = cms.InputTag("siPixelRecHits"),
                             pxlClusterCharge = cms.double(-1),
                             stripRecHits = cms.InputTag("siStripMatchedRecHits","matchedRecHit"),
                             useUnmatched = cms.bool(True),
                             rphiRecHits = cms.InputTag("siStripMatchedRecHits","rphiRecHitUnmatched"),
                             stereoRecHits = cms.InputTag("siStripMatchedRecHits","stereoRecHitUnmatched"),
                             ##   for reading a masking file in
                             maskFile = cms.string("evt1701test1.txt"),
                             withDetId = cms.bool(True),
                             fraction = cms.double(0.5),
                             epsilon = cms.double(0.01), #in cm
                             ##   for method with skipping every N
                             everyNPixel = cms.uint32(5),
                             ##   for the method with looper analysis in 2D histogramming
                             makeTrackCandidates = cms.bool(False),
                             makeTrack = cms.bool(True),
                             maskWithNoTC = cms.bool(False),
                             SeedCreatorPSet = cms.PSet(
                               ComponentName = cms.string('SeedFromConsecutiveHitsCreator'),
                               SeedMomentumForBOFF = cms.double(5.0),
                               propagator = cms.string('PropagatorWithMaterialWithLoops')
                               ),
                             collector = cms.PSet(
                                maximumTime = cms.double(-1), #time out value
                                xAxis = cms.vdouble(34, 1/65., 1/1.5),
                                invertX = cms.bool(True), 
                                nPhi = cms.uint32(180),
                                baseLineCut = cms.int32(60), # negative to roll to average occupancy
                                peakAbove= cms.uint32(4),
                                RBound=cms.double(30), #maximum radius of looper's helix
                                linkPoints=cms.bool(True),
                                annularCut = cms.double(2),
                                symetryTopologySelection = cms.uint32(2),
                                maxZForTruncation = cms.double(20),
                                phiSpread = cms.double(0.2),
                                offEdge = cms.int32(0),
                                deltaSlopeCut = cms.double(0.1),
                                phiSlopeEpsilon = cms.double(0.05)
                                )
                             )

from TrackingTools.MaterialEffects.Propagators_cff import MaterialPropagator
MaterialPropagatorWithLoops = MaterialPropagator.clone(
    ComponentName = "PropagatorWithMaterialWithLoops",
    MaxDPhi = 100.0
    )

from RecoTracker.TrackProducer.TrackProducer_cfi import TrackProducer
loopersTracks = TrackProducer.clone(
    src = 'loopersMask',
    AlgorithmName = cms.string('iter10'),
    Propagator = "PropagatorWithMaterialWithLoops"
    )

    
