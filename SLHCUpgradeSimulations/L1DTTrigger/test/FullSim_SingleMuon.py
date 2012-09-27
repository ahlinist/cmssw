import FWCore.ParameterSet.Config as cms
import os

process = cms.Process("Fullsim")

# Stack separation in mm
sep = '1'
# Number of PU events
npu = '0'
# Number of events to process
nevents = 10000;
# PID of gun particle
pid = 13;
# NOTE there are 4 fired each time
# AntiParticle is set this way:
addanti = cms.bool(False)
# Pt and Eta range of gun particle
plow  =  PTMU
phigh =  plow
elow  = -1.
ehigh =  1.
# Define Algorithms:   Clusters,  Stubs,            Tracklets,         Tracks
ObjectsFromSimHits = [ "2d" ,     "globalgeometry", "globalgeometry" , "bpphel" ]
ObjectsFromDigis   = [ "2d" ,     "pixelray",       "globalgeometry" , "bpphel" ]
# Set Stub production pt threshold
thresh = 2.0
# Set Tracklet flags
trackletonlyherm = cms.bool(True) # True creates only hermetic-FNAL style Tracklets
tracklethelixfit = cms.bool(True) # True fits the Tracklet with helicoidal style in R-Z, creating dedicated collection
trackletbeamspot = cms.bool(True) # True corrects the Tracklet fit for the beam position, creating dedicated collection
# Set Track flags
trackletseedtype = "ShortTrackletsVtxBSHelFit" # Recommended because the only algorithm available is bpphel!
# NOTE be self consistent with available seeds
# and with available fit strategies
trackhelixfit = cms.bool(True) # True applies helicoidal fit to Tracks
trackalsoseed = cms.bool(True) # True includes seed vertex within points used for the fit
tracknoduplic = cms.bool(False) # True removes duplicate candidate Tracks
# NOTE only one collection is created by the L1TkTrackBuilder
trackdsvector = cms.vuint32(  ) # empty to allow all of them



index = 1
puindex = 111
if os.getenv('pgun_index'):
   index = str(os.getenv('pgun_index'))
   puindex = int(os.getenv('pgun_index'))

theSeedValue = 12345
if os.getenv('pgun_index'):
   theSeedValue = int(12345+1000*int(os.getenv('pgun_index'))*int(os.getenv('pgun_index')))
print 'Seed: '+str(theSeedValue)


# Build output file name
guntypename = 'thisisastring'
if pid == 13:
	guntypename = 'MuN'
elif pid == -13:
	guntypename = 'MuP'
if addanti == cms.bool(True):
	guntypename = 'Mu2'
	
# out_filename = 'SingleMuonPt_50_FullSim.root'


################################################################################

# import of standard configurations ---------------------------------------------

process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')

process.load("Configuration.StandardSequences.Generator_cff")
process.load("Configuration.StandardSequences.Simulation_cff")
process.load("Configuration.StandardSequences.Services_cff")


process.load('SLHCUpgradeSimulations.Geometry.LongBarrel_cmsSimIdealGeometryXML_cff')
process.load("SLHCUpgradeSimulations.Utilities.StackedTrackerGeometry_cfi")
process.load('SLHCUpgradeSimulations.Geometry.fakeConditions_Longbarrel_cff')
process.load("SLHCUpgradeSimulations.Geometry.recoFromSimDigis_Longbarrel_cff")
process.load("SLHCUpgradeSimulations.Geometry.upgradeTracking_longbarrel_cff")

## choose magnetic field
#process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load("Configuration.StandardSequences.MagneticField_40T_cff")

process.load('Configuration/StandardSequences/Sim_cff')
process.load('Configuration/StandardSequences/Digi_cff')
process.load('Configuration/StandardSequences/L1Emulator_cff')
process.load('Configuration/StandardSequences/DigiToRaw_cff')
process.load('Configuration/StandardSequences/EndOfProcess_cff')

