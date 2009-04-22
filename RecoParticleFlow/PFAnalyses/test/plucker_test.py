import FWCore.ParameterSet.Config as cms

process = cms.Process("PROD")

import FWCore.ParameterSet.VarParsing as VarParsing

# setup 'standard'  options
options = VarParsing.VarParsing()
options.register ('beamEnergy',
                  100, # default value
                  options.multiplicity.singleton, # singleton or list
                  options.varType.int, # string, int, or float
                  "Beam energy to simulate")

options.register ('kevents',
                  1000, # default value
                  options.multiplicity.singleton, # singleton or list
                  options.varType.int, # string, int, or float
                  "Number of events in k to simulate")

options.register ('fileSuffix',
                  '', # default value
                  options.multiplicity.singleton, # singleton or list
                  options.varType.string,          # string, int, or float
                  "Label to append to output file names")

# setup any defaults you want
options.beamEnergy = 100
options.kevents = 1

# get and parse the command line arguments
options.parseArguments()



outputFile = "Plucker_" + str(options.beamEnergy) + "GeV_" + str(options.kevents) + "k_" + options.fileSuffix +".root"

print ("plucker_test.py with options:")
print "Beam energy: " + str(options.beamEnergy)
print "kevents: " + str(options.kevents)
print "fileSuffix: " + options.fileSuffix
print "Output file: " + outputFile


process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(options.kevents * 1000)
#    input=cms.untracked.int32(200)
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
#process.load("Configuration.StandardSequences.MagneticField_38T_cff")
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
    PGunParameters=cms.PSet(
        ParticleID=cms.vint32(211),
        MaxEta=cms.double(1.2),
        MaxPhi=cms.double(3.14),
        MinPhi=cms.double(0.0),
        MinEta=cms.double(0.0),
        MinE=cms.double(options.beamEnergy),
        AddAntiParticle=cms.bool(True),
        MaxE=cms.double(options.beamEnergy+0.0001)
    ),
    pythiaVerbosity=cms.untracked.bool(False),
    PythiaParameters=cms.PSet(
       pythiaUESettingsBlock,
    # This is a vector of ParameterSet names to be read, in this order
       parameterSets=cms.vstring('pythiaUESettings')
    )

)

process.particleFlowRecHitHCAL.thresh_Barrel = cms.double(0.0)
process.particleFlowClusterHCAL.thresh_Seed_Barrel = cms.double(1.4)
process.particleFlowClusterHCAL.thresh_Barrel = cms.double(0.8)
process.particleFlowBlock.pf_chi2_ECAL_HCAL = cms.double(100.0)


process.dump = cms.EDAnalyzer("EventContentAnalyzer")

process.plucker = cms.EDFilter("CalibratablePlucker" ,
    PFCandidates=cms.InputTag("particleFlow"),
)

process.extraction = cms.EDProducer("CalibratableProducer",
    useSimAsTrack=cms.bool(True),
    PFClustersHcal=cms.InputTag("particleFlowClusterHCAL"),
    deltaRClustersToTrack=cms.double(0.4),
    EventDelegateType=cms.string('DipionDelegate'),
    deltaRCandToTrack=cms.double(0.4),
    neutralMode=cms.bool(True),
    noSimDaughters=cms.bool(True),
    PFCandidates=cms.InputTag("particleFlow"),
    deltaRRechitsToTrack=cms.double(0.15),
    deltaPhi=cms.double(0.15),
    debug=cms.int32(3),
    PFRecTracks=cms.InputTag("trackerDrivenElectronSeeds"),
    PFRecHitsEcal=cms.InputTag("particleFlowRecHitECAL"),
    deltaEta=cms.double(0.15),
    PFSimParticles=cms.InputTag("particleFlowSimParticle"),
    PFRecHitsHcal=cms.InputTag("particleFlowRecHitHCAL"),
    isMC=cms.bool(True),
    PFClustersEcal=cms.InputTag("particleFlowClusterECAL"),
    clustersFromCandidates=cms.bool(True),
    rechitsFromCandidates=cms.bool(False),
    particlePDG=cms.int32(130),
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

#
#process.TFileService = cms.Service("TFileService",
#    fileName=cms.string(outputTree)
#)

process.finishup = cms.OutputModule("PoolOutputModule",
   fileName=cms.untracked.string(outputFile),
   outputCommands=cms.untracked.vstring('drop *', 'keep *_plucker_*_*'),
    
)


process.p1 = cms.Path(process.generator + process.famosWithEverything + process.caloJetMetGen * process.particleFlowSimParticle * process.plucker)
process.outpath = cms.EndPath(process.finishup)




