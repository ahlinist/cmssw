import FWCore.ParameterSet.Config as cms


################################################################################
# the simulation parameters...

#### Generation job configure:
GunPartID = [13] ### [211,11,-13] 
PT = "50"
GunMinPt = 50 #PT
GunMaxPt = 50 #PT
GunMinEta = -1.5 # default -3.
GunMaxEta = 1.5 # default 3.
GunAddAntiParticle = False # back-to-back particles

########################
NumberOfEvents = 250
########################

### output dir
#dir = "/raid4/zotto/SLHCprod/prod226/"
dir = "../../../../outputs"

### "" to get only selected issues
ThingsToKeep = cms.untracked.vstring(
       'drop *',
       'keep DT*_muon*_*_*',
       'keep Global*_muon*_*_*',
       #'keep L1Mu*_*_*_*',
       'keep L1Mu*_sim*_*_*',
       'keep *_g4SimHits_*_*',
       'drop PCaloHits_g4SimHits_*_*',
       'keep *_simSiPixelDigis_*_*',
       'keep *_GlobalStubs*_*_*',
       'keep *_LocalStubs*_*_*',
       'keep *_Tracklets*_*_*',
       'keep *_TriggerResults_*_*',
       'keep *_source_*_*',
       'keep *_genParticles_*_*'
       )

### "" to get all
#ThingsToKeep = cms.untracked.vstring('keep *')

#Stubs = "fromSimHits"
Stubs = "fromPixelDigis"
#Stubs = "fromTrackTriggerHits"

outRootFileName =\
dir + PT + "GeVPtMuons_StackedTrackerlongbarrel_" + Stubs + ".root"

outInfoFilename = dir + Stubs + "_" + PT + "GeVPtMuons_detailedInfo.txt"

PileUp = 0

#### Tracker geometry
SpecificGeometry = "longbarrel"

includelowptjunk = True 
#if includelowptjunk:
#	cfgNote = "remove pt cut and add loopers"
#else:
#	cfgNote = "include pt cut and exclude loopers"


# just one way of doing this:
# first column is the choice of Clustering algorithm,
# second column is choice of Hit-Matching algorithm
# Clustering algorithms choices are:
#     *    "a" = no clustering, all hits accepted as clusters of 1
#     *    "broadside" = 1d clustering of broadside neighbours
#     *    "2d" = 2d clustering
# Hit matching algorithms choices are:
#     *    "a" = all possible combinations of hits in stack accepted.
#          !!!Warning! With any pileup this is SLOOOOOOOOOOOW due to large combinatorials!!!
#     *    "globalgeometry" = standard matching in the global geometry frame
#     *    "window" = matching by delta-row, delta-column including calculation of the windows

StubsFromSimHits = [ "a" , "globalgeometry" ]
StubsFromDigis = [ "a" , "globalgeometry" ]
StubsFromTTHits = [ "a" , "globalgeometry" ]

################################################################################

process = cms.Process("GenFullSimDigi")

# Number of events to be generated
process.maxEvents = cms.untracked.PSet(
	input = cms.untracked.int32(NumberOfEvents)
	)

# general services, as Random Number Generator Service -------------------------
process.load("Configuration.StandardSequences.Services_cff")

# global initialization of geometry --------------------------------------------
process.load("Configuration.StandardSequences.Geometry_cff")

### replace with strawmanB geometry ############################################
process.load("SLHCUpgradeSimulations.Geometry." +
            SpecificGeometry +
            "_cmsIdealGeometryXML_cff")

# magnetic field ---------------------------------------------------------------
#process.load("Configuration.StandardSequences.MagneticField_cff")
# Parametrized magnetic field (new mapping, 4.0 and 3.8T)
#process.load("Configuration.StandardSequences.MagneticField_40T_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
# NB: Mag Field can be set to 0
#process.load("Configuration.StandardSequences.UseZeroBField_cff")

#-------------------------------------------------------------------------------
### conditions that are needed for digitization and higher levels
process.load("Configuration.StandardSequences.FakeConditions_cff")
process.SiPixelFakeGainOfflineESSource.file =\
      'SLHCUpgradeSimulations/Geometry/data/longbarrel/PixelSkimmedGeometry_empty.txt'
process.SiPixelFakeLorentzAngleESSource.file =\
      'SLHCUpgradeSimulations/Geometry/data/longbarrel/PixelSkimmedGeometry.txt'

#-------------------------------------------------------------------------------
# this config frament brings you the generator information
process.load("Configuration.StandardSequences.Generator_cff")

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

