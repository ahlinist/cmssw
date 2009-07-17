import FWCore.ParameterSet.Config as cms

process = cms.Process("HFA")

# ----------------------------------------------------------------------
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )


# ----------------------------------------------------------------------
process.source = cms.Source("PoolSource", 
    fileNames = cms.untracked.vstring(
#310pre9        'file:/shome/ursl/root/EAF0DDC1-654E-DE11-8940-001A928116BE.root'
    'file:/shome/ursl/root/D2A969BB-F16E-DE11-9AC4-001F2907DA48.root'
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
process.tree = cms.EDAnalyzer("HFTree",
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
   verbose                 = cms.untracked.int32(1),
   particleMap             = cms.untracked.string("l1extraParticleMap"),
   L1GTReadoutRecordLabel  = cms.untracked.string("gtDigis"), 
   hltL1GtObjectMap        = cms.untracked.InputTag("hltL1GtObjectMap"), 
   L1MuonsLabel            = cms.untracked.InputTag("hltL1extraParticles"), 
   HLTResultsLabel         = cms.untracked.InputTag("TriggerResults::HLT8E29"), 
   TriggerEventLabel       = cms.untracked.InputTag("hltTriggerSummaryAOD::HLT8E29"), 
                                   
   L1TriggerName = cms.untracked.string("L1_DoubleMu3"), 
   hltLabel      = cms.untracked.InputTag("TriggerResults::HLT"), 
   HLTriggerName = cms.untracked.string("HLTBJPsiMuMu"), 
   HLTfiltObj0   = cms.untracked.string("muTracks"), 
   HLTfiltObj1   = cms.untracked.string("CandFromMumu"), 
   HLTfiltObj2   = cms.untracked.string("JpsitoMumuL1Seed"), 
   HLTfiltObj3   = cms.untracked.string("JpsitoMumuL2Filtered"), 
   HLTfiltObj4   = cms.untracked.string("displacedJpsitoMumuFilter")
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
    process.muonDump*
    process.triggerDump*
#    process.signalDump*
    process.tree
)