process.VolumeBasedMagneticFieldESProducer.useParametrizedTrackerField = True
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
# process.GlobalTag.globaltag = "DESIGN42_V11::All" # Wrong DT l1 configuration
process.GlobalTag.globaltag = "DESIGN43_V0::All"
process.TrackerDigiGeometryESModule.applyAlignment = False

## choose random number generators and seeds
process.load('IOMC/RandomEngine/IOMC_cff')
#process.RandomNumberGeneratorService.generator.engineName = cms.untracked.string('TRandom3')
#process.RandomNumberGeneratorService.VtxSmeared.engineName = cms.untracked.string('TRandom3')
#process.RandomNumberGeneratorService.g4SimHits.engineName = cms.untracked.string('TRandom3')
#process.RandomNumberGeneratorService.mix.engineName = cms.untracked.string('TRandom3')
#process.RandomNumberGeneratorService.LHCTransport.engineName = cms.untracked.string('TRandom3')
#process.RandomNumberGeneratorService.simSiPixelDigis.engineName = cms.untracked.string('TRandom3')

process.RandomNumberGeneratorService.generator.initialSeed = theSeedValue+111211
process.RandomNumberGeneratorService.VtxSmeared.initialSeed=theSeedValue+22222
process.RandomNumberGeneratorService.g4SimHits.initialSeed=theSeedValue+33333
process.RandomNumberGeneratorService.mix.initialSeed=theSeedValue+33333
process.RandomNumberGeneratorService.LHCTransport.initialSeed=theSeedValue+33333
process.RandomNumberGeneratorService.simSiPixelDigis.initialSeed=theSeedValue+33333

from IOMC.RandomEngine.RandomServiceHelper import  RandomNumberServiceHelper
randHelper = RandomNumberServiceHelper(process.RandomNumberGeneratorService)
randHelper.populate()

# muon specific ----------------------------------------------------------------
process.load("Geometry.DTGeometry.dtGeometry_cfi")
process.load("Geometry.MuonNumbering.muonNumberingInitialization_cfi")
process.load("SimMuon/DTDigitizer/muonDTDigis_cfi")

# tracker specific -------------------------------------------------------------
# replace with long barrel geometry
#MOVED UPSTAIRS sep = '1'
if os.getenv('pgun_sep'):
   sep= str(os.getenv('pgun_sep'))
print 'Stack Separation: '+sep

process.siPixelFakeGainOfflineESSource = cms.ESSource("SiPixelFakeGainOfflineESSource",
        file = cms.FileInPath('SLHCUpgradeSimulations/Geometry/data/longbarrel/PixelSkimmedGeometry_empty.txt'))
process.es_prefer_fake_gain = cms.ESPrefer("SiPixelFakeGainOfflineESSource","siPixelFakeGainOfflineESSource")
process.siPixelFakeLorentzAngleESSource = cms.ESSource("SiPixelFakeLorentzAngleESSource",
        file = cms.FileInPath('SLHCUpgradeSimulations/Geometry/data/longbarrel/PixelSkimmedGeometry.txt'))
process.es_prefer_fake_lorentz = cms.ESPrefer("SiPixelFakeLorentzAngleESSource","siPixelFakeLorentzAngleESSource")

#MOVED UPSTAIRS npu = '0'
if os.getenv('pgun_npu'):
   npu= str(os.getenv('pgun_npu'))
print 'Number of PU events: '+npu

################################################################################
# MixingModule config
#   load properly pre-preprocessed minbias for pileup
#   use only relevant simhit collections for mixing
#   do not mix simtracks or simvertices so far

# minbias files for pileup simulated in 1/2 of detector

