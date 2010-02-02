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
from Configuration.StandardSequences.FakeConditions_cff import *

# muons specific ---------------------------------------------------------------
#from Geometry.DTGeometry.dtGeometry_cfi import *
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
    outputDir   = cms.untracked.string(''),
    asciiFileName = cms.untracked.string(''),
    interestingToMe  = cms.untracked.string(''),
    #
    min_invRb = cms.untracked.double(0.000045),
    max_invRb = cms.untracked.double(0.0035),
    #
    labels = cms.untracked.vstring("Stubs-5-3-0", "Stubs-5-1-0", 
                                   "Stubs-3-2-0", "Stubs-3-1-0", 
                                   "Stubs-5-3-V", "Stubs-5-0-V", 
				   "Stubs-3-0-V",
                                   "Mu-5-0", "Mu-3-0", "Mu-2-0", "Mu-1-0", 
                                   "Mu-5-V", "Mu-3-V", "Mu-2-V", "Mu-1-V",
                                   "Mu-0-V",
                                   "IMu-5-0", "IMu-3-0", "IMu-2-0", "IMu-1-0",
                                   "IMu-5-V", "IMu-3-V", "IMu-2-V", "IMu-1-V",
                                   "IMu-0-V",
                                   "mu-5-0", "mu-3-0", "mu-2-0", "mu-1-0", 
                                   "mu-5-V", "mu-3-V", "mu-2-V", "mu-1-V",
                                   "mu-0-V",
                                   "only-Mu-V"),
    #    
    MuonSeedCollectionLabel = cms.untracked.string('MuonSeed'),    
    TrackCollectionLabel    = cms.untracked.string('muons'),
    MonteCarloSource        = cms.untracked.string('source'),
    triggerResults          = cms.InputTag("TriggerResults"),
    digiTag                 = cms.InputTag("muonDTDigis"),
    #
    debug_tracks_and_vertices = cms.untracked.bool(True),
    debug_bti                 = cms.untracked.bool(True),
    debug_dtmatch             = cms.untracked.bool(True),
    debug_dttrackmatch        = cms.untracked.bool(True),
    debug_stubs               = cms.untracked.bool(True),
    debug_dttrig              = cms.untracked.bool(True),
    debug_tstheta             = cms.untracked.bool(True),
    use_TSTheta               = cms.untracked.bool(False),
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