process.source = cms.Source(
   "FlatRandomPtGunSource",
   firstRun = cms.untracked.uint32(1),
   PGunParameters = cms.untracked.PSet(
       PartID = cms.untracked.vint32(GunPartID),
       # you can request more than 1 particle
       # PartID = cms.untracked.vint32(211,11,-13),
       MinPt = cms.untracked.double(GunMinPt),
       MaxPt = cms.untracked.double(GunMaxPt),
       MinEta = cms.untracked.double(GunMinEta),
       MaxEta = cms.untracked.double(GunMaxEta),
       MinPhi = cms.untracked.double(-3.14159265359), ## it must be in radians
       MaxPhi = cms.untracked.double(3.14159265359),
       ),
   AddAntiParticle = cms.untracked.bool(GunAddAntiParticle), # back-to-back 
   Verbosity = cms.untracked.int32(0) ## for printouts, set it to 1 (or greater)   
   )

#-------------------------------------------------------------------------------
# this config frament brings 2 steps of the detector simulation:
# -- G4-based hit level detector simulation
# -- digitization (electronics readout modeling)
# it returns 2 sequences : 
# -- psim  (G4 sim)
# -- pdigi (digitization in all subsystems)
process.load("Configuration.StandardSequences.Simulation_cff")
process.g4SimHits.G4TrackingManagerVerbosity = 2

# If you want to turn on/off pile-up -------------------------------------------
# In order to operate Digis, one needs to include Mixing module 
# (pileup modeling), at least in the 0-pileup mode
#
# There're 3 possible configurations of the Mixing module :
# no-pileup, low luminosity pileup, and high luminosity pileup
#
# they come, respectively, through the 3 config fragments below
#
# *each* config returns label "mix"; thus you canNOT have them
# all together in the same configuration, but only one !!!
process.load("Configuration.StandardSequences.MixingNoPileUp_cff")
#process.load("Configuration.StandardSequences.MixingLowLumiPileUp_cff")
#process.load("Configuration.StandardSequences.MixingHighLumiPileUp_cff")


#-------------------------------------------------------------------------------
### look at L1Trigger/Configuration/python/SimL1Emulator_cff.py:
process.load("Configuration.StandardSequences.L1Emulator_cff")
#process.simDttfDigis.Debug = cms.untracked.int32(2)
process.simDttfDigis.CSCStub_Source = 'simCsctfTrackDigis'
process.simDttfDigis.DTDigi_Source = 'simDtTriggerPrimitiveDigis'

process.load("Configuration.StandardSequences.DigiToRaw_cff")

process.load("Configuration.StandardSequences.RawToDigi_cff")

# Vertex smearing is invoked here, needs an explicit inclusion
#process.load("Configuration.StandardSequences.VtxSmearedBetafuncEarlyCollision_cff")
process.load("Configuration.StandardSequences.VtxSmearedGauss_cff")

#-------------------------------------------------------------------------------
process.load("SimTracker.Configuration.SimTracker_cff")
process.simSiPixelDigis.MissCalibrate = False
process.simSiPixelDigis.AddPixelInefficiency = -1
process.simSiPixelDigis.LorentzAngle_DB = False
process.simSiPixelDigis.killModules = False
process.simSiPixelDigis.NumPixelBarrel = cms.int32(14)
process.simSiPixelDigis.NumPixelEndcap = cms.int32(3)

process.load("Configuration.StandardSequences.Reconstruction_cff")
process.siPixelClusters.src = 'simSiPixelDigis'
process.siPixelClusters.MissCalibrate = False
process.siStripZeroSuppression.RawDigiProducersList[0].RawDigiProducer = 'simSiStripDigis'
process.siStripZeroSuppression.RawDigiProducersList[1].RawDigiProducer = 'simSiStripDigis'
process.siStripZeroSuppression.RawDigiProducersList[2].RawDigiProducer = 'simSiStripDigis'
process.siStripClusters.DigiProducersList[0].DigiProducer= 'simSiStripDigis'


# Stacked Tracker Geometry -----------------------------------------------------
process.load("SLHCUpgradeSimulations.Utilities.StackedTrackerGeometry_cfi")
process.StackedTrackerGeometryESModule.make_debug_file = True

process.load("SLHCUpgradeSimulations.L1Trigger.ClusteringAlgorithmRegister_cfi")
process.load("SLHCUpgradeSimulations.L1Trigger.HitMatchingAlgorithmRegister_cfi")

