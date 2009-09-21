import FWCore.ParameterSet.Config as cms

includelowptjunk = True
#if includelowptjunk:
#       cfgNote = "remove pt cut and add loopers"
#else:
#       cfgNote = "include pt cut and exclude loopers"


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

# general services, as Random Number Generator Service -------------------------
from Configuration.StandardSequences.Services_cff import *

# global initialization of geometry --------------------------------------------
from Configuration.StandardSequences.Geometry_cff import *

### replace with strawmanB geometry ############################################
from SLHCUpgradeSimulations.Geometry.strawmanb_cmsIdealGeometryXML_cff import *

# magnetic field ---------------------------------------------------------------
#process.load("Configuration.StandardSequences.MagneticField_cff")
# Parametrized magnetic field (new mapping, 4.0 and 3.8T)
#process.load("Configuration.StandardSequences.MagneticField_40T_cff")
from Configuration.StandardSequences.MagneticField_38T_cff import *
# NB: Mag Field can be set to 0
#process.load("Configuration.StandardSequences.UseZeroBField_cff")

#-------------------------------------------------------------------------------
### conditions that are needed for digitization and higher levels
from Configuration.StandardSequences.FakeConditions_cff import *
SiPixelFakeGainOfflineESSource.file =\
       'SLHCUpgradeSimulations/Geometry/data/strawmanb/PixelSkimmedGeometry.txt'
SiPixelFakeLorentzAngleESSource.file =\
       'SLHCUpgradeSimulations/Geometry/data/strawmanb/PixelSkimmedGeometry.txt'

#-------------------------------------------------------------------------------
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

# Stacked Tracker Geometry -----------------------------------------------------
from SLHCUpgradeSimulations.Geometry.strawmanb_cmsIdealGeometryXML_cff import *
from SLHCUpgradeSimulations.Utilities.StackedTrackerGeometry_cfi import *
StackedTrackerGeometryESModule.make_debug_file = True

StubsFromSimHits = [ "a" , "globalgeometry" ]
StubsFromDigis = [ "a" , "globalgeometry" ]
StubsFromTTHits = [ "a" , "globalgeometry" ]

from SLHCUpgradeSimulations.L1Trigger.ClusteringAlgorithmRegister_cfi import *
from SLHCUpgradeSimulations.L1Trigger.HitMatchingAlgorithmRegister_cfi import *

ClusteringAlgorithm_PSimHit_ = cms.ESPrefer("ClusteringAlgorithm_" +
                                            StubsFromSimHits[0] +
                                            "_PSimHit_")
ClusteringAlgorithm_PixelDigi_ = cms.ESPrefer("ClusteringAlgorithm_" +
                                              StubsFromDigis[0] +
                                              "_PixelDigi_")
ClusteringAlgorithm_TTHit_ = cms.ESPrefer("ClusteringAlgorithm_" +
                                          StubsFromTTHits[0] +
                                          "_TTHit_")
HitMatchingAlgorithm_PSimHit_ = cms.ESPrefer("HitMatchingAlgorithm_" +
                                             StubsFromSimHits[1] +
                                             "_PSimHit_")
HitMatchingAlgorithm_PixelDigi_ = cms.ESPrefer("HitMatchingAlgorithm_" +
                                               StubsFromDigis[1] +
                                               "_PixelDigi_")
HitMatchingAlgorithm_TTHit_ = cms.ESPrefer("HitMatchingAlgorithm_" +
                                           StubsFromTTHits[1] +
                                           "_TTHit_")

from SLHCUpgradeSimulations.L1Trigger.LocalStub_cfi import *
LocalStubsFromSimHits.rawHits =\
                        cms.VInputTag(cms.InputTag("g4SimHits","TrackerHits"))
from SLHCUpgradeSimulations.L1Trigger.GlobalStub_cfi import *
from SLHCUpgradeSimulations.L1Trigger.Tracklet_cfi import *

#-------------------------------------------------------------------------------

from SLHCUpgradeSimulations.L1Trigger.TrackTriggerHitsFromPixelDigis_cfi import *


stubs_fromSimHits = cms.Sequence(LocalStubsFromSimHits *
                                 GlobalStubsFromSimHits*
                                 TrackletsFromSimHits)

stubs_fromPixelDigis = cms.Sequence(LocalStubsFromPixelDigis *
                                    GlobalStubsFromPixelDigis*
                                    TrackletsFromPixelDigis)

stubs_fromTrackTriggerHits = cms.Sequence(TrackTriggerHitsFromPixelDigis *
                                          LocalStubsFromTrackTriggerHits *
                                          GlobalStubsFromTrackTriggerHits*
                                          TrackletsFromTrackTriggerHits)
