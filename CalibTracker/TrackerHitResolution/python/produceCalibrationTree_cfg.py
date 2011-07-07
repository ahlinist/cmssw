import FWCore.ParameterSet.Config as cms

process = cms.Process('CALIB')
process.load('CalibTracker.Configuration.setupCalibrationTree_cff')
process.load('Configuration.StandardSequences.Geometry_cff')
process.load('Configuration/StandardSequences/MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')


process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("RecoLocalTracker.Configuration.RecoLocalTracker_cff")

process.GlobalTag.globaltag = 'GR_P_V20::All'

process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.StandardSequences.Services_cff')
process.add_( cms.Service( "TFileService",
                           fileName = cms.string( 'calibTree.root' ),
                           closeFileFast = cms.untracked.bool(True)  ) )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )
process.source = cms.Source (
    "PoolSource",
    fileNames = cms.untracked.vstring('/store/data/Run2011A/MinimumBias/ALCARECO/SiStripCalMinBias-v4/000/166/863/66A57C37-3B96-E011-BAF8-003048F024C2.root')
    )

# Definition of the track collection module label
# To use the prompt reco dataset uncomment the following two lines
#process.CalibrationTracks.src = 'generalTracks'
#process.shallowTracks.Tracks  = 'generalTracks'
# To use the SiStripCalMinBias AlCaReco dataset uncomment the following two lines
process.CalibrationTracks.src = 'ALCARECOSiStripCalMinBias'
process.shallowTracks.Tracks  = 'ALCARECOSiStripCalMinBias'

# BSCNoBeamHalo selection (Not to use for Cosmic Runs) --- OUTDATED!!!
## process.load('L1TriggerConfig.L1GtConfigProducers.L1GtTriggerMaskTechTrigConfig_cff')
## process.load('HLTrigger/HLTfilters/hltLevel1GTSeed_cfi')

## process.L1T1=process.hltLevel1GTSeed.clone()
## process.L1T1.L1TechTriggerSeeding = cms.bool(True)
## process.L1T1.L1SeedsLogicalExpression = cms.string('(40 OR 41) AND NOT (36 OR 37 OR 38 OR 39)')

#process.TkCalPath = cms.Path(process.L1T1*process.TkCalFullSequence)
process.TkCalPath = cms.Path(process.TkCalFullSequence)
