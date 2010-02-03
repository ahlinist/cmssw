import FWCore.ParameterSet.Config as cms


##################################################################################################
##################################################################################################
##################################################################################################
# the simulation parameters...

NumberOfEvents = 10

PileUp = 0

GunPartID = [11]
GunMinPt = 10
GunMaxPt = 50
GunMinEta = -1.5
GunMaxEta = 1.5
GunAddAntiParticle = False

Geometry = "strawmanb"

#Set the nominal threshold for TT stubs and tracklets
TT_threshold = 2

includelowptjunk = True

if includelowptjunk:
	cfgNote = "remove pt cut and add loopers"
else:
	cfgNote = "include pt cut and exclude loopers"

##################################################################################################
##################################################################################################
##################################################################################################


process = cms.Process("Fastsimwdigi")

# Number of events to be generated
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(NumberOfEvents)
)

process.load("FastSimulation.Configuration.RandomServiceInitialization_cff")
process.RandomNumberGeneratorService.simSiStripDigis = cms.PSet(
      initialSeed = cms.untracked.uint32(1234567),
      engineName = cms.untracked.string('HepJamesRandom'))
process.RandomNumberGeneratorService.simSiPixelDigis = cms.PSet(
      initialSeed = cms.untracked.uint32(1234567),
      engineName = cms.untracked.string('HepJamesRandom'))

#process.source = cms.Source("EmptySource")

# Generate muons with a flat pT particle gun, and with pT=10.
process.load("FastSimulation/Configuration/FlatPtMuonGun_cfi")
process.FlatRandomPtGunSource.PGunParameters.PartID = GunPartID
process.FlatRandomPtGunSource.PGunParameters.MinPt = GunMinPt
process.FlatRandomPtGunSource.PGunParameters.MaxPt = GunMaxPt
process.FlatRandomPtGunSource.PGunParameters.MinEta = GunMinEta
process.FlatRandomPtGunSource.PGunParameters.MaxEta = GunMaxEta
process.FlatRandomPtGunSource.AddAntiParticle = GunAddAntiParticle

# from std full sim (eventsetup for digis?)
process.load("Configuration.StandardSequences.FakeConditions_cff")

#process.load("CalibTracker/Configuration/SiPixel_FakeConditions_cff")
process.load("Configuration.StandardSequences.GeometryPilot2_cff")
#process.load("Configuration.StandardSequences.MagneticField_cff")
#process.load("CalibTracker/Configuration/SiStrip_FakeConditions_cff")
#process.load("CalibTracker/Configuration/SiStripGain.SiStripGain_Fake_cff")
#process.load("CalibTracker.Configuration.SiStripPedestals.SiStripPedestals_Fake_cff")

# Famos sequences (fake conditions)
process.load("FastSimulation.Configuration.CommonInputsFake_cff")
# Famos sequences (Frontier conditions)
#process.load("FastSimulation/Configuration/CommonInputs_cff")


process.load("FastSimulation/Configuration/FamosSequences_cff")
#process.GlobalTag.globaltag = "IDEAL_V12::All"

# replace with strawmanb geometry
process.load("SLHCUpgradeSimulations.Geometry."+Geometry+"_cmsIdealGeometryXML_cff")


# Parametrized magnetic field (new mapping, 4.0 and 3.8T)
#process.load("Configuration.StandardSequences.MagneticField_40T_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.VolumeBasedMagneticFieldESProducer.useParametrizedTrackerField = True

process.load("SLHCUpgradeSimulations.Utilities.StackedTrackerGeometry_cfi")
#process.StackedTrackerGeometryESModule.make_debug_file = True

# If you want to turn on/off pile-up
process.famosPileUp.PileUpSimulator.averageNumber = PileUp
# You may not want to simulate everything for your study
process.famosSimHits.SimulateCalorimetry = True
process.famosSimHits.SimulateTracking = True

if includelowptjunk:
	process.famosSimHits.ParticleFilter.etaMax = 3.0
	process.famosSimHits.ParticleFilter.pTMin = 0.05
	process.famosSimHits.TrackerSimHits.pTmin = 0.05
	process.famosSimHits.TrackerSimHits.firstLoop = False

