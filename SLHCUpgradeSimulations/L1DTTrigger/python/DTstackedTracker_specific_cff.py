import FWCore.ParameterSet.Config as cms

Geometry = "longbarrel"

#Set the nominal threshold for TT stubs and tracklets
TT_threshold = 2

includelowptjunk = True

#if includelowptjunk:
#       cfgNote = "remove pt cut and add loopers"
#else:
#       cfgNote = "include pt cut and exclude loopers"


# general services, as Random Number Generator Service -------------------------
from Configuration.StandardSequences.Services_cff import *

# global initialization of geometry --------------------------------------------
from Configuration.StandardSequences.Geometry_cff import *

# magnetic field ---------------------------------------------------------------
#process.load("Configuration.StandardSequences.MagneticField_cff")
# Parametrized magnetic field (new mapping, 4.0 and 3.8T)
#process.load("Configuration.StandardSequences.MagneticField_40T_cff")
from Configuration.StandardSequences.MagneticField_38T_cff import *
# NB: Mag Field can be set to 0
#process.load("Configuration.StandardSequences.UseZeroBField_cff")

#-------------------------------------------------------------------------------
### conditions that are needed for digitization and higher levels
# 2.2.6: from Configuration.StandardSequences.FakeConditions_cff import *
### I commented out the next to run without access to the internet (Ignazio):
### I can't catch any effective implication over he output.
#from Configuration.StandardSequences.FrontierConditions_GlobalTag_cff import *
#GlobalTag.globaltag = 'MC_31X_V8::All'
#from Configuration.StandardSequences.GeometryPilot2_cff import *

# sim tracker digis ------------------------------------------------------------
from SimTracker.Configuration.SimTracker_cff import *
simSiPixelDigis.MissCalibrate = False
simSiPixelDigis.AddPixelInefficiency = -1
simSiPixelDigis.LorentzAngle_DB = False
simSiPixelDigis.killModules = False
simSiPixelDigis.NumPixelBarrel = cms.int32(10)
simSiPixelDigis.NumPixelEndcap = cms.int32(3)

from Configuration.StandardSequences.Reconstruction_cff import *
siPixelClusters.src = 'simSiPixelDigis'
siPixelClusters.MissCalibrate = False
siStripZeroSuppression.RawDigiProducersList[0].RawDigiProducer = 'simSiStripDigis'
siStripZeroSuppression.RawDigiProducersList[1].RawDigiProducer = 'simSiStripDigis'
siStripZeroSuppression.RawDigiProducersList[2].RawDigiProducer = 'simSiStripDigis'
siStripClusters.DigiProducersList[0].DigiProducer= 'simSiStripDigis'

TrackerDigiGeometryESModule.applyAlignment = False

# Stacked Tracker Geometry -----------------------------------------------------
from SLHCUpgradeSimulations.Geometry.longbarrel_cmsIdealGeometryXML_cff import *
from SLHCUpgradeSimulations.Utilities.StackedTrackerGeometry_cfi import *
StackedTrackerGeometryESModule.make_debug_file = cms.bool(True)

# just one way of doing this:
# first column is the choice of Clustering algorithm,
# second column is choice of Hit-Matching algorithm
# Clustering algorithms choices are:
#     *    "a" = no clustering, all hits accepted as clusters of 1
#     *    "broadside" = 1d clustering of broadside neighbours
#     *    "2d" = 2d clustering
# Hit matching algorithms choices are:
#     *    "a" = all possible combinations of hits in stack accepted.
#          !!!Warning! With pileup this is SLOW due to large combinatorials!!!
#     *    "globalgeometry" = standard matching in the global geometry frame
#     *    "window" = matching by delta-row, delta-column; compute the windows

ClusterStubsFromSimHits = [ "a", "broadside", "2d" ]
ClusterStubsFromDigis =   [ "a", "broadside", "2d" , "neighbor" ]
ClusterStubsFromTTHits =  [ "a", "broadside", "2d" , "neighbor" ]
HitStubsFromSimHits = [ "a", "globalgeometry" ]
HitStubsFromDigis = ["a", "globalgeometry", "pixelray" , "window", "thresholds"]
HitStubsFromTTHits =["a", "globalgeometry", "pixelray" , "window", "thresholds"]


from SLHCUpgradeSimulations.L1TrackTrigger.ClusteringAlgorithmRegister_cfi import *
from SLHCUpgradeSimulations.L1TrackTrigger.HitMatchingAlgorithmRegister_cfi import *

ClusteringAlgorithm_PixelDigi_ = cms.ESPrefer("ClusteringAlgorithm_" +
                                              ClusterStubsFromDigis[2] +
                                              "_PixelDigi_")
HitMatchingAlgorithm_PixelDigi_ = cms.ESPrefer("HitMatchingAlgorithm_" +
                                               HitStubsFromDigis[3] +
                                               "_PixelDigi_")

from SLHCUpgradeSimulations.L1TrackTrigger.LocalStub_cfi import *
LocalStubsFromSimHits.rawHits =\
                        cms.VInputTag(cms.InputTag("g4SimHits","TrackerHits"))
from SLHCUpgradeSimulations.L1TrackTrigger.GlobalStub_cfi import *
from SLHCUpgradeSimulations.L1TrackTrigger.Tracklet_cfi import *
from SLHCUpgradeSimulations.L1TrackTrigger.TrackletChain_cfi import *

#-------------------------------------------------------------------------------
from SLHCUpgradeSimulations.L1TrackTrigger.TrackTriggerHitsFromPixelDigis_cfi import *


stubs_fromSimHits = cms.Sequence(LocalStubsFromSimHits *
                                 GlobalStubsFromSimHits*
                                 TrackletsFromSimHits)

stubs_fromPixelDigis = cms.Sequence(LocalStubsFromPixelDigis *
                                    GlobalStubsFromPixelDigis*
                                    TrackletsFromPixelDigis  *
                                    TrackletChainsFromPixelDigis) 

stubs_fromTrackTriggerHits = cms.Sequence(TrackTriggerHitsFromPixelDigis *
                                          LocalStubsFromTrackTriggerHits *
                                          GlobalStubsFromTrackTriggerHits*
                                          TrackletsFromTrackTriggerHits)
