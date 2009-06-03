import FWCore.ParameterSet.Config as cms

process = cms.Process("PROD")

from RecoParticleFlow.PFAnalyses.pflowOptions_cfi import *

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

process.source = cms.Source("EmptySource")

from Configuration.Generator.PythiaUESettings_cfi import *
process.generator = cms.EDProducer("Pythia6EGun",
    PGunParameters = cms.PSet(
       ParticleID = cms.vint32(211),
       MinPhi = cms.double(0),
       MaxPhi = cms.double(2 * 3.14),
       MinEta = cms.double(0),
       MaxEta = cms.double(1.0),
       MinE = cms.double(options.minBeamEnergy),
       MaxE = cms.double(options.beamEnergy+0.0001),
       AddAntiParticle = cms.bool(False)
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



process.MessageLogger = cms.Service("MessageLogger",
#    destinations=cms.untracked.vstring('Dipion_' + logLabel),
    destinations=cms.untracked.vstring('cout'),
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10)
)

process.ProductionFilterSequence = cms.Sequence(process.generator)

process.particleFlowRecHitHCAL.thresh_Barrel = cms.double(0.0)
process.particleFlowClusterHCAL.thresh_Seed_Barrel = cms.double(0.6)
process.particleFlowClusterHCAL.thresh_Barrel = cms.double(0.6)
process.particleFlowBlock.pf_chi2_ECAL_HCAL = cms.double(100.0)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('Dipion_' + fileLabel)
)

from RecoParticleFlow.PFAnalyses.pflowCalibratable_cfi import *

process.extraction = cms.EDAnalyzer("ExtractionAnalyzer",
    EventDelegate
)

process.extraction.neutralMode = cms.bool(False)
process.extraction.particlePDG=cms.int32(211)

process.finishup = cms.OutputModule("PoolOutputModule",
    fileName=cms.untracked.string("Dipion_Events_.root"),
    outputCommands=cms.untracked.vstring('keep *'),
    #outputCommands=cms.untracked.vstring('drop *', 'keep *_particleFiltration_*_*', 'keep recoMuons_*_*_*', 'keep *_calibratable_*_*', 'keep *_faketracks_*_*', 'keep recoPFRecTracks_*_*_*', 'keep recoPFRecHits_*_*_*', 'keep recoPFClusters_*_*_*', 'keep recoPFBlocks_*_*_*', 'keep recoPFCandidates_*_*_*'),
#    SelectEvents=cms.untracked.PSet(
#                SelectEvents=cms.vstring('p1')
#    ) 
)

process.famosSimulationSequence2 = cms.Sequence(
    process.offlineBeamSpot+
    process.famosPileUp+
    process.famosSimHits+
#    process.MuonSimHits+
    process.mix
)



#process.p1 = cms.Path(process.generator + process.famosWithEverything + process.caloJetMetGen + process.particleFlowSimParticle + process.extraction)
process.p1 = cms.Path(process.generator + process.famosSimulationSequence2)
process.outpath = cms.EndPath(process.finishup)


