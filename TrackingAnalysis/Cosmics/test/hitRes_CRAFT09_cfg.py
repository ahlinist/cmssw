import FWCore.ParameterSet.Config as cms

process = cms.Process("ReRec")

process.load("CondCore.DBCommon.CondDBSetup_cfi")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
# pointing skim
'/store/data/CRAFT09/Cosmics/RAW-RECO/GR09_31X_V5P_TrackingPointing_322-v1/0013/34CE5126-DC7D-DE11-B014-001EC9D7F1F7.root'
    )
)

# output module#
process.load("Configuration.EventContent.EventContentCosmics_cff")
process.load("CalibTracker.SiStripESProducers.SiStripQualityESProducer_cfi")
process.load("RecoLocalTracker.Configuration.RecoLocalTracker_Cosmics_cff")
process.load("RecoTracker.MeasurementDet.MeasurementTrackerESProducer_cfi")

# Conditions (Global Tag is used here):
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'CRAFT09_R_V4::All'
process.load("Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff")

#Geometry
process.load("Configuration.StandardSequences.Geometry_cff")

# Real data raw to digi
process.load("Configuration.StandardSequences.RawToDigi_Data_cff")

# reconstruction sequence for Cosmics
process.load("Configuration.StandardSequences.ReconstructionCosmics_cff")

# offline DQM
process.load("DQMOffline.Configuration.DQMOfflineCosmics_cff")
process.load("DQMServices.Components.MEtoEDMConverter_cff")

#L1 trigger validation
#process.load("L1Trigger.HardwareValidation.L1HardwareValidation_cff")
process.load("L1Trigger.Configuration.L1Config_cff")
process.load("L1TriggerConfig.CSCTFConfigProducers.CSCTFConfigProducer_cfi")
process.load("L1TriggerConfig.CSCTFConfigProducers.L1MuCSCTFConfigurationRcdSrc_cfi")

process.load("RecoTracker.TrackProducer.TrackRefitters_cff")

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('HitRes.root')
)

process.analysis = cms.EDFilter("HitRes",
    usePXB = cms.bool(True),
    usePXF = cms.bool(True),
    useTIB = cms.bool(True),
    useTOB = cms.bool(True),
    useTID = cms.bool(True),
    useTEC = cms.bool(True),
    ROUList = cms.vstring('TrackerHitsTIBLowTof', 
        'TrackerHitsTIBHighTof', 
        'TrackerHitsTOBLowTof', 
        'TrackerHitsTOBHighTof'),
    trajectories = cms.InputTag("TrackRefitterP5"),
    associatePixel = cms.bool(False),
    associateStrip = cms.bool(False),
    associateRecoTracks = cms.bool(False),
    tracks = cms.InputTag("TrackRefitterP5"),
    barrelOnly = cms.bool(False)
)

# Path and EndPath definitions
process.p = cms.Path(process.TrackRefitterP5*process.analysis)
#process.endjob_step = cms.Path(process.endOfProcess)

# Schedule definition
#process.schedule = cms.Schedule(process.p,process.endjob_step)
process.schedule = cms.Schedule(process.p)
