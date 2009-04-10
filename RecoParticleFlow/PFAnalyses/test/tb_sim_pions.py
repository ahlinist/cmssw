import FWCore.ParameterSet.Config as cms

process = cms.Process("PROD")

import FWCore.ParameterSet.VarParsing as VarParsing

# setup 'standard'  options
options = VarParsing.VarParsing()
options.register ('beamEnergy',
                  100, # default value
                  options.multiplicity.singleton, # singleton or list
                  options.varType.int,          # string, int, or float
                  "Beam energy to simulate")

options.register ('kevents',
                  1000, # default value
                  options.multiplicity.singleton, # singleton or list
                  options.varType.int,          # string, int, or float
                  "Number of events in k to simulate")

# setup any defaults you want
options.beamEnergy = 100
options.kevents = 1

# get and parse the command line arguments
options.parseArguments()


outputTree = "DipionDelegate_mono" + str(options.beamEnergy) + "GeV_Testbeam_" + str(options.kevents) + "k_3_1_Fast.root"
outputFile = "DipionDelegate_mono" + str(options.beamEnergy) + "GeV_Testbeam_" + str(options.kevents) + "k_3_1_Collections_Fast.root"

print ("tb_sim_pions.py with options:")
print "Beam energy: " + str(options.beamEnergy)
print "kevents: " + str(options.kevents)

print "Output file: " + outputFile
print "Output tree: " + outputTree


process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(options.kevents * 1000)
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
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.VolumeBasedMagneticFieldESProducer.useParametrizedTrackerField = True

process.famosSimHits.MaterialEffects.PairProduction = False
process.famosSimHits.MaterialEffects.Bremsstrahlung = False
process.famosSimHits.MaterialEffects.EnergyLoss = False
process.famosSimHits.MaterialEffects.MultipleScattering = False
process.famosSimHits.MaterialEffects.NuclearInteraction = False
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
        MinE = cms.double(options.beamEnergy),
        AddAntiParticle = cms.bool(False),
        MinPhi = cms.double(0.02),
        MaxE = cms.double(options.beamEnergy+0.001)
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
    debug = cms.int32(4),
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
    SimCaloRecHitsHcal = cms.InputTag("famosSimHits", "HcalHits"),
    deltaREcalCaloWindow=cms.double(0.0125),
    deltaRHcalCaloWindow=cms.double(0.04),
    nRingsEcalCaloWindow=cms.uint32(5),
    nRingsHcalCaloWindow=cms.uint32(3),
    nPanesEcalCaloWindow=cms.uint32(4),
    nPanesHcalCaloWindow=cms.uint32(4),
)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string(outputTree)
)

process.finishup = cms.OutputModule("PoolOutputModule",
   fileName=cms.untracked.string(outputFile),
   outputCommands=cms.untracked.vstring('drop *', 'keep *_particleFiltration_*_*', 'keep recoMuons_*_*_*', 'keep *_calibratable_*_*', 'keep *_faketracks_*_*', 'keep recoPFRecTracks_*_*_*', 'keep recoPFRecHits_*_*_*', 'keep recoPFClusters_*_*_*', 'keep recoPFBlocks_*_*_*', 'keep recoPFCandidates_*_*_*'),
    
)


process.p1 = cms.Path(process.generator+process.famosWithEverything+process.caloJetMetGen*process.particleFlowSimParticle*process.extraction)
process.outpath = cms.EndPath(process.finishup)




