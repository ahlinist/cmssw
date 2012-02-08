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
    if not hasattr(process,"mix"):
        process.mix = cms.EDProducer("DummyModule")
    ##function from a RAW file
    #cmsDriver.py looper -s RAW2DIGI,RECO --customise RecoTracker/Loopers/Adapter.<any>
    ##function from a RECO file
    #usually cmsDriver.py looper -s RECO --customise RecoTracker/Loopers/Adapter.<any>
    process.load('RecoTracker/Loopers/LooperClusterRemover_cfi')
    process.reconstruction_step.replace(process.siStripMatchedRecHits,
                                        process.siStripMatchedRecHits+
                                        process.offlineBeamSpot+
                                        process.mix+
                                        process.loopersMask)
    process.pixellayertriplets.BPix.skipClusters = cms.InputTag('loopersMask')
    process.pixellayertriplets.FPix.skipClusters = cms.InputTag('loopersMask')
    process.lowPtTripletStepClusters.oldClusterRemovalInfo = cms.InputTag('loopersMask')
    process.initialStepTrajectoryBuilder.clustersToSkip = cms.InputTag('loopersMask')

    for o in process.outputModules_():
        om=process.outputModules_()[o]
        om.outputCommands.append('keep *_loopersMask_*_*')
    return (process)

def looperTracks(process):
    ##function from a RAW file
    #cmsDriver.py looper -s RAW2DIGI,RECO --customise RecoTracker/Loopers/Adapter.<any>
    ##function from a RECO file
    #usually cmsDriver.py looper -s RECO --customise RecoTracker/Loopers/Adapter.<any>
    if not hasattr(process,'loopersMask'):
        adapt(process)
    process.reconstruction_step.replace(process.loopersMask,
                                        process.loopersMask+process.loopersTracks)

    for o in process.outputModules_():
        om=process.outputModules_()[o]
        om.outputCommands.append('keep *_loopersMask_*_*')
    return (process)
                                        
    
