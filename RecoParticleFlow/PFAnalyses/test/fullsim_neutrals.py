import FWCore.ParameterSet.Config as cms


process = cms.Process("PROD")


from RecoParticleFlow.PFAnalyses.pflowOptions_cfi import *

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
                                    destinations=cms.untracked.vstring('Dikaon_' + logLabel),
                                    FullSim=cms.untracked.PSet(
    threshold=cms.untracked.string('INFO')
    )
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
       MinE = cms.double(options.minBeamEnergy),
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

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(options.kevents*1000)
)

process.ProductionFilterSequence = cms.Sequence(process.generator)

process.particleFlowRecHitHCAL.thresh_Barrel = cms.double(0.0)
process.particleFlowClusterHCAL.thresh_Seed_Barrel = cms.double(1.4)
process.particleFlowClusterHCAL.thresh_Barrel = cms.double(0.8)
process.particleFlowBlock.pf_chi2_ECAL_HCAL = cms.double(100.0)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('Dikaon_' + fileLabel)
)

from RecoParticleFlow.PFAnalyses.pflowCalibratable_cfi import *
process.extraction = cms.EDAnalyzer("ExtractionAnalyzer",
    EventDelegateFullSim
)
process.extraction.neutralMode = cms.bool(True)
process.extraction.particlePDG=cms.int32(130)

process.p1 = cms.Path(
    process.ProductionFilterSequence +
    process.simulation +
    process.L1Emulator + 
    process.DigiToRaw + 
    process.RawToDigi + 
    process.localreco + 
    process.globalreco + 
    process.particleFlowReco +
    process.particleFlowSimParticle + 
    process.extraction
)

process.finishup = cms.OutputModule("PoolOutputModule",
    fileName=cms.untracked.string("Dikaon_Event_" + fileLabel),
    #outputCommands=cms.untracked.vstring('keep *'),
    outputCommands=cms.untracked.vstring('drop *', 'keep *_particleFiltration_*_*', 'keep recoMuons_*_*_*', 'keep *_calibratable_*_*', 'keep *_faketracks_*_*', 'keep recoPFRecTracks_*_*_*', 'keep recoPFRecHits_*_*_*', 'keep recoPFClusters_*_*_*', 'keep recoPFBlocks_*_*_*', 'keep recoPFCandidates_*_*_*'),
    SelectEvents=cms.untracked.PSet(
                SelectEvents=cms.vstring('p1')
    ) 
)

process.outpath = cms.EndPath(process.finishup)
