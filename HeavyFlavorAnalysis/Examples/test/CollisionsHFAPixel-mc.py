import os
import FWCore.ParameterSet.Config as cms

process = cms.Process("HFA")

# ----------------------------------------------------------------------
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )


# ----------------------------------------------------------------------
# -- Database configuration
process.load("CondCore.DBCommon.CondDBCommon_cfi")
process.load("CondCore.DBCommon.CondDBSetup_cfi")

# -- Conditions
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("RecoVertex.BeamSpotProducer.BeamSpot_cfi")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = "MC_31X_V5::All"


# ----------------------------------------------------------------------
process.source = cms.Source(
    "PoolSource", 
    fileNames = cms.untracked.vstring(
    "/store/mc/Summer09/MinBias900GeV/GEN-SIM-RECO/MC_31X_V3-v1/0017/00EEC933-8888-DE11-9954-00304865C456.root", 
    "/store/mc/Summer09/MinBias900GeV/GEN-SIM-RECO/MC_31X_V3-v1/0017/0607B847-5388-DE11-A5C2-00238BBD7584.root", 
    "/store/mc/Summer09/MinBias900GeV/GEN-SIM-RECO/MC_31X_V3-v1/0017/08A30884-4688-DE11-A66A-00163691DC02.root", 
    "/store/mc/Summer09/MinBias900GeV/GEN-SIM-RECO/MC_31X_V3-v1/0017/226283DD-6688-DE11-9F66-00144FCAB064.root", 
    "/store/mc/Summer09/MinBias900GeV/GEN-SIM-RECO/MC_31X_V3-v1/0017/30537D20-ED89-DE11-8AAE-00E081329E7A.root", 
    "/store/mc/Summer09/MinBias900GeV/GEN-SIM-RECO/MC_31X_V3-v1/0017/3232AC9A-4B88-DE11-8D7E-00163691DBB2.root", 
    "/store/mc/Summer09/MinBias900GeV/GEN-SIM-RECO/MC_31X_V3-v1/0017/365B610E-7888-DE11-802D-003048344C22.root", 
    "/store/mc/Summer09/MinBias900GeV/GEN-SIM-RECO/MC_31X_V3-v1/0017/4A77E45D-5288-DE11-B1FE-001EC9AAA2B5.root", 
    "/store/mc/Summer09/MinBias900GeV/GEN-SIM-RECO/MC_31X_V3-v1/0017/5C0C1706-7188-DE11-86C2-00304865C466.root", 
    "/store/mc/Summer09/MinBias900GeV/GEN-SIM-RECO/MC_31X_V3-v1/0017/94D13C1B-4888-DE11-BE75-00163691D236.root"
    )
    )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )


# ----------------------------------------------------------------------
# -- PixelTree
process.load("RecoTracker.TrackProducer.TrackRefitters_cff")
process.TrackRefitter.src = 'generalTracks'

try:
    rootFileName = os.environ["JOB"] + "-pixel.root"
except KeyError:
    rootFileName = "collisions-pixel.root"

process.PixelTree = cms.EDAnalyzer(
    "PixelTree",
    verbose                = cms.untracked.int32(0),
    rootFileName           = cms.untracked.string(rootFileName),
    muonCollectionLabel    = cms.untracked.InputTag('muons'),
    trackCollectionLabel   = cms.untracked.InputTag('generalTracks'),
    trajectoryInputLabel   = cms.untracked.InputTag('TrackRefitter'),
    pixelClusterLabel      = cms.untracked.InputTag('siPixelClusters'),
    L1GTReadoutRecordLabel = cms.untracked.InputTag("gtDigis"), 
    hltL1GtObjectMap       = cms.untracked.InputTag("hltL1GtObjectMap"), 
    HLTResultsLabel        = cms.untracked.InputTag("TriggerResults::HLT")
    )

# ----------------------------------------------------------------------
process.PixelFilter = cms.EDFilter(
    "SkimEvents",
    verbose                      = cms.untracked.int32(10),
    filterOnPrimaryVertex        = cms.untracked.int32(1),
    primaryVertexCollectionLabel = cms.untracked.InputTag('offlinePrimaryVertices'),
    filterOnTracks               = cms.untracked.int32(1),
    trackCollectionLabel         = cms.untracked.InputTag('generalTracks'),
    filterOnPixelCluster         = cms.untracked.int32(1),
    PixelClusterCollectionLabel  = cms.untracked.InputTag('siPixelClusters'),
    
    )


# ----------------------------------------------------------------------
process.HepPDTESSource = cms.ESSource(
    "HepPDTESSource",
    pdtFileName = cms.FileInPath('SimGeneral/HepPDTESSource/data/particle.tbl')
    )
