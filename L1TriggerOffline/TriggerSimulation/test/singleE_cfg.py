import FWCore.ParameterSet.Config as cms

process = cms.Process("PROD")
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger = cms.Service ("MessageLogger",
    destinations = cms.untracked.vstring( "debug" ),
    debug = cms.untracked.PSet(
        threshold = cms.untracked.string ( 'DEBUG' )
    ),
    debugModules = cms.untracked.vstring("trackTriggerHits")
)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)
process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
    moduleSeeds = cms.PSet(
        siTrackerGaussianSmearingRecHits = cms.untracked.uint32(24680),
        caloRecHits = cms.untracked.uint32(654321),
        paramMuons = cms.untracked.uint32(54525),
        famosSimHits = cms.untracked.uint32(13579),
        VtxSmeared = cms.untracked.uint32(123456789)
    ),
    sourceSeed = cms.untracked.uint32(123456789)
)

# event generation
process.source = cms.Source("FlatRandomEGunSource",
    PGunParameters = cms.untracked.PSet(
        # you can request more than 1 particle
        # since PartID is a vector, you can place in as many
        # PDG id's as you wish, comma seaparated
        #
        PartID = cms.untracked.vint32(11),
        MaxEta = cms.untracked.double(1.5),
        MaxPhi = cms.untracked.double(3.14159265359),
        MinEta = cms.untracked.double(-1.5),
        MinE = cms.untracked.double(35.0),
        MinPhi = cms.untracked.double(-3.14159265359), ## in radians

        MaxE = cms.untracked.double(35.0)
    ),
    Verbosity = cms.untracked.int32(0), ## set to 1 (or greater)  for printouts

    AddAntiParticle = cms.untracked.bool(False) ## back-to-back particles

)

# track trigger
process.load("SLHCUpgradeSimulations.L1Trigger.TrackTriggerNaiveGeometry_cff")
process.load("SLHCUpgradeSimulations.L1Trigger.trackTriggerHitsFromMC_cfi")
process.trackTriggerHits.inputTag = 'source'
process.trackTriggerHits.doPileUp = False

process.TFileService = cms.Service("TFileService",
    fileName = cms.string( 'trackTrigHistos.root' )
)
process.load("SLHCUpgradeSimulations.L1Trigger.trackTriggerAnalyzer_cfi")

process.p = cms.Path(process.trackTriggerHits*
                     process.trackTriggerAnalyzer)


# output file
process.output = cms.OutputModule("PoolOutputModule",
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_trackTriggerHits_*_*', 
        'keep *_trackTriggerPrimitives_*_*'),
    fileName = cms.untracked.string('singleE.root')
)

process.outpath = cms.EndPath(process.output)

