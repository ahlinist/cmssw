import FWCore.ParameterSet.Config as cms


process = cms.Process("PROD")


process.maxEvents = cms.untracked.PSet(
    input=cms.untracked.int32(1000)
)

#generation

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



process.MessageLogger = cms.Service("MessageLogger",
                                    destinations=cms.untracked.vstring('cout'),
                                    FullSim=cms.untracked.PSet(
    threshold=cms.untracked.string('INFO')
    )
)

process.source = cms.Source("EmptySource")

from Configuration.Generator.PythiaUESettings_cfi import *
process.generator = cms.EDProducer("Pythia6EGun",
    PGunParameters = cms.PSet(
       ParticleID = cms.vint32(211),
       MinPhi = cms.double(0.02),
       MaxPhi = cms.double(0.02),
       MinEta = cms.double(0.2),
       MaxEta = cms.double(0.2),
       MinE = cms.double(20),
       MaxE = cms.double(20+0.0001),
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



process.particleFlowRecHitHCAL.thresh_Barrel = cms.double(0.0)
process.particleFlowClusterHCAL.thresh_Seed_Barrel = cms.double(1.4)
process.particleFlowClusterHCAL.thresh_Barrel = cms.double(0.8)
process.particleFlowBlock.pf_chi2_ECAL_HCAL = cms.double(100.0)


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
    SimCaloRecHitsEcalEB = cms.InputTag("g4SimHits", "EcalHitsEB"),
    SimCaloRecHitsEcalEE = cms.InputTag("g4SimHits", "EcalHitsEE"),
    SimCaloRecHitsHcal = cms.InputTag("g4SimHits", "HcalHits"),
        
    deltaREcalCaloWindow=cms.double(0.01),
    deltaRHcalCaloWindow=cms.double(0.05),
    nRingsEcalCaloWindow=cms.uint32(5),
    nRingsHcalCaloWindow=cms.uint32(3),
    nPanesEcalCaloWindow=cms.uint32(1),
    nPanesHcalCaloWindow=cms.uint32(1),
)



process.TFileService = cms.Service("TFileService",
    fileName = cms.string('DipionDelegate_mono50GeV_Testbeam_100evts_3_1_FullSim_0T.root')
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
