import FWCore.ParameterSet.Config as cms

particleFiltration = cms.EDFilter("ParticleFilter",
                            
    ParticleFiltrationDelegateType=cms.string('TestbeamFiltrationDelegate'),
    # Set up to 7 to get outrageous amounts of output
    debug=cms.int32(5),

    # Reject spurious events where the HCAL has some terrible noise
    # = 0, don't do this, > 0 multiple of beam energy found in HCAL before event is rejected
    # stripAnomalousEvents=cms.uint32(6),


    # This should be true if you only want particles saved to the tree that 
    # pass the minimum set of vetos (i.e. at least muon + scintillator + beam halo)
    # Recommended: True
    applyCleaningCuts=cms.bool(True),
    #Compute vetos anyway, regardless of applyCleaningCuts
    # Recommended: True
    computeVetos=cms.bool(True),
    #If false, only save pion (Class-31) candidates
    # Recommended: True
    saveAllCleanParticles=cms.bool(False),
    #If true, alter pdg ids based on particle id: i.e. electrons and protons/kaons
    # Recommended: True
    identifyCleanParticles=cms.bool(True),
    
    # Global cut parameters   
    runinfo_cuts=cms.string("/castor/cern.ch/user/b/ballin/tbv6/testbeam_cuts.root"),
    
    #Normal tags
    BeamCounters=cms.InputTag("tbunpacker"),
    RunData=cms.InputTag("tbunpacker"),
    Timing=cms.InputTag("tbunpacker"),
    EventPosition=cms.InputTag("tbunpacker"),
    TriggerData=cms.InputTag("tbunpacker")
)
