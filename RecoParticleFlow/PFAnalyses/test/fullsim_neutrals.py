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


outputTree = "DipionDelegate_" + str(options.beamEnergy) + "GeV_" + str(options.kevents) + "k_" + options.fileSuffix +".root"
outputFile = "DipionDelegate_" + str(options.beamEnergy) + "GeV_" + str(options.kevents) + "k_Collections_" + options.fileSuffix +".root"

print ("neutral_fullsim.py with options:")
print "Beam energy: " + str(options.beamEnergy)
print "kevents: " + str(options.kevents)
print "fileSuffix: " + options.fileSuffix
print "Output file: " + outputFile
print "Output tree: " + outputTree


process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(options.kevents * 1000)
)

process.source = cms.Source("EmptySource")

from Configuration.Generator.PythiaUESettings_cfi import *
process.generator = cms.EDProducer("Pythia6EGun",
    PGunParameters = cms.PSet(
       ParticleID = cms.vint32(130),
       MinPhi = cms.double(0.02),
       MaxPhi = cms.double(0.02),
       MinEta = cms.double(0.2),
       MaxEta = cms.double(0.2),
       MinE = cms.double(options.beamEnergy),
       MaxE = cms.double(options.beamEnergy+0.0001),
       AddAntiParticle = cms.bool(True)
     ),
    pythiaVerbosity = cms.untracked.bool(False),
    PythiaParameters = cms.PSet(
        pythiaUESettingsBlock,
        # This is a vector of ParameterSet names to be read, in this order
        parameterSets = cms.vstring(
            'pythiaUESettings'
       )
    )
)

process.ProductionFilterSequence = cms.Sequence(process.generator)


process.load("Configuration.StandardSequences.SimulationRandomNumberGeneratorSeeds_cff")

#simulation
process.load("Configuration.StandardSequences.Simulation_cff")
process.load("Configuration.StandardSequences.MixingNoPileUp_cff")
process.load("Configuration.StandardSequences.Services_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.RawToDigi_cff")
process.load("Configuration.StandardSequences.DigiToRaw_cff")
process.GlobalTag.globaltag = "IDEAL_30X::All"
#reconstruction
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("Configuration.StandardSequences.MagneticField_0T_cff")
process.load("Configuration.StandardSequences.Geometry_cff")
#Trigger
process.load("Configuration.StandardSequences.L1Emulator_cff")


process.particleFlowRecHitHCAL.thresh_Barrel = cms.double(0.0)
process.particleFlowClusterHCAL.thresh_Seed_Barrel = cms.double(1.4)
process.particleFlowClusterHCAL.thresh_Barrel = cms.double(0.8)
process.particleFlowBlock.pf_chi2_ECAL_HCAL = cms.double(100.0)


process.MessageLogger = cms.Service("MessageLogger",
                                    destinations=cms.untracked.vstring('cout'),
                                    FullSim=cms.untracked.PSet(
    threshold=cms.untracked.string('INFO')
    )
)

process.extraction = cms.EDAnalyzer("ExtractionAnalyzer",
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
    SimCaloRecHitsEcalEB = cms.InputTag("g4SimHits", "EcalHitsEB"),
    SimCaloRecHitsEcalEE = cms.InputTag("g4SimHits", "EcalHitsEE"),
    SimCaloRecHitsHcal = cms.InputTag("g4SimHits", "HcalHits"),
    RawRecHitsEcalEB=cms.InputTag("ecalRecHit","EcalRecHitsEB"),
    RawRecHitsEcalEE=cms.InputTag("ecalRecHit","EcalRecHitsEE"),
    RawRecHitsHcal=cms.InputTag("hbhereco"),
        
    deltaREcalCaloWindow=cms.double(0.01),
    deltaRHcalCaloWindow=cms.double(0.05),
    nRingsEcalCaloWindow=cms.uint32(5),
    nRingsHcalCaloWindow=cms.uint32(3),
    nPanesEcalCaloWindow=cms.uint32(1),
    nPanesHcalCaloWindow=cms.uint32(1),
)



process.TFileService = cms.Service("TFileService",
    fileName = cms.string(outputTree)
)

process.p1 = cms.Path(
    process.ProductionFilterSequence +
    process.simulation + 
    process.L1Emulator + 
    process.DigiToRaw + 
    process.RawToDigi + 
    process.localreco + 
    process.globalreco + 
    process.egammareco + 
    process.particleFlowReco +
    process.particleFlowSimParticle
)


#process.finishup = cms.OutputModule("PoolOutputModule",
#   fileName=cms.untracked.string("DipionDelegate_mono100GeV_Testbeam_50evts_3_1_Collections_FullSim_38T.root"),
#    outputCommands=cms.untracked.vstring('keep *')
#    
#)

#process.outpath = cms.EndPath(
#   process.finishup
#)

#
