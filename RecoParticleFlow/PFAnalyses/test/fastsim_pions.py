import FWCore.ParameterSet.Config as cms

process = cms.Process("PROD")

from RecoParticleFlow.PFAnalyses.pflowOptions_cfi import *

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
if options.zeroT <> 0 :
	process.load("Configuration.StandardSequences.MagneticField_0T_cff")
else :
	process.load("Configuration.StandardSequences.MagneticField_40T_cff")
process.VolumeBasedMagneticFieldESProducer.useParametrizedTrackerField = True

process.famosSimHits.MaterialEffects.PairProduction = False
process.famosSimHits.MaterialEffects.Bremsstrahlung = False
process.famosSimHits.MaterialEffects.EnergyLoss = False
process.famosSimHits.MaterialEffects.MultipleScattering = False
process.famosSimHits.MaterialEffects.NuclearInteraction = False
process.famosSimHits.Calorimetry.UnfoldedMode = True

if options.randSeed <> 0:
	from random import *
	process.RandomNumberGeneratorService.generator.initialSeed=int(random()*100000)

if options.noZspSr <> 0 :
	process.ecalRecHit.RecHitsFactory.ECALBarrel.Noise=cms.double(0.04)
	process.ecalRecHit.RecHitsFactory.ECALBarrel.Threshold=cms.double(0.01)
	process.ecalRecHit.RecHitsFactory.ECALBarrel.SRThreshold=cms.double(0.01)
	process.ecalRecHit.RecHitsFactory.ECALBarrel.doDigis=cms.bool(False)
	process.hcalRecHit.RechitsFactory.hbhereco=cms.vdouble(-9999,-9999)


process.source = cms.Source("EmptySource")

from Configuration.Generator.PythiaUESettings_cfi import *
process.generator = cms.EDProducer("Pythia6EGun",
    PGunParameters = cms.PSet(
       ParticleID = cms.vint32(211),
       MinPhi = cms.double(0),
       MaxPhi = cms.double(2 * 3.14),
       MinEta = cms.double(0.2),
       MaxEta = cms.double(0.2001),
       MinE = cms.double(options.minBeamEnergy),
       MaxE = cms.double(options.beamEnergy+0.0001),
       AddAntiParticle = cms.bool(True)
     ),
    pythiaVerbosity = cms.untracked.bool(False),
    PythiaParameters = cms.PSet(
        pythiaUESettingsBlock,
        parameterSets = cms.vstring(
            'pythiaUESettings'
       )
    )
)

if options.endcapMode <> 0:
    process.generator = cms.EDProducer("Pythia6EGun",
    PGunParameters = cms.PSet(
       ParticleID = cms.vint32(211),
       MinPhi = cms.double(0),
       MaxPhi = cms.double(2 * 3.14),
       MinEta = cms.double(1.6),
       MaxEta = cms.double(1.6001),
       MinE = cms.double(options.minBeamEnergy),
       MaxE = cms.double(options.beamEnergy+0.0001),
       AddAntiParticle = cms.bool(True)
     ),
    pythiaVerbosity = cms.untracked.bool(False),
    PythiaParameters = cms.PSet(
        pythiaUESettingsBlock,
        parameterSets = cms.vstring(
            'pythiaUESettings'
       )
    )
    )

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 10
#process.MessageLogger.destinations=cms.untracked.vstring('Dipion_' + logLabel, 'cout')

genEvents = 100
if options.kevents <> 0:
    genEvents = options.kevents * 1000
else:
    print 'Generating 100 events by default (alter with kevents=X option)'
    
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(genEvents)
)

process.ProductionFilterSequence = cms.Sequence(process.generator)
process.particleFlow.pf_nsigma_HCAL = cms.double(1.0)
process.particleFlow.pf_calibMode = cms.uint32(2)
process.particleFlowBlock.pf_chi2_ECAL_HCAL = cms.double(100.0)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('Dipion_Tree_' + fileLabel)
)

import RecoParticleFlow.PFAnalyses.pflowCalibratable_cfi as calibratable

process.extractionToTree = cms.EDAnalyzer("ExtractionAnalyzer",
    calibratable.EventDelegate
)

process.extractionToEvent = cms.EDProducer("CalibratableProducer",
    calibratable.EventDelegate
)

if options.zeroT <>0 :
        #assume we still fire pi+!
        process.extractionToTree.neutralMode = cms.bool(True)
        process.extractionToTree.particlePDG=cms.int32(211)
        process.extractionToEvent.neutralMode = cms.bool(True)
        process.extractionToEvent.particlePDG=cms.int32(211)

else:
        process.extractionToTree.neutralMode = cms.bool(False)
        process.extractionToTree.particlePDG=cms.int32(211)
        process.extractionToEvent.neutralMode = cms.bool(False)
        process.extractionToEvent.particlePDG=cms.int32(211)



process.finishup = cms.OutputModule("PoolOutputModule",
    fileName=cms.untracked.string('Dipion_Events_' + fileLabel),
    #outputCommands=cms.untracked.vstring('keep *'),
    outputCommands=cms.untracked.vstring('drop *', 
                                         'keep *_particleFiltration_*_*', 
                                         'keep recoMuons_*_*_*', 
                                         'keep *_calibratable_*_*', 
                                         'keep *_faketracks_*_*', 
                                         'keep recoPFRecTracks_*_*_*', 
                                         'keep recoPFRecHits_*_*_*', 
                                         'keep recoPFClusters_*_*_*', 
                                         'keep recoPFBlocks_*_*_*', 
                                         'keep recoPFCandidates_*_*_*'),
    SelectEvents=cms.untracked.PSet(
         SelectEvents=cms.vstring('p1')
    )
)

process.p1 = cms.Path(process.generator + 
                      process.famosWithEverything + 
                      process.caloJetMetGen + 
                      process.particleFlowSimParticle + 
                      process.extractionToTree + 
                      process.extractionToEvent)

if options.outputCollections <> 0:
    process.outpath = cms.EndPath(process.finishup)


