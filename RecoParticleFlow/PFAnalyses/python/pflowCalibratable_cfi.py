import FWCore.ParameterSet.Config as cms

masterConeDeltaR = 0.15

EventDelegate = cms.PSet(

    EventDelegateType=cms.string('DipionDelegate'),
    # Set up to 7 to get outrageous amounts of output
    debug=cms.int32(2),
    isMC = cms.bool(True),
    useSimAsTrack=cms.bool(True),
    deltaRClustersToTrack=cms.double(masterConeDeltaR * 2),
    deltaRRechitsToTrack=cms.double(masterConeDeltaR),
    deltaRCandToTrack=cms.double(masterConeDeltaR * 2),
    clustersFromCandidates=cms.bool(True),
    rechitsFromCandidates=cms.bool(False),
    neutralMode=cms.bool(True),
    noSimDaughters=cms.bool(True),
    particlePDG=cms.int32(211),    
    
    PFSimParticles=cms.InputTag("particleFlowSimParticle"),
    PFRecHitsEcal=cms.InputTag("particleFlowRecHitECAL"),
    PFRecHitsHcal=cms.InputTag("particleFlowRecHitHCAL"),
    PFClustersHcal=cms.InputTag("particleFlowClusterHCAL"),
    PFClustersEcal=cms.InputTag("particleFlowClusterECAL"),
    PFCandidates=cms.InputTag("particleFlow"),
    PFRecTracks=cms.InputTag("trackerDrivenElectronSeeds"),
    SimCaloRecHitsEcalEB=cms.InputTag("famosSimHits", "EcalHitsEB"),
    SimCaloRecHitsEcalEE=cms.InputTag("famosSimHits", "EcalHitsEE"),
    SimCaloRecHitsHcal=cms.InputTag("famosSimHits", "HcalHits"),
    RawRecHitsEcalEB=cms.InputTag("ecalRecHit","EcalRecHitsEB"),
    RawRecHitsEcalEE=cms.InputTag("ecalRecHit","EcalRecHitsEE"),
    RawRecHitsHcal=cms.InputTag("hbhereco"),
    
    deltaREcalCaloWindow=cms.double(0.01),
    deltaRHcalCaloWindow=cms.double(0.05),
    nRingsEcalCaloWindow=cms.uint32(5),
    nRingsHcalCaloWindow=cms.uint32(3),
    nPanesEcalCaloWindow=cms.uint32(1),
    nPanesHcalCaloWindow=cms.uint32(2),
)
    
TestbeamDelegate = cms.PSet(
    EventDelegate,
    # Reject spurious events where the HCAL has some catastrophic noise
    # = 0, don't do this, > 0 multiple of beam energy found in HCAL before event is rejected
    stripAnomalousEvents=cms.uint32(6),
    
    # 0 => ignore, > 0 implies only save raw rechits within deltaR of beam spot.
    deltaRRecHitsToCenterECAL=cms.double(masterConeDeltaR),
    deltaRRecHitsToCenterHCAL=cms.double(masterConeDeltaR),
    deltaRClustersToCenterECAL=cms.double(masterConeDeltaR),
    deltaRClustersToCenterHCAL=cms.double(masterConeDeltaR),
    
    # If not clustersFromCandidates, get clusters within cone DeltaR = around beamspot
    # PF photons/neutrals made Delta R away from track impact on ECAL will be considered
    # as noise:
    deltaRNeutralsToTrack=cms.double(masterConeDeltaR * 2),
    deltaRPhotonsToTrack=cms.double(masterConeDeltaR),
    
    # This should be true if you only want particles saved to the tree that 
    # pass the minimum set of vetos (i.e. at least muon + scintillator + beam halo)
    # Recommended: True
    applyCleaningCuts=cms.bool(True),
    #If false, only save pion (Class-31) candidates
    # Recommended: True
    saveJustPions=cms.bool(True),
    # Global cut parameters   
    runinfo_cuts=cms.string("/afs/cern.ch/user/b/ballin/scratch0/cmssw/src/RecoParticleFlow/PFAnalyses/macros/testbeam_cuts_v1_3X.root"),
    maxEventsFromEachRun=cms.uint32(0),
    
    #Normal tags
    ParticleFiltration=cms.InputTag("particleFiltration", "particleFiltration"),
    RawRecHitsEcalEB=cms.InputTag("ecalRecHitMaker", "EcalRecHitsEB"),
    BeamCounters=cms.InputTag("tbunpacker"),
    RunData=cms.InputTag("tbunpacker"),
    Timing=cms.InputTag("tbunpacker"),
    EventPosition=cms.InputTag("tbunpacker"),
    TriggerData=cms.InputTag("tbunpacker")
    
)
#Testbeam related options
TestbeamDelegate.EventDelegateType=cms.string('TestbeamDelegate')
    