files_MB4PU_2pi = cms.untracked.vstring(
    'file:/lustre/cmswork/zotto/SLHCdata/FullSim_MB.root',
    'file:/lustre/cmswork/zotto/SLHCdata/FullSim_MB_1.root',
    'file:/lustre/cmswork/zotto/SLHCdata/FullSim_MB_2.root',
    'file:/lustre/cmswork/zotto/SLHCdata/FullSim_MB_3.root',
    'file:/lustre/cmswork/zotto/SLHCdata/FullSim_MB_4.root',
    'file:/lustre/cmswork/zotto/SLHCdata/FullSim_MB_5.root',
    'file:/lustre/cmswork/zotto/SLHCdata/FullSim_MB_6.root',
    'file:/lustre/cmswork/zotto/SLHCdata/FullSim_MB_9.root',
    'file:/lustre/cmswork/zotto/SLHCdata/FullSim_MB_17.root',
    'file:/lustre/cmswork/zotto/SLHCdata/FullSim_MB_18.root',
    'file:/lustre/cmswork/zotto/SLHCdata/FullSim_MB_19.root',
    'file:/lustre/cmswork/zotto/SLHCdata/FullSim_MB_20.root',
    'file:/lustre/cmswork/zotto/SLHCdata/FullSim_MB_21.root',
    'file:/lustre/cmswork/zotto/SLHCdata/FullSim_MB_22.root'
    )

# files_MB4PU is later feeded into mix.input.fileNames
files_MB4PU = files_MB4PU_2pi

from SimGeneral.MixingModule.mixObjects_cfi import *
process.mix = cms.EDProducer("MixingModule",
        LabelPlayback = cms.string(''),
        playback = cms.untracked.bool(False),
#        maxBunch = cms.int32(3),  ## in terms of 25 ns
#        minBunch = cms.int32(-5), ## in terms of 25 ns
        maxBunch = cms.int32(0),  ## turning off OOT pileup to compare with fastsim
        minBunch = cms.int32(0),                           
        bunchspace = cms.int32(25), ## ns
        mixProdStep1 = cms.bool(False),
        mixProdStep2 = cms.bool(False),
        useCurrentProcessOnly = cms.bool(False),
        input = cms.SecSource("PoolSource",
                    nbPileupEvents = cms.PSet( averageNumber = cms.double(npu) ),
                    seed = cms.int32(12345+1000*puindex),
                    type = cms.string('poisson'), # ('fixed'),
                    sequential = cms.untracked.bool(False),
#                    inputCommands = cms.untracked.vstring('keep *'),
#                    dropDescendantsOfDroppedBranches = cms.untracked.bool(False),
                    fileNames = files_MB4PU
        ),
        mixObjects = cms.PSet(mixCH = cms.PSet( mixCaloHits ),
                         mixTracks = cms.PSet( mixSimTracks ),
                         mixVertices = cms.PSet( mixSimVertices ),
                         mixSH = cms.PSet( mixSimHits ),
                         mixHepMC = cms.PSet( mixHepMCProducts )
        )
)

#process.load("Configuration.StandardSequences.L1Emulator_cff")
#process.load("Configuration.StandardSequences.DigiToRaw_cff")
#process.load("Configuration.StandardSequences.RawToDigi_cff")

# Re-set the beamspot scenario here! 
#process.load("Configuration.StandardSequences.VtxSmeared")
process.load("IOMC.EventVertexGenerators.VtxSmearedRealistic7TeV2011Collision_cfi")
process.load("Configuration.StandardSequences.Reconstruction_cff")

process.load("SimTracker.Configuration.SimTracker_cff")
#process.simSiPixelDigis.DeltaProductionCut = cms.double(100000); # Will turn off delta rays?

process.simSiPixelDigis.ROUList = cms.vstring(
    'g4SimHitsTrackerHitsPixelBarrelLowTof','g4SimHitsTrackerHitsPixelBarrelHighTof',
    'g4SimHitsTrackerHitsPixelEndcapLowTof','g4SimHitsTrackerHitsPixelEndcapHighTof')