process.load("SimTracker.Configuration.SimTracker_cff")
process.simSiPixelDigis.ROUList =  ['famosSimHitsTrackerHits']
process.simSiPixelDigis.MissCalibrate = False
process.simSiPixelDigis.AddPixelInefficiency = -1
process.simSiStripDigis.ROUList =  ['famosSimHitsTrackerHits']
process.simSiPixelDigis.LorentzAngle_DB = False
process.simSiPixelDigis.killModules = False

process.load("FastSimulation.CaloRecHitsProducer.CaloRecHits_cff")
process.caloRecHits.RecHitsFactory.doDigis = True

#Load Scales
process.load("L1TriggerConfig.L1ScalesProducers.L1CaloInputScalesConfig_cff")
process.load("L1TriggerConfig.L1ScalesProducers.L1CaloScalesConfig_cff")

process.load("SLHCUpgradeSimulations.L1Trigger.L1SLHCEmulator_cff")




#######################################################################################################################
#######################################################################################################################
process.load("SLHCUpgradeSimulations.L1Trigger.ClusteringAlgorithmRegister_cfi")
process.load("SLHCUpgradeSimulations.L1Trigger.HitMatchingAlgorithmRegister_cfi")

process.ClusteringAlgorithm_PixelDigi_ = cms.ESPrefer("ClusteringAlgorithm_2d_PixelDigi_")
process.HitMatchingAlgorithm_PixelDigi_ = cms.ESPrefer("HitMatchingAlgorithm_window_PixelDigi_")

process.load("SLHCUpgradeSimulations.L1Trigger.LocalStub_cfi")
process.load("SLHCUpgradeSimulations.L1Trigger.GlobalStub_cfi")
process.load("SLHCUpgradeSimulations.L1Trigger.Tracklet_cfi")
process.load("SLHCUpgradeSimulations.L1Trigger.TrackletChain_cfi")

process.HitMatchingAlgorithm_window_PixelDigi_.minPtThreshold = cms.double(TT_threshold)
process.TrackletsFromPixelDigis.minPtThreshold = cms.double(TT_threshold)
process.TrackletChainsFromPixelDigis.minPtThreshold = cms.double(TT_threshold)


#######################################################################################################################
#######################################################################################################################



process.options = cms.untracked.PSet( Rethrow = cms.untracked.vstring('ProductNotFound') )
process.TFileService = cms.Service("TFileService", fileName = cms.string('TrackTriggerPlusCaloTrigger_onthefly.root'), closeFileFast = cms.untracked.bool(True))
process.demo = cms.EDAnalyzer("TrackTriggerPlusCaloTrigger")
process.p = cms.Path(process.demo)


# To write out events (not need: FastSimulation _is_ fast!)
process.o1 = cms.OutputModule(
    "PoolOutputModule",
   outputCommands = cms.untracked.vstring('keep *',
                                           'drop *_siTrackerGaussianSmearingRecHits_*_*',
                                           'drop *_caloRecHits_*_*',
                                           'drop *_*calTriggerPrimitiveDigis_*_*',
                                           'drop PCaloHits_*_*_*',
                                           'drop PSimHits_MuonSimHits_*_*',
                                           'drop SiStrip*_*_*_*',
                                           'drop StripDigi*_*_*_*',
                                           'drop *_mix_*_*'),
    fileName = cms.untracked.string('TrackTriggerPlusCaloTrigger.root')
)
process.outpath = cms.EndPath(process.o1)


# Keep output to a nice level
#process.Timing =  cms.Service("Timing")
#process.load("FWCore/MessageService/MessageLogger_cfi")
#process.MessageLogger.destinations = cms.untracked.vstring("detailedInfo.txt")

# Make the job crash in case of missing product
process.options = cms.untracked.PSet( Rethrow = cms.untracked.vstring('ProductNotFound') )

# Famos with tracks
process.p1 = cms.Path(process.famosSimulationSequence) #simulationWithFamos)
process.p2 = cms.Path(process.trDigi*process.caloRecHits)
process.p6 = cms.Path(process.LocalStubsFromPixelDigis*process.GlobalStubsFromPixelDigis*process.TrackletsFromPixelDigis*process.TrackletChainsFromPixelDigis)
process.p7 = cms.Path(process.simEcalTriggerPrimitiveDigis*process.simHcalTriggerPrimitiveDigis*process.L1SLHCEmulator)

process.schedule = cms.Schedule(process.p1 , process.p2 , process.p6 , process.p7 , process.p ,process.outpath)
#,process.p3,process.p4
