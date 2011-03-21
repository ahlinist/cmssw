import FWCore.ParameterSet.Config as cms

#-------------------------------------------------------------------------------
from Configuration.EventContent.EventContent_cff import *

# general services, as Random Number Generator Service needed for digis
from Configuration.StandardSequences.Services_cff import *

# global initialization of geometry --------------------------------------------
from Configuration.StandardSequences.Geometry_cff import *

from Configuration.StandardSequences.MagneticField_38T_cff import *
#from Configuration.StandardSequences.MagneticField_cff import *

#-------------------------------------------------------------------------------
### conditions that are needed for digitization and higher levels
#from CalibMuon.Configuration.DT_FakeConditions_cff import *
#from Configuration.StandardSequences.FrontierConditions_GlobalTag_cff import *
#from CalibMuon.DTCalibration.DTTTrigCalibration_cfi import *

# muons specific ---------------------------------------------------------------
from Geometry.DTGeometry.dtGeometry_cfi import *
DTGeometryESModule.applyAlignment = False
#from Geometry.MuonNumbering.muonNumberingInitialization_cfi import *

### look at L1Trigger/Configuration/python/SimL1Emulator_cff.py:
from Configuration.StandardSequences.L1Emulator_cff import *
#simDttfDigis.Debug = cms.untracked.int32(1)
#simGmtDgis.Debug = 0
#simGmtDigis.BX_min = -1
#simGmtDigis.BX_max = 1
#simGmtDigis.BX_min_readout = -1
#simGmtDigis.BX_max_readout = 1

#------------------------------------------------------------------------------
### Include configuration ParameterSets
### Needed to access DTConfigManagerRcd
from L1TriggerConfig.DTTPGConfigProducers.L1DTTPGConfig_cff import *


source = cms.Source(
    "PoolSource",
    debugFlag = cms.untracked.bool(False),
    debugVebosity = cms.untracked.uint32(10),
    fileNames = cms.untracked.vstring("file:input.root")
    )


options = cms.untracked.PSet(
    FailModule = cms.untracked.vstring('ProductNotFound'),
    wantSummary = cms.untracked.bool(False)
    )


MessageLogger = cms.Service(
    "MessageLogger",
    debugModules = cms.untracked.vstring(
    'DTL1slhcProd', 
    'dttfDigis', 
    'gmtDigis'),
    destinations = cms.untracked.vstring('out.msg')
    )


DTL1slhcProd = cms.EDProducer(
    "DTL1slhcProd",
    singleMuonPt = cms.untracked.double(0),
    outputDir = cms.untracked.string(''),
    asciiFileName = cms.untracked.string(''),
    pattern_out_ampl_factor = cms.untracked.double(10000.),
    patternFileName = cms.untracked.string(''),
    interestingToMe = cms.untracked.string(''),
    #
    min_invRb = cms.untracked.double(0.000045),
    max_invRb = cms.untracked.double(0.0035),
    # Assumed radius of boundary surface of the magnetic field:
    Erre      = cms.untracked.double(360.0),
    # Correction factor computing Pt using third "DT muon" method:
    station2_correction = cms.untracked.double(1.0),
    third_method_accurate = cms.untracked.bool(False),
    # To apply PT cut to stubs
    magneticFieldStrength = cms.untracked.double(4.0112),
    ptThreshold           = cms.untracked.double(5.0),
    #
    MuonSeedCollectionLabel = cms.untracked.string('MuonSeed'),    
    TrackCollectionLabel    = cms.untracked.string('muons'),
    MonteCarloSource        = cms.untracked.string('source'),
    triggerResults          = cms.InputTag("TriggerResults"),
#    digiTag                 = cms.InputTag("muonDTDigis"),
    digiTag                 = cms.InputTag("simMuonDTDigis"),
    #
    debug_tracks_and_vertices = cms.untracked.bool(True),
    debug_bti                 = cms.untracked.bool(False),
    debug_dtmatch             = cms.untracked.bool(True),
    debug_dttrackmatch        = cms.untracked.bool(True),
    debug_stubs               = cms.untracked.bool(False),
    debug_dttrig              = cms.untracked.bool(True),
    debug_tstheta             = cms.untracked.bool(True),
    use_TSTheta               = cms.untracked.bool(False),
    use_roughTheta            = cms.untracked.bool(False),
    debug                     = cms.untracked.bool(False),
    #
    # Required in DTTrigProd class 
    # (see L1Trigger/DTTrigger/python/dtTriggerPrimitiveDigis_cfi.py):
    tTrigModeConfig = cms.PSet(debug = cms.untracked.bool(False),
                               vPropWire = cms.double(24.4),
                               tofCorrType = cms.int32(1),
                               tTrig = cms.double(500.0)),
    tTrigMode       = cms.string('DTTTrigSyncTOFCorr')
   )




