import FWCore.ParameterSet.Config as cms

#Question: how to change this globally?
masterConeDeltaR = 0.15

from RecoParticleFlow.PFAnalyses.RunDict import *

# The default EventDelegate is a DipionDelegate
EventDelegate = cms.PSet(
                         
    # Human-readable string representing this class of event delegate
    EventDelegateType=cms.string('DipionDelegate'),
    # Set up to 7 to get outrageous amounts of output
    debug=cms.int32(3),
    # Look for MonteCarlo particles?
    isMC = cms.bool(True),
    # Don't use reconstructed track as main cone for searches, but the sim particle instead
    useSimAsTrack=cms.bool(True),
    # Collect clusters from how far away?
    deltaRClustersToTrack=cms.double(masterConeDeltaR * 2),
    # And Rechits from how far?
    deltaRRechitsToTrack=cms.double(masterConeDeltaR),
    # And PFCandidates from how far?
    deltaRCandToTrack=cms.double(masterConeDeltaR * 2),
    # Don't take clusters from cone, but from the PFCandidates themselves
    clustersFromCandidates=cms.bool(True),
    # Don't take rechits from cone, but from the clusters, in turn belonging to the PFCandidates
    rechitsFromCandidates=cms.bool(True),
    # Don't expect a charged track
    neutralMode=cms.bool(False),
    # Require no tracker interactions (needed for full sim)
    noSimDaughters=cms.bool(True),
    # Which sim particle type are we looking for
    particlePDG=cms.int32(211),    
    
    # Input tag definitions
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
    nRingsHcalCaloWindow=cms.uint32(6),
    nPanesEcalCaloWindow=cms.uint32(1),
    nPanesHcalCaloWindow=cms.uint32(1),
)

# Change of tags for full sim
EventDelegateFullSim = EventDelegate.clone()
EventDelegateFullSim.SimCaloRecHitsEcalEB=cms.InputTag("g4SimHits", "EcalHitsEB")
EventDelegateFullSim.SimCaloRecHitsEcalEE=cms.InputTag("g4SimHits", "EcalHitsEE")
EventDelegateFullSim.SimCaloRecHitsHcal=cms.InputTag("g4SimHits", "HcalHits")
EventDelegateFullSim.noSimDaughters=cms.bool(True)

# Specialised EventDelegate designed to work on testbeam data
TestbeamDelegate = cms.PSet(
    # Inherit options from the DipionDelegate
    EventDelegate,
    
    # Reject spurious events where the HCAL has some catastrophic noise
    # = 0, don't do this, > 0 multiple of beam energy found in HCAL before event is rejected
    # Recommended value: 6
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
    deltaRPhotonsToTrack=cms.double(masterConeDeltaR * 2),
    
    # This should be true if you only want particles saved to the tree that 
    # pass the minimum set of vetos (i.e. at least muon + scintillator + beam halo)
    # Recommended: True
    applyCleaningCuts=cms.bool(True),
    # If true, only save pion (Class-31) candidates
    # Recommended: True
    saveJustPions=cms.bool(True),
    # Global cut parameters
    # testbeam_cuts defined  in Run_Dict.py
    runinfo_cuts=cms.string(testbeam_cuts),
    
    # (Deprecated)
    maxEventsFromEachRun=cms.uint32(0),
    
    #Normal input tags
    ParticleFiltration=cms.InputTag("particleFiltration", "particleFiltration"),
    BeamCounters=cms.InputTag("tbunpacker"),
    RunData=cms.InputTag("tbunpacker"),
    Timing=cms.InputTag("tbunpacker"),
    EventPosition=cms.InputTag("tbunpacker"),
    TriggerData=cms.InputTag("tbunpacker")   
)

# Override the DipionDelegate default options
TestbeamDelegate.RawRecHitsEcalEB=cms.InputTag("ecalRecHitMaker", "EcalRecHitsEB")
TestbeamDelegate.EventDelegateType=cms.string('TestbeamDelegate')

# Ignore this for now
IsolatedParticleExtractor=cms.PSet(
    #PFCandidate types to extract
    ParticleType = cms.uint32(1),
    #If more than one particle is found matching isolation cuts, only extract the 'best' one
    MostIsolatedOnly = cms.bool(False),
    
    #If not, just pass all PFCandidates matching particleType_ cut
    ApplyIsolationCuts = cms.bool(False),
    
    #Isolation required in the ECAL
    DeltaREcalIsolation = cms.double(0.3),
    
    #Isolation required in the HCAL
    DeltaRHcalIsolation = cms.double(0.3),
    
)
    

