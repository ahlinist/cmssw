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
    "/store/relval/CMSSW_3_1_0_pre9/RelValbJpsiX/GEN-SIM-RECO/STARTUP_31X_v1/0000/EAF0DDC1-654E-DE11-8940-001A928116BE.root"
    )
    )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )


# ----------------------------------------------------------------------
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#process.GlobalTag.globaltag = cms.string('IDEAL_31X::All')
process.GlobalTag.globaltag = cms.string('MC_31X_V3::All')
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

# ----------------------------------------------------------------------
process.genDump = cms.EDFilter("HFDumpGenerator",
    generatorCandidates = cms.untracked.string('genParticles'),
    generatorEvent = cms.untracked.string('generator')
)


# ----------------------------------------------------------------------
process.tree = cms.EDAnalyzer(
    "HFTree",
    requireCand =  cms.untracked.bool(False),
    fileName = cms.string('test.root')
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
process.signalDump = cms.EDFilter("HFDumpSignal",
    deltaR = cms.untracked.double(1.5),
    verbose = cms.untracked.int32(1),
    muonsLabel = cms.untracked.InputTag("muons"),
    muonPt = cms.untracked.double(3.0),
    kaonPt = cms.untracked.double(1.0),
    tracksLabel = cms.untracked.string('generalTracks')
)

# ----------------------------------------------------------------------
process.p = cms.Path(
    process.genParticles* 
    process.genDump*
    process.trkDump*
#    process.muonDump*
    process.triggerDump*
#    process.signalDump*
    process.tree
)