process.ClusteringAlgorithm_PSimHit_ = cms.ESPrefer("ClusteringAlgorithm_" +
                                                   StubsFromSimHits[0] +
                                                   "_PSimHit_")
process.ClusteringAlgorithm_PixelDigi_ = cms.ESPrefer("ClusteringAlgorithm_" +
                                                     StubsFromDigis[0] +
                                                     "_PixelDigi_")
process.ClusteringAlgorithm_TTHit_ = cms.ESPrefer("ClusteringAlgorithm_" +
                                                 StubsFromTTHits[0] +
                                                 "_TTHit_")
process.HitMatchingAlgorithm_PSimHit_ = cms.ESPrefer("HitMatchingAlgorithm_" +
                                                    StubsFromSimHits[1] +
                                                    "_PSimHit_")
process.HitMatchingAlgorithm_PixelDigi_ = cms.ESPrefer("HitMatchingAlgorithm_" +
                                                      StubsFromDigis[1] +
                                                      "_PixelDigi_")
process.HitMatchingAlgorithm_TTHit_ = cms.ESPrefer("HitMatchingAlgorithm_" +
                                                  StubsFromTTHits[1] +
                                                  "_TTHit_")
process.load("SLHCUpgradeSimulations.L1Trigger.LocalStub_cfi")
process.LocalStubsFromSimHits.rawHits =\
                            cms.VInputTag(cms.InputTag("g4SimHits","TrackerHits"))
process.load("SLHCUpgradeSimulations.L1Trigger.GlobalStub_cfi")
process.load("SLHCUpgradeSimulations.L1Trigger.Tracklet_cfi")

process.load("SLHCUpgradeSimulations.L1Trigger.TrackTriggerHitsFromPixelDigis_cfi")

#-------------------------------------------------------------------------------
# Event output
process.load("Configuration.EventContent.EventContent_cff")


#--- Keep output to a nice level -----------------------------------------------
#process.Timing =  cms.Service("Timing")
process.load("FWCore/MessageService/MessageLogger_cfi")
process.MessageLogger.destinations = cms.untracked.vstring(outInfoFilename)

# Make the job crash in case of missing product
#process.options = cms.untracked.PSet( Rethrow = cms.untracked.vstring('ProductNotFound') )

#process.demo = cms.EDFilter("______This_Is_Where_An_Analysis_Goes______")


#-------------------------------------------------------------------------------
process.p0 = cms.Path(process.pgen)
process.p1 = cms.Path(process.psim)
process.p2 = cms.Path(process.pdigi)
process.p3 = cms.Path(process.L1Emulator)
process.p4 = cms.Path(process.DigiToRaw)
process.p5 = cms.Path(process.RawToDigi)

process.trackerlocalreco = cms.Path(process.trackerlocalreco)
process.muonreco         = cms.Path(process.muonreco)
process.reco             = cms.Path(process.reconstruction)


if Stubs == "fromSimHits": 
   process.stubs = cms.Path(process.LocalStubsFromSimHits *
                            process.GlobalStubsFromSimHits*
                            process.TrackletsFromSimHits)          
# produces error:
#---- ProductNotFound BEGIN
#getByLabel: Found zero products matching all criteria
#Looking for type: std::vector<PSimHit>
#Looking for module label: g4SimHits
#Looking for productInstanceName: TrackerHits
#cms::Exception going through module LocalStubBuilder_PSimHit_/LocalStubsFromSimHits
#---- ProductNotFound END

elif Stubs == "fromPixelDigis":
   process.stubs = cms.Path(process.LocalStubsFromPixelDigis *
                            process.GlobalStubsFromPixelDigis*
                            process.TrackletsFromPixelDigis)  

else:
   process.stubs = cms.Path(process.TrackTriggerHitsFromPixelDigis *
                            process.LocalStubsFromTrackTriggerHits *
                            process.GlobalStubsFromTrackTriggerHits*
                            process.TrackletsFromTrackTriggerHits) 


#-------------------------------------------------------------------------------
# To write out events 
process.o1 = cms.OutputModule(
   "PoolOutputModule",
   outputCommands = ThingsToKeep,
   fileName = cms.untracked.string(outRootFileName)
   )

process.outpath = cms.EndPath(process.o1)

process.schedule = cms.Schedule(process.p0,
                               process.p1,
                               process.p2,
                               process.p3,
                               process.p4,
                               process.p5,
                               process.trackerlocalreco,
                               #process.muonreco,
                               process.stubs,
                               process.outpath)

