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
process.GlobalTag.globaltag = "MC_31X_V1::All"
#reconstruction
process.load("Configuration.StandardSequences.Reconstruction_cff")

if options.zeroT <> 0:
	process.load("Configuration.StandardSequences.MagneticField_0T_cff")
else:
	process.load("Configuration.StandardSequences.MagneticField_40T_cff")


process.load("Configuration.StandardSequences.Geometry_cff")
#Trigger
process.load("Configuration.StandardSequences.L1Emulator_cff")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 10
#process.MessageLogger.destinations=cms.untracked.vstring('cout')

process.source = cms.Source("EmptySource")

from Configuration.Generator.PythiaUESettings_cfi import *
#process.load("SimCalorimetry.HcalZeroSuppressionProducers.hcalDigisRealistic_cfi")
#process.load("SimCalorimetry.EcalSelectiveReadoutProducers.ecalDigis_cfi")
	
if options.noZspSr :
	process.simEcalDigis.srpBarrelLowInterestChannelZS = cms.double(-1.e9)
	process.simEcalDigis.srpEndcapLowInterestChannelZS = cms.double(-1.e9)
	process.simHcalDigis.HBlevel = cms.int32(-8000)
	process.simHcalDigis.HElevel = cms.int32(-8000)
	process.simHcalDigis.HOlevel = cms.int32(-8000)
	process.simHcalDigis.HFlevel = cms.int32(-9000)

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

if options.randSeed <> 0:
	from random import *
	process.RandomNumberGeneratorService.generator.initialSeed=int(random()*100000)

genEvents=10
if options.kevents <>0:
	genEvents = options.kevents * 1000
else:
	print "Option kevents not specified; will generate 10 events."

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(genEvents)
)

process.ProductionFilterSequence = cms.Sequence(process.generator)

process.particleFlow.pf_nsigma_HCAL = cms.double(5.0)
process.particleFlowBlock.pf_chi2_ECAL_HCAL = cms.double(100.0)
process.particleFlow.pf_calibMode = cms.uint32(3)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('Dipion_Tree_' + fileLabel)
)

import RecoParticleFlow.PFAnalyses.pflowCalibratable_cfi as calibratable

process.extractionToTree = cms.EDAnalyzer("ExtractionAnalyzer",
    calibratable.EventDelegateFullSim
)


process.extractionToEvent = cms.EDProducer("CalibratableProducer",
    calibratable.EventDelegateFullSim      
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
    process.extractionToEvent +
    process.extractionToTree
)

if options.outputCollections:
    process.outpath = cms.EndPath(process.finishup)
