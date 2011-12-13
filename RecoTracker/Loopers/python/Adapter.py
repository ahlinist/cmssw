import FWCore.ParameterSet.Config as cms

##function from RECO file, does not need anything else
#usually cmsDriver.py looper -s USER:RecoTracker/Loopers/Adapter ...
from RecoTracker.Loopers.LooperClusterRemover_cfi import *
from Configuration.StandardSequences.Reconstruction_cff import *
#from a RECO file
user =cms.Sequence( siPixelRecHits+
                    siStripMatchedRecHits+
                    offlineBeamSpot+
                    loopersMask)

def adapt(process):
    ##function from a RAW file
    #cmsDriver.py looper -s RAW2DIGI,RECO --customise RecoTracker/Loopers/Adapter.<any>
    ##function from a RECO file
    #usually cmsDriver.py looper -s RECO --customise RecoTracker/Loopers/Adapter.<any>
    process.load('RecoTracker/Loopers/LooperClusterRemover_cfi')
    process.reconstruction_step.replace(process.siStripMatchedRecHits,
                                        process.siStripMatchedRecHits+
                                        process.offlineBeamSpot+
                                        process.loopersMask)
    process.pixellayertriplets.BPix.skipClusters = cms.InputTag('loopersMask')
    process.pixellayertriplets.FPix.skipClusters = cms.InputTag('loopersMask')
    process.lowPtTripletStepClusters.oldClusterRemovalInfo = cms.InputTag('loopersMask')
    process.initialStepTrajectoryBuilder.clustersToSkip = cms.InputTag('loopersMask')

    return (process)

def looperTracks(process):
    ##function from a RAW file
    #cmsDriver.py looper -s RAW2DIGI,RECO --customise RecoTracker/Loopers/Adapter.<any>
    ##function from a RECO file
    #usually cmsDriver.py looper -s RECO --customise RecoTracker/Loopers/Adapter.<any>
    if not hasattr(process,'loopersMask'):
        adapt(process)
    import RecoTracker.TrackProducer.TrackProducer_cfi
    process.loopersTracks = RecoTracker.TrackProducer.TrackProducer_cfi.TrackProducer.clone(
        src = 'loopersMask',
        AlgorithmName = cms.string('iter10'),
        )
    process.reconstruction_step.replace(process.loopersMask,
                                        process.loopersMask+process.loopersTracks)
    return (process)
                                        
    