process.simSiPixelDigis.MissCalibrate = False
process.simSiPixelDigis.LorentzAngle_DB = False
process.simSiPixelDigis.killModules = False
process.simSiPixelDigis.NumPixelBarrel = cms.int32(14)
process.simSiPixelDigis.NumPixelEndcap = cms.int32(3)
process.simSiPixelDigis.AddPixelInefficiency = -1
process.simSiStripDigis.ROUList = cms.vstring(
    'g4SimHitsTrackerHitsPixelBarrelLowTof','g4SimHitsTrackerHitsPixelBarrelHighTof',
    'g4SimHitsTrackerHitsPixelEndcapLowTof','g4SimHitsTrackerHitsPixelEndcapHighTof')
process.siPixelClusters.src = 'simSiPixelDigis'
process.siPixelClusters.MissCalibrate = False

# Event output
process.load("Configuration.EventContent.EventContent_cff")

#MOVEDUPSTAIRS nevents = 2000;
if os.getenv('pgun_nevents'):
   nevents = int(os.getenv('pgun_nevents'))
print 'Generating '+str(nevents)+' events.'
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(nevents)
)

# PID of gun particle
#MOVED UPSTAIRS pid = 13;
if os.getenv('pgun_pid'):
   pid = int(os.getenv('pgun_pid'))
print 'ID of gun particle: '+str(pid)

# Pt and Eta range of gun particle
#MOVED UPSTAIRS plow  =  3.00
#MOVED UPSTAIRS phigh =  3.00
#MOVED UPSTAIRS elow  = -2.5
#MOVED UPSTAIRS ehigh =  2.5
if os.getenv('pgun_plow'):
    plow = float(os.getenv('pgun_plow'))
print 'Minimum Momentum: '+str(plow)
if os.getenv('pgun_phigh'):
    phigh= float(os.getenv('pgun_phigh'))
print 'Maximum Momentum: '+str(phigh)
if os.getenv('pgun_elow'):
    elow = float(os.getenv('pgun_elow'))
print 'Minimum Eta: '+str(elow)
if os.getenv('pgun_ehigh'):
    ehigh= float(os.getenv('pgun_ehigh'))
print 'Maximum Eta: '+str(ehigh)

process.load("FastSimulation/Configuration/FlatPtMuonGun_cfi")
#process.generator.PGunParameters.PartID = [pid, pid, pid, pid]
process.generator.PGunParameters.PartID = [pid]
process.generator.PGunParameters.MinPt = plow
process.generator.PGunParameters.MaxPt = phigh
process.generator.PGunParameters.MinEta = elow
process.generator.PGunParameters.MaxEta = ehigh
process.generator.AddAntiParticle = addanti

#######################################################################################################################
## SLHC Track-trigger primitives
#######################################################################################################################

# HERE YOU CAN OVERRIDE DEFAULTS!

# Define Algorithms: Clusters, Stubs, Tracklets, Tracks
#MOVED UPSTAIRS ObjectsFromSimHits = [ "2d" , "globalgeometry", "globalgeometry" , "bpphel" ]
#MOVED UPSTAIRS ObjectsFromDigis   = [ "2d" , "pixelray",       "globalgeometry" , "bpphel" ]

process.load("SLHCUpgradeSimulations.L1TrackTrigger.ClusteringAlgorithmRegister_cfi")
process.load("SLHCUpgradeSimulations.L1TrackTrigger.HitMatchingAlgorithmRegister_cfi")
process.load("SLHCUpgradeSimulations.L1TrackTrigger.StubPairingAlgorithmRegister_cfi")
process.load("SLHCUpgradeSimulations.L1TrackTrigger.TrackingAlgorithmRegister_cfi")

process.ClusteringAlgorithm_PSimHit_   = cms.ESPrefer("ClusteringAlgorithm_"+ObjectsFromSimHits[0]+"_PSimHit_")
process.ClusteringAlgorithm_PixelDigi_ = cms.ESPrefer("ClusteringAlgorithm_"+ObjectsFromDigis[0]+"_PixelDigi_")

