import FWCore.ParameterSet.Config as cms

isolhbherechitreflaggerJETMET = cms.EDProducer(
    'IsolatedHBHERecHitReflaggerJETMET',

    debug = cms.untracked.bool(False),

    hbheInput = cms.InputTag('hbhereco'),
    ecalInputs = cms.VInputTag(cms.InputTag("ecalRecHit","EcalRecHitsEB"), cms.InputTag("ecalRecHit","EcalRecHitsEE")),
    trackInput = cms.InputTag('generalTracks'),

    # isolation cuts
    LooseHcalIsol = cms.double(0.08),
    LooseEcalIsol = cms.double(0.08),
    LooseTrackIsol = cms.double(0.10),
    TightHcalIsol = cms.double(0.04),
    TightEcalIsol = cms.double(0.04),
    TightTrackIsol = cms.double(0.05),

    LooseRBXEne1 = cms.double(10.0),
    LooseRBXEne2 = cms.double(300.0),
    LooseRBXHits1 = cms.int32(14),
    LooseRBXHits2 = cms.int32(10),
    TightRBXEne1 = cms.double(10.0),
    TightRBXEne2 = cms.double(50.0),
    TightRBXHits1 = cms.int32(12),
    TightRBXHits2 = cms.int32(10),

    LooseHPDEne1 = cms.double(10.0),
    LooseHPDEne2 = cms.double(80.0),
    LooseHPDHits1 = cms.int32(6),
    LooseHPDHits2 = cms.int32(3),
    TightHPDEne1 = cms.double(10.0),
    TightHPDEne2 = cms.double(25.0),
    TightHPDHits1 = cms.int32(5),
    TightHPDHits2 = cms.int32(3),

    LooseDiHitEne = cms.double(50.0),
    TightDiHitEne = cms.double(15.0),
    LooseMonoHitEne = cms.double(35.0),
    TightMonoHitEne = cms.double(8.0),

    # which status bit to set when a hit is bad
    hbheFlagBit = cms.int32(31),

    # used by the object validator
    HBThreshold = cms.double(0.7),
    HESThreshold = cms.double(0.8),
    HEDThreshold = cms.double(0.8),
    EBThreshold = cms.double(0.07),
    EEThreshold = cms.double(0.3),
    HcalAcceptSeverityLevel = cms.uint32(9),
    EcalAcceptSeverityLevel = cms.uint32(1),
    UseHcalRecoveredHits = cms.bool(True),
    UseEcalRecoveredHits = cms.bool(False),
    MinValidTrackPt = cms.double(0.5),
    MinValidTrackPtBarrel = cms.double(1.0),
    MinValidTrackNHits = cms.int32(6),

    # track associator stuff
    TrackAssociatorParameters = cms.PSet(
    muonMaxDistanceSigmaX = cms.double(0.0),
    muonMaxDistanceSigmaY = cms.double(0.0),
    CSCSegmentCollectionLabel = cms.InputTag("cscSegments"),
    dRHcal = cms.double(9999.0),
    dREcal = cms.double(9999.0),
    CaloTowerCollectionLabel = cms.InputTag("towerMaker"),
    useEcal = cms.bool(True),
    dREcalPreselection = cms.double(0.05),
    HORecHitCollectionLabel = cms.InputTag("horeco"),
    dRMuon = cms.double(9999.0),
    trajectoryUncertaintyTolerance = cms.double(-1.0),
    crossedEnergyType = cms.string('SinglePointAlongTrajectory'),
    muonMaxDistanceX = cms.double(5.0),
    muonMaxDistanceY = cms.double(5.0),
    useHO = cms.bool(False),
    accountForTrajectoryChangeCalo = cms.bool(True),
    DTRecSegment4DCollectionLabel = cms.InputTag("dt4DSegments"),
    EERecHitCollectionLabel = cms.InputTag("ecalRecHit",
                                           "EcalRecHitsEE"),
    dRHcalPreselection = cms.double(0.2),
    useMuon = cms.bool(False),
    useCalo = cms.bool(True),
    EBRecHitCollectionLabel = cms.InputTag("ecalRecHit",
                                           "EcalRecHitsEB"),
    dRMuonPreselection = cms.double(0.2),
    usePreshower = cms.bool(False),
    dRPreshowerPreselection = cms.double(0.2),
    truthMatch = cms.bool(False),
    HBHERecHitCollectionLabel = cms.InputTag("hbhereco"),
    useHcal = cms.bool(True)
    ),
 )
