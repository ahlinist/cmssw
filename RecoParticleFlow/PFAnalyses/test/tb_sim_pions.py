import FWCore.ParameterSet.Config as cms

process = cms.Process("PROD")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
)

#fastsim
process.load("FastSimulation.Configuration.RandomServiceInitialization_cff")
process.load("FastSimulation.Configuration.CommonInputs_cff")
process.load("FastSimulation.Configuration.FamosSequences_cff")
process.GlobalTag.globaltag = "IDEAL_30X::All"

process.load("FastSimulation.Configuration.RandomServiceInitialization_cff")

process.famosSimHits.SimulateCalorimetry = True
process.famosSimHits.SimulateTracking = True
process.famosPileUp.PileUpSimulator.averageNumber = 0.0

process.famosSimHits.VertexGenerator.BetaStar = 0.00001
process.famosSimHits.VertexGenerator.SigmaZ = 0.00001

# Parametrized magnetic field (new mapping, 4.0 and 3.8T)
#from Configuration.StandardSequences.MagneticField_cff import *
process.load("Configuration.StandardSequences.MagneticField_40T_cff")
process.VolumeBasedMagneticFieldESProducer.useParametrizedTrackerField = True

process.famosSimHits.MaterialEffects.PairProduction = True
process.famosSimHits.MaterialEffects.Bremsstrahlung = True
process.famosSimHits.MaterialEffects.EnergyLoss = True
process.famosSimHits.MaterialEffects.MultipleScattering = True
process.famosSimHits.MaterialEffects.NuclearInteraction = True
process.famosSimHits.Calorimetry.UnfoldedMode = True

process.load("RecoParticleFlow.PFBlockProducer.particleFlowSimParticle_cff")

process.source = cms.Source("EmptySource")
from Configuration.Generator.PythiaUESettings_cfi import *

process.generator = cms.EDProducer("Pythia6EGun",
    PGunParameters = cms.PSet(
        ParticleID = cms.vint32(211),
        MaxEta = cms.double(0.22),
        MaxPhi = cms.double(0.02),
        MinEta = cms.double(0.22),
        MinE = cms.double(100.0),
        AddAntiParticle = cms.bool(False),
        MinPhi = cms.double(0.02),
        MaxE = cms.double(100.01)
    ),
    pythiaVerbosity = cms.untracked.bool(False),
    PythiaParameters = cms.PSet(
       pythiaUESettingsBlock,
    # This is a vector of ParameterSet names to be read, in this order
       parameterSets = cms.vstring('pythiaUESettings')
    )

)
#process.ProductionFilterSequence = cms.Sequence(generator)


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
    PFRecTracks = cms.InputTag("trackerDrivenElectronSeeds"),
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
    fileName = cms.string('DipionDelegate_mono100GeV_Testbeam_1k_3_1_FastAllEffects.root')
)

process.finishup = cms.OutputModule("PoolOutputModule",
   fileName=cms.untracked.string("DipionDelegate_mono100GeV_Testbeam_1k_3_1_Collections_FastAllEffects.root"),
    outputCommands=cms.untracked.vstring('keep *')
    
)



process.p1 = cms.Path(process.generator+process.famosWithEverything+process.caloJetMetGen*process.particleFlowSimParticle*process.extraction)
process.outpath = cms.EndPath(process.finishup)




