import FWCore.ParameterSet.Config as cms

##function from RECO file, does not need anything else
#usually cmsDriver.py looper -s USER:RecoTracker/Loopers/Adapter ...
#from RecoTracker.Loopers.LooperClusterRemover_cfi import *
#from Configuration.StandardSequences.Reconstruction_cff import *
#from a RECO file
#user =cms.Sequence( siPixelRecHits+
#                    siStripMatchedRecHits+
#                    offlineBeamSpot+
#                    loopersMask)

def adapt(process):
    if not hasattr(process,"mix"):
        process.mix = cms.EDProducer("DummyModule")
    ##function from a RAW file
    #cmsDriver.py looper -s RAW2DIGI,RECO --customise RecoTracker/Loopers/Adapter.<any>
    ##function from a RECO file
    #usually cmsDriver.py looper -s RECO --customise RecoTracker/Loopers/Adapter.<any>
    process.load('RecoTracker/Loopers/LooperClusterRemover_cfi')

    process.loopersSeq= cms.Sequence( process.mix+process.loopersMask )
    process.iterTracking._seq = process.loopersSeq * process.iterTracking._seq
        
    process.pixellayertriplets.BPix.skipClusters = cms.InputTag('loopersMask')
    process.pixellayertriplets.FPix.skipClusters = cms.InputTag('loopersMask')
    process.lowPtTripletStepClusters.oldClusterRemovalInfo = cms.InputTag('loopersMask')
    process.initialStepTrajectoryBuilder.clustersToSkip = cms.InputTag('loopersMask')

    ##adapt a PV finding to the loopers track only
    process.loopersVertex = process.offlinePrimaryVertices.clone(TrackLabel='loopersMask')
    process.loopersSeq+=process.loopersVertex
        
    for o in process.outputModules_():
        om=process.outputModules_()[o]
        om.outputCommands.append('keep *_loopersMask_*_*')
        om.outputCommands.append('keep *_loopersVertex_*_*')
    return (process)


def looperTracks(process):
    ##function from a RAW file
    #cmsDriver.py looper -s RAW2DIGI,RECO --customise RecoTracker/Loopers/Adapter.<any>
    ##function from a RECO file
    #usually cmsDriver.py looper -s RECO --customise RecoTracker/Loopers/Adapter.<any>
    if not hasattr(process,'loopersMask'):
        adapt(process)

    ##attempt using primarily a track refitter. The TrackCandidateOption initially used does not function
    # it should be noted that the track refitting does not function so well either (does it really matter ?)
    from RecoTracker.TrackProducer.TrackRefitter_cfi import TrackRefitter
    process.loopersTracks = TrackRefitter.clone(
        src = cms.InputTag('loopersMask')
        )
    ## augment the looping capability of RK prop
    process.RungeKuttaTrackerPropagator.MaxDPhi = 100.0
    ## replace the RK propagator with something elst just to see how further it goes
    ### SHP does not help enough
    #process.RungeKuttaTrackerPropagator = process.SteppingHelixPropagatorAlong.clone( ComponentName = 'RungeKuttaTrackerPropagator')
    ### analytical propagator ...
    ##process.RungeKuttaTrackerPropagator = process.AnalyticalPropagator.clone( ComponentName = 'RungeKuttaTrackerPropagator',MaxDPhi = 1000.0)

    process.loopersSeq+=process.loopersTracks

    ## now try to put those tracks (or the initial ones inside the generalTrack collection)
    try:
        process.generalTracks.TrackProducers.append( cms.InputTag('loopersMask') )
        process.generalTracks.hasSelector.append( 0 )
        process.generalTracks.setsToMerge[0].tLists.append(7)
    except:
        #fall back for 6.2
        process.earlyGeneralTracks.TrackProducers.append( cms.InputTag('loopersMask') )
        process.earlyGeneralTracks.selectedTrackQuals.append( cms.InputTag('dummy') )
        process.earlyGeneralTracks.hasSelector.append( 0 )
        process.earlyGeneralTracks.setsToMerge[0].tLists.append(7)

    for o in process.outputModules_():
        om=process.outputModules_()[o]
        om.outputCommands.append('keep *_loopersTracks_*_*')
    return (process)
                                        
    
