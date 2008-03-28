import FWCore.ParameterSet.Config as cms

analElId = cms.EDFilter("AnalElId",
    ElTruth = cms.InputTag("electrontruth","ElectronTrackTruth"),
    OutputFile = cms.string('ciccio.root'),
    associatePixel = cms.bool(True),
    EtaCut = cms.double(2.4),
    SimHits = cms.InputTag("g4SimHits"),
    Isol = cms.double(0.5),
    TrackLabel = cms.InputTag("merg"),
    associateStrip = cms.bool(True),
    Associator = cms.string('TrackAssociatorByHits'),
    TrTruth = cms.InputTag("trackingParticles"),
    PtCut = cms.double(1.0),
    PFRecTrackLabel = cms.InputTag("repreid"),
    ROUList = cms.vstring('TrackerHitsTIBLowTof', 'TrackerHitsTIBHighTof', 'TrackerHitsTIDLowTof', 'TrackerHitsTIDHighTof', 'TrackerHitsTOBLowTof', 'TrackerHitsTOBHighTof', 'TrackerHitsTECLowTof', 'TrackerHitsTECHighTof', 'TrackerHitsPixelBarrelLowTof', 'TrackerHitsPixelBarrelHighTof', 'TrackerHitsPixelEndcapLowTof', 'TrackerHitsPixelEndcapHighTof')
)