process.HitMatchingAlgorithm_PSimHit_   = cms.ESPrefer("HitMatchingAlgorithm_"+ObjectsFromSimHits[1]+"_PSimHit_")
process.HitMatchingAlgorithm_PixelDigi_ = cms.ESPrefer("HitMatchingAlgorithm_"+ObjectsFromDigis[1]+"_PixelDigi_")

process.StubPairingAlgorithm_PSimHit_   = cms.ESPrefer("StubPairingAlgorithm_"+ObjectsFromSimHits[2]+"_PSimHit_")
process.StubPairingAlgorithm_PixelDigi_ = cms.ESPrefer("StubPairingAlgorithm_"+ObjectsFromDigis[2]+"_PixelDigi_")

process.TrackingAlgorithm_PSimHit_   = cms.ESPrefer("TrackingAlgorithm_"+ObjectsFromSimHits[3]+"_PSimHit_")
process.TrackingAlgorithm_PixelDigi_ = cms.ESPrefer("TrackingAlgorithm_"+ObjectsFromDigis[3]+"_PixelDigi_")
#process.TrackingAlgorithm_PSimHit_.windowSize = cms.double(95.0)
#process.TrackingAlgorithm_PixelDigi_.windowSize = cms.double(95.0)


# Define input types
process.load("SLHCUpgradeSimulations.L1TrackTrigger.Beam_cfi")
process.load("SLHCUpgradeSimulations.L1TrackTrigger.Cluster_cfi")
process.load("SLHCUpgradeSimulations.L1TrackTrigger.Stub_cfi")
process.load("SLHCUpgradeSimulations.L1TrackTrigger.Tracklet_cfi")
process.load("SLHCUpgradeSimulations.L1TrackTrigger.Track_cfi")

#process.L1TkClustersFromSimHits.rawHits    = cms.VInputTag(cms.InputTag("g4SimHits","TrackerHits"))
#process.L1TkStubsFromSimHits.L1TkClusters    = cms.InputTag("L1TkClustersFromSimHits")
#process.L1TkTrackletsFromSimHits.L1TkStubs = cms.InputTag("L1TkStubsFromSimHits")
#process.L1TkTrackletsFromSimHits.CreateOnlyHermetic = cms.bool(true)
#process.L1TkTrackletsFromSimHits.DoHelixFit = cms.bool(true)
#process.L1TkTracksFromSimHits.L1TkStubsBricks = cms.InputTag("L1TkStubsFromSimHits")
#process.L1TkTracksFromSimHits.L1TkTrackletSeed = cmsInputTag("L1TkTrackletsFromSimHits","ShortTrackletsVtxBSHelFit")

process.L1TkClustersFromPixelDigis.rawHits = cms.VInputTag(cms.InputTag("simSiPixelDigis"))
process.L1TkStubsFromPixelDigis.L1TkClusters = cms.InputTag("L1TkClustersFromPixelDigis")
process.L1TkTrackletsFromPixelDigis.L1TkStubs = cms.InputTag("L1TkStubsFromPixelDigis")
process.L1TkTrackletsFromPixelDigis.CreateOnlyHermetic = trackletonlyherm
process.L1TkTrackletsFromPixelDigis.DoHelixFit = tracklethelixfit
process.L1TkTrackletsFromPixelDigis.UseBeamspotVertex = trackletbeamspot
process.L1TkTracksFromPixelDigis.L1TkStubsBricks = cms.InputTag("L1TkStubsFromPixelDigis")
# be selfconsistent with the use of HelixFit in both seed and track
process.L1TkTracksFromPixelDigis.L1TkTrackletSeed = cms.InputTag("L1TkTrackletsFromPixelDigis",trackletseedtype)
process.L1TkTracksFromPixelDigis.DoHelixFit = trackhelixfit
process.L1TkTracksFromPixelDigis.UseAlsoSeedVertex = trackalsoseed
process.L1TkTracksFromPixelDigis.SeedDoubleStacks = trackdsvector
process.L1TkTracksFromPixelDigis.RemoveDuplicates = tracknoduplic
   


