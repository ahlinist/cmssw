import FWCore.ParameterSet.Config as cms

process = cms.Process("HFA")

# ----------------------------------------------------------------------
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )


# ----------------------------------------------------------------------
process.source = cms.Source(
    "PoolSource", 
    fileNames = cms.untracked.vstring(
    "/store/user/starodumov/bsmm/reco/reco-10000.root", 
    "/store/user/starodumov/bsmm/reco/reco-10001.root"
    )
    )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )


# ----------------------------------------------------------------------
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = cms.string('MC_31X_V5::All')
process.load("Configuration.StandardSequences.MagneticField_cff")

# ----------------------------------------------------------------------
process.HepPDTESSource = cms.ESSource("HepPDTESSource",
    pdtFileName = cms.FileInPath('SimGeneral/HepPDTESSource/data/particle.tbl')
)
process.genParticles = cms.EDProducer("GenParticleProducer",
    saveBarCodes = cms.untracked.bool(True),
    src = cms.InputTag("generator"),
    abortOnUnknownPDGCode = cms.untracked.bool(False)
)


# ---------------------------------------------------------------------
process.load("TrackingTools/TransientTrack/TransientTrackBuilder_cfi")
process.load('Configuration/StandardSequences/GeometryIdeal_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.load("Configuration.StandardSequences.MagneticField_38T_cff")


# ----------------------------------------------------------------------
process.genDump = cms.EDFilter("HFDumpGenerator",
    generatorCandidates = cms.untracked.string('genParticles'),
    generatorEvent = cms.untracked.string('generator')
)


# ----------------------------------------------------------------------
process.tree = cms.EDAnalyzer(
    "HFTree",
    verbose  = cms.untracked.int32(1),
    requireCand =  cms.untracked.bool(True),
    fileName = cms.untracked.string('test.root')
    )


# ----------------------------------------------------------------------
process.load("SimTracker.TrackAssociation.TrackAssociatorByHits_cfi")


process.trkDump = cms.EDFilter("HFDumpTracks",
    # simTracksLabel = cms.untracked.string('famosSimHits'),
    # simTracksLabel = cms.untracked.string('g4SimHits'),
    verbose = cms.untracked.int32(0),
    generatorEventLabel = cms.untracked.string('generator'),
    muonsLabel = cms.untracked.InputTag("muons"),
    trackingParticlesLabel = cms.untracked.string('trackingParticles'),
    associatorLabel = cms.untracked.string('TrackAssociatorByHits'),
    doTruthMatching = cms.untracked.int32(0),
    tracksLabel = cms.untracked.string('generalTracks'),
    simTracksLabel = cms.untracked.string('allLayer1TrackCands')
)

# ----------------------------------------------------------------------
process.muonDump = cms.EDFilter("HFDumpMuons",
    verbose = cms.untracked.int32(0),
    muonsLabel = cms.untracked.InputTag("muons"),
    doTruthMatching = cms.untracked.int32(0),
)


# ----------------------------------------------------------------------
process.triggerDump = cms.EDFilter("HFDumpTrigger",
   verbose                 = cms.untracked.int32(0),
   L1GTReadoutRecordLabel  = cms.untracked.string("gtDigis"), 
   hltL1GtObjectMap        = cms.untracked.InputTag("hltL1GtObjectMap"), 
   L1MuonsLabel            = cms.untracked.InputTag("hltL1extraParticles"), 
   HLTResultsLabel         = cms.untracked.InputTag("TriggerResults::HLT"), 
   TriggerEventLabel       = cms.untracked.InputTag("hltTriggerSummaryAOD::HLT"), 
   hltLabel      = cms.untracked.InputTag("TriggerResults::HLT"), 
)

# ----------------------------------------------------------------------
process.bmtDump = cms.EDFilter(
    "HFMuonAndTrack",
    verbose = cms.untracked.int32(0), 
    muonsLabel = cms.untracked.InputTag("muons"),
    tracksLabel = cms.untracked.string('generalTracks'),
    muonPt = cms.untracked.double(3.0),
    trackPt = cms.untracked.double(1.0),
    type = cms.untracked.int32(1300), 
    massLow  = cms.untracked.double(4.0), 
    massHigh = cms.untracked.double(6.0)
)

# ----------------------------------------------------------------------
process.bmmDump = cms.EDFilter(
    "HFDimuons",
    verbose = cms.untracked.int32(0), 
    muonsLabel = cms.untracked.InputTag("muons"),
    tracksLabel = cms.untracked.string('generalTracks'),
    muonPt = cms.untracked.double(3.0),
    type = cms.untracked.int32(531), 
    massLow  = cms.untracked.double(4.0), 
    massHigh = cms.untracked.double(6.0)
)


# ----------------------------------------------------------------------
process.p = cms.Path(
    process.genParticles* 
    process.genDump*
    process.trkDump*
    process.muonDump*
    process.triggerDump*
    process.bmmDump*
    process.bmtDump*
    process.tree
)




