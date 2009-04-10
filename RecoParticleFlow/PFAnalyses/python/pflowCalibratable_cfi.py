import FWCore.ParameterSet.Config as cms

calibratable = cms.EDProducer("CalibratableProducer",
                            
    EventDelegateType=cms.string('TestbeamDelegate'),
    # Set up to 7 to get outrageous amounts of output
    debug=cms.int32(2),
    # Do a bad version of zero suppression!
    # Recommended = False
    applyThresholdsToRawRecHits=cms.bool(False),
    # Get PFClusters from PFCandidates, rather than whatever's in the cluster collections
    # Recommended = True
    clustersFromCandidates=cms.bool(True),
    
    rechitsFromCandidates=cms.bool(True),
    # Reject spurious events where the HCAL has some terrible noise
    # = 0, don't do this, > 0 multiple of beam energy found in HCAL before event is rejected
    stripAnomalousEvents=cms.uint32(6),
    # Leave alone
    maxEventsFromEachRun=cms.uint32(0),
    # 0 => ignore, > 0 implies only save raw rechits within deltaR of beam spot.
    deltaRRecHitsToCenterECAL=cms.double(0.1),
    deltaRRecHitsToCenterHCAL=cms.double(0.2),
    # If not clustersFromCandidates, get clusters within cone DeltaR = around beamspot
    deltaRClustersToTrack=cms.double(0.4),
    #Ignore this
    isMC=cms.bool(False),
    # This should be true if you only want particles saved to the tree that 
    # pass the minimum set of vetos (i.e. at least muon + scintillator + beam halo)
    # Recommended: True
    applyCleaningCuts=cms.bool(True),
    #Compute vetos anyway, regardless of applyCleaningCuts
    # Recommended: True
    computeVetos=cms.bool(True),
    #If false, only save pion (Class-31) candidates
    # Recommended: True
    saveAllCleanParticles=cms.bool(True),
    #If true, alter pdg ids based on particle id: i.e. electrons and protons/kaons
    # Recommended: True
    identifyCleanParticles=cms.bool(True),
    
    # Global cut parameters   
    runinfo_cuts=cms.string("/castor/cern.ch/user/b/ballin/tbv3/testbeam_cuts.root"),
    
    #Normal tags
    PFRecHitsEcal=cms.InputTag("particleFlowRecHitECAL"),
    PFRecHitsHcal=cms.InputTag("particleFlowRecHitHCAL"),
    PFClustersHcal=cms.InputTag("particleFlowClusterHCAL"),
    PFClustersEcal=cms.InputTag("particleFlowClusterECAL"),
    PFCandidates=cms.InputTag("particleFlow"),
    RawRecHitsEcal=cms.InputTag("ecalRecHitMaker", "EcalRecHitsEB"),
    RawRecHitsHcal=cms.InputTag("hbhereco"),
    BeamCounters=cms.InputTag("tbunpacker"),
    RunData=cms.InputTag("tbunpacker"),
    Timing=cms.InputTag("tbunpacker"),
    EventPosition=cms.InputTag("tbunpacker"),
    TriggerData=cms.InputTag("tbunpacker")
)
