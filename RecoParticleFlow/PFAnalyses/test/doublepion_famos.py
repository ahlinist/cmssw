import FWCore.ParameterSet.Config as cms

process = cms.Process("PROD")
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100000)
)
process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
    moduleSeeds = cms.PSet(
        caloRecHits = cms.untracked.uint32(754321),
        VtxSmeared = cms.untracked.uint32(223458),
        muonCSCDigis = cms.untracked.uint32(525432),
        muonDTDigis = cms.untracked.uint32(67673876),
        famosSimHits = cms.untracked.uint32(235791312),
        MuonSimHits = cms.untracked.uint32(834032),
        famosPileUp = cms.untracked.uint32(918273),
        muonRPCDigis = cms.untracked.uint32(524964),
        siTrackerGaussianSmearingRecHits = cms.untracked.uint32(34680)
    ),
    sourceSeed = cms.untracked.uint32(1234)
)

#fastsim
process.load("FastSimulation.Configuration.RandomServiceInitialization_cff")
process.load("FastSimulation.Configuration.CommonInputsFake_cff")
process.load("FastSimulation.Configuration.FamosSequences_cff")

process.famosSimHits.SimulateCalorimetry = True
process.famosSimHits.SimulateTracking = True
process.famosPileUp.PileUpSimulator.averageNumber = 0.0

process.famosSimHits.VertexGenerator.BetaStar = 0.00001
process.famosSimHits.VertexGenerator.SigmaZ = 0.00001

# Parametrized magnetic field (new mapping, 4.0 and 3.8T)
process.load("Configuration.StandardSequences.MagneticField_40T_cff")
#process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.VolumeBasedMagneticFieldESProducer.useParametrizedTrackerField = True

process.famosSimHits.MaterialEffects.PairProduction = False
process.famosSimHits.MaterialEffects.Bremsstrahlung = False
process.famosSimHits.MaterialEffects.EnergyLoss = False
process.famosSimHits.MaterialEffects.MultipleScattering = False
process.famosSimHits.MaterialEffects.NuclearInteraction = False
process.famosSimHits.Calorimetry.UnfoldedMode = True

process.load("RecoParticleFlow.PFBlockProducer.particleFlowSimParticle_cff")

process.source = cms.Source("FlatRandomEGunSource",
    PGunParameters = cms.untracked.PSet(
        PartID = cms.untracked.vint32(211),
        MaxEta = cms.untracked.double(0.5),
        MaxPhi = cms.untracked.double(3.14159265359),
        MinEta = cms.untracked.double(0.0),
        MinE = cms.untracked.double(0),
        DoubleParticle = cms.untracked.bool(True),
        MinPhi = cms.untracked.double(-3.14159265359),
        MaxE = cms.untracked.double(50.0)
    ),
    Verbosity = cms.untracked.int32(1),
    AddAntiParticle = cms.untracked.bool(True)
)

process.MessageLogger = cms.Service("MessageLogger",
    debugModules = cms.untracked.vstring('*'),
    cout = cms.untracked.PSet(
        INFO = cms.untracked.PSet(
            limit = cms.untracked.int32(0)
        ),
        PFBlockProducer = cms.untracked.PSet(
            limit = cms.untracked.int32(10000000)
        ),
        PFClusterProducer = cms.untracked.PSet(
            limit = cms.untracked.int32(10000000)
        ),
        DEBUG = cms.untracked.PSet(
            limit = cms.untracked.int32(0)
        ),
        PFProducer = cms.untracked.PSet(
            limit = cms.untracked.int32(10000000)
        ),
        threshold = cms.untracked.string('INFO')
    ),
    categories = cms.untracked.vstring('PFClusterProducer', 
        'PFBlockProducer', 
        'PFProducer'),
    destinations = cms.untracked.vstring('cout')
)

process.dump = cms.EDAnalyzer("EventContentAnalyzer")

process.extraction = cms.EDAnalyzer("ExtractionAnalyzer",
    useSimAsTrack = cms.bool(False),
    PFClustersHcal = cms.InputTag("particleFlowClusterHCAL"),
    deltaRClustersToTrack = cms.double(0.4),
    EventDelegateType = cms.string('DipionDelegate'),
    deltaRCandToTrack = cms.double(0.4),
    PFCandidates = cms.InputTag("particleFlow"),
    deltaRRechitsToTrack = cms.double(0.4),
    deltaPhi = cms.double(0.4),
    debug = cms.int32(1),
    PFRecTracks = cms.InputTag("elecpreid"),
    PFRecHitsEcal = cms.InputTag("particleFlowRecHitECAL"),
    deltaEta = cms.double(0.4),
    PFSimParticles = cms.InputTag("particleFlowSimParticle"),
    PFRecHitsHcal = cms.InputTag("particleFlowRecHitHCAL"),
    isMC = cms.bool(True),
    PFClustersEcal = cms.InputTag("particleFlowClusterECAL"),
    clustersFromCandidates = cms.bool(True),
    particlePDG = cms.int32(211),
    SimCaloRecHitsEcalEB = cms.InputTag("famosSimHits", "EcalHitsEB"),
    SimCaloRecHitsEcalEE = cms.InputTag("famosSimHits", "EcalHitsEE"),
    SimCaloRecHitsHcal = cms.InputTag("famosSimHits", "HcalHits")
)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('/castor/cern.ch/user/b/ballin/DipionDelegate_50GeV_BarrelOnly_100k_2_1_11.root')
    #fileName = cms.string('DipionDelegate_50GeV_2_1_11.root')
)

#process.finishup = cms.OutputModule("PoolOutputModule",
#   fileName=cms.untracked.string("finishup.root"),
#    outputCommands=cms.untracked.vstring('keep *')
#    
#)



process.p1 = cms.Path(process.famosWithElectrons+process.famosWithCaloTowersAndParticleFlow+process.caloJetMetGen*process.particleFlowSimParticle*process.extraction)
#process.outpath = cms.EndPath(process.finishup)




