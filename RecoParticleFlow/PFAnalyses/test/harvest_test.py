import FWCore.ParameterSet.Config as cms

process = cms.Process("PROD")

import FWCore.ParameterSet.VarParsing as VarParsing

from RecoParticleFlow.PFAnalyses.pflowOptions_cfi import *

outputFile = "Plucker_" + fileLabel
outputTree="Plucker_tree_" + fileLabel
print outputFile

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(options.kevents * 1000)
)

#fastsim
process.load("FastSimulation.Configuration.RandomServiceInitialization_cff")
process.load("FastSimulation.Configuration.CommonInputs_cff")
process.load("FastSimulation.Configuration.FamosSequences_cff")
process.GlobalTag.globaltag = "MC_31X_V1::All"

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

process.load("RecoParticleFlow.PFProducer.particleFlowSimParticle_cff")

process.source = cms.Source("EmptySource")
from Configuration.Generator.PythiaUESettings_cfi import *

process.generator = cms.EDProducer("Pythia6EGun",
    PGunParameters=cms.PSet(
        ParticleID=cms.vint32(211),
        MaxEta=cms.double(1.2),
        MaxPhi=cms.double(3.14),
        MinPhi=cms.double(0.0),
        MinEta=cms.double(0.0),
        MinE=cms.double(options.minBeamEnergy),
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

#process.particleFlowRecHitHCAL.thresh_Barrel = cms.double(0.0)
#process.particleFlowClusterHCAL.thresh_Seed_Barrel = cms.double(1.4)
#process.particleFlowClusterHCAL.thresh_Barrel = cms.double(0.8)
#process.particleFlowRecHitHCAL.isTestbeam = cms.bool(False)
process.particleFlowBlock.pf_chi2_ECAL_HCAL = cms.double(100.0)


process.dump = cms.EDAnalyzer("EventContentAnalyzer")


import RecoParticleFlow.PFAnalyses.pflowCalibratable_cfi as calibratable
 
process.harvestPF = cms.EDFilter("CalibratableHarvester" ,
    PFCandidates=cms.InputTag("particleFlow"),
    PFSimParticles=cms.InputTag("particleFlowSimParticle"),
    isMC = cms.bool(True),
    LimitCandidatesPerEvent=cms.uint32(3),
    DeltaREcalIsolation=cms.double(0.30),
)

process.extraction = cms.EDProducer("ExtractionAnalyzer",
        calibratable.EventDelegate         
)


process.TFileService = cms.Service("TFileService",
    fileName=cms.string(outputTree)
)

process.finishup = cms.OutputModule("PoolOutputModule",
   fileName=cms.untracked.string(outputFile),
   outputCommands=cms.untracked.vstring('drop *', 'keep *_harvestPF_*_*', 'keep *_extraction_*_*'),
    
)


process.p1 = cms.Path(process.generator + process.famosWithEverything + process.caloJetMetGen * process.particleFlowSimParticle * process.pluckingPF * process.extraction)
process.outpath = cms.EndPath(process.finishup)




