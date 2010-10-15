# -*- coding: utf-8 -*-

#######################################################
# CONFIGURATION FILE TO ANALYSE OUTPUT OF SIMULATIONS #
#######################################################

#####################
# preliminary setup #
#####################
#
# definition of CMS interface to framework
import FWCore.ParameterSet.Config as cms               ### REMEMBER THIS DEFINITION

######################################################
######################################################
###                                                ###
###                                                ###
###    DEFINITION OF ANALYSIS PLUGIN PARAMETERS    ###
###                                                ###
###                                                ###
######################################################
######################################################

########################################
# which data are going to be analysed? #
########################################
#
# definition of process
# in this case, Physics Analysis Toolkit template
process = cms.Process("PATtemplate")
#
# source files to feed the analyser
process.source = cms.Source(
  "PoolSource",
  fileNames = cms.untracked.vstring(
    'file:~/scratch0/CMSSW_3_3_6/src/SLHCUpgradeSimulations/L1Trigger/test/muonPair_Longbarrel_2dClust_PixelrayMatch_NoPU_L1TrkTrig.root',
  )
)
#
###################################
##################
# geometry setup #
##################
#
# magnetic field
process.load("Configuration.StandardSequences.MagneticField_40T_cff")
process.VolumeBasedMagneticFieldESProducer.useParametrizedTrackerField = True
process.TrackerDigiGeometryESModule = cms.ESProducer("TrackerDigiGeometryESModule",
    fromDDD = cms.bool(True),
    applyAlignment = cms.bool(False),
    alignmentsLabel = cms.string(''),
    appendToDataLabel = cms.string('')
)
#
# longbarrel geometry
from Geometry.CMSCommonData.cmsIdealGeometryXML_cfi import *
process.load("SLHCUpgradeSimulations.Geometry.longbarrel_cmsIdealGeometryXML_cff")
#
process.TrackerGeometricDetESModule = cms.ESProducer("TrackerGeometricDetESModule",
    fromDDD = cms.bool(True)
)


process.load("SLHCUpgradeSimulations.Utilities.StackedTrackerGeometry_cfi")

###################################
#
# define number of events to be processed (-1 means all)
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
#
# define some input tags for the analyzer used in this module
process.PATtemplate = cms.EDAnalyzer("TestL1Track",
#  trackletVTX = cms.string("center"),
  trackletVTX = cms.string("offcenter"),
  seedSuperLayer = cms.int32(0),
  numberStubs = cms.int32(6),
  windowSize = cms.double(99.0),
  tightStubsL1Trk = cms.int32(0),
#
   src = cms.InputTag("siPixelRecHits"),
#
   stereoRecHits = cms.InputTag("siStripMatchedRecHits","stereoRecHit"),
   rphiRecHits = cms.InputTag("siStripMatchedRecHits","rphiRecHit"),
   matchedRecHits = cms.InputTag("siStripMatchedRecHits","matchedRecHit"),
   simtrackHits = cms.InputTag("famosSimHits"),
#
   locStubsPixelDigis = cms.InputTag("LocalStubsFromPixelDigis"),
   locStubsSimHits = cms.InputTag("LocalStubsFromSimHits"),
   globStubsPixelDigis = cms.InputTag("GlobalStubsFromPixelDigis"),
   globStubsSimHits = cms.InputTag("GlobalStubsFromSimHits"),
#
   trackletsPixelDigis = cms.InputTag("TrackletsFromPixelDigis","ShortTracklets"),
   trackletsSimHits = cms.InputTag("TrackletsFromSimHits","ShortTracklets"),
#
   l1tracksPixelDigis = cms.InputTag("L1TracksFromPixelDigis","L1Tracks"),
   l1tracksSimHits = cms.InputTag("L1TracksFromSimHits","L1Tracks"),
#
   trackProducer = cms.InputTag("ctfWithMaterialTracks"),
#
    L1EGamma       = cms.InputTag("L1ExtraMaker","EGamma"),
    L1IsoEGamma    = cms.InputTag("L1ExtraMaker","IsoEGamma"),
    L1Tau          = cms.InputTag("L1ExtraMaker","Taus"),
    L1IsoTau       = cms.InputTag("L1ExtraMaker","IsoTaus"),
    Jets           = cms.InputTag("L1ExtraMaker","Jets"),
#
    genParticles   = cms.InputTag("genParticles"),


   # for using track hit association
   associatePixel = cms.bool(True),
   associateStrip = cms.bool(False),
   associateRecoTracks = cms.bool(False),
   ROUList = cms.vstring('famosSimHitsTrackerHits')
)

################
# define output #
################
# 
process.TFileService = cms.Service("TFileService", fileName = cms.string('muonPair_L1Tracks_Longbarrel_2dClust_PixelrayMatch_NoPU_L1TrkTrig.root' ))
#
# define the global process for cmsRun
process.p = cms.Path( process.PATtemplate )

#############################
# END OF CONFIGURATION FILE #
#############################