# Set stub production pt threshold
#MOVED UPSTAIRS thresh = 2.0
if os.getenv('pgun_thresh'):
    thresh = float(os.getenv('pgun_thresh'))
print 'Using stub production threshold: '+str(thresh)+" GeV"
process.HitMatchingAlgorithm_globalgeometry_PSimHit_.minPtThreshold = thresh
process.HitMatchingAlgorithm_globalgeometry_PixelDigi_.minPtThreshold = thresh
process.HitMatchingAlgorithm_pixelray_PixelDigi_.minPtThreshold = thresh

#######################################################################################################################


#filename = 'FullSim_N'+str(nevents)+'_PID'+str(pid)+'_PT'+str(plow)+'_'+str(phigh)+'_ETA'+str(elow)+'_'+str(ehigh)+'_PU'+str(npu)+'_thresh'+str(thresh)+'gev_'+str(index)+'_PixelRay_50ns_NOOOT.root'
#process.TFileService = cms.Service("TFileService", fileName = cms.string('file:scratch/FullSim/'+filename), closeFileFast = cms.untracked.bool(True))
#process.TFileService = cms.Service("TFileService", fileName = cms.string(filename), closeFileFast = cms.untracked.bool(True))

process.load("SimTracker.TrackAssociation.TrackAssociatorByChi2_cfi")
process.load("SimTracker.TrackAssociation.TrackAssociatorByHits_cfi")

"""
process.load("Validation.RecoTrack.MultiTrackValidator_cff")
#process.multiTrackValidator.label = ['generalTracks']
### if using simple (non-iterative) or old (as in 1_8_4) tracking
process.multiTrackValidator.label = ['ctfWithMaterialTracks']
process.multiTrackValidator.associators = ['TrackAssociatorByHits']
process.multiTrackValidator.UseAssociators = True
process.multiTrackValidator.outputFile = "validfullLB_muon_50GeV.root"
process.multiTrackValidator.nint = cms.int32(20)
process.multiTrackValidator.nintpT = cms.int32(25)
process.multiTrackValidator.maxpT = cms.double(50.0)
"""

##### with John's changes ##############################
process.load("SLHCUpgradeSimulations.Geometry.oldTracking_wtriplets")
process.pixellayertriplets.layerList = cms.vstring('BPix1+BPix2+BPix3',
        'BPix1+BPix3+BPix4',
        'BPix2+BPix3+BPix4',
        'BPix1+BPix2+BPix4',
        'BPix1+BPix2+FPix1_pos',
        'BPix1+BPix2+FPix1_neg',
        'BPix1+FPix1_pos+FPix2_pos',
        'BPix1+FPix1_neg+FPix2_neg',
        'BPix1+FPix2_pos+FPix3_pos',
        'BPix1+FPix2_neg+FPix3_neg',
        'FPix1_pos+FPix2_pos+FPix3_pos',
        'FPix1_neg+FPix2_neg+FPix3_neg')


process.load("Validation.RecoTrack.cutsTPEffic_cfi")
process.load("Validation.RecoTrack.cutsTPFake_cfi")


# restrict vertex fining in trackingtruthprod to smaller volume (note: these numbers in mm)
process.mergedtruth.simHitLabel = 'g4SimHits'
process.mergedtruth.volumeRadius = cms.double(100.0)
process.mergedtruth.volumeZ = cms.double(900.0)
process.mergedtruth.discardOutVolume = cms.bool(True)
process.mergedtruth.simHitCollections = cms.PSet(
    pixel = cms.vstring('g4SimHitsTrackerHitsPixelBarrelLowTof',
                        'g4SimHitsTrackerHitsPixelBarrelHighTof',
                        'g4SimHitsTrackerHitsPixelEndcapLowTof',
                        'g4SimHitsTrackerHitsPixelEndcapHighTof')
    )