process.genParticles = cms.EDProducer(
    "GenParticleProducer",
    saveBarCodes = cms.untracked.bool(True),
    src = cms.InputTag("generator"),
    abortOnUnknownPDGCode = cms.untracked.bool(False)
    )


# ----------------------------------------------------------------------
process.genDump = cms.EDFilter(
    "HFDumpGenerator",
    generatorCandidates = cms.untracked.string('genParticles'),
    generatorEvent = cms.untracked.string('generator')
    )


# ----------------------------------------------------------------------
try:
    rootFileName = os.environ["JOB"] + "-hfa.root"
except KeyError:
    rootFileName = "collisions-hfa.root"

process.tree = cms.EDAnalyzer(
    "HFTree",
    verbose  = cms.untracked.int32(0),
    requireCand =  cms.untracked.bool(True),
    fileName = cms.string(rootFileName)
    )


# ----------------------------------------------------------------------
process.load("TrackingTools/TransientTrack/TransientTrackBuilder_cfi")
process.load("SimTracker.TrackAssociation.TrackAssociatorByHits_cfi")
process.trkDump = cms.EDFilter(
    "HFDumpTracks",
    verbose = cms.untracked.int32(0),
    generatorEventLabel = cms.untracked.string('generator'),
    muonsLabel = cms.untracked.InputTag("muons"),
    trackingParticlesLabel = cms.untracked.string('trackingParticles'),
    associatorLabel = cms.untracked.string('TrackAssociatorByHits'),
    doTruthMatching = cms.untracked.int32(3),
    tracksLabel = cms.untracked.string('generalTracks'),
    simTracksLabel = cms.untracked.string('allLayer1TrackCands')
    )

# ----------------------------------------------------------------------
process.muonDump = cms.EDFilter(
    "HFDumpMuons",
    verbose = cms.untracked.int32(0),
    muonsLabel = cms.untracked.InputTag("muons"),
    doTruthMatching = cms.untracked.int32(0),
    )


# ----------------------------------------------------------------------
process.triggerDump = cms.EDFilter(
    "HFDumpTrigger",
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
    massLow  = cms.untracked.double(2.5), 
    massHigh = cms.untracked.double(11.0)
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
process.bupsikpDump = cms.EDFilter(
    "HFBu2JpsiKp",
    verbose = cms.untracked.int32(0), 
    muonsLabel = cms.untracked.InputTag("muons"),
    tracksLabel = cms.untracked.string('generalTracks'),
    muonPt = cms.untracked.double(3.0),
    trackPt = cms.untracked.double(0.1),
    type = cms.untracked.int32(100521) 
    )

# ----------------------------------------------------------------------
process.bdpsikstarDump = cms.EDFilter(
    "HFBd2JpsiKstar",
    verbose = cms.untracked.int32(0), 
    muonsLabel = cms.untracked.InputTag("muons"),
    tracksLabel = cms.untracked.string('generalTracks'),
    muonPt = cms.untracked.double(3.0),
    kaonPt = cms.untracked.double(0.1),
    pionPt = cms.untracked.double(0.1),
    type = cms.untracked.int32(100511) 
    )

# ----------------------------------------------------------------------
process.bspsiphiDump = cms.EDFilter(
    "HFBs2JpsiPhi",
    verbose = cms.untracked.int32(0), 
    muonsLabel = cms.untracked.InputTag("muons"),
    tracksLabel = cms.untracked.string('generalTracks'),
    muonPt = cms.untracked.double(3.0),
    trackPt = cms.untracked.double(0.1),
    type = cms.untracked.int32(100531) 
    )

# ----------------------------------------------------------------------
process.b2muD0Dump = cms.EDFilter(
    "HFB2muD0",
    verbose = cms.untracked.int32(0), 
    muonsLabel = cms.untracked.InputTag("muons"),
    tracksLabel = cms.untracked.string('generalTracks'),
    muonPt = cms.untracked.double(3.0),
    trackPt = cms.untracked.double(0.1),
    deltaR =  cms.untracked.double(1.5),
    deltaMD0 = cms.untracked.double(0.3),
    deltaMDs = cms.untracked.double(0.3)
    )

# ----------------------------------------------------------------------
process.p = cms.Path(
#    process.TrackRefitter*
#    process.PixelTree*
    process.PixelFilter* 
    process.genParticles* 
    process.genDump*
    process.trkDump*
    process.muonDump*
    process.triggerDump*
    process.bmmDump*
    process.bmtDump*
    process.bupsikpDump*
    process.bdpsikstarDump*
    process.bspsiphiDump*
    process.tree
)




