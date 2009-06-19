##############################################################################
# if you are unsure as to why this used to work but does not any more you 
# probably need to include an ESPrefer module to your config file.
# For details see the wikipage:
#
# https://twiki.cern.ch/twiki/bin/view/CMS/TrackTriggerHitsAndStubs
#
# A.W. Rose
##############################################################################


import FWCore.ParameterSet.Config as cms

##############################################################################

HitMatchingAlgorithm_a_PSimHit_ = cms.ESProducer( "HitMatchingAlgorithm_a_PSimHit_" )

HitMatchingAlgorithm_a_PixelDigi_ = cms.ESProducer( "HitMatchingAlgorithm_a_PixelDigi_" )
 
HitMatchingAlgorithm_a_TTHit_ = cms.ESProducer( "HitMatchingAlgorithm_a_TTHit_" )

##############################################################################

HitMatchingAlgorithm_globalgeometry_PSimHit_ = cms.ESProducer(
    "HitMatchingAlgorithm_globalgeometry_PSimHit_",
    ipWidth = cms.double(15.0),
    minPtThreshold = cms.double(2.0)
    )

HitMatchingAlgorithm_globalgeometry_PixelDigi_ = cms.ESProducer(
    "HitMatchingAlgorithm_globalgeometry_PixelDigi_",
    ipWidth = cms.double(200.0),
    minPtThreshold = cms.double(2.0)
    )

HitMatchingAlgorithm_globalgeometry_TTHit_ = cms.ESProducer(
    "HitMatchingAlgorithm_globalgeometry_TTHit_",
    ipWidth = cms.double(200.0),
    minPtThreshold = cms.double(2.0)
    )

##############################################################################

HitMatchingAlgorithm_window_PSimHit_ = HitMatchingAlgorithm_globalgeometry_PSimHit_

HitMatchingAlgorithm_window_PixelDigi_ = cms.ESProducer(
    "HitMatchingAlgorithm_window_PixelDigi_",
    ipWidth = cms.double(15.0),
    minPtThreshold = cms.double(5.0),
    RowResolution = cms.double(1),
    ColResolution = cms.double(1)
    )

HitMatchingAlgorithm_window_TTHit_ = cms.ESProducer(
    "HitMatchingAlgorithm_window_TTHit_",
    ipWidth = cms.double(15.0),
    minPtThreshold = cms.double(5.0),
    RowResolution = cms.double(1),
    ColResolution = cms.double(1)
    )

##############################################################################

HitMatchingAlgorithm_thresholds_PSimHit_ = HitMatchingAlgorithm_globalgeometry_PSimHit_

HitMatchingAlgorithm_thresholds_PixelDigi_ = cms.ESProducer(
    
    "HitMatchingAlgorithm_thresholds_PixelDigi_",
    
    Thresholds = cms.VPSet(
    
    cms.PSet(
    # thresholds for layer 0
    Layer = cms.uint32(0),
    RowCuts = cms.vuint32(35,51,145,162,600),
    RowOffsets = cms.vuint32(7,7,8,8,9),
    RowWindows = cms.vuint32(2,2,2,2,2),
    ColumnCutMin = cms.uint32(0),
    ColumnCutMax = cms.uint32(2)
    ),
    
    cms.PSet(
    # thresholds for layer 1
    Layer = cms.uint32(1),
    RowCuts = cms.vuint32(24,140,170,600),
    RowOffsets = cms.vuint32(7,8,8,9),
    RowWindows = cms.vuint32(3,2,3,2),                          
    ColumnCutMin = cms.uint32(0),
    ColumnCutMax = cms.uint32(2)
    )
    )
    )

HitMatchingAlgorithm_thresholds_TTHit_ = cms.ESProducer(
    
    "HitMatchingAlgorithm_thresholds_TTHit_",
    
    Thresholds = cms.VPSet(
    
    cms.PSet(
    # thresholds for layer 0
    Layer = cms.uint32(0),
    RowCuts = cms.vuint32(35,51,145,162,600),
    RowOffsets = cms.vuint32(7,7,8,8,9),
    RowWindows = cms.vuint32(2,2,2,2,2),
    ColumnCutMin = cms.uint32(0),
    ColumnCutMax = cms.uint32(2)
    ),
    
    cms.PSet(
    # thresholds for layer 1
    Layer = cms.uint32(1),
    RowCuts = cms.vuint32(24,140,170,600),
    RowOffsets = cms.vuint32(7,8,8,9),
    RowWindows = cms.vuint32(3,2,3,2),                          
    ColumnCutMin = cms.uint32(0),
    ColumnCutMax = cms.uint32(2)
    )
    )
    )

##############################################################################