process.cutsTPEffic.ptMin = cms.double(1.8)
process.cutsTPEffic.minRapidity = cms.double(-20.0)
process.cutsTPEffic.maxRapidity = cms.double(20.0)
process.cutsTPEffic.signalOnly = cms.bool(False)
process.cutsTPEffic.stableOnly = cms.bool(False)
process.cutsTPEffic.chargedOnly = cms.bool(False)


process.cutsTPFake.ptMin = cms.double(1.8)
process.cutsTPFake.minRapidity = cms.double(-20.0)
process.cutsTPFake.maxRapidity = cms.double(20.0)
process.cutsTPFake.signalOnly = cms.bool(False)
process.cutsTPFake.stableOnly = cms.bool(False)
process.cutsTPFake.chargedOnly = cms.bool(False)
#process.cutsTPFake.tip = cms.double(10.0)
#process.cutsTPFake.lip = cms.double(90.0)
############ end John's changes ###########################

### make sure the correct (modified) error routine is used
process.siPixelRecHits.CPE = 'PixelCPEfromTrackAngle'
process.MeasurementTracker.PixelCPE = 'PixelCPEfromTrackAngle'
process.ttrhbwr.PixelCPE = 'PixelCPEfromTrackAngle'
process.mixedlayerpairs.BPix.TTRHBuilder = cms.string('WithTrackAngle')
process.mixedlayerpairs.FPix.TTRHBuilder = cms.string('WithTrackAngle')
process.pixellayertriplets.BPix.TTRHBuilder = cms.string('WithTrackAngle')
process.pixellayertriplets.FPix.TTRHBuilder = cms.string('WithTrackAngle')
process.ctfWithMaterialTracks.TTRHBuilder = cms.string('WithTrackAngle')
#next may not be needed
process.load("RecoTracker.TrackProducer.TrackRefitters_cff")
process.TrackRefitter.TTRHBuilder = cms.string('WithTrackAngle')

#next may not be needed
process.load("RecoTracker.SiTrackerMRHTools.SiTrackerMultiRecHitUpdator_cff")
process.siTrackerMultiRecHitUpdator.TTRHBuilder = cms.string('WithTrackAngle')

"""
#replace with correct component in cloned version (replace with original TTRH producer)
#process.preFilterFirstStepTracks.TTRHBuilder = cms.string('WithTrackAngle')
#process.secPixelRecHits.CPE = cms.string('PixelCPEfromTrackAngle')
process.seclayertriplets.BPix.TTRHBuilder = cms.string('WithTrackAngle')
process.seclayertriplets.FPix.TTRHBuilder = cms.string('WithTrackAngle')
process.secMeasurementTracker.PixelCPE = cms.string('PixelCPEfromTrackAngle')
process.secWithMaterialTracks.TTRHBuilder = cms.string('WithTrackAngle')
process.thlayerpairs.BPix.TTRHBuilder = cms.string('WithTrackAngle')
process.thlayerpairs.FPix.TTRHBuilder = cms.string('WithTrackAngle')
process.thMeasurementTracker.PixelCPE = cms.string('PixelCPEfromTrackAngle')
process.thWithMaterialTracks.TTRHBuilder = cms.string('WithTrackAngle')
"""

"""
process.load("FWCore/MessageService/MessageLogger_cfi")
process.MessageLogger.destinations = cms.untracked.vstring("detailedInfo_FullSim")
process.Timing =  cms.Service("Timing")
"""

#process.MyModule = cms.EDAnalyzer("TrackTriggerLocalFluctuations",
#process.MyModule = cms.EDAnalyzer("TrackTriggerPrimitives",
#                                  mcType = cms.untracked.uint32(1)
#                                  )

#output_dir = '/lustre/cmswork/zotto/SLHCdata/'
output_dir = '/lustre/cmsdata/zotto/SLHCdata/'
process.o1 = cms.OutputModule("PoolOutputModule",
# definition of output file (full path)

#   fileName = cms.untracked.string('file:./' + out_filename),
   
    fileName = cms.untracked.string('file:'+ output_dir + 'FullSim_Pt'+str(plow)+'_PU'+str(npu)+'.root'),
    #cms.untracked.string('FullSim_N'+str(nevents)+'_PID'+str(pid)+'_PT'+str(plow)+'_'+str(phigh)+'_ETA'+str(elow)+'_'+str(ehigh)+'_PU'+str(npu)+'_thresh'+str(thresh)+'GeV_'+str(index)+'_PixelRay_50ns_NOOOT.root'),
    
    outputCommands = cms.untracked.vstring('drop *',
###########################################################################################
#                                           'keep PSimHit*_*_*_*',
###########################################################################################
                                           'keep *_L1TkBeams_*_*',
                                           'keep *_L1TkClustersFromPixelDigis_*_*',
                                           'keep *_L1TkStubsFromPixelDigis_*_*',
                                           'keep *_L1TkTrackletsFromPixelDigis_*_*',
                                           'keep *_L1TkTracksFromPixelDigis_*_*',
###########################################################################################
                                           'keep *_L1TkClustersFromSimHits_*_*',
                                           'keep *_L1TkStubsFromSimHits_*_*',
                                           'keep *_L1TkTrackletsFromSimHits_*_*',
                                           'keep *_L1TkTracksFromSimHits_*_*',
###########################################################################################
                                           'keep DT*_muon*_*_*',
                                           'keep Global*_muon*_*_*',
                                           'keep L1Mu*_sim*_*_*',
                                           'keep DTLayer*_*_*_*',
###########################################################################################
#                                           'keep *_famosSimHits_*_*',
                                           'keep PSimHits_g4SimHits_*_*',
                                           'keep SimTracks_g4SimHits_*_*',
                                           'keep SimVertexs_g4SimHits_*_*',
#                                           'keep *_ctfWithMaterialTracks_*_*',
                                           'keep *_simSiPixelDigis_*_*',
#                                           'keep *_simSiStripDigis_*_*',
#                                           'keep *_siStripMatchedRecHits_*_*',
#                                           'keep *_siPixelRecHits_*_*',
#                                           'keep *_siPixelClusters_*_*',
#                                           'keep *_siStripClusters_*_*',
#                                           'keep *_siPixelDigis_*_*',
#                                           'keep *_siStripDigis_*_*',
#                                           'keep *_L1ExtraMaker_*_*',
                                           'keep *_genParticles_*_*', # montecarlo tree
#                                           'keep *_offlineBeamSpot_*_*',
                                          )
)
process.outpath = cms.EndPath(process.o1)
process.analyze = cms.EDAnalyzer('EventContentAnalyzer')

process.g4SimHits.Generator.MinEtaCut = -5.5
process.g4SimHits.Generator.MaxEtaCut = 5.5
process.g4SimHits.Generator.MinPCut = 0.

process.p0 = cms.Path(process.generator*process.VtxSmeared*process.genParticles)
process.p1 = cms.Path(process.psim)
process.p2 = cms.Path(process.randomEngineStateProducer*process.mix*process.simSiPixelDigis*process.simMuonDTDigis)
#process.p3 = cms.Path(process.siPixelClusters)
process.p3 = cms.Path(process.L1Emulator)
process.p4 = cms.Path(process.L1TkBeams*process.L1TkClustersFromPixelDigis*process.L1TkStubsFromPixelDigis*process.L1TkTrackletsFromPixelDigis*process.L1TkTracksFromPixelDigis)
process.p5 = cms.Path(process.analyze)
#process.schedule = cms.Schedule(process.p0,process.p1,process.p2,process.p3,process.p4,process.p5,process.outpath)
process.schedule = cms.Schedule(process.p0,process.p1,process.p2,process.p3,process.p4,process.outpath)


