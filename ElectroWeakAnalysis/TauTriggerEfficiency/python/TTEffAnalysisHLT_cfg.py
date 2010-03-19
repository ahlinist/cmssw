#This config is for CMSSW_3_4_2_patch1

import FWCore.ParameterSet.Config as cms

#process = cms.Process("ANALYSIS")

#process.source = cms.Source("PoolSource",
#    fileNames = cms.untracked.vstring('/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/SD_AllMinBias-Feb9Skim_v1/0027/000C43E6-6916-DF11-9752-002618943879.root')
#)
hltCaloTowersTau1Regional = cms.EDProducer("CaloTowerCreatorForTauHLT",
    towers = cms.InputTag("hltTowerMakerForJets"),
    TauId = cms.int32(0),
    TauTrigger = cms.InputTag("hltL1extraParticles","Tau"),
    minimumE = cms.double(0.8),
    UseTowersInCone = cms.double(0.8),
    minimumEt = cms.double(0.5)
)


hltL1IsoSSEleRegioCkfTC = cms.EDProducer("CkfTrackCandidateMaker",
    src = cms.InputTag("hltL1IsoSSEleRegPSG"),
    TransientInitialStateEstimatorParameters = cms.PSet(
        propagatorAlongTISE = cms.string('PropagatorWithMaterial'),
        numberMeasurementsForFit = cms.int32(4),
        propagatorOppositeTISE = cms.string('PropagatorWithMaterialOpposite')
    ),
    RedundantSeedCleaner = cms.string('CachingSeedCleanerBySharedInput'),
    cleanTrajectoryAfterInOut = cms.bool(False),
    useHitsSplitting = cms.bool(False),
    doSeedingRegionRebuilding = cms.bool(False),
    maxNSeeds = cms.uint32(100000),
    NavigationSchool = cms.string('SimpleNavigationSchool'),
    TrajectoryBuilder = cms.string('CkfTrajectoryBuilder'),
    TrajectoryCleaner = cms.string('TrajectoryCleanerBySharedHits')
)


hltBLifetimeL3BJetTags = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('trackCounting3D2nd'),
    tagInfos = cms.VInputTag("hltBLifetimeL3TagInfos")
)


hltL3TauJetTracksAssociator = cms.EDProducer("JetTracksAssociatorAtVertex",
    jets = cms.InputTag("hltL25TauLeadingTrackPtCutSelector"),
    tracks = cms.InputTag("hltL3TauCtfWithMaterialTracks"),
    coneSize = cms.double(0.5)
)


hltL1IsolatedElectronHcalIsol = cms.EDProducer("EgammaHLTHcalIsolationProducersRegional",
    hfRecHitProducer = cms.InputTag("hltHfreco"),
    recoEcalCandidateProducer = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    egHcalIsoPtMin = cms.double(0.0),
    egHcalIsoConeSize = cms.double(0.15),
    hbRecHitProducer = cms.InputTag("hltHbhereco")
)


hltL25TauPixelTracksConeIsolationNoL2 = cms.EDProducer("ConeIsolation",
    MinimumTransverseMomentumInIsolationRing = cms.double(1.0),
    VariableMinCone = cms.double(0.05),
    BeamSpotProducer = cms.InputTag("hltOfflineBeamSpot"),
    MinimumNumberOfHits = cms.int32(3),
    MinimumTransverseMomentum = cms.double(1.0),
    JetTrackSrc = cms.InputTag("hltL25TauJetPixelTracksAssociatorNoL2"),
    VariableMaxCone = cms.double(0.17),
    DeltaZetTrackVertex = cms.double(0.2),
    SignalCone = cms.double(0.15),
    MaximumTransverseImpactParameter = cms.double(300.0),
    MatchingCone = cms.double(0.1),
    useVertex = cms.bool(True),
    MinimumNumberOfPixelHits = cms.int32(2),
    useBeamSpot = cms.bool(True),
    MaximumNumberOfTracksIsolationRing = cms.int32(0),
    UseFixedSizeCone = cms.bool(True),
    VariableConeParameter = cms.double(3.5),
    IsolationCone = cms.double(0.5),
    MinimumTransverseMomentumLeadingTrack = cms.double(5.0),
    MaximumChiSquared = cms.double(100.0),
    vertexSrc = cms.InputTag("hltPixelVertices")
)


hltEcalRegionalPi0EtaRecHit = cms.EDProducer("EcalRawToRecHitProducer",
    splitOutput = cms.bool(True),
    rechitCollection = cms.string(''),
    EErechitCollection = cms.string('EcalRecHitsEE'),
    EBrechitCollection = cms.string('EcalRecHitsEB'),
    lazyGetterTag = cms.InputTag("hltEcalRawToRecHitFacility"),
    sourceTag = cms.InputTag("hltEcalRegionalPi0EtaFEDs")
)


hltOniaCtfTrackCands = cms.EDProducer("ConcreteChargedCandidateProducer",
    src = cms.InputTag("hltOniaCtfTracks"),
    particleType = cms.string('mu-')
)


hltCkfL1IsoTrackCandidates = cms.EDProducer("CkfTrackCandidateMaker",
    src = cms.InputTag("hltL1IsoStartUpElectronPixelSeeds"),
    TransientInitialStateEstimatorParameters = cms.PSet(
        propagatorAlongTISE = cms.string('PropagatorWithMaterial'),
        numberMeasurementsForFit = cms.int32(4),
        propagatorOppositeTISE = cms.string('PropagatorWithMaterialOpposite')
    ),
    RedundantSeedCleaner = cms.string('CachingSeedCleanerBySharedInput'),
    cleanTrajectoryAfterInOut = cms.bool(False),
    useHitsSplitting = cms.bool(False),
    doSeedingRegionRebuilding = cms.bool(False),
    maxNSeeds = cms.uint32(100000),
    NavigationSchool = cms.string('SimpleNavigationSchool'),
    TrajectoryBuilder = cms.string('CkfTrajectoryBuilder'),
    TrajectoryCleaner = cms.string('TrajectoryCleanerBySharedHits')
)


hltL1NonIsoSSEleRegioCkfTC = cms.EDProducer("CkfTrackCandidateMaker",
    src = cms.InputTag("hltL1NonIsoSSEleRegPSG"),
    TransientInitialStateEstimatorParameters = cms.PSet(
        propagatorAlongTISE = cms.string('PropagatorWithMaterial'),
        numberMeasurementsForFit = cms.int32(4),
        propagatorOppositeTISE = cms.string('PropagatorWithMaterialOpposite')
    ),
    RedundantSeedCleaner = cms.string('CachingSeedCleanerBySharedInput'),
    cleanTrajectoryAfterInOut = cms.bool(False),
    useHitsSplitting = cms.bool(False),
    doSeedingRegionRebuilding = cms.bool(False),
    maxNSeeds = cms.uint32(100000),
    NavigationSchool = cms.string('SimpleNavigationSchool'),
    TrajectoryBuilder = cms.string('CkfTrajectoryBuilder'),
    TrajectoryCleaner = cms.string('TrajectoryCleanerBySharedHits')
)


hltBSoftMuonL25TagInfos = cms.EDProducer("SoftLepton",
    muonSelection = cms.uint32(0),
    refineJetAxis = cms.uint32(0),
    primaryVertex = cms.InputTag("nominal"),
    leptons = cms.InputTag("hltL2Muons"),
    leptonQualityCut = cms.double(0.0),
    jets = cms.InputTag("hltBSoftMuonL25Jets"),
    leptonDeltaRCut = cms.double(0.4),
    leptonChi2Cut = cms.double(0.0)
)


hltIconeCentral3Regional = cms.EDProducer("FastjetJetProducer",
    Active_Area_Repeats = cms.int32(5),
    doAreaFastjet = cms.bool(False),
    Ghost_EtaMax = cms.double(6.0),
    maxBadHcalCells = cms.uint32(9999999),
    maxRecoveredEcalCells = cms.uint32(9999999),
    jetType = cms.string('CaloJet'),
    doRhoFastjet = cms.bool(False),
    jetAlgorithm = cms.string('IterativeCone'),
    nSigmaPU = cms.double(1.0),
    GhostArea = cms.double(0.01),
    maxBadEcalCells = cms.uint32(9999999),
    doPVCorrection = cms.bool(False),
    maxRecoveredHcalCells = cms.uint32(9999999),
    rParam = cms.double(0.2),
    UseOnlyVertexTracks = cms.bool(False),
    UseOnlyOnePV = cms.bool(False),
    DzTrVtxMax = cms.double(0.0),
    maxProblematicHcalCells = cms.uint32(9999999),
    src = cms.InputTag("hltCaloTowersCentral3Regional"),
    inputEtMin = cms.double(0.5),
    srcPVs = cms.InputTag("offlinePrimaryVertices"),
    jetPtMin = cms.double(1.0),
    radiusPU = cms.double(0.5),
    maxProblematicEcalCells = cms.uint32(9999999),
    doPUOffsetCorr = cms.bool(False),
    inputEMin = cms.double(0.0)
)


hltElectronL1IsoDetaDphi = cms.EDProducer("EgammaHLTElectronDetaDphiProducer",
    electronProducer = cms.InputTag("hltPixelMatchElectronsL1Iso"),
    BSProducer = cms.InputTag("hltOfflineBeamSpot")
)


hltTowerMakerForEcal = cms.EDProducer("CaloTowersCreator",
    EBSumThreshold = cms.double(0.2),
    MomHBDepth = cms.double(0.2),
    EBWeight = cms.double(1.0),
    hfInput = cms.InputTag(""),
    AllowMissingInputs = cms.bool(True),
    UseSymEBTreshold = cms.bool(False),
    HBThreshold = cms.double(0.9),
    EESumThreshold = cms.double(0.45),
    HBGrid = cms.vdouble(),
    MomEEDepth = cms.double(0.0),
    EBThreshold = cms.double(0.09),
    UseEcalRecoveredHits = cms.bool(True),
    MomConstrMethod = cms.int32(1),
    MomHEDepth = cms.double(0.4),
    HcalThreshold = cms.double(-1000.0),
    HF2Weights = cms.vdouble(),
    HOWeights = cms.vdouble(),
    EEGrid = cms.vdouble(),
    HEDWeight = cms.double(1e-99),
    EEWeights = cms.vdouble(),
    EEWeight = cms.double(1.0),
    UseHO = cms.bool(False),
    HBWeights = cms.vdouble(),
    HF1Weight = cms.double(1e-99),
    HF2Grid = cms.vdouble(),
    HEDWeights = cms.vdouble(),
    HESThreshold = cms.double(1.4),
    HF1Grid = cms.vdouble(),
    EBWeights = cms.vdouble(),
    HOWeight = cms.double(1e-99),
    HESWeight = cms.double(1e-99),
    hbheInput = cms.InputTag(""),
    HF2Weight = cms.double(1e-99),
    HF2Threshold = cms.double(1.8),
    HcalAcceptSeverityLevel = cms.uint32(999),
    EEThreshold = cms.double(0.45),
    HOThresholdPlus1 = cms.double(1.1),
    HOThresholdPlus2 = cms.double(1.1),
    HF1Weights = cms.vdouble(),
    hoInput = cms.InputTag(""),
    HF1Threshold = cms.double(1.2),
    HOThresholdMinus1 = cms.double(1.1),
    HESGrid = cms.vdouble(),
    UseEtEETreshold = cms.bool(False),
    HESWeights = cms.vdouble(),
    UseSymEETreshold = cms.bool(False),
    HEDThreshold = cms.double(1.4),
    UseEtEBTreshold = cms.bool(False),
    EcutTower = cms.double(-1000.0),
    HEDGrid = cms.vdouble(),
    UseHcalRecoveredHits = cms.bool(True),
    HOThresholdMinus2 = cms.double(1.1),
    HOThreshold0 = cms.double(1.1),
    ecalInputs = cms.VInputTag("hltEcalRecHitAll:EcalRecHitsEB", "hltEcalRecHitAll:EcalRecHitsEE"),
    MomEBDepth = cms.double(0.3),
    HBWeight = cms.double(1e-99),
    EBGrid = cms.vdouble(),
    HOGrid = cms.vdouble(),
    EcalAcceptSeverityLevel = cms.uint32(1)
)


hltL1NonIsoHLTClusterShape = cms.EDProducer("EgammaHLTClusterShapeProducer",
    recoEcalCandidateProducer = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    ecalRechitEB = cms.InputTag("hltEcalRegionalEgammaRecHit","EcalRecHitsEB"),
    ecalRechitEE = cms.InputTag("hltEcalRegionalEgammaRecHit","EcalRecHitsEE"),
    isIeta = cms.bool(False)
)


hltTowerMakerForJets = cms.EDProducer("CaloTowersCreator",
    EBSumThreshold = cms.double(0.2),
    MomHBDepth = cms.double(0.2),
    EBWeight = cms.double(1.0),
    hfInput = cms.InputTag("hltHfreco"),
    AllowMissingInputs = cms.bool(False),
    UseSymEBTreshold = cms.bool(False),
    HBThreshold = cms.double(0.9),
    EESumThreshold = cms.double(0.45),
    HBGrid = cms.vdouble(),
    MomEEDepth = cms.double(0.0),
    EBThreshold = cms.double(0.09),
    UseEcalRecoveredHits = cms.bool(True),
    MomConstrMethod = cms.int32(1),
    MomHEDepth = cms.double(0.4),
    HcalThreshold = cms.double(-1000.0),
    HF2Weights = cms.vdouble(),
    HOWeights = cms.vdouble(),
    EEGrid = cms.vdouble(),
    HEDWeight = cms.double(1.0),
    EEWeights = cms.vdouble(),
    EEWeight = cms.double(1.0),
    UseHO = cms.bool(False),
    HBWeights = cms.vdouble(),
    HF1Weight = cms.double(1.0),
    HF2Grid = cms.vdouble(),
    HEDWeights = cms.vdouble(),
    HESThreshold = cms.double(1.4),
    HF1Grid = cms.vdouble(),
    EBWeights = cms.vdouble(),
    HOWeight = cms.double(1e-99),
    HESWeight = cms.double(1.0),
    hbheInput = cms.InputTag("hltHbhereco"),
    HF2Weight = cms.double(1.0),
    HF2Threshold = cms.double(1.8),
    HcalAcceptSeverityLevel = cms.uint32(999),
    EEThreshold = cms.double(0.45),
    HOThresholdPlus1 = cms.double(1.1),
    HOThresholdPlus2 = cms.double(1.1),
    HF1Weights = cms.vdouble(),
    hoInput = cms.InputTag("hltHoreco"),
    HF1Threshold = cms.double(1.2),
    HOThresholdMinus1 = cms.double(1.1),
    HESGrid = cms.vdouble(),
    UseEtEETreshold = cms.bool(False),
    HESWeights = cms.vdouble(),
    UseSymEETreshold = cms.bool(False),
    HEDThreshold = cms.double(1.4),
    UseEtEBTreshold = cms.bool(False),
    EcutTower = cms.double(-1000.0),
    HEDGrid = cms.vdouble(),
    UseHcalRecoveredHits = cms.bool(True),
    HOThresholdMinus2 = cms.double(1.1),
    HOThreshold0 = cms.double(1.1),
    ecalInputs = cms.VInputTag("hltEcalRegionalJetsRecHit:EcalRecHitsEB", "hltEcalRegionalJetsRecHit:EcalRecHitsEE"),
    MomEBDepth = cms.double(0.3),
    HBWeight = cms.double(1.0),
    EBGrid = cms.vdouble(),
    HOGrid = cms.vdouble(),
    EcalAcceptSeverityLevel = cms.uint32(1)
)


hltBLifetimeRegionalCkfTrackCandidatesStartup = cms.EDProducer("CkfTrackCandidateMaker",
    src = cms.InputTag("hltBLifetimeRegionalPixelSeedGeneratorStartup"),
    TransientInitialStateEstimatorParameters = cms.PSet(
        propagatorAlongTISE = cms.string('PropagatorWithMaterial'),
        numberMeasurementsForFit = cms.int32(4),
        propagatorOppositeTISE = cms.string('PropagatorWithMaterialOpposite')
    ),
    RedundantSeedCleaner = cms.string('CachingSeedCleanerBySharedInput'),
    cleanTrajectoryAfterInOut = cms.bool(False),
    useHitsSplitting = cms.bool(False),
    doSeedingRegionRebuilding = cms.bool(False),
    maxNSeeds = cms.uint32(100000),
    NavigationSchool = cms.string('SimpleNavigationSchool'),
    TrajectoryBuilder = cms.string('bJetRegionalTrajectoryBuilder'),
    TrajectoryCleaner = cms.string('TrajectoryCleanerBySharedHits')
)


hltIcone5Tau4 = cms.EDProducer("FastjetJetProducer",
    Active_Area_Repeats = cms.int32(5),
    doAreaFastjet = cms.bool(False),
    Ghost_EtaMax = cms.double(6.0),
    maxBadHcalCells = cms.uint32(9999999),
    maxRecoveredEcalCells = cms.uint32(9999999),
    jetType = cms.string('CaloJet'),
    doRhoFastjet = cms.bool(False),
    jetAlgorithm = cms.string('IterativeCone'),
    nSigmaPU = cms.double(1.0),
    GhostArea = cms.double(0.01),
    maxBadEcalCells = cms.uint32(9999999),
    doPVCorrection = cms.bool(False),
    maxRecoveredHcalCells = cms.uint32(9999999),
    rParam = cms.double(0.5),
    UseOnlyVertexTracks = cms.bool(False),
    UseOnlyOnePV = cms.bool(False),
    DzTrVtxMax = cms.double(0.0),
    maxProblematicHcalCells = cms.uint32(9999999),
    src = cms.InputTag("hltCaloTowersTau4"),
    inputEtMin = cms.double(0.5),
    srcPVs = cms.InputTag("offlinePrimaryVertices"),
    jetPtMin = cms.double(1.0),
    radiusPU = cms.double(0.5),
    maxProblematicEcalCells = cms.uint32(9999999),
    doPUOffsetCorr = cms.bool(False),
    inputEMin = cms.double(0.0)
)


hltIcone5Tau2 = cms.EDProducer("FastjetJetProducer",
    Active_Area_Repeats = cms.int32(5),
    doAreaFastjet = cms.bool(False),
    Ghost_EtaMax = cms.double(6.0),
    maxBadHcalCells = cms.uint32(9999999),
    maxRecoveredEcalCells = cms.uint32(9999999),
    jetType = cms.string('CaloJet'),
    doRhoFastjet = cms.bool(False),
    jetAlgorithm = cms.string('IterativeCone'),
    nSigmaPU = cms.double(1.0),
    GhostArea = cms.double(0.01),
    maxBadEcalCells = cms.uint32(9999999),
    doPVCorrection = cms.bool(False),
    maxRecoveredHcalCells = cms.uint32(9999999),
    rParam = cms.double(0.5),
    UseOnlyVertexTracks = cms.bool(False),
    UseOnlyOnePV = cms.bool(False),
    DzTrVtxMax = cms.double(0.0),
    maxProblematicHcalCells = cms.uint32(9999999),
    src = cms.InputTag("hltCaloTowersTau2"),
    inputEtMin = cms.double(0.5),
    srcPVs = cms.InputTag("offlinePrimaryVertices"),
    jetPtMin = cms.double(1.0),
    radiusPU = cms.double(0.5),
    maxProblematicEcalCells = cms.uint32(9999999),
    doPUOffsetCorr = cms.bool(False),
    inputEMin = cms.double(0.0)
)


hltL1NonIsoSiStripElectronPixelSeeds = cms.EDProducer("SiStripElectronSeedProducer",
    endcapSuperClusters = cms.InputTag("hltCorrectedMulti5x5EndcapSuperClustersWithPreshowerL1NonIsolated"),
    barrelSuperClusters = cms.InputTag("hltCorrectedHybridSuperClustersL1NonIsolated")
)


hltOniaPixelTrackCands = cms.EDProducer("ConcreteChargedCandidateProducer",
    src = cms.InputTag("hltOniaPixelTrackSelector"),
    particleType = cms.string('mu-')
)


openHltBPerfMeasL3BJetTags = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('softLeptonByDistance'),
    tagInfos = cms.VInputTag(cms.InputTag("openHltBSoftmuonL3TagInfos"))
)


hltBLifetimeL25Jets = cms.EDProducer("EtMinCaloJetSelector",
    filter = cms.bool(False),
    src = cms.InputTag("hltBLifetimeHighestEtJets"),
    etMin = cms.double(35.0)
)


hltBLifetimeL3AssociatorStartupU = cms.EDProducer("JetTracksAssociatorAtVertex",
    jets = cms.InputTag("hltBLifetimeL3JetsStartupU"),
    tracks = cms.InputTag("hltBLifetimeRegionalCtfWithMaterialTracksStartupU"),
    coneSize = cms.double(0.5)
)


hltL2TauIsolationProducer = cms.EDProducer("L2TauIsolationProducer",
    L2TauJetCollection = cms.InputTag("hltL2TauJets"),
    ECALIsolation = cms.PSet(
        innerCone = cms.double(0.15),
        runAlgorithm = cms.bool(True),
        outerCone = cms.double(0.5)
    ),
    TowerIsolation = cms.PSet(
        innerCone = cms.double(0.2),
        runAlgorithm = cms.bool(True),
        outerCone = cms.double(0.5)
    ),
    EERecHits = cms.InputTag("hltEcalRecHitAll","EcalRecHitsEE"),
    EBRecHits = cms.InputTag("hltEcalRecHitAll","EcalRecHitsEB"),
    ECALClustering = cms.PSet(
        runAlgorithm = cms.bool(True),
        clusterRadius = cms.double(0.08)
    ),
    towerThreshold = cms.double(0.2),
    crystalThreshold = cms.double(0.1)
)


openhltL25TauConeIsolation = cms.EDProducer("ConeIsolation",
    MinimumTransverseMomentumInIsolationRing = cms.double(1.5),
    VariableMinCone = cms.double(0.05),
    BeamSpotProducer = cms.InputTag("hltOfflineBeamSpot"),
    MinimumNumberOfHits = cms.int32(5),
    MinimumTransverseMomentum = cms.double(1.0),
    JetTrackSrc = cms.InputTag("openhltL25TauJetTracksAssociator"),
    VariableMaxCone = cms.double(0.17),
    DeltaZetTrackVertex = cms.double(0.2),
    SignalCone = cms.double(0.15),
    MaximumTransverseImpactParameter = cms.double(300.0),
    MatchingCone = cms.double(0.2),
    useVertex = cms.bool(True),
    MinimumNumberOfPixelHits = cms.int32(2),
    useBeamSpot = cms.bool(True),
    MaximumNumberOfTracksIsolationRing = cms.int32(1),
    UseFixedSizeCone = cms.bool(True),
    VariableConeParameter = cms.double(3.5),
    IsolationCone = cms.double(0.5),
    MinimumTransverseMomentumLeadingTrack = cms.double(5.0),
    MaximumChiSquared = cms.double(100.0),
    vertexSrc = cms.InputTag("hltPixelVertices")
)


hltCkfL1NonIsoLargeWindowTrackCandidates = cms.EDProducer("CkfTrackCandidateMaker",
    src = cms.InputTag("hltL1NonIsoLargeWindowElectronPixelSeeds"),
    TransientInitialStateEstimatorParameters = cms.PSet(
        propagatorAlongTISE = cms.string('PropagatorWithMaterial'),
        numberMeasurementsForFit = cms.int32(4),
        propagatorOppositeTISE = cms.string('PropagatorWithMaterialOpposite')
    ),
    RedundantSeedCleaner = cms.string('CachingSeedCleanerBySharedInput'),
    cleanTrajectoryAfterInOut = cms.bool(False),
    useHitsSplitting = cms.bool(False),
    doSeedingRegionRebuilding = cms.bool(False),
    maxNSeeds = cms.uint32(100000),
    NavigationSchool = cms.string('SimpleNavigationSchool'),
    TrajectoryBuilder = cms.string('CkfTrajectoryBuilder'),
    TrajectoryCleaner = cms.string('TrajectoryCleanerBySharedHits')
)


hltEcalRegionalEgammaFEDs = cms.EDProducer("EcalRawToRecHitRoI",
    JetJobPSet = cms.VPSet(),
    MuJobPSet = cms.PSet(

    ),
    CandJobPSet = cms.VPSet(),
    doES = cms.bool(True),
    type = cms.string('egamma'),
    sourceTag = cms.InputTag("hltEcalRawToRecHitFacility"),
    EmJobPSet = cms.VPSet(cms.PSet(
        regionEtaMargin = cms.double(0.25),
        regionPhiMargin = cms.double(0.4),
        Ptmin = cms.double(5.0),
        Source = cms.InputTag("hltL1extraParticles","Isolated")
    ), 
        cms.PSet(
            regionEtaMargin = cms.double(0.25),
            regionPhiMargin = cms.double(0.4),
            Ptmin = cms.double(5.0),
            Source = cms.InputTag("hltL1extraParticles","NonIsolated")
        )),
    sourceTag_es = cms.InputTag("hltESRawToRecHitFacility")
)


hltCorrectedHybridSuperClustersL1NonIsolated = cms.EDProducer("EgammaHLTRemoveDuplicatedSC",
    L1NonIsoSkimmedCollection = cms.string(''),
    L1IsoSC = cms.InputTag("hltCorrectedHybridSuperClustersL1Isolated"),
    L1NonIsoUskimmedSC = cms.InputTag("hltCorrectedHybridSuperClustersL1NonIsolatedTemp")
)


hltL1IsoElectronsRegionalCTFFinalFitWithMaterial = cms.EDProducer("TrackProducer",
    src = cms.InputTag("hltL1IsoElectronsRegionalCkfTrackCandidates"),
    clusterRemovalInfo = cms.InputTag(""),
    beamSpot = cms.InputTag("hltOfflineBeamSpot"),
    Fitter = cms.string('hltKFFittingSmoother'),
    useHitsSplitting = cms.bool(False),
    alias = cms.untracked.string('hltEgammaRegionalCTFFinalFitWithMaterial'),
    NavigationSchool = cms.string(''),
    TrajectoryInEvent = cms.bool(False),
    TTRHBuilder = cms.string('WithTrackAngle'),
    AlgorithmName = cms.string('undefAlgorithm'),
    Propagator = cms.string('PropagatorWithMaterial')
)


hltL1IsoSSEleTrackIsol = cms.EDProducer("EgammaHLTElectronTrackIsolationProducers",
    egTrkIsoVetoConeSize = cms.double(0.02),
    trackProducer = cms.InputTag("hltL1IsoSSEleRegioCTF"),
    electronProducer = cms.InputTag("hltPixelMatchElectronsL1IsoSS"),
    egTrkIsoConeSize = cms.double(0.2),
    egTrkIsoRSpan = cms.double(999999.0),
    egTrkIsoPtMin = cms.double(1.5),
    egTrkIsoZSpan = cms.double(0.1)
)


hltCorrectedHybridSuperClustersL1NonIsolatedTemp = cms.EDProducer("EgammaSCCorrectionMaker",
    corectedSuperClusterCollection = cms.string(''),
    sigmaElectronicNoise = cms.double(0.03),
    superClusterAlgo = cms.string('Hybrid'),
    etThresh = cms.double(5.0),
    rawSuperClusterProducer = cms.InputTag("hltHybridSuperClustersL1NonIsolated"),
    applyEnergyCorrection = cms.bool(True),
    fix_fCorrPset = cms.PSet(

    ),
    isl_fCorrPset = cms.PSet(

    ),
    VerbosityLevel = cms.string('ERROR'),
    dyn_fCorrPset = cms.PSet(

    ),
    hyb_fCorrPset = cms.PSet(
        brLinearLowThr = cms.double(1.1),
        fBremVec = cms.vdouble(-0.05208, 0.1331, 0.9196, -0.0005735, 1.343),
        brLinearHighThr = cms.double(8.0),
        fEtEtaVec = cms.vdouble(1.0012, -0.5714, 0.0, 0.0, 0.0, 
            0.5549, 12.74, 1.0448, 0.0, 0.0, 
            0.0, 0.0, 8.0, 1.023, -0.00181, 
            0.0, 0.0)
    ),
    recHitProducer = cms.InputTag("hltEcalRegionalEgammaRecHit","EcalRecHitsEB")
)


hltBLifetimeL25BJetTagsStartup = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('trackCounting3D2nd'),
    tagInfos = cms.VInputTag("hltBLifetimeL25TagInfosStartup")
)


hltSiStripRawToClustersFacility = cms.EDProducer("SiStripRawToClusters",
    ProductLabel = cms.InputTag("source"),
    Algorithms = cms.PSet(
        SiStripFedZeroSuppressionMode = cms.uint32(4),
        CommonModeNoiseSubtractionMode = cms.string('Median')
    ),
    Clusterizer = cms.PSet(
        ChannelThreshold = cms.double(2.0),
        MaxSequentialBad = cms.uint32(1),
        MaxSequentialHoles = cms.uint32(0),
        Algorithm = cms.string('ThreeThresholdAlgorithm'),
        MaxAdjacentBad = cms.uint32(0),
        QualityLabel = cms.string(''),
        SeedThreshold = cms.double(3.0),
        ClusterThreshold = cms.double(5.0)
    )
)


hltL3TrackCandidateFromL2 = cms.EDProducer("CkfTrajectoryMaker",
    src = cms.InputTag("hltL3TrajectorySeed"),
    TransientInitialStateEstimatorParameters = cms.PSet(
        propagatorAlongTISE = cms.string('PropagatorWithMaterial'),
        numberMeasurementsForFit = cms.int32(4),
        propagatorOppositeTISE = cms.string('PropagatorWithMaterialOpposite')
    ),
    RedundantSeedCleaner = cms.string('CachingSeedCleanerBySharedInput'),
    cleanTrajectoryAfterInOut = cms.bool(False),
    useHitsSplitting = cms.bool(False),
    doSeedingRegionRebuilding = cms.bool(False),
    maxNSeeds = cms.uint32(100000),
    NavigationSchool = cms.string('SimpleNavigationSchool'),
    TrajectoryBuilder = cms.string('muonCkfTrajectoryBuilder'),
    TrajectoryCleaner = cms.string('TrajectoryCleanerBySharedHits'),
    trackCandidateAlso = cms.bool(True)
)


hltPixelMatchElectronsL1NonIsoLW = cms.EDProducer("EgammaHLTPixelMatchElectronProducers",
    BSProducer = cms.InputTag("hltOfflineBeamSpot"),
    TrackProducer = cms.InputTag("hltCtfL1NonIsoLW")
)


hltCaloTowersTau2Regional = cms.EDProducer("CaloTowerCreatorForTauHLT",
    towers = cms.InputTag("hltTowerMakerForJets"),
    TauId = cms.int32(1),
    TauTrigger = cms.InputTag("hltL1extraParticles","Tau"),
    minimumE = cms.double(0.8),
    UseTowersInCone = cms.double(0.8),
    minimumEt = cms.double(0.5)
)


hltL1IsoLWEleRegPSG = cms.EDProducer("EgammaHLTRegionalPixelSeedGeneratorProducers",
    deltaPhiRegion = cms.double(0.3),
    vertexZ = cms.double(0.0),
    originHalfLength = cms.double(0.5),
    BSProducer = cms.InputTag("hltOfflineBeamSpot"),
    UseZInVertex = cms.bool(True),
    OrderedHitsFactoryPSet = cms.PSet(
        ComponentName = cms.string('StandardHitPairGenerator'),
        SeedingLayers = cms.string('MixedLayerPairs')
    ),
    deltaEtaRegion = cms.double(0.3),
    ptMin = cms.double(1.5),
    candTag = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    candTagEle = cms.InputTag("hltPixelMatchElectronsL1IsoLW"),
    originRadius = cms.double(0.02)
)


openHltBLifetimeL3TagInfosStartup = cms.EDProducer("TrackIPProducer",
    maximumTransverseImpactParameter = cms.double(0.2),
    minimumNumberOfHits = cms.int32(8),
    minimumTransverseMomentum = cms.double(1.0),
    primaryVertex = cms.InputTag("hltPixelVertices"),
    maximumLongitudinalImpactParameter = cms.double(17.0),
    computeProbabilities = cms.bool(False),
    ghostTrackPriorDeltaR = cms.double(0.03),
    jetTracks = cms.InputTag("openHltBLifetimeL3AssociatorStartup"),
    jetDirectionUsingGhostTrack = cms.bool(False),
    minimumNumberOfPixelHits = cms.int32(2),
    jetDirectionUsingTracks = cms.bool(False),
    computeGhostTrack = cms.bool(False),
    useTrackQuality = cms.bool(False),
    maximumChiSquared = cms.double(20.0)
)


hltL1extraParticles = cms.EDProducer("L1ExtraParticlesProd",
    tauJetSource = cms.InputTag("hltGctDigis","tauJets"),
    etHadSource = cms.InputTag("hltGctDigis"),
    etTotalSource = cms.InputTag("hltGctDigis"),
    nonIsolatedEmSource = cms.InputTag("hltGctDigis","nonIsoEm"),
    centralJetSource = cms.InputTag("hltGctDigis","cenJets"),
    etMissSource = cms.InputTag("hltGctDigis"),
    produceMuonParticles = cms.bool(True),
    hfRingEtSumsSource = cms.InputTag("hltGctDigis"),
    forwardJetSource = cms.InputTag("hltGctDigis","forJets"),
    ignoreHtMiss = cms.bool(False),
    htMissSource = cms.InputTag("hltGctDigis"),
    produceCaloParticles = cms.bool(True),
    muonSource = cms.InputTag("hltGtDigis"),
    isolatedEmSource = cms.InputTag("hltGctDigis","isoEm"),
    centralBxOnly = cms.bool(True),
    hfRingBitCountsSource = cms.InputTag("hltGctDigis")
)


hltL1IsoElectronTrackIsol = cms.EDProducer("EgammaHLTElectronTrackIsolationProducers",
    egTrkIsoVetoConeSize = cms.double(0.02),
    trackProducer = cms.InputTag("hltL1IsoElectronsRegionalCTFFinalFitWithMaterial"),
    electronProducer = cms.InputTag("hltPixelMatchElectronsL1Iso"),
    egTrkIsoConeSize = cms.double(0.2),
    egTrkIsoRSpan = cms.double(999999.0),
    egTrkIsoPtMin = cms.double(1.5),
    egTrkIsoZSpan = cms.double(0.1)
)


hltIconeCentral4Regional = cms.EDProducer("FastjetJetProducer",
    Active_Area_Repeats = cms.int32(5),
    doAreaFastjet = cms.bool(False),
    Ghost_EtaMax = cms.double(6.0),
    maxBadHcalCells = cms.uint32(9999999),
    maxRecoveredEcalCells = cms.uint32(9999999),
    jetType = cms.string('CaloJet'),
    doRhoFastjet = cms.bool(False),
    jetAlgorithm = cms.string('IterativeCone'),
    nSigmaPU = cms.double(1.0),
    GhostArea = cms.double(0.01),
    maxBadEcalCells = cms.uint32(9999999),
    doPVCorrection = cms.bool(False),
    maxRecoveredHcalCells = cms.uint32(9999999),
    rParam = cms.double(0.2),
    UseOnlyVertexTracks = cms.bool(False),
    UseOnlyOnePV = cms.bool(False),
    DzTrVtxMax = cms.double(0.0),
    maxProblematicHcalCells = cms.uint32(9999999),
    src = cms.InputTag("hltCaloTowersCentral4Regional"),
    inputEtMin = cms.double(0.5),
    srcPVs = cms.InputTag("offlinePrimaryVertices"),
    jetPtMin = cms.double(1.0),
    radiusPU = cms.double(0.5),
    maxProblematicEcalCells = cms.uint32(9999999),
    doPUOffsetCorr = cms.bool(False),
    inputEMin = cms.double(0.0)
)


hltL2TauJets = cms.EDProducer("L2TauJetsMerger",
    EtMin = cms.double(15.0),
    JetSrc = cms.VInputTag("hltIconeTau1Regional", "hltIconeTau2Regional", "hltIconeTau3Regional", "hltIconeTau4Regional", "hltIconeCentral1Regional", 
        "hltIconeCentral2Regional", "hltIconeCentral3Regional", "hltIconeCentral4Regional")
)


openhltL2TauJets = cms.EDProducer("L2TauJetsMerger",
    EtMin = cms.double(15.0),
    JetSrc = cms.VInputTag("hltIconeTau1Regional", "hltIconeTau2Regional", "hltIconeTau3Regional", "hltIconeTau4Regional", "hltIconeCentral1Regional", 
        "hltIconeCentral2Regional", "hltIconeCentral3Regional", "hltIconeCentral4Regional")
)


hltBSoftmuonHighestEtJets = cms.EDProducer("LargestEtCaloJetSelector",
    maxNumber = cms.uint32(2),
    filter = cms.bool(False),
    src = cms.InputTag("hltIterativeCone5CaloJets")
)


hltL3MuonCandidates = cms.EDProducer("L3MuonCandidateProducer",
    InputObjects = cms.InputTag("hltL3Muons")
)


hltPixelTrackCands = cms.EDProducer("ConcreteChargedCandidateProducer",
    src = cms.InputTag("hltPixelTracks"),
    particleType = cms.string('mu-')
)


hltTriggerSummaryRAW = cms.EDProducer("TriggerSummaryProducerRAW",
    processName = cms.string('@')
)


hltL25TauPixelTracksConeIsolation = cms.EDProducer("ConeIsolation",
    MinimumTransverseMomentumInIsolationRing = cms.double(1.0),
    VariableMinCone = cms.double(0.05),
    BeamSpotProducer = cms.InputTag("hltOfflineBeamSpot"),
    MinimumNumberOfHits = cms.int32(2),
    MinimumTransverseMomentum = cms.double(1.0),
    JetTrackSrc = cms.InputTag("hltL25TauJetPixelTracksAssociator"),
    VariableMaxCone = cms.double(0.17),
    DeltaZetTrackVertex = cms.double(0.2),
    SignalCone = cms.double(0.15),
    MaximumTransverseImpactParameter = cms.double(300.0),
    MatchingCone = cms.double(0.1),
    useVertex = cms.bool(True),
    MinimumNumberOfPixelHits = cms.int32(2),
    useBeamSpot = cms.bool(True),
    MaximumNumberOfTracksIsolationRing = cms.int32(0),
    UseFixedSizeCone = cms.bool(True),
    VariableConeParameter = cms.double(3.5),
    IsolationCone = cms.double(0.5),
    MinimumTransverseMomentumLeadingTrack = cms.double(3.0),
    MaximumChiSquared = cms.double(100.0),
    vertexSrc = cms.InputTag("hltPixelVertices")
)


hltBLifetimeL3BJetTagsStartup = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('trackCounting3D2nd'),
    tagInfos = cms.VInputTag("hltBLifetimeL3TagInfosStartup")
)


hltBLifetimeL3JetsStartup = cms.EDProducer("GetJetsFromHLTobject",
    jets = cms.InputTag("hltBLifetimeL25FilterStartup")
)


hltBLifetimeL3TagInfosStartup = cms.EDProducer("TrackIPProducer",
    maximumTransverseImpactParameter = cms.double(0.2),
    minimumNumberOfHits = cms.int32(8),
    minimumTransverseMomentum = cms.double(1.0),
    primaryVertex = cms.InputTag("hltPixelVertices"),
    maximumLongitudinalImpactParameter = cms.double(17.0),
    computeProbabilities = cms.bool(False),
    ghostTrackPriorDeltaR = cms.double(0.03),
    jetTracks = cms.InputTag("hltBLifetimeL3AssociatorStartup"),
    jetDirectionUsingGhostTrack = cms.bool(False),
    minimumNumberOfPixelHits = cms.int32(2),
    jetDirectionUsingTracks = cms.bool(False),
    computeGhostTrack = cms.bool(False),
    useTrackQuality = cms.bool(False),
    maximumChiSquared = cms.double(20.0)
)


hltTowerMakerForAll = cms.EDProducer("CaloTowersCreator",
    EBSumThreshold = cms.double(0.2),
    MomHBDepth = cms.double(0.2),
    EBWeight = cms.double(1.0),
    hfInput = cms.InputTag("hltHfreco"),
    AllowMissingInputs = cms.bool(False),
    UseSymEBTreshold = cms.bool(False),
    HBThreshold = cms.double(0.9),
    EESumThreshold = cms.double(0.45),
    HBGrid = cms.vdouble(),
    MomEEDepth = cms.double(0.0),
    EBThreshold = cms.double(0.09),
    UseEcalRecoveredHits = cms.bool(True),
    MomConstrMethod = cms.int32(1),
    MomHEDepth = cms.double(0.4),
    HcalThreshold = cms.double(-1000.0),
    HF2Weights = cms.vdouble(),
    HOWeights = cms.vdouble(),
    EEGrid = cms.vdouble(),
    HEDWeight = cms.double(1.0),
    EEWeights = cms.vdouble(),
    EEWeight = cms.double(1.0),
    UseHO = cms.bool(False),
    HBWeights = cms.vdouble(),
    HF1Weight = cms.double(1.0),
    HF2Grid = cms.vdouble(),
    HEDWeights = cms.vdouble(),
    HESThreshold = cms.double(1.4),
    HF1Grid = cms.vdouble(),
    EBWeights = cms.vdouble(),
    HOWeight = cms.double(1e-99),
    HESWeight = cms.double(1.0),
    hbheInput = cms.InputTag("hltHbhereco"),
    HF2Weight = cms.double(1.0),
    HF2Threshold = cms.double(1.8),
    HcalAcceptSeverityLevel = cms.uint32(999),
    EEThreshold = cms.double(0.45),
    HOThresholdPlus1 = cms.double(1.1),
    HOThresholdPlus2 = cms.double(1.1),
    HF1Weights = cms.vdouble(),
    hoInput = cms.InputTag("hltHoreco"),
    HF1Threshold = cms.double(1.2),
    HOThresholdMinus1 = cms.double(1.1),
    HESGrid = cms.vdouble(),
    UseEtEETreshold = cms.bool(False),
    HESWeights = cms.vdouble(),
    UseSymEETreshold = cms.bool(False),
    HEDThreshold = cms.double(1.4),
    UseEtEBTreshold = cms.bool(False),
    EcutTower = cms.double(-1000.0),
    HEDGrid = cms.vdouble(),
    UseHcalRecoveredHits = cms.bool(True),
    HOThresholdMinus2 = cms.double(1.1),
    HOThreshold0 = cms.double(1.1),
    ecalInputs = cms.VInputTag("hltEcalRecHitAll:EcalRecHitsEB", "hltEcalRecHitAll:EcalRecHitsEE"),
    MomEBDepth = cms.double(0.3),
    HBWeight = cms.double(1.0),
    EBGrid = cms.vdouble(),
    HOGrid = cms.vdouble(),
    EcalAcceptSeverityLevel = cms.uint32(1)
)


hltBSoftMuonL25Jets = cms.EDProducer("EtMinCaloJetSelector",
    filter = cms.bool(False),
    src = cms.InputTag("hltSelector4Jets"),
    etMin = cms.double(20.0)
)


hltL1NonIsoLWEleRegioCkfTC = cms.EDProducer("CkfTrackCandidateMaker",
    src = cms.InputTag("hltL1NonIsoLWEleRegPSG"),
    TransientInitialStateEstimatorParameters = cms.PSet(
        propagatorAlongTISE = cms.string('PropagatorWithMaterial'),
        numberMeasurementsForFit = cms.int32(4),
        propagatorOppositeTISE = cms.string('PropagatorWithMaterialOpposite')
    ),
    RedundantSeedCleaner = cms.string('CachingSeedCleanerBySharedInput'),
    cleanTrajectoryAfterInOut = cms.bool(False),
    useHitsSplitting = cms.bool(False),
    doSeedingRegionRebuilding = cms.bool(False),
    maxNSeeds = cms.uint32(100000),
    NavigationSchool = cms.string('SimpleNavigationSchool'),
    TrajectoryBuilder = cms.string('CkfTrajectoryBuilder'),
    TrajectoryCleaner = cms.string('TrajectoryCleanerBySharedHits')
)


hltHITCkfTrackCandidatesHB1E31 = cms.EDProducer("CkfTrackCandidateMaker",
    src = cms.InputTag("hltHITPixelTripletSeedGeneratorHB1E31"),
    TransientInitialStateEstimatorParameters = cms.PSet(
        propagatorAlongTISE = cms.string('PropagatorWithMaterial'),
        numberMeasurementsForFit = cms.int32(4),
        propagatorOppositeTISE = cms.string('PropagatorWithMaterialOpposite')
    ),
    RedundantSeedCleaner = cms.string('CachingSeedCleanerBySharedInput'),
    cleanTrajectoryAfterInOut = cms.bool(False),
    useHitsSplitting = cms.bool(False),
    doSeedingRegionRebuilding = cms.bool(False),
    maxNSeeds = cms.uint32(100000),
    NavigationSchool = cms.string('SimpleNavigationSchool'),
    TrajectoryBuilder = cms.string('CkfTrajectoryBuilder'),
    TrajectoryCleaner = cms.string('TrajectoryCleanerBySharedHits')
)


hltL1NonIsoElectronsRegionalCkfTrackCandidates = cms.EDProducer("CkfTrackCandidateMaker",
    src = cms.InputTag("hltL1NonIsoElectronsRegionalPixelSeedGenerator"),
    TransientInitialStateEstimatorParameters = cms.PSet(
        propagatorAlongTISE = cms.string('PropagatorWithMaterial'),
        numberMeasurementsForFit = cms.int32(4),
        propagatorOppositeTISE = cms.string('PropagatorWithMaterialOpposite')
    ),
    RedundantSeedCleaner = cms.string('CachingSeedCleanerBySharedInput'),
    cleanTrajectoryAfterInOut = cms.bool(False),
    useHitsSplitting = cms.bool(False),
    doSeedingRegionRebuilding = cms.bool(False),
    maxNSeeds = cms.uint32(100000),
    NavigationSchool = cms.string('SimpleNavigationSchool'),
    TrajectoryBuilder = cms.string('CkfTrajectoryBuilder'),
    TrajectoryCleaner = cms.string('TrajectoryCleanerBySharedHits')
)


hltHITCtfWithMaterialTracksHE8E29 = cms.EDProducer("TrackProducer",
    src = cms.InputTag("hltHITCkfTrackCandidatesHE8E29"),
    clusterRemovalInfo = cms.InputTag(""),
    beamSpot = cms.InputTag("hltOfflineBeamSpot"),
    Fitter = cms.string('hltKFFittingSmoother'),
    useHitsSplitting = cms.bool(False),
    alias = cms.untracked.string('hltHITCtfWithMaterialTracksHE1E31'),
    NavigationSchool = cms.string(''),
    TrajectoryInEvent = cms.bool(False),
    TTRHBuilder = cms.string('WithTrackAngle'),
    AlgorithmName = cms.string('undefAlgorithm'),
    Propagator = cms.string('PropagatorWithMaterial')
)


hltL25TauLeadingTrackPtCutSelector = cms.EDProducer("IsolatedTauJetsSelector",
    UseIsolationDiscriminator = cms.bool(False),
    UseInHLTOpen = cms.bool(False),
    MinimumTransverseMomentumLeadingTrack = cms.double(5.0),
    JetSrc = cms.VInputTag("hltL25TauConeIsolation")
)


hltIcone5Central1 = cms.EDProducer("FastjetJetProducer",
    Active_Area_Repeats = cms.int32(5),
    doAreaFastjet = cms.bool(False),
    Ghost_EtaMax = cms.double(6.0),
    maxBadHcalCells = cms.uint32(9999999),
    maxRecoveredEcalCells = cms.uint32(9999999),
    jetType = cms.string('CaloJet'),
    doRhoFastjet = cms.bool(False),
    jetAlgorithm = cms.string('IterativeCone'),
    nSigmaPU = cms.double(1.0),
    GhostArea = cms.double(0.01),
    maxBadEcalCells = cms.uint32(9999999),
    doPVCorrection = cms.bool(False),
    maxRecoveredHcalCells = cms.uint32(9999999),
    rParam = cms.double(0.5),
    UseOnlyVertexTracks = cms.bool(False),
    UseOnlyOnePV = cms.bool(False),
    DzTrVtxMax = cms.double(0.0),
    maxProblematicHcalCells = cms.uint32(9999999),
    src = cms.InputTag("hltCaloTowersCentral1"),
    inputEtMin = cms.double(0.5),
    srcPVs = cms.InputTag("offlinePrimaryVertices"),
    jetPtMin = cms.double(1.0),
    radiusPU = cms.double(0.5),
    maxProblematicEcalCells = cms.uint32(9999999),
    doPUOffsetCorr = cms.bool(False),
    inputEMin = cms.double(0.0)
)


hltL2TauNarrowConeIsolationProducer = cms.EDProducer("L2TauNarrowConeIsolationProducer",
    ECALIsolation = cms.PSet(
        innerCone = cms.double(0.15),
        runAlgorithm = cms.bool(True),
        outerCone = cms.double(0.5)
    ),
    CaloTowers = cms.InputTag("hltTowerMakerForJets"),
    crystalThresholdEB = cms.double(0.15),
    TowerIsolation = cms.PSet(
        innerCone = cms.double(0.2),
        runAlgorithm = cms.bool(True),
        outerCone = cms.double(0.5)
    ),
    crystalThresholdEE = cms.double(0.45),
    EBRecHits = cms.InputTag("hltEcalRegionalJetsRecHit","EcalRecHitsEB"),
    L2TauJetCollection = cms.InputTag("hltL2TauJets"),
    associationRadius = cms.double(0.5),
    EERecHits = cms.InputTag("hltEcalRegionalJetsRecHit","EcalRecHitsEE"),
    ECALClustering = cms.PSet(
        runAlgorithm = cms.bool(True),
        clusterRadius = cms.double(0.08)
    ),
    towerThreshold = cms.double(1.0)
)


hltRpcRecHits = cms.EDProducer("RPCRecHitProducer",
    recAlgo = cms.string('RPCRecHitStandardAlgo'),
    deadvecfile = cms.FileInPath('RecoLocalMuon/RPCRecHit/data/RPCDeadVec.dat'),
    rpcDigiLabel = cms.InputTag("hltMuonRPCDigis"),
    maskvecfile = cms.FileInPath('RecoLocalMuon/RPCRecHit/data/RPCMaskVec.dat'),
    recAlgoConfig = cms.PSet(

    ),
    deadSource = cms.string('File'),
    maskSource = cms.string('File')
)


hltL3Muons = cms.EDProducer("L3MuonProducer",
    ServiceParameters = cms.PSet(
        Propagators = cms.untracked.vstring('SmartPropagatorAny', 
            'SteppingHelixPropagatorAny', 
            'SmartPropagator', 
            'SteppingHelixPropagatorOpposite'),
        RPCLayers = cms.bool(True),
        UseMuonNavigation = cms.untracked.bool(True)
    ),
    MuonCollectionLabel = cms.InputTag("hltL2Muons","UpdatedAtVtx"),
    TrackLoaderParameters = cms.PSet(
        PutTkTrackIntoEvent = cms.untracked.bool(True),
        VertexConstraint = cms.bool(False),
        MuonSeededTracksInstance = cms.untracked.string('L2Seeded'),
        Smoother = cms.string('KFSmootherForMuonTrackLoader'),
        MuonUpdatorAtVertexParameters = cms.PSet(
            MaxChi2 = cms.double(1000000.0),
            Propagator = cms.string('SteppingHelixPropagatorOpposite'),
            BeamSpotPositionErrors = cms.vdouble(0.1, 0.1, 5.3)
        ),
        SmoothTkTrack = cms.untracked.bool(False),
        DoSmoothing = cms.bool(True)
    ),
    L3TrajBuilderParameters = cms.PSet(
        ScaleTECyFactor = cms.double(-1.0),
        GlbRefitterParameters = cms.PSet(
            TrackerSkipSection = cms.int32(-1),
            DoPredictionsOnly = cms.bool(False),
            PropDirForCosmics = cms.bool(False),
            HitThreshold = cms.int32(1),
            MuonHitsOption = cms.int32(1),
            Chi2CutRPC = cms.double(1.0),
            Fitter = cms.string('L3MuKFFitter'),
            DTRecSegmentLabel = cms.InputTag("hltDt4DSegments"),
            TrackerRecHitBuilder = cms.string('WithTrackAngle'),
            MuonRecHitBuilder = cms.string('MuonRecHitBuilder'),
            RefitDirection = cms.string('insideOut'),
            CSCRecSegmentLabel = cms.InputTag("hltCscSegments"),
            Chi2CutCSC = cms.double(150.0),
            Chi2CutDT = cms.double(10.0),
            RefitRPCHits = cms.bool(True),
            SkipStation = cms.int32(-1),
            Propagator = cms.string('SmartPropagatorAny'),
            TrackerSkipSystem = cms.int32(-1)
        ),
        ScaleTECxFactor = cms.double(-1.0),
        TrackerRecHitBuilder = cms.string('WithTrackAngle'),
        MuonRecHitBuilder = cms.string('MuonRecHitBuilder'),
        MuonTrackingRegionBuilder = cms.PSet(
            EtaR_UpperLimit_Par1 = cms.double(0.25),
            Eta_fixed = cms.double(0.2),
            OnDemand = cms.double(-1.0),
            Rescale_Dz = cms.double(3.0),
            Eta_min = cms.double(0.05),
            Rescale_phi = cms.double(3.0),
            EtaR_UpperLimit_Par2 = cms.double(0.15),
            DeltaZ_Region = cms.double(15.9),
            Rescale_eta = cms.double(3.0),
            PhiR_UpperLimit_Par2 = cms.double(0.2),
            vertexCollection = cms.InputTag("pixelVertices"),
            Phi_fixed = cms.double(0.2),
            DeltaR = cms.double(0.2),
            EscapePt = cms.double(1.5),
            UseFixedRegion = cms.bool(False),
            PhiR_UpperLimit_Par1 = cms.double(0.6),
            Phi_min = cms.double(0.05),
            UseVertex = cms.bool(False),
            beamSpot = cms.InputTag("hltOfflineBeamSpot")
        ),
        RefitRPCHits = cms.bool(True),
        TrackTransformer = cms.PSet(
            DoPredictionsOnly = cms.bool(False),
            Fitter = cms.string('L3MuKFFitter'),
            TrackerRecHitBuilder = cms.string('WithTrackAngle'),
            Smoother = cms.string('KFSmootherForMuonTrackLoader'),
            MuonRecHitBuilder = cms.string('MuonRecHitBuilder'),
            RefitDirection = cms.string('insideOut'),
            RefitRPCHits = cms.bool(True),
            Propagator = cms.string('SmartPropagatorAny')
        ),
        GlobalMuonTrackMatcher = cms.PSet(
            Pt_threshold1 = cms.double(0.0),
            DeltaDCut_3 = cms.double(15.0),
            MinP = cms.double(2.5),
            MinPt = cms.double(1.0),
            Chi2Cut_1 = cms.double(50.0),
            Pt_threshold2 = cms.double(999999999.0),
            LocChi2Cut = cms.double(0.001),
            Eta_threshold = cms.double(1.2),
            Quality_3 = cms.double(7.0),
            Quality_2 = cms.double(15.0),
            Chi2Cut_2 = cms.double(50.0),
            Chi2Cut_3 = cms.double(200.0),
            DeltaDCut_1 = cms.double(40.0),
            DeltaRCut_2 = cms.double(0.2),
            DeltaRCut_3 = cms.double(1.0),
            DeltaDCut_2 = cms.double(10.0),
            DeltaRCut_1 = cms.double(0.1),
            Propagator = cms.string('SmartPropagator'),
            Quality_1 = cms.double(20.0)
        ),
        PtCut = cms.double(1.0),
        TrackerPropagator = cms.string('SteppingHelixPropagatorAny'),
        tkTrajLabel = cms.InputTag("hltL3TkTracksFromL2")
    )
)


hltIcone5Central4 = cms.EDProducer("FastjetJetProducer",
    Active_Area_Repeats = cms.int32(5),
    doAreaFastjet = cms.bool(False),
    Ghost_EtaMax = cms.double(6.0),
    maxBadHcalCells = cms.uint32(9999999),
    maxRecoveredEcalCells = cms.uint32(9999999),
    jetType = cms.string('CaloJet'),
    doRhoFastjet = cms.bool(False),
    jetAlgorithm = cms.string('IterativeCone'),
    nSigmaPU = cms.double(1.0),
    GhostArea = cms.double(0.01),
    maxBadEcalCells = cms.uint32(9999999),
    doPVCorrection = cms.bool(False),
    maxRecoveredHcalCells = cms.uint32(9999999),
    rParam = cms.double(0.5),
    UseOnlyVertexTracks = cms.bool(False),
    UseOnlyOnePV = cms.bool(False),
    DzTrVtxMax = cms.double(0.0),
    maxProblematicHcalCells = cms.uint32(9999999),
    src = cms.InputTag("hltCaloTowersCentral4"),
    inputEtMin = cms.double(0.5),
    srcPVs = cms.InputTag("offlinePrimaryVertices"),
    jetPtMin = cms.double(1.0),
    radiusPU = cms.double(0.5),
    maxProblematicEcalCells = cms.uint32(9999999),
    doPUOffsetCorr = cms.bool(False),
    inputEMin = cms.double(0.0)
)


hltSelector4JetsU = cms.EDProducer("LargestEtCaloJetSelector",
    maxNumber = cms.uint32(4),
    filter = cms.bool(False),
    src = cms.InputTag("hltMCJetCorJetIcone5HF07")
)


hltBLifetimeL25JetsStartupU = cms.EDProducer("EtMinCaloJetSelector",
    filter = cms.bool(False),
    src = cms.InputTag("hltSelector4JetsU"),
    etMin = cms.double(15.0)
)


hltBSoftMuonL3BJetTagsByPt = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('softLeptonByPt'),
    tagInfos = cms.VInputTag("hltBSoftMuonL3TagInfos")
)


hltHITIPTCorrectorHB8E29 = cms.EDProducer("IPTCorrector",
    corTracksLabel = cms.InputTag("hltHITCtfWithMaterialTracksHB8E29"),
    filterLabel = cms.InputTag("hltIsolPixelTrackL2FilterHB8E29"),
    associationCone = cms.double(0.2)
)


hltBSoftmuonL25Jets = cms.EDProducer("EtMinCaloJetSelector",
    filter = cms.bool(False),
    src = cms.InputTag("hltBSoftmuonHighestEtJets"),
    etMin = cms.double(20.0)
)


hltHoreco = cms.EDProducer("HcalSimpleReconstructor",
    correctionPhaseNS = cms.double(13.0),
    digiLabel = cms.InputTag("hltHcalDigis"),
    samplesToAdd = cms.int32(4),
    Subdetector = cms.string('HO'),
    correctForTimeslew = cms.bool(True),
    dropZSmarkedPassed = cms.bool(True),
    correctForPhaseContainment = cms.bool(True),
    firstSample = cms.int32(4)
)


hltEcalRegionalPi0EtaFEDs = cms.EDProducer("EcalRawToRecHitRoI",
    JetJobPSet = cms.VPSet(),
    MuJobPSet = cms.PSet(

    ),
    CandJobPSet = cms.VPSet(),
    doES = cms.bool(True),
    type = cms.string('egamma'),
    sourceTag = cms.InputTag("hltEcalRawToRecHitFacility"),
    EmJobPSet = cms.VPSet(cms.PSet(
        regionEtaMargin = cms.double(0.25),
        regionPhiMargin = cms.double(0.4),
        Ptmin = cms.double(2.0),
        Source = cms.InputTag("hltL1extraParticles","Isolated")
    ), 
        cms.PSet(
            regionEtaMargin = cms.double(0.25),
            regionPhiMargin = cms.double(0.4),
            Ptmin = cms.double(2.0),
            Source = cms.InputTag("hltL1extraParticles","NonIsolated")
        )),
    sourceTag_es = cms.InputTag("hltESRawToRecHitFacility")
)


simCsctfDigis = cms.EDProducer("CSCTFCandidateProducer",
    CSCTrackProducer = cms.untracked.InputTag("simCsctfTrackDigis"),
    MuonSorter = cms.PSet(
        MaxBX = cms.int32(9),
        MinBX = cms.int32(3)
    )
)


openHltBLifetimeL3Associator = cms.EDProducer("JetTracksAssociatorAtVertex",
    jets = cms.InputTag("hltIterativeCone5CaloJets"),
    tracks = cms.InputTag("openHltBLifetimeRegionalCtfWithMaterialTracks"),
    coneSize = cms.double(0.5)
)


hltOniaCkfTrackCandidates = cms.EDProducer("CkfTrackCandidateMaker",
    src = cms.InputTag("hltOniaSeeds"),
    TransientInitialStateEstimatorParameters = cms.PSet(
        propagatorAlongTISE = cms.string('PropagatorWithMaterial'),
        numberMeasurementsForFit = cms.int32(4),
        propagatorOppositeTISE = cms.string('PropagatorWithMaterialOpposite')
    ),
    RedundantSeedCleaner = cms.string('CachingSeedCleanerBySharedInput'),
    cleanTrajectoryAfterInOut = cms.bool(False),
    useHitsSplitting = cms.bool(False),
    doSeedingRegionRebuilding = cms.bool(False),
    maxNSeeds = cms.uint32(100000),
    NavigationSchool = cms.string('SimpleNavigationSchool'),
    TrajectoryBuilder = cms.string('oniaTrajectoryBuilder'),
    TrajectoryCleaner = cms.string('TrajectoryCleanerBySharedHits')
)


hltStoppedHSCPTowerMakerForAll = cms.EDProducer("CaloTowersCreator",
    EBSumThreshold = cms.double(0.2),
    MomHBDepth = cms.double(0.2),
    EBWeight = cms.double(1.0),
    hfInput = cms.InputTag(""),
    AllowMissingInputs = cms.bool(True),
    UseSymEBTreshold = cms.bool(False),
    HBThreshold = cms.double(0.9),
    EESumThreshold = cms.double(0.45),
    HBGrid = cms.vdouble(),
    MomEEDepth = cms.double(0.0),
    EBThreshold = cms.double(0.09),
    UseEcalRecoveredHits = cms.bool(True),
    MomConstrMethod = cms.int32(1),
    MomHEDepth = cms.double(0.4),
    HcalThreshold = cms.double(-1000.0),
    HF2Weights = cms.vdouble(),
    HOWeights = cms.vdouble(),
    EEGrid = cms.vdouble(),
    HEDWeight = cms.double(1.0),
    EEWeights = cms.vdouble(),
    EEWeight = cms.double(1.0),
    UseHO = cms.bool(False),
    HBWeights = cms.vdouble(),
    HF1Weight = cms.double(1.0),
    HF2Grid = cms.vdouble(),
    HEDWeights = cms.vdouble(),
    HESThreshold = cms.double(1.4),
    HF1Grid = cms.vdouble(),
    EBWeights = cms.vdouble(),
    HOWeight = cms.double(1e-99),
    HESWeight = cms.double(1.0),
    hbheInput = cms.InputTag("hltHbhereco"),
    HF2Weight = cms.double(1.0),
    HF2Threshold = cms.double(1.8),
    HcalAcceptSeverityLevel = cms.uint32(999),
    EEThreshold = cms.double(0.45),
    HOThresholdPlus1 = cms.double(1.1),
    HOThresholdPlus2 = cms.double(1.1),
    HF1Weights = cms.vdouble(),
    hoInput = cms.InputTag(""),
    HF1Threshold = cms.double(1.2),
    HOThresholdMinus1 = cms.double(1.1),
    HESGrid = cms.vdouble(),
    UseEtEETreshold = cms.bool(False),
    HESWeights = cms.vdouble(),
    UseSymEETreshold = cms.bool(False),
    HEDThreshold = cms.double(1.4),
    UseEtEBTreshold = cms.bool(False),
    EcutTower = cms.double(-1000.0),
    HEDGrid = cms.vdouble(),
    UseHcalRecoveredHits = cms.bool(True),
    HOThresholdMinus2 = cms.double(1.1),
    HOThreshold0 = cms.double(1.1),
    ecalInputs = cms.VInputTag(),
    MomEBDepth = cms.double(0.3),
    HBWeight = cms.double(1.0),
    EBGrid = cms.vdouble(),
    HOGrid = cms.vdouble(),
    EcalAcceptSeverityLevel = cms.uint32(1)
)


hltMCJetCorJetIcone5 = cms.EDProducer("CaloJetCorrectionProducer",
    alias = cms.untracked.string('MCJetCorJetIcone5'),
    correctors = cms.vstring('MCJetCorrectorIcone5'),
    src = cms.InputTag("hltIterativeCone5CaloJets")
)


hltL2MuonSeeds = cms.EDProducer("L2MuonSeedGenerator",
    ServiceParameters = cms.PSet(
        Propagators = cms.untracked.vstring('SteppingHelixPropagatorAny'),
        RPCLayers = cms.bool(True),
        UseMuonNavigation = cms.untracked.bool(True)
    ),
    InputObjects = cms.InputTag("hltL1extraParticles"),
    L1MaxEta = cms.double(2.5),
    L1MinPt = cms.double(0.0),
    L1MinQuality = cms.uint32(1),
    GMTReadoutCollection = cms.InputTag("hltGtDigis"),
    Propagator = cms.string('SteppingHelixPropagatorAny')
)


hltCtfL1NonIsoSS = cms.EDProducer("TrackProducer",
    src = cms.InputTag("hltCkfL1NonIsoSSTC"),
    clusterRemovalInfo = cms.InputTag(""),
    beamSpot = cms.InputTag("hltOfflineBeamSpot"),
    Fitter = cms.string('hltKFFittingSmoother'),
    useHitsSplitting = cms.bool(False),
    alias = cms.untracked.string('hltCtfL1NonIsoSS'),
    NavigationSchool = cms.string(''),
    TrajectoryInEvent = cms.bool(True),
    TTRHBuilder = cms.string('WithTrackAngle'),
    AlgorithmName = cms.string('undefAlgorithm'),
    Propagator = cms.string('PropagatorWithMaterial')
)


hltL1IsolatedPhotonHcalIsol = cms.EDProducer("EgammaHLTHcalIsolationProducersRegional",
    hfRecHitProducer = cms.InputTag("hltHfreco"),
    recoEcalCandidateProducer = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    egHcalIsoPtMin = cms.double(0.0),
    egHcalIsoConeSize = cms.double(0.3),
    hbRecHitProducer = cms.InputTag("hltHbhereco")
)


TauOpenHLT = cms.EDProducer("HLTTauProducer",
    L25TrackIsoJets = cms.InputTag("openhltL25TauConeIsolation"),
    L3TrackIsoJets = cms.InputTag("openhltL25TauConeIsolation"),
    SignalCone = cms.double(0.15),
    MatchingCone = cms.double(0.2),
    L2EcalIsoJets = cms.InputTag("openhltL2TauIsolationProducer"),
    IsolationCone = cms.double(0.5),
    MinPtTracks = cms.double(1.0)
)


hltBLifetimeL25Associator = cms.EDProducer("JetTracksAssociatorAtVertex",
    jets = cms.InputTag("hltBLifetimeL25Jets"),
    tracks = cms.InputTag("hltPixelTracks"),
    coneSize = cms.double(0.5)
)


hltCaloTowersCentral4Regional = cms.EDProducer("CaloTowerCreatorForTauHLT",
    towers = cms.InputTag("hltTowerMakerForJets"),
    TauId = cms.int32(3),
    TauTrigger = cms.InputTag("hltL1extraParticles","Central"),
    minimumE = cms.double(0.8),
    UseTowersInCone = cms.double(0.8),
    minimumEt = cms.double(0.5)
)


hltCorrectedIslandEndcapSuperClustersHI = cms.EDProducer("EgammaSCCorrectionMaker",
    corectedSuperClusterCollection = cms.string(''),
    sigmaElectronicNoise = cms.double(0.15),
    superClusterAlgo = cms.string('Island'),
    etThresh = cms.double(0.0),
    rawSuperClusterProducer = cms.InputTag("hltIslandSuperClustersHI","islandEndcapSuperClustersHI"),
    applyEnergyCorrection = cms.bool(True),
    fix_fCorrPset = cms.PSet(

    ),
    isl_fCorrPset = cms.PSet(
        brLinearLowThr = cms.double(0.0),
        fEtEtaVec = cms.vdouble(0.0),
        brLinearHighThr = cms.double(0.0),
        fBremVec = cms.vdouble(0.0)
    ),
    VerbosityLevel = cms.string('ERROR'),
    dyn_fCorrPset = cms.PSet(

    ),
    hyb_fCorrPset = cms.PSet(

    ),
    recHitProducer = cms.InputTag("hltEcalRecHitAll","EcalRecHitsEE")
)


hltIterativeCone5CaloJets = cms.EDProducer("FastjetJetProducer",
    Active_Area_Repeats = cms.int32(5),
    doAreaFastjet = cms.bool(False),
    Ghost_EtaMax = cms.double(6.0),
    maxBadHcalCells = cms.uint32(9999999),
    maxRecoveredEcalCells = cms.uint32(9999999),
    jetType = cms.string('CaloJet'),
    doRhoFastjet = cms.bool(False),
    jetAlgorithm = cms.string('IterativeCone'),
    nSigmaPU = cms.double(1.0),
    GhostArea = cms.double(0.01),
    maxBadEcalCells = cms.uint32(9999999),
    doPVCorrection = cms.bool(False),
    maxRecoveredHcalCells = cms.uint32(9999999),
    rParam = cms.double(0.5),
    UseOnlyVertexTracks = cms.bool(False),
    UseOnlyOnePV = cms.bool(False),
    DzTrVtxMax = cms.double(0.0),
    maxProblematicHcalCells = cms.uint32(9999999),
    src = cms.InputTag("hltTowerMakerForAll"),
    inputEtMin = cms.double(0.5),
    srcPVs = cms.InputTag("offlinePrimaryVertices"),
    jetPtMin = cms.double(1.0),
    radiusPU = cms.double(0.5),
    maxProblematicEcalCells = cms.uint32(9999999),
    doPUOffsetCorr = cms.bool(False),
    inputEMin = cms.double(0.0)
)


hltGtDigis = cms.EDProducer("L1GlobalTriggerRawToDigi",
    DaqGtFedId = cms.untracked.int32(813),
    Verbosity = cms.untracked.int32(0),
    UnpackBxInEvent = cms.int32(5),
    ActiveBoardsMask = cms.uint32(65535),
    DaqGtInputTag = cms.InputTag("source")
)


hltMuonCSCDigis = cms.EDProducer("CSCDCCUnpacker",
    PrintEventNumber = cms.untracked.bool(False),
    UseExaminer = cms.bool(True),
    ErrorMask = cms.uint32(0),
    InputObjects = cms.InputTag("source"),
    UseFormatStatus = cms.bool(True),
    ExaminerMask = cms.uint32(535557110),
    UnpackStatusDigis = cms.bool(False),
    UseSelectiveUnpacking = cms.bool(True)
)


hltCorrectedIslandBarrelSuperClustersHI = cms.EDProducer("EgammaSCCorrectionMaker",
    corectedSuperClusterCollection = cms.string(''),
    sigmaElectronicNoise = cms.double(0.03),
    superClusterAlgo = cms.string('Island'),
    etThresh = cms.double(0.0),
    rawSuperClusterProducer = cms.InputTag("hltIslandSuperClustersHI","islandBarrelSuperClustersHI"),
    applyEnergyCorrection = cms.bool(True),
    fix_fCorrPset = cms.PSet(

    ),
    isl_fCorrPset = cms.PSet(
        brLinearLowThr = cms.double(0.0),
        fEtEtaVec = cms.vdouble(0.0),
        brLinearHighThr = cms.double(0.0),
        fBremVec = cms.vdouble(0.0)
    ),
    VerbosityLevel = cms.string('ERROR'),
    dyn_fCorrPset = cms.PSet(

    ),
    hyb_fCorrPset = cms.PSet(

    ),
    recHitProducer = cms.InputTag("hltEcalRecHitAll","EcalRecHitsEB")
)


hltBSoftmuonL3BJetTags = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('softLeptonByPt'),
    tagInfos = cms.VInputTag("hltBSoftmuonL3TagInfos")
)


hltDTTFUnpacker = cms.EDProducer("DTTFFEDReader",
    DTTF_FED_Source = cms.InputTag("source")
)


hltPixelMatchElectronsL1NonIso = cms.EDProducer("EgammaHLTPixelMatchElectronProducers",
    BSProducer = cms.InputTag("hltOfflineBeamSpot"),
    TrackProducer = cms.InputTag("hltCtfL1NonIsoWithMaterialTracks")
)


openhltL25TauCtfWithMaterialTracks = cms.EDProducer("TrackProducer",
    src = cms.InputTag("openhltL25TauCkfTrackCandidates"),
    clusterRemovalInfo = cms.InputTag(""),
    beamSpot = cms.InputTag("hltOfflineBeamSpot"),
    Fitter = cms.string('FittingSmootherRK'),
    useHitsSplitting = cms.bool(False),
    alias = cms.untracked.string('ctfWithMaterialTracks'),
    NavigationSchool = cms.string(''),
    TrajectoryInEvent = cms.bool(True),
    TTRHBuilder = cms.string('WithTrackAngle'),
    AlgorithmName = cms.string('undefAlgorithm'),
    Propagator = cms.string('RungeKuttaTrackerPropagator')
)


hltL1IsoSiStripElectronPixelSeeds = cms.EDProducer("SiStripElectronSeedProducer",
    endcapSuperClusters = cms.InputTag("hltCorrectedMulti5x5EndcapSuperClustersWithPreshowerL1Isolated"),
    barrelSuperClusters = cms.InputTag("hltCorrectedHybridSuperClustersL1Isolated")
)


openHltBLifetimeL3TagInfos = cms.EDProducer("TrackIPProducer",
    maximumTransverseImpactParameter = cms.double(0.2),
    minimumNumberOfHits = cms.int32(8),
    minimumTransverseMomentum = cms.double(1.0),
    primaryVertex = cms.InputTag("hltPixelVertices"),
    maximumLongitudinalImpactParameter = cms.double(17.0),
    computeProbabilities = cms.bool(False),
    ghostTrackPriorDeltaR = cms.double(0.03),
    jetTracks = cms.InputTag("openHltBLifetimeL3Associator"),
    jetDirectionUsingGhostTrack = cms.bool(False),
    minimumNumberOfPixelHits = cms.int32(2),
    jetDirectionUsingTracks = cms.bool(False),
    computeGhostTrack = cms.bool(False),
    useTrackQuality = cms.bool(False),
    maximumChiSquared = cms.double(5.0)
)


openHltBLifetimeL3AssociatorStartup = cms.EDProducer("JetTracksAssociatorAtVertex",
    jets = cms.InputTag("hltIterativeCone5CaloJets"),
    tracks = cms.InputTag("openHltBLifetimeRegionalCtfWithMaterialTracksStartup"),
    coneSize = cms.double(0.5)
)


hltGctDigis = cms.EDProducer("GctRawToDigi",
    gctFedId = cms.int32(745),
    unpackSharedRegions = cms.bool(False),
    inputLabel = cms.InputTag("source"),
    hltMode = cms.bool(False),
    unpackerVersion = cms.uint32(0)
)


simCsctfTrackDigis = cms.EDProducer("CSCTFTrackProducer",
    isTMB07 = cms.bool(True),
    DTproducer = cms.untracked.InputTag("simDtTriggerPrimitiveDigis"),
    SectorReceiverInput = cms.untracked.InputTag("simCscTriggerPrimitiveDigis","MPCSORTED"),
    useDT = cms.bool(True),
    SectorProcessor = cms.PSet(
        MaxBX = cms.int32(9),
        MinBX = cms.int32(3),
        maxdphi13_accp = cms.uint32(64),
        SRLUT = cms.PSet(
            UseMiniLUTs = cms.untracked.bool(True),
            ReadLUTs = cms.untracked.bool(False),
            LUTPath = cms.untracked.string('./'),
            Binary = cms.untracked.bool(False)
        ),
        mindphip_halo = cms.uint32(128),
        AllowALCTonly = cms.bool(False),
        PTLUT = cms.PSet(
            LowQualityFlag = cms.untracked.uint32(4),
            ReadPtLUT = cms.untracked.bool(False),
            PtMethod = cms.untracked.uint32(1)
        ),
        QualityEnableME1b = cms.uint32(65535),
        QualityEnableME1c = cms.uint32(65535),
        QualityEnableME1a = cms.uint32(65535),
        QualityEnableME1f = cms.uint32(65535),
        QualityEnableME1d = cms.uint32(65535),
        QualityEnableME1e = cms.uint32(65535),
        QualityEnableME3a = cms.uint32(65535),
        QualityEnableME3b = cms.uint32(65535),
        QualityEnableME3c = cms.uint32(65535),
        maxdphi112_accp = cms.uint32(64),
        mindeta112_accp = cms.uint32(18),
        maxdeta113_accp = cms.uint32(30),
        EtaMin = cms.vuint32(22, 22, 14, 14, 14, 
            14, 10, 22),
        maxdphi113_accp = cms.uint32(64),
        maxdeta13_accp = cms.uint32(27),
        mindeta12_accp = cms.uint32(7),
        mbbPhiOff = cms.uint32(1982),
        maxdphi12_accp = cms.uint32(64),
        trigger_on_ME1a = cms.bool(False),
        mbaPhiOff = cms.uint32(0),
        EtaMax = cms.vuint32(127, 127, 127, 127, 127, 
            24, 24, 127),
        CoreLatency = cms.uint32(7),
        PreTrigger = cms.uint32(2),
        mindetap = cms.uint32(8),
        trigger_on_MB1d = cms.bool(False),
        maxdeta112_accp = cms.uint32(25),
        mindeta13_accp = cms.uint32(13),
        run_core = cms.bool(True),
        rescaleSinglesPhi = cms.bool(False),
        trigger_on_MB1a = cms.bool(False),
        singlesTrackPt = cms.uint32(31),
        QualityEnableME2c = cms.uint32(65535),
        EtaWindows = cms.vuint32(6, 6, 6, 6, 6, 
            6, 6),
        QualityEnableME2a = cms.uint32(65535),
        trigger_on_ME4 = cms.bool(False),
        initializeFromPSet = cms.bool(True),
        maxdeta12_accp = cms.uint32(16),
        QualityEnableME4a = cms.uint32(65535),
        QualityEnableME4c = cms.uint32(65535),
        QualityEnableME4b = cms.uint32(65535),
        trigger_on_ME1b = cms.bool(False),
        AllowCLCTonly = cms.bool(False),
        widePhi = cms.uint32(1),
        mindetap_halo = cms.uint32(8),
        curvedp = cms.uint32(200),
        singlesTrackOutput = cms.uint32(1),
        straightp = cms.uint32(60),
        mindphip = cms.uint32(128),
        mindeta113_accp = cms.uint32(28),
        kill_fiber = cms.uint32(0),
        trigger_on_ME2 = cms.bool(False),
        BXAdepth = cms.uint32(2),
        trigger_on_ME3 = cms.bool(False),
        QualityEnableME2b = cms.uint32(65535)
    )
)


hltCkfL1NonIsoLWTC = cms.EDProducer("CkfTrackCandidateMaker",
    src = cms.InputTag("hltL1NonIsoLargeWindowElectronPixelSeeds"),
    TransientInitialStateEstimatorParameters = cms.PSet(
        propagatorAlongTISE = cms.string('PropagatorWithMaterial'),
        numberMeasurementsForFit = cms.int32(4),
        propagatorOppositeTISE = cms.string('PropagatorWithMaterialOpposite')
    ),
    RedundantSeedCleaner = cms.string('CachingSeedCleanerBySharedInput'),
    cleanTrajectoryAfterInOut = cms.bool(False),
    useHitsSplitting = cms.bool(False),
    doSeedingRegionRebuilding = cms.bool(False),
    maxNSeeds = cms.uint32(100000),
    NavigationSchool = cms.string('SimpleNavigationSchool'),
    TrajectoryBuilder = cms.string('CkfTrajectoryBuilder'),
    TrajectoryCleaner = cms.string('TrajectoryCleanerBySharedHits')
)


hltBLifetimeRegionalCkfTrackCandidatesStartupU = cms.EDProducer("CkfTrackCandidateMaker",
    src = cms.InputTag("hltBLifetimeRegionalPixelSeedGeneratorStartupU"),
    TransientInitialStateEstimatorParameters = cms.PSet(
        propagatorAlongTISE = cms.string('PropagatorWithMaterial'),
        numberMeasurementsForFit = cms.int32(4),
        propagatorOppositeTISE = cms.string('PropagatorWithMaterialOpposite')
    ),
    RedundantSeedCleaner = cms.string('CachingSeedCleanerBySharedInput'),
    cleanTrajectoryAfterInOut = cms.bool(False),
    useHitsSplitting = cms.bool(False),
    doSeedingRegionRebuilding = cms.bool(False),
    maxNSeeds = cms.uint32(100000),
    NavigationSchool = cms.string('SimpleNavigationSchool'),
    TrajectoryBuilder = cms.string('bJetRegionalTrajectoryBuilder'),
    TrajectoryCleaner = cms.string('TrajectoryCleanerBySharedHits')
)


openHltBSoftmuonL25BJetTags = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('softLeptonByDistance'),
    tagInfos = cms.VInputTag(cms.InputTag("openHltBSoftmuonL25TagInfos"))
)


hltHITPixelTripletSeedGeneratorHB1E31 = cms.EDProducer("SeedGeneratorFromRegionHitsEDProducer",
    SeedComparitorPSet = cms.PSet(
        ComponentName = cms.string('none')
    ),
    ClusterCheckPSet = cms.PSet(
        MaxNumberOfCosmicClusters = cms.uint32(50000),
        ClusterCollectionLabel = cms.InputTag("siStripClusters"),
        doClusterCheck = cms.bool(False)
    ),
    RegionFactoryPSet = cms.PSet(
        ComponentName = cms.string('HITRegionalPixelSeedGenerator'),
        RegionPSet = cms.PSet(
            deltaEtaTrackRegion = cms.double(0.05),
            useL1Jets = cms.bool(False),
            deltaPhiTrackRegion = cms.double(0.05),
            isoTrackSrc = cms.InputTag("hltIsolPixelTrackL2FilterHB1E31"),
            precise = cms.bool(True),
            deltaEtaL1JetRegion = cms.double(0.3),
            useTracks = cms.bool(False),
            originRadius = cms.double(0.2),
            trackSrc = cms.InputTag("hltHITPixelTracksHB"),
            useIsoTracks = cms.bool(True),
            l1tjetSrc = cms.InputTag("hltl1extraParticles","Tau"),
            deltaPhiL1JetRegion = cms.double(0.3),
            vertexSrc = cms.string('hltHITPixelVerticesHB'),
            fixedReg = cms.bool(False),
            etaCenter = cms.double(0.0),
            phiCenter = cms.double(0.0),
            originZPos = cms.double(0.0),
            originHalfLength = cms.double(0.2),
            ptMin = cms.double(1.0)
        )
    ),
    SeedCreatorPSet = cms.PSet(
        ComponentName = cms.string('SeedFromConsecutiveHitsCreator'),
        propagator = cms.string('PropagatorWithMaterial')
    ),
    OrderedHitsFactoryPSet = cms.PSet(
        ComponentName = cms.string('StandardHitTripletGenerator'),
        SeedingLayers = cms.string('PixelLayerTriplets'),
        GeneratorPSet = cms.PSet(
            useBending = cms.bool(True),
            useFixedPreFiltering = cms.bool(False),
            ComponentName = cms.string('PixelTripletHLTGenerator'),
            extraHitRPhitolerance = cms.double(0.06),
            useMultScattering = cms.bool(True),
            phiPreFiltering = cms.double(0.3),
            extraHitRZtolerance = cms.double(0.06)
        )
    ),
    TTRHBuilder = cms.string('WithTrackAngle')
)


hltDt1DRecHits = cms.EDProducer("DTRecHitProducer",
    debug = cms.untracked.bool(False),
    recAlgo = cms.string('DTLinearDriftFromDBAlgo'),
    dtDigiLabel = cms.InputTag("hltMuonDTDigis"),
    recAlgoConfig = cms.PSet(
        minTime = cms.double(-3.0),
        debug = cms.untracked.bool(False),
        tTrigModeConfig = cms.PSet(
            vPropWire = cms.double(24.4),
            doTOFCorrection = cms.bool(True),
            tofCorrType = cms.int32(1),
            wirePropCorrType = cms.int32(1),
            doWirePropCorrection = cms.bool(True),
            doT0Correction = cms.bool(True),
            debug = cms.untracked.bool(False)
        ),
        maxTime = cms.double(420.0),
        tTrigMode = cms.string('DTTTrigSyncFromDB')
    )
)


simGtDigis = cms.EDProducer("L1GlobalTrigger",
    ProduceL1GtObjectMapRecord = cms.bool(True),
    EmulateBxInEvent = cms.int32(3),
    ProduceL1GtEvmRecord = cms.bool(True),
    ReadTechnicalTriggerRecords = cms.bool(True),
    RecordLength = cms.vint32(3, 0),
    ProduceL1GtDaqRecord = cms.bool(True),
    GmtInputTag = cms.InputTag("simGmtDigis"),
    AlternativeNrBxBoardEvm = cms.uint32(0),
    TechnicalTriggersInputTags = cms.VInputTag(cms.InputTag("simBscDigis"), cms.InputTag("simRpcTechTrigDigis")),
    CastorInputTag = cms.InputTag("castorL1Digis"),
    GctInputTag = cms.InputTag("simGctDigis"),
    AlternativeNrBxBoardDaq = cms.uint32(0),
    WritePsbL1GtDaqRecord = cms.bool(True),
    BstLengthBytes = cms.int32(-1)
)


hltL1HLTDoubleLooseIsoTau15Trk5L3JetsMatch = cms.EDProducer("L1HLTJetsMatching",
    JetSrc = cms.InputTag("hltL3TauIsolationSelector"),
    EtMin = cms.double(15.0),
    L1TauTrigger = cms.InputTag("hltL1sDoubleLooseIsoTau15Trk5")
)


hltCaloTowersCentral2Regional = cms.EDProducer("CaloTowerCreatorForTauHLT",
    towers = cms.InputTag("hltTowerMakerForJets"),
    TauId = cms.int32(1),
    TauTrigger = cms.InputTag("hltL1extraParticles","Central"),
    minimumE = cms.double(0.8),
    UseTowersInCone = cms.double(0.8),
    minimumEt = cms.double(0.5)
)


hltCtfL1IsoLargeWindowWithMaterialTracks = cms.EDProducer("TrackProducer",
    src = cms.InputTag("hltCkfL1IsoLargeWindowTrackCandidates"),
    clusterRemovalInfo = cms.InputTag(""),
    beamSpot = cms.InputTag("hltOfflineBeamSpot"),
    Fitter = cms.string('hltKFFittingSmoother'),
    useHitsSplitting = cms.bool(False),
    alias = cms.untracked.string('ctfWithMaterialTracks'),
    NavigationSchool = cms.string(''),
    TrajectoryInEvent = cms.bool(True),
    TTRHBuilder = cms.string('WithTrackAngle'),
    AlgorithmName = cms.string('undefAlgorithm'),
    Propagator = cms.string('PropagatorWithMaterial')
)


hltCaloTowersCentral1Regional = cms.EDProducer("CaloTowerCreatorForTauHLT",
    towers = cms.InputTag("hltTowerMakerForJets"),
    TauId = cms.int32(0),
    TauTrigger = cms.InputTag("hltL1extraParticles","Central"),
    minimumE = cms.double(0.8),
    UseTowersInCone = cms.double(0.8),
    minimumEt = cms.double(0.5)
)


openHltBLifetimeRegionalCtfWithMaterialTracks = cms.EDProducer("TrackProducer",
    src = cms.InputTag("openHltBLifetimeRegionalCkfTrackCandidates"),
    clusterRemovalInfo = cms.InputTag(""),
    beamSpot = cms.InputTag("hltOfflineBeamSpot"),
    Fitter = cms.string('FittingSmootherRK'),
    useHitsSplitting = cms.bool(False),
    alias = cms.untracked.string('ctfWithMaterialTracks'),
    NavigationSchool = cms.string(''),
    TrajectoryInEvent = cms.bool(True),
    TTRHBuilder = cms.string('WithTrackAngle'),
    AlgorithmName = cms.string('undefAlgorithm'),
    Propagator = cms.string('RungeKuttaTrackerPropagator')
)


hltCorrectedMulti5x5EndcapSuperClustersWithPreshowerL1Isolated = cms.EDProducer("EgammaSCCorrectionMaker",
    corectedSuperClusterCollection = cms.string(''),
    sigmaElectronicNoise = cms.double(0.15),
    superClusterAlgo = cms.string('Multi5x5'),
    etThresh = cms.double(5.0),
    rawSuperClusterProducer = cms.InputTag("hltMulti5x5EndcapSuperClustersWithPreshowerL1Isolated"),
    applyEnergyCorrection = cms.bool(True),
    fix_fCorrPset = cms.PSet(
        brLinearLowThr = cms.double(0.6),
        fBremVec = cms.vdouble(-0.04163, 0.08552, 0.95048, -0.002308, 1.077),
        brLinearHighThr = cms.double(6.0),
        fEtEtaVec = cms.vdouble(0.9746, -6.512, 0.0, 0.0, 0.02771, 
            4.983, 0.0, 0.0, -0.007288, -0.9446, 
            0.0, 0.0, 0.0, 0.0, 0.0, 
            1.0, 1.0)
    ),
    isl_fCorrPset = cms.PSet(

    ),
    VerbosityLevel = cms.string('ERROR'),
    dyn_fCorrPset = cms.PSet(

    ),
    hyb_fCorrPset = cms.PSet(

    ),
    recHitProducer = cms.InputTag("hltEcalRegionalEgammaRecHit","EcalRecHitsEE")
)


openHltBLifetimeRegionalCkfTrackCandidatesStartup = cms.EDProducer("CkfTrackCandidateMaker",
    src = cms.InputTag("openHltBLifetimeRegionalPixelSeedGeneratorStartup"),
    TransientInitialStateEstimatorParameters = cms.PSet(
        propagatorAlongTISE = cms.string('PropagatorWithMaterial'),
        numberMeasurementsForFit = cms.int32(4),
        propagatorOppositeTISE = cms.string('PropagatorWithMaterialOpposite')
    ),
    RedundantSeedCleaner = cms.string('CachingSeedCleanerBySharedInput'),
    cleanTrajectoryAfterInOut = cms.bool(False),
    useHitsSplitting = cms.bool(False),
    doSeedingRegionRebuilding = cms.bool(False),
    maxNSeeds = cms.uint32(100000),
    NavigationSchool = cms.string('SimpleNavigationSchool'),
    TrajectoryBuilder = cms.string('bJetRegionalTrajectoryBuilder'),
    TrajectoryCleaner = cms.string('TrajectoryCleanerBySharedHits')
)


hltOniaPixelTrackSelector = cms.EDProducer("QuarkoniaTrackSelector",
    MinTrackPt = cms.double(0.0),
    muonCandidates = cms.InputTag("hltL3MuonCandidates"),
    MaxTrackEta = cms.double(999.0),
    tracks = cms.InputTag("hltPixelTracks"),
    MaxMasses = cms.vdouble(5, 12),
    checkCharge = cms.bool(False),
    MinMasses = cms.vdouble(2, 8),
    MinTrackP = cms.double(2.5)
)


hltIconeTau4Regional = cms.EDProducer("FastjetJetProducer",
    Active_Area_Repeats = cms.int32(5),
    doAreaFastjet = cms.bool(False),
    Ghost_EtaMax = cms.double(6.0),
    maxBadHcalCells = cms.uint32(9999999),
    maxRecoveredEcalCells = cms.uint32(9999999),
    jetType = cms.string('CaloJet'),
    doRhoFastjet = cms.bool(False),
    jetAlgorithm = cms.string('IterativeCone'),
    nSigmaPU = cms.double(1.0),
    GhostArea = cms.double(0.01),
    maxBadEcalCells = cms.uint32(9999999),
    doPVCorrection = cms.bool(False),
    maxRecoveredHcalCells = cms.uint32(9999999),
    rParam = cms.double(0.2),
    UseOnlyVertexTracks = cms.bool(False),
    UseOnlyOnePV = cms.bool(False),
    DzTrVtxMax = cms.double(0.0),
    maxProblematicHcalCells = cms.uint32(9999999),
    src = cms.InputTag("hltCaloTowersTau4Regional"),
    inputEtMin = cms.double(0.5),
    srcPVs = cms.InputTag("offlinePrimaryVertices"),
    jetPtMin = cms.double(1.0),
    radiusPU = cms.double(0.5),
    maxProblematicEcalCells = cms.uint32(9999999),
    doPUOffsetCorr = cms.bool(False),
    inputEMin = cms.double(0.0)
)


hltL3TrajectorySeed = cms.EDProducer("TSGFromL2Muon",
    tkSeedGenerator = cms.string('TSGForRoadSearchOI'),
    TSGFromCombinedHits = cms.PSet(

    ),
    ServiceParameters = cms.PSet(
        Propagators = cms.untracked.vstring('SteppingHelixPropagatorOpposite', 
            'SteppingHelixPropagatorAlong'),
        RPCLayers = cms.bool(True),
        UseMuonNavigation = cms.untracked.bool(True)
    ),
    TSGFromPropagation = cms.PSet(

    ),
    TSGFromPixelTriplets = cms.PSet(

    ),
    MuonCollectionLabel = cms.InputTag("hltL2Muons","UpdatedAtVtx"),
    TSGForRoadSearchOI = cms.PSet(
        propagatorCompatibleName = cms.string('SteppingHelixPropagatorOpposite'),
        option = cms.uint32(3),
        ComponentName = cms.string('TSGForRoadSearch'),
        errorMatrixPset = cms.PSet(
            action = cms.string('use'),
            atIP = cms.bool(True),
            errorMatrixValuesPSet = cms.PSet(
                pf3_V12 = cms.PSet(
                    action = cms.string('scale'),
                    values = cms.vdouble(1.0, 1.0, 1.0, 1.0, 1.0, 
                        1.0, 1.0, 1.0, 1.0, 1.0, 
                        1.0, 1.0)
                ),
                pf3_V13 = cms.PSet(
                    action = cms.string('scale'),
                    values = cms.vdouble(1.0, 1.0, 1.0, 1.0, 1.0, 
                        1.0, 1.0, 1.0, 1.0, 1.0, 
                        1.0, 1.0)
                ),
                pf3_V11 = cms.PSet(
                    action = cms.string('scale'),
                    values = cms.vdouble(3.0, 3.0, 3.0, 5.0, 4.0, 
                        5.0, 10.0, 7.0, 10.0, 10.0, 
                        10.0, 10.0)
                ),
                pf3_V14 = cms.PSet(
                    action = cms.string('scale'),
                    values = cms.vdouble(1.0, 1.0, 1.0, 1.0, 1.0, 
                        1.0, 1.0, 1.0, 1.0, 1.0, 
                        1.0, 1.0)
                ),
                yAxis = cms.vdouble(0.0, 1.0, 1.4, 10.0),
                pf3_V34 = cms.PSet(
                    action = cms.string('scale'),
                    values = cms.vdouble(1.0, 1.0, 1.0, 1.0, 1.0, 
                        1.0, 1.0, 1.0, 1.0, 1.0, 
                        1.0, 1.0)
                ),
                pf3_V15 = cms.PSet(
                    action = cms.string('scale'),
                    values = cms.vdouble(1.0, 1.0, 1.0, 1.0, 1.0, 
                        1.0, 1.0, 1.0, 1.0, 1.0, 
                        1.0, 1.0)
                ),
                pf3_V35 = cms.PSet(
                    action = cms.string('scale'),
                    values = cms.vdouble(1.0, 1.0, 1.0, 1.0, 1.0, 
                        1.0, 1.0, 1.0, 1.0, 1.0, 
                        1.0, 1.0)
                ),
                pf3_V45 = cms.PSet(
                    action = cms.string('scale'),
                    values = cms.vdouble(1.0, 1.0, 1.0, 1.0, 1.0, 
                        1.0, 1.0, 1.0, 1.0, 1.0, 
                        1.0, 1.0)
                ),
                pf3_V44 = cms.PSet(
                    action = cms.string('scale'),
                    values = cms.vdouble(3.0, 3.0, 3.0, 5.0, 4.0, 
                        5.0, 10.0, 7.0, 10.0, 10.0, 
                        10.0, 10.0)
                ),
                xAxis = cms.vdouble(0.0, 13.0, 30.0, 70.0, 1000.0),
                pf3_V23 = cms.PSet(
                    action = cms.string('scale'),
                    values = cms.vdouble(1.0, 1.0, 1.0, 1.0, 1.0, 
                        1.0, 1.0, 1.0, 1.0, 1.0, 
                        1.0, 1.0)
                ),
                pf3_V22 = cms.PSet(
                    action = cms.string('scale'),
                    values = cms.vdouble(3.0, 3.0, 3.0, 5.0, 4.0, 
                        5.0, 10.0, 7.0, 10.0, 10.0, 
                        10.0, 10.0)
                ),
                pf3_V55 = cms.PSet(
                    action = cms.string('scale'),
                    values = cms.vdouble(3.0, 3.0, 3.0, 5.0, 4.0, 
                        5.0, 10.0, 7.0, 10.0, 10.0, 
                        10.0, 10.0)
                ),
                zAxis = cms.vdouble(-3.14159, 3.14159),
                pf3_V33 = cms.PSet(
                    action = cms.string('scale'),
                    values = cms.vdouble(3.0, 3.0, 3.0, 5.0, 4.0, 
                        5.0, 10.0, 7.0, 10.0, 10.0, 
                        10.0, 10.0)
                ),
                pf3_V25 = cms.PSet(
                    action = cms.string('scale'),
                    values = cms.vdouble(1.0, 1.0, 1.0, 1.0, 1.0, 
                        1.0, 1.0, 1.0, 1.0, 1.0, 
                        1.0, 1.0)
                ),
                pf3_V24 = cms.PSet(
                    action = cms.string('scale'),
                    values = cms.vdouble(1.0, 1.0, 1.0, 1.0, 1.0, 
                        1.0, 1.0, 1.0, 1.0, 1.0, 
                        1.0, 1.0)
                )
            )
        ),
        propagatorName = cms.string('SteppingHelixPropagatorAlong'),
        manySeeds = cms.bool(False),
        copyMuonRecHit = cms.bool(False),
        maxChi2 = cms.double(40.0)
    ),
    MuonTrackingRegionBuilder = cms.PSet(

    ),
    TSGFromMixedPairs = cms.PSet(

    ),
    PCut = cms.double(2.5),
    TrackerSeedCleaner = cms.PSet(

    ),
    PtCut = cms.double(1.0),
    TSGForRoadSearchIOpxl = cms.PSet(

    ),
    TSGFromPixelPairs = cms.PSet(

    )
)


openHltBLifetimeRegionalCtfWithMaterialTracksStartup = cms.EDProducer("TrackProducer",
    src = cms.InputTag("openHltBLifetimeRegionalCkfTrackCandidatesStartup"),
    clusterRemovalInfo = cms.InputTag(""),
    beamSpot = cms.InputTag("hltOfflineBeamSpot"),
    Fitter = cms.string('FittingSmootherRK'),
    useHitsSplitting = cms.bool(False),
    alias = cms.untracked.string('ctfWithMaterialTracks'),
    NavigationSchool = cms.string(''),
    TrajectoryInEvent = cms.bool(True),
    TTRHBuilder = cms.string('WithTrackAngle'),
    AlgorithmName = cms.string('undefAlgorithm'),
    Propagator = cms.string('RungeKuttaTrackerPropagator')
)


hltL1NonIsoElectronTrackIsol = cms.EDProducer("EgammaHLTElectronTrackIsolationProducers",
    egTrkIsoVetoConeSize = cms.double(0.02),
    trackProducer = cms.InputTag("hltL1NonIsoElectronsRegionalCTFFinalFitWithMaterial"),
    electronProducer = cms.InputTag("hltPixelMatchElectronsL1NonIso"),
    egTrkIsoConeSize = cms.double(0.2),
    egTrkIsoRSpan = cms.double(999999.0),
    egTrkIsoPtMin = cms.double(1.5),
    egTrkIsoZSpan = cms.double(0.1)
)


hltEcalDigis = cms.EDProducer("EcalRawToDigi",
    orderedDCCIdList = cms.vint32(1, 2, 3, 4, 5, 
        6, 7, 8, 9, 10, 
        11, 12, 13, 14, 15, 
        16, 17, 18, 19, 20, 
        21, 22, 23, 24, 25, 
        26, 27, 28, 29, 30, 
        31, 32, 33, 34, 35, 
        36, 37, 38, 39, 40, 
        41, 42, 43, 44, 45, 
        46, 47, 48, 49, 50, 
        51, 52, 53, 54),
    FedLabel = cms.InputTag("listfeds"),
    tccUnpacking = cms.bool(False),
    srpUnpacking = cms.bool(False),
    syncCheck = cms.bool(False),
    silentMode = cms.untracked.bool(True),
    feUnpacking = cms.bool(True),
    orderedFedList = cms.vint32(601, 602, 603, 604, 605, 
        606, 607, 608, 609, 610, 
        611, 612, 613, 614, 615, 
        616, 617, 618, 619, 620, 
        621, 622, 623, 624, 625, 
        626, 627, 628, 629, 630, 
        631, 632, 633, 634, 635, 
        636, 637, 638, 639, 640, 
        641, 642, 643, 644, 645, 
        646, 647, 648, 649, 650, 
        651, 652, 653, 654),
    eventPut = cms.bool(True),
    InputLabel = cms.string('rawDataCollector'),
    numbXtalTSamples = cms.int32(10),
    feIdCheck = cms.bool(True),
    FEDs = cms.vint32(601, 602, 603, 604, 605, 
        606, 607, 608, 609, 610, 
        611, 612, 613, 614, 615, 
        616, 617, 618, 619, 620, 
        621, 622, 623, 624, 625, 
        626, 627, 628, 629, 630, 
        631, 632, 633, 634, 635, 
        636, 637, 638, 639, 640, 
        641, 642, 643, 644, 645, 
        646, 647, 648, 649, 650, 
        651, 652, 653, 654),
    DoRegional = cms.bool(False),
    forceToKeepFRData = cms.bool(False),
    headerUnpacking = cms.bool(True),
    numbTriggerTSamples = cms.int32(1),
    memUnpacking = cms.bool(False)
)


hltCtfL1NonIsoWithMaterialTracks = cms.EDProducer("TrackProducer",
    src = cms.InputTag("hltCkfL1NonIsoTrackCandidates"),
    clusterRemovalInfo = cms.InputTag(""),
    beamSpot = cms.InputTag("hltOfflineBeamSpot"),
    Fitter = cms.string('hltKFFittingSmoother'),
    useHitsSplitting = cms.bool(False),
    alias = cms.untracked.string('ctfWithMaterialTracks'),
    NavigationSchool = cms.string(''),
    TrajectoryInEvent = cms.bool(False),
    TTRHBuilder = cms.string('WithTrackAngle'),
    AlgorithmName = cms.string('undefAlgorithm'),
    Propagator = cms.string('PropagatorWithMaterial')
)


hltL3TauCtfWithMaterialTracks = cms.EDProducer("TrackProducer",
    src = cms.InputTag("hltL3TauCkfTrackCandidates"),
    clusterRemovalInfo = cms.InputTag(""),
    beamSpot = cms.InputTag("hltOfflineBeamSpot"),
    Fitter = cms.string('FittingSmootherRK'),
    useHitsSplitting = cms.bool(False),
    alias = cms.untracked.string('ctfWithMaterialTracks'),
    NavigationSchool = cms.string(''),
    TrajectoryInEvent = cms.bool(True),
    TTRHBuilder = cms.string('WithTrackAngle'),
    AlgorithmName = cms.string('undefAlgorithm'),
    Propagator = cms.string('RungeKuttaTrackerPropagator')
)


hltBLifetimeRegionalCtfWithMaterialTracksStartup = cms.EDProducer("TrackProducer",
    src = cms.InputTag("hltBLifetimeRegionalCkfTrackCandidatesStartup"),
    clusterRemovalInfo = cms.InputTag(""),
    beamSpot = cms.InputTag("hltOfflineBeamSpot"),
    Fitter = cms.string('FittingSmootherRK'),
    useHitsSplitting = cms.bool(False),
    alias = cms.untracked.string('ctfWithMaterialTracks'),
    NavigationSchool = cms.string(''),
    TrajectoryInEvent = cms.bool(True),
    TTRHBuilder = cms.string('WithTrackAngle'),
    AlgorithmName = cms.string('undefAlgorithm'),
    Propagator = cms.string('RungeKuttaTrackerPropagator')
)


hltIconeTau1Regional = cms.EDProducer("FastjetJetProducer",
    Active_Area_Repeats = cms.int32(5),
    doAreaFastjet = cms.bool(False),
    Ghost_EtaMax = cms.double(6.0),
    maxBadHcalCells = cms.uint32(9999999),
    maxRecoveredEcalCells = cms.uint32(9999999),
    jetType = cms.string('CaloJet'),
    doRhoFastjet = cms.bool(False),
    jetAlgorithm = cms.string('IterativeCone'),
    nSigmaPU = cms.double(1.0),
    GhostArea = cms.double(0.01),
    maxBadEcalCells = cms.uint32(9999999),
    doPVCorrection = cms.bool(False),
    maxRecoveredHcalCells = cms.uint32(9999999),
    rParam = cms.double(0.2),
    UseOnlyVertexTracks = cms.bool(False),
    UseOnlyOnePV = cms.bool(False),
    DzTrVtxMax = cms.double(0.0),
    maxProblematicHcalCells = cms.uint32(9999999),
    src = cms.InputTag("hltCaloTowersTau1Regional"),
    inputEtMin = cms.double(0.5),
    srcPVs = cms.InputTag("offlinePrimaryVertices"),
    jetPtMin = cms.double(1.0),
    radiusPU = cms.double(0.5),
    maxProblematicEcalCells = cms.uint32(9999999),
    doPUOffsetCorr = cms.bool(False),
    inputEMin = cms.double(0.0)
)


openHltBLifetimeRegionalPixelSeedGenerator = cms.EDProducer("SeedGeneratorFromRegionHitsEDProducer",
    SeedComparitorPSet = cms.PSet(
        ComponentName = cms.string('none')
    ),
    ClusterCheckPSet = cms.PSet(
        MaxNumberOfCosmicClusters = cms.uint32(50000),
        ClusterCollectionLabel = cms.InputTag("hltSiStripClusters"),
        doClusterCheck = cms.bool(False)
    ),
    RegionFactoryPSet = cms.PSet(
        ComponentName = cms.string('TauRegionalPixelSeedGenerator'),
        RegionPSet = cms.PSet(
            precise = cms.bool(True),
            deltaPhiRegion = cms.double(0.25),
            originHalfLength = cms.double(0.2),
            originRadius = cms.double(0.2),
            deltaEtaRegion = cms.double(0.25),
            ptMin = cms.double(1.0),
            JetSrc = cms.InputTag("hltIterativeCone5CaloJets"),
            originZPos = cms.double(0.0),
            vertexSrc = cms.InputTag("hltPixelVertices")
        )
    ),
    SeedCreatorPSet = cms.PSet(
        ComponentName = cms.string('SeedFromConsecutiveHitsCreator'),
        propagator = cms.string('PropagatorWithMaterial')
    ),
    OrderedHitsFactoryPSet = cms.PSet(
        ComponentName = cms.string('StandardHitPairGenerator'),
        SeedingLayers = cms.string('PixelLayerPairs')
    ),
    TTRHBuilder = cms.string('WithTrackAngle')
)


hltEcalFixedAlphaBetaFitUncalibRecHit = cms.EDProducer("EcalUncalibRecHitProducer",
    EEdigiCollection = cms.InputTag("hltEcalDigis","eeDigis"),
    betaEE = cms.double(1.37),
    betaEB = cms.double(1.7),
    EBdigiCollection = cms.InputTag("hltEcalDigis","ebDigis"),
    EEhitCollection = cms.string('EcalUncalibRecHitsEE'),
    AlphaBetaFilename = cms.untracked.string('NOFILE'),
    alphaEE = cms.double(1.63),
    MinAmplEndcap = cms.double(14.0),
    MinAmplBarrel = cms.double(8.0),
    algo = cms.string('EcalUncalibRecHitWorkerFixedAlphaBetaFit'),
    alphaEB = cms.double(1.2),
    UseDynamicPedestal = cms.bool(True),
    EBhitCollection = cms.string('EcalUncalibRecHitsEB')
)


hltIcone5Tau3 = cms.EDProducer("FastjetJetProducer",
    Active_Area_Repeats = cms.int32(5),
    doAreaFastjet = cms.bool(False),
    Ghost_EtaMax = cms.double(6.0),
    maxBadHcalCells = cms.uint32(9999999),
    maxRecoveredEcalCells = cms.uint32(9999999),
    jetType = cms.string('CaloJet'),
    doRhoFastjet = cms.bool(False),
    jetAlgorithm = cms.string('IterativeCone'),
    nSigmaPU = cms.double(1.0),
    GhostArea = cms.double(0.01),
    maxBadEcalCells = cms.uint32(9999999),
    doPVCorrection = cms.bool(False),
    maxRecoveredHcalCells = cms.uint32(9999999),
    rParam = cms.double(0.5),
    UseOnlyVertexTracks = cms.bool(False),
    UseOnlyOnePV = cms.bool(False),
    DzTrVtxMax = cms.double(0.0),
    maxProblematicHcalCells = cms.uint32(9999999),
    src = cms.InputTag("hltCaloTowersTau3"),
    inputEtMin = cms.double(0.5),
    srcPVs = cms.InputTag("offlinePrimaryVertices"),
    jetPtMin = cms.double(1.0),
    radiusPU = cms.double(0.5),
    maxProblematicEcalCells = cms.uint32(9999999),
    doPUOffsetCorr = cms.bool(False),
    inputEMin = cms.double(0.0)
)


hltESRegionalPi0EtaRecHit = cms.EDProducer("EcalRawToRecHitProducer",
    splitOutput = cms.bool(False),
    rechitCollection = cms.string('EcalRecHitsES'),
    EErechitCollection = cms.string(''),
    EBrechitCollection = cms.string(''),
    lazyGetterTag = cms.InputTag("hltESRawToRecHitFacility"),
    sourceTag = cms.InputTag("hltEcalRegionalPi0EtaFEDs","es")
)


hltSiPixelClusters = cms.EDProducer("SiPixelClusterProducer",
    src = cms.InputTag("hltSiPixelDigis"),
    ChannelThreshold = cms.int32(1000),
    SplitClusters = cms.bool(False),
    MissCalibrate = cms.untracked.bool(True),
    VCaltoElectronGain = cms.int32(65),
    VCaltoElectronOffset = cms.int32(-414),
    payloadType = cms.string('HLT'),
    SeedThreshold = cms.int32(1000),
    ClusterThreshold = cms.double(4000.0)
)


hltL1IsoLWEleTrackIsol = cms.EDProducer("EgammaHLTElectronTrackIsolationProducers",
    egTrkIsoVetoConeSize = cms.double(0.02),
    trackProducer = cms.InputTag("hltL1IsoLWEleRegioCTF"),
    electronProducer = cms.InputTag("hltPixelMatchElectronsL1IsoLW"),
    egTrkIsoConeSize = cms.double(0.2),
    egTrkIsoRSpan = cms.double(999999.0),
    egTrkIsoPtMin = cms.double(1.5),
    egTrkIsoZSpan = cms.double(0.1)
)


hltL1GtObjectMap = cms.EDProducer("L1GlobalTrigger",
    CastorInputTag = cms.InputTag("castorL1Digis"),
    EmulateBxInEvent = cms.int32(1),
    ProduceL1GtDaqRecord = cms.bool(False),
    ReadTechnicalTriggerRecords = cms.bool(True),
    RecordLength = cms.vint32(3, 0),
    ProduceL1GtEvmRecord = cms.bool(False),
    GmtInputTag = cms.InputTag("hltGtDigis"),
    AlternativeNrBxBoardEvm = cms.uint32(0),
    TechnicalTriggersInputTags = cms.VInputTag("simBscDigis"),
    ProduceL1GtObjectMapRecord = cms.bool(True),
    GctInputTag = cms.InputTag("hltGctDigis"),
    AlternativeNrBxBoardDaq = cms.uint32(0),
    WritePsbL1GtDaqRecord = cms.bool(False),
    BstLengthBytes = cms.int32(-1)
)


hltHITPixelTracksHB = cms.EDProducer("PixelTrackProducer",
    FilterPSet = cms.PSet(
        nSigmaTipMaxTolerance = cms.double(0.0),
        chi2 = cms.double(1000.0),
        ComponentName = cms.string('PixelTrackFilterByKinematics'),
        nSigmaInvPtTolerance = cms.double(0.0),
        ptMin = cms.double(0.7),
        tipMax = cms.double(1.0)
    ),
    useFilterWithES = cms.bool(False),
    FitterPSet = cms.PSet(
        ComponentName = cms.string('PixelFitterByConformalMappingAndLine'),
        TTRHBuilder = cms.string('TTRHBuilderPixelOnly'),
        fixImpactParameter = cms.double(0.0)
    ),
    RegionFactoryPSet = cms.PSet(
        ComponentName = cms.string('GlobalRegionProducerFromBeamSpot'),
        RegionPSet = cms.PSet(
            precise = cms.bool(True),
            originRadius = cms.double(0.0015),
            nSigmaZ = cms.double(3.0),
            ptMin = cms.double(0.7),
            beamSpot = cms.InputTag("hltOfflineBeamSpot")
        )
    ),
    CleanerPSet = cms.PSet(
        ComponentName = cms.string('PixelTrackCleanerBySharedHits')
    ),
    OrderedHitsFactoryPSet = cms.PSet(
        ComponentName = cms.string('StandardHitTripletGenerator'),
        SeedingLayers = cms.string('PixelLayerTripletsHITHB'),
        GeneratorPSet = cms.PSet(
            useBending = cms.bool(True),
            useFixedPreFiltering = cms.bool(False),
            phiPreFiltering = cms.double(0.3),
            extraHitRPhitolerance = cms.double(0.06),
            useMultScattering = cms.bool(True),
            ComponentName = cms.string('PixelTripletHLTGenerator'),
            extraHitRZtolerance = cms.double(0.06)
        )
    )
)


hltIcone5Tau1 = cms.EDProducer("FastjetJetProducer",
    Active_Area_Repeats = cms.int32(5),
    doAreaFastjet = cms.bool(False),
    Ghost_EtaMax = cms.double(6.0),
    maxBadHcalCells = cms.uint32(9999999),
    maxRecoveredEcalCells = cms.uint32(9999999),
    jetType = cms.string('CaloJet'),
    doRhoFastjet = cms.bool(False),
    jetAlgorithm = cms.string('IterativeCone'),
    nSigmaPU = cms.double(1.0),
    GhostArea = cms.double(0.01),
    maxBadEcalCells = cms.uint32(9999999),
    doPVCorrection = cms.bool(False),
    maxRecoveredHcalCells = cms.uint32(9999999),
    rParam = cms.double(0.5),
    UseOnlyVertexTracks = cms.bool(False),
    UseOnlyOnePV = cms.bool(False),
    DzTrVtxMax = cms.double(0.0),
    maxProblematicHcalCells = cms.uint32(9999999),
    src = cms.InputTag("hltCaloTowersTau1"),
    inputEtMin = cms.double(0.5),
    srcPVs = cms.InputTag("offlinePrimaryVertices"),
    jetPtMin = cms.double(1.0),
    radiusPU = cms.double(0.5),
    maxProblematicEcalCells = cms.uint32(9999999),
    doPUOffsetCorr = cms.bool(False),
    inputEMin = cms.double(0.0)
)


hltEcalRegionalMuonsFEDs = cms.EDProducer("EcalRawToRecHitRoI",
    JetJobPSet = cms.VPSet(),
    MuJobPSet = cms.PSet(

    ),
    CandJobPSet = cms.VPSet(cms.PSet(
        bePrecise = cms.bool(False),
        propagatorNameToBePrecise = cms.string(''),
        epsilon = cms.double(0.01),
        regionPhiMargin = cms.double(0.3),
        cType = cms.string('chargedcandidate'),
        Source = cms.InputTag("hltL2MuonCandidates"),
        Ptmin = cms.double(0.0),
        regionEtaMargin = cms.double(0.3)
    )),
    doES = cms.bool(False),
    type = cms.string('candidate'),
    sourceTag = cms.InputTag("hltEcalRawToRecHitFacility"),
    EmJobPSet = cms.VPSet(),
    sourceTag_es = cms.InputTag("NotNeededoESfalse")
)


hltEcalRegionalRestFEDs = cms.EDProducer("EcalRawToRecHitRoI",
    JetJobPSet = cms.VPSet(),
    MuJobPSet = cms.PSet(

    ),
    CandJobPSet = cms.VPSet(),
    doES = cms.bool(False),
    type = cms.string('all'),
    sourceTag = cms.InputTag("hltEcalRawToRecHitFacility"),
    EmJobPSet = cms.VPSet(),
    sourceTag_es = cms.InputTag("NotNeededoESfalse")
)


hltL1IsolatedPhotonEcalIsol = cms.EDProducer("EgammaHLTEcalRecIsolationProducer",
    etMinBarrel = cms.double(-9999.0),
    etMinEndcap = cms.double(-9999.0),
    tryBoth = cms.bool(True),
    eMinEndcap = cms.double(0.3),
    ecalEndcapRecHitCollection = cms.InputTag("EcalRecHitsEE"),
    intRadiusEndcap = cms.double(0.07),
    ecalEndcapRecHitProducer = cms.InputTag("hltEcalRegionalEgammaRecHit"),
    jurassicWidth = cms.double(0.02),
    ecalBarrelRecHitProducer = cms.InputTag("hltEcalRegionalEgammaRecHit"),
    extRadius = cms.double(0.4),
    useIsolEt = cms.bool(True),
    recoEcalCandidateProducer = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    ecalBarrelRecHitCollection = cms.InputTag("EcalRecHitsEB"),
    intRadiusBarrel = cms.double(0.045),
    subtract = cms.bool(False),
    eMinBarrel = cms.double(0.08)
)


hltMCJetCorJetIcone5HF07 = cms.EDProducer("CaloJetCorrectionProducer",
    alias = cms.untracked.string('MCJetCorJetIcone5'),
    correctors = cms.vstring('MCJetCorrectorIcone5HF07'),
    src = cms.InputTag("hltIterativeCone5CaloJets")
)


hltMuonDTDigis = cms.EDProducer("DTUnpackingModule",
    dataType = cms.string('DDU'),
    fedbyType = cms.untracked.bool(False),
    inputLabel = cms.untracked.InputTag("source"),
    readOutParameters = cms.PSet(
        debug = cms.untracked.bool(False),
        rosParameters = cms.PSet(
            writeSC = cms.untracked.bool(True),
            readingDDU = cms.untracked.bool(True),
            performDataIntegrityMonitor = cms.untracked.bool(False),
            readDDUIDfromDDU = cms.untracked.bool(True),
            debug = cms.untracked.bool(False),
            localDAQ = cms.untracked.bool(False)
        ),
        localDAQ = cms.untracked.bool(False),
        performDataIntegrityMonitor = cms.untracked.bool(False)
    )
)


hltBSoftMuonL25BJetTagsUByDR = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('softLeptonByDistance'),
    tagInfos = cms.VInputTag("hltBSoftMuonL25TagInfosU")
)


hltEcalRecHitAB = cms.EDProducer("EcalRecHitProducer",
    recoverEEVFE = cms.bool(False),
    EErechitCollection = cms.string('EcalRecHitsEE'),
    recoverEBIsolatedChannels = cms.bool(False),
    recoverEBVFE = cms.bool(False),
    laserCorrection = cms.bool(False),
    killDeadChannels = cms.bool(True),
    EEuncalibRecHitCollection = cms.InputTag("hltEcalFixedAlphaBetaFitUncalibRecHit","EcalUncalibRecHitsEE"),
    ebFEToBeRecovered = cms.InputTag(""),
    recoverEEIsolatedChannels = cms.bool(False),
    eeDetIdToBeRecovered = cms.InputTag(""),
    recoverEBFE = cms.bool(False),
    eeFEToBeRecovered = cms.InputTag(""),
    ebDetIdToBeRecovered = cms.InputTag(""),
    singleChannelRecoveryThreshold = cms.double(8.0),
    ChannelStatusToBeExcluded = cms.vint32(1, 2, 3, 4, 8, 
        9, 10, 11, 12, 13, 
        14, 78, 142),
    EBrechitCollection = cms.string('EcalRecHitsEB'),
    triggerPrimitiveDigiCollection = cms.InputTag("hltEcalDigis","EcalTriggerPrimitives"),
    recoverEEFE = cms.bool(False),
    singleChannelRecoveryMethod = cms.string('NeuralNetworks'),
    flagsMapDBReco = cms.vint32(0, 0, 0, 0, 4, 
        -1, -1, -1, 4, 4, 
        7, 7, 7, 8, 9),
    EBuncalibRecHitCollection = cms.InputTag("hltEcalFixedAlphaBetaFitUncalibRecHit","EcalUncalibRecHitsEB"),
    algoRecover = cms.string('EcalRecHitWorkerRecover'),
    algo = cms.string('EcalRecHitWorkerSimple')
)


hltHfreco = cms.EDProducer("HcalSimpleReconstructor",
    correctionPhaseNS = cms.double(0.0),
    digiLabel = cms.InputTag("hltHcalDigis"),
    samplesToAdd = cms.int32(4),
    Subdetector = cms.string('HF'),
    correctForTimeslew = cms.bool(False),
    dropZSmarkedPassed = cms.bool(True),
    correctForPhaseContainment = cms.bool(False),
    firstSample = cms.int32(3)
)


hltL1NonIsoLWEleTrackIsol = cms.EDProducer("EgammaHLTElectronTrackIsolationProducers",
    egTrkIsoVetoConeSize = cms.double(0.02),
    trackProducer = cms.InputTag("hltL1NonIsoLWEleRegioCTF"),
    electronProducer = cms.InputTag("hltPixelMatchElectronsL1NonIsoLW"),
    egTrkIsoConeSize = cms.double(0.2),
    egTrkIsoRSpan = cms.double(999999.0),
    egTrkIsoPtMin = cms.double(1.5),
    egTrkIsoZSpan = cms.double(0.1)
)


openhltL25TauJetTracksAssociator = cms.EDProducer("JetTracksAssociatorAtVertex",
    jets = cms.InputTag("openhltL2TauJets"),
    tracks = cms.InputTag("openhltL25TauCtfWithMaterialTracks"),
    coneSize = cms.double(0.5)
)


hltL2MuonCandidates = cms.EDProducer("L2MuonCandidateProducer",
    InputObjects = cms.InputTag("hltL2Muons","UpdatedAtVtx")
)


hltMet = cms.EDProducer("METProducer",
    src = cms.InputTag("hltTowerMakerForAll"),
    METType = cms.string('CaloMET'),
    InputType = cms.string('CandidateCollection'),
    HO_PhiResPar = cms.vdouble(0.02511),
    HF_PhiResPar = cms.vdouble(0.05022),
    calculateSignificance = cms.bool(False),
    EB_EtResPar = cms.vdouble(0.2, 0.03, 0.005),
    EE_PhiResPar = cms.vdouble(0.02511),
    HB_EtResPar = cms.vdouble(0.0, 1.22, 0.05),
    HB_PhiResPar = cms.vdouble(0.02511),
    alias = cms.string('RawCaloMET'),
    HE_PhiResPar = cms.vdouble(0.02511),
    noHF = cms.bool(False),
    EE_EtResPar = cms.vdouble(0.2, 0.03, 0.005),
    globalThreshold = cms.double(0.3),
    HF_EtResPar = cms.vdouble(0.0, 1.82, 0.09),
    HE_EtResPar = cms.vdouble(0.0, 1.3, 0.05),
    HO_EtResPar = cms.vdouble(0.0, 1.3, 0.005),
    EB_PhiResPar = cms.vdouble(0.00502),
    onlyFiducialParticles = cms.bool(False)
)


hltHITCkfTrackCandidatesHE8E29 = cms.EDProducer("CkfTrackCandidateMaker",
    src = cms.InputTag("hltHITPixelTripletSeedGeneratorHE8E29"),
    TransientInitialStateEstimatorParameters = cms.PSet(
        propagatorAlongTISE = cms.string('PropagatorWithMaterial'),
        numberMeasurementsForFit = cms.int32(4),
        propagatorOppositeTISE = cms.string('PropagatorWithMaterialOpposite')
    ),
    RedundantSeedCleaner = cms.string('CachingSeedCleanerBySharedInput'),
    cleanTrajectoryAfterInOut = cms.bool(False),
    useHitsSplitting = cms.bool(False),
    doSeedingRegionRebuilding = cms.bool(False),
    maxNSeeds = cms.uint32(100000),
    NavigationSchool = cms.string('SimpleNavigationSchool'),
    TrajectoryBuilder = cms.string('CkfTrajectoryBuilder'),
    TrajectoryCleaner = cms.string('TrajectoryCleanerBySharedHits')
)


hltHIPixelVertices = cms.EDProducer("PixelVertexProducerMedian",
    TrackCollection = cms.string('hltHIPixelTracks'),
    PtMin = cms.double(0.5)
)


hltL3TauCkfTrackCandidates = cms.EDProducer("CkfTrackCandidateMaker",
    src = cms.InputTag("hltL3TauPixelSeeds"),
    TransientInitialStateEstimatorParameters = cms.PSet(
        propagatorAlongTISE = cms.string('PropagatorWithMaterial'),
        numberMeasurementsForFit = cms.int32(4),
        propagatorOppositeTISE = cms.string('PropagatorWithMaterialOpposite')
    ),
    RedundantSeedCleaner = cms.string('CachingSeedCleanerBySharedInput'),
    cleanTrajectoryAfterInOut = cms.bool(False),
    useHitsSplitting = cms.bool(False),
    doSeedingRegionRebuilding = cms.bool(False),
    maxNSeeds = cms.uint32(100000),
    NavigationSchool = cms.string('SimpleNavigationSchool'),
    TrajectoryBuilder = cms.string('trajBuilderL3'),
    TrajectoryCleaner = cms.string('TrajectoryCleanerBySharedHits')
)


hltBLifetimeHighestEtJets = cms.EDProducer("LargestEtCaloJetSelector",
    maxNumber = cms.uint32(4),
    filter = cms.bool(False),
    src = cms.InputTag("hltIterativeCone5CaloJets")
)


hltIsolPixelTrackProdHE8E29 = cms.EDProducer("IsolatedPixelTrackCandidateProducer",
    EBEtaBoundary = cms.double(1.479),
    L1eTauJetsSource = cms.InputTag("hltL1extraParticles","Tau"),
    MaxVtxDXYSeed = cms.double(101.0),
    tauUnbiasCone = cms.double(1.2),
    PixelIsolationConeSizeAtEC = cms.double(40.0),
    L1GTSeedLabel = cms.InputTag("hltL1sIsoTrack8E29"),
    minPTrack = cms.double(5.0),
    maxPTrackForIsolation = cms.double(3.0),
    MagFieldRecordName = cms.string('VolumeBasedMagneticField'),
    VertexLabel = cms.InputTag("hltHITPixelVerticesHE"),
    PixelTracksSources = cms.VInputTag("hltHITPixelTracksHB", "hltHITPixelTracksHE"),
    MaxVtxDXYIsol = cms.double(101.0),
    tauAssociationCone = cms.double(0.0),
    ExtrapolationConeSize = cms.double(1.0)
)


openHltBLifetimeL25BJetTags = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('trackCounting3D2nd'),
    tagInfos = cms.VInputTag(cms.InputTag("openHltBLifetimeL25TagInfos"))
)


hltIconeTau3Regional = cms.EDProducer("FastjetJetProducer",
    Active_Area_Repeats = cms.int32(5),
    doAreaFastjet = cms.bool(False),
    Ghost_EtaMax = cms.double(6.0),
    maxBadHcalCells = cms.uint32(9999999),
    maxRecoveredEcalCells = cms.uint32(9999999),
    jetType = cms.string('CaloJet'),
    doRhoFastjet = cms.bool(False),
    jetAlgorithm = cms.string('IterativeCone'),
    nSigmaPU = cms.double(1.0),
    GhostArea = cms.double(0.01),
    maxBadEcalCells = cms.uint32(9999999),
    doPVCorrection = cms.bool(False),
    maxRecoveredHcalCells = cms.uint32(9999999),
    rParam = cms.double(0.2),
    UseOnlyVertexTracks = cms.bool(False),
    UseOnlyOnePV = cms.bool(False),
    DzTrVtxMax = cms.double(0.0),
    maxProblematicHcalCells = cms.uint32(9999999),
    src = cms.InputTag("hltCaloTowersTau3Regional"),
    inputEtMin = cms.double(0.5),
    srcPVs = cms.InputTag("offlinePrimaryVertices"),
    jetPtMin = cms.double(1.0),
    radiusPU = cms.double(0.5),
    maxProblematicEcalCells = cms.uint32(9999999),
    doPUOffsetCorr = cms.bool(False),
    inputEMin = cms.double(0.0)
)


hltEcalRegionalJetsFEDs = cms.EDProducer("EcalRawToRecHitRoI",
    JetJobPSet = cms.VPSet(cms.PSet(
        regionEtaMargin = cms.double(1.0),
        regionPhiMargin = cms.double(1.0),
        Ptmin = cms.double(14.0),
        Source = cms.InputTag("hltL1extraParticles","Central")
    ), 
        cms.PSet(
            regionEtaMargin = cms.double(1.0),
            regionPhiMargin = cms.double(1.0),
            Ptmin = cms.double(20.0),
            Source = cms.InputTag("hltL1extraParticles","Forward")
        ), 
        cms.PSet(
            regionEtaMargin = cms.double(1.0),
            regionPhiMargin = cms.double(1.0),
            Ptmin = cms.double(14.0),
            Source = cms.InputTag("hltL1extraParticles","Tau")
        )),
    MuJobPSet = cms.PSet(

    ),
    CandJobPSet = cms.VPSet(),
    doES = cms.bool(False),
    type = cms.string('jet'),
    sourceTag = cms.InputTag("hltEcalRawToRecHitFacility"),
    EmJobPSet = cms.VPSet(),
    sourceTag_es = cms.InputTag("NotNeededoESfalse")
)


openHltBLifetimeRegionalCkfTrackCandidates = cms.EDProducer("CkfTrackCandidateMaker",
    src = cms.InputTag("openHltBLifetimeRegionalPixelSeedGenerator"),
    TransientInitialStateEstimatorParameters = cms.PSet(
        propagatorAlongTISE = cms.string('PropagatorWithMaterial'),
        numberMeasurementsForFit = cms.int32(4),
        propagatorOppositeTISE = cms.string('PropagatorWithMaterialOpposite')
    ),
    RedundantSeedCleaner = cms.string('CachingSeedCleanerBySharedInput'),
    cleanTrajectoryAfterInOut = cms.bool(False),
    useHitsSplitting = cms.bool(False),
    doSeedingRegionRebuilding = cms.bool(False),
    maxNSeeds = cms.uint32(100000),
    NavigationSchool = cms.string('SimpleNavigationSchool'),
    TrajectoryBuilder = cms.string('bJetRegionalTrajectoryBuilder'),
    TrajectoryCleaner = cms.string('TrajectoryCleanerBySharedHits')
)


hltIslandSuperClustersHI = cms.EDProducer("SuperClusterProducer",
    barrelSuperclusterCollection = cms.string('islandBarrelSuperClustersHI'),
    endcapEtaSearchRoad = cms.double(0.14),
    barrelClusterCollection = cms.string('islandBarrelBasicClustersHI'),
    endcapClusterProducer = cms.string('hltIslandBasicClustersHI'),
    barrelPhiSearchRoad = cms.double(0.8),
    endcapPhiSearchRoad = cms.double(0.6),
    VerbosityLevel = cms.string('ERROR'),
    seedTransverseEnergyThreshold = cms.double(1.0),
    barrelClusterProducer = cms.string('hltIslandBasicClustersHI'),
    endcapSuperclusterCollection = cms.string('islandEndcapSuperClustersHI'),
    barrelEtaSearchRoad = cms.double(0.06),
    doEndcaps = cms.bool(True),
    endcapClusterCollection = cms.string('islandEndcapBasicClustersHI'),
    doBarrel = cms.bool(True)
)


hltIconeCentral2Regional = cms.EDProducer("FastjetJetProducer",
    Active_Area_Repeats = cms.int32(5),
    doAreaFastjet = cms.bool(False),
    Ghost_EtaMax = cms.double(6.0),
    maxBadHcalCells = cms.uint32(9999999),
    maxRecoveredEcalCells = cms.uint32(9999999),
    jetType = cms.string('CaloJet'),
    doRhoFastjet = cms.bool(False),
    jetAlgorithm = cms.string('IterativeCone'),
    nSigmaPU = cms.double(1.0),
    GhostArea = cms.double(0.01),
    maxBadEcalCells = cms.uint32(9999999),
    doPVCorrection = cms.bool(False),
    maxRecoveredHcalCells = cms.uint32(9999999),
    rParam = cms.double(0.2),
    UseOnlyVertexTracks = cms.bool(False),
    UseOnlyOnePV = cms.bool(False),
    DzTrVtxMax = cms.double(0.0),
    maxProblematicHcalCells = cms.uint32(9999999),
    src = cms.InputTag("hltCaloTowersCentral2Regional"),
    inputEtMin = cms.double(0.5),
    srcPVs = cms.InputTag("offlinePrimaryVertices"),
    jetPtMin = cms.double(1.0),
    radiusPU = cms.double(0.5),
    maxProblematicEcalCells = cms.uint32(9999999),
    doPUOffsetCorr = cms.bool(False),
    inputEMin = cms.double(0.0)
)


hltL1NonIsoElectronsRegionalCTFFinalFitWithMaterial = cms.EDProducer("TrackProducer",
    src = cms.InputTag("hltL1NonIsoElectronsRegionalCkfTrackCandidates"),
    clusterRemovalInfo = cms.InputTag(""),
    beamSpot = cms.InputTag("hltOfflineBeamSpot"),
    Fitter = cms.string('hltKFFittingSmoother'),
    useHitsSplitting = cms.bool(False),
    alias = cms.untracked.string('hltEgammaRegionalCTFFinalFitWithMaterial'),
    NavigationSchool = cms.string(''),
    TrajectoryInEvent = cms.bool(False),
    TTRHBuilder = cms.string('WithTrackAngle'),
    AlgorithmName = cms.string('undefAlgorithm'),
    Propagator = cms.string('PropagatorWithMaterial')
)


hltSelector4Jets = cms.EDProducer("LargestEtCaloJetSelector",
    maxNumber = cms.uint32(4),
    filter = cms.bool(False),
    src = cms.InputTag("hltMCJetCorJetIcone5")
)


hltHITPixelTripletSeedGeneratorHE1E31 = cms.EDProducer("SeedGeneratorFromRegionHitsEDProducer",
    SeedComparitorPSet = cms.PSet(
        ComponentName = cms.string('none')
    ),
    ClusterCheckPSet = cms.PSet(
        MaxNumberOfCosmicClusters = cms.uint32(50000),
        ClusterCollectionLabel = cms.InputTag("siStripClusters"),
        doClusterCheck = cms.bool(False)
    ),
    RegionFactoryPSet = cms.PSet(
        ComponentName = cms.string('HITRegionalPixelSeedGenerator'),
        RegionPSet = cms.PSet(
            deltaEtaTrackRegion = cms.double(0.05),
            useL1Jets = cms.bool(False),
            deltaPhiTrackRegion = cms.double(0.05),
            isoTrackSrc = cms.InputTag("hltIsolPixelTrackL2FilterHE1E31"),
            precise = cms.bool(True),
            deltaEtaL1JetRegion = cms.double(0.3),
            useTracks = cms.bool(False),
            originRadius = cms.double(0.2),
            trackSrc = cms.InputTag("hltHITPixelTracksHE"),
            useIsoTracks = cms.bool(True),
            l1tjetSrc = cms.InputTag("hltl1extraParticles","Tau"),
            deltaPhiL1JetRegion = cms.double(0.3),
            vertexSrc = cms.string('hltHITPixelVerticesHE'),
            fixedReg = cms.bool(False),
            etaCenter = cms.double(0.0),
            phiCenter = cms.double(0.0),
            originZPos = cms.double(0.0),
            originHalfLength = cms.double(0.2),
            ptMin = cms.double(0.5)
        )
    ),
    SeedCreatorPSet = cms.PSet(
        ComponentName = cms.string('SeedFromConsecutiveHitsCreator'),
        propagator = cms.string('PropagatorWithMaterial')
    ),
    OrderedHitsFactoryPSet = cms.PSet(
        ComponentName = cms.string('StandardHitTripletGenerator'),
        SeedingLayers = cms.string('PixelLayerTriplets'),
        GeneratorPSet = cms.PSet(
            useBending = cms.bool(True),
            useFixedPreFiltering = cms.bool(False),
            ComponentName = cms.string('PixelTripletHLTGenerator'),
            extraHitRPhitolerance = cms.double(0.06),
            useMultScattering = cms.bool(True),
            phiPreFiltering = cms.double(0.3),
            extraHitRZtolerance = cms.double(0.06)
        )
    ),
    TTRHBuilder = cms.string('WithTrackAngle')
)


hltCosmicMuonBarrelOnly = cms.EDProducer("CosmicMuonProducer",
    ServiceParameters = cms.PSet(
        Propagators = cms.untracked.vstring('SteppingHelixPropagatorAny', 
            'SteppingHelixPropagatorOpposite', 
            'StraightLinePropagator'),
        RPCLayers = cms.bool(True),
        UseMuonNavigation = cms.untracked.bool(True)
    ),
    TrajectoryBuilderParameters = cms.PSet(
        BackwardMuonTrajectoryUpdatorParameters = cms.PSet(
            MaxChi2 = cms.double(100.0),
            Granularity = cms.int32(2),
            RescaleErrorFactor = cms.double(1.0),
            UseInvalidHits = cms.bool(False),
            RescaleError = cms.bool(False)
        ),
        RPCRecSegmentLabel = cms.InputTag("hltRpcRecHits"),
        MuonTrajectoryUpdatorParameters = cms.PSet(
            MaxChi2 = cms.double(3000.0),
            Granularity = cms.int32(0),
            RescaleErrorFactor = cms.double(1.0),
            UseInvalidHits = cms.bool(False),
            RescaleError = cms.bool(False)
        ),
        EnableRPCMeasurement = cms.bool(True),
        CSCRecSegmentLabel = cms.InputTag("hltCscSegments"),
        BuildTraversingMuon = cms.bool(False),
        MuonSmootherParameters = cms.PSet(
            PropagatorAlong = cms.string('SteppingHelixPropagatorAny'),
            PropagatorOpposite = cms.string('SteppingHelixPropagatorAny'),
            RescalingFactor = cms.double(5.0)
        ),
        EnableDTMeasurement = cms.bool(True),
        DTRecSegmentLabel = cms.InputTag("hltDt4DSegments"),
        Propagator = cms.string('SteppingHelixPropagatorAny'),
        EnableCSCMeasurement = cms.bool(False),
        MuonNavigationParameters = cms.PSet(
            Barrel = cms.bool(True),
            Endcap = cms.bool(True)
        )
    ),
    TrackLoaderParameters = cms.PSet(
        PutTrajectoryIntoEvent = cms.untracked.bool(False),
        PutTkTrackIntoEvent = cms.untracked.bool(True),
        SmoothTkTrack = cms.untracked.bool(False),
        MuonSeededTracksInstance = cms.untracked.string('L2Seeded'),
        AllowNoVertex = cms.untracked.bool(True),
        Smoother = cms.string('KFSmootherForMuonTrackLoader'),
        MuonUpdatorAtVertexParameters = cms.PSet(
            MaxChi2 = cms.double(1000000.0),
            BeamSpotPosition = cms.vdouble(0.0, 0.0, 0.0),
            Propagator = cms.string('SteppingHelixPropagatorOpposite'),
            BeamSpotPositionErrors = cms.vdouble(0.1, 0.1, 5.3)
        ),
        VertexConstraint = cms.bool(False),
        DoSmoothing = cms.bool(False)
    ),
    MuonSeedCollectionLabel = cms.string('hltCosmicMuonSeedBarrelOnly')
)


hltElectronL1IsoLargeWindowDetaDphi = cms.EDProducer("EgammaHLTElectronDetaDphiProducer",
    electronProducer = cms.InputTag("hltPixelMatchLargeWindowElectronsL1Iso"),
    BSProducer = cms.InputTag("hltOfflineBeamSpot")
)


hltMulti5x5BasicClustersL1Isolated = cms.EDProducer("EgammaHLTMulti5x5ClusterProducer",
    endcapHitProducer = cms.InputTag("hltEcalRegionalEgammaRecHit"),
    barrelClusterCollection = cms.string('notused'),
    regionEtaMargin = cms.double(0.3),
    regionPhiMargin = cms.double(0.4),
    VerbosityLevel = cms.string('ERROR'),
    barrelHitCollection = cms.string('EcalRecHitsEB'),
    posCalc_t0_endcPresh = cms.double(1.2),
    posCalc_logweight = cms.bool(True),
    doIsolated = cms.bool(True),
    Multi5x5EndcapSeedThr = cms.double(0.18),
    posCalc_w0 = cms.double(4.2),
    l1UpperThr = cms.double(999.0),
    endcapClusterCollection = cms.string('multi5x5EndcapBasicClusters'),
    l1LowerThr = cms.double(5.0),
    posCalc_t0_endc = cms.double(3.1),
    l1TagIsolated = cms.InputTag("hltL1extraParticles","Isolated"),
    doEndcaps = cms.bool(True),
    Multi5x5BarrelSeedThr = cms.double(0.5),
    posCalc_x0 = cms.double(0.89),
    endcapHitCollection = cms.string('EcalRecHitsEE'),
    l1TagNonIsolated = cms.InputTag("hltL1extraParticles","NonIsolated"),
    barrelHitProducer = cms.InputTag("hltEcalRegionalEgammaRecHit"),
    l1LowerThrIgnoreIsolation = cms.double(999.0),
    posCalc_t0_barl = cms.double(7.4),
    doBarrel = cms.bool(False)
)


hltL3TkTracksFromL2 = cms.EDProducer("TrackProducer",
    src = cms.InputTag("hltL3TrackCandidateFromL2"),
    clusterRemovalInfo = cms.InputTag(""),
    beamSpot = cms.InputTag("hltOfflineBeamSpot"),
    Fitter = cms.string('hltKFFittingSmoother'),
    useHitsSplitting = cms.bool(False),
    alias = cms.untracked.string(''),
    NavigationSchool = cms.string(''),
    TrajectoryInEvent = cms.bool(True),
    TTRHBuilder = cms.string('WithTrackAngle'),
    AlgorithmName = cms.string('undefAlgorithm'),
    Propagator = cms.string('PropagatorWithMaterial')
)


hltPixelTracks = cms.EDProducer("PixelTrackProducer",
    FilterPSet = cms.PSet(
        nSigmaTipMaxTolerance = cms.double(0.0),
        chi2 = cms.double(1000.0),
        ComponentName = cms.string('PixelTrackFilterByKinematics'),
        nSigmaInvPtTolerance = cms.double(0.0),
        ptMin = cms.double(0.1),
        tipMax = cms.double(1.0)
    ),
    useFilterWithES = cms.bool(False),
    FitterPSet = cms.PSet(
        ComponentName = cms.string('PixelFitterByHelixProjections'),
        TTRHBuilder = cms.string('TTRHBuilderPixelOnly')
    ),
    RegionFactoryPSet = cms.PSet(
        ComponentName = cms.string('GlobalRegionProducer'),
        RegionPSet = cms.PSet(
            precise = cms.bool(True),
            originHalfLength = cms.double(15.9),
            originRadius = cms.double(0.2),
            originYPos = cms.double(0.0),
            ptMin = cms.double(0.9),
            originXPos = cms.double(0.0),
            originZPos = cms.double(0.0)
        )
    ),
    CleanerPSet = cms.PSet(
        ComponentName = cms.string('PixelTrackCleanerBySharedHits')
    ),
    OrderedHitsFactoryPSet = cms.PSet(
        ComponentName = cms.string('StandardHitTripletGenerator'),
        SeedingLayers = cms.string('PixelLayerTriplets'),
        GeneratorPSet = cms.PSet(
            useBending = cms.bool(True),
            useFixedPreFiltering = cms.bool(False),
            phiPreFiltering = cms.double(0.3),
            extraHitRPhitolerance = cms.double(0.06),
            useMultScattering = cms.bool(True),
            ComponentName = cms.string('PixelTripletHLTGenerator'),
            extraHitRZtolerance = cms.double(0.06)
        )
    )
)


hltPixelVerticesForMinBias = cms.EDProducer("PixelVertexProducer",
    WtAverage = cms.bool(True),
    Method2 = cms.bool(True),
    beamSpot = cms.InputTag("hltOfflineBeamSpot"),
    Verbosity = cms.int32(0),
    UseError = cms.bool(True),
    TrackCollection = cms.InputTag("hltPixelTracksForMinBias"),
    ZSeparation = cms.double(0.05),
    NTrkMin = cms.int32(2),
    ZOffset = cms.double(5.0),
    Finder = cms.string('DivisiveVertexFinder'),
    PtMin = cms.double(0.2)
)


hltHITIPTCorrectorHB1E31 = cms.EDProducer("IPTCorrector",
    corTracksLabel = cms.InputTag("hltHITCtfWithMaterialTracksHB1E31"),
    filterLabel = cms.InputTag("hltIsolPixelTrackL2FilterHB1E31"),
    associationCone = cms.double(0.2)
)


hltL1IsoSSEleRegioCTF = cms.EDProducer("TrackProducer",
    src = cms.InputTag("hltL1IsoSSEleRegioCkfTC"),
    clusterRemovalInfo = cms.InputTag(""),
    beamSpot = cms.InputTag("hltOfflineBeamSpot"),
    Fitter = cms.string('hltKFFittingSmoother'),
    useHitsSplitting = cms.bool(False),
    alias = cms.untracked.string('hltL1IsoSSEleRegioCTF'),
    NavigationSchool = cms.string(''),
    TrajectoryInEvent = cms.bool(False),
    TTRHBuilder = cms.string('WithTrackAngle'),
    AlgorithmName = cms.string('undefAlgorithm'),
    Propagator = cms.string('PropagatorWithMaterial')
)


hltBLifetimeRegionalPixelSeedGenerator = cms.EDProducer("SeedGeneratorFromRegionHitsEDProducer",
    SeedComparitorPSet = cms.PSet(
        ComponentName = cms.string('none')
    ),
    ClusterCheckPSet = cms.PSet(
        MaxNumberOfCosmicClusters = cms.uint32(50000),
        ClusterCollectionLabel = cms.InputTag("hltSiStripClusters"),
        doClusterCheck = cms.bool(False)
    ),
    RegionFactoryPSet = cms.PSet(
        ComponentName = cms.string('TauRegionalPixelSeedGenerator'),
        RegionPSet = cms.PSet(
            precise = cms.bool(True),
            deltaPhiRegion = cms.double(0.25),
            originHalfLength = cms.double(0.2),
            originRadius = cms.double(0.2),
            deltaEtaRegion = cms.double(0.25),
            ptMin = cms.double(1.0),
            JetSrc = cms.InputTag("hltBLifetimeL3Jets"),
            originZPos = cms.double(0.0),
            vertexSrc = cms.InputTag("hltPixelVertices")
        )
    ),
    SeedCreatorPSet = cms.PSet(
        ComponentName = cms.string('SeedFromConsecutiveHitsCreator'),
        propagator = cms.string('PropagatorWithMaterial')
    ),
    OrderedHitsFactoryPSet = cms.PSet(
        ComponentName = cms.string('StandardHitPairGenerator'),
        SeedingLayers = cms.string('PixelLayerPairs')
    ),
    TTRHBuilder = cms.string('WithTrackAngle')
)


hltIcone2Central3 = cms.EDProducer("IterativeConeJetProducer",
    correctInputToSignalVertex = cms.bool(False),
    verbose = cms.untracked.bool(False),
    inputEtMin = cms.double(0.5),
    pvCollection = cms.InputTag("offlinePrimaryVertices"),
    jetPtMin = cms.double(0.0),
    coneRadius = cms.double(0.2),
    alias = cms.untracked.string('IC5CaloJet'),
    seedThreshold = cms.double(1.0),
    debugLevel = cms.untracked.int32(0),
    jetType = cms.untracked.string('CaloJet'),
    src = cms.InputTag("hltCaloTowersCentral3"),
    inputEMin = cms.double(0.0)
)


hltCaloTowersTau4 = cms.EDProducer("CaloTowerCreatorForTauHLT",
    towers = cms.InputTag("hltTowerMakerForAll"),
    TauId = cms.int32(3),
    TauTrigger = cms.InputTag("hltL1extraParticles","Tau"),
    minimumE = cms.double(0.8),
    UseTowersInCone = cms.double(0.8),
    minimumEt = cms.double(0.5)
)


hltIcone2Central1 = cms.EDProducer("IterativeConeJetProducer",
    correctInputToSignalVertex = cms.bool(False),
    verbose = cms.untracked.bool(False),
    inputEtMin = cms.double(0.5),
    pvCollection = cms.InputTag("offlinePrimaryVertices"),
    jetPtMin = cms.double(0.0),
    coneRadius = cms.double(0.2),
    alias = cms.untracked.string('IC5CaloJet'),
    seedThreshold = cms.double(1.0),
    debugLevel = cms.untracked.int32(0),
    jetType = cms.untracked.string('CaloJet'),
    src = cms.InputTag("hltCaloTowersCentral1"),
    inputEMin = cms.double(0.0)
)


hltCaloTowersTau2 = cms.EDProducer("CaloTowerCreatorForTauHLT",
    towers = cms.InputTag("hltTowerMakerForAll"),
    TauId = cms.int32(1),
    TauTrigger = cms.InputTag("hltL1extraParticles","Tau"),
    minimumE = cms.double(0.8),
    UseTowersInCone = cms.double(0.8),
    minimumEt = cms.double(0.5)
)


hltCaloTowersTau3 = cms.EDProducer("CaloTowerCreatorForTauHLT",
    towers = cms.InputTag("hltTowerMakerForAll"),
    TauId = cms.int32(2),
    TauTrigger = cms.InputTag("hltL1extraParticles","Tau"),
    minimumE = cms.double(0.8),
    UseTowersInCone = cms.double(0.8),
    minimumEt = cms.double(0.5)
)


hltIcone2Central4 = cms.EDProducer("IterativeConeJetProducer",
    correctInputToSignalVertex = cms.bool(False),
    verbose = cms.untracked.bool(False),
    inputEtMin = cms.double(0.5),
    pvCollection = cms.InputTag("offlinePrimaryVertices"),
    jetPtMin = cms.double(0.0),
    coneRadius = cms.double(0.2),
    alias = cms.untracked.string('IC5CaloJet'),
    seedThreshold = cms.double(1.0),
    debugLevel = cms.untracked.int32(0),
    jetType = cms.untracked.string('CaloJet'),
    src = cms.InputTag("hltCaloTowersCentral4"),
    inputEMin = cms.double(0.0)
)


hltCaloTowersTau1 = cms.EDProducer("CaloTowerCreatorForTauHLT",
    towers = cms.InputTag("hltTowerMakerForAll"),
    TauId = cms.int32(0),
    TauTrigger = cms.InputTag("hltL1extraParticles","Tau"),
    minimumE = cms.double(0.8),
    UseTowersInCone = cms.double(0.8),
    minimumEt = cms.double(0.5)
)


hltDt4DSegments = cms.EDProducer("DTRecSegment4DProducer",
    debug = cms.untracked.bool(False),
    Reco4DAlgoName = cms.string('DTCombinatorialPatternReco4D'),
    recHits2DLabel = cms.InputTag("dt2DSegments"),
    Reco4DAlgoConfig = cms.PSet(
        segmCleanerMode = cms.int32(1),
        Reco2DAlgoName = cms.string('DTCombinatorialPatternReco'),
        recAlgoConfig = cms.PSet(
            minTime = cms.double(-3.0),
            debug = cms.untracked.bool(False),
            tTrigModeConfig = cms.PSet(
                vPropWire = cms.double(24.4),
                doTOFCorrection = cms.bool(True),
                tofCorrType = cms.int32(1),
                wirePropCorrType = cms.int32(1),
                doWirePropCorrection = cms.bool(True),
                doT0Correction = cms.bool(True),
                debug = cms.untracked.bool(False)
            ),
            maxTime = cms.double(420.0),
            tTrigMode = cms.string('DTTTrigSyncFromDB')
        ),
        nSharedHitsMax = cms.int32(2),
        hit_afterT0_resolution = cms.double(0.03),
        Reco2DAlgoConfig = cms.PSet(
            segmCleanerMode = cms.int32(1),
            recAlgoConfig = cms.PSet(
                minTime = cms.double(-3.0),
                debug = cms.untracked.bool(False),
                tTrigModeConfig = cms.PSet(
                    vPropWire = cms.double(24.4),
                    doTOFCorrection = cms.bool(True),
                    tofCorrType = cms.int32(1),
                    wirePropCorrType = cms.int32(1),
                    doWirePropCorrection = cms.bool(True),
                    doT0Correction = cms.bool(True),
                    debug = cms.untracked.bool(False)
                ),
                maxTime = cms.double(420.0),
                tTrigMode = cms.string('DTTTrigSyncFromDB')
            ),
            nSharedHitsMax = cms.int32(2),
            AlphaMaxPhi = cms.double(1.0),
            hit_afterT0_resolution = cms.double(0.03),
            MaxAllowedHits = cms.uint32(50),
            performT0_vdriftSegCorrection = cms.bool(False),
            AlphaMaxTheta = cms.double(0.1),
            debug = cms.untracked.bool(False),
            recAlgo = cms.string('DTLinearDriftFromDBAlgo'),
            nUnSharedHitsMin = cms.int32(2),
            performT0SegCorrection = cms.bool(False)
        ),
        performT0_vdriftSegCorrection = cms.bool(False),
        debug = cms.untracked.bool(False),
        recAlgo = cms.string('DTLinearDriftFromDBAlgo'),
        nUnSharedHitsMin = cms.int32(2),
        AllDTRecHits = cms.bool(True),
        performT0SegCorrection = cms.bool(False)
    ),
    recHits1DLabel = cms.InputTag("hltDt1DRecHits")
)


hltL1NonIsoSSEleRegioCTF = cms.EDProducer("TrackProducer",
    src = cms.InputTag("hltL1NonIsoSSEleRegioCkfTC"),
    clusterRemovalInfo = cms.InputTag(""),
    beamSpot = cms.InputTag("hltOfflineBeamSpot"),
    Fitter = cms.string('hltKFFittingSmoother'),
    useHitsSplitting = cms.bool(False),
    alias = cms.untracked.string('hltL1NonIsoSSEleRegioCTF'),
    NavigationSchool = cms.string(''),
    TrajectoryInEvent = cms.bool(False),
    TTRHBuilder = cms.string('WithTrackAngle'),
    AlgorithmName = cms.string('undefAlgorithm'),
    Propagator = cms.string('PropagatorWithMaterial')
)


openhltL2TauIsolationProducer = cms.EDProducer("L2TauNarrowConeIsolationProducer",
    ECALIsolation = cms.PSet(
        innerCone = cms.double(0.15),
        runAlgorithm = cms.bool(True),
        outerCone = cms.double(0.5)
    ),
    CaloTowers = cms.InputTag("hltTowerMakerForAll"),
    crystalThresholdEB = cms.double(0.15),
    TowerIsolation = cms.PSet(
        innerCone = cms.double(0.2),
        runAlgorithm = cms.bool(True),
        outerCone = cms.double(0.5)
    ),
    crystalThresholdEE = cms.double(0.45),
    EBRecHits = cms.InputTag("hltEcalRecHitAll","EcalRecHitsEB"),
    L2TauJetCollection = cms.InputTag("openhltL2TauJets"),
    associationRadius = cms.double(0.5),
    EERecHits = cms.InputTag("hltEcalRecHitAll","EcalRecHitsEE"),
    ECALClustering = cms.PSet(
        runAlgorithm = cms.bool(True),
        clusterRadius = cms.double(0.08)
    ),
    towerThreshold = cms.double(1.0)
)


hltMulti5x5SuperClustersL1Isolated = cms.EDProducer("Multi5x5SuperClusterProducer",
    barrelSuperclusterCollection = cms.string('multi5x5BarrelSuperClusters'),
    endcapEtaSearchRoad = cms.double(0.14),
    barrelClusterCollection = cms.string('multi5x5BarrelBasicClusters'),
    barrelPhiSearchRoad = cms.double(0.8),
    endcapClusterProducer = cms.string('hltMulti5x5BasicClustersL1Isolated'),
    dynamicPhiRoad = cms.bool(False),
    endcapPhiSearchRoad = cms.double(0.6),
    VerbosityLevel = cms.string('ERROR'),
    seedTransverseEnergyThreshold = cms.double(1.0),
    barrelClusterProducer = cms.string('notused'),
    endcapSuperclusterCollection = cms.string('multi5x5EndcapSuperClusters'),
    barrelEtaSearchRoad = cms.double(0.06),
    doBarrel = cms.bool(False),
    doEndcaps = cms.bool(True),
    endcapClusterCollection = cms.string('multi5x5EndcapBasicClusters'),
    bremRecoveryPset = cms.PSet(
        barrel = cms.PSet(

        ),
        endcap = cms.PSet(
            a = cms.double(47.85),
            c = cms.double(0.1201),
            b = cms.double(108.8)
        ),
        doEndcaps = cms.bool(True),
        doBarrel = cms.bool(False)
    )
)


hltBSoftmuonL25TagInfos = cms.EDProducer("SoftLepton",
    muonSelection = cms.uint32(0),
    refineJetAxis = cms.uint32(0),
    primaryVertex = cms.InputTag("nominal"),
    leptons = cms.InputTag("hltL2Muons"),
    leptonQualityCut = cms.double(0.0),
    jets = cms.InputTag("hltBSoftmuonL25Jets"),
    leptonDeltaRCut = cms.double(0.4),
    leptonChi2Cut = cms.double(0.0)
)


hltL2MuonIsolations = cms.EDProducer("L2MuonIsolationProducer",
    IsolatorPSet = cms.PSet(
        ConeSizes = cms.vdouble(0.24, 0.24, 0.24, 0.24, 0.24, 
            0.24, 0.24, 0.24, 0.24, 0.24, 
            0.24, 0.24, 0.24, 0.24, 0.24, 
            0.24, 0.24, 0.24, 0.24, 0.24, 
            0.24, 0.24, 0.24, 0.24, 0.24, 
            0.24),
        ComponentName = cms.string('SimpleCutsIsolator'),
        EtaBounds = cms.vdouble(0.0435, 0.1305, 0.2175, 0.3045, 0.3915, 
            0.4785, 0.5655, 0.6525, 0.7395, 0.8265, 
            0.9135, 1.0005, 1.0875, 1.1745, 1.2615, 
            1.3485, 1.4355, 1.5225, 1.6095, 1.6965, 
            1.785, 1.88, 1.9865, 2.1075, 2.247, 
            2.411),
        Thresholds = cms.vdouble(4.0, 3.7, 4.0, 3.5, 3.4, 
            3.4, 3.2, 3.4, 3.1, 2.9, 
            2.9, 2.7, 3.1, 3.0, 2.4, 
            2.1, 2.0, 2.3, 2.2, 2.4, 
            2.5, 2.5, 2.6, 2.9, 3.1, 
            2.9)
    ),
    StandAloneCollectionLabel = cms.InputTag("hltL2Muons","UpdatedAtVtx"),
    ExtractorPSet = cms.PSet(
        DR_Veto_H = cms.double(0.1),
        Vertex_Constraint_Z = cms.bool(False),
        Threshold_H = cms.double(0.5),
        ComponentName = cms.string('CaloExtractor'),
        Threshold_E = cms.double(0.2),
        DR_Max = cms.double(0.24),
        DR_Veto_E = cms.double(0.07),
        Weight_E = cms.double(1.5),
        Vertex_Constraint_XY = cms.bool(False),
        DepositLabel = cms.untracked.string('EcalPlusHcal'),
        CaloTowerCollectionLabel = cms.InputTag("hltTowerMakerForMuons"),
        Weight_H = cms.double(1.0)
    )
)


hltL25TauCtfWithMaterialTracks = cms.EDProducer("TrackProducer",
    src = cms.InputTag("hltL25TauCkfTrackCandidates"),
    clusterRemovalInfo = cms.InputTag(""),
    beamSpot = cms.InputTag("hltOfflineBeamSpot"),
    Fitter = cms.string('FittingSmootherRK'),
    useHitsSplitting = cms.bool(False),
    alias = cms.untracked.string('ctfWithMaterialTracks'),
    NavigationSchool = cms.string(''),
    TrajectoryInEvent = cms.bool(True),
    TTRHBuilder = cms.string('WithTrackAngle'),
    AlgorithmName = cms.string('undefAlgorithm'),
    Propagator = cms.string('RungeKuttaTrackerPropagator')
)


hltPixelMatchElectronsL1IsoSS = cms.EDProducer("EgammaHLTPixelMatchElectronProducers",
    BSProducer = cms.InputTag("hltOfflineBeamSpot"),
    TrackProducer = cms.InputTag("hltCtfL1IsoSS")
)


hltBLifetimeL25AssociatorStartupU = cms.EDProducer("JetTracksAssociatorAtVertex",
    jets = cms.InputTag("hltBLifetimeL25JetsStartupU"),
    tracks = cms.InputTag("hltPixelTracks"),
    coneSize = cms.double(0.5)
)


hltBSoftMuonL3BJetTagsUByDR = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('softLeptonByDistance'),
    tagInfos = cms.VInputTag("hltBSoftMuonL3TagInfosU")
)


openHltBLifetimeRegionalPixelSeedGeneratorStartup = cms.EDProducer("SeedGeneratorFromRegionHitsEDProducer",
    SeedComparitorPSet = cms.PSet(
        ComponentName = cms.string('none')
    ),
    ClusterCheckPSet = cms.PSet(
        MaxNumberOfCosmicClusters = cms.uint32(50000),
        ClusterCollectionLabel = cms.InputTag("hltSiStripClusters"),
        doClusterCheck = cms.bool(False)
    ),
    RegionFactoryPSet = cms.PSet(
        ComponentName = cms.string('TauRegionalPixelSeedGenerator'),
        RegionPSet = cms.PSet(
            precise = cms.bool(True),
            deltaPhiRegion = cms.double(0.5),
            originHalfLength = cms.double(0.2),
            originRadius = cms.double(0.2),
            deltaEtaRegion = cms.double(0.5),
            ptMin = cms.double(1.0),
            JetSrc = cms.InputTag("hltIterativeCone5CaloJets"),
            originZPos = cms.double(0.0),
            vertexSrc = cms.InputTag("hltPixelVertices")
        )
    ),
    SeedCreatorPSet = cms.PSet(
        ComponentName = cms.string('SeedFromConsecutiveHitsCreator'),
        propagator = cms.string('PropagatorWithMaterial')
    ),
    OrderedHitsFactoryPSet = cms.PSet(
        ComponentName = cms.string('StandardHitPairGenerator'),
        SeedingLayers = cms.string('PixelLayerPairs')
    ),
    TTRHBuilder = cms.string('WithTrackAngle')
)


hltMCJetCorJetIcone5Regional = cms.EDProducer("CaloJetCorrectionProducer",
    alias = cms.untracked.string('corJetIcone5'),
    correctors = cms.vstring('MCJetCorrectorIcone5'),
    src = cms.InputTag("hltIterativeCone5CaloJetsRegional")
)


hltCosmicMuonSeedBarrelOnly = cms.EDProducer("CosmicMuonSeedGenerator",
    MaxSeeds = cms.int32(10),
    CSCRecSegmentLabel = cms.InputTag("hltCscSegments"),
    EnableDTMeasurement = cms.bool(True),
    MaxCSCChi2 = cms.double(300.0),
    MaxDTChi2 = cms.double(300.0),
    DTRecSegmentLabel = cms.InputTag("hltDt4DSegments"),
    EnableCSCMeasurement = cms.bool(False)
)


hltCkfL1NonIsoTrackCandidates = cms.EDProducer("CkfTrackCandidateMaker",
    src = cms.InputTag("hltL1NonIsoStartUpElectronPixelSeeds"),
    TransientInitialStateEstimatorParameters = cms.PSet(
        propagatorAlongTISE = cms.string('PropagatorWithMaterial'),
        numberMeasurementsForFit = cms.int32(4),
        propagatorOppositeTISE = cms.string('PropagatorWithMaterialOpposite')
    ),
    RedundantSeedCleaner = cms.string('CachingSeedCleanerBySharedInput'),
    cleanTrajectoryAfterInOut = cms.bool(False),
    useHitsSplitting = cms.bool(False),
    doSeedingRegionRebuilding = cms.bool(False),
    maxNSeeds = cms.uint32(100000),
    NavigationSchool = cms.string('SimpleNavigationSchool'),
    TrajectoryBuilder = cms.string('CkfTrajectoryBuilder'),
    TrajectoryCleaner = cms.string('TrajectoryCleanerBySharedHits')
)


hltL25TauPixelTracksIsolationSelector = cms.EDProducer("IsolatedTauJetsSelector",
    UseIsolationDiscriminator = cms.bool(True),
    UseInHLTOpen = cms.bool(False),
    MinimumTransverseMomentumLeadingTrack = cms.double(3.0),
    JetSrc = cms.VInputTag("hltL25TauPixelTracksConeIsolation")
)


hltCkfL1IsoSSTC = cms.EDProducer("CkfTrackCandidateMaker",
    src = cms.InputTag("hltL1IsoSiStripElectronPixelSeeds"),
    TransientInitialStateEstimatorParameters = cms.PSet(
        propagatorAlongTISE = cms.string('PropagatorWithMaterial'),
        numberMeasurementsForFit = cms.int32(4),
        propagatorOppositeTISE = cms.string('PropagatorWithMaterialOpposite')
    ),
    RedundantSeedCleaner = cms.string('CachingSeedCleanerBySharedInput'),
    cleanTrajectoryAfterInOut = cms.bool(False),
    useHitsSplitting = cms.bool(False),
    doSeedingRegionRebuilding = cms.bool(False),
    maxNSeeds = cms.uint32(100000),
    NavigationSchool = cms.string('SimpleNavigationSchool'),
    TrajectoryBuilder = cms.string('CkfTrajectoryBuilder'),
    TrajectoryCleaner = cms.string('TrajectoryCleanerBySharedHits')
)


hltCtfL1IsoWithMaterialTracks = cms.EDProducer("TrackProducer",
    src = cms.InputTag("hltCkfL1IsoTrackCandidates"),
    clusterRemovalInfo = cms.InputTag(""),
    beamSpot = cms.InputTag("hltOfflineBeamSpot"),
    Fitter = cms.string('hltKFFittingSmoother'),
    useHitsSplitting = cms.bool(False),
    alias = cms.untracked.string('ctfWithMaterialTracks'),
    NavigationSchool = cms.string(''),
    TrajectoryInEvent = cms.bool(True),
    TTRHBuilder = cms.string('WithTrackAngle'),
    AlgorithmName = cms.string('undefAlgorithm'),
    Propagator = cms.string('PropagatorWithMaterial')
)


hltJet30Ht = cms.EDProducer("METProducer",
    src = cms.InputTag("hltMCJetCorJetIcone5"),
    METType = cms.string('MET'),
    InputType = cms.string('CaloJetCollection'),
    HO_PhiResPar = cms.vdouble(0.02511),
    HF_PhiResPar = cms.vdouble(0.05022),
    calculateSignificance = cms.bool(False),
    EB_EtResPar = cms.vdouble(0.2, 0.03, 0.005),
    EE_PhiResPar = cms.vdouble(0.02511),
    HB_EtResPar = cms.vdouble(0.0, 1.22, 0.05),
    HB_PhiResPar = cms.vdouble(0.02511),
    alias = cms.string('HTMET'),
    HE_PhiResPar = cms.vdouble(0.02511),
    noHF = cms.bool(False),
    EE_EtResPar = cms.vdouble(0.2, 0.03, 0.005),
    globalThreshold = cms.double(30.0),
    HF_EtResPar = cms.vdouble(0.0, 1.82, 0.09),
    HE_EtResPar = cms.vdouble(0.0, 1.3, 0.05),
    HO_EtResPar = cms.vdouble(0.0, 1.3, 0.005),
    EB_PhiResPar = cms.vdouble(0.00502),
    onlyFiducialParticles = cms.bool(False)
)


hltIslandBasicClustersHI = cms.EDProducer("IslandClusterProducer",
    posCalc_x0 = cms.double(0.89),
    endcapHitCollection = cms.string('EcalRecHitsEE'),
    endcapHitProducer = cms.string('hltEcalRecHitAll'),
    barrelClusterCollection = cms.string('islandBarrelBasicClustersHI'),
    IslandEndcapSeedThr = cms.double(0.18),
    posCalc_t0_endc = cms.double(3.1),
    posCalc_t0_barl = cms.double(7.4),
    barrelShapeAssociation = cms.string('islandBarrelShapeAssoc'),
    endcapShapeAssociation = cms.string('islandEndcapShapeAssoc'),
    clustershapecollectionEE = cms.string('islandEndcapShape'),
    clustershapecollectionEB = cms.string('islandBarrelShape'),
    VerbosityLevel = cms.string('ERROR'),
    barrelHitProducer = cms.string('hltEcalRecHitAll'),
    IslandBarrelSeedThr = cms.double(0.5),
    posCalc_w0 = cms.double(4.2),
    barrelHitCollection = cms.string('EcalRecHitsEB'),
    posCalc_t0_endcPresh = cms.double(1.2),
    endcapClusterCollection = cms.string('islandEndcapBasicClustersHI'),
    posCalc_logweight = cms.bool(True)
)


hltIsolPixelTrackProdHB1E31 = cms.EDProducer("IsolatedPixelTrackCandidateProducer",
    EBEtaBoundary = cms.double(1.479),
    L1eTauJetsSource = cms.InputTag("hltL1extraParticles","Tau"),
    MaxVtxDXYSeed = cms.double(101.0),
    tauUnbiasCone = cms.double(1.2),
    PixelIsolationConeSizeAtEC = cms.double(40.0),
    L1GTSeedLabel = cms.InputTag("hltL1sIsoTrack1E31"),
    minPTrack = cms.double(5.0),
    maxPTrackForIsolation = cms.double(3.0),
    MagFieldRecordName = cms.string('VolumeBasedMagneticField'),
    VertexLabel = cms.InputTag("hltHITPixelVerticesHB"),
    PixelTracksSources = cms.VInputTag("hltHITPixelTracksHB"),
    MaxVtxDXYIsol = cms.double(101.0),
    tauAssociationCone = cms.double(0.0),
    ExtrapolationConeSize = cms.double(1.0)
)


hltPixelMatchElectronsL1NonIsoSS = cms.EDProducer("EgammaHLTPixelMatchElectronProducers",
    BSProducer = cms.InputTag("hltOfflineBeamSpot"),
    TrackProducer = cms.InputTag("hltCtfL1NonIsoSS")
)


simGctDigis = cms.EDProducer("L1GctEmulator",
    useImprovedTauAlgorithm = cms.bool(False),
    ignoreRCTTauVetoBitsForIsolation = cms.bool(False),
    postSamples = cms.uint32(1),
    preSamples = cms.uint32(1),
    inputLabel = cms.InputTag("simRctDigis"),
    writeInternalData = cms.bool(False),
    jetFinderType = cms.string('hardwareJetFinder')
)


hltCscSegments = cms.EDProducer("CSCSegmentProducer",
    inputObjects = cms.InputTag("hltCsc2DRecHits"),
    algo_psets = cms.VPSet(cms.PSet(
        chamber_types = cms.vstring('ME1/a', 
            'ME1/b', 
            'ME1/2', 
            'ME1/3', 
            'ME2/1', 
            'ME2/2', 
            'ME3/1', 
            'ME3/2', 
            'ME4/1', 
            'ME4/2'),
        algo_name = cms.string('CSCSegAlgoST'),
        parameters_per_chamber_type = cms.vint32(2, 1, 1, 1, 1, 
            1, 1, 1, 1, 1),
        algo_psets = cms.VPSet(cms.PSet(
            maxRatioResidualPrune = cms.double(3.0),
            yweightPenalty = cms.double(1.5),
            maxRecHitsInCluster = cms.int32(20),
            hitDropLimit6Hits = cms.double(0.3333),
            BPMinImprovement = cms.double(10000.0),
            tanPhiMax = cms.double(0.5),
            onlyBestSegment = cms.bool(False),
            dRPhiFineMax = cms.double(8.0),
            curvePenalty = cms.double(2.0),
            dXclusBoxMax = cms.double(4.0),
            BrutePruning = cms.bool(True),
            curvePenaltyThreshold = cms.double(0.85),
            hitDropLimit4Hits = cms.double(0.6),
            useShowering = cms.bool(False),
            CSCDebug = cms.untracked.bool(False),
            tanThetaMax = cms.double(1.2),
            minHitsPerSegment = cms.int32(3),
            yweightPenaltyThreshold = cms.double(1.0),
            dPhiFineMax = cms.double(0.025),
            hitDropLimit5Hits = cms.double(0.8),
            preClustering = cms.bool(True),
            maxDPhi = cms.double(999.0),
            maxDTheta = cms.double(999.0),
            Pruning = cms.bool(True),
            dYclusBoxMax = cms.double(8.0)
        ), 
            cms.PSet(
                maxRatioResidualPrune = cms.double(3.0),
                yweightPenalty = cms.double(1.5),
                maxRecHitsInCluster = cms.int32(24),
                hitDropLimit6Hits = cms.double(0.3333),
                BPMinImprovement = cms.double(10000.0),
                tanPhiMax = cms.double(0.5),
                onlyBestSegment = cms.bool(False),
                dRPhiFineMax = cms.double(8.0),
                curvePenalty = cms.double(2.0),
                dXclusBoxMax = cms.double(4.0),
                BrutePruning = cms.bool(True),
                curvePenaltyThreshold = cms.double(0.85),
                hitDropLimit4Hits = cms.double(0.6),
                useShowering = cms.bool(False),
                CSCDebug = cms.untracked.bool(False),
                tanThetaMax = cms.double(1.2),
                minHitsPerSegment = cms.int32(3),
                yweightPenaltyThreshold = cms.double(1.0),
                dPhiFineMax = cms.double(0.025),
                hitDropLimit5Hits = cms.double(0.8),
                preClustering = cms.bool(True),
                maxDPhi = cms.double(999.0),
                maxDTheta = cms.double(999.0),
                Pruning = cms.bool(True),
                dYclusBoxMax = cms.double(8.0)
            ))
    )),
    algo_type = cms.int32(1)
)


hltL3TkTracksFromL2NoVtx = cms.EDProducer("TrackProducer",
    src = cms.InputTag("hltL3TrackCandidateFromL2NoVtx"),
    clusterRemovalInfo = cms.InputTag(""),
    beamSpot = cms.InputTag("hltOfflineBeamSpot"),
    Fitter = cms.string('hltKFFittingSmoother'),
    useHitsSplitting = cms.bool(False),
    alias = cms.untracked.string(''),
    NavigationSchool = cms.string(''),
    TrajectoryInEvent = cms.bool(False),
    TTRHBuilder = cms.string('WithTrackAngle'),
    AlgorithmName = cms.string('undefAlgorithm'),
    Propagator = cms.string('PropagatorWithMaterial')
)


hltCtfL1NonIsoLargeWindowWithMaterialTracks = cms.EDProducer("TrackProducer",
    src = cms.InputTag("hltCkfL1NonIsoLargeWindowTrackCandidates"),
    clusterRemovalInfo = cms.InputTag(""),
    beamSpot = cms.InputTag("hltOfflineBeamSpot"),
    Fitter = cms.string('hltKFFittingSmoother'),
    useHitsSplitting = cms.bool(False),
    alias = cms.untracked.string('ctfWithMaterialTracks'),
    NavigationSchool = cms.string(''),
    TrajectoryInEvent = cms.bool(False),
    TTRHBuilder = cms.string('WithTrackAngle'),
    AlgorithmName = cms.string('undefAlgorithm'),
    Propagator = cms.string('PropagatorWithMaterial')
)


hltHITPixelTripletSeedGeneratorHB8E29 = cms.EDProducer("SeedGeneratorFromRegionHitsEDProducer",
    SeedComparitorPSet = cms.PSet(
        ComponentName = cms.string('none')
    ),
    ClusterCheckPSet = cms.PSet(
        MaxNumberOfCosmicClusters = cms.uint32(50000),
        ClusterCollectionLabel = cms.InputTag("siStripClusters"),
        doClusterCheck = cms.bool(False)
    ),
    RegionFactoryPSet = cms.PSet(
        ComponentName = cms.string('HITRegionalPixelSeedGenerator'),
        RegionPSet = cms.PSet(
            deltaEtaTrackRegion = cms.double(0.05),
            useL1Jets = cms.bool(False),
            deltaPhiTrackRegion = cms.double(0.05),
            isoTrackSrc = cms.InputTag("hltIsolPixelTrackL2FilterHB8E29"),
            precise = cms.bool(True),
            deltaEtaL1JetRegion = cms.double(0.3),
            useTracks = cms.bool(False),
            originRadius = cms.double(0.2),
            trackSrc = cms.InputTag("hltHITPixelTracksHB"),
            useIsoTracks = cms.bool(True),
            l1tjetSrc = cms.InputTag("hltl1extraParticles","Tau"),
            deltaPhiL1JetRegion = cms.double(0.3),
            vertexSrc = cms.string('hltHITPixelVerticesHB'),
            fixedReg = cms.bool(False),
            etaCenter = cms.double(0.0),
            phiCenter = cms.double(0.0),
            originZPos = cms.double(0.0),
            originHalfLength = cms.double(0.2),
            ptMin = cms.double(1.0)
        )
    ),
    SeedCreatorPSet = cms.PSet(
        ComponentName = cms.string('SeedFromConsecutiveHitsCreator'),
        propagator = cms.string('PropagatorWithMaterial')
    ),
    OrderedHitsFactoryPSet = cms.PSet(
        ComponentName = cms.string('StandardHitTripletGenerator'),
        SeedingLayers = cms.string('PixelLayerTriplets'),
        GeneratorPSet = cms.PSet(
            useBending = cms.bool(True),
            useFixedPreFiltering = cms.bool(False),
            ComponentName = cms.string('PixelTripletHLTGenerator'),
            extraHitRPhitolerance = cms.double(0.06),
            useMultScattering = cms.bool(True),
            phiPreFiltering = cms.double(0.3),
            extraHitRZtolerance = cms.double(0.06)
        )
    ),
    TTRHBuilder = cms.string('WithTrackAngle')
)


hltL1IsoElectronsRegionalPixelSeedGenerator = cms.EDProducer("EgammaHLTRegionalPixelSeedGeneratorProducers",
    deltaPhiRegion = cms.double(0.3),
    vertexZ = cms.double(0.0),
    originHalfLength = cms.double(0.5),
    BSProducer = cms.InputTag("hltOfflineBeamSpot"),
    UseZInVertex = cms.bool(True),
    OrderedHitsFactoryPSet = cms.PSet(
        ComponentName = cms.string('StandardHitPairGenerator'),
        SeedingLayers = cms.string('MixedLayerPairs')
    ),
    deltaEtaRegion = cms.double(0.3),
    ptMin = cms.double(1.5),
    candTag = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    candTagEle = cms.InputTag("hltPixelMatchElectronsL1Iso"),
    originRadius = cms.double(0.02)
)


hltL1NonIsoLWEleRegioCTF = cms.EDProducer("TrackProducer",
    src = cms.InputTag("hltL1NonIsoLWEleRegioCkfTC"),
    clusterRemovalInfo = cms.InputTag(""),
    beamSpot = cms.InputTag("hltOfflineBeamSpot"),
    Fitter = cms.string('hltKFFittingSmoother'),
    useHitsSplitting = cms.bool(False),
    alias = cms.untracked.string('hltL1NonIsoLWEleRegioCTF'),
    NavigationSchool = cms.string(''),
    TrajectoryInEvent = cms.bool(False),
    TTRHBuilder = cms.string('WithTrackAngle'),
    AlgorithmName = cms.string('undefAlgorithm'),
    Propagator = cms.string('PropagatorWithMaterial')
)


hltHIPixelTracks = cms.EDProducer("PixelTrackProducer",
    FilterPSet = cms.PSet(
        nSigmaTipMaxTolerance = cms.double(0.0),
        chi2 = cms.double(1000.0),
        ComponentName = cms.string('PixelTrackFilterByKinematics'),
        nSigmaInvPtTolerance = cms.double(0.0),
        ptMin = cms.double(0.1),
        tipMax = cms.double(1.0)
    ),
    useFilterWithES = cms.bool(False),
    FitterPSet = cms.PSet(
        ComponentName = cms.string('PixelFitterByHelixProjections'),
        TTRHBuilder = cms.string('TTRHBuilderPixelOnly')
    ),
    RegionFactoryPSet = cms.PSet(
        ComponentName = cms.string('HITrackingRegionProducer'),
        RegionPSet = cms.PSet(
            precise = cms.bool(True),
            originHalfLength = cms.double(15.9),
            directionYCoord = cms.double(1.0),
            originYPos = cms.double(0.0),
            directionXCoord = cms.double(1.0),
            directionZCoord = cms.double(0.0),
            VertexCollection = cms.string('hltHIPixelVertices'),
            ptMin = cms.double(0.5),
            originXPos = cms.double(0.0),
            siPixelRecHits = cms.string('hltSiPixelRecHits'),
            originZPos = cms.double(0.0),
            useFoundVertices = cms.bool(False),
            originRadius = cms.double(0.1)
        )
    ),
    CleanerPSet = cms.PSet(
        ComponentName = cms.string('PixelTrackCleanerBySharedHits')
    ),
    OrderedHitsFactoryPSet = cms.PSet(
        ComponentName = cms.string('StandardHitTripletGenerator'),
        GeneratorPSet = cms.PSet(
            useBending = cms.bool(True),
            useFixedPreFiltering = cms.bool(False),
            phiPreFiltering = cms.double(0.3),
            extraHitRPhitolerance = cms.double(0.06),
            useMultScattering = cms.bool(True),
            ComponentName = cms.string('PixelTripletHLTGenerator'),
            extraHitRZtolerance = cms.double(0.06)
        ),
        SeedingLayers = cms.string('PixelLayerTriplets')
    )
)


hltEcalCalibrationRaw = cms.EDProducer("SubdetFEDSelector",
    rawInputLabel = cms.InputTag("source"),
    getSiPixel = cms.bool(False),
    getHCAL = cms.bool(False),
    getECAL = cms.bool(True),
    getMuon = cms.bool(False),
    getTrigger = cms.bool(False),
    getSiStrip = cms.bool(False)
)


hltSiPixelRecHits = cms.EDProducer("SiPixelRecHitConverter",
    src = cms.InputTag("hltSiPixelClusters"),
    CPE = cms.string('PixelCPEGeneric')
)


hltCtfL1IsoLW = cms.EDProducer("TrackProducer",
    src = cms.InputTag("hltCkfL1IsoLWTC"),
    clusterRemovalInfo = cms.InputTag(""),
    beamSpot = cms.InputTag("hltOfflineBeamSpot"),
    Fitter = cms.string('hltKFFittingSmoother'),
    useHitsSplitting = cms.bool(False),
    alias = cms.untracked.string('hltCtfL1IsoLW'),
    NavigationSchool = cms.string(''),
    TrajectoryInEvent = cms.bool(True),
    TTRHBuilder = cms.string('WithTrackAngle'),
    AlgorithmName = cms.string('undefAlgorithm'),
    Propagator = cms.string('PropagatorWithMaterial')
)


hltL1HLTDoubleLooseIsoTau15JetsMatch = cms.EDProducer("L1HLTJetsMatching",
    JetSrc = cms.InputTag("hltL2TauRelaxingIsolationSelector","Isolated"),
    EtMin = cms.double(15.0),
    L1TauTrigger = cms.InputTag("hltL1sDoubleLooseIsoTau15")
)


hltHITCtfWithMaterialTracksHB8E29 = cms.EDProducer("TrackProducer",
    src = cms.InputTag("hltHITCkfTrackCandidatesHB8E29"),
    clusterRemovalInfo = cms.InputTag(""),
    beamSpot = cms.InputTag("hltOfflineBeamSpot"),
    Fitter = cms.string('hltKFFittingSmoother'),
    useHitsSplitting = cms.bool(False),
    alias = cms.untracked.string('hltHITCtfWithMaterialTracksHB1E31'),
    NavigationSchool = cms.string(''),
    TrajectoryInEvent = cms.bool(False),
    TTRHBuilder = cms.string('WithTrackAngle'),
    AlgorithmName = cms.string('undefAlgorithm'),
    Propagator = cms.string('PropagatorWithMaterial')
)


hltL1IsoEgammaRegionalCTFFinalFitWithMaterial = cms.EDProducer("TrackProducer",
    src = cms.InputTag("hltL1IsoEgammaRegionalCkfTrackCandidates"),
    clusterRemovalInfo = cms.InputTag(""),
    beamSpot = cms.InputTag("hltOfflineBeamSpot"),
    Fitter = cms.string('hltKFFittingSmoother'),
    useHitsSplitting = cms.bool(False),
    alias = cms.untracked.string('hltEgammaRegionalCTFFinalFitWithMaterial'),
    NavigationSchool = cms.string(''),
    TrajectoryInEvent = cms.bool(False),
    TTRHBuilder = cms.string('WithTrackAngle'),
    AlgorithmName = cms.string('undefAlgorithm'),
    Propagator = cms.string('PropagatorWithMaterial')
)


hltIterativeCone5CaloJetsRegional = cms.EDProducer("FastjetJetProducer",
    Active_Area_Repeats = cms.int32(5),
    doAreaFastjet = cms.bool(False),
    Ghost_EtaMax = cms.double(6.0),
    maxBadHcalCells = cms.uint32(9999999),
    maxRecoveredEcalCells = cms.uint32(9999999),
    jetType = cms.string('CaloJet'),
    doRhoFastjet = cms.bool(False),
    jetAlgorithm = cms.string('IterativeCone'),
    nSigmaPU = cms.double(1.0),
    GhostArea = cms.double(0.01),
    maxBadEcalCells = cms.uint32(9999999),
    doPVCorrection = cms.bool(False),
    maxRecoveredHcalCells = cms.uint32(9999999),
    rParam = cms.double(0.5),
    UseOnlyVertexTracks = cms.bool(False),
    UseOnlyOnePV = cms.bool(False),
    DzTrVtxMax = cms.double(0.0),
    maxProblematicHcalCells = cms.uint32(9999999),
    src = cms.InputTag("hltTowerMakerForJets"),
    inputEtMin = cms.double(0.5),
    srcPVs = cms.InputTag("offlinePrimaryVertices"),
    jetPtMin = cms.double(1.0),
    radiusPU = cms.double(0.5),
    maxProblematicEcalCells = cms.uint32(9999999),
    doPUOffsetCorr = cms.bool(False),
    inputEMin = cms.double(0.0)
)


hltHITIPTCorrectorHE1E31 = cms.EDProducer("IPTCorrector",
    corTracksLabel = cms.InputTag("hltHITCtfWithMaterialTracksHE1E31"),
    filterLabel = cms.InputTag("hltIsolPixelTrackL2FilterHE1E31"),
    associationCone = cms.double(0.2)
)


hltHITCkfTrackCandidatesHE1E31 = cms.EDProducer("CkfTrackCandidateMaker",
    src = cms.InputTag("hltHITPixelTripletSeedGeneratorHE1E31"),
    TransientInitialStateEstimatorParameters = cms.PSet(
        propagatorAlongTISE = cms.string('PropagatorWithMaterial'),
        numberMeasurementsForFit = cms.int32(4),
        propagatorOppositeTISE = cms.string('PropagatorWithMaterialOpposite')
    ),
    RedundantSeedCleaner = cms.string('CachingSeedCleanerBySharedInput'),
    cleanTrajectoryAfterInOut = cms.bool(False),
    useHitsSplitting = cms.bool(False),
    doSeedingRegionRebuilding = cms.bool(False),
    maxNSeeds = cms.uint32(100000),
    NavigationSchool = cms.string('SimpleNavigationSchool'),
    TrajectoryBuilder = cms.string('CkfTrajectoryBuilder'),
    TrajectoryCleaner = cms.string('TrajectoryCleanerBySharedHits')
)


hltL3TrackCandidateFromL2NoVtx = cms.EDProducer("CkfTrajectoryMaker",
    src = cms.InputTag("hltL3TrajectorySeedNoVtx"),
    TransientInitialStateEstimatorParameters = cms.PSet(
        propagatorAlongTISE = cms.string('PropagatorWithMaterial'),
        numberMeasurementsForFit = cms.int32(4),
        propagatorOppositeTISE = cms.string('PropagatorWithMaterialOpposite')
    ),
    RedundantSeedCleaner = cms.string('CachingSeedCleanerBySharedInput'),
    cleanTrajectoryAfterInOut = cms.bool(False),
    useHitsSplitting = cms.bool(False),
    doSeedingRegionRebuilding = cms.bool(False),
    maxNSeeds = cms.uint32(100000),
    NavigationSchool = cms.string('SimpleNavigationSchool'),
    TrajectoryBuilder = cms.string('muonCkfTrajectoryBuilder'),
    TrajectoryCleaner = cms.string('TrajectoryCleanerBySharedHits'),
    trackCandidateAlso = cms.bool(True)
)


hltL1NonIsoStartUpElectronPixelSeeds = cms.EDProducer("ElectronSeedProducer",
    endcapSuperClusters = cms.InputTag("hltCorrectedMulti5x5EndcapSuperClustersWithPreshowerL1NonIsolated"),
    SeedConfiguration = cms.PSet(
        searchInTIDTEC = cms.bool(True),
        HighPtThreshold = cms.double(35.0),
        r2MinF = cms.double(-0.08),
        OrderedHitsFactoryPSet = cms.PSet(
            ComponentName = cms.string('StandardHitPairGenerator'),
            SeedingLayers = cms.string('MixedLayerPairs'),
            useOnDemandTracker = cms.untracked.int32(0)
        ),
        DeltaPhi1Low = cms.double(0.23),
        DeltaPhi1High = cms.double(0.08),
        ePhiMin1 = cms.double(-0.035),
        PhiMin2 = cms.double(-0.005),
        LowPtThreshold = cms.double(3.0),
        RegionPSet = cms.PSet(
            deltaPhiRegion = cms.double(0.4),
            originHalfLength = cms.double(15.0),
            useZInVertex = cms.bool(True),
            deltaEtaRegion = cms.double(0.1),
            ptMin = cms.double(1.5),
            originRadius = cms.double(0.2),
            VertexProducer = cms.InputTag("dummyVertices")
        ),
        maxHOverE = cms.double(999999.0),
        dynamicPhiRoad = cms.bool(False),
        ePhiMax1 = cms.double(0.025),
        DeltaPhi2 = cms.double(0.004),
        SizeWindowENeg = cms.double(0.675),
        nSigmasDeltaZ1 = cms.double(5.0),
        rMaxI = cms.double(0.11),
        PhiMax2 = cms.double(0.005),
        preFilteredSeeds = cms.bool(True),
        r2MaxF = cms.double(0.08),
        pPhiMin1 = cms.double(-0.025),
        initialSeeds = cms.InputTag("noSeedsHere"),
        pPhiMax1 = cms.double(0.035),
        hbheModule = cms.string('hbhereco'),
        SCEtCut = cms.double(3.0),
        z2MaxB = cms.double(0.05),
        fromTrackerSeeds = cms.bool(True),
        hcalRecHits = cms.InputTag("hltHbhereco"),
        z2MinB = cms.double(-0.05),
        hbheInstance = cms.string(''),
        rMinI = cms.double(-0.11),
        hOverEConeSize = cms.double(0.0),
        hOverEHBMinE = cms.double(999999.0),
        applyHOverECut = cms.bool(False),
        hOverEHFMinE = cms.double(999999.0)
    ),
    barrelSuperClusters = cms.InputTag("hltCorrectedHybridSuperClustersL1NonIsolated")
)


hltL1NonIsoPhotonHollowTrackIsol = cms.EDProducer("EgammaHLTPhotonTrackIsolationProducersRegional",
    egTrkIsoConeSize = cms.double(0.3),
    countTracks = cms.bool(False),
    trackProducer = cms.InputTag("hltL1NonIsoEgammaRegionalCTFFinalFitWithMaterial"),
    egTrkIsoVetoConeSize = cms.double(0.06),
    egTrkIsoRSpan = cms.double(999999.0),
    recoEcalCandidateProducer = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    egTrkIsoPtMin = cms.double(1.5),
    egTrkIsoZSpan = cms.double(999999.0)
)


simRpcTechTrigDigis = cms.EDProducer("RPCTechnicalTrigger",
    BitNumbers = cms.vuint32(24, 25, 26, 27, 28, 
        29, 30),
    RPCDigiLabel = cms.InputTag("simMuonRPCDigis"),
    Verbosity = cms.untracked.int32(0),
    UseEventSetup = cms.untracked.int32(0),
    ConfigFile = cms.string('hardware-pseudoconfig.txt'),
    RPCSimLinkInstance = cms.InputTag("RPCDigiSimLink"),
    BitNames = cms.vstring('L1Tech_RPC_TTU_barrel_Cosmics/v0', 
        'L1Tech_RPC_TTU_pointing_Cosmics/v0', 
        'L1Tech_RPC_TTU_RBplus2_Cosmics/v0', 
        'L1Tech_RPC_TTU_RBplus1_Cosmics/v0', 
        'L1Tech_RPC_TTU_RB0_Cosmics/v0', 
        'L1Tech_RPC_TTU_RBminus1_Cosmics/v0', 
        'L1Tech_RPC_TTU_RBminus2_Cosmics/v0'),
    UseRPCSimLink = cms.untracked.int32(0)
)


hltL2TauRelaxingIsolationSelector = cms.EDProducer("L2TauRelaxingIsolationSelector",
    EcalIsolationEt = cms.vdouble(5.0, 0.025, 0.00075),
    SeedTowerEt = cms.double(-10.0),
    ClusterEtaRMS = cms.vdouble(1000.0, 0.0, 0.0),
    ClusterDRRMS = cms.vdouble(1000.0, 0.0, 0.0),
    NumberOfClusters = cms.vdouble(1000.0, 0.0, 0.0),
    L2InfoAssociation = cms.InputTag("hltL2TauNarrowConeIsolationProducer"),
    ClusterPhiRMS = cms.vdouble(1000.0, 0.0, 0.0),
    TowerIsolationEt = cms.vdouble(1000.0, 0.0, 0.0),
    MinJetEt = cms.double(15.0)
)


hltL1HLTSingleIsoTau30JetsMatch = cms.EDProducer("L1HLTJetsMatching",
    JetSrc = cms.InputTag("hltL3TauIsolationSelector"),
    EtMin = cms.double(30.0),
    L1TauTrigger = cms.InputTag("hltL1sSingleIsoTau30Trk5")
)


hltOfflineBeamSpot = cms.EDProducer("BeamSpotProducer")


hltL25TauJetPixelTracksAssociatorNoL2 = cms.EDProducer("JetTracksAssociatorAtVertex",
    jets = cms.InputTag("hltL2TauRelaxingIsolationSelector","Isolated"),
    tracks = cms.InputTag("hltPixelTracks"),
    coneSize = cms.double(0.5)
)


hltBSoftmuonL3TagInfos = cms.EDProducer("SoftLepton",
    muonSelection = cms.uint32(0),
    refineJetAxis = cms.uint32(0),
    primaryVertex = cms.InputTag("nominal"),
    leptons = cms.InputTag("hltL3Muons"),
    leptonQualityCut = cms.double(0.0),
    jets = cms.InputTag("hltBSoftmuonL25Jets"),
    leptonDeltaRCut = cms.double(0.4),
    leptonChi2Cut = cms.double(0.0)
)


simDtTriggerPrimitiveDigis = cms.EDProducer("DTTrigProd",
    debug = cms.untracked.bool(False),
    tTrigModeConfig = cms.PSet(
        debug = cms.untracked.bool(False),
        kFactor = cms.double(-2.0),
        vPropWire = cms.double(24.4),
        tofCorrType = cms.int32(1),
        tTrig = cms.double(500.0)
    ),
    digiTag = cms.InputTag("simMuonDTDigis"),
    DTTFSectorNumbering = cms.bool(True),
    tTrigMode = cms.string('DTTTrigSyncTOFCorr')
)


hltL1NonIsoSSEleTrackIsol = cms.EDProducer("EgammaHLTElectronTrackIsolationProducers",
    egTrkIsoVetoConeSize = cms.double(0.02),
    trackProducer = cms.InputTag("hltL1NonIsoSSEleRegioCTF"),
    electronProducer = cms.InputTag("hltPixelMatchElectronsL1NonIsoSS"),
    egTrkIsoConeSize = cms.double(0.2),
    egTrkIsoRSpan = cms.double(999999.0),
    egTrkIsoPtMin = cms.double(1.5),
    egTrkIsoZSpan = cms.double(0.1)
)


hltESRegionalEgammaRecHit = cms.EDProducer("EcalRawToRecHitProducer",
    splitOutput = cms.bool(False),
    rechitCollection = cms.string('EcalRecHitsES'),
    EErechitCollection = cms.string(''),
    EBrechitCollection = cms.string(''),
    lazyGetterTag = cms.InputTag("hltESRawToRecHitFacility"),
    sourceTag = cms.InputTag("hltEcalRegionalEgammaFEDs","es")
)


hltPixelCands = cms.EDProducer("ConcreteChargedCandidateProducer",
    src = cms.InputTag("hltPixelTracksForMinBias"),
    particleType = cms.string('pi+')
)


hltBSoftMuonL25TagInfosU = cms.EDProducer("SoftLepton",
    muonSelection = cms.uint32(0),
    refineJetAxis = cms.uint32(0),
    primaryVertex = cms.InputTag("nominal"),
    leptons = cms.InputTag("hltL2Muons"),
    leptonQualityCut = cms.double(0.0),
    jets = cms.InputTag("hltBSoftMuonL25JetsU"),
    leptonDeltaRCut = cms.double(0.4),
    leptonChi2Cut = cms.double(0.0)
)


hltElectronL1NonIsoLargeWindowDetaDphi = cms.EDProducer("EgammaHLTElectronDetaDphiProducer",
    electronProducer = cms.InputTag("hltPixelMatchLargeWindowElectronsL1NonIso"),
    BSProducer = cms.InputTag("hltOfflineBeamSpot")
)


hltBLifetimeRegionalPixelSeedGeneratorStartup = cms.EDProducer("SeedGeneratorFromRegionHitsEDProducer",
    SeedComparitorPSet = cms.PSet(
        ComponentName = cms.string('none')
    ),
    ClusterCheckPSet = cms.PSet(
        MaxNumberOfCosmicClusters = cms.uint32(50000),
        ClusterCollectionLabel = cms.InputTag("hltSiStripClusters"),
        doClusterCheck = cms.bool(False)
    ),
    RegionFactoryPSet = cms.PSet(
        ComponentName = cms.string('TauRegionalPixelSeedGenerator'),
        RegionPSet = cms.PSet(
            precise = cms.bool(True),
            deltaPhiRegion = cms.double(0.5),
            originHalfLength = cms.double(0.2),
            originRadius = cms.double(0.2),
            deltaEtaRegion = cms.double(0.5),
            ptMin = cms.double(1.0),
            JetSrc = cms.InputTag("hltBLifetimeL3JetsStartup"),
            originZPos = cms.double(0.0),
            vertexSrc = cms.InputTag("hltPixelVertices")
        )
    ),
    SeedCreatorPSet = cms.PSet(
        ComponentName = cms.string('SeedFromConsecutiveHitsCreator'),
        propagator = cms.string('PropagatorWithMaterial')
    ),
    OrderedHitsFactoryPSet = cms.PSet(
        ComponentName = cms.string('StandardHitPairGenerator'),
        SeedingLayers = cms.string('PixelLayerPairs')
    ),
    TTRHBuilder = cms.string('WithTrackAngle')
)


hltBLifetimeL3TagInfos = cms.EDProducer("TrackIPProducer",
    maximumTransverseImpactParameter = cms.double(0.2),
    minimumNumberOfHits = cms.int32(8),
    minimumTransverseMomentum = cms.double(1.0),
    primaryVertex = cms.InputTag("hltPixelVertices"),
    maximumLongitudinalImpactParameter = cms.double(17.0),
    computeProbabilities = cms.bool(False),
    ghostTrackPriorDeltaR = cms.double(0.03),
    jetTracks = cms.InputTag("hltBLifetimeL3Associator"),
    jetDirectionUsingGhostTrack = cms.bool(False),
    minimumNumberOfPixelHits = cms.int32(2),
    jetDirectionUsingTracks = cms.bool(False),
    computeGhostTrack = cms.bool(False),
    useTrackQuality = cms.bool(False),
    maximumChiSquared = cms.double(5.0)
)


hltBLifetimeL25BJetTagsStartupU = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('trackCounting3D2nd'),
    tagInfos = cms.VInputTag("hltBLifetimeL25TagInfosStartupU")
)


openHltBLifetimeL25TagInfos = cms.EDProducer("TrackIPProducer",
    maximumTransverseImpactParameter = cms.double(0.2),
    minimumNumberOfHits = cms.int32(3),
    minimumTransverseMomentum = cms.double(1.0),
    primaryVertex = cms.InputTag("hltPixelVertices"),
    maximumLongitudinalImpactParameter = cms.double(17.0),
    computeProbabilities = cms.bool(False),
    ghostTrackPriorDeltaR = cms.double(0.03),
    jetTracks = cms.InputTag("openHltBLifetimeL25Associator"),
    jetDirectionUsingGhostTrack = cms.bool(False),
    minimumNumberOfPixelHits = cms.int32(2),
    jetDirectionUsingTracks = cms.bool(False),
    computeGhostTrack = cms.bool(False),
    useTrackQuality = cms.bool(False),
    maximumChiSquared = cms.double(5.0)
)


hltBLifetimeRegionalPixelSeedGeneratorStartupU = cms.EDProducer("SeedGeneratorFromRegionHitsEDProducer",
    SeedComparitorPSet = cms.PSet(
        ComponentName = cms.string('none')
    ),
    ClusterCheckPSet = cms.PSet(
        MaxNumberOfCosmicClusters = cms.uint32(50000),
        ClusterCollectionLabel = cms.InputTag("hltSiStripClusters"),
        doClusterCheck = cms.bool(False)
    ),
    RegionFactoryPSet = cms.PSet(
        ComponentName = cms.string('TauRegionalPixelSeedGenerator'),
        RegionPSet = cms.PSet(
            precise = cms.bool(True),
            deltaPhiRegion = cms.double(0.5),
            originHalfLength = cms.double(0.2),
            originRadius = cms.double(0.2),
            deltaEtaRegion = cms.double(0.5),
            ptMin = cms.double(1.0),
            JetSrc = cms.InputTag("hltBLifetimeL3JetsStartupU"),
            originZPos = cms.double(0.0),
            vertexSrc = cms.InputTag("hltPixelVertices")
        )
    ),
    SeedCreatorPSet = cms.PSet(
        ComponentName = cms.string('SeedFromConsecutiveHitsCreator'),
        propagator = cms.string('PropagatorWithMaterial')
    ),
    OrderedHitsFactoryPSet = cms.PSet(
        ComponentName = cms.string('StandardHitPairGenerator'),
        SeedingLayers = cms.string('PixelLayerPairs')
    ),
    TTRHBuilder = cms.string('WithTrackAngle')
)


hltL1IsoHLTClusterShape = cms.EDProducer("EgammaHLTClusterShapeProducer",
    recoEcalCandidateProducer = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    ecalRechitEB = cms.InputTag("hltEcalRegionalEgammaRecHit","EcalRecHitsEB"),
    ecalRechitEE = cms.InputTag("hltEcalRegionalEgammaRecHit","EcalRecHitsEE"),
    isIeta = cms.bool(False)
)


hltJet15UHt = cms.EDProducer("METProducer",
    src = cms.InputTag("hltMCJetCorJetIcone5HF07"),
    METType = cms.string('MET'),
    InputType = cms.string('CaloJetCollection'),
    HO_PhiResPar = cms.vdouble(0.02511),
    HF_PhiResPar = cms.vdouble(0.05022),
    calculateSignificance = cms.bool(False),
    EB_EtResPar = cms.vdouble(0.2, 0.03, 0.005),
    EE_PhiResPar = cms.vdouble(0.02511),
    HB_EtResPar = cms.vdouble(0.0, 1.22, 0.05),
    HB_PhiResPar = cms.vdouble(0.02511),
    alias = cms.string('HTMET'),
    HE_PhiResPar = cms.vdouble(0.02511),
    noHF = cms.bool(False),
    EE_EtResPar = cms.vdouble(0.2, 0.03, 0.005),
    globalThreshold = cms.double(30.0),
    HF_EtResPar = cms.vdouble(0.0, 1.82, 0.09),
    HE_EtResPar = cms.vdouble(0.0, 1.3, 0.05),
    HO_EtResPar = cms.vdouble(0.0, 1.3, 0.005),
    EB_PhiResPar = cms.vdouble(0.00502),
    onlyFiducialParticles = cms.bool(False)
)


hltEcalRawToRecHitFacility = cms.EDProducer("EcalRawToRecHitFacility",
    sourceTag = cms.InputTag("source"),
    workerName = cms.string('')
)


hltBLifetimeRegionalCtfWithMaterialTracks = cms.EDProducer("TrackProducer",
    src = cms.InputTag("hltBLifetimeRegionalCkfTrackCandidates"),
    clusterRemovalInfo = cms.InputTag(""),
    beamSpot = cms.InputTag("hltOfflineBeamSpot"),
    Fitter = cms.string('FittingSmootherRK'),
    useHitsSplitting = cms.bool(False),
    alias = cms.untracked.string('ctfWithMaterialTracks'),
    NavigationSchool = cms.string(''),
    TrajectoryInEvent = cms.bool(True),
    TTRHBuilder = cms.string('WithTrackAngle'),
    AlgorithmName = cms.string('undefAlgorithm'),
    Propagator = cms.string('RungeKuttaTrackerPropagator')
)


hltPixelMatchElectronsL1IsoLW = cms.EDProducer("EgammaHLTPixelMatchElectronProducers",
    BSProducer = cms.InputTag("hltOfflineBeamSpot"),
    TrackProducer = cms.InputTag("hltCtfL1IsoLW")
)


hltCaloTowersCentral1 = cms.EDProducer("CaloTowerCreatorForTauHLT",
    towers = cms.InputTag("hltTowerMakerForAll"),
    TauId = cms.int32(0),
    TauTrigger = cms.InputTag("hltL1extraParticles","Central"),
    minimumE = cms.double(0.8),
    UseTowersInCone = cms.double(0.8),
    minimumEt = cms.double(0.5)
)


hltCaloTowersCentral3 = cms.EDProducer("CaloTowerCreatorForTauHLT",
    towers = cms.InputTag("hltTowerMakerForAll"),
    TauId = cms.int32(2),
    TauTrigger = cms.InputTag("hltL1extraParticles","Central"),
    minimumE = cms.double(0.8),
    UseTowersInCone = cms.double(0.8),
    minimumEt = cms.double(0.5)
)


hltCaloTowersCentral2 = cms.EDProducer("CaloTowerCreatorForTauHLT",
    towers = cms.InputTag("hltTowerMakerForAll"),
    TauId = cms.int32(1),
    TauTrigger = cms.InputTag("hltL1extraParticles","Central"),
    minimumE = cms.double(0.8),
    UseTowersInCone = cms.double(0.8),
    minimumEt = cms.double(0.5)
)


hltBSoftmuonL25BJetTags = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('softLeptonByDistance'),
    tagInfos = cms.VInputTag("hltBSoftmuonL25TagInfos")
)


hltHITIPTCorrectorHE8E29 = cms.EDProducer("IPTCorrector",
    corTracksLabel = cms.InputTag("hltHITCtfWithMaterialTracksHE8E29"),
    filterLabel = cms.InputTag("hltIsolPixelTrackL2FilterHE8E29"),
    associationCone = cms.double(0.2)
)


hltBLifetimeL25TagInfosStartupU = cms.EDProducer("TrackIPProducer",
    maximumTransverseImpactParameter = cms.double(0.2),
    minimumNumberOfHits = cms.int32(3),
    minimumTransverseMomentum = cms.double(1.0),
    primaryVertex = cms.InputTag("hltPixelVertices"),
    maximumLongitudinalImpactParameter = cms.double(17.0),
    computeProbabilities = cms.bool(False),
    ghostTrackPriorDeltaR = cms.double(0.03),
    jetTracks = cms.InputTag("hltBLifetimeL25AssociatorStartupU"),
    jetDirectionUsingGhostTrack = cms.bool(False),
    minimumNumberOfPixelHits = cms.int32(2),
    jetDirectionUsingTracks = cms.bool(False),
    computeGhostTrack = cms.bool(False),
    useTrackQuality = cms.bool(False),
    maximumChiSquared = cms.double(5.0)
)


hltCaloTowersTau4Regional = cms.EDProducer("CaloTowerCreatorForTauHLT",
    towers = cms.InputTag("hltTowerMakerForJets"),
    TauId = cms.int32(3),
    TauTrigger = cms.InputTag("hltL1extraParticles","Tau"),
    minimumE = cms.double(0.8),
    UseTowersInCone = cms.double(0.8),
    minimumEt = cms.double(0.5)
)


hltL25TauPixelSeeds = cms.EDProducer("SeedGeneratorFromRegionHitsEDProducer",
    SeedComparitorPSet = cms.PSet(
        ComponentName = cms.string('none')
    ),
    ClusterCheckPSet = cms.PSet(
        MaxNumberOfCosmicClusters = cms.uint32(50000),
        ClusterCollectionLabel = cms.InputTag("hltSiStripClusters"),
        doClusterCheck = cms.bool(False)
    ),
    RegionFactoryPSet = cms.PSet(
        ComponentName = cms.string('TauRegionalPixelSeedGenerator'),
        RegionPSet = cms.PSet(
            precise = cms.bool(True),
            deltaPhiRegion = cms.double(0.2),
            originHalfLength = cms.double(0.2),
            originRadius = cms.double(0.2),
            deltaEtaRegion = cms.double(0.2),
            vertexSrc = cms.InputTag("hltPixelVertices"),
            JetSrc = cms.InputTag("hltL2TauRelaxingIsolationSelector","Isolated"),
            originZPos = cms.double(0.0),
            ptMin = cms.double(4.0)
        )
    ),
    SeedCreatorPSet = cms.PSet(
        ComponentName = cms.string('SeedFromConsecutiveHitsCreator'),
        propagator = cms.string('PropagatorWithMaterial')
    ),
    OrderedHitsFactoryPSet = cms.PSet(
        ComponentName = cms.string('StandardHitPairGenerator'),
        SeedingLayers = cms.string('PixelLayerPairs')
    ),
    TTRHBuilder = cms.string('WithTrackAngle')
)


hltL1NonIsoEgammaRegionalCTFFinalFitWithMaterial = cms.EDProducer("TrackProducer",
    src = cms.InputTag("hltL1NonIsoEgammaRegionalCkfTrackCandidates"),
    clusterRemovalInfo = cms.InputTag(""),
    beamSpot = cms.InputTag("hltOfflineBeamSpot"),
    Fitter = cms.string('hltKFFittingSmoother'),
    useHitsSplitting = cms.bool(False),
    alias = cms.untracked.string('hltEgammaRegionalCTFFinalFitWithMaterial'),
    NavigationSchool = cms.string(''),
    TrajectoryInEvent = cms.bool(False),
    TTRHBuilder = cms.string('WithTrackAngle'),
    AlgorithmName = cms.string('undefAlgorithm'),
    Propagator = cms.string('PropagatorWithMaterial')
)


hltPixelMatchLargeWindowElectronsL1Iso = cms.EDProducer("EgammaHLTPixelMatchElectronProducers",
    BSProducer = cms.InputTag("hltOfflineBeamSpot"),
    TrackProducer = cms.InputTag("hltCtfL1IsoLargeWindowWithMaterialTracks")
)


hltCaloTowersTau3Regional = cms.EDProducer("CaloTowerCreatorForTauHLT",
    towers = cms.InputTag("hltTowerMakerForJets"),
    TauId = cms.int32(2),
    TauTrigger = cms.InputTag("hltL1extraParticles","Tau"),
    minimumE = cms.double(0.8),
    UseTowersInCone = cms.double(0.8),
    minimumEt = cms.double(0.5)
)


hltL1NonIsoLWEleRegPSG = cms.EDProducer("EgammaHLTRegionalPixelSeedGeneratorProducers",
    deltaPhiRegion = cms.double(0.3),
    vertexZ = cms.double(0.0),
    originHalfLength = cms.double(0.5),
    BSProducer = cms.InputTag("hltOfflineBeamSpot"),
    UseZInVertex = cms.bool(True),
    OrderedHitsFactoryPSet = cms.PSet(
        ComponentName = cms.string('StandardHitPairGenerator'),
        SeedingLayers = cms.string('MixedLayerPairs')
    ),
    deltaEtaRegion = cms.double(0.3),
    ptMin = cms.double(1.5),
    candTag = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    candTagEle = cms.InputTag("hltPixelMatchElectronsL1NonIsoLW"),
    originRadius = cms.double(0.02)
)


hltHITCtfWithMaterialTracksHB1E31 = cms.EDProducer("TrackProducer",
    src = cms.InputTag("hltHITCkfTrackCandidatesHB1E31"),
    clusterRemovalInfo = cms.InputTag(""),
    beamSpot = cms.InputTag("hltOfflineBeamSpot"),
    Fitter = cms.string('hltKFFittingSmoother'),
    useHitsSplitting = cms.bool(False),
    alias = cms.untracked.string('hltHITCtfWithMaterialTracksHB1E31'),
    NavigationSchool = cms.string(''),
    TrajectoryInEvent = cms.bool(False),
    TTRHBuilder = cms.string('WithTrackAngle'),
    AlgorithmName = cms.string('undefAlgorithm'),
    Propagator = cms.string('PropagatorWithMaterial')
)


hltStoppedHSCPIterativeCone5CaloJets = cms.EDProducer("FastjetJetProducer",
    Active_Area_Repeats = cms.int32(5),
    doAreaFastjet = cms.bool(False),
    Ghost_EtaMax = cms.double(6.0),
    maxBadHcalCells = cms.uint32(9999999),
    maxRecoveredEcalCells = cms.uint32(9999999),
    jetType = cms.string('CaloJet'),
    doRhoFastjet = cms.bool(False),
    jetAlgorithm = cms.string('IterativeCone'),
    nSigmaPU = cms.double(1.0),
    GhostArea = cms.double(0.01),
    maxBadEcalCells = cms.uint32(9999999),
    doPVCorrection = cms.bool(False),
    maxRecoveredHcalCells = cms.uint32(9999999),
    rParam = cms.double(0.5),
    UseOnlyVertexTracks = cms.bool(False),
    UseOnlyOnePV = cms.bool(False),
    DzTrVtxMax = cms.double(0.0),
    maxProblematicHcalCells = cms.uint32(9999999),
    src = cms.InputTag("hltStoppedHSCPTowerMakerForAll"),
    inputEtMin = cms.double(0.5),
    srcPVs = cms.InputTag("offlinePrimaryVertices"),
    jetPtMin = cms.double(1.0),
    radiusPU = cms.double(0.5),
    maxProblematicEcalCells = cms.uint32(9999999),
    doPUOffsetCorr = cms.bool(False),
    inputEMin = cms.double(0.0)
)


hltL1NonIsolatedPhotonEcalIsol = cms.EDProducer("EgammaHLTEcalRecIsolationProducer",
    etMinBarrel = cms.double(-9999.0),
    etMinEndcap = cms.double(-9999.0),
    tryBoth = cms.bool(True),
    eMinEndcap = cms.double(0.3),
    ecalEndcapRecHitCollection = cms.InputTag("EcalRecHitsEE"),
    intRadiusEndcap = cms.double(0.07),
    ecalEndcapRecHitProducer = cms.InputTag("hltEcalRegionalEgammaRecHit"),
    jurassicWidth = cms.double(0.02),
    ecalBarrelRecHitProducer = cms.InputTag("hltEcalRegionalEgammaRecHit"),
    extRadius = cms.double(0.4),
    useIsolEt = cms.bool(True),
    recoEcalCandidateProducer = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    ecalBarrelRecHitCollection = cms.InputTag("EcalRecHitsEB"),
    intRadiusBarrel = cms.double(0.045),
    subtract = cms.bool(False),
    eMinBarrel = cms.double(0.08)
)


hltL1NonIsolatedElectronHcalIsol = cms.EDProducer("EgammaHLTHcalIsolationProducersRegional",
    hfRecHitProducer = cms.InputTag("hltHfreco"),
    recoEcalCandidateProducer = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    egHcalIsoPtMin = cms.double(0.0),
    egHcalIsoConeSize = cms.double(0.15),
    hbRecHitProducer = cms.InputTag("hltHbhereco")
)


hltL1NonIsoLargeWindowElectronPixelSeeds = cms.EDProducer("ElectronSeedProducer",
    endcapSuperClusters = cms.InputTag("hltCorrectedMulti5x5EndcapSuperClustersWithPreshowerL1NonIsolated"),
    SeedConfiguration = cms.PSet(
        searchInTIDTEC = cms.bool(True),
        HighPtThreshold = cms.double(35.0),
        r2MinF = cms.double(-0.3),
        OrderedHitsFactoryPSet = cms.PSet(
            ComponentName = cms.string('StandardHitPairGenerator'),
            SeedingLayers = cms.string('MixedLayerPairs'),
            useOnDemandTracker = cms.untracked.int32(0)
        ),
        DeltaPhi1Low = cms.double(0.23),
        DeltaPhi1High = cms.double(0.08),
        ePhiMin1 = cms.double(-0.045),
        PhiMin2 = cms.double(-0.01),
        LowPtThreshold = cms.double(3.0),
        RegionPSet = cms.PSet(
            deltaPhiRegion = cms.double(0.4),
            originHalfLength = cms.double(15.0),
            useZInVertex = cms.bool(True),
            deltaEtaRegion = cms.double(0.1),
            ptMin = cms.double(1.5),
            originRadius = cms.double(0.2),
            VertexProducer = cms.InputTag("dummyVertices")
        ),
        maxHOverE = cms.double(999999.0),
        dynamicPhiRoad = cms.bool(False),
        ePhiMax1 = cms.double(0.03),
        DeltaPhi2 = cms.double(0.004),
        SizeWindowENeg = cms.double(0.675),
        nSigmasDeltaZ1 = cms.double(5.0),
        rMaxI = cms.double(0.2),
        PhiMax2 = cms.double(0.01),
        preFilteredSeeds = cms.bool(True),
        r2MaxF = cms.double(0.3),
        pPhiMin1 = cms.double(-0.03),
        initialSeeds = cms.InputTag("noSeedsHere"),
        pPhiMax1 = cms.double(0.045),
        hbheModule = cms.string('hbhereco'),
        SCEtCut = cms.double(3.0),
        z2MaxB = cms.double(0.2),
        fromTrackerSeeds = cms.bool(True),
        hcalRecHits = cms.InputTag("hltHbhereco"),
        z2MinB = cms.double(-0.2),
        hbheInstance = cms.string(''),
        rMinI = cms.double(-0.2),
        hOverEConeSize = cms.double(0.0),
        hOverEHBMinE = cms.double(999999.0),
        applyHOverECut = cms.bool(False),
        hOverEHFMinE = cms.double(999999.0)
    ),
    barrelSuperClusters = cms.InputTag("hltCorrectedHybridSuperClustersL1NonIsolated")
)


hltIcone2Central2 = cms.EDProducer("IterativeConeJetProducer",
    correctInputToSignalVertex = cms.bool(False),
    verbose = cms.untracked.bool(False),
    inputEtMin = cms.double(0.5),
    pvCollection = cms.InputTag("offlinePrimaryVertices"),
    jetPtMin = cms.double(0.0),
    coneRadius = cms.double(0.2),
    alias = cms.untracked.string('IC5CaloJet'),
    seedThreshold = cms.double(1.0),
    debugLevel = cms.untracked.int32(0),
    jetType = cms.untracked.string('CaloJet'),
    src = cms.InputTag("hltCaloTowersCentral2"),
    inputEMin = cms.double(0.0)
)


hltL2MuonCandidatesNoVtx = cms.EDProducer("L2MuonCandidateProducer",
    InputObjects = cms.InputTag("hltL2Muons")
)


hltTowerMakerForEcalABFit = cms.EDProducer("CaloTowersCreator",
    EBSumThreshold = cms.double(0.2),
    MomHBDepth = cms.double(0.2),
    EBWeight = cms.double(1.0),
    hfInput = cms.InputTag(""),
    AllowMissingInputs = cms.bool(True),
    UseSymEBTreshold = cms.bool(False),
    HBThreshold = cms.double(0.9),
    EESumThreshold = cms.double(0.45),
    HBGrid = cms.vdouble(),
    MomEEDepth = cms.double(0.0),
    EBThreshold = cms.double(0.09),
    UseEcalRecoveredHits = cms.bool(True),
    MomConstrMethod = cms.int32(1),
    MomHEDepth = cms.double(0.4),
    HcalThreshold = cms.double(-1000.0),
    HF2Weights = cms.vdouble(),
    HOWeights = cms.vdouble(),
    EEGrid = cms.vdouble(),
    HEDWeight = cms.double(1.0),
    EEWeights = cms.vdouble(),
    EEWeight = cms.double(1.0),
    UseHO = cms.bool(False),
    HBWeights = cms.vdouble(),
    HF1Weight = cms.double(1.0),
    HF2Grid = cms.vdouble(),
    HEDWeights = cms.vdouble(),
    HESThreshold = cms.double(1.4),
    HF1Grid = cms.vdouble(),
    EBWeights = cms.vdouble(),
    HOWeight = cms.double(1e-99),
    HESWeight = cms.double(1.0),
    hbheInput = cms.InputTag(""),
    HF2Weight = cms.double(1.0),
    HF2Threshold = cms.double(1.8),
    HcalAcceptSeverityLevel = cms.uint32(999),
    EEThreshold = cms.double(0.45),
    HOThresholdPlus1 = cms.double(1.1),
    HOThresholdPlus2 = cms.double(1.1),
    HF1Weights = cms.vdouble(),
    hoInput = cms.InputTag(""),
    HF1Threshold = cms.double(1.2),
    HOThresholdMinus1 = cms.double(1.1),
    HESGrid = cms.vdouble(),
    UseEtEETreshold = cms.bool(False),
    HESWeights = cms.vdouble(),
    UseSymEETreshold = cms.bool(False),
    HEDThreshold = cms.double(1.4),
    UseEtEBTreshold = cms.bool(False),
    EcutTower = cms.double(-1000.0),
    HEDGrid = cms.vdouble(),
    UseHcalRecoveredHits = cms.bool(True),
    HOThresholdMinus2 = cms.double(1.1),
    HOThreshold0 = cms.double(1.1),
    ecalInputs = cms.VInputTag("hltEcalRecHitAB:EcalRecHitsEB", "hltEcalRecHitAB:EcalRecHitsEE"),
    MomEBDepth = cms.double(0.3),
    HBWeight = cms.double(1.0),
    EBGrid = cms.vdouble(),
    HOGrid = cms.vdouble(),
    EcalAcceptSeverityLevel = cms.uint32(1)
)


hltBLifetimeL3JetsStartupU = cms.EDProducer("GetJetsFromHLTobject",
    jets = cms.InputTag("hltBLifetimeL25FilterStartupU")
)


hltRecoHIEcalCandidate = cms.EDProducer("EgammaHLTRecoEcalCandidateProducers",
    scIslandEndcapProducer = cms.InputTag("hltCorrectedIslandEndcapSuperClustersHI"),
    scHybridBarrelProducer = cms.InputTag("hltCorrectedIslandBarrelSuperClustersHI"),
    recoEcalCandidateCollection = cms.string('')
)


hltHybridSuperClustersL1NonIsolated = cms.EDProducer("EgammaHLTHybridClusterProducer",
    regionEtaMargin = cms.double(0.14),
    regionPhiMargin = cms.double(0.4),
    ecalhitcollection = cms.string('EcalRecHitsEB'),
    posCalc_logweight = cms.bool(True),
    doIsolated = cms.bool(False),
    basicclusterCollection = cms.string(''),
    posCalc_w0 = cms.double(4.2),
    l1UpperThr = cms.double(999.0),
    l1LowerThr = cms.double(5.0),
    eseed = cms.double(0.35),
    ethresh = cms.double(0.1),
    ewing = cms.double(1.0),
    step = cms.int32(10),
    debugLevel = cms.string('INFO'),
    l1TagIsolated = cms.InputTag("hltL1extraParticles","Isolated"),
    superclusterCollection = cms.string(''),
    posCalc_x0 = cms.double(0.89),
    HybridBarrelSeedThr = cms.double(1.5),
    l1TagNonIsolated = cms.InputTag("hltL1extraParticles","NonIsolated"),
    posCalc_t0 = cms.double(7.4),
    l1LowerThrIgnoreIsolation = cms.double(999.0),
    ecalhitproducer = cms.InputTag("hltEcalRegionalEgammaRecHit")
)


hltBLifetimeL25AssociatorStartup = cms.EDProducer("JetTracksAssociatorAtVertex",
    jets = cms.InputTag("hltBLifetimeL25JetsStartup"),
    tracks = cms.InputTag("hltPixelTracks"),
    coneSize = cms.double(0.5)
)


openHltBSoftmuonL3TagInfos = cms.EDProducer("SoftLepton",
    muonSelection = cms.uint32(0),
    refineJetAxis = cms.uint32(0),
    primaryVertex = cms.InputTag("nominal"),
    leptons = cms.InputTag("hltL3Muons"),
    leptonQualityCut = cms.double(0.0),
    jets = cms.InputTag("hltIterativeCone5CaloJets"),
    leptonDeltaRCut = cms.double(0.4),
    leptonChi2Cut = cms.double(0.0)
)


hltCkfL1IsoLargeWindowTrackCandidates = cms.EDProducer("CkfTrackCandidateMaker",
    src = cms.InputTag("hltL1IsoLargeWindowElectronPixelSeeds"),
    TransientInitialStateEstimatorParameters = cms.PSet(
        propagatorAlongTISE = cms.string('PropagatorWithMaterial'),
        numberMeasurementsForFit = cms.int32(4),
        propagatorOppositeTISE = cms.string('PropagatorWithMaterialOpposite')
    ),
    RedundantSeedCleaner = cms.string('CachingSeedCleanerBySharedInput'),
    cleanTrajectoryAfterInOut = cms.bool(False),
    useHitsSplitting = cms.bool(False),
    doSeedingRegionRebuilding = cms.bool(False),
    maxNSeeds = cms.uint32(100000),
    NavigationSchool = cms.string('SimpleNavigationSchool'),
    TrajectoryBuilder = cms.string('CkfTrajectoryBuilder'),
    TrajectoryCleaner = cms.string('TrajectoryCleanerBySharedHits')
)


hltL3TauPixelSeeds = cms.EDProducer("SeedGeneratorFromRegionHitsEDProducer",
    SeedComparitorPSet = cms.PSet(
        ComponentName = cms.string('none')
    ),
    ClusterCheckPSet = cms.PSet(
        MaxNumberOfCosmicClusters = cms.uint32(50000),
        ClusterCollectionLabel = cms.InputTag("hltSiStripClusters"),
        doClusterCheck = cms.bool(False)
    ),
    RegionFactoryPSet = cms.PSet(
        ComponentName = cms.string('TauRegionalPixelSeedGenerator'),
        RegionPSet = cms.PSet(
            precise = cms.bool(True),
            deltaPhiRegion = cms.double(0.5),
            originHalfLength = cms.double(0.2),
            originRadius = cms.double(0.2),
            deltaEtaRegion = cms.double(0.5),
            vertexSrc = cms.InputTag("hltPixelVertices"),
            JetSrc = cms.InputTag("hltL25TauLeadingTrackPtCutSelector"),
            originZPos = cms.double(0.0),
            ptMin = cms.double(0.9)
        )
    ),
    SeedCreatorPSet = cms.PSet(
        ComponentName = cms.string('SeedFromConsecutiveHitsCreator'),
        propagator = cms.string('PropagatorWithMaterial')
    ),
    OrderedHitsFactoryPSet = cms.PSet(
        ComponentName = cms.string('StandardHitPairGenerator'),
        SeedingLayers = cms.string('PixelLayerPairs')
    ),
    TTRHBuilder = cms.string('WithTrackAngle')
)


hltL1HLTSingleLooseIsoTau20JetsMatch = cms.EDProducer("L1HLTJetsMatching",
    JetSrc = cms.InputTag("hltL2TauRelaxingIsolationSelector","Isolated"),
    EtMin = cms.double(20.0),
    L1TauTrigger = cms.InputTag("hltL1sSingleLooseIsoTau20")
)


hltIterativeCone5PileupSubtractionCaloJets = cms.EDProducer("FastjetJetProducer",
    Active_Area_Repeats = cms.int32(5),
    doAreaFastjet = cms.bool(False),
    Ghost_EtaMax = cms.double(6.0),
    maxBadHcalCells = cms.uint32(9999999),
    maxRecoveredEcalCells = cms.uint32(9999999),
    jetType = cms.string('CaloJet'),
    doRhoFastjet = cms.bool(False),
    jetAlgorithm = cms.string('IterativeCone'),
    nSigmaPU = cms.double(1.0),
    GhostArea = cms.double(0.01),
    maxBadEcalCells = cms.uint32(9999999),
    doPVCorrection = cms.bool(False),
    maxRecoveredHcalCells = cms.uint32(9999999),
    rParam = cms.double(0.5),
    UseOnlyVertexTracks = cms.bool(False),
    UseOnlyOnePV = cms.bool(False),
    DzTrVtxMax = cms.double(0.0),
    maxProblematicHcalCells = cms.uint32(9999999),
    src = cms.InputTag("hltTowerMakerForAll"),
    inputEtMin = cms.double(0.5),
    srcPVs = cms.InputTag("offlinePrimaryVertices"),
    jetPtMin = cms.double(10.0),
    radiusPU = cms.double(0.5),
    maxProblematicEcalCells = cms.uint32(9999999),
    doPUOffsetCorr = cms.bool(True),
    inputEMin = cms.double(0.0)
)


hltHITPixelTripletSeedGeneratorHE8E29 = cms.EDProducer("SeedGeneratorFromRegionHitsEDProducer",
    SeedComparitorPSet = cms.PSet(
        ComponentName = cms.string('none')
    ),
    ClusterCheckPSet = cms.PSet(
        MaxNumberOfCosmicClusters = cms.uint32(50000),
        ClusterCollectionLabel = cms.InputTag("siStripClusters"),
        doClusterCheck = cms.bool(False)
    ),
    RegionFactoryPSet = cms.PSet(
        ComponentName = cms.string('HITRegionalPixelSeedGenerator'),
        RegionPSet = cms.PSet(
            deltaEtaTrackRegion = cms.double(0.05),
            useL1Jets = cms.bool(False),
            deltaPhiTrackRegion = cms.double(0.05),
            isoTrackSrc = cms.InputTag("hltIsolPixelTrackL2FilterHE8E29"),
            precise = cms.bool(True),
            deltaEtaL1JetRegion = cms.double(0.3),
            useTracks = cms.bool(False),
            originRadius = cms.double(0.2),
            trackSrc = cms.InputTag("hltHITPixelTracksHE"),
            useIsoTracks = cms.bool(True),
            l1tjetSrc = cms.InputTag("hltl1extraParticles","Tau"),
            deltaPhiL1JetRegion = cms.double(0.3),
            vertexSrc = cms.string('hltHITPixelVerticesHE'),
            fixedReg = cms.bool(False),
            etaCenter = cms.double(0.0),
            phiCenter = cms.double(0.0),
            originZPos = cms.double(0.0),
            originHalfLength = cms.double(0.2),
            ptMin = cms.double(0.5)
        )
    ),
    SeedCreatorPSet = cms.PSet(
        ComponentName = cms.string('SeedFromConsecutiveHitsCreator'),
        propagator = cms.string('PropagatorWithMaterial')
    ),
    OrderedHitsFactoryPSet = cms.PSet(
        ComponentName = cms.string('StandardHitTripletGenerator'),
        SeedingLayers = cms.string('PixelLayerTriplets'),
        GeneratorPSet = cms.PSet(
            useBending = cms.bool(True),
            useFixedPreFiltering = cms.bool(False),
            ComponentName = cms.string('PixelTripletHLTGenerator'),
            extraHitRPhitolerance = cms.double(0.06),
            useMultScattering = cms.bool(True),
            phiPreFiltering = cms.double(0.3),
            extraHitRZtolerance = cms.double(0.06)
        )
    ),
    TTRHBuilder = cms.string('WithTrackAngle')
)


hltL1IsoEgammaRegionalCkfTrackCandidates = cms.EDProducer("CkfTrackCandidateMaker",
    src = cms.InputTag("hltL1IsoEgammaRegionalPixelSeedGenerator"),
    TransientInitialStateEstimatorParameters = cms.PSet(
        propagatorAlongTISE = cms.string('PropagatorWithMaterial'),
        numberMeasurementsForFit = cms.int32(4),
        propagatorOppositeTISE = cms.string('PropagatorWithMaterialOpposite')
    ),
    RedundantSeedCleaner = cms.string('CachingSeedCleanerBySharedInput'),
    cleanTrajectoryAfterInOut = cms.bool(False),
    useHitsSplitting = cms.bool(False),
    doSeedingRegionRebuilding = cms.bool(False),
    maxNSeeds = cms.uint32(100000),
    NavigationSchool = cms.string('SimpleNavigationSchool'),
    TrajectoryBuilder = cms.string('CkfTrajectoryBuilder'),
    TrajectoryCleaner = cms.string('TrajectoryCleanerBySharedHits')
)


openHltBLifetimeL3BJetTags = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('trackCounting3D2nd'),
    tagInfos = cms.VInputTag(cms.InputTag("openHltBLifetimeL3TagInfos"))
)


simRctDigis = cms.EDProducer("L1RCTProducer",
    useDebugTpgScales = cms.bool(False),
    hcalDigis = cms.VInputTag(cms.InputTag("simHcalTriggerPrimitiveDigis")),
    ecalDigis = cms.VInputTag(cms.InputTag("simEcalTriggerPrimitiveDigis")),
    useEcal = cms.bool(True),
    useHcal = cms.bool(True),
    BunchCrossings = cms.vint32(0)
)


hltIsolPixelTrackProdHB8E29 = cms.EDProducer("IsolatedPixelTrackCandidateProducer",
    EBEtaBoundary = cms.double(1.479),
    L1eTauJetsSource = cms.InputTag("hltL1extraParticles","Tau"),
    MaxVtxDXYSeed = cms.double(101.0),
    tauUnbiasCone = cms.double(1.2),
    PixelIsolationConeSizeAtEC = cms.double(40.0),
    L1GTSeedLabel = cms.InputTag("hltL1sIsoTrack8E29"),
    minPTrack = cms.double(5.0),
    maxPTrackForIsolation = cms.double(3.0),
    MagFieldRecordName = cms.string('VolumeBasedMagneticField'),
    VertexLabel = cms.InputTag("hltHITPixelVerticesHB"),
    PixelTracksSources = cms.VInputTag("hltHITPixelTracksHB"),
    MaxVtxDXYIsol = cms.double(101.0),
    tauAssociationCone = cms.double(0.0),
    ExtrapolationConeSize = cms.double(1.0)
)


hltPixelVertices = cms.EDProducer("PixelVertexProducer",
    WtAverage = cms.bool(True),
    Method2 = cms.bool(True),
    beamSpot = cms.InputTag("hltOfflineBeamSpot"),
    Verbosity = cms.int32(0),
    UseError = cms.bool(True),
    TrackCollection = cms.InputTag("hltPixelTracks"),
    ZSeparation = cms.double(0.05),
    NTrkMin = cms.int32(2),
    ZOffset = cms.double(5.0),
    Finder = cms.string('DivisiveVertexFinder'),
    PtMin = cms.double(1.0)
)


hltIconeCentral1Regional = cms.EDProducer("FastjetJetProducer",
    Active_Area_Repeats = cms.int32(5),
    doAreaFastjet = cms.bool(False),
    Ghost_EtaMax = cms.double(6.0),
    maxBadHcalCells = cms.uint32(9999999),
    maxRecoveredEcalCells = cms.uint32(9999999),
    jetType = cms.string('CaloJet'),
    doRhoFastjet = cms.bool(False),
    jetAlgorithm = cms.string('IterativeCone'),
    nSigmaPU = cms.double(1.0),
    GhostArea = cms.double(0.01),
    maxBadEcalCells = cms.uint32(9999999),
    doPVCorrection = cms.bool(False),
    maxRecoveredHcalCells = cms.uint32(9999999),
    rParam = cms.double(0.2),
    UseOnlyVertexTracks = cms.bool(False),
    UseOnlyOnePV = cms.bool(False),
    DzTrVtxMax = cms.double(0.0),
    maxProblematicHcalCells = cms.uint32(9999999),
    src = cms.InputTag("hltCaloTowersCentral1Regional"),
    inputEtMin = cms.double(0.5),
    srcPVs = cms.InputTag("offlinePrimaryVertices"),
    jetPtMin = cms.double(1.0),
    radiusPU = cms.double(0.5),
    maxProblematicEcalCells = cms.uint32(9999999),
    doPUOffsetCorr = cms.bool(False),
    inputEMin = cms.double(0.0)
)


hltBLifetimeL3BJetTagsStartupU = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('trackCounting3D2nd'),
    tagInfos = cms.VInputTag("hltBLifetimeL3TagInfosStartupU")
)


hltL25TauCkfTrackCandidates = cms.EDProducer("CkfTrackCandidateMaker",
    src = cms.InputTag("hltL25TauPixelSeeds"),
    TransientInitialStateEstimatorParameters = cms.PSet(
        propagatorAlongTISE = cms.string('PropagatorWithMaterial'),
        numberMeasurementsForFit = cms.int32(4),
        propagatorOppositeTISE = cms.string('PropagatorWithMaterialOpposite')
    ),
    RedundantSeedCleaner = cms.string('CachingSeedCleanerBySharedInput'),
    cleanTrajectoryAfterInOut = cms.bool(False),
    useHitsSplitting = cms.bool(False),
    doSeedingRegionRebuilding = cms.bool(False),
    maxNSeeds = cms.uint32(100000),
    NavigationSchool = cms.string('SimpleNavigationSchool'),
    TrajectoryBuilder = cms.string('trajBuilderL3'),
    TrajectoryCleaner = cms.string('TrajectoryCleanerBySharedHits')
)


hltCtfL1NonIsoLW = cms.EDProducer("TrackProducer",
    src = cms.InputTag("hltCkfL1NonIsoLWTC"),
    clusterRemovalInfo = cms.InputTag(""),
    beamSpot = cms.InputTag("hltOfflineBeamSpot"),
    Fitter = cms.string('hltKFFittingSmoother'),
    useHitsSplitting = cms.bool(False),
    alias = cms.untracked.string('hltCtfL1NonIsoLW'),
    NavigationSchool = cms.string(''),
    TrajectoryInEvent = cms.bool(True),
    TTRHBuilder = cms.string('WithTrackAngle'),
    AlgorithmName = cms.string('undefAlgorithm'),
    Propagator = cms.string('PropagatorWithMaterial')
)


hltL3TauConeIsolation = cms.EDProducer("ConeIsolation",
    MinimumTransverseMomentumInIsolationRing = cms.double(1.0),
    VariableMinCone = cms.double(0.05),
    BeamSpotProducer = cms.InputTag("hltOfflineBeamSpot"),
    MinimumNumberOfHits = cms.int32(5),
    MinimumTransverseMomentum = cms.double(1.0),
    JetTrackSrc = cms.InputTag("hltL3TauJetTracksAssociator"),
    VariableMaxCone = cms.double(0.17),
    DeltaZetTrackVertex = cms.double(0.2),
    SignalCone = cms.double(0.15),
    MaximumTransverseImpactParameter = cms.double(300.0),
    MatchingCone = cms.double(0.2),
    useVertex = cms.bool(True),
    MinimumNumberOfPixelHits = cms.int32(2),
    useBeamSpot = cms.bool(True),
    MaximumNumberOfTracksIsolationRing = cms.int32(0),
    UseFixedSizeCone = cms.bool(True),
    VariableConeParameter = cms.double(3.5),
    IsolationCone = cms.double(0.5),
    MinimumTransverseMomentumLeadingTrack = cms.double(5.0),
    MaximumChiSquared = cms.double(100.0),
    vertexSrc = cms.InputTag("hltPixelVertices")
)


hltMulti5x5EndcapSuperClustersWithPreshowerL1NonIsolated = cms.EDProducer("PreshowerClusterProducer",
    assocSClusterCollection = cms.string(''),
    preshStripEnergyCut = cms.double(0.0),
    preshClusterCollectionY = cms.string('preshowerYClusters'),
    preshCalibGamma = cms.double(0.024),
    etThresh = cms.double(5.0),
    preshRecHitProducer = cms.InputTag("hltESRegionalEgammaRecHit","EcalRecHitsES"),
    preshClusterCollectionX = cms.string('preshowerXClusters'),
    preshCalibPlaneY = cms.double(0.7),
    preshCalibPlaneX = cms.double(1.0),
    preshCalibMIP = cms.double(9e-05),
    endcapSClusterProducer = cms.InputTag("hltMulti5x5SuperClustersL1NonIsolated","multi5x5EndcapSuperClusters"),
    preshNclust = cms.int32(4),
    debugLevel = cms.string(''),
    preshClusterEnergyCut = cms.double(0.0),
    preshSeededNstrip = cms.int32(15)
)


hltMuonRPCDigis = cms.EDProducer("RPCUnpackingModule",
    InputLabel = cms.InputTag("source"),
    doSynchro = cms.bool(False)
)


hltBSoftMuonL25BJetTagsByDR = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('softLeptonByDistance'),
    tagInfos = cms.VInputTag("hltBSoftMuonL25TagInfos")
)


hltL1IsoLWEleRegioCTF = cms.EDProducer("TrackProducer",
    src = cms.InputTag("hltL1IsoLWEleRegioCkfTC"),
    clusterRemovalInfo = cms.InputTag(""),
    beamSpot = cms.InputTag("hltOfflineBeamSpot"),
    Fitter = cms.string('hltKFFittingSmoother'),
    useHitsSplitting = cms.bool(False),
    alias = cms.untracked.string('hltL1IsoLWEleRegioCTF'),
    NavigationSchool = cms.string(''),
    TrajectoryInEvent = cms.bool(False),
    TTRHBuilder = cms.string('WithTrackAngle'),
    AlgorithmName = cms.string('undefAlgorithm'),
    Propagator = cms.string('PropagatorWithMaterial')
)


hltEcalRegionalJetsRecHit = cms.EDProducer("EcalRawToRecHitProducer",
    splitOutput = cms.bool(True),
    rechitCollection = cms.string('NotNeededsplitOutputTrue'),
    EErechitCollection = cms.string('EcalRecHitsEE'),
    EBrechitCollection = cms.string('EcalRecHitsEB'),
    lazyGetterTag = cms.InputTag("hltEcalRawToRecHitFacility"),
    sourceTag = cms.InputTag("hltEcalRegionalJetsFEDs")
)


hltL25TauPixelTracksLeadingTrackPtCutSelectorNoL2 = cms.EDProducer("IsolatedTauJetsSelector",
    UseIsolationDiscriminator = cms.bool(False),
    UseInHLTOpen = cms.bool(False),
    MinimumTransverseMomentumLeadingTrack = cms.double(3.0),
    JetSrc = cms.VInputTag("hltL25TauPixelTracksConeIsolationNoL2")
)


hltL1IsoLargeWindowElectronPixelSeeds = cms.EDProducer("ElectronSeedProducer",
    endcapSuperClusters = cms.InputTag("hltCorrectedMulti5x5EndcapSuperClustersWithPreshowerL1Isolated"),
    SeedConfiguration = cms.PSet(
        searchInTIDTEC = cms.bool(True),
        HighPtThreshold = cms.double(35.0),
        r2MinF = cms.double(-0.3),
        OrderedHitsFactoryPSet = cms.PSet(
            ComponentName = cms.string('StandardHitPairGenerator'),
            SeedingLayers = cms.string('MixedLayerPairs'),
            useOnDemandTracker = cms.untracked.int32(0)
        ),
        DeltaPhi1Low = cms.double(0.23),
        DeltaPhi1High = cms.double(0.08),
        ePhiMin1 = cms.double(-0.045),
        PhiMin2 = cms.double(-0.01),
        LowPtThreshold = cms.double(3.0),
        RegionPSet = cms.PSet(
            deltaPhiRegion = cms.double(0.4),
            originHalfLength = cms.double(15.0),
            useZInVertex = cms.bool(True),
            deltaEtaRegion = cms.double(0.1),
            ptMin = cms.double(1.5),
            originRadius = cms.double(0.2),
            VertexProducer = cms.InputTag("dummyVertices")
        ),
        maxHOverE = cms.double(999999.0),
        dynamicPhiRoad = cms.bool(False),
        ePhiMax1 = cms.double(0.03),
        DeltaPhi2 = cms.double(0.004),
        SizeWindowENeg = cms.double(0.675),
        nSigmasDeltaZ1 = cms.double(5.0),
        rMaxI = cms.double(0.2),
        PhiMax2 = cms.double(0.01),
        preFilteredSeeds = cms.bool(True),
        r2MaxF = cms.double(0.3),
        pPhiMin1 = cms.double(-0.03),
        initialSeeds = cms.InputTag("noSeedsHere"),
        pPhiMax1 = cms.double(0.045),
        hbheModule = cms.string('hbhereco'),
        SCEtCut = cms.double(3.0),
        z2MaxB = cms.double(0.2),
        fromTrackerSeeds = cms.bool(True),
        hcalRecHits = cms.InputTag("hltHbhereco"),
        z2MinB = cms.double(-0.2),
        hbheInstance = cms.string(''),
        rMinI = cms.double(-0.2),
        hOverEConeSize = cms.double(0.0),
        hOverEHBMinE = cms.double(999999.0),
        applyHOverECut = cms.bool(False),
        hOverEHFMinE = cms.double(999999.0)
    ),
    barrelSuperClusters = cms.InputTag("hltCorrectedHybridSuperClustersL1Isolated")
)


hltBSoftMuonL3TagInfos = cms.EDProducer("SoftLepton",
    muonSelection = cms.uint32(0),
    refineJetAxis = cms.uint32(0),
    primaryVertex = cms.InputTag("nominal"),
    leptons = cms.InputTag("hltL3Muons"),
    leptonQualityCut = cms.double(0.0),
    jets = cms.InputTag("hltBSoftMuonL25Jets"),
    leptonDeltaRCut = cms.double(0.4),
    leptonChi2Cut = cms.double(0.0)
)


hltIcone2Tau1 = cms.EDProducer("IterativeConeJetProducer",
    correctInputToSignalVertex = cms.bool(False),
    verbose = cms.untracked.bool(False),
    inputEtMin = cms.double(0.5),
    pvCollection = cms.InputTag("offlinePrimaryVertices"),
    jetPtMin = cms.double(0.0),
    coneRadius = cms.double(0.2),
    alias = cms.untracked.string('IC5CaloJet'),
    seedThreshold = cms.double(1.0),
    debugLevel = cms.untracked.int32(0),
    jetType = cms.untracked.string('CaloJet'),
    src = cms.InputTag("hltCaloTowersTau1"),
    inputEMin = cms.double(0.0)
)


hltIcone2Tau3 = cms.EDProducer("IterativeConeJetProducer",
    correctInputToSignalVertex = cms.bool(False),
    verbose = cms.untracked.bool(False),
    inputEtMin = cms.double(0.5),
    pvCollection = cms.InputTag("offlinePrimaryVertices"),
    jetPtMin = cms.double(0.0),
    coneRadius = cms.double(0.2),
    alias = cms.untracked.string('IC5CaloJet'),
    seedThreshold = cms.double(1.0),
    debugLevel = cms.untracked.int32(0),
    jetType = cms.untracked.string('CaloJet'),
    src = cms.InputTag("hltCaloTowersTau3"),
    inputEMin = cms.double(0.0)
)


hltIcone2Tau2 = cms.EDProducer("IterativeConeJetProducer",
    correctInputToSignalVertex = cms.bool(False),
    verbose = cms.untracked.bool(False),
    inputEtMin = cms.double(0.5),
    pvCollection = cms.InputTag("offlinePrimaryVertices"),
    jetPtMin = cms.double(0.0),
    coneRadius = cms.double(0.2),
    alias = cms.untracked.string('IC5CaloJet'),
    seedThreshold = cms.double(1.0),
    debugLevel = cms.untracked.int32(0),
    jetType = cms.untracked.string('CaloJet'),
    src = cms.InputTag("hltCaloTowersTau2"),
    inputEMin = cms.double(0.0)
)


hltL2TauIsolationSelectorNoCut = cms.EDProducer("L2TauIsolationSelector",
    L2InfoAssociation = cms.InputTag("hltL2TauIsolationProducer"),
    SeedTowerEt = cms.double(-10.0),
    ClusterEtaRMS = cms.double(1000.0),
    ClusterDRRMS = cms.double(1000.0),
    ECALIsolEt = cms.double(1000.0),
    TowerIsolEt = cms.double(1000.0),
    ClusterPhiRMS = cms.double(1000.0),
    ClusterNClusters = cms.int32(1000),
    MinJetEt = cms.double(15.0)
)


hltIcone2Tau4 = cms.EDProducer("IterativeConeJetProducer",
    correctInputToSignalVertex = cms.bool(False),
    verbose = cms.untracked.bool(False),
    inputEtMin = cms.double(0.5),
    pvCollection = cms.InputTag("offlinePrimaryVertices"),
    jetPtMin = cms.double(0.0),
    coneRadius = cms.double(0.2),
    alias = cms.untracked.string('IC5CaloJet'),
    seedThreshold = cms.double(1.0),
    debugLevel = cms.untracked.int32(0),
    jetType = cms.untracked.string('CaloJet'),
    src = cms.InputTag("hltCaloTowersTau4"),
    inputEMin = cms.double(0.0)
)


openHltBSoftmuonL3BJetTags = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('softLeptonByPt'),
    tagInfos = cms.VInputTag(cms.InputTag("openHltBSoftmuonL3TagInfos"))
)


hltL25TauPixelTracksIsolationSelectorNoL2 = cms.EDProducer("IsolatedTauJetsSelector",
    UseIsolationDiscriminator = cms.bool(False),
    UseInHLTOpen = cms.bool(False),
    MinimumTransverseMomentumLeadingTrack = cms.double(3.0),
    JetSrc = cms.VInputTag("hltL25TauPixelTracksConeIsolationNoL2")
)


hltMulti5x5EndcapSuperClustersWithPreshowerL1Isolated = cms.EDProducer("PreshowerClusterProducer",
    assocSClusterCollection = cms.string(''),
    preshStripEnergyCut = cms.double(0.0),
    preshClusterCollectionY = cms.string('preshowerYClusters'),
    preshCalibGamma = cms.double(0.024),
    etThresh = cms.double(5.0),
    preshRecHitProducer = cms.InputTag("hltESRegionalEgammaRecHit","EcalRecHitsES"),
    preshClusterCollectionX = cms.string('preshowerXClusters'),
    preshCalibPlaneY = cms.double(0.7),
    preshCalibPlaneX = cms.double(1.0),
    preshCalibMIP = cms.double(9e-05),
    endcapSClusterProducer = cms.InputTag("hltMulti5x5SuperClustersL1Isolated","multi5x5EndcapSuperClusters"),
    preshNclust = cms.int32(4),
    debugLevel = cms.string(''),
    preshClusterEnergyCut = cms.double(0.0),
    preshSeededNstrip = cms.int32(15)
)


hltBSoftMuonL3BJetTagsByDR = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('softLeptonByDistance'),
    tagInfos = cms.VInputTag("hltBSoftMuonL3TagInfos")
)


hltCtfL1IsoSS = cms.EDProducer("TrackProducer",
    src = cms.InputTag("hltCkfL1IsoSSTC"),
    clusterRemovalInfo = cms.InputTag(""),
    beamSpot = cms.InputTag("hltOfflineBeamSpot"),
    Fitter = cms.string('hltKFFittingSmoother'),
    useHitsSplitting = cms.bool(False),
    alias = cms.untracked.string('hltCtfL1IsoSS'),
    NavigationSchool = cms.string(''),
    TrajectoryInEvent = cms.bool(True),
    TTRHBuilder = cms.string('WithTrackAngle'),
    AlgorithmName = cms.string('undefAlgorithm'),
    Propagator = cms.string('PropagatorWithMaterial')
)


hltBLifetimeL3TagInfosStartupU = cms.EDProducer("TrackIPProducer",
    maximumTransverseImpactParameter = cms.double(0.2),
    minimumNumberOfHits = cms.int32(8),
    minimumTransverseMomentum = cms.double(1.0),
    primaryVertex = cms.InputTag("hltPixelVertices"),
    maximumLongitudinalImpactParameter = cms.double(17.0),
    computeProbabilities = cms.bool(False),
    ghostTrackPriorDeltaR = cms.double(0.03),
    jetTracks = cms.InputTag("hltBLifetimeL3AssociatorStartupU"),
    jetDirectionUsingGhostTrack = cms.bool(False),
    minimumNumberOfPixelHits = cms.int32(2),
    jetDirectionUsingTracks = cms.bool(False),
    computeGhostTrack = cms.bool(False),
    useTrackQuality = cms.bool(False),
    maximumChiSquared = cms.double(20.0)
)


hltL1NonIsoSSEleRegPSG = cms.EDProducer("EgammaHLTRegionalPixelSeedGeneratorProducers",
    deltaPhiRegion = cms.double(0.3),
    vertexZ = cms.double(0.0),
    originHalfLength = cms.double(0.5),
    BSProducer = cms.InputTag("hltOfflineBeamSpot"),
    UseZInVertex = cms.bool(True),
    OrderedHitsFactoryPSet = cms.PSet(
        ComponentName = cms.string('StandardHitPairGenerator'),
        SeedingLayers = cms.string('MixedLayerPairs')
    ),
    deltaEtaRegion = cms.double(0.3),
    ptMin = cms.double(1.5),
    candTag = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    candTagEle = cms.InputTag("hltPixelMatchElectronsL1NonIsoSS"),
    originRadius = cms.double(0.02)
)


hltBLifetimeRegionalCtfWithMaterialTracksStartupU = cms.EDProducer("TrackProducer",
    src = cms.InputTag("hltBLifetimeRegionalCkfTrackCandidatesStartupU"),
    clusterRemovalInfo = cms.InputTag(""),
    beamSpot = cms.InputTag("hltOfflineBeamSpot"),
    Fitter = cms.string('FittingSmootherRK'),
    useHitsSplitting = cms.bool(False),
    alias = cms.untracked.string('ctfWithMaterialTracks'),
    NavigationSchool = cms.string(''),
    TrajectoryInEvent = cms.bool(True),
    TTRHBuilder = cms.string('WithTrackAngle'),
    AlgorithmName = cms.string('undefAlgorithm'),
    Propagator = cms.string('RungeKuttaTrackerPropagator')
)


hltElectronL1NonIsoDetaDphi = cms.EDProducer("EgammaHLTElectronDetaDphiProducer",
    electronProducer = cms.InputTag("hltPixelMatchElectronsL1NonIso"),
    BSProducer = cms.InputTag("hltOfflineBeamSpot")
)


hltHbhereco = cms.EDProducer("HcalSimpleReconstructor",
    correctionPhaseNS = cms.double(13.0),
    digiLabel = cms.InputTag("hltHcalDigis"),
    samplesToAdd = cms.int32(4),
    Subdetector = cms.string('HBHE'),
    correctForTimeslew = cms.bool(True),
    dropZSmarkedPassed = cms.bool(True),
    correctForPhaseContainment = cms.bool(True),
    firstSample = cms.int32(4)
)


hltOniaCtfTracks = cms.EDProducer("TrackProducer",
    src = cms.InputTag("hltOniaCkfTrackCandidates"),
    clusterRemovalInfo = cms.InputTag(""),
    beamSpot = cms.InputTag("hltOfflineBeamSpot"),
    Fitter = cms.string('FittingSmootherRK'),
    useHitsSplitting = cms.bool(False),
    alias = cms.untracked.string('hltOniaCtfTracks'),
    NavigationSchool = cms.string(''),
    TrajectoryInEvent = cms.bool(True),
    TTRHBuilder = cms.string('WithTrackAngle'),
    AlgorithmName = cms.string('undefAlgorithm'),
    Propagator = cms.string('RungeKuttaTrackerPropagator')
)


hltL1IsoEgammaRegionalPixelSeedGenerator = cms.EDProducer("EgammaHLTRegionalPixelSeedGeneratorProducers",
    deltaPhiRegion = cms.double(0.3),
    vertexZ = cms.double(0.0),
    originHalfLength = cms.double(15.0),
    BSProducer = cms.InputTag("hltOfflineBeamSpot"),
    UseZInVertex = cms.bool(False),
    OrderedHitsFactoryPSet = cms.PSet(
        ComponentName = cms.string('StandardHitPairGenerator'),
        SeedingLayers = cms.string('PixelLayerPairs')
    ),
    deltaEtaRegion = cms.double(0.3),
    ptMin = cms.double(1.5),
    candTag = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    candTagEle = cms.InputTag("pixelMatchElectrons"),
    originRadius = cms.double(0.02)
)


hltBLifetimeL3AssociatorStartup = cms.EDProducer("JetTracksAssociatorAtVertex",
    jets = cms.InputTag("hltBLifetimeL3JetsStartup"),
    tracks = cms.InputTag("hltBLifetimeRegionalCtfWithMaterialTracksStartup"),
    coneSize = cms.double(0.5)
)


hltCaloTowersCentral4 = cms.EDProducer("CaloTowerCreatorForTauHLT",
    towers = cms.InputTag("hltTowerMakerForAll"),
    TauId = cms.int32(3),
    TauTrigger = cms.InputTag("hltL1extraParticles","Central"),
    minimumE = cms.double(0.8),
    UseTowersInCone = cms.double(0.8),
    minimumEt = cms.double(0.5)
)


hltCorrectedMulti5x5EndcapSuperClustersWithPreshowerL1NonIsolatedTemp = cms.EDProducer("EgammaSCCorrectionMaker",
    corectedSuperClusterCollection = cms.string(''),
    sigmaElectronicNoise = cms.double(0.15),
    superClusterAlgo = cms.string('Multi5x5'),
    etThresh = cms.double(5.0),
    rawSuperClusterProducer = cms.InputTag("hltMulti5x5EndcapSuperClustersWithPreshowerL1NonIsolated"),
    applyEnergyCorrection = cms.bool(True),
    fix_fCorrPset = cms.PSet(
        brLinearLowThr = cms.double(0.6),
        fBremVec = cms.vdouble(-0.04163, 0.08552, 0.95048, -0.002308, 1.077),
        brLinearHighThr = cms.double(6.0),
        fEtEtaVec = cms.vdouble(0.9746, -6.512, 0.0, 0.0, 0.02771, 
            4.983, 0.0, 0.0, -0.007288, -0.9446, 
            0.0, 0.0, 0.0, 0.0, 0.0, 
            1.0, 1.0)
    ),
    isl_fCorrPset = cms.PSet(

    ),
    VerbosityLevel = cms.string('ERROR'),
    dyn_fCorrPset = cms.PSet(

    ),
    hyb_fCorrPset = cms.PSet(

    ),
    recHitProducer = cms.InputTag("hltEcalRegionalEgammaRecHit","EcalRecHitsEE")
)


hltMulti5x5BasicClustersL1NonIsolated = cms.EDProducer("EgammaHLTMulti5x5ClusterProducer",
    endcapHitProducer = cms.InputTag("hltEcalRegionalEgammaRecHit"),
    barrelClusterCollection = cms.string('notused'),
    regionEtaMargin = cms.double(0.3),
    regionPhiMargin = cms.double(0.4),
    VerbosityLevel = cms.string('ERROR'),
    barrelHitCollection = cms.string('EcalRecHitsEB'),
    posCalc_t0_endcPresh = cms.double(1.2),
    posCalc_logweight = cms.bool(True),
    doIsolated = cms.bool(False),
    Multi5x5EndcapSeedThr = cms.double(0.5),
    posCalc_w0 = cms.double(4.2),
    l1UpperThr = cms.double(999.0),
    endcapClusterCollection = cms.string('multi5x5EndcapBasicClusters'),
    l1LowerThr = cms.double(5.0),
    posCalc_t0_endc = cms.double(3.1),
    l1TagIsolated = cms.InputTag("hltL1extraParticles","Isolated"),
    doEndcaps = cms.bool(True),
    Multi5x5BarrelSeedThr = cms.double(0.5),
    posCalc_x0 = cms.double(0.89),
    endcapHitCollection = cms.string('EcalRecHitsEE'),
    l1TagNonIsolated = cms.InputTag("hltL1extraParticles","NonIsolated"),
    barrelHitProducer = cms.InputTag("hltEcalRegionalEgammaRecHit"),
    l1LowerThrIgnoreIsolation = cms.double(999.0),
    posCalc_t0_barl = cms.double(7.4),
    doBarrel = cms.bool(False)
)


hltL1NonIsoEgammaRegionalCkfTrackCandidates = cms.EDProducer("CkfTrackCandidateMaker",
    src = cms.InputTag("hltL1NonIsoEgammaRegionalPixelSeedGenerator"),
    TransientInitialStateEstimatorParameters = cms.PSet(
        propagatorAlongTISE = cms.string('PropagatorWithMaterial'),
        numberMeasurementsForFit = cms.int32(4),
        propagatorOppositeTISE = cms.string('PropagatorWithMaterialOpposite')
    ),
    RedundantSeedCleaner = cms.string('CachingSeedCleanerBySharedInput'),
    cleanTrajectoryAfterInOut = cms.bool(False),
    useHitsSplitting = cms.bool(False),
    doSeedingRegionRebuilding = cms.bool(False),
    maxNSeeds = cms.uint32(100000),
    NavigationSchool = cms.string('SimpleNavigationSchool'),
    TrajectoryBuilder = cms.string('CkfTrajectoryBuilder'),
    TrajectoryCleaner = cms.string('TrajectoryCleanerBySharedHits')
)


openhltL25TauCkfTrackCandidates = cms.EDProducer("CkfTrackCandidateMaker",
    src = cms.InputTag("openhltL25TauPixelSeeds"),
    TransientInitialStateEstimatorParameters = cms.PSet(
        propagatorAlongTISE = cms.string('PropagatorWithMaterial'),
        numberMeasurementsForFit = cms.int32(4),
        propagatorOppositeTISE = cms.string('PropagatorWithMaterialOpposite')
    ),
    RedundantSeedCleaner = cms.string('CachingSeedCleanerBySharedInput'),
    cleanTrajectoryAfterInOut = cms.bool(False),
    useHitsSplitting = cms.bool(False),
    doSeedingRegionRebuilding = cms.bool(False),
    maxNSeeds = cms.uint32(100000),
    NavigationSchool = cms.string('SimpleNavigationSchool'),
    TrajectoryBuilder = cms.string('trajBuilderL3'),
    TrajectoryCleaner = cms.string('TrajectoryCleanerBySharedHits')
)


hltBSoftMuonL3TagInfosU = cms.EDProducer("SoftLepton",
    muonSelection = cms.uint32(0),
    refineJetAxis = cms.uint32(0),
    primaryVertex = cms.InputTag("nominal"),
    leptons = cms.InputTag("hltL3Muons"),
    leptonQualityCut = cms.double(0.0),
    jets = cms.InputTag("hltBSoftMuonL25JetsU"),
    leptonDeltaRCut = cms.double(0.4),
    leptonChi2Cut = cms.double(0.0)
)


hltBLifetimeL25BJetTags = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('trackCounting3D2nd'),
    tagInfos = cms.VInputTag("hltBLifetimeL25TagInfos")
)


hltMulti5x5SuperClustersL1NonIsolated = cms.EDProducer("Multi5x5SuperClusterProducer",
    barrelSuperclusterCollection = cms.string('multi5x5BarrelSuperClusters'),
    endcapEtaSearchRoad = cms.double(0.14),
    barrelClusterCollection = cms.string('multi5x5BarrelBasicClusters'),
    barrelPhiSearchRoad = cms.double(0.8),
    endcapClusterProducer = cms.string('hltMulti5x5BasicClustersL1NonIsolated'),
    dynamicPhiRoad = cms.bool(False),
    endcapPhiSearchRoad = cms.double(0.6),
    VerbosityLevel = cms.string('ERROR'),
    seedTransverseEnergyThreshold = cms.double(1.0),
    barrelClusterProducer = cms.string('notused'),
    endcapSuperclusterCollection = cms.string('multi5x5EndcapSuperClusters'),
    barrelEtaSearchRoad = cms.double(0.06),
    doBarrel = cms.bool(False),
    doEndcaps = cms.bool(True),
    endcapClusterCollection = cms.string('multi5x5EndcapBasicClusters'),
    bremRecoveryPset = cms.PSet(
        barrel = cms.PSet(

        ),
        endcap = cms.PSet(
            a = cms.double(47.85),
            c = cms.double(0.1201),
            b = cms.double(108.8)
        ),
        doEndcaps = cms.bool(True),
        doBarrel = cms.bool(False)
    )
)


hltHITCkfTrackCandidatesHB8E29 = cms.EDProducer("CkfTrackCandidateMaker",
    src = cms.InputTag("hltHITPixelTripletSeedGeneratorHB8E29"),
    TransientInitialStateEstimatorParameters = cms.PSet(
        propagatorAlongTISE = cms.string('PropagatorWithMaterial'),
        numberMeasurementsForFit = cms.int32(4),
        propagatorOppositeTISE = cms.string('PropagatorWithMaterialOpposite')
    ),
    RedundantSeedCleaner = cms.string('CachingSeedCleanerBySharedInput'),
    cleanTrajectoryAfterInOut = cms.bool(False),
    useHitsSplitting = cms.bool(False),
    doSeedingRegionRebuilding = cms.bool(False),
    maxNSeeds = cms.uint32(100000),
    NavigationSchool = cms.string('SimpleNavigationSchool'),
    TrajectoryBuilder = cms.string('CkfTrajectoryBuilder'),
    TrajectoryCleaner = cms.string('TrajectoryCleanerBySharedHits')
)


hltCkfL1IsoLWTC = cms.EDProducer("CkfTrackCandidateMaker",
    src = cms.InputTag("hltL1IsoLargeWindowElectronPixelSeeds"),
    TransientInitialStateEstimatorParameters = cms.PSet(
        propagatorAlongTISE = cms.string('PropagatorWithMaterial'),
        numberMeasurementsForFit = cms.int32(4),
        propagatorOppositeTISE = cms.string('PropagatorWithMaterialOpposite')
    ),
    RedundantSeedCleaner = cms.string('CachingSeedCleanerBySharedInput'),
    cleanTrajectoryAfterInOut = cms.bool(False),
    useHitsSplitting = cms.bool(False),
    doSeedingRegionRebuilding = cms.bool(False),
    maxNSeeds = cms.uint32(100000),
    NavigationSchool = cms.string('SimpleNavigationSchool'),
    TrajectoryBuilder = cms.string('CkfTrajectoryBuilder'),
    TrajectoryCleaner = cms.string('TrajectoryCleanerBySharedHits')
)


hltL3TrajectorySeedNoVtx = cms.EDProducer("TSGFromL2Muon",
    tkSeedGenerator = cms.string('TSGForRoadSearchOI'),
    TSGFromCombinedHits = cms.PSet(

    ),
    ServiceParameters = cms.PSet(
        Propagators = cms.untracked.vstring('SteppingHelixPropagatorOpposite', 
            'SteppingHelixPropagatorAlong'),
        RPCLayers = cms.bool(True),
        UseMuonNavigation = cms.untracked.bool(True)
    ),
    TSGFromPropagation = cms.PSet(

    ),
    TSGFromPixelTriplets = cms.PSet(

    ),
    MuonCollectionLabel = cms.InputTag("hltL2Muons"),
    TSGForRoadSearchOI = cms.PSet(
        propagatorCompatibleName = cms.string('SteppingHelixPropagatorOpposite'),
        option = cms.uint32(3),
        ComponentName = cms.string('TSGForRoadSearch'),
        errorMatrixPset = cms.PSet(
            action = cms.string('use'),
            atIP = cms.bool(True),
            errorMatrixValuesPSet = cms.PSet(
                pf3_V12 = cms.PSet(
                    action = cms.string('scale'),
                    values = cms.vdouble(1.0, 1.0, 1.0, 1.0, 1.0, 
                        1.0, 1.0, 1.0, 1.0, 1.0, 
                        1.0, 1.0)
                ),
                pf3_V13 = cms.PSet(
                    action = cms.string('scale'),
                    values = cms.vdouble(1.0, 1.0, 1.0, 1.0, 1.0, 
                        1.0, 1.0, 1.0, 1.0, 1.0, 
                        1.0, 1.0)
                ),
                pf3_V11 = cms.PSet(
                    action = cms.string('scale'),
                    values = cms.vdouble(3.0, 3.0, 3.0, 5.0, 4.0, 
                        5.0, 10.0, 7.0, 10.0, 10.0, 
                        10.0, 10.0)
                ),
                pf3_V14 = cms.PSet(
                    action = cms.string('scale'),
                    values = cms.vdouble(1.0, 1.0, 1.0, 1.0, 1.0, 
                        1.0, 1.0, 1.0, 1.0, 1.0, 
                        1.0, 1.0)
                ),
                yAxis = cms.vdouble(0.0, 1.0, 1.4, 10.0),
                pf3_V34 = cms.PSet(
                    action = cms.string('scale'),
                    values = cms.vdouble(1.0, 1.0, 1.0, 1.0, 1.0, 
                        1.0, 1.0, 1.0, 1.0, 1.0, 
                        1.0, 1.0)
                ),
                pf3_V15 = cms.PSet(
                    action = cms.string('scale'),
                    values = cms.vdouble(1.0, 1.0, 1.0, 1.0, 1.0, 
                        1.0, 1.0, 1.0, 1.0, 1.0, 
                        1.0, 1.0)
                ),
                pf3_V35 = cms.PSet(
                    action = cms.string('scale'),
                    values = cms.vdouble(1.0, 1.0, 1.0, 1.0, 1.0, 
                        1.0, 1.0, 1.0, 1.0, 1.0, 
                        1.0, 1.0)
                ),
                pf3_V45 = cms.PSet(
                    action = cms.string('scale'),
                    values = cms.vdouble(1.0, 1.0, 1.0, 1.0, 1.0, 
                        1.0, 1.0, 1.0, 1.0, 1.0, 
                        1.0, 1.0)
                ),
                pf3_V44 = cms.PSet(
                    action = cms.string('scale'),
                    values = cms.vdouble(3.0, 3.0, 3.0, 5.0, 4.0, 
                        5.0, 10.0, 7.0, 10.0, 10.0, 
                        10.0, 10.0)
                ),
                xAxis = cms.vdouble(0.0, 13.0, 30.0, 70.0, 1000.0),
                pf3_V23 = cms.PSet(
                    action = cms.string('scale'),
                    values = cms.vdouble(1.0, 1.0, 1.0, 1.0, 1.0, 
                        1.0, 1.0, 1.0, 1.0, 1.0, 
                        1.0, 1.0)
                ),
                pf3_V22 = cms.PSet(
                    action = cms.string('scale'),
                    values = cms.vdouble(3.0, 3.0, 3.0, 5.0, 4.0, 
                        5.0, 10.0, 7.0, 10.0, 10.0, 
                        10.0, 10.0)
                ),
                pf3_V55 = cms.PSet(
                    action = cms.string('scale'),
                    values = cms.vdouble(3.0, 3.0, 3.0, 5.0, 4.0, 
                        5.0, 10.0, 7.0, 10.0, 10.0, 
                        10.0, 10.0)
                ),
                zAxis = cms.vdouble(-3.14159, 3.14159),
                pf3_V33 = cms.PSet(
                    action = cms.string('scale'),
                    values = cms.vdouble(3.0, 3.0, 3.0, 5.0, 4.0, 
                        5.0, 10.0, 7.0, 10.0, 10.0, 
                        10.0, 10.0)
                ),
                pf3_V25 = cms.PSet(
                    action = cms.string('scale'),
                    values = cms.vdouble(1.0, 1.0, 1.0, 1.0, 1.0, 
                        1.0, 1.0, 1.0, 1.0, 1.0, 
                        1.0, 1.0)
                ),
                pf3_V24 = cms.PSet(
                    action = cms.string('scale'),
                    values = cms.vdouble(1.0, 1.0, 1.0, 1.0, 1.0, 
                        1.0, 1.0, 1.0, 1.0, 1.0, 
                        1.0, 1.0)
                )
            )
        ),
        propagatorName = cms.string('SteppingHelixPropagatorAlong'),
        manySeeds = cms.bool(False),
        copyMuonRecHit = cms.bool(False),
        maxChi2 = cms.double(40.0)
    ),
    MuonTrackingRegionBuilder = cms.PSet(

    ),
    TSGFromMixedPairs = cms.PSet(

    ),
    PCut = cms.double(2.5),
    TrackerSeedCleaner = cms.PSet(

    ),
    PtCut = cms.double(1.0),
    TSGForRoadSearchIOpxl = cms.PSet(

    ),
    TSGFromPixelPairs = cms.PSet(

    )
)


hltL1NonIsoRecoEcalCandidate = cms.EDProducer("EgammaHLTRecoEcalCandidateProducers",
    scIslandEndcapProducer = cms.InputTag("hltCorrectedMulti5x5EndcapSuperClustersWithPreshowerL1NonIsolated"),
    scHybridBarrelProducer = cms.InputTag("hltCorrectedHybridSuperClustersL1NonIsolated"),
    recoEcalCandidateCollection = cms.string('')
)


hltIconeTau2Regional = cms.EDProducer("FastjetJetProducer",
    Active_Area_Repeats = cms.int32(5),
    doAreaFastjet = cms.bool(False),
    Ghost_EtaMax = cms.double(6.0),
    maxBadHcalCells = cms.uint32(9999999),
    maxRecoveredEcalCells = cms.uint32(9999999),
    jetType = cms.string('CaloJet'),
    doRhoFastjet = cms.bool(False),
    jetAlgorithm = cms.string('IterativeCone'),
    nSigmaPU = cms.double(1.0),
    GhostArea = cms.double(0.01),
    maxBadEcalCells = cms.uint32(9999999),
    doPVCorrection = cms.bool(False),
    maxRecoveredHcalCells = cms.uint32(9999999),
    rParam = cms.double(0.2),
    UseOnlyVertexTracks = cms.bool(False),
    UseOnlyOnePV = cms.bool(False),
    DzTrVtxMax = cms.double(0.0),
    maxProblematicHcalCells = cms.uint32(9999999),
    src = cms.InputTag("hltCaloTowersTau2Regional"),
    inputEtMin = cms.double(0.5),
    srcPVs = cms.InputTag("offlinePrimaryVertices"),
    jetPtMin = cms.double(1.0),
    radiusPU = cms.double(0.5),
    maxProblematicEcalCells = cms.uint32(9999999),
    doPUOffsetCorr = cms.bool(False),
    inputEMin = cms.double(0.0)
)


hltSiStripClusters = cms.EDProducer("MeasurementTrackerSiStripRefGetterProducer",
    InputModuleLabel = cms.InputTag("hltSiStripRawToClustersFacility"),
    measurementTrackerName = cms.string('')
)


hltCorrectedMulti5x5EndcapSuperClustersWithPreshowerL1NonIsolated = cms.EDProducer("EgammaHLTRemoveDuplicatedSC",
    L1NonIsoSkimmedCollection = cms.string(''),
    L1IsoSC = cms.InputTag("hltCorrectedMulti5x5EndcapSuperClustersWithPreshowerL1Isolated"),
    L1NonIsoUskimmedSC = cms.InputTag("hltCorrectedMulti5x5EndcapSuperClustersWithPreshowerL1NonIsolatedTemp")
)


hltBSoftMuonL25JetsU = cms.EDProducer("EtMinCaloJetSelector",
    filter = cms.bool(False),
    src = cms.InputTag("hltSelector4JetsU"),
    etMin = cms.double(10.0)
)


hltL3MuonCandidatesNoVtx = cms.EDProducer("L3MuonCandidateProducer",
    InputObjects = cms.InputTag("hltL3MuonsNoVtx")
)


hltL1NonIsoEgammaRegionalPixelSeedGenerator = cms.EDProducer("EgammaHLTRegionalPixelSeedGeneratorProducers",
    deltaPhiRegion = cms.double(0.3),
    vertexZ = cms.double(0.0),
    originHalfLength = cms.double(15.0),
    BSProducer = cms.InputTag("hltOfflineBeamSpot"),
    UseZInVertex = cms.bool(False),
    OrderedHitsFactoryPSet = cms.PSet(
        ComponentName = cms.string('StandardHitPairGenerator'),
        SeedingLayers = cms.string('PixelLayerPairs')
    ),
    deltaEtaRegion = cms.double(0.3),
    ptMin = cms.double(1.5),
    candTag = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    candTagEle = cms.InputTag("pixelMatchElectrons"),
    originRadius = cms.double(0.02)
)


hltL1IsoPhotonHollowTrackIsol = cms.EDProducer("EgammaHLTPhotonTrackIsolationProducersRegional",
    egTrkIsoConeSize = cms.double(0.3),
    countTracks = cms.bool(False),
    trackProducer = cms.InputTag("hltL1IsoEgammaRegionalCTFFinalFitWithMaterial"),
    egTrkIsoVetoConeSize = cms.double(0.06),
    egTrkIsoRSpan = cms.double(999999.0),
    recoEcalCandidateProducer = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    egTrkIsoPtMin = cms.double(1.5),
    egTrkIsoZSpan = cms.double(999999.0)
)


hltPixelMatchLargeWindowElectronsL1NonIso = cms.EDProducer("EgammaHLTPixelMatchElectronProducers",
    BSProducer = cms.InputTag("hltOfflineBeamSpot"),
    TrackProducer = cms.InputTag("hltCtfL1NonIsoLargeWindowWithMaterialTracks")
)


openHltBLifetimeL3BJetTagsStartup = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('trackCounting3D2nd'),
    tagInfos = cms.VInputTag(cms.InputTag("openHltBLifetimeL3TagInfosStartup"))
)


hltL25TauConeIsolation = cms.EDProducer("ConeIsolation",
    MinimumTransverseMomentumInIsolationRing = cms.double(1.0),
    VariableMinCone = cms.double(0.05),
    BeamSpotProducer = cms.InputTag("hltOfflineBeamSpot"),
    MinimumNumberOfHits = cms.int32(5),
    MinimumTransverseMomentum = cms.double(1.0),
    JetTrackSrc = cms.InputTag("hltL25TauJetTracksAssociator"),
    VariableMaxCone = cms.double(0.17),
    DeltaZetTrackVertex = cms.double(0.2),
    SignalCone = cms.double(0.15),
    MaximumTransverseImpactParameter = cms.double(300.0),
    MatchingCone = cms.double(0.2),
    useVertex = cms.bool(True),
    MinimumNumberOfPixelHits = cms.int32(2),
    useBeamSpot = cms.bool(True),
    MaximumNumberOfTracksIsolationRing = cms.int32(0),
    UseFixedSizeCone = cms.bool(True),
    VariableConeParameter = cms.double(3.5),
    IsolationCone = cms.double(0.5),
    MinimumTransverseMomentumLeadingTrack = cms.double(5.0),
    MaximumChiSquared = cms.double(100.0),
    vertexSrc = cms.InputTag("hltPixelVertices")
)


hltBSoftmuonL3BJetTagsByDR = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('softLeptonByDistance'),
    tagInfos = cms.VInputTag("hltBSoftmuonL3TagInfos")
)


hltCsc2DRecHits = cms.EDProducer("CSCRecHitDProducer",
    XTasymmetry_ME1b = cms.double(0.0),
    XTasymmetry_ME1a = cms.double(0.0),
    XTasymmetry_ME41 = cms.double(0.0),
    ConstSyst_ME1b = cms.double(0.007),
    CSCStripxtalksOffset = cms.double(0.03),
    CSCUseCalibrations = cms.bool(True),
    CSCNoOfTimeBinsForDynamicPedestal = cms.int32(2),
    XTasymmetry_ME22 = cms.double(0.0),
    UseFivePoleFit = cms.bool(True),
    XTasymmetry_ME21 = cms.double(0.0),
    ConstSyst_ME21 = cms.double(0.0),
    ConstSyst_ME31 = cms.double(0.0),
    CSCStripClusterChargeCut = cms.double(25.0),
    UseFourPoleFit = cms.bool(True),
    ConstSyst_ME32 = cms.double(0.0),
    ConstSyst_ME1a = cms.double(0.022),
    readBadChambers = cms.bool(True),
    NoiseLevel_ME13 = cms.double(8.0),
    NoiseLevel_ME12 = cms.double(9.0),
    NoiseLevel_ME32 = cms.double(9.0),
    NoiseLevel_ME31 = cms.double(9.0),
    ConstSyst_ME22 = cms.double(0.0),
    ConstSyst_ME41 = cms.double(0.0),
    CSCStripPeakThreshold = cms.double(10.0),
    readBadChannels = cms.bool(True),
    UseParabolaFit = cms.bool(False),
    XTasymmetry_ME13 = cms.double(0.0),
    XTasymmetry_ME12 = cms.double(0.0),
    wireDigiTag = cms.InputTag("hltMuonCSCDigis","MuonCSCWireDigi"),
    ConstSyst_ME12 = cms.double(0.0),
    ConstSyst_ME13 = cms.double(0.0),
    XTasymmetry_ME31 = cms.double(0.0),
    XTasymmetry_ME32 = cms.double(0.0),
    UseAverageTime = cms.bool(False),
    NoiseLevel_ME1a = cms.double(7.0),
    NoiseLevel_ME1b = cms.double(8.0),
    CSCWireClusterDeltaT = cms.int32(1),
    CSCUseStaticPedestals = cms.bool(False),
    stripDigiTag = cms.InputTag("hltMuonCSCDigis","MuonCSCStripDigi"),
    CSCstripWireDeltaTime = cms.int32(8),
    NoiseLevel_ME21 = cms.double(9.0),
    NoiseLevel_ME22 = cms.double(9.0),
    NoiseLevel_ME41 = cms.double(9.0)
)


hltSiPixelDigis = cms.EDProducer("SiPixelRawToDigi",
    IncludeErrors = cms.bool(False),
    InputLabel = cms.InputTag("source")
)


hltL3MuonIsolations = cms.EDProducer("L3MuonIsolationProducer",
    OutputMuIsoDeposits = cms.bool(True),
    CutsPSet = cms.PSet(
        ConeSizes = cms.vdouble(0.24, 0.24, 0.24, 0.24, 0.24, 
            0.24, 0.24, 0.24, 0.24, 0.24, 
            0.24, 0.24, 0.24, 0.24, 0.24, 
            0.24, 0.24, 0.24, 0.24, 0.24, 
            0.24, 0.24, 0.24, 0.24, 0.24, 
            0.24),
        ComponentName = cms.string('SimpleCuts'),
        Thresholds = cms.vdouble(1.1, 1.1, 1.1, 1.1, 1.2, 
            1.1, 1.2, 1.1, 1.2, 1.0, 
            1.1, 1.0, 1.0, 1.1, 1.0, 
            1.0, 1.1, 0.9, 1.1, 0.9, 
            1.1, 1.0, 1.0, 0.9, 0.8, 
            0.1),
        maxNTracks = cms.int32(-1),
        EtaBounds = cms.vdouble(0.0435, 0.1305, 0.2175, 0.3045, 0.3915, 
            0.4785, 0.5655, 0.6525, 0.7395, 0.8265, 
            0.9135, 1.0005, 1.0875, 1.1745, 1.2615, 
            1.3485, 1.4355, 1.5225, 1.6095, 1.6965, 
            1.785, 1.88, 1.9865, 2.1075, 2.247, 
            2.411),
        applyCutsORmaxNTracks = cms.bool(False)
    ),
    TrackPt_Min = cms.double(-1.0),
    ExtractorPSet = cms.PSet(
        Chi2Prob_Min = cms.double(-1.0),
        Diff_z = cms.double(0.2),
        inputTrackCollection = cms.InputTag("hltPixelTracks"),
        ReferenceRadius = cms.double(6.0),
        BeamSpotLabel = cms.InputTag("hltOfflineBeamSpot"),
        ComponentName = cms.string('PixelTrackExtractor'),
        DR_Max = cms.double(0.24),
        Diff_r = cms.double(0.1),
        VetoLeadingTrack = cms.bool(True),
        DR_VetoPt = cms.double(0.025),
        DR_Veto = cms.double(0.01),
        NHits_Min = cms.uint32(0),
        Chi2Ndof_Max = cms.double(1e+64),
        Pt_Min = cms.double(-1.0),
        DepositLabel = cms.untracked.string('PXLS'),
        BeamlineOption = cms.string('BeamSpotFromEvent'),
        PropagateTracksToRadius = cms.bool(True),
        PtVeto_Min = cms.double(2.0)
    ),
    inputMuonCollection = cms.InputTag("hltL3Muons")
)


hltTowerMakerForMuons = cms.EDProducer("CaloTowersCreator",
    EBSumThreshold = cms.double(0.2),
    MomHBDepth = cms.double(0.2),
    EBWeight = cms.double(1.0),
    hfInput = cms.InputTag("hltHfreco"),
    AllowMissingInputs = cms.bool(False),
    UseSymEBTreshold = cms.bool(False),
    HBThreshold = cms.double(0.9),
    EESumThreshold = cms.double(0.45),
    HBGrid = cms.vdouble(),
    MomEEDepth = cms.double(0.0),
    EBThreshold = cms.double(0.09),
    UseEcalRecoveredHits = cms.bool(True),
    MomConstrMethod = cms.int32(1),
    MomHEDepth = cms.double(0.4),
    HcalThreshold = cms.double(-1000.0),
    HF2Weights = cms.vdouble(),
    HOWeights = cms.vdouble(),
    EEGrid = cms.vdouble(),
    HEDWeight = cms.double(1.0),
    EEWeights = cms.vdouble(),
    EEWeight = cms.double(1.0),
    UseHO = cms.bool(False),
    HBWeights = cms.vdouble(),
    HF1Weight = cms.double(1.0),
    HF2Grid = cms.vdouble(),
    HEDWeights = cms.vdouble(),
    HESThreshold = cms.double(1.4),
    HF1Grid = cms.vdouble(),
    EBWeights = cms.vdouble(),
    HOWeight = cms.double(1e-99),
    HESWeight = cms.double(1.0),
    hbheInput = cms.InputTag("hltHbhereco"),
    HF2Weight = cms.double(1.0),
    HF2Threshold = cms.double(1.8),
    HcalAcceptSeverityLevel = cms.uint32(999),
    EEThreshold = cms.double(0.45),
    HOThresholdPlus1 = cms.double(1.1),
    HOThresholdPlus2 = cms.double(1.1),
    HF1Weights = cms.vdouble(),
    hoInput = cms.InputTag("hltHoreco"),
    HF1Threshold = cms.double(1.2),
    HOThresholdMinus1 = cms.double(1.1),
    HESGrid = cms.vdouble(),
    UseEtEETreshold = cms.bool(False),
    HESWeights = cms.vdouble(),
    UseSymEETreshold = cms.bool(False),
    HEDThreshold = cms.double(1.4),
    UseEtEBTreshold = cms.bool(False),
    EcutTower = cms.double(-1000.0),
    HEDGrid = cms.vdouble(),
    UseHcalRecoveredHits = cms.bool(True),
    HOThresholdMinus2 = cms.double(1.1),
    HOThreshold0 = cms.double(1.1),
    ecalInputs = cms.VInputTag("hltEcalRegionalMuonsRecHit:EcalRecHitsEB", "hltEcalRegionalMuonsRecHit:EcalRecHitsEE"),
    MomEBDepth = cms.double(0.3),
    HBWeight = cms.double(1.0),
    EBGrid = cms.vdouble(),
    HOGrid = cms.vdouble(),
    EcalAcceptSeverityLevel = cms.uint32(1)
)


hltL25TauJetPixelTracksAssociator = cms.EDProducer("JetTracksAssociatorAtVertex",
    jets = cms.InputTag("hltL2TauRelaxingIsolationSelector","Isolated"),
    tracks = cms.InputTag("hltPixelTracks"),
    coneSize = cms.double(0.5)
)


hltHITPixelVerticesHE = cms.EDProducer("PixelVertexProducer",
    WtAverage = cms.bool(True),
    Method2 = cms.bool(True),
    beamSpot = cms.InputTag("hltOfflineBeamSpot"),
    Verbosity = cms.int32(0),
    UseError = cms.bool(True),
    TrackCollection = cms.InputTag("hltHITPixelTracksHE"),
    ZSeparation = cms.double(0.05),
    NTrkMin = cms.int32(2),
    ZOffset = cms.double(5.0),
    Finder = cms.string('DivisiveVertexFinder'),
    PtMin = cms.double(1.0)
)


hltHybridSuperClustersL1Isolated = cms.EDProducer("EgammaHLTHybridClusterProducer",
    regionEtaMargin = cms.double(0.14),
    regionPhiMargin = cms.double(0.4),
    ecalhitcollection = cms.string('EcalRecHitsEB'),
    posCalc_logweight = cms.bool(True),
    doIsolated = cms.bool(True),
    basicclusterCollection = cms.string(''),
    posCalc_w0 = cms.double(4.2),
    l1UpperThr = cms.double(999.0),
    l1LowerThr = cms.double(5.0),
    eseed = cms.double(0.35),
    ethresh = cms.double(0.1),
    ewing = cms.double(1.0),
    step = cms.int32(10),
    debugLevel = cms.string('INFO'),
    l1TagIsolated = cms.InputTag("hltL1extraParticles","Isolated"),
    superclusterCollection = cms.string(''),
    posCalc_x0 = cms.double(0.89),
    HybridBarrelSeedThr = cms.double(1.5),
    l1TagNonIsolated = cms.InputTag("hltL1extraParticles","NonIsolated"),
    posCalc_t0 = cms.double(7.4),
    l1LowerThrIgnoreIsolation = cms.double(999.0),
    ecalhitproducer = cms.InputTag("hltEcalRegionalEgammaRecHit")
)


hltBLifetimeL25TagInfos = cms.EDProducer("TrackIPProducer",
    maximumTransverseImpactParameter = cms.double(0.2),
    minimumNumberOfHits = cms.int32(3),
    minimumTransverseMomentum = cms.double(1.0),
    primaryVertex = cms.InputTag("hltPixelVertices"),
    maximumLongitudinalImpactParameter = cms.double(17.0),
    computeProbabilities = cms.bool(False),
    ghostTrackPriorDeltaR = cms.double(0.03),
    jetTracks = cms.InputTag("hltBLifetimeL25Associator"),
    jetDirectionUsingGhostTrack = cms.bool(False),
    minimumNumberOfPixelHits = cms.int32(2),
    jetDirectionUsingTracks = cms.bool(False),
    computeGhostTrack = cms.bool(False),
    useTrackQuality = cms.bool(False),
    maximumChiSquared = cms.double(5.0)
)


hltHITPixelVerticesHB = cms.EDProducer("PixelVertexProducer",
    WtAverage = cms.bool(True),
    Method2 = cms.bool(True),
    beamSpot = cms.InputTag("hltOfflineBeamSpot"),
    Verbosity = cms.int32(0),
    UseError = cms.bool(True),
    TrackCollection = cms.InputTag("hltHITPixelTracksHB"),
    ZSeparation = cms.double(0.05),
    NTrkMin = cms.int32(2),
    ZOffset = cms.double(5.0),
    Finder = cms.string('DivisiveVertexFinder'),
    PtMin = cms.double(1.0)
)


hltCorrectedHybridSuperClustersL1Isolated = cms.EDProducer("EgammaSCCorrectionMaker",
    corectedSuperClusterCollection = cms.string(''),
    sigmaElectronicNoise = cms.double(0.03),
    superClusterAlgo = cms.string('Hybrid'),
    etThresh = cms.double(5.0),
    rawSuperClusterProducer = cms.InputTag("hltHybridSuperClustersL1Isolated"),
    applyEnergyCorrection = cms.bool(True),
    fix_fCorrPset = cms.PSet(

    ),
    isl_fCorrPset = cms.PSet(

    ),
    VerbosityLevel = cms.string('ERROR'),
    dyn_fCorrPset = cms.PSet(

    ),
    hyb_fCorrPset = cms.PSet(
        brLinearLowThr = cms.double(1.1),
        fBremVec = cms.vdouble(-0.05208, 0.1331, 0.9196, -0.0005735, 1.343),
        brLinearHighThr = cms.double(8.0),
        fEtEtaVec = cms.vdouble(1.0012, -0.5714, 0.0, 0.0, 0.0, 
            0.5549, 12.74, 1.0448, 0.0, 0.0, 
            0.0, 0.0, 8.0, 1.023, -0.00181, 
            0.0, 0.0)
    ),
    recHitProducer = cms.InputTag("hltEcalRegionalEgammaRecHit","EcalRecHitsEB")
)


hltL1IsoStartUpElectronPixelSeeds = cms.EDProducer("ElectronSeedProducer",
    endcapSuperClusters = cms.InputTag("hltCorrectedMulti5x5EndcapSuperClustersWithPreshowerL1Isolated"),
    SeedConfiguration = cms.PSet(
        searchInTIDTEC = cms.bool(True),
        HighPtThreshold = cms.double(35.0),
        r2MinF = cms.double(-0.08),
        OrderedHitsFactoryPSet = cms.PSet(
            ComponentName = cms.string('StandardHitPairGenerator'),
            SeedingLayers = cms.string('MixedLayerPairs'),
            useOnDemandTracker = cms.untracked.int32(0)
        ),
        DeltaPhi1Low = cms.double(0.23),
        DeltaPhi1High = cms.double(0.08),
        ePhiMin1 = cms.double(-0.035),
        PhiMin2 = cms.double(-0.005),
        LowPtThreshold = cms.double(3.0),
        RegionPSet = cms.PSet(
            deltaPhiRegion = cms.double(0.4),
            originHalfLength = cms.double(15.0),
            useZInVertex = cms.bool(True),
            deltaEtaRegion = cms.double(0.1),
            ptMin = cms.double(1.5),
            originRadius = cms.double(0.2),
            VertexProducer = cms.InputTag("dummyVertices")
        ),
        maxHOverE = cms.double(999999.0),
        dynamicPhiRoad = cms.bool(False),
        ePhiMax1 = cms.double(0.025),
        DeltaPhi2 = cms.double(0.004),
        SizeWindowENeg = cms.double(0.675),
        nSigmasDeltaZ1 = cms.double(5.0),
        rMaxI = cms.double(0.11),
        PhiMax2 = cms.double(0.005),
        preFilteredSeeds = cms.bool(True),
        r2MaxF = cms.double(0.08),
        pPhiMin1 = cms.double(-0.025),
        initialSeeds = cms.InputTag("noSeedsHere"),
        pPhiMax1 = cms.double(0.035),
        hbheModule = cms.string('hbhereco'),
        SCEtCut = cms.double(3.0),
        z2MaxB = cms.double(0.05),
        fromTrackerSeeds = cms.bool(True),
        hcalRecHits = cms.InputTag("hltHbhereco"),
        z2MinB = cms.double(-0.05),
        hbheInstance = cms.string(''),
        rMinI = cms.double(-0.11),
        hOverEConeSize = cms.double(0.0),
        hOverEHBMinE = cms.double(999999.0),
        applyHOverECut = cms.bool(False),
        hOverEHFMinE = cms.double(999999.0)
    ),
    barrelSuperClusters = cms.InputTag("hltCorrectedHybridSuperClustersL1Isolated")
)


hltL1IsoRecoEcalCandidate = cms.EDProducer("EgammaHLTRecoEcalCandidateProducers",
    scIslandEndcapProducer = cms.InputTag("hltCorrectedMulti5x5EndcapSuperClustersWithPreshowerL1Isolated"),
    scHybridBarrelProducer = cms.InputTag("hltCorrectedHybridSuperClustersL1Isolated"),
    recoEcalCandidateCollection = cms.string('')
)


hltHITCtfWithMaterialTracksHE1E31 = cms.EDProducer("TrackProducer",
    src = cms.InputTag("hltHITCkfTrackCandidatesHE1E31"),
    clusterRemovalInfo = cms.InputTag(""),
    beamSpot = cms.InputTag("hltOfflineBeamSpot"),
    Fitter = cms.string('hltKFFittingSmoother'),
    useHitsSplitting = cms.bool(False),
    alias = cms.untracked.string('hltHITCtfWithMaterialTracksHE1E31'),
    NavigationSchool = cms.string(''),
    TrajectoryInEvent = cms.bool(False),
    TTRHBuilder = cms.string('WithTrackAngle'),
    AlgorithmName = cms.string('undefAlgorithm'),
    Propagator = cms.string('PropagatorWithMaterial')
)


hltCkfL1NonIsoSSTC = cms.EDProducer("CkfTrackCandidateMaker",
    src = cms.InputTag("hltL1NonIsoSiStripElectronPixelSeeds"),
    TransientInitialStateEstimatorParameters = cms.PSet(
        propagatorAlongTISE = cms.string('PropagatorWithMaterial'),
        numberMeasurementsForFit = cms.int32(4),
        propagatorOppositeTISE = cms.string('PropagatorWithMaterialOpposite')
    ),
    RedundantSeedCleaner = cms.string('CachingSeedCleanerBySharedInput'),
    cleanTrajectoryAfterInOut = cms.bool(False),
    useHitsSplitting = cms.bool(False),
    doSeedingRegionRebuilding = cms.bool(False),
    maxNSeeds = cms.uint32(100000),
    NavigationSchool = cms.string('SimpleNavigationSchool'),
    TrajectoryBuilder = cms.string('CkfTrajectoryBuilder'),
    TrajectoryCleaner = cms.string('TrajectoryCleanerBySharedHits')
)


hltTriggerSummaryAOD = cms.EDProducer("TriggerSummaryProducerAOD",
    processName = cms.string('@')
)


hltHITPixelTracksHE = cms.EDProducer("PixelTrackProducer",
    FilterPSet = cms.PSet(
        nSigmaTipMaxTolerance = cms.double(0.0),
        chi2 = cms.double(1000.0),
        ComponentName = cms.string('PixelTrackFilterByKinematics'),
        nSigmaInvPtTolerance = cms.double(0.0),
        ptMin = cms.double(0.35),
        tipMax = cms.double(1.0)
    ),
    useFilterWithES = cms.bool(False),
    FitterPSet = cms.PSet(
        ComponentName = cms.string('PixelFitterByConformalMappingAndLine'),
        TTRHBuilder = cms.string('TTRHBuilderPixelOnly'),
        fixImpactParameter = cms.double(0.0)
    ),
    RegionFactoryPSet = cms.PSet(
        ComponentName = cms.string('GlobalRegionProducerFromBeamSpot'),
        RegionPSet = cms.PSet(
            precise = cms.bool(True),
            originRadius = cms.double(0.0015),
            nSigmaZ = cms.double(3.0),
            ptMin = cms.double(0.35),
            beamSpot = cms.InputTag("hltOfflineBeamSpot")
        )
    ),
    CleanerPSet = cms.PSet(
        ComponentName = cms.string('PixelTrackCleanerBySharedHits')
    ),
    OrderedHitsFactoryPSet = cms.PSet(
        ComponentName = cms.string('StandardHitTripletGenerator'),
        SeedingLayers = cms.string('PixelLayerTripletsHITHE'),
        GeneratorPSet = cms.PSet(
            useBending = cms.bool(True),
            useFixedPreFiltering = cms.bool(False),
            phiPreFiltering = cms.double(0.3),
            extraHitRPhitolerance = cms.double(0.06),
            useMultScattering = cms.bool(True),
            ComponentName = cms.string('PixelTripletHLTGenerator'),
            extraHitRZtolerance = cms.double(0.06)
        )
    )
)


hltL25TauPixelTracksLeadingTrackPtCutSelector = cms.EDProducer("IsolatedTauJetsSelector",
    UseIsolationDiscriminator = cms.bool(False),
    UseInHLTOpen = cms.bool(False),
    MinimumTransverseMomentumLeadingTrack = cms.double(3.0),
    JetSrc = cms.VInputTag("hltL25TauPixelTracksConeIsolation")
)


hltEcalRegionalEgammaRecHit = cms.EDProducer("EcalRawToRecHitProducer",
    splitOutput = cms.bool(True),
    rechitCollection = cms.string('NotNeededsplitOutputTrue'),
    EErechitCollection = cms.string('EcalRecHitsEE'),
    EBrechitCollection = cms.string('EcalRecHitsEB'),
    lazyGetterTag = cms.InputTag("hltEcalRawToRecHitFacility"),
    sourceTag = cms.InputTag("hltEcalRegionalEgammaFEDs")
)


hltBLifetimeL3Jets = cms.EDProducer("GetJetsFromHLTobject",
    jets = cms.InputTag("hltBLifetimeL25filter")
)


hltL1HLTDoubleLooseIsoTau15Trk5JetsMatch = cms.EDProducer("L1HLTJetsMatching",
    JetSrc = cms.InputTag("hltL25TauLeadingTrackPtCutSelector"),
    EtMin = cms.double(15.0),
    L1TauTrigger = cms.InputTag("hltL1sDoubleLooseIsoTau15Trk5")
)


hltCaloTowersCentral3Regional = cms.EDProducer("CaloTowerCreatorForTauHLT",
    towers = cms.InputTag("hltTowerMakerForJets"),
    TauId = cms.int32(2),
    TauTrigger = cms.InputTag("hltL1extraParticles","Central"),
    minimumE = cms.double(0.8),
    UseTowersInCone = cms.double(0.8),
    minimumEt = cms.double(0.5)
)


hltEcalRegionalMuonsRecHit = cms.EDProducer("EcalRawToRecHitProducer",
    splitOutput = cms.bool(True),
    rechitCollection = cms.string('NotNeededsplitOutputTrue'),
    EErechitCollection = cms.string('EcalRecHitsEE'),
    EBrechitCollection = cms.string('EcalRecHitsEB'),
    lazyGetterTag = cms.InputTag("hltEcalRawToRecHitFacility"),
    sourceTag = cms.InputTag("hltEcalRegionalMuonsFEDs")
)


hltL1NonIsoElectronsRegionalPixelSeedGenerator = cms.EDProducer("EgammaHLTRegionalPixelSeedGeneratorProducers",
    deltaPhiRegion = cms.double(0.3),
    vertexZ = cms.double(0.0),
    originHalfLength = cms.double(0.5),
    BSProducer = cms.InputTag("hltOfflineBeamSpot"),
    UseZInVertex = cms.bool(True),
    OrderedHitsFactoryPSet = cms.PSet(
        ComponentName = cms.string('StandardHitPairGenerator'),
        SeedingLayers = cms.string('MixedLayerPairs')
    ),
    deltaEtaRegion = cms.double(0.3),
    ptMin = cms.double(1.5),
    candTag = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    candTagEle = cms.InputTag("hltPixelMatchElectronsL1NonIso"),
    originRadius = cms.double(0.02)
)


simBscDigis = cms.EDProducer("BSCTrigger",
    theHits = cms.InputTag("g4SimHits","BSCHits"),
    bitNumbers = cms.vuint32(32, 33, 34, 35, 36, 
        37, 38, 39, 40, 41, 
        42, 43),
    resolution = cms.double(3.0),
    bitNames = cms.vstring('L1Tech_BSC_minBias_inner_threshold1', 
        'L1Tech_BSC_minBias_inner_threshold2', 
        'L1Tech_BSC_minBias_OR', 
        'L1Tech_BSC_HighMultiplicity', 
        'L1Tech_BSC_halo_beam2_inner', 
        'L1Tech_BSC_halo_beam2_outer', 
        'L1Tech_BSC_halo_beam1_inner', 
        'L1Tech_BSC_halo_beam1_outer', 
        'L1Tech_BSC_minBias_threshold1', 
        'L1Tech_BSC_minBias_threshold2', 
        'L1Tech_BSC_splash_beam1', 
        'L1Tech_BSC_splash_beam2'),
    coincidence = cms.double(72.85)
)


hltESRawToRecHitFacility = cms.EDProducer("EcalRawToRecHitFacility",
    sourceTag = cms.InputTag("source"),
    workerName = cms.string('esRawToRecHit')
)


hltL1IsoElectronsRegionalCkfTrackCandidates = cms.EDProducer("CkfTrackCandidateMaker",
    src = cms.InputTag("hltL1IsoElectronsRegionalPixelSeedGenerator"),
    TransientInitialStateEstimatorParameters = cms.PSet(
        propagatorAlongTISE = cms.string('PropagatorWithMaterial'),
        numberMeasurementsForFit = cms.int32(4),
        propagatorOppositeTISE = cms.string('PropagatorWithMaterialOpposite')
    ),
    RedundantSeedCleaner = cms.string('CachingSeedCleanerBySharedInput'),
    cleanTrajectoryAfterInOut = cms.bool(False),
    useHitsSplitting = cms.bool(False),
    doSeedingRegionRebuilding = cms.bool(False),
    maxNSeeds = cms.uint32(100000),
    NavigationSchool = cms.string('SimpleNavigationSchool'),
    TrajectoryBuilder = cms.string('CkfTrajectoryBuilder'),
    TrajectoryCleaner = cms.string('TrajectoryCleanerBySharedHits')
)


hltBLifetimeL3Associator = cms.EDProducer("JetTracksAssociatorAtVertex",
    jets = cms.InputTag("hltBLifetimeL3Jets"),
    tracks = cms.InputTag("hltBLifetimeRegionalCtfWithMaterialTracks"),
    coneSize = cms.double(0.5)
)


hltOniaSeeds = cms.EDProducer("SeedGeneratorFromProtoTracksEDProducer",
    InputCollection = cms.InputTag("hltOniaPixelTrackSelector"),
    TTRHBuilder = cms.string('WithTrackAngle'),
    useProtoTrackKinematics = cms.bool(False)
)


dtTriggerPrimitiveDigis = cms.EDProducer("DTTrigProd",
    debug = cms.untracked.bool(False),
    tTrigModeConfig = cms.PSet(
        debug = cms.untracked.bool(False),
        kFactor = cms.double(-2.0),
        vPropWire = cms.double(24.4),
        tofCorrType = cms.int32(1),
        tTrig = cms.double(500.0)
    ),
    digiTag = cms.InputTag("muonDTDigis"),
    tTrigMode = cms.string('DTTTrigSyncTOFCorr'),
    DTTFSectorNumbering = cms.bool(True)
)


hltL3TauIsolationSelector = cms.EDProducer("IsolatedTauJetsSelector",
    UseIsolationDiscriminator = cms.bool(True),
    UseInHLTOpen = cms.bool(False),
    MinimumTransverseMomentumLeadingTrack = cms.double(5.0),
    JetSrc = cms.VInputTag("hltL3TauConeIsolation")
)


hltBLifetimeRegionalCkfTrackCandidates = cms.EDProducer("CkfTrackCandidateMaker",
    src = cms.InputTag("hltBLifetimeRegionalPixelSeedGenerator"),
    TransientInitialStateEstimatorParameters = cms.PSet(
        propagatorAlongTISE = cms.string('PropagatorWithMaterial'),
        numberMeasurementsForFit = cms.int32(4),
        propagatorOppositeTISE = cms.string('PropagatorWithMaterialOpposite')
    ),
    RedundantSeedCleaner = cms.string('CachingSeedCleanerBySharedInput'),
    cleanTrajectoryAfterInOut = cms.bool(False),
    useHitsSplitting = cms.bool(False),
    doSeedingRegionRebuilding = cms.bool(False),
    maxNSeeds = cms.uint32(100000),
    NavigationSchool = cms.string('SimpleNavigationSchool'),
    TrajectoryBuilder = cms.string('bJetRegionalTrajectoryBuilder'),
    TrajectoryCleaner = cms.string('TrajectoryCleanerBySharedHits')
)


hltL1NonIsolatedPhotonHcalIsol = cms.EDProducer("EgammaHLTHcalIsolationProducersRegional",
    hfRecHitProducer = cms.InputTag("hltHfreco"),
    recoEcalCandidateProducer = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    egHcalIsoPtMin = cms.double(0.0),
    egHcalIsoConeSize = cms.double(0.3),
    hbRecHitProducer = cms.InputTag("hltHbhereco")
)


hltL1IsoSSEleRegPSG = cms.EDProducer("EgammaHLTRegionalPixelSeedGeneratorProducers",
    deltaPhiRegion = cms.double(0.3),
    vertexZ = cms.double(0.0),
    originHalfLength = cms.double(0.5),
    BSProducer = cms.InputTag("hltOfflineBeamSpot"),
    UseZInVertex = cms.bool(True),
    OrderedHitsFactoryPSet = cms.PSet(
        ComponentName = cms.string('StandardHitPairGenerator'),
        SeedingLayers = cms.string('MixedLayerPairs')
    ),
    deltaEtaRegion = cms.double(0.3),
    ptMin = cms.double(1.5),
    candTag = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    candTagEle = cms.InputTag("hltPixelMatchElectronsL1IsoSS"),
    originRadius = cms.double(0.02)
)


hltHcalDigis = cms.EDProducer("HcalRawToDigi",
    UnpackZDC = cms.untracked.bool(True),
    FilterDataQuality = cms.bool(True),
    InputLabel = cms.InputTag("source"),
    UnpackCalib = cms.untracked.bool(True),
    lastSample = cms.int32(9),
    firstSample = cms.int32(0)
)


hltPixelMatchElectronsL1Iso = cms.EDProducer("EgammaHLTPixelMatchElectronProducers",
    BSProducer = cms.InputTag("hltOfflineBeamSpot"),
    TrackProducer = cms.InputTag("hltCtfL1IsoWithMaterialTracks")
)


hltL3MuonsNoVtx = cms.EDProducer("L3TkMuonProducer",
    InputObjects = cms.InputTag("hltL3TkTracksFromL2NoVtx")
)


hltEcalRecHitAll = cms.EDProducer("EcalRawToRecHitProducer",
    splitOutput = cms.bool(True),
    rechitCollection = cms.string('NotNeededsplitOutputTrue'),
    EErechitCollection = cms.string('EcalRecHitsEE'),
    EBrechitCollection = cms.string('EcalRecHitsEB'),
    lazyGetterTag = cms.InputTag("hltEcalRawToRecHitFacility"),
    sourceTag = cms.InputTag("hltEcalRegionalRestFEDs")
)


hltIcone5Central3 = cms.EDProducer("FastjetJetProducer",
    Active_Area_Repeats = cms.int32(5),
    doAreaFastjet = cms.bool(False),
    Ghost_EtaMax = cms.double(6.0),
    maxBadHcalCells = cms.uint32(9999999),
    maxRecoveredEcalCells = cms.uint32(9999999),
    jetType = cms.string('CaloJet'),
    doRhoFastjet = cms.bool(False),
    jetAlgorithm = cms.string('IterativeCone'),
    nSigmaPU = cms.double(1.0),
    GhostArea = cms.double(0.01),
    maxBadEcalCells = cms.uint32(9999999),
    doPVCorrection = cms.bool(False),
    maxRecoveredHcalCells = cms.uint32(9999999),
    rParam = cms.double(0.5),
    UseOnlyVertexTracks = cms.bool(False),
    UseOnlyOnePV = cms.bool(False),
    DzTrVtxMax = cms.double(0.0),
    maxProblematicHcalCells = cms.uint32(9999999),
    src = cms.InputTag("hltCaloTowersCentral3"),
    inputEtMin = cms.double(0.5),
    srcPVs = cms.InputTag("offlinePrimaryVertices"),
    jetPtMin = cms.double(1.0),
    radiusPU = cms.double(0.5),
    maxProblematicEcalCells = cms.uint32(9999999),
    doPUOffsetCorr = cms.bool(False),
    inputEMin = cms.double(0.0)
)


hltHtMet = cms.EDProducer("METProducer",
    src = cms.InputTag("hltMCJetCorJetIcone5"),
    METType = cms.string('MET'),
    InputType = cms.string('CaloJetCollection'),
    HO_PhiResPar = cms.vdouble(0.02511),
    HF_PhiResPar = cms.vdouble(0.05022),
    calculateSignificance = cms.bool(False),
    EB_EtResPar = cms.vdouble(0.2, 0.03, 0.005),
    EE_PhiResPar = cms.vdouble(0.02511),
    HB_EtResPar = cms.vdouble(0.0, 1.22, 0.05),
    HB_PhiResPar = cms.vdouble(0.02511),
    alias = cms.string('HTMET'),
    HE_PhiResPar = cms.vdouble(0.02511),
    noHF = cms.bool(False),
    EE_EtResPar = cms.vdouble(0.2, 0.03, 0.005),
    globalThreshold = cms.double(5.0),
    HF_EtResPar = cms.vdouble(0.0, 1.82, 0.09),
    HE_EtResPar = cms.vdouble(0.0, 1.3, 0.05),
    HO_EtResPar = cms.vdouble(0.0, 1.3, 0.005),
    EB_PhiResPar = cms.vdouble(0.00502),
    onlyFiducialParticles = cms.bool(False)
)


hltL2Muons = cms.EDProducer("L2MuonProducer",
    ServiceParameters = cms.PSet(
        Propagators = cms.untracked.vstring('FastSteppingHelixPropagatorAny', 
            'FastSteppingHelixPropagatorOpposite'),
        RPCLayers = cms.bool(True),
        UseMuonNavigation = cms.untracked.bool(True)
    ),
    L2TrajBuilderParameters = cms.PSet(
        DoRefit = cms.bool(False),
        SeedPropagator = cms.string('FastSteppingHelixPropagatorAny'),
        FilterParameters = cms.PSet(
            NumberOfSigma = cms.double(3.0),
            FitDirection = cms.string('insideOut'),
            DTRecSegmentLabel = cms.InputTag("hltDt4DSegments"),
            MaxChi2 = cms.double(1000.0),
            MuonTrajectoryUpdatorParameters = cms.PSet(
                MaxChi2 = cms.double(25.0),
                Granularity = cms.int32(0),
                RescaleErrorFactor = cms.double(100.0),
                UseInvalidHits = cms.bool(True),
                RescaleError = cms.bool(False)
            ),
            EnableRPCMeasurement = cms.bool(True),
            CSCRecSegmentLabel = cms.InputTag("hltCscSegments"),
            EnableDTMeasurement = cms.bool(True),
            RPCRecSegmentLabel = cms.InputTag("hltRpcRecHits"),
            Propagator = cms.string('FastSteppingHelixPropagatorAny'),
            EnableCSCMeasurement = cms.bool(True)
        ),
        NavigationType = cms.string('Standard'),
        SeedTransformerParameters = cms.PSet(
            Fitter = cms.string('KFFitterSmootherForL2Muon'),
            MuonRecHitBuilder = cms.string('MuonRecHitBuilder'),
            NMinRecHits = cms.uint32(2),
            UseSubRecHits = cms.bool(False),
            Propagator = cms.string('FastSteppingHelixPropagatorAny'),
            RescaleError = cms.double(100.0)
        ),
        DoBackwardFilter = cms.bool(True),
        SeedPosition = cms.string('in'),
        BWFilterParameters = cms.PSet(
            NumberOfSigma = cms.double(3.0),
            CSCRecSegmentLabel = cms.InputTag("hltCscSegments"),
            FitDirection = cms.string('outsideIn'),
            DTRecSegmentLabel = cms.InputTag("hltDt4DSegments"),
            MaxChi2 = cms.double(100.0),
            MuonTrajectoryUpdatorParameters = cms.PSet(
                MaxChi2 = cms.double(25.0),
                Granularity = cms.int32(2),
                RescaleErrorFactor = cms.double(100.0),
                UseInvalidHits = cms.bool(True),
                RescaleError = cms.bool(False)
            ),
            EnableRPCMeasurement = cms.bool(True),
            BWSeedType = cms.string('fromGenerator'),
            EnableDTMeasurement = cms.bool(True),
            RPCRecSegmentLabel = cms.InputTag("hltRpcRecHits"),
            Propagator = cms.string('FastSteppingHelixPropagatorAny'),
            EnableCSCMeasurement = cms.bool(True)
        ),
        DoSeedRefit = cms.bool(False)
    ),
    InputObjects = cms.InputTag("hltL2MuonSeeds"),
    TrackLoaderParameters = cms.PSet(
        Smoother = cms.string('KFSmootherForMuonTrackLoader'),
        DoSmoothing = cms.bool(False),
        MuonUpdatorAtVertexParameters = cms.PSet(
            MaxChi2 = cms.double(1000000.0),
            BeamSpotPosition = cms.vdouble(0.0, 0.0, 0.0),
            Propagator = cms.string('FastSteppingHelixPropagatorOpposite'),
            BeamSpotPositionErrors = cms.vdouble(0.1, 0.1, 5.3)
        ),
        VertexConstraint = cms.bool(True)
    )
)


hltIcone5Central2 = cms.EDProducer("FastjetJetProducer",
    Active_Area_Repeats = cms.int32(5),
    doAreaFastjet = cms.bool(False),
    Ghost_EtaMax = cms.double(6.0),
    maxBadHcalCells = cms.uint32(9999999),
    maxRecoveredEcalCells = cms.uint32(9999999),
    jetType = cms.string('CaloJet'),
    doRhoFastjet = cms.bool(False),
    jetAlgorithm = cms.string('IterativeCone'),
    nSigmaPU = cms.double(1.0),
    GhostArea = cms.double(0.01),
    maxBadEcalCells = cms.uint32(9999999),
    doPVCorrection = cms.bool(False),
    maxRecoveredHcalCells = cms.uint32(9999999),
    rParam = cms.double(0.5),
    UseOnlyVertexTracks = cms.bool(False),
    UseOnlyOnePV = cms.bool(False),
    DzTrVtxMax = cms.double(0.0),
    maxProblematicHcalCells = cms.uint32(9999999),
    src = cms.InputTag("hltCaloTowersCentral2"),
    inputEtMin = cms.double(0.5),
    srcPVs = cms.InputTag("offlinePrimaryVertices"),
    jetPtMin = cms.double(1.0),
    radiusPU = cms.double(0.5),
    maxProblematicEcalCells = cms.uint32(9999999),
    doPUOffsetCorr = cms.bool(False),
    inputEMin = cms.double(0.0)
)


hltPixelTracksForMinBias = cms.EDProducer("PixelTrackProducer",
    FilterPSet = cms.PSet(
        nSigmaTipMaxTolerance = cms.double(0.0),
        chi2 = cms.double(1000.0),
        ComponentName = cms.string('PixelTrackFilterByKinematics'),
        nSigmaInvPtTolerance = cms.double(0.0),
        ptMin = cms.double(0.1),
        tipMax = cms.double(1.0)
    ),
    useFilterWithES = cms.bool(False),
    FitterPSet = cms.PSet(
        ComponentName = cms.string('PixelFitterByHelixProjections'),
        TTRHBuilder = cms.string('TTRHBuilderPixelOnly')
    ),
    RegionFactoryPSet = cms.PSet(
        ComponentName = cms.string('GlobalRegionProducer'),
        RegionPSet = cms.PSet(
            precise = cms.bool(True),
            originHalfLength = cms.double(30.0),
            originZPos = cms.double(0.0),
            originYPos = cms.double(0.0),
            ptMin = cms.double(0.2),
            originXPos = cms.double(0.0),
            originRadius = cms.double(0.5)
        )
    ),
    CleanerPSet = cms.PSet(
        ComponentName = cms.string('PixelTrackCleanerBySharedHits')
    ),
    OrderedHitsFactoryPSet = cms.PSet(
        ComponentName = cms.string('StandardHitTripletGenerator'),
        GeneratorPSet = cms.PSet(
            useBending = cms.bool(True),
            useFixedPreFiltering = cms.bool(False),
            phiPreFiltering = cms.double(0.3),
            extraHitRPhitolerance = cms.double(0.06),
            useMultScattering = cms.bool(True),
            ComponentName = cms.string('PixelTripletHLTGenerator'),
            extraHitRZtolerance = cms.double(0.06)
        ),
        SeedingLayers = cms.string('PixelLayerTriplets')
    )
)


hltBLifetimeL25JetsStartup = cms.EDProducer("EtMinCaloJetSelector",
    filter = cms.bool(False),
    src = cms.InputTag("hltSelector4JetsRegional"),
    etMin = cms.double(30.0)
)


openhltL25TauPixelSeeds = cms.EDProducer("SeedGeneratorFromRegionHitsEDProducer",
    SeedComparitorPSet = cms.PSet(
        ComponentName = cms.string('none')
    ),
    ClusterCheckPSet = cms.PSet(
        MaxNumberOfCosmicClusters = cms.uint32(50000),
        ClusterCollectionLabel = cms.InputTag("hltSiStripClusters"),
        doClusterCheck = cms.bool(False)
    ),
    RegionFactoryPSet = cms.PSet(
        ComponentName = cms.string('TauRegionalPixelSeedGenerator'),
        RegionPSet = cms.PSet(
            precise = cms.bool(True),
            deltaPhiRegion = cms.double(0.5),
            originHalfLength = cms.double(0.2),
            originRadius = cms.double(0.2),
            deltaEtaRegion = cms.double(0.5),
            vertexSrc = cms.InputTag("hltPixelVertices"),
            JetSrc = cms.InputTag("openhltL2TauJets"),
            originZPos = cms.double(0.0),
            ptMin = cms.double(0.9)
        )
    ),
    SeedCreatorPSet = cms.PSet(
        ComponentName = cms.string('SeedFromConsecutiveHitsCreator'),
        propagator = cms.string('PropagatorWithMaterial')
    ),
    OrderedHitsFactoryPSet = cms.PSet(
        ComponentName = cms.string('StandardHitPairGenerator'),
        SeedingLayers = cms.string('PixelLayerPairs')
    ),
    TTRHBuilder = cms.string('WithTrackAngle')
)


hltBSoftMuonL3BJetTagsUByPt = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('softLeptonByPt'),
    tagInfos = cms.VInputTag("hltBSoftMuonL3TagInfosU")
)


hltL1IsoLWEleRegioCkfTC = cms.EDProducer("CkfTrackCandidateMaker",
    src = cms.InputTag("hltL1IsoLWEleRegPSG"),
    TransientInitialStateEstimatorParameters = cms.PSet(
        propagatorAlongTISE = cms.string('PropagatorWithMaterial'),
        numberMeasurementsForFit = cms.int32(4),
        propagatorOppositeTISE = cms.string('PropagatorWithMaterialOpposite')
    ),
    RedundantSeedCleaner = cms.string('CachingSeedCleanerBySharedInput'),
    cleanTrajectoryAfterInOut = cms.bool(False),
    useHitsSplitting = cms.bool(False),
    doSeedingRegionRebuilding = cms.bool(False),
    maxNSeeds = cms.uint32(100000),
    NavigationSchool = cms.string('SimpleNavigationSchool'),
    TrajectoryBuilder = cms.string('CkfTrajectoryBuilder'),
    TrajectoryCleaner = cms.string('TrajectoryCleanerBySharedHits')
)


hltSelector4JetsRegional = cms.EDProducer("LargestEtCaloJetSelector",
    maxNumber = cms.uint32(4),
    filter = cms.bool(False),
    src = cms.InputTag("hltMCJetCorJetIcone5Regional")
)


simCscTriggerPrimitiveDigis = cms.EDProducer("CSCTriggerPrimitivesProducer",
    clctParamMTCC = cms.PSet(
        clctDriftDelay = cms.uint32(2),
        clctMinSeparation = cms.uint32(10),
        clctPidThreshPretrig = cms.uint32(2),
        clctFifoTbins = cms.uint32(12),
        verbosity = cms.untracked.int32(0),
        clctNplanesHitPretrig = cms.uint32(4),
        clctHitPersist = cms.uint32(6),
        clctFifoPretrig = cms.uint32(7),
        clctNplanesHitPattern = cms.uint32(1)
    ),
    MaxBX = cms.int32(9),
    CSCComparatorDigiProducer = cms.InputTag("simMuonCSCDigis","MuonCSCComparatorDigi"),
    alctParamMTCC = cms.PSet(
        alctAccelMode = cms.uint32(0),
        alctTrigMode = cms.uint32(2),
        verbosity = cms.untracked.int32(0),
        alctL1aWindowWidth = cms.uint32(3),
        alctNplanesHitAccelPretrig = cms.uint32(2),
        alctDriftDelay = cms.uint32(3),
        alctNplanesHitPattern = cms.uint32(4),
        alctNplanesHitAccelPattern = cms.uint32(4),
        alctFifoTbins = cms.uint32(16),
        alctNplanesHitPretrig = cms.uint32(2),
        alctFifoPretrig = cms.uint32(10)
    ),
    clctParamOldMC = cms.PSet(
        clctDriftDelay = cms.uint32(2),
        clctMinSeparation = cms.uint32(10),
        clctPidThreshPretrig = cms.uint32(2),
        clctFifoTbins = cms.uint32(12),
        verbosity = cms.untracked.int32(0),
        clctNplanesHitPretrig = cms.uint32(2),
        clctHitPersist = cms.uint32(6),
        clctFifoPretrig = cms.uint32(7),
        clctNplanesHitPattern = cms.uint32(4)
    ),
    MinBX = cms.int32(3),
    tmbParam = cms.PSet(
        alctTrigEnable = cms.uint32(0),
        verbosity = cms.untracked.int32(0),
        clctTrigEnable = cms.uint32(0),
        tmbL1aWindowSize = cms.uint32(7),
        matchTrigWindowSize = cms.uint32(7),
        mpcBlockMe1a = cms.uint32(1),
        matchTrigEnable = cms.uint32(1)
    ),
    commonParam = cms.PSet(
        isTMB07 = cms.bool(True),
        isMTCC = cms.bool(False)
    ),
    CSCWireDigiProducer = cms.InputTag("simMuonCSCDigis","MuonCSCWireDigi"),
    alctParam07 = cms.PSet(
        alctAccelMode = cms.uint32(0),
        alctTrigMode = cms.uint32(2),
        verbosity = cms.untracked.int32(0),
        alctL1aWindowWidth = cms.uint32(7),
        alctNplanesHitAccelPretrig = cms.uint32(2),
        alctDriftDelay = cms.uint32(2),
        alctNplanesHitPattern = cms.uint32(4),
        alctNplanesHitAccelPattern = cms.uint32(4),
        alctFifoTbins = cms.uint32(16),
        alctNplanesHitPretrig = cms.uint32(2),
        alctFifoPretrig = cms.uint32(10)
    ),
    alctParamOldMC = cms.PSet(
        alctAccelMode = cms.uint32(1),
        alctTrigMode = cms.uint32(3),
        verbosity = cms.untracked.int32(0),
        alctL1aWindowWidth = cms.uint32(5),
        alctNplanesHitAccelPretrig = cms.uint32(2),
        alctDriftDelay = cms.uint32(3),
        alctNplanesHitPattern = cms.uint32(4),
        alctNplanesHitAccelPattern = cms.uint32(4),
        alctFifoTbins = cms.uint32(16),
        alctNplanesHitPretrig = cms.uint32(2),
        alctFifoPretrig = cms.uint32(10)
    ),
    clctParam07 = cms.PSet(
        clctDriftDelay = cms.uint32(2),
        clctMinSeparation = cms.uint32(10),
        clctPidThreshPretrig = cms.uint32(2),
        clctFifoTbins = cms.uint32(12),
        verbosity = cms.untracked.int32(0),
        clctNplanesHitPretrig = cms.uint32(2),
        clctHitPersist = cms.uint32(6),
        clctFifoPretrig = cms.uint32(7),
        clctNplanesHitPattern = cms.uint32(4)
    )
)


openHltBLifetimeL25Associator = cms.EDProducer("JetTracksAssociatorAtVertex",
    jets = cms.InputTag("hltIterativeCone5CaloJets"),
    tracks = cms.InputTag("hltPixelTracks"),
    coneSize = cms.double(0.5)
)


hltBLifetimeL25TagInfosStartup = cms.EDProducer("TrackIPProducer",
    maximumTransverseImpactParameter = cms.double(0.2),
    minimumNumberOfHits = cms.int32(3),
    minimumTransverseMomentum = cms.double(1.0),
    primaryVertex = cms.InputTag("hltPixelVertices"),
    maximumLongitudinalImpactParameter = cms.double(17.0),
    computeProbabilities = cms.bool(False),
    ghostTrackPriorDeltaR = cms.double(0.03),
    jetTracks = cms.InputTag("hltBLifetimeL25AssociatorStartup"),
    jetDirectionUsingGhostTrack = cms.bool(False),
    minimumNumberOfPixelHits = cms.int32(2),
    jetDirectionUsingTracks = cms.bool(False),
    computeGhostTrack = cms.bool(False),
    useTrackQuality = cms.bool(False),
    maximumChiSquared = cms.double(5.0)
)


hltIsolPixelTrackProdHE1E31 = cms.EDProducer("IsolatedPixelTrackCandidateProducer",
    EBEtaBoundary = cms.double(1.479),
    L1eTauJetsSource = cms.InputTag("hltL1extraParticles","Tau"),
    MaxVtxDXYSeed = cms.double(101.0),
    tauUnbiasCone = cms.double(1.2),
    PixelIsolationConeSizeAtEC = cms.double(40.0),
    L1GTSeedLabel = cms.InputTag("hltL1sIsoTrack1E31"),
    minPTrack = cms.double(5.0),
    maxPTrackForIsolation = cms.double(3.0),
    MagFieldRecordName = cms.string('VolumeBasedMagneticField'),
    VertexLabel = cms.InputTag("hltHITPixelVerticesHE"),
    PixelTracksSources = cms.VInputTag("hltHITPixelTracksHE", "hltHITPixelTracksHB"),
    MaxVtxDXYIsol = cms.double(101.0),
    tauAssociationCone = cms.double(0.0),
    ExtrapolationConeSize = cms.double(1.0)
)


openHltBSoftmuonL25TagInfos = cms.EDProducer("SoftLepton",
    muonSelection = cms.uint32(0),
    refineJetAxis = cms.uint32(0),
    primaryVertex = cms.InputTag("nominal"),
    leptons = cms.InputTag("hltL2Muons"),
    leptonQualityCut = cms.double(0.0),
    jets = cms.InputTag("hltIterativeCone5CaloJets"),
    leptonDeltaRCut = cms.double(0.4),
    leptonChi2Cut = cms.double(0.0)
)


hltL25TauJetTracksAssociator = cms.EDProducer("JetTracksAssociatorAtVertex",
    jets = cms.InputTag("hltL2TauRelaxingIsolationSelector","Isolated"),
    tracks = cms.InputTag("hltL25TauCtfWithMaterialTracks"),
    coneSize = cms.double(0.5)
)


hltL1sJet100MET60Aco = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleJet150'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltSingleMu3L3PreFilterNoVtx = cms.EDFilter("HLTMuonL3PreFilter",
    PreviousCandTag = cms.InputTag("hltSingleMu3L2PreFilteredNoVtx"),
    MinPt = cms.double(3.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    MinNhits = cms.int32(0),
    SaveTag = cms.untracked.bool(True),
    NSigmaPt = cms.double(0.0),
    MaxDz = cms.double(9999.0),
    BeamSpotTag = cms.InputTag("hltOfflineBeamSpot"),
    CandTag = cms.InputTag("hltL3MuonCandidatesNoVtx"),
    MaxDr = cms.double(9999.0)
)


hltJpsiMML2Filtered = cms.EDFilter("HLTMuonDimuonL2Filter",
    FastAccept = cms.bool(False),
    MinPtMax = cms.double(3.0),
    PreviousCandTag = cms.InputTag("hltJpsiMML1Filtered"),
    ChargeOpt = cms.int32(0),
    MaxPtBalance = cms.double(999999.0),
    MaxDz = cms.double(9999.0),
    MaxAcop = cms.double(3.15),
    MinNhits = cms.int32(0),
    MinPtMin = cms.double(3.0),
    NSigmaPt = cms.double(0.0),
    MinPtBalance = cms.double(-1.0),
    MaxInvMass = cms.double(5.0),
    BeamSpotTag = cms.InputTag("hltOfflineBeamSpot"),
    MinPtPair = cms.double(0.0),
    CandTag = cms.InputTag("hltL2MuonCandidates"),
    MaxDr = cms.double(100.0),
    MinInvMass = cms.double(1.0),
    MaxEta = cms.double(2.5),
    MinAcop = cms.double(-1.0)
)


hltMuBLifetimeL1Filtered = cms.EDFilter("HLTMuonL1Filter",
    PreviousCandTag = cms.InputTag("hltL1sMuB"),
    MinPt = cms.double(7.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    SelectQualities = cms.vint32(),
    CandTag = cms.InputTag("hltL1extraParticles")
)


hltL1NonIsoHLTNonIsoSingleElectronSiStripEt20EtFilter = cms.EDFilter("HLTEgammaEtFilter",
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    inputTag = cms.InputTag("hltL1NonIsoHLTNonIsoSingleElectronSiStripEt20L1MatchFilterRegional"),
    etcutEB = cms.double(20.0),
    etcutEE = cms.double(20.0),
    ncandcut = cms.int32(1)
)


hltPreDoubleEle5JpsiL1R = cms.EDFilter("HLTPrescaler")


hltPreHT240 = cms.EDFilter("HLTPrescaler")


hltPreJet15U = cms.EDFilter("HLTPrescaler")


hltL1sL1Mu14L1SingleJet6U = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleMu14 AND L1_SingleJet6U'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hlt2jet30L2Mu9DoubleJet30 = cms.EDFilter("HLT1CaloJet",
    MaxEta = cms.double(5.0),
    saveTag = cms.untracked.bool(True),
    inputTag = cms.InputTag("hltMCJetCorJetIcone5"),
    MinPt = cms.double(30.0),
    MinN = cms.int32(2)
)


hltPreCSCBeamHaloOverlapRing2 = cms.EDFilter("HLTPrescaler")


hltPreCSCBeamHaloOverlapRing1 = cms.EDFilter("HLTPrescaler")


hltMuonPointingFilter = cms.EDFilter("HLTMuonPointingFilter",
    maxZ = cms.double(280.0),
    PropagatorName = cms.string('SteppingHelixPropagatorAny'),
    radius = cms.double(90.0),
    SALabel = cms.string('hltCosmicMuonBarrelOnly')
)


hltPreFirstPath = cms.EDFilter("HLTPrescaler")


hltL1sMET50 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_ETM40'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltL1NonIsoDoublePhotonEt5UpsL1MatchFilterRegional = cms.EDFilter("HLTEgammaL1MatchFilterRegional",
    doIsolated = cms.bool(False),
    endcap_end = cms.double(2.65),
    candNonIsolatedTag = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    region_eta_size_ecap = cms.double(1.0),
    barrel_end = cms.double(1.4791),
    l1IsolatedTag = cms.InputTag("hltL1extraParticles","Isolated"),
    candIsolatedTag = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    region_phi_size = cms.double(1.044),
    region_eta_size = cms.double(0.522),
    L1SeedFilterTag = cms.InputTag("hltL1sL1SingleEG8orL1DoubleEG5"),
    ncandcut = cms.int32(2),
    l1NonIsolatedTag = cms.InputTag("hltL1extraParticles","NonIsolated")
)


hltDiMuonL3PreFiltered0 = cms.EDFilter("HLTMuonL3PreFilter",
    PreviousCandTag = cms.InputTag("hltDiMuonL2PreFiltered0"),
    MinPt = cms.double(0.0),
    MinN = cms.int32(2),
    MaxEta = cms.double(2.5),
    MinNhits = cms.int32(0),
    SaveTag = cms.untracked.bool(True),
    NSigmaPt = cms.double(0.0),
    MaxDz = cms.double(9999.0),
    BeamSpotTag = cms.InputTag("hltOfflineBeamSpot"),
    CandTag = cms.InputTag("hltL3MuonCandidates"),
    MaxDr = cms.double(2.0)
)


hltL1sDoubleJet40MET70Aco = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleJet150'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hlt1SumET120 = cms.EDFilter("HLTGlobalSumsCaloMET",
    observable = cms.string('sumEt'),
    MinN = cms.int32(1),
    Min = cms.double(120.0),
    Max = cms.double(-1.0),
    inputTag = cms.InputTag("hltMet"),
    saveTag = cms.untracked.bool(True)
)


hltPreEcalCalibration = cms.EDFilter("HLTPrescaler")


hltPreDoubleEle5UpsL1R = cms.EDFilter("HLTPrescaler")


hltPreEle15SC10LWL1R = cms.EDFilter("HLTPrescaler")


hltPreL1MET20 = cms.EDFilter("HLTPrescaler")


hltDiMuonIsoL2PreFiltered = cms.EDFilter("HLTMuonL2PreFilter",
    PreviousCandTag = cms.InputTag("hltDiMuonIsoL1Filtered"),
    MinPt = cms.double(3.0),
    MinN = cms.int32(2),
    MaxEta = cms.double(2.5),
    MinNhits = cms.int32(0),
    NSigmaPt = cms.double(0.0),
    MaxDz = cms.double(9999.0),
    BeamSpotTag = cms.InputTag("hltOfflineBeamSpot"),
    CandTag = cms.InputTag("hltL2MuonCandidates"),
    MaxDr = cms.double(9999.0)
)


hlt4jet30 = cms.EDFilter("HLT1CaloJet",
    MaxEta = cms.double(5.0),
    saveTag = cms.untracked.bool(True),
    inputTag = cms.InputTag("hltMCJetCorJetIcone5"),
    MinPt = cms.double(30.0),
    MinN = cms.int32(4)
)


hlt4jet35 = cms.EDFilter("HLT1CaloJet",
    MaxEta = cms.double(5.0),
    saveTag = cms.untracked.bool(True),
    inputTag = cms.InputTag("hltMCJetCorJetIcone5"),
    MinPt = cms.double(35.0),
    MinN = cms.int32(4)
)


hlt1MET50 = cms.EDFilter("HLT1CaloMET",
    MaxEta = cms.double(-1.0),
    saveTag = cms.untracked.bool(True),
    inputTag = cms.InputTag("hltMet"),
    MinPt = cms.double(50.0),
    MinN = cms.int32(1)
)


hltRapGap40 = cms.EDFilter("HLTRapGapFilter",
    maxEta = cms.double(5.0),
    minEta = cms.double(3.0),
    caloThresh = cms.double(40.0),
    inputTag = cms.InputTag("hltMCJetCorJetIcone5"),
    saveTag = cms.untracked.bool(True)
)


hltPreStoppedHSCP8E29 = cms.EDFilter("HLTPrescaler")


hltPreMinBiasPixelSingleTrack = cms.EDFilter("HLTPrescaler")


hlt3jet60 = cms.EDFilter("HLT1CaloJet",
    MaxEta = cms.double(5.0),
    saveTag = cms.untracked.bool(True),
    inputTag = cms.InputTag("hltMCJetCorJetIcone5"),
    MinPt = cms.double(60.0),
    MinN = cms.int32(3)
)


hltHT200 = cms.EDFilter("HLTGlobalSumsMET",
    observable = cms.string('sumEt'),
    MinN = cms.int32(1),
    Min = cms.double(200.0),
    Max = cms.double(-1.0),
    inputTag = cms.InputTag("hltJet30Ht"),
    saveTag = cms.untracked.bool(True)
)


hltPrePhoton15LEIL1R = cms.EDFilter("HLTPrescaler")


hltL1sDiJetAve130U = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleJet70'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltPreDiJetAve50U = cms.EDFilter("HLTPrescaler")


hltL1sL1Mu14L1Mu5Jet20 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleMu14 AND L1_Mu5_Jet20'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltL1sStoppedHSCP1E31 = cms.EDFilter("HLTLevel1GTSeed",
    saveTags = cms.untracked.bool(False),
    L1SeedsLogicalExpression = cms.string('L1_SingleJet15_NotBptxC'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltL1NonIsoHLTVLEIDoublePhotonEt15EcalIsolFilter = cms.EDFilter("HLTEgammaGenericFilter",
    doIsolated = cms.bool(False),
    thrOverE2EE = cms.double(-1.0),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    thrOverE2EB = cms.double(-1.0),
    thrRegularEE = cms.double(5.0),
    thrOverEEE = cms.double(0.2),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    thrOverEEB = cms.double(0.2),
    thrRegularEB = cms.double(5.0),
    lessThan = cms.bool(True),
    useEt = cms.bool(False),
    ncandcut = cms.int32(1),
    isoTag = cms.InputTag("hltL1IsolatedPhotonEcalIsol"),
    candTag = cms.InputTag("hltL1NonIsoHLTVLEIDoublePhotonEt15EtFilter"),
    nonIsoTag = cms.InputTag("hltL1NonIsolatedPhotonEcalIsol")
)


hltFilterL2EcalIsolationMuonTauNoL1Tau = cms.EDFilter("HLT1Tau",
    MaxEta = cms.double(5.0),
    inputTag = cms.InputTag("hltL2TauRelaxingIsolationSelector","Isolated"),
    MinPt = cms.double(15.0),
    MinN = cms.int32(1)
)


hltBLifetimeL3FilterStartup = cms.EDFilter("HLTJetTag",
    JetTag = cms.InputTag("hltBLifetimeL3BJetTagsStartup"),
    MinTag = cms.double(3.5),
    MaxTag = cms.double(99999.0),
    SaveTag = cms.bool(True),
    MinJets = cms.int32(1)
)


hltL1NonIsoDoubleElectronEt5UpsL1MatchFilterRegional = cms.EDFilter("HLTEgammaL1MatchFilterRegional",
    doIsolated = cms.bool(False),
    endcap_end = cms.double(2.65),
    candNonIsolatedTag = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    region_eta_size_ecap = cms.double(1.0),
    barrel_end = cms.double(1.4791),
    l1IsolatedTag = cms.InputTag("hltL1extraParticles","Isolated"),
    candIsolatedTag = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    region_phi_size = cms.double(1.044),
    region_eta_size = cms.double(0.522),
    L1SeedFilterTag = cms.InputTag("hltL1sL1SingleEG8orL1DoubleEG5"),
    ncandcut = cms.int32(2),
    l1NonIsolatedTag = cms.InputTag("hltL1extraParticles","NonIsolated")
)


hltL1sMuon5Jet20 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_Mu5_Jet20'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltL1NonIsoDoubleElectronEt5UpsPMMassFilter = cms.EDFilter("HLTPMMassFilter",
    lowerMassCut = cms.double(8.0),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    isElectron1 = cms.untracked.bool(False),
    SaveTag = cms.untracked.bool(True),
    isElectron2 = cms.untracked.bool(False),
    upperMassCut = cms.double(11.0),
    candTag = cms.InputTag("hltL1NonIsoDoubleElectronEt5UpsPixelMatchFilter"),
    nZcandcut = cms.int32(1),
    reqOppCharge = cms.untracked.bool(True)
)


hltL1NonIsoDoublePhotonEt5UpsHcalIsolFilter = cms.EDFilter("HLTEgammaGenericFilter",
    doIsolated = cms.bool(False),
    thrOverE2EE = cms.double(-1.0),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    thrOverE2EB = cms.double(-1.0),
    thrRegularEE = cms.double(999999.9),
    thrOverEEE = cms.double(-1.0),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    thrOverEEB = cms.double(-1.0),
    thrRegularEB = cms.double(999999.9),
    lessThan = cms.bool(True),
    useEt = cms.bool(False),
    ncandcut = cms.int32(2),
    isoTag = cms.InputTag("hltL1IsolatedElectronHcalIsol"),
    candTag = cms.InputTag("hltL1NonIsoDoublePhotonEt5UpsEcalIsolFilter"),
    nonIsoTag = cms.InputTag("hltL1NonIsolatedElectronHcalIsol")
)


hltEle10SWL1RTripleJet30ElePixelMatchFilter = cms.EDFilter("HLTElectronPixelMatchFilter",
    doIsolated = cms.bool(False),
    L1IsoPixelSeedsTag = cms.InputTag("hltL1IsoStartUpElectronPixelSeeds"),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    L1NonIsoPixelSeedsTag = cms.InputTag("hltL1NonIsoStartUpElectronPixelSeeds"),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    SaveTag = cms.untracked.bool(True),
    npixelmatchcut = cms.double(1.0),
    ncandcut = cms.int32(1),
    candTag = cms.InputTag("hltEle10SWL1RTripleJet30EleHcalIsolFilter")
)


hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdPixelMatchFilter = cms.EDFilter("HLTElectronPixelMatchFilter",
    doIsolated = cms.bool(False),
    L1IsoPixelSeedsTag = cms.InputTag("hltL1IsoLargeWindowElectronPixelSeeds"),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    L1NonIsoPixelSeedsTag = cms.InputTag("hltL1NonIsoLargeWindowElectronPixelSeeds"),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    npixelmatchcut = cms.double(1.0),
    ncandcut = cms.int32(1),
    candTag = cms.InputTag("hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdHcalIsolFilter")
)


hltPhiJet1metAco = cms.EDFilter("HLTAcoFilter",
    saveTags = cms.untracked.bool(True),
    maxDeltaPhi = cms.double(2.89),
    inputJetTag = cms.InputTag("hltIterativeCone5CaloJets"),
    Acoplanar = cms.string('Jet1Met'),
    inputMETTag = cms.InputTag("hlt1MET70"),
    minDeltaPhi = cms.double(0.0),
    minEtJet1 = cms.double(60.0),
    minEtJet2 = cms.double(-1.0)
)


hltL1NonIsoHLTNonIsoSingleElectronEt15EleIdL1MatchFilterRegional = cms.EDFilter("HLTEgammaL1MatchFilterRegional",
    doIsolated = cms.bool(False),
    endcap_end = cms.double(2.65),
    candNonIsolatedTag = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    region_eta_size_ecap = cms.double(1.0),
    barrel_end = cms.double(1.4791),
    l1IsolatedTag = cms.InputTag("hltL1extraParticles","Isolated"),
    candIsolatedTag = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    region_phi_size = cms.double(1.044),
    region_eta_size = cms.double(0.522),
    L1SeedFilterTag = cms.InputTag("hltL1sL1SingleEG8"),
    ncandcut = cms.int32(1),
    l1NonIsolatedTag = cms.InputTag("hltL1extraParticles","NonIsolated")
)


hltL1sMET25 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_ETM20'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hlt3jet30Ele10SWL1RTripleJet30 = cms.EDFilter("HLT1CaloJet",
    MaxEta = cms.double(5.0),
    saveTag = cms.untracked.bool(True),
    inputTag = cms.InputTag("hltMCJetCorJetIcone5"),
    MinPt = cms.double(30.0),
    MinN = cms.int32(3)
)


hltPreDiJetAve130U = cms.EDFilter("HLTPrescaler")


hltFilterL2EtCutDoubleLooseIsoTau15 = cms.EDFilter("HLT1Tau",
    MaxEta = cms.double(5.0),
    inputTag = cms.InputTag("hltL2TauJets"),
    MinPt = cms.double(15.0),
    MinN = cms.int32(2)
)


hltPixelMBForAlignment = cms.EDFilter("HLTPixlMBForAlignmentFilter",
    pixlTag = cms.InputTag("hltPixelCands"),
    MinIsol = cms.double(0.05),
    MinTrks = cms.uint32(2),
    MinPt = cms.double(5.0),
    MinSep = cms.double(1.0)
)


hltL1NonIsoHLTNonIsoSingleElectronEt15LTIOneOEMinusOneOPFilter = cms.EDFilter("HLTElectronOneOEMinusOneOPFilterRegional",
    doIsolated = cms.bool(False),
    electronNonIsolatedProducer = cms.InputTag("hltPixelMatchElectronsL1NonIso"),
    barrelcut = cms.double(999.9),
    electronIsolatedProducer = cms.InputTag("hltPixelMatchElectronsL1Iso"),
    ncandcut = cms.int32(1),
    candTag = cms.InputTag("hltL1NonIsoHLTNonIsoSingleElectronEt15LTIPixelMatchFilter"),
    endcapcut = cms.double(999.9)
)


hltPreDoubleJet125MET60 = cms.EDFilter("HLTPrescaler")


hltSingleMu13L2Filtered11 = cms.EDFilter("HLTMuonL2PreFilter",
    PreviousCandTag = cms.InputTag("hltL1SingleMu10L1Filtered0"),
    MinPt = cms.double(11.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    MinNhits = cms.int32(0),
    NSigmaPt = cms.double(0.0),
    MaxDz = cms.double(9999.0),
    BeamSpotTag = cms.InputTag("hltOfflineBeamSpot"),
    CandTag = cms.InputTag("hltL2MuonCandidates"),
    MaxDr = cms.double(9999.0)
)


hltHIL1sPhoton15 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleEG5'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltPreJet250 = cms.EDFilter("HLTPrescaler")


hltL1sStoppedHSCP8E29 = cms.EDFilter("HLTLevel1GTSeed",
    saveTags = cms.untracked.bool(False),
    L1SeedsLogicalExpression = cms.string('L1_SingleJet10U_NotBptxC'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltL1sMinBiasEcal = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleEG1 OR L1_SingleEG2 OR L1_DoubleEG1'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltL1sHighMultiplicityBSC = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('35'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(True)
)


hltPreIsoTrackHB8E29 = cms.EDFilter("HLTPrescaler")


hlt2jet150 = cms.EDFilter("HLT1CaloJet",
    MaxEta = cms.double(5.0),
    saveTag = cms.untracked.bool(True),
    inputTag = cms.InputTag("hltMCJetCorJetIcone5Regional"),
    MinPt = cms.double(150.0),
    MinN = cms.int32(2)
)


hltL1NonIsoHLTNonIsoSingleElectronEt15LTIESDoubleSC15 = cms.EDFilter("HLTEgammaEtFilter",
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    inputTag = cms.InputTag("hltL1NonIsoHLTNonIsoSingleElectronEt15LTIESscWrapper"),
    SaveTag = cms.untracked.bool(True),
    etcutEB = cms.double(15.0),
    ncandcut = cms.int32(2),
    etcutEE = cms.double(15.0)
)


hltPreMultiTowerEcal = cms.EDFilter("HLTPrescaler")


hltL1NonIsoHLTNonIsoDoubleElectronEt5EtFilter = cms.EDFilter("HLTEgammaEtFilter",
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    inputTag = cms.InputTag("hltL1NonIsoHLTNonIsoDoubleElectronEt5L1MatchFilterRegional"),
    etcutEB = cms.double(5.0),
    etcutEE = cms.double(5.0),
    ncandcut = cms.int32(2)
)


hltL1sDoubleJet60MET60Aco = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleJet150'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltDiJetAve30U8E29 = cms.EDFilter("HLTDiJetAveFilter",
    inputJetTag = cms.InputTag("hltIterativeCone5CaloJets"),
    minPtAve = cms.double(30.0),
    minPtJet3 = cms.double(99999.0),
    saveTag = cms.untracked.bool(True),
    minDphi = cms.double(-1.0)
)


hltPreHcalPhiSym = cms.EDFilter("HLTPrescaler")


hltPreIsoMu15 = cms.EDFilter("HLTPrescaler")


hltJpsiMML3Filtered = cms.EDFilter("HLTMuonDimuonL3Filter",
    FastAccept = cms.bool(False),
    MinPtMax = cms.double(3.0),
    PreviousCandTag = cms.InputTag("hltJpsiMML2Filtered"),
    ChargeOpt = cms.int32(0),
    MaxPtBalance = cms.double(999999.0),
    MaxDz = cms.double(9999.0),
    MaxAcop = cms.double(3.15),
    MinNhits = cms.int32(0),
    MinPtMin = cms.double(3.0),
    SaveTag = cms.untracked.bool(True),
    NSigmaPt = cms.double(0.0),
    MinPtBalance = cms.double(-1.0),
    MaxInvMass = cms.double(3.4),
    BeamSpotTag = cms.InputTag("hltOfflineBeamSpot"),
    MinPtPair = cms.double(0.0),
    CandTag = cms.InputTag("hltL3MuonCandidates"),
    MaxDr = cms.double(2.0),
    MinInvMass = cms.double(2.8),
    MaxEta = cms.double(2.5),
    MinAcop = cms.double(-1.0)
)


hltPreIsoMu11 = cms.EDFilter("HLTPrescaler")


hlt1jet110 = cms.EDFilter("HLT1CaloJet",
    MaxEta = cms.double(5.0),
    saveTag = cms.untracked.bool(True),
    inputTag = cms.InputTag("hltMCJetCorJetIcone5Regional"),
    MinPt = cms.double(110.0),
    MinN = cms.int32(1)
)


hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdDetaFilter = cms.EDFilter("HLTElectronGenericFilter",
    doIsolated = cms.bool(False),
    nonIsoTag = cms.InputTag("hltElectronL1NonIsoLargeWindowDetaDphi","Deta"),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    thrRegularEE = cms.double(0.011),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    thrRegularEB = cms.double(0.008),
    lessThan = cms.bool(True),
    ncandcut = cms.int32(1),
    isoTag = cms.InputTag("hltElectronL1IsoLargeWindowDetaDphi","Deta"),
    thrOverPtEE = cms.double(-1.0),
    candTag = cms.InputTag("hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdOneOEMinusOneOPFilter"),
    thrTimesPtEE = cms.double(-1.0),
    thrTimesPtEB = cms.double(-1.0),
    thrOverPtEB = cms.double(-1.0)
)


hltL1MuOpenL1Filtered0 = cms.EDFilter("HLTMuonL1Filter",
    PreviousCandTag = cms.InputTag("hltL1sL1SingleMuOpenL1SingleMu0"),
    MinPt = cms.double(0.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    SaveTag = cms.untracked.bool(True),
    SelectQualities = cms.vint32(),
    CandTag = cms.InputTag("hltL1extraParticles")
)


hltL1sDiJetAve30U1E31 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleJet30'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltPreMET25 = cms.EDFilter("HLTPrescaler")


hltL1sMuNoIsoJets30 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_Mu3_TripleJet15_00002'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltL1SingleMu10L1Filtered0 = cms.EDFilter("HLTMuonL1Filter",
    PreviousCandTag = cms.InputTag("hltL1sL1SingleMu10"),
    MinPt = cms.double(0.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    SelectQualities = cms.vint32(),
    CandTag = cms.InputTag("hltL1extraParticles")
)


hltL1NonIsoHLTLEITISinglePhotonEt20HcalIsolFilter = cms.EDFilter("HLTEgammaGenericFilter",
    doIsolated = cms.bool(False),
    thrOverE2EE = cms.double(-1.0),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    thrOverE2EB = cms.double(-1.0),
    thrRegularEE = cms.double(999999.9),
    thrOverEEE = cms.double(-1.0),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    thrOverEEB = cms.double(-1.0),
    thrRegularEB = cms.double(999999.9),
    lessThan = cms.bool(True),
    useEt = cms.bool(False),
    ncandcut = cms.int32(1),
    isoTag = cms.InputTag("hltL1IsolatedPhotonHcalIsol"),
    candTag = cms.InputTag("hltL1NonIsoHLTLEITISinglePhotonEt20EcalIsolFilter"),
    nonIsoTag = cms.InputTag("hltL1NonIsolatedPhotonHcalIsol")
)


hltPrePhoton25L1R = cms.EDFilter("HLTPrescaler")


hltL1NonIsoHLTNonIsoSingleElectronLWEt15EtFilterESet20 = cms.EDFilter("HLTEgammaEtFilter",
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    inputTag = cms.InputTag("hltL1NonIsoHLTNonIsoSingleElectronLWEt15PixelMatchFilter"),
    SaveTag = cms.untracked.bool(True),
    etcutEB = cms.double(20.0),
    ncandcut = cms.int32(1),
    etcutEE = cms.double(20.0)
)


hltPreDoublePhoton5JpsiL1R = cms.EDFilter("HLTPrescaler")


hltL1NonIsoHLTIsoSingleElectronEt20LTIEleIdL1MatchFilterRegional = cms.EDFilter("HLTEgammaL1MatchFilterRegional",
    doIsolated = cms.bool(False),
    endcap_end = cms.double(2.65),
    candNonIsolatedTag = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    region_eta_size_ecap = cms.double(1.0),
    barrel_end = cms.double(1.4791),
    l1IsolatedTag = cms.InputTag("hltL1extraParticles","Isolated"),
    candIsolatedTag = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    region_phi_size = cms.double(1.044),
    region_eta_size = cms.double(0.522),
    L1SeedFilterTag = cms.InputTag("hltL1sL1SingleEG8"),
    ncandcut = cms.int32(1),
    l1NonIsolatedTag = cms.InputTag("hltL1extraParticles","NonIsolated")
)


hltPreL1Mu30 = cms.EDFilter("HLTPrescaler")


hltBoolEnd = cms.EDFilter("HLTBool",
    result = cms.bool(True)
)


hltPreForwardBSC = cms.EDFilter("HLTPrescaler")


hltL1NonIsoHLTNonIsoSingleElectronLWEt15EtFilterESet25 = cms.EDFilter("HLTEgammaEtFilter",
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    inputTag = cms.InputTag("hltL1NonIsoHLTNonIsoSingleElectronLWEt15PixelMatchFilter"),
    SaveTag = cms.untracked.bool(True),
    etcutEB = cms.double(25.0),
    ncandcut = cms.int32(1),
    etcutEE = cms.double(25.0)
)


hltBoolFinalPath = cms.EDFilter("HLTBool",
    result = cms.bool(False)
)


hltPreActivityDT = cms.EDFilter("HLTPrescaler")


hltVetoL1SingleEG2 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('NOT L1_SingleEG2'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltHIPrePhoton10 = cms.EDFilter("HLTPrescaler")


hltL1NonIsoHLTNonIsoSinglePhotonEt10EtFilter = cms.EDFilter("HLTEgammaEtFilter",
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    inputTag = cms.InputTag("hltL1NonIsoHLTNonIsoSinglePhotonEt10L1MatchFilterRegional"),
    etcutEB = cms.double(10.0),
    etcutEE = cms.double(10.0),
    ncandcut = cms.int32(1)
)


hltL1NonIsoDoublePhotonEt5JpsiEtFilter = cms.EDFilter("HLTEgammaEtFilter",
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    inputTag = cms.InputTag("hltL1NonIsoDoublePhotonEt5JpsiL1MatchFilterRegional"),
    etcutEB = cms.double(4.0),
    etcutEE = cms.double(4.0),
    ncandcut = cms.int32(2)
)


hltL1NonIsoHLTLEITISinglePhotonEt25L1MatchFilterRegional = cms.EDFilter("HLTEgammaL1MatchFilterRegional",
    doIsolated = cms.bool(False),
    endcap_end = cms.double(2.65),
    candNonIsolatedTag = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    region_eta_size_ecap = cms.double(1.0),
    barrel_end = cms.double(1.4791),
    l1IsolatedTag = cms.InputTag("hltL1extraParticles","Isolated"),
    candIsolatedTag = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    region_phi_size = cms.double(1.044),
    region_eta_size = cms.double(0.522),
    L1SeedFilterTag = cms.InputTag("hltL1sL1SingleEG8"),
    ncandcut = cms.int32(1),
    l1NonIsolatedTag = cms.InputTag("hltL1extraParticles","NonIsolated")
)


hltL1NonIsoHLTNonIsoSingleElectronLWEt15ESscWrapper = cms.EDFilter("HLTEgammaTriggerFilterObjectWrapper",
    doIsolated = cms.bool(False),
    candIsolatedTag = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    candNonIsolatedTag = cms.InputTag("hltL1NonIsoRecoEcalCandidate")
)


hlt3jet85 = cms.EDFilter("HLT1CaloJet",
    MaxEta = cms.double(5.0),
    saveTag = cms.untracked.bool(True),
    inputTag = cms.InputTag("hltMCJetCorJetIcone5Regional"),
    MinPt = cms.double(85.0),
    MinN = cms.int32(3)
)


hltL1sL1SingleMu10 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleMu10'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltBJet10U = cms.EDFilter("HLT1CaloBJet",
    MaxEta = cms.double(3.0),
    saveTag = cms.untracked.bool(True),
    inputTag = cms.InputTag("hltMCJetCorJetIcone5HF07"),
    MinPt = cms.double(10.0),
    MinN = cms.int32(1)
)


hltL1NonIsoHLTNonIsoSingleElectronEt15EleIdDetaFilter = cms.EDFilter("HLTElectronGenericFilter",
    doIsolated = cms.bool(False),
    nonIsoTag = cms.InputTag("hltElectronL1NonIsoDetaDphi","Deta"),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    thrRegularEE = cms.double(0.011),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    thrRegularEB = cms.double(0.008),
    lessThan = cms.bool(True),
    ncandcut = cms.int32(1),
    isoTag = cms.InputTag("hltElectronL1IsoDetaDphi","Deta"),
    thrOverPtEE = cms.double(-1.0),
    candTag = cms.InputTag("hltL1NonIsoHLTNonIsoSingleElectronEt15EleIdOneOEMinusOneOPFilter"),
    thrTimesPtEE = cms.double(-1.0),
    thrTimesPtEB = cms.double(-1.0),
    thrOverPtEB = cms.double(-1.0)
)


hlt1jet180 = cms.EDFilter("HLT1CaloJet",
    MaxEta = cms.double(5.0),
    saveTag = cms.untracked.bool(True),
    inputTag = cms.InputTag("hltMCJetCorJetIcone5"),
    MinPt = cms.double(180.0),
    MinN = cms.int32(1)
)


hltPreMET50 = cms.EDFilter("HLTPrescaler")


hlt1jet50 = cms.EDFilter("HLT1CaloJet",
    MaxEta = cms.double(5.0),
    saveTag = cms.untracked.bool(True),
    inputTag = cms.InputTag("hltMCJetCorJetIcone5"),
    MinPt = cms.double(50.0),
    MinN = cms.int32(1)
)


hltPreIsoMu7BTagMuJet20 = cms.EDFilter("HLTPrescaler")


hltMuBSoftIsoL3IsoFiltered = cms.EDFilter("HLTMuonIsoFilter",
    PreviousCandTag = cms.InputTag("hltMuBSoftIsoL3PreFiltered"),
    MinN = cms.int32(1),
    SaveTag = cms.untracked.bool(True),
    CandTag = cms.InputTag("hltL3MuonCandidates"),
    IsolatorPSet = cms.PSet(

    ),
    DepTag = cms.VInputTag("hltL3MuonIsolations")
)


hltRapGap20U = cms.EDFilter("HLTRapGapFilter",
    maxEta = cms.double(5.0),
    minEta = cms.double(3.0),
    caloThresh = cms.double(20.0),
    inputTag = cms.InputTag("hltMCJetCorJetIcone5HF07"),
    saveTag = cms.untracked.bool(True)
)


hltPreHT200 = cms.EDFilter("HLTPrescaler")


hltL1sL1Mu14L1SingleEG10 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleMu14 AND L1_SingleEG10'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltSingleMuIsoL3IsoFiltered9 = cms.EDFilter("HLTMuonIsoFilter",
    PreviousCandTag = cms.InputTag("hltSingleMuIsoL3PreFiltered9"),
    MinN = cms.int32(1),
    SaveTag = cms.untracked.bool(True),
    CandTag = cms.InputTag("hltL3MuonCandidates"),
    IsolatorPSet = cms.PSet(

    ),
    DepTag = cms.VInputTag("hltL3MuonIsolations")
)


hltDiMuonIsoL2IsoFiltered = cms.EDFilter("HLTMuonIsoFilter",
    IsolatorPSet = cms.PSet(

    ),
    DepTag = cms.VInputTag("hltL2MuonIsolations"),
    PreviousCandTag = cms.InputTag("hltDiMuonIsoL2PreFiltered"),
    MinN = cms.int32(2),
    CandTag = cms.InputTag("hltL2MuonCandidates")
)


hltBSoftmuonL25filter = cms.EDFilter("HLTJetTag",
    JetTag = cms.InputTag("hltBSoftmuonL25BJetTags"),
    MinTag = cms.double(0.5),
    MaxTag = cms.double(99999.0),
    SaveTag = cms.bool(False),
    MinJets = cms.int32(1)
)


hltL1SingleMu0L1Filtered0 = cms.EDFilter("HLTMuonL1Filter",
    PreviousCandTag = cms.InputTag("hltL1sL1SingleMuOpenL1SingleMu0L1SingleMu3"),
    MinPt = cms.double(0.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    SelectQualities = cms.vint32(),
    CandTag = cms.InputTag("hltL1extraParticles")
)


hltL1NonIsoHLTIsoSingleElectronEt20LTIEleIdHcalIsolFilter = cms.EDFilter("HLTEgammaGenericFilter",
    doIsolated = cms.bool(False),
    thrOverE2EE = cms.double(-1.0),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    thrOverE2EB = cms.double(-1.0),
    thrRegularEE = cms.double(999999.9),
    thrOverEEE = cms.double(-1.0),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    thrOverEEB = cms.double(-1.0),
    thrRegularEB = cms.double(999999.9),
    lessThan = cms.bool(True),
    useEt = cms.bool(False),
    ncandcut = cms.int32(1),
    isoTag = cms.InputTag("hltL1IsolatedElectronHcalIsol"),
    candTag = cms.InputTag("hltL1NonIsoHLTIsoSingleElectronEt20LTIEleIdClusterShapeFilter"),
    nonIsoTag = cms.InputTag("hltL1NonIsolatedElectronHcalIsol")
)


hlt1jet1METAco = cms.EDFilter("HLT2JetMET",
    saveTags = cms.untracked.bool(True),
    MinMinv = cms.double(0.0),
    MinN = cms.int32(1),
    MaxMinv = cms.double(-1.0),
    MinDeta = cms.double(0.0),
    inputTag1 = cms.InputTag("hlt1jet100"),
    inputTag2 = cms.InputTag("hlt1MET60"),
    MaxDphi = cms.double(2.1),
    MaxDeta = cms.double(-1.0),
    MinDphi = cms.double(0.0)
)


hltTriggerType = cms.EDFilter("HLTTriggerTypeFilter",
    SelectedTriggerType = cms.int32(1)
)


hltSingleMuIsoL3IsoFiltered3 = cms.EDFilter("HLTMuonIsoFilter",
    PreviousCandTag = cms.InputTag("hltSingleMuIsoL3PreFiltered3"),
    MinN = cms.int32(1),
    SaveTag = cms.untracked.bool(True),
    CandTag = cms.InputTag("hltL3MuonCandidates"),
    IsolatorPSet = cms.PSet(

    ),
    DepTag = cms.VInputTag("hltL3MuonIsolations")
)


hltL1sL1SingleEG5 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleEG5'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltDoubleMuLevel1PathL1OpenFiltered = cms.EDFilter("HLTMuonL1Filter",
    PreviousCandTag = cms.InputTag("hltL1sL1DoubleMuOpen"),
    MinPt = cms.double(0.0),
    MinN = cms.int32(2),
    MaxEta = cms.double(2.5),
    SaveTag = cms.untracked.bool(True),
    SelectQualities = cms.vint32(),
    CandTag = cms.InputTag("hltL1extraParticles")
)


hltPreEle25SWEleIdLTIL1R = cms.EDFilter("HLTPrescaler")


hltMuBSoftIsoL2IsoFiltered = cms.EDFilter("HLTMuonIsoFilter",
    IsolatorPSet = cms.PSet(

    ),
    DepTag = cms.VInputTag("hltL2MuonIsolations"),
    PreviousCandTag = cms.InputTag("hltMuBSoftIsoL2PreFiltered"),
    MinN = cms.int32(1),
    CandTag = cms.InputTag("hltL2MuonCandidates")
)


hltSingleMu3L3Filtered3 = cms.EDFilter("HLTMuonL3PreFilter",
    PreviousCandTag = cms.InputTag("hltSingleMu3L2Filtered3"),
    MinPt = cms.double(3.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    MinNhits = cms.int32(0),
    SaveTag = cms.untracked.bool(True),
    NSigmaPt = cms.double(0.0),
    MaxDz = cms.double(9999.0),
    BeamSpotTag = cms.InputTag("hltOfflineBeamSpot"),
    CandTag = cms.InputTag("hltL3MuonCandidates"),
    MaxDr = cms.double(2.0)
)


hltMuNoL2IsoJetsL3PreFiltered = cms.EDFilter("HLTMuonL3PreFilter",
    PreviousCandTag = cms.InputTag("hltMuNoL2IsoJetsL2PreFiltered"),
    MinPt = cms.double(8.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    MinNhits = cms.int32(0),
    NSigmaPt = cms.double(0.0),
    MaxDz = cms.double(9999.0),
    BeamSpotTag = cms.InputTag("hltOfflineBeamSpot"),
    CandTag = cms.InputTag("hltL3MuonCandidates"),
    MaxDr = cms.double(2.0)
)


hltL1sL1BscMinBiasORBptxPlusORMinus = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_BscMinBiasOR_BptxPlusORMinus'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltMuNoIsoJetsL2PreFiltered = cms.EDFilter("HLTMuonL2PreFilter",
    PreviousCandTag = cms.InputTag("hltMuNoIsoJetsL1Filtered"),
    MinPt = cms.double(11.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    MinNhits = cms.int32(0),
    NSigmaPt = cms.double(0.0),
    MaxDz = cms.double(9999.0),
    BeamSpotTag = cms.InputTag("hltOfflineBeamSpot"),
    CandTag = cms.InputTag("hltL2MuonCandidates"),
    MaxDr = cms.double(9999.0)
)


hltAlCaPhiSymStream = cms.EDFilter("HLTEcalPhiSymFilter",
    eCut_endcap = cms.double(0.75),
    endcapHitCollection = cms.InputTag("hltEcalRecHitAll","EcalRecHitsEE"),
    eCut_endcap_high = cms.double(999999.0),
    eCut_barrel = cms.double(0.15),
    eCut_barrel_high = cms.double(999999.0),
    statusThreshold = cms.uint32(3),
    useRecoFlag = cms.bool(False),
    phiSymBarrelHitCollection = cms.string('phiSymEcalRecHitsEB'),
    barrelHitCollection = cms.InputTag("hltEcalRecHitAll","EcalRecHitsEB"),
    phiSymEndcapHitCollection = cms.string('phiSymEcalRecHitsEE')
)


hltL1Mu5Jet15L1Filtered0 = cms.EDFilter("HLTMuonL1Filter",
    PreviousCandTag = cms.InputTag("hltL1sL1Mu5Jet20"),
    MinPt = cms.double(0.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    SelectQualities = cms.vint32(),
    CandTag = cms.InputTag("hltL1extraParticles")
)


hltPreL2Mu5HT50 = cms.EDFilter("HLTPrescaler")


hltL1sL1Mu = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleMu7 OR L1_DoubleMu3'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltL1sL1EG5HTT100 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_EG5_HTT100'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltPhiJet1Jet2Aco = cms.EDFilter("HLTAcoFilter",
    saveTags = cms.untracked.bool(True),
    maxDeltaPhi = cms.double(2.7646),
    inputJetTag = cms.InputTag("hltIterativeCone5CaloJets"),
    Acoplanar = cms.string('Jet1Jet2'),
    inputMETTag = cms.InputTag("hlt1MET70"),
    minDeltaPhi = cms.double(0.0),
    minEtJet1 = cms.double(40.0),
    minEtJet2 = cms.double(40.0)
)


hltPreActivityL1A = cms.EDFilter("HLTPrescaler")


hltBSoftMuonL25FilterUByDR = cms.EDFilter("HLTJetTag",
    JetTag = cms.InputTag("hltBSoftMuonL25BJetTagsUByDR"),
    MinTag = cms.double(0.5),
    MaxTag = cms.double(99999.0),
    SaveTag = cms.bool(False),
    MinJets = cms.int32(1)
)


hltL1SingleMu5L1Filtered0 = cms.EDFilter("HLTMuonL1Filter",
    PreviousCandTag = cms.InputTag("hltL1sL1SingleMu5"),
    MinPt = cms.double(0.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    SelectQualities = cms.vint32(),
    CandTag = cms.InputTag("hltL1extraParticles")
)


hltL1NonIsoHLTNonIsoDoubleElectronEt10HcalIsolFilter = cms.EDFilter("HLTEgammaGenericFilter",
    doIsolated = cms.bool(False),
    thrOverE2EE = cms.double(-1.0),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    thrOverE2EB = cms.double(-1.0),
    thrRegularEE = cms.double(999999.9),
    thrOverEEE = cms.double(-1.0),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    thrOverEEB = cms.double(-1.0),
    thrRegularEB = cms.double(999999.9),
    lessThan = cms.bool(True),
    useEt = cms.bool(False),
    ncandcut = cms.int32(2),
    isoTag = cms.InputTag("hltL1IsolatedElectronHcalIsol"),
    candTag = cms.InputTag("hltL1NonIsoHLTNonIsoDoubleElectronEt10EtFilter"),
    nonIsoTag = cms.InputTag("hltL1NonIsolatedElectronHcalIsol")
)


hltL1sL1Jet15 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleJet15'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltFilterL25LeadingTrackPtCutDoubleLooseIsoTau15Trk5 = cms.EDFilter("HLT1Tau",
    MaxEta = cms.double(5.0),
    inputTag = cms.InputTag("hltL1HLTDoubleLooseIsoTau15Trk5JetsMatch"),
    MinPt = cms.double(15.0),
    MinN = cms.int32(2)
)


hltL1NonIsoHLTNonIsoSingleElectronEt20ESDoubleSC15 = cms.EDFilter("HLTEgammaEtFilter",
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    inputTag = cms.InputTag("hltL1NonIsoHLTNonIsoSingleElectronEt20ESscWrapper"),
    SaveTag = cms.untracked.bool(True),
    etcutEB = cms.double(15.0),
    ncandcut = cms.int32(2),
    etcutEE = cms.double(15.0)
)


hltBSoftMuonL3FilterUByDR = cms.EDFilter("HLTJetTag",
    JetTag = cms.InputTag("hltBSoftMuonL3BJetTagsUByDR"),
    MinTag = cms.double(0.5),
    MaxTag = cms.double(99999.0),
    SaveTag = cms.bool(True),
    MinJets = cms.int32(1)
)


hltEle10SWL1RTripleJet30EleHcalIsolFilter = cms.EDFilter("HLTEgammaGenericFilter",
    doIsolated = cms.bool(False),
    thrOverE2EE = cms.double(-1.0),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    thrOverE2EB = cms.double(-1.0),
    thrRegularEE = cms.double(999999.9),
    thrOverEEE = cms.double(-1.0),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    thrOverEEB = cms.double(-1.0),
    thrRegularEB = cms.double(999999.9),
    lessThan = cms.bool(True),
    useEt = cms.bool(False),
    ncandcut = cms.int32(1),
    isoTag = cms.InputTag("hltL1IsolatedElectronHcalIsol"),
    candTag = cms.InputTag("hltEle10SWL1RTripleJet30EleEtFilter"),
    nonIsoTag = cms.InputTag("hltL1NonIsolatedElectronHcalIsol")
)


hltPreEle10LWL1RHT200 = cms.EDFilter("HLTPrescaler")


hltL1sSingleLooseIsoTau20 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleTauJet20U OR L1_SingleJet30U'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltPreAlCaEcalPhiSym = cms.EDFilter("HLTPrescaler")


hltPreDiJetAve30U1E31 = cms.EDFilter("HLTPrescaler")


hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdEtFilter = cms.EDFilter("HLTEgammaEtFilter",
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    inputTag = cms.InputTag("hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdL1MatchFilterRegional"),
    etcutEB = cms.double(10.0),
    etcutEE = cms.double(10.0),
    ncandcut = cms.int32(1)
)


hltL1NonIsoHLTVLEIDoublePhotonEt15L1MatchFilterRegional = cms.EDFilter("HLTEgammaL1MatchFilterRegional",
    doIsolated = cms.bool(False),
    endcap_end = cms.double(2.65),
    candNonIsolatedTag = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    region_eta_size_ecap = cms.double(1.0),
    barrel_end = cms.double(1.4791),
    l1IsolatedTag = cms.InputTag("hltL1extraParticles","Isolated"),
    candIsolatedTag = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    region_phi_size = cms.double(1.044),
    region_eta_size = cms.double(0.522),
    L1SeedFilterTag = cms.InputTag("hltL1sL1DoubleEG5"),
    ncandcut = cms.int32(2),
    l1NonIsolatedTag = cms.InputTag("hltL1extraParticles","NonIsolated")
)


hltL1NonIsoHLTNonIsoSingleElectronEt15PixelMatchFilter = cms.EDFilter("HLTElectronPixelMatchFilter",
    doIsolated = cms.bool(False),
    L1IsoPixelSeedsTag = cms.InputTag("hltL1IsoStartUpElectronPixelSeeds"),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    L1NonIsoPixelSeedsTag = cms.InputTag("hltL1NonIsoStartUpElectronPixelSeeds"),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    SaveTag = cms.untracked.bool(True),
    npixelmatchcut = cms.double(1.0),
    ncandcut = cms.int32(1),
    candTag = cms.InputTag("hltL1NonIsoHLTNonIsoSingleElectronEt15HcalIsolFilter")
)


hltHcalSimpleRecHitFilter = cms.EDFilter("HLTHcalSimpleRecHitFilter",
    threshold = cms.double(3.0),
    maskedChannels = cms.vint32(8137, 8141, 8146, 8149, 8150, 
        8153),
    HFRecHitCollection = cms.InputTag("hltHfreco")
)


hltL1sMET60 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_ETM40'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltPreIsoMu14IsoTauTrk3NoL2 = cms.EDFilter("HLTPrescaler")


hltRandomEventsFilter = cms.EDFilter("HLTTriggerTypeFilter",
    SelectedTriggerType = cms.int32(3)
)


hltMuNoL2IsoJetsL2PreFiltered = cms.EDFilter("HLTMuonL2PreFilter",
    PreviousCandTag = cms.InputTag("hltMuNoL2IsoJetsL1Filtered"),
    MinPt = cms.double(6.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    MinNhits = cms.int32(0),
    NSigmaPt = cms.double(0.0),
    MaxDz = cms.double(9999.0),
    BeamSpotTag = cms.InputTag("hltOfflineBeamSpot"),
    CandTag = cms.InputTag("hltL2MuonCandidates"),
    MaxDr = cms.double(9999.0)
)


hltPreIsoMu14IsoTauTrk3NoL1 = cms.EDFilter("HLTPrescaler")


hltPrePhoton30L1R8E29 = cms.EDFilter("HLTPrescaler")


hltL1NonIsoHLTIsoSingleElectronEt20LTIEleIdClusterShapeFilter = cms.EDFilter("HLTEgammaGenericFilter",
    doIsolated = cms.bool(False),
    thrOverE2EE = cms.double(-1.0),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    thrOverE2EB = cms.double(-1.0),
    thrRegularEE = cms.double(0.04),
    thrOverEEE = cms.double(-1.0),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    thrOverEEB = cms.double(-1.0),
    thrRegularEB = cms.double(0.015),
    lessThan = cms.bool(True),
    useEt = cms.bool(False),
    ncandcut = cms.int32(1),
    isoTag = cms.InputTag("hltL1IsoHLTClusterShape"),
    candTag = cms.InputTag("hltL1NonIsoHLTIsoSingleElectronEt20LTIEleIdEtFilter"),
    nonIsoTag = cms.InputTag("hltL1NonIsoHLTClusterShape")
)


hltUpsilonMML1Filtered = cms.EDFilter("HLTMuonL1Filter",
    PreviousCandTag = cms.InputTag("hltL1sL1DoubleMu3"),
    MinPt = cms.double(0.0),
    MinN = cms.int32(2),
    MaxEta = cms.double(2.5),
    SelectQualities = cms.vint32(),
    CandTag = cms.InputTag("hltL1extraParticles")
)


hltPreL2Mu7Photon9L1R = cms.EDFilter("HLTPrescaler")


hltPreFwdJet40 = cms.EDFilter("HLTPrescaler")


hltPreEle15SWL1R = cms.EDFilter("HLTPrescaler")


hltPreL1BscMinBiasORBptxPlusORMinus = cms.EDFilter("HLTPrescaler")


hltL1sMET100 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_ETM70'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltPreEle15SWEleIdL1R = cms.EDFilter("HLTPrescaler")


hltL1NonIsoHLTLEITISinglePhotonEt25EcalIsolFilter = cms.EDFilter("HLTEgammaGenericFilter",
    doIsolated = cms.bool(False),
    thrOverE2EE = cms.double(-1.0),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    thrOverE2EB = cms.double(-1.0),
    thrRegularEE = cms.double(3.0),
    thrOverEEE = cms.double(0.1),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    thrOverEEB = cms.double(0.1),
    thrRegularEB = cms.double(3.0),
    lessThan = cms.bool(True),
    useEt = cms.bool(False),
    ncandcut = cms.int32(1),
    isoTag = cms.InputTag("hltL1IsolatedPhotonEcalIsol"),
    candTag = cms.InputTag("hltL1NonIsoHLTLEITISinglePhotonEt25EtFilter"),
    nonIsoTag = cms.InputTag("hltL1NonIsolatedPhotonEcalIsol")
)


hltPreEle25LWL1R = cms.EDFilter("HLTPrescaler")


hltPreMu14Jet50 = cms.EDFilter("HLTPrescaler")


hltL1sL1DoubleMuOpen = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_DoubleMuOpen'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltL1sL1DoubleMu3 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_DoubleMu3'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltPrePhoton20HLTLEITIL1R = cms.EDFilter("HLTPrescaler")


hltPreDoubleJet150 = cms.EDFilter("HLTPrescaler")


hltL1NonIsoHLTNonIsoDoubleElectronEt5L1MatchFilterRegional = cms.EDFilter("HLTEgammaL1MatchFilterRegional",
    doIsolated = cms.bool(False),
    endcap_end = cms.double(2.65),
    candNonIsolatedTag = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    region_eta_size_ecap = cms.double(1.0),
    barrel_end = cms.double(1.4791),
    l1IsolatedTag = cms.InputTag("hltL1extraParticles","Isolated"),
    candIsolatedTag = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    region_phi_size = cms.double(1.044),
    region_eta_size = cms.double(0.522),
    L1SeedFilterTag = cms.InputTag("hltL1sL1DoubleEG5"),
    ncandcut = cms.int32(2),
    l1NonIsolatedTag = cms.InputTag("hltL1extraParticles","NonIsolated")
)


hltPreHighMultiplicityBSC = cms.EDFilter("HLTPrescaler")


hltPreMinBiasBSCOR = cms.EDFilter("HLTPrescaler")


hltPrePhoton30L1R1E31 = cms.EDFilter("HLTPrescaler")


hltPreMinBiasBSC = cms.EDFilter("HLTPrescaler")


hltL1NonIsoDoublePhotonEt5eeResEtFilter = cms.EDFilter("HLTEgammaEtFilter",
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    inputTag = cms.InputTag("hltL1NonIsoDoublePhotonEt5eeResL1MatchFilterRegional"),
    etcutEB = cms.double(4.0),
    etcutEE = cms.double(4.0),
    ncandcut = cms.int32(2)
)


hltPreSplashBSC = cms.EDFilter("HLTPrescaler")


hltHT240 = cms.EDFilter("HLTGlobalSumsMET",
    observable = cms.string('sumEt'),
    MinN = cms.int32(1),
    Min = cms.double(240.0),
    Max = cms.double(-1.0),
    inputTag = cms.InputTag("hltJet30Ht"),
    saveTag = cms.untracked.bool(True)
)


hltPreDoublePhoton15L1R = cms.EDFilter("HLTPrescaler")


hltL1NonIsoSinglePhotonEt15LEIEtFilter = cms.EDFilter("HLTEgammaEtFilter",
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    inputTag = cms.InputTag("hltL1NonIsoSinglePhotonEt15LEIL1MatchFilterRegional"),
    etcutEB = cms.double(15.0),
    etcutEE = cms.double(15.0),
    ncandcut = cms.int32(1)
)


hltL2Filtered9L2Mu9DoubleJet30 = cms.EDFilter("HLTMuonL2PreFilter",
    PreviousCandTag = cms.InputTag("hltL1Mu5Jet15L1Filtered0"),
    MinPt = cms.double(9.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    MinNhits = cms.int32(0),
    SaveTag = cms.untracked.bool(True),
    NSigmaPt = cms.double(0.0),
    MaxDz = cms.double(9999.0),
    BeamSpotTag = cms.InputTag("hltOfflineBeamSpot"),
    CandTag = cms.InputTag("hltL2MuonCandidates"),
    MaxDr = cms.double(9999.0)
)


hltHIPhoton10 = cms.EDFilter("HLT1Photon",
    MaxEta = cms.double(1.479),
    inputTag = cms.InputTag("hltRecoHIEcalCandidate"),
    MinPt = cms.double(10.0),
    MinN = cms.int32(1)
)


hltFilterL25PixelTracksIsolationMuonTauNoL1Tau = cms.EDFilter("HLT1Tau",
    MaxEta = cms.double(5.0),
    saveTag = cms.untracked.bool(True),
    inputTag = cms.InputTag("hltL25TauPixelTracksIsolationSelector"),
    MinPt = cms.double(15.0),
    MinN = cms.int32(1)
)


hltPreIsoTrackHE8E29 = cms.EDFilter("HLTPrescaler")


hltL1sJet30 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleJet15'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltHIPreJet35U = cms.EDFilter("HLTPrescaler")


hltPreHIMinBiasCalo = cms.EDFilter("HLTPrescaler")


hltL1sJet15U = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleJet6U'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltL1sBTagMuJet10U = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_Mu3_Jet6U'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltDiJetAve50U = cms.EDFilter("HLTDiJetAveFilter",
    inputJetTag = cms.InputTag("hltIterativeCone5CaloJets"),
    minPtAve = cms.double(50.0),
    minPtJet3 = cms.double(99999.0),
    saveTag = cms.untracked.bool(True),
    minDphi = cms.double(-1.0)
)


hltPreSingleLooseIsoTau20 = cms.EDFilter("HLTPrescaler")


hltL1NonIsoHLTNonIsoSinglePhotonEt15L1MatchFilterRegional = cms.EDFilter("HLTEgammaL1MatchFilterRegional",
    doIsolated = cms.bool(False),
    endcap_end = cms.double(2.65),
    candNonIsolatedTag = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    region_eta_size_ecap = cms.double(1.0),
    barrel_end = cms.double(1.4791),
    l1IsolatedTag = cms.InputTag("hltL1extraParticles","Isolated"),
    candIsolatedTag = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    region_phi_size = cms.double(1.044),
    region_eta_size = cms.double(0.522),
    L1SeedFilterTag = cms.InputTag("hltL1sL1SingleEG8"),
    ncandcut = cms.int32(1),
    l1NonIsolatedTag = cms.InputTag("hltL1extraParticles","NonIsolated")
)


hltL1NonIsoDoublePhotonEt5JpsiEcalIsolFilter = cms.EDFilter("HLTEgammaGenericFilter",
    doIsolated = cms.bool(False),
    thrOverE2EE = cms.double(-1.0),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    thrOverE2EB = cms.double(-1.0),
    thrRegularEE = cms.double(999999.9),
    thrOverEEE = cms.double(-1.0),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    thrOverEEB = cms.double(-1.0),
    thrRegularEB = cms.double(999999.9),
    lessThan = cms.bool(True),
    useEt = cms.bool(False),
    ncandcut = cms.int32(2),
    isoTag = cms.InputTag("hltL1IsolatedPhotonEcalIsol"),
    candTag = cms.InputTag("hltL1NonIsoDoublePhotonEt5JpsiClusterShapeFilter"),
    nonIsoTag = cms.InputTag("hltL1NonIsolatedPhotonEcalIsol")
)


hltPreJet100MET60Aco = cms.EDFilter("HLTPrescaler")


hltL1NonIsoDoubleElectronEt5JpsiL1MatchFilterRegional = cms.EDFilter("HLTEgammaL1MatchFilterRegional",
    doIsolated = cms.bool(False),
    endcap_end = cms.double(2.65),
    candNonIsolatedTag = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    region_eta_size_ecap = cms.double(1.0),
    barrel_end = cms.double(1.4791),
    l1IsolatedTag = cms.InputTag("hltL1extraParticles","Isolated"),
    candIsolatedTag = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    region_phi_size = cms.double(1.044),
    region_eta_size = cms.double(0.522),
    L1SeedFilterTag = cms.InputTag("hltL1sL1SingleEG8orL1DoubleEG5"),
    ncandcut = cms.int32(2),
    l1NonIsolatedTag = cms.InputTag("hltL1extraParticles","NonIsolated")
)


hltPreL2Mu9DoubleJet30 = cms.EDFilter("HLTPrescaler")


hltL1NonIsoDoubleElectronEt5UpsHcalIsolFilter = cms.EDFilter("HLTEgammaGenericFilter",
    doIsolated = cms.bool(False),
    thrOverE2EE = cms.double(-1.0),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    thrOverE2EB = cms.double(-1.0),
    thrRegularEE = cms.double(999999.9),
    thrOverEEE = cms.double(-1.0),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    thrOverEEB = cms.double(-1.0),
    thrRegularEB = cms.double(999999.9),
    lessThan = cms.bool(True),
    useEt = cms.bool(False),
    ncandcut = cms.int32(2),
    isoTag = cms.InputTag("hltL1IsolatedElectronHcalIsol"),
    candTag = cms.InputTag("hltL1NonIsoDoubleElectronEt5UpsEcalIsolFilter"),
    nonIsoTag = cms.InputTag("hltL1NonIsolatedElectronHcalIsol")
)


hltPreEle10SWL1R = cms.EDFilter("HLTPrescaler")


hltL1sHT240 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_HTT100'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltL2Mu7Photon9EtFilter = cms.EDFilter("HLTEgammaEtFilter",
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    inputTag = cms.InputTag("hltL2Mu7Photon9L1MatchFilterRegional"),
    etcutEB = cms.double(9.0),
    etcutEE = cms.double(9.0),
    ncandcut = cms.int32(1)
)


hltFilterL2EcalIsolationSingleLooseIsoTau20 = cms.EDFilter("HLT1Tau",
    MaxEta = cms.double(5.0),
    saveTag = cms.untracked.bool(True),
    inputTag = cms.InputTag("hltL1HLTSingleLooseIsoTau20JetsMatch"),
    MinPt = cms.double(20.0),
    MinN = cms.int32(1)
)


hltPreEle10LWL1R = cms.EDFilter("HLTPrescaler")


hlt1MET100 = cms.EDFilter("HLT1CaloMET",
    MaxEta = cms.double(-1.0),
    saveTag = cms.untracked.bool(True),
    inputTag = cms.InputTag("hltMet"),
    MinPt = cms.double(100.0),
    MinN = cms.int32(1)
)


hltL1NonIsoHLTNonIsoDoublePhotonEt15HcalIsolFilter = cms.EDFilter("HLTEgammaGenericFilter",
    doIsolated = cms.bool(False),
    thrOverE2EE = cms.double(-1.0),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    thrOverE2EB = cms.double(-1.0),
    thrRegularEE = cms.double(999999.9),
    thrOverEEE = cms.double(-1.0),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    SaveTag = cms.untracked.bool(True),
    thrRegularEB = cms.double(999999.9),
    lessThan = cms.bool(True),
    useEt = cms.bool(False),
    ncandcut = cms.int32(2),
    thrOverEEB = cms.double(-1.0),
    isoTag = cms.InputTag("hltL1IsolatedPhotonHcalIsol"),
    candTag = cms.InputTag("hltL1NonIsoHLTNonIsoDoublePhotonEt15EtFilter"),
    nonIsoTag = cms.InputTag("hltL1NonIsolatedPhotonHcalIsol")
)


hltL1sIsoTrack8E29 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleJet20U OR L1_SingleJet30U OR L1_SingleJet40U OR L1_SingleJet50U OR L1_SingleJet60U OR L1_SingleTauJet10U OR L1_SingleTauJet20U OR L1_SingleTauJet30U OR L1_SingleTauJet50U'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltPhiJet2metAco = cms.EDFilter("HLTAcoFilter",
    saveTags = cms.untracked.bool(True),
    maxDeltaPhi = cms.double(3.141593),
    inputJetTag = cms.InputTag("hltIterativeCone5CaloJets"),
    Acoplanar = cms.string('Jet2Met'),
    inputMETTag = cms.InputTag("hlt1MET70"),
    minDeltaPhi = cms.double(0.377),
    minEtJet1 = cms.double(50.0),
    minEtJet2 = cms.double(50.0)
)


hltL1sHighMult40 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_ETT60'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(3),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltL1sL1SingleEG20 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleEG20'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(3),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltPreHcalCalibration = cms.EDFilter("HLTPrescaler")


hltPreMinBiasPixelDoubleIsoTrack5 = cms.EDFilter("HLTPrescaler")


hltL1NonIsoHLTNonIsoDoublePhotonEt10HcalIsolFilter = cms.EDFilter("HLTEgammaGenericFilter",
    doIsolated = cms.bool(False),
    thrOverE2EE = cms.double(-1.0),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    thrOverE2EB = cms.double(-1.0),
    thrRegularEE = cms.double(999999.9),
    thrOverEEE = cms.double(-1.0),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    SaveTag = cms.untracked.bool(True),
    thrRegularEB = cms.double(999999.9),
    lessThan = cms.bool(True),
    useEt = cms.bool(False),
    ncandcut = cms.int32(2),
    thrOverEEB = cms.double(-1.0),
    isoTag = cms.InputTag("hltL1IsolatedPhotonHcalIsol"),
    candTag = cms.InputTag("hltL1NonIsoHLTNonIsoDoublePhotonEt10EtFilter"),
    nonIsoTag = cms.InputTag("hltL1NonIsolatedPhotonHcalIsol")
)


hlt1jet15U = cms.EDFilter("HLT1CaloJet",
    MaxEta = cms.double(5.0),
    saveTag = cms.untracked.bool(True),
    inputTag = cms.InputTag("hltMCJetCorJetIcone5HF07"),
    MinPt = cms.double(15.0),
    MinN = cms.int32(1)
)


hltFilterL2EcalIsolationMuonTau = cms.EDFilter("HLT1Tau",
    MaxEta = cms.double(5.0),
    inputTag = cms.InputTag("hltL2TauRelaxingIsolationSelector","Isolated"),
    MinPt = cms.double(15.0),
    MinN = cms.int32(1)
)


hltPreIsoMu14IsoTauTrk3 = cms.EDFilter("HLTPrescaler")


hltL1NonIsoHLTNonIsoSingleElectronEt15EleIdHcalIsolFilter = cms.EDFilter("HLTEgammaGenericFilter",
    doIsolated = cms.bool(False),
    thrOverE2EE = cms.double(-1.0),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    thrOverE2EB = cms.double(-1.0),
    thrRegularEE = cms.double(999999.9),
    thrOverEEE = cms.double(-1.0),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    thrOverEEB = cms.double(-1.0),
    thrRegularEB = cms.double(999999.9),
    lessThan = cms.bool(True),
    useEt = cms.bool(False),
    ncandcut = cms.int32(1),
    isoTag = cms.InputTag("hltL1IsolatedElectronHcalIsol"),
    candTag = cms.InputTag("hltL1NonIsoHLTNonIsoSingleElectronEt15EleIdClusterShapeFilter"),
    nonIsoTag = cms.InputTag("hltL1NonIsolatedElectronHcalIsol")
)


hltPreJet50U = cms.EDFilter("HLTPrescaler")


hltL1NonIsoDoubleElectronEt5UpsPixelMatchFilter = cms.EDFilter("HLTElectronPixelMatchFilter",
    doIsolated = cms.bool(False),
    L1IsoPixelSeedsTag = cms.InputTag("hltL1IsoStartUpElectronPixelSeeds"),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    L1NonIsoPixelSeedsTag = cms.InputTag("hltL1NonIsoStartUpElectronPixelSeeds"),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    npixelmatchcut = cms.double(1.0),
    ncandcut = cms.int32(2),
    candTag = cms.InputTag("hltL1NonIsoDoubleElectronEt5UpsHcalIsolFilter")
)


hltL1NonIsoHLTNonIsoSingleElectronEt15LTITrackIsolFilter = cms.EDFilter("HLTElectronGenericFilter",
    doIsolated = cms.bool(False),
    nonIsoTag = cms.InputTag("hltL1NonIsoElectronTrackIsol"),
    L1NonIsoCand = cms.InputTag("hltPixelMatchElectronsL1NonIso"),
    thrRegularEE = cms.double(0.5),
    L1IsoCand = cms.InputTag("hltPixelMatchElectronsL1Iso"),
    SaveTag = cms.untracked.bool(True),
    thrRegularEB = cms.double(0.5),
    lessThan = cms.bool(True),
    ncandcut = cms.int32(1),
    isoTag = cms.InputTag("hltL1IsoElectronTrackIsol"),
    thrOverPtEE = cms.double(-1.0),
    candTag = cms.InputTag("hltL1NonIsoHLTNonIsoSingleElectronEt15LTIOneOEMinusOneOPFilter"),
    thrTimesPtEE = cms.double(8.0),
    thrTimesPtEB = cms.double(8.0),
    thrOverPtEB = cms.double(-1.0)
)


hltHIPreMML1 = cms.EDFilter("HLTPrescaler")


hltPreDoubleEle5SWL1R = cms.EDFilter("HLTPrescaler")


hltHIPrePhoton20 = cms.EDFilter("HLTPrescaler")


hltL1sJet250 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleJet70'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdHcalIsolFilter = cms.EDFilter("HLTEgammaGenericFilter",
    doIsolated = cms.bool(False),
    thrOverE2EE = cms.double(-1.0),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    thrOverE2EB = cms.double(-1.0),
    thrRegularEE = cms.double(999999.9),
    thrOverEEE = cms.double(-1.0),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    thrOverEEB = cms.double(-1.0),
    thrRegularEB = cms.double(999999.9),
    lessThan = cms.bool(True),
    useEt = cms.bool(False),
    ncandcut = cms.int32(1),
    isoTag = cms.InputTag("hltL1IsolatedElectronHcalIsol"),
    candTag = cms.InputTag("hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdClusterShapeFilter"),
    nonIsoTag = cms.InputTag("hltL1NonIsolatedElectronHcalIsol")
)


hltPreIsoTrackHB1E31 = cms.EDFilter("HLTPrescaler")


hltHT50Mu5HT50 = cms.EDFilter("HLTGlobalSumsMET",
    observable = cms.string('sumEt'),
    MinN = cms.int32(1),
    Min = cms.double(50.0),
    Max = cms.double(-1.0),
    inputTag = cms.InputTag("hltJet30Ht"),
    saveTag = cms.untracked.bool(True)
)


hlt2jetAco = cms.EDFilter("HLT2JetJet",
    saveTags = cms.untracked.bool(True),
    MinMinv = cms.double(0.0),
    MinN = cms.int32(1),
    MaxMinv = cms.double(-1.0),
    MinDeta = cms.double(0.0),
    inputTag1 = cms.InputTag("hlt2jet125"),
    inputTag2 = cms.InputTag("hlt2jet125"),
    MaxDphi = cms.double(2.1),
    MaxDeta = cms.double(-1.0),
    MinDphi = cms.double(0.0)
)


hltMuBLifetimeIsoL3IsoFiltered = cms.EDFilter("HLTMuonIsoFilter",
    PreviousCandTag = cms.InputTag("hltMuBLifetimeIsoL3PreFiltered"),
    MinN = cms.int32(1),
    SaveTag = cms.untracked.bool(True),
    CandTag = cms.InputTag("hltL3MuonCandidates"),
    IsolatorPSet = cms.PSet(

    ),
    DepTag = cms.VInputTag("hltL3MuonIsolations")
)


hltL1NonIsoDoubleElectronEt5UpsEtFilter = cms.EDFilter("HLTEgammaEtFilter",
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    inputTag = cms.InputTag("hltL1NonIsoDoubleElectronEt5UpsL1MatchFilterRegional"),
    etcutEB = cms.double(4.0),
    etcutEE = cms.double(4.0),
    ncandcut = cms.int32(2)
)


hltPreIsoMu9 = cms.EDFilter("HLTPrescaler")


hltPreDoubleJet40MET70Aco = cms.EDFilter("HLTPrescaler")


hltIsolPixelTrackL2FilterHE8E29 = cms.EDFilter("HLTPixelIsolTrackFilter",
    MaxPtNearby = cms.double(2.0),
    MinEtaTrack = cms.double(1.4),
    MinEnergyTrack = cms.double(20.0),
    MinPtTrack = cms.double(3.5),
    DropMultiL2Event = cms.bool(False),
    NMaxTrackCandidates = cms.int32(5),
    MaxEtaTrack = cms.double(2.0),
    candTag = cms.InputTag("hltIsolPixelTrackProdHE8E29"),
    filterTrackEnergy = cms.bool(True)
)


hltL1sBTagIPJet120 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleJet70'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltPreAlCaEcalEta8E29 = cms.EDFilter("HLTPrescaler")


openhltTauPrescaler = cms.EDFilter("HLTPrescaler",
    makeFilterObject = cms.bool(True),
    eventOffset = cms.uint32(0),
    prescaleFactor = cms.uint32(1)
)


hltPreIsoMu3 = cms.EDFilter("HLTPrescaler")


hltHIL1sPhoton30 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleEG5'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltL1NonIsoHLTLEITISinglePhotonEt10HcalIsolFilter = cms.EDFilter("HLTEgammaGenericFilter",
    doIsolated = cms.bool(False),
    thrOverE2EE = cms.double(-1.0),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    thrOverE2EB = cms.double(-1.0),
    thrRegularEE = cms.double(999999.9),
    thrOverEEE = cms.double(-1.0),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    thrOverEEB = cms.double(-1.0),
    thrRegularEB = cms.double(999999.9),
    lessThan = cms.bool(True),
    useEt = cms.bool(False),
    ncandcut = cms.int32(1),
    isoTag = cms.InputTag("hltL1IsolatedPhotonHcalIsol"),
    candTag = cms.InputTag("hltL1NonIsoHLTLEITISinglePhotonEt10EcalIsolFilter"),
    nonIsoTag = cms.InputTag("hltL1NonIsolatedPhotonHcalIsol")
)


hltDiMuonIsoL1Filtered = cms.EDFilter("HLTMuonL1Filter",
    PreviousCandTag = cms.InputTag("hltL1sL1DoubleMu3"),
    MinPt = cms.double(0.0),
    MinN = cms.int32(2),
    MaxEta = cms.double(2.5),
    SelectQualities = cms.vint32(),
    CandTag = cms.InputTag("hltL1extraParticles")
)


hltL1NonIsoDoubleElectronEt5JpsiPMMassFilter = cms.EDFilter("HLTPMMassFilter",
    lowerMassCut = cms.double(2.0),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    isElectron1 = cms.untracked.bool(False),
    SaveTag = cms.untracked.bool(True),
    isElectron2 = cms.untracked.bool(False),
    upperMassCut = cms.double(4.6),
    candTag = cms.InputTag("hltL1NonIsoDoubleElectronEt5JpsiPixelMatchFilter"),
    nZcandcut = cms.int32(1),
    reqOppCharge = cms.untracked.bool(True)
)


hltL1sDiJetAve15U1E31 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleJet15'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltL1NonIsoHLTNonIsoSinglePhotonEt20HcalIsolFilter = cms.EDFilter("HLTEgammaGenericFilter",
    doIsolated = cms.bool(False),
    thrOverE2EE = cms.double(-1.0),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    thrOverE2EB = cms.double(-1.0),
    thrRegularEE = cms.double(999999.9),
    thrOverEEE = cms.double(-1.0),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    SaveTag = cms.untracked.bool(True),
    thrRegularEB = cms.double(999999.9),
    lessThan = cms.bool(True),
    useEt = cms.bool(False),
    ncandcut = cms.int32(1),
    thrOverEEB = cms.double(-1.0),
    isoTag = cms.InputTag("hltL1IsolatedPhotonHcalIsol"),
    candTag = cms.InputTag("hltL1NonIsoHLTNonIsoSinglePhotonEt20EtFilter"),
    nonIsoTag = cms.InputTag("hltL1NonIsolatedPhotonHcalIsol")
)


hlt1jet50U = cms.EDFilter("HLT1CaloJet",
    MaxEta = cms.double(5.0),
    saveTag = cms.untracked.bool(True),
    inputTag = cms.InputTag("hltMCJetCorJetIcone5HF07"),
    MinPt = cms.double(50.0),
    MinN = cms.int32(1)
)


hltL1NonIsoHLTNonIsoSingleElectronSiStripEt20HcalIsolFilter = cms.EDFilter("HLTEgammaGenericFilter",
    doIsolated = cms.bool(False),
    thrOverE2EE = cms.double(-1.0),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    thrOverE2EB = cms.double(-1.0),
    thrRegularEE = cms.double(999999.9),
    thrOverEEE = cms.double(-1.0),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    thrOverEEB = cms.double(-1.0),
    thrRegularEB = cms.double(999999.9),
    lessThan = cms.bool(True),
    useEt = cms.bool(False),
    ncandcut = cms.int32(1),
    isoTag = cms.InputTag("hltL1IsolatedElectronHcalIsol"),
    candTag = cms.InputTag("hltL1NonIsoHLTNonIsoSingleElectronSiStripEt20EtFilter"),
    nonIsoTag = cms.InputTag("hltL1NonIsolatedElectronHcalIsol")
)


hltPreL1TechHFcoincidencePM = cms.EDFilter("HLTPrescaler")


hltL1sL1SingleEG2 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleEG2'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltL1NonIsoHLTIsoSingleElectronEt20LTIEleIdPixelMatchFilter = cms.EDFilter("HLTElectronPixelMatchFilter",
    doIsolated = cms.bool(False),
    L1IsoPixelSeedsTag = cms.InputTag("hltL1IsoStartUpElectronPixelSeeds"),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    L1NonIsoPixelSeedsTag = cms.InputTag("hltL1NonIsoStartUpElectronPixelSeeds"),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    npixelmatchcut = cms.double(1.0),
    ncandcut = cms.int32(1),
    candTag = cms.InputTag("hltL1NonIsoHLTIsoSingleElectronEt20LTIEleIdHcalIsolFilter")
)


hlt1MET35 = cms.EDFilter("HLT1CaloMET",
    MaxEta = cms.double(-1.0),
    saveTag = cms.untracked.bool(True),
    inputTag = cms.InputTag("hltMet"),
    MinPt = cms.double(35.0),
    MinN = cms.int32(1)
)


hltL1NonIsoHLTLEITISinglePhotonEt10TrackIsolFilter = cms.EDFilter("HLTEgammaGenericFilter",
    doIsolated = cms.bool(False),
    thrOverE2EE = cms.double(-1.0),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    thrOverE2EB = cms.double(-1.0),
    thrRegularEE = cms.double(4.0),
    thrOverEEE = cms.double(0.05),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    SaveTag = cms.untracked.bool(True),
    thrRegularEB = cms.double(4.0),
    lessThan = cms.bool(True),
    useEt = cms.bool(True),
    ncandcut = cms.int32(1),
    thrOverEEB = cms.double(0.05),
    isoTag = cms.InputTag("hltL1IsoPhotonHollowTrackIsol"),
    candTag = cms.InputTag("hltL1NonIsoHLTLEITISinglePhotonEt10HcalIsolFilter"),
    nonIsoTag = cms.InputTag("hltL1NonIsoPhotonHollowTrackIsol")
)


simGmtDigis = cms.EDFilter("L1MuGlobalMuonTrigger",
    BX_min_readout = cms.int32(-2),
    BX_min = cms.int32(-4),
    CSCCandidates = cms.InputTag("simCsctfDigis","CSC"),
    BX_max = cms.int32(4),
    BX_max_readout = cms.int32(2),
    Debug = cms.untracked.int32(0),
    RPCbCandidates = cms.InputTag("simRpcTriggerDigis","RPCb"),
    DTCandidates = cms.InputTag("simDttfDigis","DT"),
    WriteLUTsAndRegs = cms.untracked.bool(False),
    RPCfCandidates = cms.InputTag("simRpcTriggerDigis","RPCf"),
    MipIsoData = cms.InputTag("simRctDigis")
)


hltL1NonIsoHLTIsoSingleElectronEt20LTIEleIdOneOEMinusOneOPFilter = cms.EDFilter("HLTElectronOneOEMinusOneOPFilterRegional",
    doIsolated = cms.bool(False),
    electronNonIsolatedProducer = cms.InputTag("hltPixelMatchElectronsL1NonIso"),
    barrelcut = cms.double(999.9),
    electronIsolatedProducer = cms.InputTag("hltPixelMatchElectronsL1Iso"),
    ncandcut = cms.int32(1),
    candTag = cms.InputTag("hltL1NonIsoHLTIsoSingleElectronEt20LTIEleIdPixelMatchFilter"),
    endcapcut = cms.double(999.9)
)


hltL1sMinBiasBSCOR = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('34'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(True)
)


hltL1sJet180 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleJet70'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltOverlapsHLTCSCBeamHaloOverlapRing2 = cms.EDFilter("HLTCSCOverlapFilter",
    fillHists = cms.bool(False),
    minHits = cms.uint32(4),
    ring2 = cms.bool(True),
    ring1 = cms.bool(False),
    yWindow = cms.double(2.0),
    input = cms.InputTag("hltCsc2DRecHits"),
    xWindow = cms.double(2.0)
)


hltL1NonIsoHLTNonIsoSingleElectronEt15EtFilter = cms.EDFilter("HLTEgammaEtFilter",
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    inputTag = cms.InputTag("hltL1NonIsoHLTNonIsoSingleElectronEt15L1MatchFilterRegional"),
    etcutEB = cms.double(15.0),
    etcutEE = cms.double(15.0),
    ncandcut = cms.int32(1)
)


hltHI1jet90U = cms.EDFilter("HLT1CaloJet",
    MaxEta = cms.double(5.0),
    saveTag = cms.untracked.bool(True),
    inputTag = cms.InputTag("hltIterativeCone5PileupSubtractionCaloJets"),
    MinPt = cms.double(90.0),
    MinN = cms.int32(1)
)


hltL1sTrackPointing = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleMuOpen'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltPreL1HFTech = cms.EDFilter("HLTPrescaler")


hltL1sBTagIPJet80 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleJet70'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltPrePhoton10L1R = cms.EDFilter("HLTPrescaler")


hltL1NonIsoDoublePhotonEt5JpsiClusterShapeFilter = cms.EDFilter("HLTEgammaGenericFilter",
    doIsolated = cms.bool(False),
    thrOverE2EE = cms.double(-1.0),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    thrOverE2EB = cms.double(-1.0),
    thrRegularEE = cms.double(0.042),
    thrOverEEE = cms.double(-1.0),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    thrOverEEB = cms.double(-1.0),
    thrRegularEB = cms.double(0.016),
    lessThan = cms.bool(True),
    useEt = cms.bool(False),
    ncandcut = cms.int32(2),
    isoTag = cms.InputTag("hltL1IsoHLTClusterShape"),
    candTag = cms.InputTag("hltL1NonIsoDoublePhotonEt5JpsiEtFilter"),
    nonIsoTag = cms.InputTag("hltL1NonIsoHLTClusterShape")
)


hltL1NonIsoHLTNonIsoDoubleElectronEt5HcalIsolFilter = cms.EDFilter("HLTEgammaGenericFilter",
    doIsolated = cms.bool(False),
    thrOverE2EE = cms.double(-1.0),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    thrOverE2EB = cms.double(-1.0),
    thrRegularEE = cms.double(999999.9),
    thrOverEEE = cms.double(-1.0),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    thrOverEEB = cms.double(-1.0),
    thrRegularEB = cms.double(999999.9),
    lessThan = cms.bool(True),
    useEt = cms.bool(False),
    ncandcut = cms.int32(2),
    isoTag = cms.InputTag("hltL1IsolatedElectronHcalIsol"),
    candTag = cms.InputTag("hltL1NonIsoHLTNonIsoDoubleElectronEt5EtFilter"),
    nonIsoTag = cms.InputTag("hltL1NonIsolatedElectronHcalIsol")
)


hltPreDoublePhoton5eeResL1R = cms.EDFilter("HLTPrescaler")


hltFilterL3IsolationCutSingleIsoTau30Trk5 = cms.EDFilter("HLT1Tau",
    MaxEta = cms.double(5.0),
    saveTag = cms.untracked.bool(True),
    inputTag = cms.InputTag("hltL1HLTSingleIsoTau30JetsMatch"),
    MinPt = cms.double(30.0),
    MinN = cms.int32(1)
)


hlt1MET45 = cms.EDFilter("HLT1CaloMET",
    MaxEta = cms.double(-1.0),
    saveTag = cms.untracked.bool(True),
    inputTag = cms.InputTag("hltMet"),
    MinPt = cms.double(45.0),
    MinN = cms.int32(1)
)


hltMuBSoftIsoL3PreFiltered = cms.EDFilter("HLTMuonL3PreFilter",
    PreviousCandTag = cms.InputTag("hltMuBSoftIsoL2IsoFiltered"),
    MinPt = cms.double(7.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    MinNhits = cms.int32(0),
    NSigmaPt = cms.double(0.0),
    MaxDz = cms.double(9999.0),
    BeamSpotTag = cms.InputTag("hltOfflineBeamSpot"),
    CandTag = cms.InputTag("hltL3MuonCandidates"),
    MaxDr = cms.double(2.0)
)


hltL1NonIsoHLTLEITISinglePhotonEt25HcalIsolFilter = cms.EDFilter("HLTEgammaGenericFilter",
    doIsolated = cms.bool(False),
    thrOverE2EE = cms.double(-1.0),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    thrOverE2EB = cms.double(-1.0),
    thrRegularEE = cms.double(999999.9),
    thrOverEEE = cms.double(-1.0),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    thrOverEEB = cms.double(-1.0),
    thrRegularEB = cms.double(999999.9),
    lessThan = cms.bool(True),
    useEt = cms.bool(False),
    ncandcut = cms.int32(1),
    isoTag = cms.InputTag("hltL1IsolatedPhotonHcalIsol"),
    candTag = cms.InputTag("hltL1NonIsoHLTLEITISinglePhotonEt25EcalIsolFilter"),
    nonIsoTag = cms.InputTag("hltL1NonIsolatedPhotonHcalIsol")
)


hltSingleMuIsoL3PreFiltered3 = cms.EDFilter("HLTMuonL3PreFilter",
    PreviousCandTag = cms.InputTag("hltSingleMuIsoL2IsoFiltered3"),
    MinPt = cms.double(3.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    MinNhits = cms.int32(0),
    NSigmaPt = cms.double(0.0),
    MaxDz = cms.double(9999.0),
    BeamSpotTag = cms.InputTag("hltOfflineBeamSpot"),
    CandTag = cms.InputTag("hltL3MuonCandidates"),
    MaxDr = cms.double(2.0)
)


hltPreL1SingleForJet = cms.EDFilter("HLTPrescaler")


hltSingleMuIsoL1Filtered10 = cms.EDFilter("HLTMuonL1Filter",
    PreviousCandTag = cms.InputTag("hltL1sL1SingleMu10"),
    MinPt = cms.double(0.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    SelectQualities = cms.vint32(),
    CandTag = cms.InputTag("hltL1extraParticles")
)


hltMuBSoftIsoL2PreFiltered = cms.EDFilter("HLTMuonL2PreFilter",
    PreviousCandTag = cms.InputTag("hltMuBSoftL1Filtered"),
    MinPt = cms.double(5.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    MinNhits = cms.int32(0),
    NSigmaPt = cms.double(0.0),
    MaxDz = cms.double(9999.0),
    BeamSpotTag = cms.InputTag("hltOfflineBeamSpot"),
    CandTag = cms.InputTag("hltL2MuonCandidates"),
    MaxDr = cms.double(9999.0)
)


hltL1sCSCBeamHaloOverlapRing2 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleMuBeamHalo'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltL1sCSCBeamHaloOverlapRing1 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleMuBeamHalo'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltL1sHT200 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_HTT100'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltPreEle25SWL1R = cms.EDFilter("HLTPrescaler")


hltLogMonitorFilter = cms.EDFilter("HLTLogMonitorFilter",
    default_threshold = cms.uint32(10),
    categories = cms.VPSet()
)


hltPreLogMonitor = cms.EDFilter("HLTPrescaler")


hltL1NonIsoHLTNonIsoSinglePhotonEt20L1MatchFilterRegional = cms.EDFilter("HLTEgammaL1MatchFilterRegional",
    doIsolated = cms.bool(False),
    endcap_end = cms.double(2.65),
    candNonIsolatedTag = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    region_eta_size_ecap = cms.double(1.0),
    barrel_end = cms.double(1.4791),
    l1IsolatedTag = cms.InputTag("hltL1extraParticles","Isolated"),
    candIsolatedTag = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    region_phi_size = cms.double(1.044),
    region_eta_size = cms.double(0.522),
    L1SeedFilterTag = cms.InputTag("hltL1sL1SingleEG8"),
    ncandcut = cms.int32(1),
    l1NonIsolatedTag = cms.InputTag("hltL1extraParticles","NonIsolated")
)


hltPreL1DoubleEG5 = cms.EDFilter("HLTPrescaler")


hltL1sDoubleFwdJet50 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleJet30'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltL1NonIsoHLTNonIsoDoubleElectronEt5PixelMatchFilter = cms.EDFilter("HLTElectronPixelMatchFilter",
    doIsolated = cms.bool(False),
    L1IsoPixelSeedsTag = cms.InputTag("hltL1IsoStartUpElectronPixelSeeds"),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    L1NonIsoPixelSeedsTag = cms.InputTag("hltL1NonIsoStartUpElectronPixelSeeds"),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    SaveTag = cms.untracked.bool(True),
    npixelmatchcut = cms.double(1.0),
    ncandcut = cms.int32(2),
    candTag = cms.InputTag("hltL1NonIsoHLTNonIsoDoubleElectronEt5HcalIsolFilter")
)


hltPhysicsDeclared = cms.EDFilter("HLTPhysicsDeclared",
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    invert = cms.bool(False)
)


hltPreL1BPTX = cms.EDFilter("HLTPrescaler")


hltL1sJet110 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleJet70'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltPreDoubleMu3Psi2S = cms.EDFilter("HLTPrescaler")


hlt4jet15U = cms.EDFilter("HLT1CaloJet",
    MaxEta = cms.double(5.0),
    saveTag = cms.untracked.bool(True),
    inputTag = cms.InputTag("hltMCJetCorJetIcone5HF07"),
    MinPt = cms.double(15.0),
    MinN = cms.int32(4)
)


hltPreJet140 = cms.EDFilter("HLTPrescaler")


hltHIL1sJet35U = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleJet30'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltUpsilonMML3Filtered = cms.EDFilter("HLTMuonDimuonL3Filter",
    FastAccept = cms.bool(False),
    MinPtMax = cms.double(3.0),
    PreviousCandTag = cms.InputTag("hltUpsilonMML2Filtered"),
    ChargeOpt = cms.int32(0),
    MaxPtBalance = cms.double(999999.0),
    MaxDz = cms.double(9999.0),
    MaxAcop = cms.double(3.15),
    MinNhits = cms.int32(0),
    MinPtMin = cms.double(3.0),
    SaveTag = cms.untracked.bool(True),
    NSigmaPt = cms.double(0.0),
    MinPtBalance = cms.double(-1.0),
    MaxInvMass = cms.double(11.0),
    BeamSpotTag = cms.InputTag("hltOfflineBeamSpot"),
    MinPtPair = cms.double(0.0),
    CandTag = cms.InputTag("hltL3MuonCandidates"),
    MaxDr = cms.double(2.0),
    MinInvMass = cms.double(8.0),
    MaxEta = cms.double(2.5),
    MinAcop = cms.double(-1.0)
)


hltL1sGlobalRunHPDNoise = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleJet10U_NotBptxC'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltBJet50U = cms.EDFilter("HLT1CaloBJet",
    MaxEta = cms.double(3.0),
    saveTag = cms.untracked.bool(True),
    inputTag = cms.InputTag("hltMCJetCorJetIcone5HF07"),
    MinPt = cms.double(50.0),
    MinN = cms.int32(1)
)


hltPreDoubleMu3Vtx2mm = cms.EDFilter("HLTPrescaler")


hltBLifetimeL25filter = cms.EDFilter("HLTJetTag",
    JetTag = cms.InputTag("hltBLifetimeL25BJetTags"),
    MinTag = cms.double(3.5),
    MaxTag = cms.double(99999.0),
    SaveTag = cms.bool(False),
    MinJets = cms.int32(1)
)


hltPreL2Mu9 = cms.EDFilter("HLTPrescaler")


hltL1NonIsoHLTNonIsoDoubleElectronEt10L1MatchFilterRegional = cms.EDFilter("HLTEgammaL1MatchFilterRegional",
    doIsolated = cms.bool(False),
    endcap_end = cms.double(2.65),
    candNonIsolatedTag = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    region_eta_size_ecap = cms.double(1.0),
    barrel_end = cms.double(1.4791),
    l1IsolatedTag = cms.InputTag("hltL1extraParticles","Isolated"),
    candIsolatedTag = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    region_phi_size = cms.double(1.044),
    region_eta_size = cms.double(0.522),
    L1SeedFilterTag = cms.InputTag("hltL1sL1DoubleEG5"),
    ncandcut = cms.int32(2),
    l1NonIsolatedTag = cms.InputTag("hltL1extraParticles","NonIsolated")
)


hltBSoftmuonL3filter = cms.EDFilter("HLTJetTag",
    JetTag = cms.InputTag("hltBSoftmuonL3BJetTags"),
    MinTag = cms.double(0.7),
    MaxTag = cms.double(99999.0),
    SaveTag = cms.bool(True),
    MinJets = cms.int32(1)
)


hltPreTripleJet85 = cms.EDFilter("HLTPrescaler")


hltPreQuadJet15U = cms.EDFilter("HLTPrescaler")


hltL1NonIsoHLTNonIsoSingleElectronEt15EleIdEtFilterESet25LTI = cms.EDFilter("HLTEgammaEtFilter",
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    inputTag = cms.InputTag("hltL1NonIsoHLTNonIsoSingleElectronEt15EleIdPixelMatchFilter"),
    etcutEB = cms.double(25.0),
    etcutEE = cms.double(25.0),
    ncandcut = cms.int32(1)
)


hltPreZeroBias = cms.EDFilter("HLTPrescaler")


hltPreDoubleLooseIsoTau15 = cms.EDFilter("HLTPrescaler")


hltL1sL1BPTXPlusOnly = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_BptxPlus AND NOT L1_ZeroBias'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltPreHcalNZS8E29 = cms.EDFilter("HLTPrescaler")


hltL1NonIsoHLTNonIsoSingleElectronSiStripEt15L1MatchFilterRegional = cms.EDFilter("HLTEgammaL1MatchFilterRegional",
    doIsolated = cms.bool(False),
    endcap_end = cms.double(2.65),
    candNonIsolatedTag = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    region_eta_size_ecap = cms.double(1.0),
    barrel_end = cms.double(1.4791),
    l1IsolatedTag = cms.InputTag("hltL1extraParticles","Isolated"),
    candIsolatedTag = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    region_phi_size = cms.double(1.044),
    region_eta_size = cms.double(0.522),
    L1SeedFilterTag = cms.InputTag("hltL1sL1SingleEG8"),
    ncandcut = cms.int32(1),
    l1NonIsolatedTag = cms.InputTag("hltL1extraParticles","NonIsolated")
)


hltL1NonIsoDoubleElectronEt5JpsiPixelMatchFilter = cms.EDFilter("HLTElectronPixelMatchFilter",
    doIsolated = cms.bool(False),
    L1IsoPixelSeedsTag = cms.InputTag("hltL1IsoStartUpElectronPixelSeeds"),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    L1NonIsoPixelSeedsTag = cms.InputTag("hltL1NonIsoStartUpElectronPixelSeeds"),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    npixelmatchcut = cms.double(1.0),
    ncandcut = cms.int32(2),
    candTag = cms.InputTag("hltL1NonIsoDoubleElectronEt5JpsiHcalIsolFilter")
)


hltPreL1Mu14L1SingleJet6U = cms.EDFilter("HLTPrescaler")


hltFilterL25PixelTracksLeadingTrackPtCutMuonTau = cms.EDFilter("HLT1Tau",
    MaxEta = cms.double(5.0),
    inputTag = cms.InputTag("hltL25TauPixelTracksLeadingTrackPtCutSelector"),
    MinPt = cms.double(15.0),
    MinN = cms.int32(1)
)


hltPreL1BPTXMinusOnly = cms.EDFilter("HLTPrescaler")


hlt1jet100 = cms.EDFilter("HLT1CaloJet",
    MaxEta = cms.double(5.0),
    saveTag = cms.untracked.bool(True),
    inputTag = cms.InputTag("hltMCJetCorJetIcone5"),
    MinPt = cms.double(100.0),
    MinN = cms.int32(1)
)


hltIsolPixelTrackL2FilterHB1E31 = cms.EDFilter("HLTPixelIsolTrackFilter",
    MaxPtNearby = cms.double(2.0),
    MinEtaTrack = cms.double(0.0),
    MinEnergyTrack = cms.double(8.0),
    MinPtTrack = cms.double(3.5),
    DropMultiL2Event = cms.bool(False),
    NMaxTrackCandidates = cms.int32(10),
    MaxEtaTrack = cms.double(1.45),
    candTag = cms.InputTag("hltIsolPixelTrackProdHB1E31"),
    filterTrackEnergy = cms.bool(True)
)


hltMuonTauL1FilteredNoL1Tau = cms.EDFilter("HLTMuonL1Filter",
    PreviousCandTag = cms.InputTag("hltL1sMuon5Jet20"),
    MinPt = cms.double(0.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    SelectQualities = cms.vint32(),
    CandTag = cms.InputTag("hltL1extraParticles")
)


hltSingleMu5L2Filtered4 = cms.EDFilter("HLTMuonL2PreFilter",
    PreviousCandTag = cms.InputTag("hltL1SingleMu3L1Filtered0"),
    MinPt = cms.double(4.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    MinNhits = cms.int32(0),
    NSigmaPt = cms.double(0.0),
    MaxDz = cms.double(9999.0),
    BeamSpotTag = cms.InputTag("hltOfflineBeamSpot"),
    CandTag = cms.InputTag("hltL2MuonCandidates"),
    MaxDr = cms.double(9999.0)
)


hltL1sJet80 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleJet50'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltSingleMuIsoL2IsoFiltered11 = cms.EDFilter("HLTMuonIsoFilter",
    IsolatorPSet = cms.PSet(

    ),
    DepTag = cms.VInputTag("hltL2MuonIsolations"),
    PreviousCandTag = cms.InputTag("hltSingleMuIsoL2PreFiltered11"),
    MinN = cms.int32(1),
    CandTag = cms.InputTag("hltL2MuonCandidates")
)


hltSingleMuIsoL2IsoFiltered12 = cms.EDFilter("HLTMuonIsoFilter",
    IsolatorPSet = cms.PSet(

    ),
    DepTag = cms.VInputTag("hltL2MuonIsolations"),
    PreviousCandTag = cms.InputTag("hltSingleMuIsoL2PreFiltered12"),
    MinN = cms.int32(1),
    CandTag = cms.InputTag("hltL2MuonCandidates")
)


hltSingleMu3L2Filtered3 = cms.EDFilter("HLTMuonL2PreFilter",
    PreviousCandTag = cms.InputTag("hltL1SingleMu0L1Filtered0"),
    MinPt = cms.double(3.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    MinNhits = cms.int32(0),
    NSigmaPt = cms.double(0.0),
    MaxDz = cms.double(9999.0),
    BeamSpotTag = cms.InputTag("hltOfflineBeamSpot"),
    CandTag = cms.InputTag("hltL2MuonCandidates"),
    MaxDr = cms.double(9999.0)
)


hltL1NonIsoHLTNonIsoSingleElectronEt15EleIdESDoubleSC15 = cms.EDFilter("HLTEgammaEtFilter",
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    inputTag = cms.InputTag("hltL1NonIsoHLTNonIsoSingleElectronEt15LTIESscWrapper"),
    SaveTag = cms.untracked.bool(True),
    etcutEB = cms.double(15.0),
    ncandcut = cms.int32(2),
    etcutEE = cms.double(15.0)
)


hltMuJetsL2IsoFiltered = cms.EDFilter("HLTMuonIsoFilter",
    IsolatorPSet = cms.PSet(

    ),
    DepTag = cms.VInputTag("hltL2MuonIsolations"),
    PreviousCandTag = cms.InputTag("hltMuJetsL2PreFiltered"),
    MinN = cms.int32(1),
    CandTag = cms.InputTag("hltL2MuonCandidates")
)


hltL2Mu11L2Filtered11 = cms.EDFilter("HLTMuonL2PreFilter",
    PreviousCandTag = cms.InputTag("hltL1SingleMu7L1Filtered0"),
    MinPt = cms.double(11.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    MinNhits = cms.int32(0),
    SaveTag = cms.untracked.bool(True),
    NSigmaPt = cms.double(0.0),
    MaxDz = cms.double(9999.0),
    BeamSpotTag = cms.InputTag("hltOfflineBeamSpot"),
    CandTag = cms.InputTag("hltL2MuonCandidates"),
    MaxDr = cms.double(9999.0)
)


hltPreIsoMu14LooseIsoTauNoL25 = cms.EDFilter("HLTPrescaler")


hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdDphiFilter = cms.EDFilter("HLTElectronGenericFilter",
    doIsolated = cms.bool(False),
    nonIsoTag = cms.InputTag("hltElectronL1NonIsoLargeWindowDetaDphi","Dphi"),
    L1NonIsoCand = cms.InputTag("hltPixelMatchLargeWindowElectronsL1NonIso"),
    thrRegularEE = cms.double(0.1),
    L1IsoCand = cms.InputTag("hltPixelMatchLargeWindowElectronsL1Iso"),
    SaveTag = cms.untracked.bool(True),
    thrRegularEB = cms.double(0.1),
    lessThan = cms.bool(True),
    ncandcut = cms.int32(1),
    isoTag = cms.InputTag("hltElectronL1IsoLargeWindowDetaDphi","Dphi"),
    thrOverPtEE = cms.double(-1.0),
    candTag = cms.InputTag("hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdDetaFilter"),
    thrTimesPtEE = cms.double(-1.0),
    thrTimesPtEB = cms.double(-1.0),
    thrOverPtEB = cms.double(-1.0)
)


hltEle10LWL1RHT200EleHcalIsolFilter = cms.EDFilter("HLTEgammaGenericFilter",
    doIsolated = cms.bool(False),
    thrOverE2EE = cms.double(-1.0),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    thrOverE2EB = cms.double(-1.0),
    thrRegularEE = cms.double(999999.9),
    thrOverEEE = cms.double(-1.0),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    thrOverEEB = cms.double(-1.0),
    thrRegularEB = cms.double(999999.9),
    lessThan = cms.bool(True),
    useEt = cms.bool(False),
    ncandcut = cms.int32(1),
    isoTag = cms.InputTag("hltL1IsolatedElectronHcalIsol"),
    candTag = cms.InputTag("hltEle10LWL1RHT200EleEtFilter"),
    nonIsoTag = cms.InputTag("hltL1NonIsolatedElectronHcalIsol")
)


hltSingleL2Mu0L2PreFilteredNoVtx = cms.EDFilter("HLTMuonL2PreFilter",
    PreviousCandTag = cms.InputTag("hltSingleMu0L1Filtered"),
    MinPt = cms.double(0.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    MinNhits = cms.int32(0),
    SaveTag = cms.untracked.bool(True),
    NSigmaPt = cms.double(0.0),
    MaxDz = cms.double(9999.0),
    BeamSpotTag = cms.InputTag("hltOfflineBeamSpot"),
    CandTag = cms.InputTag("hltL2MuonCandidatesNoVtx"),
    MaxDr = cms.double(9999.0)
)


hltL1sDiJetAve50U = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleJet50'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hlt1jet30 = cms.EDFilter("HLT1CaloJet",
    MaxEta = cms.double(5.0),
    saveTag = cms.untracked.bool(True),
    inputTag = cms.InputTag("hltMCJetCorJetIcone5"),
    MinPt = cms.double(30.0),
    MinN = cms.int32(1)
)


hltL1NonIsoHLTNonIsoSinglePhotonEt15EtFilterESet308E29 = cms.EDFilter("HLTEgammaEtFilter",
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    inputTag = cms.InputTag("hltL1NonIsoHLTNonIsoSinglePhotonEt15EtFilter"),
    SaveTag = cms.untracked.bool(True),
    etcutEB = cms.double(30.0),
    ncandcut = cms.int32(1),
    etcutEE = cms.double(30.0)
)


hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdL1MatchFilterRegional = cms.EDFilter("HLTEgammaL1MatchFilterRegional",
    doIsolated = cms.bool(False),
    endcap_end = cms.double(2.65),
    candNonIsolatedTag = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    region_eta_size_ecap = cms.double(1.0),
    barrel_end = cms.double(1.4791),
    l1IsolatedTag = cms.InputTag("hltL1extraParticles","Isolated"),
    candIsolatedTag = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    region_phi_size = cms.double(1.044),
    region_eta_size = cms.double(0.522),
    L1SeedFilterTag = cms.InputTag("hltL1sL1SingleEG5"),
    ncandcut = cms.int32(1),
    l1NonIsolatedTag = cms.InputTag("hltL1extraParticles","NonIsolated")
)


hltPreZeroBias1kHz = cms.EDFilter("HLTPrescaler")


hltL1NonIsoHLTNonIsoSinglePhotonEt30EtFilterESet70 = cms.EDFilter("HLTEgammaEtFilter",
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    inputTag = cms.InputTag("hltL1NonIsoHLTNonIsoSinglePhotonEt30EtFilter"),
    SaveTag = cms.untracked.bool(True),
    etcutEB = cms.double(70.0),
    ncandcut = cms.int32(1),
    etcutEE = cms.double(70.0)
)


hltPreBTagIPJet50U = cms.EDFilter("HLTPrescaler")


hltPreEle20SiStripL1R = cms.EDFilter("HLTPrescaler")


hltEle10SWL1RTripleJet30L1MatchFilterRegional = cms.EDFilter("HLTEgammaL1MatchFilterRegional",
    doIsolated = cms.bool(False),
    endcap_end = cms.double(2.65),
    candNonIsolatedTag = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    region_eta_size_ecap = cms.double(1.0),
    barrel_end = cms.double(1.4791),
    l1IsolatedTag = cms.InputTag("hltL1extraParticles","Isolated"),
    candIsolatedTag = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    region_phi_size = cms.double(1.044),
    region_eta_size = cms.double(0.522),
    L1SeedFilterTag = cms.InputTag("hltL1sEG5TripleJet15"),
    ncandcut = cms.int32(1),
    l1NonIsolatedTag = cms.InputTag("hltL1extraParticles","NonIsolated")
)


hltL1sBTagMuJet20 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_Mu3_Jet15'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltIsolPixelTrackL2FilterHB8E29 = cms.EDFilter("HLTPixelIsolTrackFilter",
    MaxPtNearby = cms.double(2.0),
    MinEtaTrack = cms.double(0.0),
    MinEnergyTrack = cms.double(8.0),
    MinPtTrack = cms.double(3.5),
    DropMultiL2Event = cms.bool(False),
    NMaxTrackCandidates = cms.int32(10),
    MaxEtaTrack = cms.double(1.45),
    candTag = cms.InputTag("hltIsolPixelTrackProdHB8E29"),
    filterTrackEnergy = cms.bool(True)
)


hltBSoftMuonL25FilterByDR = cms.EDFilter("HLTJetTag",
    JetTag = cms.InputTag("hltBSoftMuonL25BJetTagsByDR"),
    MinTag = cms.double(0.5),
    MaxTag = cms.double(99999.0),
    SaveTag = cms.bool(False),
    MinJets = cms.int32(1)
)


hltHcalCalibTypeFilter = cms.EDFilter("HLTHcalCalibTypeFilter",
    InputTag = cms.InputTag("source"),
    CalibTypes = cms.vint32(1, 2, 3, 4, 5, 
        6)
)


hltL1NonIsoHLTIsoSingleElectronEt20LTIEleIdDetaFilter = cms.EDFilter("HLTElectronGenericFilter",
    doIsolated = cms.bool(False),
    nonIsoTag = cms.InputTag("hltElectronL1NonIsoDetaDphi","Deta"),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    thrRegularEE = cms.double(0.011),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    thrRegularEB = cms.double(0.008),
    lessThan = cms.bool(True),
    ncandcut = cms.int32(1),
    isoTag = cms.InputTag("hltElectronL1IsoDetaDphi","Deta"),
    thrOverPtEE = cms.double(-1.0),
    candTag = cms.InputTag("hltL1NonIsoHLTIsoSingleElectronEt20LTIEleIdOneOEMinusOneOPFilter"),
    thrTimesPtEE = cms.double(-1.0),
    thrTimesPtEB = cms.double(-1.0),
    thrOverPtEB = cms.double(-1.0)
)


hltL1sL1SingleMuOpenL1SingleMu0 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleMuOpen OR L1_SingleMu0'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltPreEle15SiStripL1R = cms.EDFilter("HLTPrescaler")


hltEgammaSCOnlyEtFilter = cms.EDFilter("HLTEgammaEtFilter",
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    inputTag = cms.InputTag("hltEgammaSCOnlyL1MatchFilterRegional"),
    etcutEB = cms.double(5.0),
    etcutEE = cms.double(5.0),
    ncandcut = cms.int32(1)
)


hltHIMML1Seed = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_DoubleMuOpen'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltCaloTowerFilter = cms.EDFilter("HLTCaloTowerFilter",
    MaxEta = cms.double(3.0),
    inputTag = cms.InputTag("hltTowerMakerForEcal"),
    MinPt = cms.double(2.0),
    MinN = cms.uint32(1)
)


hltPreMinBiasEcal = cms.EDFilter("HLTPrescaler")


hltBLifetimeL25FilterStartupU = cms.EDFilter("HLTJetTag",
    JetTag = cms.InputTag("hltBLifetimeL25BJetTagsStartupU"),
    MinTag = cms.double(2.5),
    MaxTag = cms.double(99999.0),
    SaveTag = cms.bool(False),
    MinJets = cms.int32(1)
)


hltPreRandom = cms.EDFilter("HLTPrescaler")


hltL1sCSCBeamHaloRing2or3 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleMuBeamHalo'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltPreMET45 = cms.EDFilter("HLTPrescaler")


hlt1MET75 = cms.EDFilter("HLT1CaloMET",
    MaxEta = cms.double(-1.0),
    saveTag = cms.untracked.bool(True),
    inputTag = cms.InputTag("hltMet"),
    MinPt = cms.double(75.0),
    MinN = cms.int32(1)
)


hltMuJetsL3PreFiltered = cms.EDFilter("HLTMuonL3PreFilter",
    PreviousCandTag = cms.InputTag("hltMuJetsL2IsoFiltered"),
    MinPt = cms.double(7.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    MinNhits = cms.int32(0),
    NSigmaPt = cms.double(0.0),
    MaxDz = cms.double(9999.0),
    BeamSpotTag = cms.InputTag("hltOfflineBeamSpot"),
    CandTag = cms.InputTag("hltL3MuonCandidates"),
    MaxDr = cms.double(2.0)
)


hlt1MET70 = cms.EDFilter("HLT1CaloMET",
    MaxEta = cms.double(-1.0),
    saveTag = cms.untracked.bool(True),
    inputTag = cms.InputTag("hltMet"),
    MinPt = cms.double(70.0),
    MinN = cms.int32(1)
)


hltPreDiJetAve15U1E31 = cms.EDFilter("HLTPrescaler")


hltL1NonIsoDoubleElectronEt5JpsiClusterShapeFilter = cms.EDFilter("HLTEgammaGenericFilter",
    doIsolated = cms.bool(False),
    thrOverE2EE = cms.double(-1.0),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    thrOverE2EB = cms.double(-1.0),
    thrRegularEE = cms.double(0.042),
    thrOverEEE = cms.double(-1.0),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    thrOverEEB = cms.double(-1.0),
    thrRegularEB = cms.double(0.016),
    lessThan = cms.bool(True),
    useEt = cms.bool(False),
    ncandcut = cms.int32(2),
    isoTag = cms.InputTag("hltL1IsoHLTClusterShape"),
    candTag = cms.InputTag("hltL1NonIsoDoubleElectronEt5JpsiEtFilter"),
    nonIsoTag = cms.InputTag("hltL1NonIsoHLTClusterShape")
)


hlt1MET25 = cms.EDFilter("HLT1CaloMET",
    MaxEta = cms.double(-1.0),
    saveTag = cms.untracked.bool(True),
    inputTag = cms.InputTag("hltMet"),
    MinPt = cms.double(25.0),
    MinN = cms.int32(1)
)


hltMhtHtFilter = cms.EDFilter("HLTMhtHtFilter",
    inputJetTag = cms.InputTag("hltMCJetCorJetIcone5"),
    minMht = cms.double(100.0),
    minPtJet = cms.double(30.0)
)


hltMuBLifetimeIsoL2IsoFiltered = cms.EDFilter("HLTMuonIsoFilter",
    IsolatorPSet = cms.PSet(

    ),
    DepTag = cms.VInputTag("hltL2MuonIsolations"),
    PreviousCandTag = cms.InputTag("hltMuBLifetimeIsoL2PreFiltered"),
    MinN = cms.int32(1),
    CandTag = cms.InputTag("hltL2MuonCandidates")
)


hltPreEle15SWLTIL1R = cms.EDFilter("HLTPrescaler")


hltL1NonIsoDoublePhotonEt5JpsiL1MatchFilterRegional = cms.EDFilter("HLTEgammaL1MatchFilterRegional",
    doIsolated = cms.bool(False),
    endcap_end = cms.double(2.65),
    candNonIsolatedTag = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    region_eta_size_ecap = cms.double(1.0),
    barrel_end = cms.double(1.4791),
    l1IsolatedTag = cms.InputTag("hltL1extraParticles","Isolated"),
    candIsolatedTag = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    region_phi_size = cms.double(1.044),
    region_eta_size = cms.double(0.522),
    L1SeedFilterTag = cms.InputTag("hltL1sL1SingleEG8orL1DoubleEG5"),
    ncandcut = cms.int32(2),
    l1NonIsolatedTag = cms.InputTag("hltL1extraParticles","NonIsolated")
)


hltMuNoL2IsoJetsL3IsoFiltered = cms.EDFilter("HLTMuonIsoFilter",
    PreviousCandTag = cms.InputTag("hltMuNoL2IsoJetsL3PreFiltered"),
    MinN = cms.int32(1),
    SaveTag = cms.untracked.bool(True),
    CandTag = cms.InputTag("hltL3MuonCandidates"),
    IsolatorPSet = cms.PSet(

    ),
    DepTag = cms.VInputTag("hltL2MuonIsolations")
)


hltIsolPixelTrackL3FilterHE8E29 = cms.EDFilter("HLTPixelIsolTrackFilter",
    MaxPtNearby = cms.double(2.0),
    MinEtaTrack = cms.double(1.4),
    MinEnergyTrack = cms.double(20.0),
    MinPtTrack = cms.double(20.0),
    DropMultiL2Event = cms.bool(False),
    NMaxTrackCandidates = cms.int32(15),
    MaxEtaTrack = cms.double(2.0),
    candTag = cms.InputTag("hltHITIPTCorrectorHE8E29"),
    filterTrackEnergy = cms.bool(True)
)


hlt4jet60 = cms.EDFilter("HLT1CaloJet",
    MaxEta = cms.double(5.0),
    saveTag = cms.untracked.bool(True),
    inputTag = cms.InputTag("hltMCJetCorJetIcone5"),
    MinPt = cms.double(60.0),
    MinN = cms.int32(4)
)


hltSameSignMuL2PreFiltered = cms.EDFilter("HLTMuonDimuonL2Filter",
    FastAccept = cms.bool(False),
    MinPtMax = cms.double(3.0),
    PreviousCandTag = cms.InputTag("hltSameSignMuL1Filtered"),
    ChargeOpt = cms.int32(1),
    MaxPtBalance = cms.double(999999.0),
    MaxDz = cms.double(9999.0),
    MaxAcop = cms.double(3.15),
    MinNhits = cms.int32(0),
    MinPtMin = cms.double(3.0),
    NSigmaPt = cms.double(0.0),
    MinPtBalance = cms.double(-1.0),
    MaxInvMass = cms.double(9999.0),
    BeamSpotTag = cms.InputTag("hltOfflineBeamSpot"),
    MinPtPair = cms.double(0.0),
    CandTag = cms.InputTag("hltL2MuonCandidates"),
    MaxDr = cms.double(100.0),
    MinInvMass = cms.double(0.0),
    MaxEta = cms.double(2.5),
    MinAcop = cms.double(-1.0)
)


hltL1sL1SingleEG8 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleEG8'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltL1NonIsoDoublePhotonEt5eeResL1MatchFilterRegional = cms.EDFilter("HLTEgammaL1MatchFilterRegional",
    doIsolated = cms.bool(False),
    endcap_end = cms.double(2.65),
    candNonIsolatedTag = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    region_eta_size_ecap = cms.double(1.0),
    barrel_end = cms.double(1.4791),
    l1IsolatedTag = cms.InputTag("hltL1extraParticles","Isolated"),
    candIsolatedTag = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    region_phi_size = cms.double(1.044),
    region_eta_size = cms.double(0.522),
    L1SeedFilterTag = cms.InputTag("hltL1sL1SingleEG8orL1DoubleEG5"),
    ncandcut = cms.int32(2),
    l1NonIsolatedTag = cms.InputTag("hltL1extraParticles","NonIsolated")
)


hltL1sL1MET20 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_ETM20'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltBJet80 = cms.EDFilter("HLT1CaloBJet",
    MaxEta = cms.double(3.0),
    saveTag = cms.untracked.bool(True),
    inputTag = cms.InputTag("hltMCJetCorJetIcone5Regional"),
    MinPt = cms.double(80.0),
    MinN = cms.int32(1)
)


hltSingleMuIsoL2PreFiltered11 = cms.EDFilter("HLTMuonL2PreFilter",
    PreviousCandTag = cms.InputTag("hltSingleMuIsoL1Filtered10"),
    MinPt = cms.double(11.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    MinNhits = cms.int32(0),
    NSigmaPt = cms.double(0.0),
    MaxDz = cms.double(9999.0),
    BeamSpotTag = cms.InputTag("hltOfflineBeamSpot"),
    CandTag = cms.InputTag("hltL2MuonCandidates"),
    MaxDr = cms.double(9999.0)
)


hltL1NonIsoHLTNonIsoSingleElectronLWEt10EtFilter = cms.EDFilter("HLTEgammaEtFilter",
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    inputTag = cms.InputTag("hltL1NonIsoHLTNonIsoSingleElectronLWEt10L1MatchFilterRegional"),
    etcutEB = cms.double(10.0),
    etcutEE = cms.double(10.0),
    ncandcut = cms.int32(1)
)


hltMuonTauIsoL2PreFiltered = cms.EDFilter("HLTMuonL2PreFilter",
    PreviousCandTag = cms.InputTag("hltMuonTauL1Filtered"),
    MinPt = cms.double(12.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    MinNhits = cms.int32(0),
    NSigmaPt = cms.double(0.0),
    MaxDz = cms.double(9999.0),
    BeamSpotTag = cms.InputTag("hltOfflineBeamSpot"),
    CandTag = cms.InputTag("hltL2MuonCandidates"),
    MaxDr = cms.double(9999.0)
)


hltL1NonIsoHLTNonIsoSingleElectronLWEt10HcalIsolFilter = cms.EDFilter("HLTEgammaGenericFilter",
    doIsolated = cms.bool(False),
    thrOverE2EE = cms.double(-1.0),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    thrOverE2EB = cms.double(-1.0),
    thrRegularEE = cms.double(999999.9),
    thrOverEEE = cms.double(-1.0),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    thrOverEEB = cms.double(-1.0),
    thrRegularEB = cms.double(999999.9),
    lessThan = cms.bool(True),
    useEt = cms.bool(False),
    ncandcut = cms.int32(1),
    isoTag = cms.InputTag("hltL1IsolatedElectronHcalIsol"),
    candTag = cms.InputTag("hltL1NonIsoHLTNonIsoSingleElectronLWEt10EtFilter"),
    nonIsoTag = cms.InputTag("hltL1NonIsolatedElectronHcalIsol")
)


hltL1sEG5TripleJet15 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_EG5_TripleJet15'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltL1NonIsoHLTNonIsoSingleElectronEt15EleIdDphiFilter = cms.EDFilter("HLTElectronGenericFilter",
    doIsolated = cms.bool(False),
    nonIsoTag = cms.InputTag("hltElectronL1NonIsoDetaDphi","Dphi"),
    L1NonIsoCand = cms.InputTag("hltPixelMatchElectronsL1NonIso"),
    thrRegularEE = cms.double(0.1),
    L1IsoCand = cms.InputTag("hltPixelMatchElectronsL1Iso"),
    SaveTag = cms.untracked.bool(True),
    thrRegularEB = cms.double(0.1),
    lessThan = cms.bool(True),
    ncandcut = cms.int32(1),
    isoTag = cms.InputTag("hltElectronL1IsoDetaDphi","Dphi"),
    thrOverPtEE = cms.double(-1.0),
    candTag = cms.InputTag("hltL1NonIsoHLTNonIsoSingleElectronEt15EleIdDetaFilter"),
    thrTimesPtEE = cms.double(-1.0),
    thrTimesPtEB = cms.double(-1.0),
    thrOverPtEB = cms.double(-1.0)
)


hltL2Filtered3Mu8HT50 = cms.EDFilter("HLTMuonL2PreFilter",
    PreviousCandTag = cms.InputTag("hltL1Mu3Jet15L1Filtered0"),
    MinPt = cms.double(8.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    MinNhits = cms.int32(0),
    SaveTag = cms.untracked.bool(True),
    NSigmaPt = cms.double(0.0),
    MaxDz = cms.double(9999.0),
    BeamSpotTag = cms.InputTag("hltOfflineBeamSpot"),
    CandTag = cms.InputTag("hltL2MuonCandidates"),
    MaxDr = cms.double(9999.0)
)


hltPrePhoton70L1R = cms.EDFilter("HLTPrescaler")


hltPreEle20SC15SWL1R = cms.EDFilter("HLTPrescaler")


hltL1NonIsoHLTIsoSingleElectronEt20LTIEleIdDphiFilter = cms.EDFilter("HLTElectronGenericFilter",
    doIsolated = cms.bool(False),
    nonIsoTag = cms.InputTag("hltElectronL1NonIsoDetaDphi","Dphi"),
    L1NonIsoCand = cms.InputTag("hltPixelMatchElectronsL1NonIso"),
    thrRegularEE = cms.double(0.1),
    L1IsoCand = cms.InputTag("hltPixelMatchElectronsL1Iso"),
    thrRegularEB = cms.double(0.1),
    lessThan = cms.bool(True),
    ncandcut = cms.int32(1),
    isoTag = cms.InputTag("hltElectronL1IsoDetaDphi","Dphi"),
    thrOverPtEE = cms.double(-1.0),
    candTag = cms.InputTag("hltL1NonIsoHLTIsoSingleElectronEt20LTIEleIdDetaFilter"),
    thrTimesPtEE = cms.double(-1.0),
    thrTimesPtEB = cms.double(-1.0),
    thrOverPtEB = cms.double(-1.0)
)


hltPreActivityCaloTower = cms.EDFilter("HLTPrescaler")


hltPreL1MuOpen = cms.EDFilter("HLTPrescaler")


hltL1NonIsoDoubleElectronEt5JpsiEtFilter = cms.EDFilter("HLTEgammaEtFilter",
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    inputTag = cms.InputTag("hltL1NonIsoDoubleElectronEt5JpsiL1MatchFilterRegional"),
    etcutEB = cms.double(4.0),
    etcutEE = cms.double(4.0),
    ncandcut = cms.int32(2)
)


hltPreIsoMu7BTagIPJet35 = cms.EDFilter("HLTPrescaler")


hltL1NonIsoHLTNonIsoDoubleElectronEt10PixelMatchFilter = cms.EDFilter("HLTElectronPixelMatchFilter",
    doIsolated = cms.bool(False),
    L1IsoPixelSeedsTag = cms.InputTag("hltL1IsoStartUpElectronPixelSeeds"),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    L1NonIsoPixelSeedsTag = cms.InputTag("hltL1NonIsoStartUpElectronPixelSeeds"),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    SaveTag = cms.untracked.bool(True),
    npixelmatchcut = cms.double(1.0),
    ncandcut = cms.int32(2),
    candTag = cms.InputTag("hltL1NonIsoHLTNonIsoDoubleElectronEt10HcalIsolFilter")
)


hltL1NonIsoHLTIsoSingleElectronEt20LTIEleIdTrackIsolFilter = cms.EDFilter("HLTElectronGenericFilter",
    doIsolated = cms.bool(False),
    nonIsoTag = cms.InputTag("hltL1NonIsoElectronTrackIsol"),
    L1NonIsoCand = cms.InputTag("hltPixelMatchElectronsL1NonIso"),
    thrRegularEE = cms.double(0.5),
    L1IsoCand = cms.InputTag("hltPixelMatchElectronsL1Iso"),
    SaveTag = cms.untracked.bool(True),
    thrRegularEB = cms.double(0.5),
    lessThan = cms.bool(True),
    ncandcut = cms.int32(1),
    isoTag = cms.InputTag("hltL1IsoElectronTrackIsol"),
    thrOverPtEE = cms.double(-1.0),
    candTag = cms.InputTag("hltL1NonIsoHLTIsoSingleElectronEt20LTIEleIdDphiFilter"),
    thrTimesPtEE = cms.double(8.0),
    thrTimesPtEB = cms.double(8.0),
    thrOverPtEB = cms.double(-1.0)
)


hltL1sDoubleJet125Aco = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleJet150 OR L1_DoubleJet70'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltL1NonIsoHLTNonIsoSingleElectronEt10PixelMatchFilter = cms.EDFilter("HLTElectronPixelMatchFilter",
    doIsolated = cms.bool(False),
    L1IsoPixelSeedsTag = cms.InputTag("hltL1IsoStartUpElectronPixelSeeds"),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    L1NonIsoPixelSeedsTag = cms.InputTag("hltL1NonIsoStartUpElectronPixelSeeds"),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    SaveTag = cms.untracked.bool(True),
    npixelmatchcut = cms.double(1.0),
    ncandcut = cms.int32(1),
    candTag = cms.InputTag("hltL1NonIsoHLTNonIsoSingleElectronEt10HcalIsolFilter")
)


hltPreBTagMuJet10U = cms.EDFilter("HLTPrescaler")


hltL1sMET75 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_ETM50'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltPreDoubleMu3SameSign = cms.EDFilter("HLTPrescaler")


hltL1NonIsoHLTNonIsoSingleElectronSiStripEt15HcalIsolFilter = cms.EDFilter("HLTEgammaGenericFilter",
    doIsolated = cms.bool(False),
    thrOverE2EE = cms.double(-1.0),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    thrOverE2EB = cms.double(-1.0),
    thrRegularEE = cms.double(999999.9),
    thrOverEEE = cms.double(-1.0),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    thrOverEEB = cms.double(-1.0),
    thrRegularEB = cms.double(999999.9),
    lessThan = cms.bool(True),
    useEt = cms.bool(False),
    ncandcut = cms.int32(1),
    isoTag = cms.InputTag("hltL1IsolatedElectronHcalIsol"),
    candTag = cms.InputTag("hltL1NonIsoHLTNonIsoSingleElectronSiStripEt15EtFilter"),
    nonIsoTag = cms.InputTag("hltL1NonIsolatedElectronHcalIsol")
)


hltL1NonIsoHLTNonIsoSinglePhotonEt15HcalIsolFilter = cms.EDFilter("HLTEgammaGenericFilter",
    doIsolated = cms.bool(False),
    thrOverE2EE = cms.double(-1.0),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    thrOverE2EB = cms.double(-1.0),
    thrRegularEE = cms.double(999999.9),
    thrOverEEE = cms.double(-1.0),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    SaveTag = cms.untracked.bool(True),
    thrRegularEB = cms.double(999999.9),
    lessThan = cms.bool(True),
    useEt = cms.bool(False),
    ncandcut = cms.int32(1),
    thrOverEEB = cms.double(-1.0),
    isoTag = cms.InputTag("hltL1IsolatedPhotonHcalIsol"),
    candTag = cms.InputTag("hltL1NonIsoHLTNonIsoSinglePhotonEt15EtFilter"),
    nonIsoTag = cms.InputTag("hltL1NonIsolatedPhotonHcalIsol")
)


hltDiMuonL2PreFiltered0 = cms.EDFilter("HLTMuonL2PreFilter",
    PreviousCandTag = cms.InputTag("hltDiMuonL1Filtered0"),
    MinPt = cms.double(0.0),
    MinN = cms.int32(2),
    MaxEta = cms.double(2.5),
    MinNhits = cms.int32(0),
    NSigmaPt = cms.double(0.0),
    MaxDz = cms.double(9999.0),
    BeamSpotTag = cms.InputTag("hltOfflineBeamSpot"),
    CandTag = cms.InputTag("hltL2MuonCandidates"),
    MaxDr = cms.double(9999.0)
)


hltPreIsoMu13 = cms.EDFilter("HLTPrescaler")


hltL2Mu7Photon9HcalIsolFilter = cms.EDFilter("HLTEgammaGenericFilter",
    doIsolated = cms.bool(False),
    thrOverE2EE = cms.double(-1.0),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    thrOverE2EB = cms.double(-1.0),
    thrRegularEE = cms.double(999999.9),
    thrOverEEE = cms.double(-1.0),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    SaveTag = cms.untracked.bool(True),
    thrRegularEB = cms.double(999999.9),
    lessThan = cms.bool(True),
    useEt = cms.bool(False),
    ncandcut = cms.int32(1),
    thrOverEEB = cms.double(-1.0),
    isoTag = cms.InputTag("hltL1IsolatedPhotonHcalIsol"),
    candTag = cms.InputTag("hltL2Mu7Photon9EtFilter"),
    nonIsoTag = cms.InputTag("hltL1NonIsolatedPhotonHcalIsol")
)


hltHI1jet35U = cms.EDFilter("HLT1CaloJet",
    MaxEta = cms.double(5.0),
    saveTag = cms.untracked.bool(True),
    inputTag = cms.InputTag("hltIterativeCone5PileupSubtractionCaloJets"),
    MinPt = cms.double(35.0),
    MinN = cms.int32(1)
)


hltHIL1sJet50U = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleJet15'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltL1NonIsoHLTNonIsoSingleElectronEt15EtFilterESet25 = cms.EDFilter("HLTEgammaEtFilter",
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    inputTag = cms.InputTag("hltL1NonIsoHLTNonIsoSingleElectronEt15PixelMatchFilter"),
    SaveTag = cms.untracked.bool(True),
    etcutEB = cms.double(25.0),
    ncandcut = cms.int32(1),
    etcutEE = cms.double(25.0)
)


hltL1NonIsoHLTNonIsoSingleElectronEt15EleIdEtFilter = cms.EDFilter("HLTEgammaEtFilter",
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    inputTag = cms.InputTag("hltL1NonIsoHLTNonIsoSingleElectronEt15EleIdL1MatchFilterRegional"),
    etcutEB = cms.double(15.0),
    etcutEE = cms.double(15.0),
    ncandcut = cms.int32(1)
)


hltPreHFThreshold3 = cms.EDFilter("HLTPrescaler")


hltZMML3Filtered = cms.EDFilter("HLTMuonDimuonL3Filter",
    FastAccept = cms.bool(False),
    MinPtMax = cms.double(7.0),
    PreviousCandTag = cms.InputTag("hltZMML2Filtered"),
    ChargeOpt = cms.int32(0),
    MaxPtBalance = cms.double(999999.0),
    MaxDz = cms.double(9999.0),
    MaxAcop = cms.double(3.15),
    MinNhits = cms.int32(0),
    MinPtMin = cms.double(7.0),
    SaveTag = cms.untracked.bool(True),
    NSigmaPt = cms.double(0.0),
    MinPtBalance = cms.double(-1.0),
    MaxInvMass = cms.double(1e+30),
    BeamSpotTag = cms.InputTag("hltOfflineBeamSpot"),
    MinPtPair = cms.double(0.0),
    CandTag = cms.InputTag("hltL3MuonCandidates"),
    MaxDr = cms.double(2.0),
    MinInvMass = cms.double(70.0),
    MaxEta = cms.double(2.5),
    MinAcop = cms.double(-1.0)
)


hltL1NonIsoHLTNonIsoSinglePhotonEt25EtFilter = cms.EDFilter("HLTEgammaEtFilter",
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    inputTag = cms.InputTag("hltL1NonIsoHLTNonIsoSinglePhotonEt25L1MatchFilterRegional"),
    etcutEB = cms.double(25.0),
    etcutEE = cms.double(25.0),
    ncandcut = cms.int32(1)
)


hltUpsilonMML2Filtered = cms.EDFilter("HLTMuonDimuonL2Filter",
    FastAccept = cms.bool(False),
    MinPtMax = cms.double(3.0),
    PreviousCandTag = cms.InputTag("hltUpsilonMML1Filtered"),
    ChargeOpt = cms.int32(0),
    MaxPtBalance = cms.double(999999.0),
    MaxDz = cms.double(9999.0),
    MaxAcop = cms.double(3.15),
    MinNhits = cms.int32(0),
    MinPtMin = cms.double(3.0),
    NSigmaPt = cms.double(0.0),
    MinPtBalance = cms.double(-1.0),
    MaxInvMass = cms.double(13.0),
    BeamSpotTag = cms.InputTag("hltOfflineBeamSpot"),
    MinPtPair = cms.double(0.0),
    CandTag = cms.InputTag("hltL2MuonCandidates"),
    MaxDr = cms.double(100.0),
    MinInvMass = cms.double(6.0),
    MaxEta = cms.double(2.5),
    MinAcop = cms.double(-1.0)
)


hltL1Mu3Jet15L1Filtered0 = cms.EDFilter("HLTMuonL1Filter",
    PreviousCandTag = cms.InputTag("hltL1sL1Mu5Jet20"),
    MinPt = cms.double(0.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    SelectQualities = cms.vint32(),
    CandTag = cms.InputTag("hltL1extraParticles")
)


hltL1sJet140 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleJet70'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltL1sDiJetAve15U8E29 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleJet6U'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltHT100U = cms.EDFilter("HLTGlobalSumsMET",
    observable = cms.string('sumEt'),
    MinN = cms.int32(1),
    Min = cms.double(100.0),
    Max = cms.double(-1.0),
    inputTag = cms.InputTag("hltJet15UHt"),
    saveTag = cms.untracked.bool(True)
)


hltL1NonIsoHLTNonIsoSingleElectronLWEt15L1MatchFilterRegional = cms.EDFilter("HLTEgammaL1MatchFilterRegional",
    doIsolated = cms.bool(False),
    endcap_end = cms.double(2.65),
    candNonIsolatedTag = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    region_eta_size_ecap = cms.double(1.0),
    barrel_end = cms.double(1.4791),
    l1IsolatedTag = cms.InputTag("hltL1extraParticles","Isolated"),
    candIsolatedTag = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    region_phi_size = cms.double(1.044),
    region_eta_size = cms.double(0.522),
    L1SeedFilterTag = cms.InputTag("hltL1sL1SingleEG8"),
    ncandcut = cms.int32(1),
    l1NonIsolatedTag = cms.InputTag("hltL1extraParticles","NonIsolated")
)


hltL1NonIsoHLTNonIsoSingleElectronLWEt15EtFilter = cms.EDFilter("HLTEgammaEtFilter",
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    inputTag = cms.InputTag("hltL1NonIsoHLTNonIsoSingleElectronLWEt15L1MatchFilterRegional"),
    etcutEB = cms.double(15.0),
    etcutEE = cms.double(15.0),
    ncandcut = cms.int32(1)
)


hlt2jetGapFilter = cms.EDFilter("HLT2jetGapFilter",
    minEta = cms.double(1.7),
    inputTag = cms.InputTag("hltMCJetCorJetIcone5"),
    minEt = cms.double(50.0),
    saveTag = cms.untracked.bool(True)
)


hltPreJet50 = cms.EDFilter("HLTPrescaler")


hltPhi2metAco = cms.EDFilter("HLTPhi2METFilter",
    saveTags = cms.untracked.bool(True),
    maxDeltaPhi = cms.double(3.1514),
    inputJetTag = cms.InputTag("hltIterativeCone5CaloJets"),
    inputMETTag = cms.InputTag("hlt1MET60"),
    minDeltaPhi = cms.double(0.377),
    minEtJet1 = cms.double(60.0),
    minEtJet2 = cms.double(60.0)
)


hltL1sL1Mu5Jet20 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_Mu5_Jet20'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltL1sBTagIPJet50U = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleJet30U'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltFilterL25PixelTracksIsolationMuonTau = cms.EDFilter("HLT1Tau",
    MaxEta = cms.double(5.0),
    saveTag = cms.untracked.bool(True),
    inputTag = cms.InputTag("hltL25TauPixelTracksIsolationSelector"),
    MinPt = cms.double(15.0),
    MinN = cms.int32(1)
)


hltPreDiJetAve30U8E29 = cms.EDFilter("HLTPrescaler")


hltPreDoubleJet50MET70Aco = cms.EDFilter("HLTPrescaler")


hltEgammaSCOnlyL1MatchFilterRegional = cms.EDFilter("HLTEgammaL1MatchFilterRegional",
    doIsolated = cms.bool(False),
    endcap_end = cms.double(2.65),
    candNonIsolatedTag = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    region_eta_size_ecap = cms.double(1.0),
    barrel_end = cms.double(1.4791),
    l1IsolatedTag = cms.InputTag("hltL1extraParticles","Isolated"),
    candIsolatedTag = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    region_phi_size = cms.double(1.044),
    region_eta_size = cms.double(0.522),
    L1SeedFilterTag = cms.InputTag("hltL1sEgammaOnlySC"),
    ncandcut = cms.int32(1),
    l1NonIsolatedTag = cms.InputTag("hltL1extraParticles","NonIsolated")
)


hltPreDoublePhoton5UpsL1R = cms.EDFilter("HLTPrescaler")


hltPreHT100 = cms.EDFilter("HLTPrescaler")


hltL1sL1Jet6U = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleJet6U'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltL1sL1TechRPCTTURBst1collisions = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('31'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(True)
)


hltEle10LWL1RHT200ElePixelMatchFilter = cms.EDFilter("HLTElectronPixelMatchFilter",
    doIsolated = cms.bool(False),
    L1IsoPixelSeedsTag = cms.InputTag("hltL1IsoLargeWindowElectronPixelSeeds"),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    L1NonIsoPixelSeedsTag = cms.InputTag("hltL1NonIsoLargeWindowElectronPixelSeeds"),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    SaveTag = cms.untracked.bool(True),
    npixelmatchcut = cms.double(1.0),
    ncandcut = cms.int32(1),
    candTag = cms.InputTag("hltEle10LWL1RHT200EleHcalIsolFilter")
)


hltL1NonIsoHLTNonIsoSingleElectronSiStripEt15EtFilter = cms.EDFilter("HLTEgammaEtFilter",
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    inputTag = cms.InputTag("hltL1NonIsoHLTNonIsoSingleElectronSiStripEt15L1MatchFilterRegional"),
    etcutEB = cms.double(15.0),
    etcutEE = cms.double(15.0),
    ncandcut = cms.int32(1)
)


hltPreAlCaEcalPi08E29 = cms.EDFilter("HLTPrescaler")


hltL1NonIsoSinglePhotonEt15HTITrackIsolFilter = cms.EDFilter("HLTEgammaGenericFilter",
    doIsolated = cms.bool(False),
    thrOverE2EE = cms.double(-1.0),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    thrOverE2EB = cms.double(-1.0),
    thrRegularEE = cms.double(4.0),
    thrOverEEE = cms.double(0.05),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    SaveTag = cms.untracked.bool(True),
    thrRegularEB = cms.double(4.0),
    lessThan = cms.bool(True),
    useEt = cms.bool(True),
    ncandcut = cms.int32(1),
    thrOverEEB = cms.double(0.05),
    isoTag = cms.InputTag("hltL1IsoPhotonHollowTrackIsol"),
    candTag = cms.InputTag("hltL1NonIsoSinglePhotonEt15HTIHcalIsolFilter"),
    nonIsoTag = cms.InputTag("hltL1NonIsoPhotonHollowTrackIsol")
)


hltPreEle15SC15SWLTIL1R = cms.EDFilter("HLTPrescaler")


hltPrePhoton15L1R = cms.EDFilter("HLTPrescaler")


hltPreIsoMu7Jet40 = cms.EDFilter("HLTPrescaler")


hltL1NonIsoSinglePhotonEt15LEIHcalIsolFilter = cms.EDFilter("HLTEgammaGenericFilter",
    doIsolated = cms.bool(False),
    thrOverE2EE = cms.double(-1.0),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    thrOverE2EB = cms.double(-1.0),
    thrRegularEE = cms.double(999999.9),
    thrOverEEE = cms.double(-1.0),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    SaveTag = cms.untracked.bool(True),
    thrRegularEB = cms.double(999999.9),
    lessThan = cms.bool(True),
    useEt = cms.bool(False),
    ncandcut = cms.int32(1),
    thrOverEEB = cms.double(-1.0),
    isoTag = cms.InputTag("hltL1IsolatedPhotonHcalIsol"),
    candTag = cms.InputTag("hltL1NonIsoSinglePhotonEt15LEIEcalIsolFilter"),
    nonIsoTag = cms.InputTag("hltL1NonIsolatedPhotonHcalIsol")
)


hltDiMuonL3PreFiltered = cms.EDFilter("HLTMuonL3PreFilter",
    PreviousCandTag = cms.InputTag("hltDiMuonL2PreFiltered"),
    MinPt = cms.double(3.0),
    MinN = cms.int32(2),
    MaxEta = cms.double(2.5),
    MinNhits = cms.int32(0),
    SaveTag = cms.untracked.bool(True),
    NSigmaPt = cms.double(0.0),
    MaxDz = cms.double(9999.0),
    BeamSpotTag = cms.InputTag("hltOfflineBeamSpot"),
    CandTag = cms.InputTag("hltL3MuonCandidates"),
    MaxDr = cms.double(2.0)
)


hltPreStoppedHSCP1E31 = cms.EDFilter("HLTPrescaler")


hltPreEgammaOnlySC = cms.EDFilter("HLTPrescaler")


hltL1sAlCaEcalPi0Eta1E31 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleIsoEG5 OR L1_SingleIsoEG8 OR L1_SingleIsoEG10 OR L1_SingleIsoEG12 OR L1_SingleIsoEG15 OR L1_SingleEG2 OR L1_SingleEG5 OR L1_SingleEG8 OR L1_SingleEG10 OR L1_SingleEG12 OR L1_SingleEG15 OR L1_SingleEG20 OR L1_SingleJet15 OR L1_SingleJet30 OR L1_SingleJet50 OR L1_SingleJet70 OR L1_SingleJet100 OR L1_DoubleJet70 OR L1_DoubleEG5'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltL1sDiJetAve30U8E29 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleJet20U'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltL1NonIsoHLTNonIsoSingleElectronEt15EleIdOneOEMinusOneOPFilter = cms.EDFilter("HLTElectronOneOEMinusOneOPFilterRegional",
    doIsolated = cms.bool(False),
    electronNonIsolatedProducer = cms.InputTag("hltPixelMatchElectronsL1NonIso"),
    barrelcut = cms.double(999.9),
    electronIsolatedProducer = cms.InputTag("hltPixelMatchElectronsL1Iso"),
    ncandcut = cms.int32(1),
    candTag = cms.InputTag("hltL1NonIsoHLTNonIsoSingleElectronEt15EleIdPixelMatchFilter"),
    endcapcut = cms.double(999.9)
)


hltBoolFirstPath = cms.EDFilter("HLTBool",
    result = cms.bool(False)
)


hltL1NonIsoHLTNonIsoDoublePhotonEt10EtFilter = cms.EDFilter("HLTEgammaEtFilter",
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    inputTag = cms.InputTag("hltL1NonIsoHLTNonIsoDoublePhotonEt10L1MatchFilterRegional"),
    etcutEB = cms.double(10.0),
    etcutEE = cms.double(10.0),
    ncandcut = cms.int32(2)
)


hlt2jet125New = cms.EDFilter("HLT1CaloJet",
    MaxEta = cms.double(5.0),
    saveTag = cms.untracked.bool(True),
    inputTag = cms.InputTag("hltMCJetCorJetIcone5"),
    MinPt = cms.double(125.0),
    MinN = cms.int32(2)
)


hltL1sCSCBeamHalo = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleMuBeamHalo'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltL1NonIsoHLTLEITISinglePhotonEt25EtFilter = cms.EDFilter("HLTEgammaEtFilter",
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    inputTag = cms.InputTag("hltL1NonIsoHLTLEITISinglePhotonEt25L1MatchFilterRegional"),
    etcutEB = cms.double(25.0),
    etcutEE = cms.double(25.0),
    ncandcut = cms.int32(1)
)


hltL1NonIsoHLTNonIsoDoublePhotonEt10L1MatchFilterRegional = cms.EDFilter("HLTEgammaL1MatchFilterRegional",
    doIsolated = cms.bool(False),
    endcap_end = cms.double(2.65),
    candNonIsolatedTag = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    region_eta_size_ecap = cms.double(1.0),
    barrel_end = cms.double(1.4791),
    l1IsolatedTag = cms.InputTag("hltL1extraParticles","Isolated"),
    candIsolatedTag = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    region_phi_size = cms.double(1.044),
    region_eta_size = cms.double(0.522),
    L1SeedFilterTag = cms.InputTag("hltL1sL1DoubleEG5"),
    ncandcut = cms.int32(2),
    l1NonIsolatedTag = cms.InputTag("hltL1extraParticles","NonIsolated")
)


hltDiMuonL1Filtered = cms.EDFilter("HLTMuonL1Filter",
    PreviousCandTag = cms.InputTag("hltL1sL1DoubleMu3"),
    MinPt = cms.double(0.0),
    MinN = cms.int32(2),
    MaxEta = cms.double(2.5),
    SelectQualities = cms.vint32(),
    CandTag = cms.InputTag("hltL1extraParticles")
)


hltL1NonIsoHLTNonIsoDoublePhotonEt15L1MatchFilterRegional = cms.EDFilter("HLTEgammaL1MatchFilterRegional",
    doIsolated = cms.bool(False),
    endcap_end = cms.double(2.65),
    candNonIsolatedTag = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    region_eta_size_ecap = cms.double(1.0),
    barrel_end = cms.double(1.4791),
    l1IsolatedTag = cms.InputTag("hltL1extraParticles","Isolated"),
    candIsolatedTag = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    region_phi_size = cms.double(1.044),
    region_eta_size = cms.double(0.522),
    L1SeedFilterTag = cms.InputTag("hltL1sL1DoubleEG5"),
    ncandcut = cms.int32(2),
    l1NonIsolatedTag = cms.InputTag("hltL1extraParticles","NonIsolated")
)


hltMuJetsMinPt4L2PreFiltered = cms.EDFilter("HLTMuonL2PreFilter",
    PreviousCandTag = cms.InputTag("hltMuJetsMinPt4L1Filtered"),
    MinPt = cms.double(3.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    MinNhits = cms.int32(0),
    NSigmaPt = cms.double(0.0),
    MaxDz = cms.double(9999.0),
    BeamSpotTag = cms.InputTag("hltOfflineBeamSpot"),
    CandTag = cms.InputTag("hltL2MuonCandidates"),
    MaxDr = cms.double(9999.0)
)


hltPreEle15LWL1R = cms.EDFilter("HLTPrescaler")


hltFilterL2EtCutDoubleLooseIsoTau15Trk5 = cms.EDFilter("HLT1Tau",
    MaxEta = cms.double(5.0),
    inputTag = cms.InputTag("hltL2TauJets"),
    MinPt = cms.double(15.0),
    MinN = cms.int32(2)
)


hltIsolPixelTrackL3FilterHE1E31 = cms.EDFilter("HLTPixelIsolTrackFilter",
    MaxPtNearby = cms.double(2.0),
    MinEtaTrack = cms.double(1.4),
    MinEnergyTrack = cms.double(20.0),
    MinPtTrack = cms.double(20.0),
    DropMultiL2Event = cms.bool(False),
    NMaxTrackCandidates = cms.int32(15),
    MaxEtaTrack = cms.double(2.0),
    candTag = cms.InputTag("hltHITIPTCorrectorHE1E31"),
    filterTrackEnergy = cms.bool(True)
)


hltL1NonIsoDoubleElectronEt5JpsiHcalIsolFilter = cms.EDFilter("HLTEgammaGenericFilter",
    doIsolated = cms.bool(False),
    thrOverE2EE = cms.double(-1.0),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    thrOverE2EB = cms.double(-1.0),
    thrRegularEE = cms.double(999999.9),
    thrOverEEE = cms.double(-1.0),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    thrOverEEB = cms.double(-1.0),
    thrRegularEB = cms.double(999999.9),
    lessThan = cms.bool(True),
    useEt = cms.bool(False),
    ncandcut = cms.int32(2),
    isoTag = cms.InputTag("hltL1IsolatedElectronHcalIsol"),
    candTag = cms.InputTag("hltL1NonIsoDoubleElectronEt5JpsiEcalIsolFilter"),
    nonIsoTag = cms.InputTag("hltL1NonIsolatedElectronHcalIsol")
)


hltPreDiJetAve70U = cms.EDFilter("HLTPrescaler")


hltL1NonIsoHLTNonIsoSinglePhotonEt30EtFilter = cms.EDFilter("HLTEgammaEtFilter",
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    inputTag = cms.InputTag("hltL1NonIsoHLTNonIsoSinglePhotonEt30L1MatchFilterRegional"),
    etcutEB = cms.double(30.0),
    etcutEE = cms.double(30.0),
    ncandcut = cms.int32(1)
)


hltPreSingleEG2NoBPTX = cms.EDFilter("HLTPrescaler")


hltMuNoL2IsoJetsHLT1jet40 = cms.EDFilter("HLT1CaloJet",
    MaxEta = cms.double(5.0),
    saveTag = cms.untracked.bool(True),
    inputTag = cms.InputTag("hltMCJetCorJetIcone5"),
    MinPt = cms.double(40.0),
    MinN = cms.int32(1)
)


hlt2jet125 = cms.EDFilter("HLT1CaloJet",
    MaxEta = cms.double(5.0),
    saveTag = cms.untracked.bool(True),
    inputTag = cms.InputTag("hltMCJetCorJetIcone5Regional"),
    MinPt = cms.double(125.0),
    MinN = cms.int32(2)
)


hltSingleMu13L3Filtered13 = cms.EDFilter("HLTMuonL3PreFilter",
    PreviousCandTag = cms.InputTag("hltSingleMu13L2Filtered11"),
    MinPt = cms.double(13.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    MinNhits = cms.int32(0),
    SaveTag = cms.untracked.bool(True),
    NSigmaPt = cms.double(0.0),
    MaxDz = cms.double(9999.0),
    BeamSpotTag = cms.InputTag("hltOfflineBeamSpot"),
    CandTag = cms.InputTag("hltL3MuonCandidates"),
    MaxDr = cms.double(2.0)
)


hltEle10LWL1RHT200L1MatchFilterRegional = cms.EDFilter("HLTEgammaL1MatchFilterRegional",
    doIsolated = cms.bool(False),
    endcap_end = cms.double(2.65),
    candNonIsolatedTag = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    region_eta_size_ecap = cms.double(1.0),
    barrel_end = cms.double(1.4791),
    l1IsolatedTag = cms.InputTag("hltL1extraParticles","Isolated"),
    candIsolatedTag = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    region_phi_size = cms.double(1.044),
    region_eta_size = cms.double(0.522),
    L1SeedFilterTag = cms.InputTag("hltL1sL1EG5HTT100"),
    ncandcut = cms.int32(1),
    l1NonIsolatedTag = cms.InputTag("hltL1extraParticles","NonIsolated")
)


hltHI1jet75U = cms.EDFilter("HLT1CaloJet",
    MaxEta = cms.double(5.0),
    saveTag = cms.untracked.bool(True),
    inputTag = cms.InputTag("hltIterativeCone5PileupSubtractionCaloJets"),
    MinPt = cms.double(75.0),
    MinN = cms.int32(1)
)


hltPreQuadJet30 = cms.EDFilter("HLTPrescaler")


hltL1sTripleJet85 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleJet150 OR L1_DoubleJet70 OR L1_TripleJet50_00002'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltHIPrePhoton30 = cms.EDFilter("HLTPrescaler")


hltDiMuonL3PreFilteredRelaxedVtx2mm = cms.EDFilter("HLTMuonL3PreFilter",
    PreviousCandTag = cms.InputTag("hltDiMuonL2PreFiltered"),
    MinPt = cms.double(3.0),
    MinN = cms.int32(2),
    MaxEta = cms.double(2.5),
    MinNhits = cms.int32(0),
    SaveTag = cms.untracked.bool(True),
    NSigmaPt = cms.double(0.0),
    MaxDz = cms.double(9999.0),
    BeamSpotTag = cms.InputTag("hltOfflineBeamSpot"),
    CandTag = cms.InputTag("hltL3MuonCandidates"),
    MaxDr = cms.double(0.2)
)


hltHIPhoton30 = cms.EDFilter("HLT1Photon",
    MaxEta = cms.double(1.479),
    inputTag = cms.InputTag("hltRecoHIEcalCandidate"),
    MinPt = cms.double(30.0),
    MinN = cms.int32(1)
)


hltL2Mu9L2Filtered9 = cms.EDFilter("HLTMuonL2PreFilter",
    PreviousCandTag = cms.InputTag("hltL1SingleMu7L1Filtered0"),
    MinPt = cms.double(9.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    MinNhits = cms.int32(0),
    SaveTag = cms.untracked.bool(True),
    NSigmaPt = cms.double(0.0),
    MaxDz = cms.double(9999.0),
    BeamSpotTag = cms.InputTag("hltOfflineBeamSpot"),
    CandTag = cms.InputTag("hltL2MuonCandidates"),
    MaxDr = cms.double(9999.0)
)


hltDiMuonIsoL3IsoFiltered = cms.EDFilter("HLTMuonIsoFilter",
    PreviousCandTag = cms.InputTag("hltDiMuonIsoL3PreFiltered"),
    MinN = cms.int32(2),
    SaveTag = cms.untracked.bool(True),
    CandTag = cms.InputTag("hltL3MuonCandidates"),
    IsolatorPSet = cms.PSet(

    ),
    DepTag = cms.VInputTag("hltL3MuonIsolations")
)


hltMuHLTJets3jet30 = cms.EDFilter("HLT1CaloJet",
    MaxEta = cms.double(5.0),
    saveTag = cms.untracked.bool(True),
    inputTag = cms.InputTag("hltMCJetCorJetIcone5"),
    MinPt = cms.double(30.0),
    MinN = cms.int32(3)
)


hltPreL1Jet15 = cms.EDFilter("HLTPrescaler")


hltSingleMuIsoL3PreFiltered9 = cms.EDFilter("HLTMuonL3PreFilter",
    PreviousCandTag = cms.InputTag("hltSingleMuIsoL2IsoFiltered7"),
    MinPt = cms.double(9.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    MinNhits = cms.int32(0),
    NSigmaPt = cms.double(0.0),
    MaxDz = cms.double(9999.0),
    BeamSpotTag = cms.InputTag("hltOfflineBeamSpot"),
    CandTag = cms.InputTag("hltL3MuonCandidates"),
    MaxDr = cms.double(2.0)
)


hltDiJetAve30U1E31 = cms.EDFilter("HLTDiJetAveFilter",
    inputJetTag = cms.InputTag("hltIterativeCone5CaloJets"),
    minPtAve = cms.double(30.0),
    minPtJet3 = cms.double(99999.0),
    saveTag = cms.untracked.bool(True),
    minDphi = cms.double(-1.0)
)


hltL1NonIsoHLTNonIsoSingleElectronSiStripEt20L1MatchFilterRegional = cms.EDFilter("HLTEgammaL1MatchFilterRegional",
    doIsolated = cms.bool(False),
    endcap_end = cms.double(2.65),
    candNonIsolatedTag = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    region_eta_size_ecap = cms.double(1.0),
    barrel_end = cms.double(1.4791),
    l1IsolatedTag = cms.InputTag("hltL1extraParticles","Isolated"),
    candIsolatedTag = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    region_phi_size = cms.double(1.044),
    region_eta_size = cms.double(0.522),
    L1SeedFilterTag = cms.InputTag("hltL1sL1SingleEG8"),
    ncandcut = cms.int32(1),
    l1NonIsolatedTag = cms.InputTag("hltL1extraParticles","NonIsolated")
)


hltHcalSimpleRecHitFilter10 = cms.EDFilter("HLTHcalSimpleRecHitFilter",
    threshold = cms.double(10.0),
    maskedChannels = cms.vint32(8137, 8141, 8146, 8149, 8150, 
        8153),
    HFRecHitCollection = cms.InputTag("hltHfreco")
)


hltL1sIsoTrack1E31 = cms.EDFilter("HLTLevel1GTSeed",
    saveTags = cms.untracked.bool(False),
    L1SeedsLogicalExpression = cms.string('L1_SingleJet50 OR L1_SingleJet70 OR L1_SingleJet100 OR L1_SingleTauJet40 OR L1_SingleTauJet60 OR L1_SingleTauJet80'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltPreL2Mu0NoVertex = cms.EDFilter("HLTPrescaler")


hltPreJet30U = cms.EDFilter("HLTPrescaler")


hltPrePhoton25HLTLEITIL1R = cms.EDFilter("HLTPrescaler")


hltL1sRPCBarrelCosmics = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('24'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(True)
)


hltL1NonIsoHLTNonIsoSingleElectronEt15LTIEcalIsolFilter = cms.EDFilter("HLTEgammaGenericFilter",
    doIsolated = cms.bool(False),
    thrOverE2EE = cms.double(-1.0),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    thrOverE2EB = cms.double(-1.0),
    thrRegularEE = cms.double(999999.9),
    thrOverEEE = cms.double(-1.0),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    thrOverEEB = cms.double(-1.0),
    thrRegularEB = cms.double(999999.9),
    lessThan = cms.bool(True),
    useEt = cms.bool(False),
    ncandcut = cms.int32(1),
    isoTag = cms.InputTag("hltL1IsolatedPhotonEcalIsol"),
    candTag = cms.InputTag("hltL1NonIsoHLTNonIsoSingleElectronEt15LTIEtFilter"),
    nonIsoTag = cms.InputTag("hltL1NonIsolatedPhotonEcalIsol")
)


hltPreL1Jet6U = cms.EDFilter("HLTPrescaler")


hltHIL1sPhoton20 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleEG5'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltL1NonIsoDoublePhotonEt5JpsiPMMassFilter = cms.EDFilter("HLTPMMassFilter",
    lowerMassCut = cms.double(2.0),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    isElectron1 = cms.untracked.bool(False),
    SaveTag = cms.untracked.bool(True),
    isElectron2 = cms.untracked.bool(False),
    upperMassCut = cms.double(4.6),
    candTag = cms.InputTag("hltL1NonIsoDoublePhotonEt5JpsiHcalIsolFilter"),
    nZcandcut = cms.int32(1),
    reqOppCharge = cms.untracked.bool(True)
)


hltL1NonIsoHLTNonIsoSinglePhotonEt25HcalIsolFilter = cms.EDFilter("HLTEgammaGenericFilter",
    doIsolated = cms.bool(False),
    thrOverE2EE = cms.double(-1.0),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    thrOverE2EB = cms.double(-1.0),
    thrRegularEE = cms.double(999999.9),
    thrOverEEE = cms.double(-1.0),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    SaveTag = cms.untracked.bool(True),
    thrRegularEB = cms.double(999999.9),
    lessThan = cms.bool(True),
    useEt = cms.bool(False),
    ncandcut = cms.int32(1),
    thrOverEEB = cms.double(-1.0),
    isoTag = cms.InputTag("hltL1IsolatedPhotonHcalIsol"),
    candTag = cms.InputTag("hltL1NonIsoHLTNonIsoSinglePhotonEt25EtFilter"),
    nonIsoTag = cms.InputTag("hltL1NonIsolatedPhotonHcalIsol")
)


hltL1Mu3EG5L1Filtered0 = cms.EDFilter("HLTMuonL1Filter",
    PreviousCandTag = cms.InputTag("hltL1sL1Mu3EG5"),
    MinPt = cms.double(0.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    SelectQualities = cms.vint32(),
    CandTag = cms.InputTag("hltL1extraParticles")
)


hltL1NonIsoDoubleElectronEt5JpsiEcalIsolFilter = cms.EDFilter("HLTEgammaGenericFilter",
    doIsolated = cms.bool(False),
    thrOverE2EE = cms.double(-1.0),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    thrOverE2EB = cms.double(-1.0),
    thrRegularEE = cms.double(999999.9),
    thrOverEEE = cms.double(-1.0),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    thrOverEEB = cms.double(-1.0),
    thrRegularEB = cms.double(999999.9),
    lessThan = cms.bool(True),
    useEt = cms.bool(False),
    ncandcut = cms.int32(2),
    isoTag = cms.InputTag("hltL1IsolatedPhotonEcalIsol"),
    candTag = cms.InputTag("hltL1NonIsoDoubleElectronEt5JpsiClusterShapeFilter"),
    nonIsoTag = cms.InputTag("hltL1NonIsolatedPhotonEcalIsol")
)


hltMuonTauIsoL2IsoFiltered = cms.EDFilter("HLTMuonIsoFilter",
    IsolatorPSet = cms.PSet(

    ),
    DepTag = cms.VInputTag("hltL2MuonIsolations"),
    PreviousCandTag = cms.InputTag("hltMuonTauIsoL2PreFiltered"),
    MinN = cms.int32(1),
    CandTag = cms.InputTag("hltL2MuonCandidates")
)


hltL1sSplashEcalSumET = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleEG5 OR L1_SingleMuBeamHalo'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(3),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltL1NonIsoDoubleElectronEt5UpsEcalIsolFilter = cms.EDFilter("HLTEgammaGenericFilter",
    doIsolated = cms.bool(False),
    thrOverE2EE = cms.double(-1.0),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    thrOverE2EB = cms.double(-1.0),
    thrRegularEE = cms.double(999999.9),
    thrOverEEE = cms.double(-1.0),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    thrOverEEB = cms.double(-1.0),
    thrRegularEB = cms.double(999999.9),
    lessThan = cms.bool(True),
    useEt = cms.bool(False),
    ncandcut = cms.int32(2),
    isoTag = cms.InputTag("hltL1IsolatedPhotonEcalIsol"),
    candTag = cms.InputTag("hltL1NonIsoDoubleElectronEt5UpsClusterShapeFilter"),
    nonIsoTag = cms.InputTag("hltL1NonIsolatedPhotonEcalIsol")
)


hltSplashEcalEtSum = cms.EDFilter("HLTGlobalSumsCaloMET",
    observable = cms.string('sumEt'),
    MinN = cms.int32(1),
    Min = cms.double(30.0),
    Max = cms.double(-1.0),
    inputTag = cms.InputTag("hltEcalMet"),
    saveTag = cms.untracked.bool(True)
)


hltL1NonIsoHLTNonIsoSingleElectronEt20ESscWrapper = cms.EDFilter("HLTEgammaTriggerFilterObjectWrapper",
    doIsolated = cms.bool(False),
    candIsolatedTag = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    candNonIsolatedTag = cms.InputTag("hltL1NonIsoRecoEcalCandidate")
)


hltEle10LWL1RHT200 = cms.EDFilter("HLTGlobalSumsMET",
    observable = cms.string('sumEt'),
    MinN = cms.int32(1),
    Min = cms.double(200.0),
    Max = cms.double(-1.0),
    inputTag = cms.InputTag("hltJet30Ht"),
    saveTag = cms.untracked.bool(True)
)


hltL1sMinBiasHcal = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleHfBitCountsRing1_1 OR L1_SingleHfBitCountsRing2_1 OR L1_DoubleHfBitCountsRing1_P1N1 OR L1_DoubleHfBitCountsRing2_P1N1 OR L1_SingleHfRingEtSumsRing1_4 OR L1_DoubleHfRingEtSumsRing1_P4N4 OR L1_SingleHfRingEtSumsRing2_4 OR L1_DoubleHfRingEtSumsRing2_P4N4'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltL1NonIsoDoublePhotonEt5JpsiHcalIsolFilter = cms.EDFilter("HLTEgammaGenericFilter",
    doIsolated = cms.bool(False),
    thrOverE2EE = cms.double(-1.0),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    thrOverE2EB = cms.double(-1.0),
    thrRegularEE = cms.double(999999.9),
    thrOverEEE = cms.double(-1.0),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    thrOverEEB = cms.double(-1.0),
    thrRegularEB = cms.double(999999.9),
    lessThan = cms.bool(True),
    useEt = cms.bool(False),
    ncandcut = cms.int32(2),
    isoTag = cms.InputTag("hltL1IsolatedElectronHcalIsol"),
    candTag = cms.InputTag("hltL1NonIsoDoublePhotonEt5JpsiEcalIsolFilter"),
    nonIsoTag = cms.InputTag("hltL1NonIsolatedElectronHcalIsol")
)


hltPreEle10LWEleIdL1R = cms.EDFilter("HLTPrescaler")


hltSingleMu15L3PreFiltered15 = cms.EDFilter("HLTMuonL3PreFilter",
    PreviousCandTag = cms.InputTag("hltSingleMu15L2PreFiltered12"),
    MinPt = cms.double(15.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    MinNhits = cms.int32(0),
    SaveTag = cms.untracked.bool(True),
    NSigmaPt = cms.double(0.0),
    MaxDz = cms.double(9999.0),
    BeamSpotTag = cms.InputTag("hltOfflineBeamSpot"),
    CandTag = cms.InputTag("hltL3MuonCandidates"),
    MaxDr = cms.double(2.0)
)


hltPreL1BPTXPlusOnly = cms.EDFilter("HLTPrescaler")


hltL1NonIsoHLTNonIsoSingleElectronLWEt15ESDoubleSC10 = cms.EDFilter("HLTEgammaEtFilter",
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    inputTag = cms.InputTag("hltL1NonIsoHLTNonIsoSingleElectronLWEt15ESscWrapper"),
    SaveTag = cms.untracked.bool(True),
    etcutEB = cms.double(10.0),
    ncandcut = cms.int32(2),
    etcutEE = cms.double(10.0)
)


hltFilterL25PixelTracksLeadingTrackPtCutMuonTauNoL1 = cms.EDFilter("HLT1Tau",
    MaxEta = cms.double(5.0),
    inputTag = cms.InputTag("hltL25TauPixelTracksLeadingTrackPtCutSelector"),
    MinPt = cms.double(15.0),
    MinN = cms.int32(1)
)


hltPrePixelFEDSize = cms.EDFilter("HLTPrescaler")


hltFilterL3IsolationCutDoubleLooseIsoTau15Trk5 = cms.EDFilter("HLT1Tau",
    MaxEta = cms.double(5.0),
    saveTag = cms.untracked.bool(True),
    inputTag = cms.InputTag("hltL1HLTDoubleLooseIsoTau15Trk5L3JetsMatch"),
    MinPt = cms.double(15.0),
    MinN = cms.int32(1)
)


hltSingleMuIsoL3PreFiltered13 = cms.EDFilter("HLTMuonL3PreFilter",
    PreviousCandTag = cms.InputTag("hltSingleMuIsoL2IsoFiltered11"),
    MinPt = cms.double(13.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    MinNhits = cms.int32(0),
    NSigmaPt = cms.double(0.0),
    MaxDz = cms.double(9999.0),
    BeamSpotTag = cms.InputTag("hltOfflineBeamSpot"),
    CandTag = cms.InputTag("hltL3MuonCandidates"),
    MaxDr = cms.double(2.0)
)


hltSingleMuIsoL3PreFiltered11 = cms.EDFilter("HLTMuonL3PreFilter",
    PreviousCandTag = cms.InputTag("hltSingleMuIsoL2IsoFiltered9"),
    MinPt = cms.double(11.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    MinNhits = cms.int32(0),
    NSigmaPt = cms.double(0.0),
    MaxDz = cms.double(9999.0),
    BeamSpotTag = cms.InputTag("hltOfflineBeamSpot"),
    CandTag = cms.InputTag("hltL3MuonCandidates"),
    MaxDr = cms.double(2.0)
)


hltSingleMuIsoL3PreFiltered15 = cms.EDFilter("HLTMuonL3PreFilter",
    PreviousCandTag = cms.InputTag("hltSingleMuIsoL2IsoFiltered12"),
    MinPt = cms.double(15.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    MinNhits = cms.int32(0),
    NSigmaPt = cms.double(0.0),
    MaxDz = cms.double(9999.0),
    BeamSpotTag = cms.InputTag("hltOfflineBeamSpot"),
    CandTag = cms.InputTag("hltL3MuonCandidates"),
    MaxDr = cms.double(2.0)
)


hltPreDoubleMu0 = cms.EDFilter("HLTPrescaler")


hltPreDoubleMu3 = cms.EDFilter("HLTPrescaler")


hltL1sL1HFtech = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('8 OR 9 OR 10'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(True)
)


hltDiJetAve70U = cms.EDFilter("HLTDiJetAveFilter",
    inputJetTag = cms.InputTag("hltIterativeCone5CaloJets"),
    minPtAve = cms.double(70.0),
    minPtJet3 = cms.double(99999.0),
    saveTag = cms.untracked.bool(True),
    minDphi = cms.double(-1.0)
)


hltL1NonIsoHLTLEITISinglePhotonEt25TrackIsolFilter = cms.EDFilter("HLTEgammaGenericFilter",
    doIsolated = cms.bool(False),
    thrOverE2EE = cms.double(-1.0),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    thrOverE2EB = cms.double(-1.0),
    thrRegularEE = cms.double(4.0),
    thrOverEEE = cms.double(0.05),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    SaveTag = cms.untracked.bool(True),
    thrRegularEB = cms.double(4.0),
    lessThan = cms.bool(True),
    useEt = cms.bool(True),
    ncandcut = cms.int32(1),
    thrOverEEB = cms.double(0.05),
    isoTag = cms.InputTag("hltL1IsoPhotonHollowTrackIsol"),
    candTag = cms.InputTag("hltL1NonIsoHLTLEITISinglePhotonEt25HcalIsolFilter"),
    nonIsoTag = cms.InputTag("hltL1NonIsoPhotonHollowTrackIsol")
)


hltPreMET100 = cms.EDFilter("HLTPrescaler")


hltPreMu15 = cms.EDFilter("HLTPrescaler")


hltPreMu11 = cms.EDFilter("HLTPrescaler")


hltMuJetsMinPtL3PreFiltered = cms.EDFilter("HLTMuonL3PreFilter",
    PreviousCandTag = cms.InputTag("hltMuJetsMinPt4L2PreFiltered"),
    MinPt = cms.double(5.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    MinNhits = cms.int32(0),
    SaveTag = cms.untracked.bool(True),
    NSigmaPt = cms.double(0.0),
    MaxDz = cms.double(9999.0),
    BeamSpotTag = cms.InputTag("hltOfflineBeamSpot"),
    CandTag = cms.InputTag("hltL3MuonCandidates"),
    MaxDr = cms.double(2.0)
)


hltL1NonIsoHLTNonIsoSingleElectronEt15EleIdTrackIsolFilterESet25LTI = cms.EDFilter("HLTElectronGenericFilter",
    doIsolated = cms.bool(False),
    nonIsoTag = cms.InputTag("hltL1NonIsoElectronTrackIsol"),
    L1NonIsoCand = cms.InputTag("hltPixelMatchElectronsL1NonIso"),
    thrRegularEE = cms.double(0.5),
    L1IsoCand = cms.InputTag("hltPixelMatchElectronsL1Iso"),
    SaveTag = cms.untracked.bool(True),
    thrRegularEB = cms.double(0.5),
    lessThan = cms.bool(True),
    ncandcut = cms.int32(1),
    isoTag = cms.InputTag("hltL1IsoElectronTrackIsol"),
    thrOverPtEE = cms.double(-1.0),
    candTag = cms.InputTag("hltL1NonIsoHLTNonIsoSingleElectronEt15EleIdDphiFilter"),
    thrTimesPtEE = cms.double(8.0),
    thrTimesPtEB = cms.double(8.0),
    thrOverPtEB = cms.double(-1.0)
)


hltSingleMu5L3Filtered5 = cms.EDFilter("HLTMuonL3PreFilter",
    PreviousCandTag = cms.InputTag("hltSingleMu5L2Filtered4"),
    MinPt = cms.double(5.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    MinNhits = cms.int32(0),
    SaveTag = cms.untracked.bool(True),
    NSigmaPt = cms.double(0.0),
    MaxDz = cms.double(9999.0),
    BeamSpotTag = cms.InputTag("hltOfflineBeamSpot"),
    CandTag = cms.InputTag("hltL3MuonCandidates"),
    MaxDr = cms.double(2.0)
)


hltL1NonIsoHLTNonIsoSingleElectronLWEt10L1MatchFilterRegional = cms.EDFilter("HLTEgammaL1MatchFilterRegional",
    doIsolated = cms.bool(False),
    endcap_end = cms.double(2.65),
    candNonIsolatedTag = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    region_eta_size_ecap = cms.double(1.0),
    barrel_end = cms.double(1.4791),
    l1IsolatedTag = cms.InputTag("hltL1extraParticles","Isolated"),
    candIsolatedTag = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    region_phi_size = cms.double(1.044),
    region_eta_size = cms.double(0.522),
    L1SeedFilterTag = cms.InputTag("hltL1sL1SingleEG5"),
    ncandcut = cms.int32(1),
    l1NonIsolatedTag = cms.InputTag("hltL1extraParticles","NonIsolated")
)


hltPreMu13 = cms.EDFilter("HLTPrescaler")


hltEcalTowerFilter = cms.EDFilter("HLTEcalTowerFilter",
    MaxEta = cms.double(2.75),
    inputTag = cms.InputTag("hltTowerMakerForEcalABFit"),
    MinE = cms.double(2.0),
    MinN = cms.int32(1)
)


hltL1sDoubleLooseIsoTau15Trk5 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_DoubleTauJet30 OR L1_DoubleJet70'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltL1NonIsoHLTNonIsoDoubleElectronEt10EtFilter = cms.EDFilter("HLTEgammaEtFilter",
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    inputTag = cms.InputTag("hltL1NonIsoHLTNonIsoDoubleElectronEt10L1MatchFilterRegional"),
    etcutEB = cms.double(10.0),
    etcutEE = cms.double(10.0),
    ncandcut = cms.int32(2)
)


hltL1sEgammaOnlySC = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleEG5'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltL1sTripleJet60MET60 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleJet150'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltPreSplashEcalSumET = cms.EDFilter("HLTPrescaler")


hltStoppedHSCP1CaloJetEnergy = cms.EDFilter("HLT1CaloJetEnergy",
    MaxEta = cms.double(3.0),
    saveTag = cms.untracked.bool(True),
    inputTag = cms.InputTag("hltStoppedHSCPIterativeCone5CaloJets"),
    MinE = cms.double(20.0),
    MinN = cms.int32(1)
)


hltDiJetAve15U1E31 = cms.EDFilter("HLTDiJetAveFilter",
    inputJetTag = cms.InputTag("hltIterativeCone5CaloJets"),
    minPtAve = cms.double(15.0),
    minPtJet3 = cms.double(99999.0),
    saveTag = cms.untracked.bool(True),
    minDphi = cms.double(-1.0)
)


hltFilterL2EcalIsolationMuonTauNoL25 = cms.EDFilter("HLT1Tau",
    MaxEta = cms.double(5.0),
    saveTag = cms.untracked.bool(True),
    inputTag = cms.InputTag("hltL2TauRelaxingIsolationSelector","Isolated"),
    MinPt = cms.double(15.0),
    MinN = cms.int32(1)
)


hltSingleMuIsoL2PreFiltered7 = cms.EDFilter("HLTMuonL2PreFilter",
    PreviousCandTag = cms.InputTag("hltSingleMuIsoL1Filtered7"),
    MinPt = cms.double(7.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    MinNhits = cms.int32(0),
    NSigmaPt = cms.double(0.0),
    MaxDz = cms.double(9999.0),
    BeamSpotTag = cms.InputTag("hltOfflineBeamSpot"),
    CandTag = cms.InputTag("hltL2MuonCandidates"),
    MaxDr = cms.double(9999.0)
)


hltL1SingleMu7L1Filtered0 = cms.EDFilter("HLTMuonL1Filter",
    PreviousCandTag = cms.InputTag("hltL1sL1SingleMu7"),
    MinPt = cms.double(0.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    SelectQualities = cms.vint32(),
    CandTag = cms.InputTag("hltL1extraParticles")
)


hltFilterL2EcalIsolationDoubleLooseIsoTau15 = cms.EDFilter("HLT1Tau",
    MaxEta = cms.double(5.0),
    saveTag = cms.untracked.bool(True),
    inputTag = cms.InputTag("hltL1HLTDoubleLooseIsoTau15JetsMatch"),
    MinPt = cms.double(15.0),
    MinN = cms.int32(2)
)


hltPreHTMHT = cms.EDFilter("HLTPrescaler")


hltPreCalibration = cms.EDFilter("HLTPrescaler")


hltL1NonIsoSinglePhotonEt15HTIHcalIsolFilter = cms.EDFilter("HLTEgammaGenericFilter",
    doIsolated = cms.bool(False),
    thrOverE2EE = cms.double(-1.0),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    thrOverE2EB = cms.double(-1.0),
    thrRegularEE = cms.double(999999.9),
    thrOverEEE = cms.double(-1.0),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    thrOverEEB = cms.double(-1.0),
    thrRegularEB = cms.double(999999.9),
    lessThan = cms.bool(True),
    useEt = cms.bool(False),
    ncandcut = cms.int32(1),
    isoTag = cms.InputTag("hltL1IsolatedPhotonHcalIsol"),
    candTag = cms.InputTag("hltL1NonIsoSinglePhotonEt15HTIEtFilter"),
    nonIsoTag = cms.InputTag("hltL1NonIsolatedPhotonHcalIsol")
)


hltPreDoubleJet125Aco = cms.EDFilter("HLTPrescaler")


hltL1sHcalPhiSym = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleEG1 OR L1_SingleEG2 OR L1_DoubleEG1'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltL1NonIsoHLTNonIsoSingleElectronLWEt15PixelMatchFilter = cms.EDFilter("HLTElectronPixelMatchFilter",
    doIsolated = cms.bool(False),
    L1IsoPixelSeedsTag = cms.InputTag("hltL1IsoLargeWindowElectronPixelSeeds"),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    L1NonIsoPixelSeedsTag = cms.InputTag("hltL1NonIsoLargeWindowElectronPixelSeeds"),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    SaveTag = cms.untracked.bool(True),
    npixelmatchcut = cms.double(1.0),
    ncandcut = cms.int32(1),
    candTag = cms.InputTag("hltL1NonIsoHLTNonIsoSingleElectronLWEt15HcalIsolFilter")
)


hltL1NonIsoHLTNonIsoSinglePhotonEt30HcalIsolFilter = cms.EDFilter("HLTEgammaGenericFilter",
    doIsolated = cms.bool(False),
    thrOverE2EE = cms.double(-1.0),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    thrOverE2EB = cms.double(-1.0),
    thrRegularEE = cms.double(999999.9),
    thrOverEEE = cms.double(-1.0),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    SaveTag = cms.untracked.bool(True),
    thrRegularEB = cms.double(999999.9),
    lessThan = cms.bool(True),
    useEt = cms.bool(False),
    ncandcut = cms.int32(1),
    thrOverEEB = cms.double(-1.0),
    isoTag = cms.InputTag("hltL1IsolatedPhotonHcalIsol"),
    candTag = cms.InputTag("hltL1NonIsoHLTNonIsoSinglePhotonEt30EtFilter"),
    nonIsoTag = cms.InputTag("hltL1NonIsolatedPhotonHcalIsol")
)


hltBJet120 = cms.EDFilter("HLT1CaloBJet",
    MaxEta = cms.double(3.0),
    saveTag = cms.untracked.bool(True),
    inputTag = cms.InputTag("hltMCJetCorJetIcone5Regional"),
    MinPt = cms.double(120.0),
    MinN = cms.int32(1)
)


hltL1sL1SingleForJet = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleForJet2U OR L1_SingleForJet4U'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltL1sDoubleJet50MET70Aco = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleJet150'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltPreAlCaEcalPi01E31 = cms.EDFilter("HLTPrescaler")


hltFilter23HLTCSCBeamHaloRing2or3 = cms.EDFilter("HLTCSCRing2or3Filter",
    input = cms.InputTag("hltCsc2DRecHits"),
    xWindow = cms.double(2.0),
    minHits = cms.uint32(4),
    yWindow = cms.double(2.0)
)


hltL1NonIsoHLTLEITISinglePhotonEt20EtFilter = cms.EDFilter("HLTEgammaEtFilter",
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    inputTag = cms.InputTag("hltL1NonIsoHLTLEITISinglePhotonEt20L1MatchFilterRegional"),
    etcutEB = cms.double(20.0),
    etcutEE = cms.double(20.0),
    ncandcut = cms.int32(1)
)


hltPreDoubleMu7Z = cms.EDFilter("HLTPrescaler")


hltL1NonIsoHLTNonIsoSingleElectronEt15LTIPixelMatchFilter = cms.EDFilter("HLTElectronPixelMatchFilter",
    doIsolated = cms.bool(False),
    L1IsoPixelSeedsTag = cms.InputTag("hltL1IsoStartUpElectronPixelSeeds"),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    L1NonIsoPixelSeedsTag = cms.InputTag("hltL1NonIsoStartUpElectronPixelSeeds"),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    npixelmatchcut = cms.double(1.0),
    ncandcut = cms.int32(1),
    candTag = cms.InputTag("hltL1NonIsoHLTNonIsoSingleElectronEt15LTIHcalIsolFilter")
)


hltHIPreJet75U = cms.EDFilter("HLTPrescaler")


hltL1sJet30U = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleJet20U'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltL1sHT100 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_HTT50'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltL1NonIsoHLTNonIsoSinglePhotonEt10L1MatchFilterRegional = cms.EDFilter("HLTEgammaL1MatchFilterRegional",
    doIsolated = cms.bool(False),
    endcap_end = cms.double(2.65),
    candNonIsolatedTag = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    region_eta_size_ecap = cms.double(1.0),
    barrel_end = cms.double(1.4791),
    l1IsolatedTag = cms.InputTag("hltL1extraParticles","Isolated"),
    candIsolatedTag = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    region_phi_size = cms.double(1.044),
    region_eta_size = cms.double(0.522),
    L1SeedFilterTag = cms.InputTag("hltL1sL1SingleEG5"),
    ncandcut = cms.int32(1),
    l1NonIsolatedTag = cms.InputTag("hltL1extraParticles","NonIsolated")
)


hltL1NonIsoHLTVLEIDoublePhotonEt15HcalIsolFilter = cms.EDFilter("HLTEgammaGenericFilter",
    doIsolated = cms.bool(False),
    thrOverE2EE = cms.double(-1.0),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    thrOverE2EB = cms.double(-1.0),
    thrRegularEE = cms.double(999999.9),
    thrOverEEE = cms.double(-1.0),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    SaveTag = cms.untracked.bool(True),
    thrRegularEB = cms.double(999999.9),
    lessThan = cms.bool(True),
    useEt = cms.bool(False),
    ncandcut = cms.int32(2),
    thrOverEEB = cms.double(-1.0),
    isoTag = cms.InputTag("hltL1IsolatedPhotonHcalIsol"),
    candTag = cms.InputTag("hltL1NonIsoHLTVLEIDoublePhotonEt15EcalIsolFilter"),
    nonIsoTag = cms.InputTag("hltL1NonIsolatedPhotonHcalIsol")
)


hltL1sJet50U = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleJet30U'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltPreAlCaDTErrors = cms.EDFilter("HLTPrescaler")


hltL1NonIsoDoublePhotonEt5UpsEcalIsolFilter = cms.EDFilter("HLTEgammaGenericFilter",
    doIsolated = cms.bool(False),
    thrOverE2EE = cms.double(-1.0),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    thrOverE2EB = cms.double(-1.0),
    thrRegularEE = cms.double(999999.9),
    thrOverEEE = cms.double(-1.0),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    thrOverEEB = cms.double(-1.0),
    thrRegularEB = cms.double(999999.9),
    lessThan = cms.bool(True),
    useEt = cms.bool(False),
    ncandcut = cms.int32(2),
    isoTag = cms.InputTag("hltL1IsolatedPhotonEcalIsol"),
    candTag = cms.InputTag("hltL1NonIsoDoublePhotonEt5UpsClusterShapeFilter"),
    nonIsoTag = cms.InputTag("hltL1NonIsolatedPhotonEcalIsol")
)


hltHIPrePhoton15 = cms.EDFilter("HLTPrescaler")


hltL1sMuNoL2IsoJets = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_Mu5_Jet20'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltRPCMuonNoHitsL1Filtered0 = cms.EDFilter("HLTMuonL1Filter",
    PreviousCandTag = cms.InputTag("hltL1sL1SingleMuOpenL1SingleMu0"),
    MinPt = cms.double(0.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(1.6),
    SaveTag = cms.untracked.bool(True),
    SelectQualities = cms.vint32(6),
    CandTag = cms.InputTag("hltL1extraParticles")
)


hltPreL1SingleEG20 = cms.EDFilter("HLTPrescaler")


hltL1NonIsoHLTNonIsoSingleElectronEt15LTIEtFilter = cms.EDFilter("HLTEgammaEtFilter",
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    inputTag = cms.InputTag("hltL1NonIsoHLTNonIsoSingleElectronEt15LTIL1MatchFilterRegional"),
    etcutEB = cms.double(15.0),
    etcutEE = cms.double(15.0),
    ncandcut = cms.int32(1)
)


hltL1NonIsoHLTNonIsoSingleElectronEt15HcalIsolFilter = cms.EDFilter("HLTEgammaGenericFilter",
    doIsolated = cms.bool(False),
    thrOverE2EE = cms.double(-1.0),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    thrOverE2EB = cms.double(-1.0),
    thrRegularEE = cms.double(999999.9),
    thrOverEEE = cms.double(-1.0),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    thrOverEEB = cms.double(-1.0),
    thrRegularEB = cms.double(999999.9),
    lessThan = cms.bool(True),
    useEt = cms.bool(False),
    ncandcut = cms.int32(1),
    isoTag = cms.InputTag("hltL1IsolatedElectronHcalIsol"),
    candTag = cms.InputTag("hltL1NonIsoHLTNonIsoSingleElectronEt15EtFilter"),
    nonIsoTag = cms.InputTag("hltL1NonIsolatedElectronHcalIsol")
)


hltL1NonIsoHLTNonIsoSingleElectronSiStripEt20PixelMatchFilter = cms.EDFilter("HLTElectronPixelMatchFilter",
    doIsolated = cms.bool(False),
    L1IsoPixelSeedsTag = cms.InputTag("hltL1IsoSiStripElectronPixelSeeds"),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    L1NonIsoPixelSeedsTag = cms.InputTag("hltL1NonIsoSiStripElectronPixelSeeds"),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    SaveTag = cms.untracked.bool(True),
    npixelmatchcut = cms.double(1.0),
    ncandcut = cms.int32(1),
    candTag = cms.InputTag("hltL1NonIsoHLTNonIsoSingleElectronSiStripEt20HcalIsolFilter")
)


hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdClusterShapeFilter = cms.EDFilter("HLTEgammaGenericFilter",
    doIsolated = cms.bool(False),
    thrOverE2EE = cms.double(-1.0),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    thrOverE2EB = cms.double(-1.0),
    thrRegularEE = cms.double(0.04),
    thrOverEEE = cms.double(-1.0),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    thrOverEEB = cms.double(-1.0),
    thrRegularEB = cms.double(0.015),
    lessThan = cms.bool(True),
    useEt = cms.bool(False),
    ncandcut = cms.int32(1),
    isoTag = cms.InputTag("hltL1IsoHLTClusterShape"),
    candTag = cms.InputTag("hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdEtFilter"),
    nonIsoTag = cms.InputTag("hltL1NonIsoHLTClusterShape")
)


hltL1NonIsoHLTNonIsoSingleElectronEt20HcalIsolFilter = cms.EDFilter("HLTEgammaGenericFilter",
    doIsolated = cms.bool(False),
    thrOverE2EE = cms.double(-1.0),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    thrOverE2EB = cms.double(-1.0),
    thrRegularEE = cms.double(999999.9),
    thrOverEEE = cms.double(-1.0),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    thrOverEEB = cms.double(-1.0),
    thrRegularEB = cms.double(999999.9),
    lessThan = cms.bool(True),
    useEt = cms.bool(False),
    ncandcut = cms.int32(1),
    isoTag = cms.InputTag("hltL1IsolatedElectronHcalIsol"),
    candTag = cms.InputTag("hltL1NonIsoHLTNonIsoSingleElectronEt20EtFilter"),
    nonIsoTag = cms.InputTag("hltL1NonIsolatedElectronHcalIsol")
)


hltDTActivityFilter = cms.EDFilter("HLTDTActivityFilter",
    processDCC = cms.bool(True),
    processDDU = cms.bool(True),
    inputDDU = cms.InputTag("hltMuonDTDigis"),
    minChamberLayers = cms.int32(6),
    processingMode = cms.int32(2),
    maxStation = cms.int32(3),
    processDigis = cms.bool(True),
    inputDCC = cms.InputTag("hltDTTFUnpacker"),
    minDDUBX = cms.int32(9),
    inputDigis = cms.InputTag("hltMuonDTDigis"),
    minActiveChambs = cms.int32(1),
    maxDDUBX = cms.int32(14),
    minQual = cms.int32(2)
)


hltFilterL2EcalIsolationSingleIsoTau30Trk5 = cms.EDFilter("HLT1Tau",
    MaxEta = cms.double(5.0),
    inputTag = cms.InputTag("hltL2TauRelaxingIsolationSelector","Isolated"),
    MinPt = cms.double(30.0),
    MinN = cms.int32(1)
)


hltSingleMu11L3Filtered11 = cms.EDFilter("HLTMuonL3PreFilter",
    PreviousCandTag = cms.InputTag("hltSingleMu11L2Filtered9"),
    MinPt = cms.double(11.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    MinNhits = cms.int32(0),
    SaveTag = cms.untracked.bool(True),
    NSigmaPt = cms.double(0.0),
    MaxDz = cms.double(9999.0),
    BeamSpotTag = cms.InputTag("hltOfflineBeamSpot"),
    CandTag = cms.InputTag("hltL3MuonCandidates"),
    MaxDr = cms.double(2.0)
)


hltL1SingleMu3L1Filtered0 = cms.EDFilter("HLTMuonL1Filter",
    PreviousCandTag = cms.InputTag("hltL1sL1SingleMu3"),
    MinPt = cms.double(0.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    SelectQualities = cms.vint32(),
    CandTag = cms.InputTag("hltL1extraParticles")
)


hltSingleMu7L3Filtered7 = cms.EDFilter("HLTMuonL3PreFilter",
    PreviousCandTag = cms.InputTag("hltSingleMu7L2Filtered5"),
    MinPt = cms.double(7.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    MinNhits = cms.int32(0),
    SaveTag = cms.untracked.bool(True),
    NSigmaPt = cms.double(0.0),
    MaxDz = cms.double(9999.0),
    BeamSpotTag = cms.InputTag("hltOfflineBeamSpot"),
    CandTag = cms.InputTag("hltL3MuonCandidates"),
    MaxDr = cms.double(2.0)
)


hltPrePhoton15HTIL1R = cms.EDFilter("HLTPrescaler")


hltL1sL1SingleMu7 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleMu7'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltL1sL1SingleMu0 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleMu0'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltL1sL1SingleMu3 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleMu3'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltL1NonIsoDoublePhotonEt5eeResClusterShapeFilter = cms.EDFilter("HLTEgammaGenericFilter",
    doIsolated = cms.bool(False),
    thrOverE2EE = cms.double(-1.0),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    thrOverE2EB = cms.double(-1.0),
    thrRegularEE = cms.double(0.042),
    thrOverEEE = cms.double(-1.0),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    thrOverEEB = cms.double(-1.0),
    thrRegularEB = cms.double(0.016),
    lessThan = cms.bool(True),
    useEt = cms.bool(False),
    ncandcut = cms.int32(2),
    isoTag = cms.InputTag("hltL1IsoHLTClusterShape"),
    candTag = cms.InputTag("hltL1NonIsoDoublePhotonEt5eeResEtFilter"),
    nonIsoTag = cms.InputTag("hltL1NonIsoHLTClusterShape")
)


hltPreMinBias = cms.EDFilter("HLTPrescaler")


hltL1NonIsoHLTNonIsoSingleElectronSiStripEt15PixelMatchFilter = cms.EDFilter("HLTElectronPixelMatchFilter",
    doIsolated = cms.bool(False),
    L1IsoPixelSeedsTag = cms.InputTag("hltL1IsoSiStripElectronPixelSeeds"),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    L1NonIsoPixelSeedsTag = cms.InputTag("hltL1NonIsoSiStripElectronPixelSeeds"),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    SaveTag = cms.untracked.bool(True),
    npixelmatchcut = cms.double(1.0),
    ncandcut = cms.int32(1),
    candTag = cms.InputTag("hltL1NonIsoHLTNonIsoSingleElectronSiStripEt15HcalIsolFilter")
)


simDttfDigis = cms.EDFilter("DTTrackFinder",
    Open_LUTs = cms.untracked.bool(False),
    Extrapolation_nbits_PhiB = cms.untracked.int32(8),
    BX_max = cms.untracked.int32(7),
    BX_min = cms.untracked.int32(-9),
    EtaTrackFinder = cms.untracked.bool(True),
    CSCStub_Source = cms.InputTag("simCsctfTrackDigis"),
    Extrapolation_Filter = cms.untracked.int32(1),
    PT_Assignment_nbits_Phi = cms.untracked.int32(12),
    Overlap = cms.untracked.bool(True),
    PT_Assignment_nbits_PhiB = cms.untracked.int32(10),
    DTDigi_Source = cms.InputTag("simDtTriggerPrimitiveDigis"),
    Extrapolation_21 = cms.untracked.bool(False),
    PHI_Assignment_nbits_PhiB = cms.untracked.int32(10),
    Debug = cms.untracked.int32(0),
    OutOfTime_Filter = cms.untracked.bool(False),
    PHI_Assignment_nbits_Phi = cms.untracked.int32(10),
    Extrapolation_nbits_Phi = cms.untracked.int32(8),
    OutOfTime_Filter_Window = cms.untracked.int32(1)
)


hltIsolPixelTrackL3FilterHB1E31 = cms.EDFilter("HLTPixelIsolTrackFilter",
    MaxPtNearby = cms.double(2.0),
    MinEtaTrack = cms.double(0.0),
    MinEnergyTrack = cms.double(20.0),
    MinPtTrack = cms.double(20.0),
    DropMultiL2Event = cms.bool(False),
    NMaxTrackCandidates = cms.int32(10),
    MaxEtaTrack = cms.double(1.45),
    candTag = cms.InputTag("hltHITIPTCorrectorHB1E31"),
    filterTrackEnergy = cms.bool(True)
)


hltMuJetsHLT1jet40 = cms.EDFilter("HLT1CaloJet",
    MaxEta = cms.double(5.0),
    saveTag = cms.untracked.bool(True),
    inputTag = cms.InputTag("hltMCJetCorJetIcone5"),
    MinPt = cms.double(40.0),
    MinN = cms.int32(1)
)


hltMuJetsMinPt4L1Filtered = cms.EDFilter("HLTMuonL1Filter",
    PreviousCandTag = cms.InputTag("hltL1sMuNoIsoJets30"),
    MinPt = cms.double(5.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    SelectQualities = cms.vint32(),
    CandTag = cms.InputTag("hltL1extraParticles")
)


hltFilterL25PixelTracksIsolationMuonTauNoL2 = cms.EDFilter("HLT1Tau",
    MaxEta = cms.double(5.0),
    saveTag = cms.untracked.bool(True),
    inputTag = cms.InputTag("hltL25TauPixelTracksIsolationSelectorNoL2"),
    MinPt = cms.double(15.0),
    MinN = cms.int32(1)
)


hltMuonTauIsoL3PreFiltered = cms.EDFilter("HLTMuonL3PreFilter",
    PreviousCandTag = cms.InputTag("hltMuonTauIsoL2IsoFiltered"),
    MinPt = cms.double(14.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    MinNhits = cms.int32(0),
    NSigmaPt = cms.double(0.0),
    MaxDz = cms.double(9999.0),
    BeamSpotTag = cms.InputTag("hltOfflineBeamSpot"),
    CandTag = cms.InputTag("hltL3MuonCandidates"),
    MaxDr = cms.double(2.0)
)


hltL1sQuadJet35MET60 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleJet150'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltPrePhoton20L1R = cms.EDFilter("HLTPrescaler")


hltPreMET75 = cms.EDFilter("HLTPrescaler")


hltHT300 = cms.EDFilter("HLTGlobalSumsMET",
    observable = cms.string('sumEt'),
    MinN = cms.int32(1),
    Min = cms.double(300.0),
    Max = cms.double(-1.0),
    inputTag = cms.InputTag("hltJet30Ht"),
    saveTag = cms.untracked.bool(True)
)


hltPreBackwardBSC = cms.EDFilter("HLTPrescaler")


hltPreL1BSC = cms.EDFilter("HLTPrescaler")


hltL1sL1Mu14L1ETM40 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleMu14 AND L1_ETM40'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hlt1MET60 = cms.EDFilter("HLT1CaloMET",
    MaxEta = cms.double(-1.0),
    saveTag = cms.untracked.bool(True),
    inputTag = cms.InputTag("hltMet"),
    MinPt = cms.double(60.0),
    MinN = cms.int32(1)
)


hltPreTripleJet60MET60 = cms.EDFilter("HLTPrescaler")


hltDiMuonIsoL3PreFiltered = cms.EDFilter("HLTMuonL3PreFilter",
    PreviousCandTag = cms.InputTag("hltDiMuonIsoL2IsoFiltered"),
    MinPt = cms.double(3.0),
    MinN = cms.int32(2),
    MaxEta = cms.double(2.5),
    MinNhits = cms.int32(0),
    NSigmaPt = cms.double(0.0),
    MaxDz = cms.double(9999.0),
    BeamSpotTag = cms.InputTag("hltOfflineBeamSpot"),
    CandTag = cms.InputTag("hltL3MuonCandidates"),
    MaxDr = cms.double(2.0)
)


hltL1sSingleEG1 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleEG1'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltMultiTowerFilter = cms.EDFilter("HLTCaloTowerFilter",
    MaxEta = cms.double(1.5),
    inputTag = cms.InputTag("hltTowerMakerForEcal"),
    MinPt = cms.double(2.0),
    MinN = cms.uint32(5)
)


hltPreEle20SWEleIdLTIL1R = cms.EDFilter("HLTPrescaler")


hltL1NonIsoDoublePhotonEt5eeResHcalIsolFilter = cms.EDFilter("HLTEgammaGenericFilter",
    doIsolated = cms.bool(False),
    thrOverE2EE = cms.double(-1.0),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    thrOverE2EB = cms.double(-1.0),
    thrRegularEE = cms.double(999999.9),
    thrOverEEE = cms.double(-1.0),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    thrOverEEB = cms.double(-1.0),
    thrRegularEB = cms.double(999999.9),
    lessThan = cms.bool(True),
    useEt = cms.bool(False),
    ncandcut = cms.int32(2),
    isoTag = cms.InputTag("hltL1IsolatedElectronHcalIsol"),
    candTag = cms.InputTag("hltL1NonIsoDoublePhotonEt5eeResEcalIsolFilter"),
    nonIsoTag = cms.InputTag("hltL1NonIsolatedElectronHcalIsol")
)


hlt1HighMult40 = cms.EDFilter("HLTSingleVertexPixelTrackFilter",
    MaxPt = cms.double(10000.0),
    MinPt = cms.double(0.2),
    MaxVz = cms.double(10.0),
    MaxEta = cms.double(1.0),
    trackCollection = cms.InputTag("hltPixelCands"),
    vertexCollection = cms.InputTag("hltPixelVerticesForMinBias"),
    MinTrks = cms.int32(40),
    MinSep = cms.double(0.12)
)


hltRPCMuonNormaL1Filtered0 = cms.EDFilter("HLTMuonL1Filter",
    PreviousCandTag = cms.InputTag("hltL1sL1SingleMuOpenL1SingleMu0"),
    MinPt = cms.double(0.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(1.6),
    SaveTag = cms.untracked.bool(True),
    SelectQualities = cms.vint32(),
    CandTag = cms.InputTag("hltL1extraParticles")
)


hltPreDoubleMu3JPsi = cms.EDFilter("HLTPrescaler")


hltHIL1sJet90U = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleJet15'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltSingleMu7L2Filtered5 = cms.EDFilter("HLTMuonL2PreFilter",
    PreviousCandTag = cms.InputTag("hltL1SingleMu5L1Filtered0"),
    MinPt = cms.double(5.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    MinNhits = cms.int32(0),
    NSigmaPt = cms.double(0.0),
    MaxDz = cms.double(9999.0),
    BeamSpotTag = cms.InputTag("hltOfflineBeamSpot"),
    CandTag = cms.InputTag("hltL2MuonCandidates"),
    MaxDr = cms.double(9999.0)
)


hltPsi2SMML3Filtered = cms.EDFilter("HLTMuonDimuonL3Filter",
    FastAccept = cms.bool(False),
    MinPtMax = cms.double(3.0),
    PreviousCandTag = cms.InputTag("hltPsi2SMML2Filtered"),
    ChargeOpt = cms.int32(0),
    MaxPtBalance = cms.double(999999.0),
    MaxDz = cms.double(9999.0),
    MaxAcop = cms.double(3.15),
    MinNhits = cms.int32(0),
    MinPtMin = cms.double(3.0),
    SaveTag = cms.untracked.bool(True),
    NSigmaPt = cms.double(0.0),
    MinPtBalance = cms.double(-1.0),
    MaxInvMass = cms.double(3.9),
    BeamSpotTag = cms.InputTag("hltOfflineBeamSpot"),
    MinPtPair = cms.double(0.0),
    CandTag = cms.InputTag("hltL3MuonCandidates"),
    MaxDr = cms.double(2.0),
    MinInvMass = cms.double(3.5),
    MaxEta = cms.double(2.5),
    MinAcop = cms.double(-1.0)
)


hltSingleMuIsoL1Filtered3 = cms.EDFilter("HLTMuonL1Filter",
    PreviousCandTag = cms.InputTag("hltL1sL1SingleMu3"),
    MinPt = cms.double(0.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    SelectQualities = cms.vint32(),
    CandTag = cms.InputTag("hltL1extraParticles")
)


hltAlCaPi0RegRecHits = cms.EDFilter("HLTPi0RecHitsFilter",
    statusLevelRecHitsToUse = cms.int32(1),
    seleMinvMaxPi0 = cms.double(0.22),
    doSelForPi0Endcap = cms.bool(True),
    preshRecHitProducer = cms.InputTag("hltESRegionalPi0EtaRecHit","EcalRecHitsES"),
    pi0ESCollection = cms.string('pi0EcalRecHitsES'),
    preshCalibMIP = cms.double(9e-05),
    flagLevelRecHitsToUse = cms.int32(1),
    selePi0Iso = cms.double(0.5),
    storeIsoClusRecHitEtaEB = cms.bool(True),
    seleEtaIsoEndCap = cms.double(0.5),
    barrelHits = cms.InputTag("hltEcalRegionalPi0EtaRecHit","EcalRecHitsEB"),
    etaEndcapHitCollection = cms.string('etaEcalRecHitsEE'),
    ParameterLogWeighted = cms.bool(True),
    useDBStatus = cms.bool(True),
    seleMinvMinPi0EndCap = cms.double(0.05),
    selePtEtaEndCap_region2 = cms.double(4.0),
    nMinRecHitsSel1stCluster = cms.int32(1),
    debugLevelES = cms.string(' '),
    seleMinvMinPi0 = cms.double(0.06),
    seleMinvMaxEtaEndCap = cms.double(0.8),
    ptMinForIsolationEta = cms.double(1.0),
    doSelForEtaBarrel = cms.bool(False),
    preshCalibGamma = cms.double(0.024),
    seleMinvMinEta = cms.double(0.3),
    selePi0IsoEndCap = cms.double(0.5),
    seleS4S9Gamma = cms.double(0.83),
    massHighPi0Cand = cms.double(0.163),
    store5x5RecHitEtaEB = cms.bool(True),
    selePtGamma = cms.double(1.0),
    store5x5RecHitEtaEE = cms.bool(True),
    selePtGammaEta = cms.double(1.2),
    clusPhiSize = cms.int32(3),
    selePi0BeltDREndCap = cms.double(0.2),
    seleEtaBeltDR = cms.double(0.3),
    seleEtaIso = cms.double(0.5),
    selePi0BeltDeta = cms.double(0.05),
    pi0EndcapHitCollection = cms.string('pi0EcalRecHitsEE'),
    preshNclust = cms.int32(4),
    seleEtaBeltDeta = cms.double(0.1),
    region1_EtaEndCap = cms.double(2.0),
    seleS9S25GammaEtaEndCap = cms.double(0.85),
    preshSeededNstrip = cms.int32(15),
    pi0BarrelHitCollection = cms.string('pi0EcalRecHitsEB'),
    selePtGammaEtaEndCap_region1 = cms.double(1.5),
    selePtGammaEtaEndCap_region2 = cms.double(1.2),
    selePtGammaEtaEndCap_region3 = cms.double(0.7),
    seleMinvMaxEta = cms.double(0.8),
    useRecoFlag = cms.bool(False),
    maxNumberofSeeds = cms.uint32(1000),
    seleEtaBeltDetaEndCap = cms.double(0.1),
    endcapHits = cms.InputTag("hltEcalRegionalPi0EtaRecHit","EcalRecHitsEE"),
    selePi0BeltDR = cms.double(0.2),
    storeIsoClusRecHitEtaEE = cms.bool(True),
    preshCalibPlaneY = cms.double(0.7),
    preshCalibPlaneX = cms.double(1.0),
    selePtPi0 = cms.double(2.0),
    ParameterT0_endcPresh = cms.double(1.2),
    preshStripEnergyCut = cms.double(0.0),
    selePtPi0EndCap_region3 = cms.double(1.2),
    selePtPi0EndCap_region2 = cms.double(2.0),
    selePtPi0EndCap_region1 = cms.double(3.0),
    etaBarrelHitCollection = cms.string('etaEcalRecHitsEB'),
    doSelForEtaEndcap = cms.bool(False),
    seleEtaBeltDREndCap = cms.double(0.3),
    ptMinForIsolationEndCap = cms.double(0.5),
    region2_EtaEndCap = cms.double(2.5),
    seleMinvMaxPi0EndCap = cms.double(0.3),
    seleS4S9GammaEndCap = cms.double(0.9),
    clusSeedThrEndCap = cms.double(1.0),
    ParameterX0 = cms.double(0.89),
    ptMinForIsolation = cms.double(1.0),
    ParameterT0_endc = cms.double(3.1),
    seleMinvMinEtaEndCap = cms.double(0.3),
    ParameterT0_barl = cms.double(7.4),
    seleS4S9GammaEta = cms.double(0.9),
    selePtEta = cms.double(4.0),
    seleS9S25GammaEta = cms.double(0.8),
    selePtGammaPi0EndCap_region3 = cms.double(0.3),
    selePtGammaPi0EndCap_region2 = cms.double(0.5),
    selePtGammaPi0EndCap_region1 = cms.double(0.8),
    selePi0BeltDetaEndCap = cms.double(0.05),
    removePi0CandidatesForEta = cms.bool(True),
    doSelForPi0Barrel = cms.bool(True),
    clusSeedThr = cms.double(0.5),
    storeIsoClusRecHitPi0EB = cms.bool(True),
    storeIsoClusRecHitPi0EE = cms.bool(True),
    etaESCollection = cms.string('etaEcalRecHitsES'),
    clusEtaSize = cms.int32(3),
    nMinRecHitsSel2ndCluster = cms.int32(1),
    region1_Pi0EndCap = cms.double(2.0),
    preshClusterEnergyCut = cms.double(0.0),
    ParameterW0 = cms.double(4.2),
    selePtEtaEndCap_region1 = cms.double(5.0),
    ptMinForIsolationEtaEndCap = cms.double(0.5),
    selePtEtaEndCap_region3 = cms.double(3.0),
    store5x5IsoClusRecHitEtaEE = cms.bool(True),
    store5x5IsoClusRecHitEtaEB = cms.bool(True),
    storeRecHitES = cms.bool(True),
    debugLevel = cms.int32(0),
    region2_Pi0EndCap = cms.double(2.5),
    seleS4S9GammaEtaEndCap = cms.double(0.9),
    massLowPi0Cand = cms.double(0.104),
    maxNumberofClusters = cms.uint32(200)
)


hltPreEle10SWL1RTripleJet30 = cms.EDFilter("HLTPrescaler")


hltL1sL1BPTX = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('3'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(True)
)


hlt1jet250 = cms.EDFilter("HLT1CaloJet",
    MaxEta = cms.double(5.0),
    saveTag = cms.untracked.bool(True),
    inputTag = cms.InputTag("hltMCJetCorJetIcone5Regional"),
    MinPt = cms.double(250.0),
    MinN = cms.int32(1)
)


hltL1NonIsoDoublePhotonEt5eeResEcalIsolFilter = cms.EDFilter("HLTEgammaGenericFilter",
    doIsolated = cms.bool(False),
    thrOverE2EE = cms.double(-1.0),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    thrOverE2EB = cms.double(-1.0),
    thrRegularEE = cms.double(999999.9),
    thrOverEEE = cms.double(-1.0),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    thrOverEEB = cms.double(-1.0),
    thrRegularEB = cms.double(999999.9),
    lessThan = cms.bool(True),
    useEt = cms.bool(False),
    ncandcut = cms.int32(2),
    isoTag = cms.InputTag("hltL1IsolatedPhotonEcalIsol"),
    candTag = cms.InputTag("hltL1NonIsoDoublePhotonEt5eeResClusterShapeFilter"),
    nonIsoTag = cms.InputTag("hltL1NonIsolatedPhotonEcalIsol")
)


hltL1sHTMHT = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_HTT200'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltL1sDiJetAve70U = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleJet70'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltPreMinBiasHcal = cms.EDFilter("HLTPrescaler")


hltL1sMuNoIsoJets = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_Mu5_Jet20'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltL1sL1BPTXMinusOnly = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_BptxMinus AND NOT L1_ZeroBias'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltPreActivityEcal = cms.EDFilter("HLTPrescaler")


hltL1sTrackerCosmics = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('25'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(True)
)


hltL1sMuon5Tau20 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_Mu5_TauJet20 OR L1_Mu5_Jet20'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltL1NonIsoHLTNonIsoSingleElectronEt15EleIdClusterShapeFilter = cms.EDFilter("HLTEgammaGenericFilter",
    doIsolated = cms.bool(False),
    thrOverE2EE = cms.double(-1.0),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    thrOverE2EB = cms.double(-1.0),
    thrRegularEE = cms.double(0.04),
    thrOverEEE = cms.double(-1.0),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    thrOverEEB = cms.double(-1.0),
    thrRegularEB = cms.double(0.015),
    lessThan = cms.bool(True),
    useEt = cms.bool(False),
    ncandcut = cms.int32(1),
    isoTag = cms.InputTag("hltL1IsoHLTClusterShape"),
    candTag = cms.InputTag("hltL1NonIsoHLTNonIsoSingleElectronEt15EleIdEtFilter"),
    nonIsoTag = cms.InputTag("hltL1NonIsoHLTClusterShape")
)


hltL2Mu15L2Filtered15 = cms.EDFilter("HLTMuonL2PreFilter",
    PreviousCandTag = cms.InputTag("hltL1SingleMu7L1Filtered0"),
    MinPt = cms.double(15.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    MinNhits = cms.int32(0),
    SaveTag = cms.untracked.bool(True),
    NSigmaPt = cms.double(0.0),
    MaxDz = cms.double(9999.0),
    BeamSpotTag = cms.InputTag("hltOfflineBeamSpot"),
    CandTag = cms.InputTag("hltL2MuonCandidates"),
    MaxDr = cms.double(9999.0)
)


hltPreL1Mu14L1SingleJet15 = cms.EDFilter("HLTPrescaler")


hltPreL1DoubleMuOpen = cms.EDFilter("HLTPrescaler")


hltL1NonIsoHLTNonIsoSinglePhotonEt10HcalIsolFilter = cms.EDFilter("HLTEgammaGenericFilter",
    doIsolated = cms.bool(False),
    thrOverE2EE = cms.double(-1.0),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    thrOverE2EB = cms.double(-1.0),
    thrRegularEE = cms.double(999999.9),
    thrOverEEE = cms.double(-1.0),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    SaveTag = cms.untracked.bool(True),
    thrRegularEB = cms.double(999999.9),
    lessThan = cms.bool(True),
    useEt = cms.bool(False),
    ncandcut = cms.int32(1),
    thrOverEEB = cms.double(-1.0),
    isoTag = cms.InputTag("hltL1IsolatedPhotonHcalIsol"),
    candTag = cms.InputTag("hltL1NonIsoHLTNonIsoSinglePhotonEt10EtFilter"),
    nonIsoTag = cms.InputTag("hltL1NonIsolatedPhotonHcalIsol")
)


hltL1sL1DoubleEG5 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_DoubleEG5'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltPreDoubleMu3Upsilon = cms.EDFilter("HLTPrescaler")


hltPreBTagIPJet120 = cms.EDFilter("HLTPrescaler")


hltL1NonIsoHLTLEITISinglePhotonEt10EtFilter = cms.EDFilter("HLTEgammaEtFilter",
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    inputTag = cms.InputTag("hltL1NonIsoHLTLEITISinglePhotonEt10L1MatchFilterRegional"),
    etcutEB = cms.double(10.0),
    etcutEE = cms.double(10.0),
    ncandcut = cms.int32(1)
)


hltL1sMET45 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_ETM30'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltL1NonIsoHLTNonIsoSingleElectronLWEt10PixelMatchFilter = cms.EDFilter("HLTElectronPixelMatchFilter",
    doIsolated = cms.bool(False),
    L1IsoPixelSeedsTag = cms.InputTag("hltL1IsoLargeWindowElectronPixelSeeds"),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    L1NonIsoPixelSeedsTag = cms.InputTag("hltL1NonIsoLargeWindowElectronPixelSeeds"),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    SaveTag = cms.untracked.bool(True),
    npixelmatchcut = cms.double(1.0),
    ncandcut = cms.int32(1),
    candTag = cms.InputTag("hltL1NonIsoHLTNonIsoSingleElectronLWEt10HcalIsolFilter")
)


hltMuBSoftL1Filtered = cms.EDFilter("HLTMuonL1Filter",
    PreviousCandTag = cms.InputTag("hltL1sMuB"),
    MinPt = cms.double(3.0),
    MinN = cms.int32(2),
    MaxEta = cms.double(2.5),
    SelectQualities = cms.vint32(),
    CandTag = cms.InputTag("hltL1extraParticles")
)


hltPreDoubleLooseIsoTau15Trk5 = cms.EDFilter("HLTPrescaler")


hltL1sMinBiasBSC = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('32 OR 33 OR 40 OR 41'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(True)
)


hltEle10SWL1RTripleJet30EleEtFilter = cms.EDFilter("HLTEgammaEtFilter",
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    inputTag = cms.InputTag("hltEle10SWL1RTripleJet30L1MatchFilterRegional"),
    etcutEB = cms.double(10.0),
    etcutEE = cms.double(10.0),
    ncandcut = cms.int32(1)
)


hltMuNoIsoJetsL1Filtered = cms.EDFilter("HLTMuonL1Filter",
    PreviousCandTag = cms.InputTag("hltL1sMuNoIsoJets"),
    MinPt = cms.double(14.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    SelectQualities = cms.vint32(),
    CandTag = cms.InputTag("hltL1extraParticles")
)


hltMuBLifetimeIsoL3PreFiltered = cms.EDFilter("HLTMuonL3PreFilter",
    PreviousCandTag = cms.InputTag("hltMuBLifetimeIsoL2IsoFiltered"),
    MinPt = cms.double(7.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    MinNhits = cms.int32(0),
    NSigmaPt = cms.double(0.0),
    MaxDz = cms.double(9999.0),
    BeamSpotTag = cms.InputTag("hltOfflineBeamSpot"),
    CandTag = cms.InputTag("hltL3MuonCandidates"),
    MaxDr = cms.double(2.0)
)


hltPreZeroBiasPixelSingleTrack = cms.EDFilter("HLTPrescaler")


hltL1sMET60HT350 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_HTT300'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltL1NonIsoHLTLEITISinglePhotonEt20EcalIsolFilter = cms.EDFilter("HLTEgammaGenericFilter",
    doIsolated = cms.bool(False),
    thrOverE2EE = cms.double(-1.0),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    thrOverE2EB = cms.double(-1.0),
    thrRegularEE = cms.double(3.0),
    thrOverEEE = cms.double(0.1),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    thrOverEEB = cms.double(0.1),
    thrRegularEB = cms.double(3.0),
    lessThan = cms.bool(True),
    useEt = cms.bool(False),
    ncandcut = cms.int32(1),
    isoTag = cms.InputTag("hltL1IsolatedPhotonEcalIsol"),
    candTag = cms.InputTag("hltL1NonIsoHLTLEITISinglePhotonEt20EtFilter"),
    nonIsoTag = cms.InputTag("hltL1NonIsolatedPhotonEcalIsol")
)


hltPreTrackPointing = cms.EDFilter("HLTPrescaler")


hltL1sDoubleLooseIsoTau15 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_DoubleTauJet14U OR L1_DoubleJet30U'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltSameSignMuL3PreFiltered = cms.EDFilter("HLTMuonDimuonL3Filter",
    FastAccept = cms.bool(False),
    MinPtMax = cms.double(3.0),
    PreviousCandTag = cms.InputTag("hltSameSignMuL2PreFiltered"),
    ChargeOpt = cms.int32(1),
    MaxPtBalance = cms.double(999999.0),
    MaxDz = cms.double(9999.0),
    MaxAcop = cms.double(3.15),
    MinNhits = cms.int32(0),
    MinPtMin = cms.double(3.0),
    SaveTag = cms.untracked.bool(True),
    NSigmaPt = cms.double(0.0),
    MinPtBalance = cms.double(-1.0),
    MaxInvMass = cms.double(9999.0),
    BeamSpotTag = cms.InputTag("hltOfflineBeamSpot"),
    MinPtPair = cms.double(0.0),
    CandTag = cms.InputTag("hltL3MuonCandidates"),
    MaxDr = cms.double(2.0),
    MinInvMass = cms.double(0.0),
    MaxEta = cms.double(2.5),
    MinAcop = cms.double(-1.0)
)


hltMuonTauIsoL2IsoFilteredNoL1Tau = cms.EDFilter("HLTMuonIsoFilter",
    IsolatorPSet = cms.PSet(

    ),
    DepTag = cms.VInputTag("hltL2MuonIsolations"),
    PreviousCandTag = cms.InputTag("hltMuonTauIsoL2PreFilteredNoL1Tau"),
    MinN = cms.int32(1),
    CandTag = cms.InputTag("hltL2MuonCandidates")
)


hltHIPreJet90U = cms.EDFilter("HLTPrescaler")


hltL1sJet60MET70Aco = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleJet150'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltMuonTauIsoL3IsoFiltered = cms.EDFilter("HLTMuonIsoFilter",
    PreviousCandTag = cms.InputTag("hltMuonTauIsoL3PreFiltered"),
    MinN = cms.int32(1),
    SaveTag = cms.untracked.bool(True),
    CandTag = cms.InputTag("hltL3MuonCandidates"),
    IsolatorPSet = cms.PSet(

    ),
    DepTag = cms.VInputTag("hltL3MuonIsolations")
)


hltPreTkMu3NoVertex = cms.EDFilter("HLTPrescaler")


hltPreL1Mu20 = cms.EDFilter("HLTPrescaler")


hltDiMuonL2PreFiltered = cms.EDFilter("HLTMuonL2PreFilter",
    PreviousCandTag = cms.InputTag("hltDiMuonL1Filtered"),
    MinPt = cms.double(3.0),
    MinN = cms.int32(2),
    MaxEta = cms.double(2.5),
    MinNhits = cms.int32(0),
    NSigmaPt = cms.double(0.0),
    MaxDz = cms.double(9999.0),
    BeamSpotTag = cms.InputTag("hltOfflineBeamSpot"),
    CandTag = cms.InputTag("hltL2MuonCandidates"),
    MaxDr = cms.double(9999.0)
)


hltPreDoublePhoton15VLEIL1R = cms.EDFilter("HLTPrescaler")


hltL1sDoubleFwdJet40MET60 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_ETM40'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltPreBTagMuJet20 = cms.EDFilter("HLTPrescaler")


hltPixelActivityFilter = cms.EDFilter("HLTPixelActivityFilter",
    inputTag = cms.InputTag("hltSiPixelClusters"),
    minClusters = cms.uint32(3)
)


hltSingleMu0L1Filtered = cms.EDFilter("HLTMuonL1Filter",
    PreviousCandTag = cms.InputTag("hltL1sL1SingleMu0"),
    MinPt = cms.double(0.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    SelectQualities = cms.vint32(),
    CandTag = cms.InputTag("hltL1extraParticles")
)


hltPreGlobalRunHPDNoise = cms.EDFilter("HLTPrescaler")


hltPreHFThreshold10 = cms.EDFilter("HLTPrescaler")


hltPreRPCBarrelCosmics = cms.EDFilter("HLTPrescaler")


hltPrePhysics = cms.EDFilter("HLTPrescaler")


hltOverlapsHLTCSCBeamHaloOverlapRing1 = cms.EDFilter("HLTCSCOverlapFilter",
    fillHists = cms.bool(False),
    minHits = cms.uint32(4),
    ring2 = cms.bool(False),
    ring1 = cms.bool(True),
    yWindow = cms.double(2.0),
    input = cms.InputTag("hltCsc2DRecHits"),
    xWindow = cms.double(2.0)
)


hltL1NonIsoDoublePhotonEt5UpsClusterShapeFilter = cms.EDFilter("HLTEgammaGenericFilter",
    doIsolated = cms.bool(False),
    thrOverE2EE = cms.double(-1.0),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    thrOverE2EB = cms.double(-1.0),
    thrRegularEE = cms.double(0.042),
    thrOverEEE = cms.double(-1.0),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    thrOverEEB = cms.double(-1.0),
    thrRegularEB = cms.double(0.016),
    lessThan = cms.bool(True),
    useEt = cms.bool(False),
    ncandcut = cms.int32(2),
    isoTag = cms.InputTag("hltL1IsoHLTClusterShape"),
    candTag = cms.InputTag("hltL1NonIsoDoublePhotonEt5UpsEtFilter"),
    nonIsoTag = cms.InputTag("hltL1NonIsoHLTClusterShape")
)


hltPreRPCMuonNoHits = cms.EDFilter("HLTPrescaler")


rpcTriggerDigis = cms.EDFilter("RPCTrigger",
    RPCTriggerDebug = cms.untracked.int32(0),
    label = cms.string('muonRPCDigis')
)


hltPreMinBiasPixelDoubleTrack = cms.EDFilter("HLTPrescaler")


hltL1NonIsoDoublePhotonEt5eeResPMMassFilter = cms.EDFilter("HLTPMMassFilter",
    lowerMassCut = cms.double(2.0),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    isElectron1 = cms.untracked.bool(False),
    SaveTag = cms.untracked.bool(True),
    isElectron2 = cms.untracked.bool(False),
    upperMassCut = cms.double(999999.9),
    candTag = cms.InputTag("hltL1NonIsoDoublePhotonEt5eeResHcalIsolFilter"),
    nZcandcut = cms.int32(1),
    reqOppCharge = cms.untracked.bool(True)
)


hltL1NonIsoHLTLEITISinglePhotonEt10L1MatchFilterRegional = cms.EDFilter("HLTEgammaL1MatchFilterRegional",
    doIsolated = cms.bool(False),
    endcap_end = cms.double(2.65),
    candNonIsolatedTag = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    region_eta_size_ecap = cms.double(1.0),
    barrel_end = cms.double(1.4791),
    l1IsolatedTag = cms.InputTag("hltL1extraParticles","Isolated"),
    candIsolatedTag = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    region_phi_size = cms.double(1.044),
    region_eta_size = cms.double(0.522),
    L1SeedFilterTag = cms.InputTag("hltL1sL1SingleEG5"),
    ncandcut = cms.int32(1),
    l1NonIsolatedTag = cms.InputTag("hltL1extraParticles","NonIsolated")
)


hltL1NonIsoHLTNonIsoSingleElectronEt20EtFilter = cms.EDFilter("HLTEgammaEtFilter",
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    inputTag = cms.InputTag("hltL1NonIsoHLTNonIsoSingleElectronEt20L1MatchFilterRegional"),
    etcutEB = cms.double(20.0),
    etcutEE = cms.double(20.0),
    ncandcut = cms.int32(1)
)


hltL1NonIsoDoubleElectronEt5UpsClusterShapeFilter = cms.EDFilter("HLTEgammaGenericFilter",
    doIsolated = cms.bool(False),
    thrOverE2EE = cms.double(-1.0),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    thrOverE2EB = cms.double(-1.0),
    thrRegularEE = cms.double(0.042),
    thrOverEEE = cms.double(-1.0),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    thrOverEEB = cms.double(-1.0),
    thrRegularEB = cms.double(0.016),
    lessThan = cms.bool(True),
    useEt = cms.bool(False),
    ncandcut = cms.int32(2),
    isoTag = cms.InputTag("hltL1IsoHLTClusterShape"),
    candTag = cms.InputTag("hltL1NonIsoDoubleElectronEt5UpsEtFilter"),
    nonIsoTag = cms.InputTag("hltL1NonIsoHLTClusterShape")
)


hltSingleMu3L2PreFilteredNoVtx = cms.EDFilter("HLTMuonL2PreFilter",
    PreviousCandTag = cms.InputTag("hltSingleMu0L1Filtered"),
    MinPt = cms.double(0.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    MinNhits = cms.int32(0),
    NSigmaPt = cms.double(0.0),
    MaxDz = cms.double(9999.0),
    BeamSpotTag = cms.InputTag("hltOfflineBeamSpot"),
    CandTag = cms.InputTag("hltL2MuonCandidatesNoVtx"),
    MaxDr = cms.double(9999.0)
)


hltPrePhoton10HLTLEITIL1R = cms.EDFilter("HLTPrescaler")


hltL1sAlCaEcalPi0Eta8E29 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleIsoEG5 OR L1_SingleIsoEG8 OR L1_SingleIsoEG10 OR L1_SingleIsoEG12 OR L1_SingleIsoEG15 OR L1_SingleEG2 OR L1_SingleEG5 OR L1_SingleEG8 OR L1_SingleEG10 OR L1_SingleEG12 OR L1_SingleEG15 OR L1_SingleEG20 OR L1_SingleJet6U OR L1_SingleJet20U OR L1_SingleJet30U OR L1_SingleJet40U OR L1_SingleJet50U OR L1_DoubleJet30U OR L1_DoubleEG5'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltL1sBackwardBSC = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('38 OR 39'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(True)
)


hltPreTrackerCosmics = cms.EDFilter("HLTPrescaler")


hltTauL1SeedFilter = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleTauJet10 OR L1_SingleJet15'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltSingleMuIsoL1Filtered7 = cms.EDFilter("HLTMuonL1Filter",
    PreviousCandTag = cms.InputTag("hltL1sL1SingleMu7"),
    MinPt = cms.double(0.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    SelectQualities = cms.vint32(),
    CandTag = cms.InputTag("hltL1extraParticles")
)


hltL1NonIsoHLTNonIsoSingleElectronEt20L1MatchFilterRegional = cms.EDFilter("HLTEgammaL1MatchFilterRegional",
    doIsolated = cms.bool(False),
    endcap_end = cms.double(2.65),
    candNonIsolatedTag = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    region_eta_size_ecap = cms.double(1.0),
    barrel_end = cms.double(1.4791),
    l1IsolatedTag = cms.InputTag("hltL1extraParticles","Isolated"),
    candIsolatedTag = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    region_phi_size = cms.double(1.044),
    region_eta_size = cms.double(0.522),
    L1SeedFilterTag = cms.InputTag("hltL1sL1SingleEG8"),
    ncandcut = cms.int32(1),
    l1NonIsolatedTag = cms.InputTag("hltL1extraParticles","NonIsolated")
)


hltPreMET60HT350 = cms.EDFilter("HLTPrescaler")


hltL1sSingleIsoTau30Trk5 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleTauJet40 OR L1_SingleJet100'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltDynAlCaDTErrors = cms.EDFilter("HLTDynamicPrescaler")


simRpcTriggerDigis = cms.EDFilter("RPCTrigger",
    label = cms.string('simMuonRPCDigis'),
    RPCTriggerDebug = cms.untracked.int32(0)
)


hltL1sL1TechHFcoincidencePM = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('9'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(True)
)


hltSingleMuIsoL2IsoFiltered9 = cms.EDFilter("HLTMuonIsoFilter",
    IsolatorPSet = cms.PSet(

    ),
    DepTag = cms.VInputTag("hltL2MuonIsolations"),
    PreviousCandTag = cms.InputTag("hltSingleMuIsoL2PreFiltered9"),
    MinN = cms.int32(1),
    CandTag = cms.InputTag("hltL2MuonCandidates")
)


hltPreL1SingleEG5_NoBPTX = cms.EDFilter("HLTPrescaler")


hltPreMu5TripleJet30 = cms.EDFilter("HLTPrescaler")


hltSingleMuIsoL2IsoFiltered7 = cms.EDFilter("HLTMuonIsoFilter",
    IsolatorPSet = cms.PSet(

    ),
    DepTag = cms.VInputTag("hltL2MuonIsolations"),
    PreviousCandTag = cms.InputTag("hltSingleMuIsoL2PreFiltered7"),
    MinN = cms.int32(1),
    CandTag = cms.InputTag("hltL2MuonCandidates")
)


hltL1NonIsoHLTVLEIDoublePhotonEt15EtFilter = cms.EDFilter("HLTEgammaEtFilter",
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    inputTag = cms.InputTag("hltL1NonIsoHLTVLEIDoublePhotonEt15L1MatchFilterRegional"),
    etcutEB = cms.double(15.0),
    etcutEE = cms.double(15.0),
    ncandcut = cms.int32(2)
)


hltSingleMuIsoL2IsoFiltered3 = cms.EDFilter("HLTMuonIsoFilter",
    IsolatorPSet = cms.PSet(

    ),
    DepTag = cms.VInputTag("hltL2MuonIsolations"),
    PreviousCandTag = cms.InputTag("hltSingleMuIsoL2PreFiltered3"),
    MinN = cms.int32(1),
    CandTag = cms.InputTag("hltL2MuonCandidates")
)


hltBPTXCoincidence = cms.EDFilter("HLTLevel1Activity",
    technicalBits = cms.uint64(0),
    ignoreL1Mask = cms.bool(False),
    physicsLoBits = cms.uint64(1),
    physicsHiBits = cms.uint64(262144),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    bunchCrossings = cms.vint32(0, 1, -1, 2, -2)
)


hltSingleMuIsoL3IsoFiltered13 = cms.EDFilter("HLTMuonIsoFilter",
    PreviousCandTag = cms.InputTag("hltSingleMuIsoL3PreFiltered13"),
    MinN = cms.int32(1),
    SaveTag = cms.untracked.bool(True),
    CandTag = cms.InputTag("hltL3MuonCandidates"),
    IsolatorPSet = cms.PSet(

    ),
    DepTag = cms.VInputTag("hltL3MuonIsolations")
)


hlt1HT350 = cms.EDFilter("HLTGlobalSumsMET",
    observable = cms.string('sumEt'),
    MinN = cms.int32(1),
    Min = cms.double(350.0),
    Max = cms.double(-1.0),
    inputTag = cms.InputTag("hltHtMet"),
    saveTag = cms.untracked.bool(True)
)


hltSingleMuIsoL3IsoFiltered11 = cms.EDFilter("HLTMuonIsoFilter",
    PreviousCandTag = cms.InputTag("hltSingleMuIsoL3PreFiltered11"),
    MinN = cms.int32(1),
    SaveTag = cms.untracked.bool(True),
    CandTag = cms.InputTag("hltL3MuonCandidates"),
    IsolatorPSet = cms.PSet(

    ),
    DepTag = cms.VInputTag("hltL3MuonIsolations")
)


hltZMML1Filtered = cms.EDFilter("HLTMuonL1Filter",
    PreviousCandTag = cms.InputTag("hltL1sL1DoubleMu3"),
    MinPt = cms.double(0.0),
    MinN = cms.int32(2),
    MaxEta = cms.double(2.5),
    SelectQualities = cms.vint32(),
    CandTag = cms.InputTag("hltL1extraParticles")
)


hltSingleMuIsoL3IsoFiltered15 = cms.EDFilter("HLTMuonIsoFilter",
    PreviousCandTag = cms.InputTag("hltSingleMuIsoL3PreFiltered15"),
    MinN = cms.int32(1),
    SaveTag = cms.untracked.bool(True),
    CandTag = cms.InputTag("hltL3MuonCandidates"),
    IsolatorPSet = cms.PSet(

    ),
    DepTag = cms.VInputTag("hltL3MuonIsolations")
)


hltL1Mu20HQL1Filtered20 = cms.EDFilter("HLTMuonL1Filter",
    PreviousCandTag = cms.InputTag("hltL1sL1SingleMu20"),
    MinPt = cms.double(20.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    SaveTag = cms.untracked.bool(True),
    SelectQualities = cms.vint32(7),
    CandTag = cms.InputTag("hltL1extraParticles")
)


hltL1sQuadJet60 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleJet150 OR L1_DoubleJet70 OR L1_TripleJet50_00002 OR L1_QuadJet30_00002'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltPreMET35 = cms.EDFilter("HLTPrescaler")


hltL1NonIsoHLTNonIsoSingleElectronEt15L1MatchFilterRegional = cms.EDFilter("HLTEgammaL1MatchFilterRegional",
    doIsolated = cms.bool(False),
    endcap_end = cms.double(2.65),
    candNonIsolatedTag = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    region_eta_size_ecap = cms.double(1.0),
    barrel_end = cms.double(1.4791),
    l1IsolatedTag = cms.InputTag("hltL1extraParticles","Isolated"),
    candIsolatedTag = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    region_phi_size = cms.double(1.044),
    region_eta_size = cms.double(0.522),
    L1SeedFilterTag = cms.InputTag("hltL1sL1SingleEG8"),
    ncandcut = cms.int32(1),
    l1NonIsolatedTag = cms.InputTag("hltL1extraParticles","NonIsolated")
)


hltBLifetimeL3FilterStartupU = cms.EDFilter("HLTJetTag",
    JetTag = cms.InputTag("hltBLifetimeL3BJetTagsStartupU"),
    MinTag = cms.double(3.5),
    MaxTag = cms.double(99999.0),
    SaveTag = cms.bool(True),
    MinJets = cms.int32(1)
)


hltEle10LWL1RHT200EleEtFilter = cms.EDFilter("HLTEgammaEtFilter",
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    inputTag = cms.InputTag("hltEle10LWL1RHT200L1MatchFilterRegional"),
    etcutEB = cms.double(10.0),
    etcutEE = cms.double(10.0),
    ncandcut = cms.int32(1)
)


hltPreL1TechRPCTTURBst1collisions = cms.EDFilter("HLTPrescaler")


hlt1jet80 = cms.EDFilter("HLT1CaloJet",
    MaxEta = cms.double(5.0),
    saveTag = cms.untracked.bool(True),
    inputTag = cms.InputTag("hltMCJetCorJetIcone5Regional"),
    MinPt = cms.double(80.0),
    MinN = cms.int32(1)
)


hltMuNoL2IsoJetsL1Filtered = cms.EDFilter("HLTMuonL1Filter",
    PreviousCandTag = cms.InputTag("hltL1sMuNoL2IsoJets"),
    MinPt = cms.double(8.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    SelectQualities = cms.vint32(),
    CandTag = cms.InputTag("hltL1extraParticles")
)


hltPreHcalNZS1E31 = cms.EDFilter("HLTPrescaler")


hltL1NonIsoSinglePhotonEt15HTIEtFilter = cms.EDFilter("HLTEgammaEtFilter",
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    inputTag = cms.InputTag("hltL1NonIsoSinglePhotonEt15HTIL1MatchFilterRegional"),
    etcutEB = cms.double(15.0),
    etcutEE = cms.double(15.0),
    ncandcut = cms.int32(1)
)


hltL2Mu7Photon9L1MatchFilterRegional = cms.EDFilter("HLTEgammaL1MatchFilterRegional",
    doIsolated = cms.bool(False),
    endcap_end = cms.double(2.65),
    candNonIsolatedTag = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    region_eta_size_ecap = cms.double(1.0),
    barrel_end = cms.double(1.4791),
    l1IsolatedTag = cms.InputTag("hltL1extraParticles","Isolated"),
    candIsolatedTag = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    region_phi_size = cms.double(1.044),
    region_eta_size = cms.double(0.522),
    L1SeedFilterTag = cms.InputTag("hltL1sL1Mu3EG5"),
    ncandcut = cms.int32(1),
    l1NonIsolatedTag = cms.InputTag("hltL1extraParticles","NonIsolated")
)


hltJpsiMML1Filtered = cms.EDFilter("HLTMuonL1Filter",
    PreviousCandTag = cms.InputTag("hltL1sL1DoubleMu3"),
    MinPt = cms.double(0.0),
    MinN = cms.int32(2),
    MaxEta = cms.double(2.5),
    SelectQualities = cms.vint32(),
    CandTag = cms.InputTag("hltL1extraParticles")
)


hltSingleMuIsoL2PreFiltered3 = cms.EDFilter("HLTMuonL2PreFilter",
    PreviousCandTag = cms.InputTag("hltSingleMuIsoL1Filtered3"),
    MinPt = cms.double(3.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    MinNhits = cms.int32(0),
    NSigmaPt = cms.double(0.0),
    MaxDz = cms.double(9999.0),
    BeamSpotTag = cms.InputTag("hltOfflineBeamSpot"),
    CandTag = cms.InputTag("hltL2MuonCandidates"),
    MaxDr = cms.double(9999.0)
)


hltL1NonIsoHLTLEITISinglePhotonEt20TrackIsolFilter = cms.EDFilter("HLTEgammaGenericFilter",
    doIsolated = cms.bool(False),
    thrOverE2EE = cms.double(-1.0),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    thrOverE2EB = cms.double(-1.0),
    thrRegularEE = cms.double(4.0),
    thrOverEEE = cms.double(0.05),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    SaveTag = cms.untracked.bool(True),
    thrRegularEB = cms.double(4.0),
    lessThan = cms.bool(True),
    useEt = cms.bool(True),
    ncandcut = cms.int32(1),
    thrOverEEB = cms.double(0.05),
    isoTag = cms.InputTag("hltL1IsoPhotonHollowTrackIsol"),
    candTag = cms.InputTag("hltL1NonIsoHLTLEITISinglePhotonEt20HcalIsolFilter"),
    nonIsoTag = cms.InputTag("hltL1NonIsoPhotonHollowTrackIsol")
)


hltPreL1Mu14L1ETM30 = cms.EDFilter("HLTPrescaler")


hltL1NonIsoHLTNonIsoSingleElectronEt15EleIdPixelMatchFilter = cms.EDFilter("HLTElectronPixelMatchFilter",
    doIsolated = cms.bool(False),
    L1IsoPixelSeedsTag = cms.InputTag("hltL1IsoStartUpElectronPixelSeeds"),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    L1NonIsoPixelSeedsTag = cms.InputTag("hltL1NonIsoStartUpElectronPixelSeeds"),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    npixelmatchcut = cms.double(1.0),
    ncandcut = cms.int32(1),
    candTag = cms.InputTag("hltL1NonIsoHLTNonIsoSingleElectronEt15EleIdHcalIsolFilter")
)


hltHIL1sJet75U = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleJet15'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltBLifetimeL3filter = cms.EDFilter("HLTJetTag",
    JetTag = cms.InputTag("hltBLifetimeL3BJetTags"),
    MinTag = cms.double(6.0),
    MaxTag = cms.double(99999.0),
    SaveTag = cms.bool(True),
    MinJets = cms.int32(1)
)


hltHIPhoton20 = cms.EDFilter("HLT1Photon",
    MaxEta = cms.double(1.479),
    inputTag = cms.InputTag("hltRecoHIEcalCandidate"),
    MinPt = cms.double(20.0),
    MinN = cms.int32(1)
)


hltPreTriggerSummaryRAW = cms.EDFilter("HLTPrescaler")


hltFilterL2EtCutSingleIsoTau30Trk5 = cms.EDFilter("HLT1Tau",
    MaxEta = cms.double(5.0),
    inputTag = cms.InputTag("hltL2TauJets"),
    MinPt = cms.double(30.0),
    MinN = cms.int32(1)
)


hltMuBLifetimeIsoL2PreFiltered = cms.EDFilter("HLTMuonL2PreFilter",
    PreviousCandTag = cms.InputTag("hltMuBLifetimeL1Filtered"),
    MinPt = cms.double(5.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    MinNhits = cms.int32(0),
    NSigmaPt = cms.double(0.0),
    MaxDz = cms.double(9999.0),
    BeamSpotTag = cms.InputTag("hltOfflineBeamSpot"),
    CandTag = cms.InputTag("hltL2MuonCandidates"),
    MaxDr = cms.double(9999.0)
)


hltFilterL2EtCutSingleLooseIsoTau20 = cms.EDFilter("HLT1Tau",
    MaxEta = cms.double(5.0),
    inputTag = cms.InputTag("hltL2TauJets"),
    MinPt = cms.double(20.0),
    MinN = cms.int32(1)
)


hltPreIsoTrackHE1E31 = cms.EDFilter("HLTPrescaler")


hltL1sMET35 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_ETM20'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltDTROMonitorFilter = cms.EDFilter("HLTDTROMonitorFilter",
    inputLabel = cms.InputTag("source")
)


hltBJet20 = cms.EDFilter("HLT1CaloBJet",
    MaxEta = cms.double(3.0),
    saveTag = cms.untracked.bool(True),
    inputTag = cms.InputTag("hltMCJetCorJetIcone5"),
    MinPt = cms.double(20.0),
    MinN = cms.int32(1)
)


hltL1sMuJets = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_Mu5_Jet20'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltL1NonIsoDoublePhotonEt5UpsEtFilter = cms.EDFilter("HLTEgammaEtFilter",
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    inputTag = cms.InputTag("hltL1NonIsoDoublePhotonEt5UpsL1MatchFilterRegional"),
    etcutEB = cms.double(4.0),
    etcutEE = cms.double(4.0),
    ncandcut = cms.int32(2)
)


hltFilterL25PixelTracksLeadingTrackPtCutMuonTauNoL2 = cms.EDFilter("HLT1Tau",
    MaxEta = cms.double(5.0),
    inputTag = cms.InputTag("hltL25TauPixelTracksLeadingTrackPtCutSelectorNoL2"),
    MinPt = cms.double(15.0),
    MinN = cms.int32(1)
)


hltMuonTauL1Filtered = cms.EDFilter("HLTMuonL1Filter",
    PreviousCandTag = cms.InputTag("hltL1sMuon5Tau20"),
    MinPt = cms.double(0.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    SelectQualities = cms.vint32(),
    CandTag = cms.InputTag("hltL1extraParticles")
)


hltL1sJet180MET60 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleJet150'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltL1sTechTrigHCALNoise = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('(11 OR 12) AND ( NOT 0) AND ( NOT 1) AND ( NOT 2)'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(True)
)


hltL1sMuB = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_Mu5_Jet20'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltHIL1sPhoton10 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleEG5'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltPreJet80 = cms.EDFilter("HLTPrescaler")


hltL1sSplashBSC = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('42 OR 43'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(True)
)


hltPreQuadJet35MET60 = cms.EDFilter("HLTPrescaler")


hltAlCaEtaRegRecHits = cms.EDFilter("HLTPi0RecHitsFilter",
    statusLevelRecHitsToUse = cms.int32(1),
    seleMinvMaxPi0 = cms.double(0.22),
    doSelForPi0Endcap = cms.bool(False),
    preshRecHitProducer = cms.InputTag("hltESRegionalPi0EtaRecHit","EcalRecHitsES"),
    pi0ESCollection = cms.string('pi0EcalRecHitsES'),
    preshCalibMIP = cms.double(9e-05),
    flagLevelRecHitsToUse = cms.int32(1),
    selePi0Iso = cms.double(0.5),
    storeIsoClusRecHitEtaEB = cms.bool(True),
    seleEtaIsoEndCap = cms.double(0.5),
    barrelHits = cms.InputTag("hltEcalRegionalPi0EtaRecHit","EcalRecHitsEB"),
    etaEndcapHitCollection = cms.string('etaEcalRecHitsEE'),
    ParameterLogWeighted = cms.bool(True),
    useDBStatus = cms.bool(True),
    seleMinvMinPi0EndCap = cms.double(0.05),
    selePtEtaEndCap_region2 = cms.double(4.0),
    nMinRecHitsSel1stCluster = cms.int32(1),
    debugLevelES = cms.string(' '),
    seleMinvMinPi0 = cms.double(0.06),
    seleMinvMaxEtaEndCap = cms.double(0.8),
    ptMinForIsolationEta = cms.double(1.0),
    doSelForEtaBarrel = cms.bool(True),
    preshCalibGamma = cms.double(0.024),
    seleMinvMinEta = cms.double(0.3),
    selePi0IsoEndCap = cms.double(0.5),
    seleS4S9Gamma = cms.double(0.83),
    massHighPi0Cand = cms.double(0.163),
    store5x5RecHitEtaEB = cms.bool(True),
    selePtGamma = cms.double(1.0),
    store5x5RecHitEtaEE = cms.bool(True),
    selePtGammaEta = cms.double(1.2),
    clusPhiSize = cms.int32(3),
    selePi0BeltDREndCap = cms.double(0.2),
    seleEtaBeltDR = cms.double(0.3),
    seleEtaIso = cms.double(0.5),
    selePi0BeltDeta = cms.double(0.05),
    pi0EndcapHitCollection = cms.string('pi0EcalRecHitsEE'),
    preshNclust = cms.int32(4),
    seleEtaBeltDeta = cms.double(0.1),
    region1_EtaEndCap = cms.double(2.0),
    seleS9S25GammaEtaEndCap = cms.double(0.85),
    preshSeededNstrip = cms.int32(15),
    pi0BarrelHitCollection = cms.string('pi0EcalRecHitsEB'),
    selePtGammaEtaEndCap_region1 = cms.double(1.5),
    selePtGammaEtaEndCap_region2 = cms.double(1.2),
    selePtGammaEtaEndCap_region3 = cms.double(0.7),
    seleMinvMaxEta = cms.double(0.8),
    useRecoFlag = cms.bool(False),
    maxNumberofSeeds = cms.uint32(1000),
    seleEtaBeltDetaEndCap = cms.double(0.1),
    endcapHits = cms.InputTag("hltEcalRegionalPi0EtaRecHit","EcalRecHitsEE"),
    selePi0BeltDR = cms.double(0.2),
    storeIsoClusRecHitEtaEE = cms.bool(True),
    preshCalibPlaneY = cms.double(0.7),
    preshCalibPlaneX = cms.double(1.0),
    selePtPi0 = cms.double(2.0),
    ParameterT0_endcPresh = cms.double(1.2),
    preshStripEnergyCut = cms.double(0.0),
    selePtPi0EndCap_region3 = cms.double(1.2),
    selePtPi0EndCap_region2 = cms.double(2.0),
    selePtPi0EndCap_region1 = cms.double(3.0),
    etaBarrelHitCollection = cms.string('etaEcalRecHitsEB'),
    doSelForEtaEndcap = cms.bool(True),
    seleEtaBeltDREndCap = cms.double(0.3),
    ptMinForIsolationEndCap = cms.double(0.5),
    region2_EtaEndCap = cms.double(2.5),
    seleMinvMaxPi0EndCap = cms.double(0.3),
    seleS4S9GammaEndCap = cms.double(0.9),
    clusSeedThrEndCap = cms.double(1.0),
    ParameterX0 = cms.double(0.89),
    ptMinForIsolation = cms.double(1.0),
    ParameterT0_endc = cms.double(3.1),
    seleMinvMinEtaEndCap = cms.double(0.3),
    ParameterT0_barl = cms.double(7.4),
    seleS4S9GammaEta = cms.double(0.9),
    selePtEta = cms.double(4.0),
    seleS9S25GammaEta = cms.double(0.8),
    selePtGammaPi0EndCap_region3 = cms.double(0.3),
    selePtGammaPi0EndCap_region2 = cms.double(0.5),
    selePtGammaPi0EndCap_region1 = cms.double(0.8),
    selePi0BeltDetaEndCap = cms.double(0.05),
    removePi0CandidatesForEta = cms.bool(True),
    doSelForPi0Barrel = cms.bool(False),
    clusSeedThr = cms.double(0.5),
    storeIsoClusRecHitPi0EB = cms.bool(True),
    storeIsoClusRecHitPi0EE = cms.bool(True),
    etaESCollection = cms.string('etaEcalRecHitsES'),
    clusEtaSize = cms.int32(3),
    nMinRecHitsSel2ndCluster = cms.int32(1),
    region1_Pi0EndCap = cms.double(2.0),
    preshClusterEnergyCut = cms.double(0.0),
    ParameterW0 = cms.double(4.2),
    selePtEtaEndCap_region1 = cms.double(5.0),
    ptMinForIsolationEtaEndCap = cms.double(0.5),
    selePtEtaEndCap_region3 = cms.double(3.0),
    store5x5IsoClusRecHitEtaEE = cms.bool(True),
    store5x5IsoClusRecHitEtaEB = cms.bool(True),
    storeRecHitES = cms.bool(True),
    debugLevel = cms.int32(0),
    region2_Pi0EndCap = cms.double(2.5),
    seleS4S9GammaEtaEndCap = cms.double(0.9),
    massLowPi0Cand = cms.double(0.104),
    maxNumberofClusters = cms.uint32(200)
)


hltL1sHIMinBiasCalo = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleHfBitCountsRing1_1 OR L1_SingleHfBitCountsRing2_1 OR L1_DoubleHfBitCountsRing1_P1N1 OR L1_DoubleHfBitCountsRing2_P1N1 OR L1_SingleHfRingEtSumsRing1_4 OR L1_DoubleHfRingEtSumsRing1_P4N4 OR L1_SingleHfRingEtSumsRing2_4 OR L1_DoubleHfRingEtSumsRing2_P4N4 OR L1_SingleEG1'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltL1NonIsoHLTLEITISinglePhotonEt10EcalIsolFilter = cms.EDFilter("HLTEgammaGenericFilter",
    doIsolated = cms.bool(False),
    thrOverE2EE = cms.double(-1.0),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    thrOverE2EB = cms.double(-1.0),
    thrRegularEE = cms.double(3.0),
    thrOverEEE = cms.double(0.1),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    thrOverEEB = cms.double(0.1),
    thrRegularEB = cms.double(3.0),
    lessThan = cms.bool(True),
    useEt = cms.bool(False),
    ncandcut = cms.int32(1),
    isoTag = cms.InputTag("hltL1IsolatedPhotonEcalIsol"),
    candTag = cms.InputTag("hltL1NonIsoHLTLEITISinglePhotonEt10EtFilter"),
    nonIsoTag = cms.InputTag("hltL1NonIsolatedPhotonEcalIsol")
)


hltL1NonIsoHLTNonIsoSingleElectronEt10EtFilter = cms.EDFilter("HLTEgammaEtFilter",
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    inputTag = cms.InputTag("hltL1NonIsoHLTNonIsoSingleElectronEt10L1MatchFilterRegional"),
    etcutEB = cms.double(10.0),
    etcutEE = cms.double(10.0),
    ncandcut = cms.int32(1)
)


hltL1sMinBias = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_MinBias_HTT10'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltPsi2SMML2Filtered = cms.EDFilter("HLTMuonDimuonL2Filter",
    FastAccept = cms.bool(False),
    MinPtMax = cms.double(3.0),
    PreviousCandTag = cms.InputTag("hltJpsiMML1Filtered"),
    ChargeOpt = cms.int32(0),
    MaxPtBalance = cms.double(999999.0),
    MaxDz = cms.double(9999.0),
    MaxAcop = cms.double(3.15),
    MinNhits = cms.int32(0),
    MinPtMin = cms.double(3.0),
    NSigmaPt = cms.double(0.0),
    MinPtBalance = cms.double(-1.0),
    MaxInvMass = cms.double(5.6),
    BeamSpotTag = cms.InputTag("hltOfflineBeamSpot"),
    MinPtPair = cms.double(0.0),
    CandTag = cms.InputTag("hltL2MuonCandidates"),
    MaxDr = cms.double(100.0),
    MinInvMass = cms.double(1.6),
    MaxEta = cms.double(2.5),
    MinAcop = cms.double(-1.0)
)


hltL1NonIsoHLTNonIsoSingleElectronLWEt15HcalIsolFilter = cms.EDFilter("HLTEgammaGenericFilter",
    doIsolated = cms.bool(False),
    thrOverE2EE = cms.double(-1.0),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    thrOverE2EB = cms.double(-1.0),
    thrRegularEE = cms.double(999999.9),
    thrOverEEE = cms.double(-1.0),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    thrOverEEB = cms.double(-1.0),
    thrRegularEB = cms.double(999999.9),
    lessThan = cms.bool(True),
    useEt = cms.bool(False),
    ncandcut = cms.int32(1),
    isoTag = cms.InputTag("hltL1IsolatedElectronHcalIsol"),
    candTag = cms.InputTag("hltL1NonIsoHLTNonIsoSingleElectronLWEt15EtFilter"),
    nonIsoTag = cms.InputTag("hltL1NonIsolatedElectronHcalIsol")
)


hltPreMu9 = cms.EDFilter("HLTPrescaler")


hltFilterL2EcalIsolationDoubleLooseIsoTau15Trk5 = cms.EDFilter("HLT1Tau",
    MaxEta = cms.double(5.0),
    inputTag = cms.InputTag("hltL2TauRelaxingIsolationSelector","Isolated"),
    MinPt = cms.double(15.0),
    MinN = cms.int32(2)
)


hltL1sAlCaEcalPhiSym = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_ZeroBias OR L1_SingleEG1 OR L1_SingleEG2 OR L1_SingleEG5 OR L1_DoubleEG1 OR L1_SingleHfBitCountsRing1_1 OR L1_DoubleHfBitCountsRing1_P1N1 OR L1_SingleHfRingEtSumsRing1_4 OR L1_DoubleHfRingEtSumsRing1_P4N4 OR L1_SingleHfRingEtSumsRing2_4 OR L1_DoubleHfRingEtSumsRing2_P4N4'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltPreL1Mu20HQ = cms.EDFilter("HLTPrescaler")


hltPreMu3 = cms.EDFilter("HLTPrescaler")


hltL1NonIsoHLTNonIsoDoublePhotonEt15EtFilter = cms.EDFilter("HLTEgammaEtFilter",
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    inputTag = cms.InputTag("hltL1NonIsoHLTNonIsoDoublePhotonEt15L1MatchFilterRegional"),
    etcutEB = cms.double(15.0),
    etcutEE = cms.double(15.0),
    ncandcut = cms.int32(2)
)


hltMuonTauIsoL2PreFilteredNoL1Tau = cms.EDFilter("HLTMuonL2PreFilter",
    PreviousCandTag = cms.InputTag("hltMuonTauL1FilteredNoL1Tau"),
    MinPt = cms.double(12.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    MinNhits = cms.int32(0),
    NSigmaPt = cms.double(0.0),
    MaxDz = cms.double(9999.0),
    BeamSpotTag = cms.InputTag("hltOfflineBeamSpot"),
    CandTag = cms.InputTag("hltL2MuonCandidates"),
    MaxDr = cms.double(9999.0)
)


hltPreMu7 = cms.EDFilter("HLTPrescaler")


hltPreMu5 = cms.EDFilter("HLTPrescaler")


hltPreL1Mu14L1SingleEG10 = cms.EDFilter("HLTPrescaler")


hltL1NonIsoHLTNonIsoSingleElectronEt10HcalIsolFilter = cms.EDFilter("HLTEgammaGenericFilter",
    doIsolated = cms.bool(False),
    thrOverE2EE = cms.double(-1.0),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    thrOverE2EB = cms.double(-1.0),
    thrRegularEE = cms.double(999999.9),
    thrOverEEE = cms.double(-1.0),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    thrOverEEB = cms.double(-1.0),
    thrRegularEB = cms.double(999999.9),
    lessThan = cms.bool(True),
    useEt = cms.bool(False),
    ncandcut = cms.int32(1),
    isoTag = cms.InputTag("hltL1IsolatedElectronHcalIsol"),
    candTag = cms.InputTag("hltL1NonIsoHLTNonIsoSingleElectronEt10EtFilter"),
    nonIsoTag = cms.InputTag("hltL1NonIsolatedElectronHcalIsol")
)


hltL1sFwdJet20U = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_IsoEG10_Jet6U_ForJet6U'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltL1sL1SingleEG8orL1DoubleEG5 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleEG8 OR L1_DoubleEG5'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltIsolPixelTrackL2FilterHE1E31 = cms.EDFilter("HLTPixelIsolTrackFilter",
    MaxPtNearby = cms.double(2.0),
    MinEtaTrack = cms.double(1.4),
    MinEnergyTrack = cms.double(20.0),
    MinPtTrack = cms.double(3.5),
    DropMultiL2Event = cms.bool(False),
    NMaxTrackCandidates = cms.int32(5),
    MaxEtaTrack = cms.double(2.0),
    candTag = cms.InputTag("hltIsolPixelTrackProdHE1E31"),
    filterTrackEnergy = cms.bool(True)
)


hltPixelFEDSizeFilter = cms.EDFilter("HLTFEDSizeFilter",
    requireAllFEDs = cms.bool(False),
    threshold = cms.uint32(8320),
    firstFED = cms.uint32(0),
    rawData = cms.InputTag("source"),
    lastFED = cms.uint32(39)
)


hltL1NonIsoHLTNonIsoSingleElectronEt15LTIL1MatchFilterRegional = cms.EDFilter("HLTEgammaL1MatchFilterRegional",
    doIsolated = cms.bool(False),
    endcap_end = cms.double(2.65),
    candNonIsolatedTag = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    region_eta_size_ecap = cms.double(1.0),
    barrel_end = cms.double(1.4791),
    l1IsolatedTag = cms.InputTag("hltL1extraParticles","Isolated"),
    candIsolatedTag = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    region_phi_size = cms.double(1.044),
    region_eta_size = cms.double(0.522),
    L1SeedFilterTag = cms.InputTag("hltL1sL1SingleEG8"),
    ncandcut = cms.int32(1),
    l1NonIsolatedTag = cms.InputTag("hltL1extraParticles","NonIsolated")
)


hltPreDiJetAve15U8E29 = cms.EDFilter("HLTPrescaler")


hltL1sQuadJet15U = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_QuadJet6U'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltL1NonIsoSinglePhotonEt15LEIL1MatchFilterRegional = cms.EDFilter("HLTEgammaL1MatchFilterRegional",
    doIsolated = cms.bool(False),
    endcap_end = cms.double(2.65),
    candNonIsolatedTag = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    region_eta_size_ecap = cms.double(1.0),
    barrel_end = cms.double(1.4791),
    l1IsolatedTag = cms.InputTag("hltL1extraParticles","Isolated"),
    candIsolatedTag = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    region_phi_size = cms.double(1.044),
    region_eta_size = cms.double(0.522),
    L1SeedFilterTag = cms.InputTag("hltL1sL1SingleEG8"),
    ncandcut = cms.int32(1),
    l1NonIsolatedTag = cms.InputTag("hltL1extraParticles","NonIsolated")
)


hltL1NonIsoHLTIsoSingleElectronEt20LTIEleIdEtFilter = cms.EDFilter("HLTEgammaEtFilter",
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    inputTag = cms.InputTag("hltL1NonIsoHLTIsoSingleElectronEt20LTIEleIdL1MatchFilterRegional"),
    etcutEB = cms.double(20.0),
    etcutEE = cms.double(20.0),
    ncandcut = cms.int32(1)
)


hltSingleMuIsoL2PreFiltered12 = cms.EDFilter("HLTMuonL2PreFilter",
    PreviousCandTag = cms.InputTag("hltSingleMuIsoL1Filtered10"),
    MinPt = cms.double(12.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    MinNhits = cms.int32(0),
    NSigmaPt = cms.double(0.0),
    MaxDz = cms.double(9999.0),
    BeamSpotTag = cms.InputTag("hltOfflineBeamSpot"),
    CandTag = cms.InputTag("hltL2MuonCandidates"),
    MaxDr = cms.double(9999.0)
)


hltL1sL1Mu14L1ETM30 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleMu14 AND L1_ETM30'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltDiMuonL1Filtered0 = cms.EDFilter("HLTMuonL1Filter",
    PreviousCandTag = cms.InputTag("hltL1sL1DoubleMuOpen"),
    MinPt = cms.double(0.0),
    MinN = cms.int32(2),
    MaxEta = cms.double(2.5),
    SelectQualities = cms.vint32(),
    CandTag = cms.InputTag("hltL1extraParticles")
)


hltPreJet110 = cms.EDFilter("HLTPrescaler")


hltL1NonIsoHLTNonIsoSinglePhotonEt15EtFilter = cms.EDFilter("HLTEgammaEtFilter",
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    inputTag = cms.InputTag("hltL1NonIsoHLTNonIsoSinglePhotonEt15L1MatchFilterRegional"),
    etcutEB = cms.double(15.0),
    etcutEE = cms.double(15.0),
    ncandcut = cms.int32(1)
)


hltSingleMu9L2Filtered7 = cms.EDFilter("HLTMuonL2PreFilter",
    PreviousCandTag = cms.InputTag("hltL1SingleMu7L1Filtered0"),
    MinPt = cms.double(7.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    MinNhits = cms.int32(0),
    NSigmaPt = cms.double(0.0),
    MaxDz = cms.double(9999.0),
    BeamSpotTag = cms.InputTag("hltOfflineBeamSpot"),
    CandTag = cms.InputTag("hltL2MuonCandidates"),
    MaxDr = cms.double(9999.0)
)


hltL1sDoubleJet125MET60 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleJet150'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltStoppedHSCPHpdFilter = cms.EDFilter("HLTHPDFilter",
    rbxSpikeEnergy = cms.double(50.0),
    energy = cms.double(-99.0),
    inputTag = cms.InputTag("hltHbhereco"),
    hpdSpikeIsolationEnergy = cms.double(1.0),
    hpdSpikeEnergy = cms.double(10.0),
    rbxSpikeUnbalance = cms.double(0.2)
)


hltBSoftMuonL3FilterByDR = cms.EDFilter("HLTJetTag",
    JetTag = cms.InputTag("hltBSoftMuonL3BJetTagsByDR"),
    MinTag = cms.double(0.5),
    MaxTag = cms.double(99999.0),
    SaveTag = cms.bool(True),
    MinJets = cms.int32(1)
)


hltHIPhoton15 = cms.EDFilter("HLT1Photon",
    MaxEta = cms.double(2.0),
    inputTag = cms.InputTag("hltRecoHIEcalCandidate"),
    MinPt = cms.double(15.0),
    MinN = cms.int32(1)
)


hltPreEle20LWL1R = cms.EDFilter("HLTPrescaler")


hltL1sSumET120 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_ETT60'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltPreJet180MET60 = cms.EDFilter("HLTPrescaler")


hltSingleMu15L1Filtered0 = cms.EDFilter("HLTMuonL1Filter",
    PreviousCandTag = cms.InputTag("hltL1sL1SingleMu10"),
    MinPt = cms.double(0.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    SelectQualities = cms.vint32(),
    CandTag = cms.InputTag("hltL1extraParticles")
)


hltMuJetsL1Filtered = cms.EDFilter("HLTMuonL1Filter",
    PreviousCandTag = cms.InputTag("hltL1sMuJets"),
    MinPt = cms.double(7.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    SelectQualities = cms.vint32(),
    CandTag = cms.InputTag("hltL1extraParticles")
)


hltL1sL1SingleMuOpenL1SingleMu0L1SingleMu3 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleMuOpen OR L1_SingleMu0 OR L1_SingleMu3'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltBLifetimeL25FilterStartup = cms.EDFilter("HLTJetTag",
    JetTag = cms.InputTag("hltBLifetimeL25BJetTagsStartup"),
    MinTag = cms.double(2.5),
    MaxTag = cms.double(99999.0),
    SaveTag = cms.bool(False),
    MinJets = cms.int32(1)
)


hlt2jetvbf = cms.EDFilter("HLTJetVBFFilter",
    minDeltaEta = cms.double(2.5),
    minEt = cms.double(40.0),
    inputTag = cms.InputTag("hltMCJetCorJetIcone5"),
    saveTag = cms.untracked.bool(True)
)


hltDiJetAve130U = cms.EDFilter("HLTDiJetAveFilter",
    inputJetTag = cms.InputTag("hltIterativeCone5CaloJets"),
    minPtAve = cms.double(130.0),
    minPtJet3 = cms.double(99999.0),
    saveTag = cms.untracked.bool(True),
    minDphi = cms.double(-1.0)
)


hltL1NonIsoHLTNonIsoSingleElectronEt15LTIHcalIsolFilter = cms.EDFilter("HLTEgammaGenericFilter",
    doIsolated = cms.bool(False),
    thrOverE2EE = cms.double(-1.0),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    thrOverE2EB = cms.double(-1.0),
    thrRegularEE = cms.double(999999.9),
    thrOverEEE = cms.double(-1.0),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    thrOverEEB = cms.double(-1.0),
    thrRegularEB = cms.double(999999.9),
    lessThan = cms.bool(True),
    useEt = cms.bool(False),
    ncandcut = cms.int32(1),
    isoTag = cms.InputTag("hltL1IsolatedElectronHcalIsol"),
    candTag = cms.InputTag("hltL1NonIsoHLTNonIsoSingleElectronEt15LTIEcalIsolFilter"),
    nonIsoTag = cms.InputTag("hltL1NonIsolatedElectronHcalIsol")
)


hltL1NonIsoHLTNonIsoSinglePhotonEt25L1MatchFilterRegional = cms.EDFilter("HLTEgammaL1MatchFilterRegional",
    doIsolated = cms.bool(False),
    endcap_end = cms.double(2.65),
    candNonIsolatedTag = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    region_eta_size_ecap = cms.double(1.0),
    barrel_end = cms.double(1.4791),
    l1IsolatedTag = cms.InputTag("hltL1extraParticles","Isolated"),
    candIsolatedTag = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    region_phi_size = cms.double(1.044),
    region_eta_size = cms.double(0.522),
    L1SeedFilterTag = cms.InputTag("hltL1sL1SingleEG8"),
    ncandcut = cms.int32(1),
    l1NonIsolatedTag = cms.InputTag("hltL1extraParticles","NonIsolated")
)


hlt1jet140 = cms.EDFilter("HLT1CaloJet",
    MaxEta = cms.double(5.0),
    saveTag = cms.untracked.bool(True),
    inputTag = cms.InputTag("hltMCJetCorJetIcone5Regional"),
    MinPt = cms.double(140.0),
    MinN = cms.int32(1)
)


hltPreEle15SWEleIdSC15L1R = cms.EDFilter("HLTPrescaler")


hltSingleMuIsoL2PreFiltered9 = cms.EDFilter("HLTMuonL2PreFilter",
    PreviousCandTag = cms.InputTag("hltSingleMuIsoL1Filtered7"),
    MinPt = cms.double(9.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    MinNhits = cms.int32(0),
    NSigmaPt = cms.double(0.0),
    MaxDz = cms.double(9999.0),
    BeamSpotTag = cms.InputTag("hltOfflineBeamSpot"),
    CandTag = cms.InputTag("hltL2MuonCandidates"),
    MaxDr = cms.double(9999.0)
)


hltL1Mu30L1Filtered30 = cms.EDFilter("HLTMuonL1Filter",
    PreviousCandTag = cms.InputTag("hltL1sL1SingleMu20"),
    MinPt = cms.double(30.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    SaveTag = cms.untracked.bool(True),
    SelectQualities = cms.vint32(),
    CandTag = cms.InputTag("hltL1extraParticles")
)


hltSingleMu9L3Filtered9 = cms.EDFilter("HLTMuonL3PreFilter",
    PreviousCandTag = cms.InputTag("hltSingleMu9L2Filtered7"),
    MinPt = cms.double(9.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    MinNhits = cms.int32(0),
    SaveTag = cms.untracked.bool(True),
    NSigmaPt = cms.double(0.0),
    MaxDz = cms.double(9999.0),
    BeamSpotTag = cms.InputTag("hltOfflineBeamSpot"),
    CandTag = cms.InputTag("hltL3MuonCandidates"),
    MaxDr = cms.double(2.0)
)


hltPreTechTrigHCALNoise = cms.EDFilter("HLTPrescaler")


hltL1NonIsoHLTNonIsoSingleElectronEt15LTIESscWrapper = cms.EDFilter("HLTEgammaTriggerFilterObjectWrapper",
    doIsolated = cms.bool(False),
    candIsolatedTag = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    candNonIsolatedTag = cms.InputTag("hltL1NonIsoRecoEcalCandidate")
)


hltPreL1Mu = cms.EDFilter("HLTPrescaler")


hltL1sHcalNZS1E31 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_ZeroBias OR L1_MinBias_HTT10 OR L1_SingleJet15 OR L1_SingleJet30 OR L1_SingleJet50 OR L1_SingleJet70 OR L1_SingleJet100 OR L1_SingleTauJet30 OR L1_SingleTauJet40 OR L1_SingleTauJet60 OR L1_SingleTauJet80 OR L1_SingleIsoEG5 OR L1_SingleIsoEG8 OR L1_SingleIsoEG10 OR L1_SingleIsoEG15 OR L1_SingleEG1 OR L1_SingleEG2 OR L1_SingleEG5 OR L1_SingleEG8 OR L1_SingleEG10 OR L1_SingleEG12 OR L1_SingleEG15 OR L1_SingleEG20 OR L1_SingleMuOpen OR L1_SingleMu0 OR L1_SingleMu3 OR L1_SingleMu5 OR L1_SingleMu7 OR L1_SingleMu10 OR L1_SingleMu14 OR L1_SingleMu20'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltHIPreJet50U = cms.EDFilter("HLTPrescaler")


hltPreL1SingleEG8 = cms.EDFilter("HLTPrescaler")


hltPreL1SingleEG5 = cms.EDFilter("HLTPrescaler")


hltPreL1SingleEG2 = cms.EDFilter("HLTPrescaler")


hltPreJet180 = cms.EDFilter("HLTPrescaler")


hltL1sL1BSC = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('36 OR 37 OR 38 OR 39'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(True)
)


hltL1NonIsoHLTNonIsoSinglePhotonEt20EtFilter = cms.EDFilter("HLTEgammaEtFilter",
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    inputTag = cms.InputTag("hltL1NonIsoHLTNonIsoSinglePhotonEt20L1MatchFilterRegional"),
    etcutEB = cms.double(20.0),
    etcutEE = cms.double(20.0),
    ncandcut = cms.int32(1)
)


hltPreJet30 = cms.EDFilter("HLTPrescaler")


hltPreDoubleFwdJet50 = cms.EDFilter("HLTPrescaler")


hltPreL2Mu15 = cms.EDFilter("HLTPrescaler")


hltPreDoubleIsoMu3 = cms.EDFilter("HLTPrescaler")


hltDiJetAve15U8E29 = cms.EDFilter("HLTDiJetAveFilter",
    inputJetTag = cms.InputTag("hltIterativeCone5CaloJets"),
    minPtAve = cms.double(15.0),
    minPtJet3 = cms.double(99999.0),
    saveTag = cms.untracked.bool(True),
    minDphi = cms.double(-1.0)
)


hltPreL2Mu11 = cms.EDFilter("HLTPrescaler")


hltMuNoIsoJetsL3PreFiltered = cms.EDFilter("HLTMuonL3PreFilter",
    PreviousCandTag = cms.InputTag("hltMuNoIsoJetsL2PreFiltered"),
    MinPt = cms.double(14.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    MinNhits = cms.int32(0),
    SaveTag = cms.untracked.bool(True),
    NSigmaPt = cms.double(0.0),
    MaxDz = cms.double(9999.0),
    BeamSpotTag = cms.InputTag("hltOfflineBeamSpot"),
    CandTag = cms.InputTag("hltL3MuonCandidates"),
    MaxDr = cms.double(2.0)
)


hltPreCSCBeamHaloRing2or3 = cms.EDFilter("HLTPrescaler")


hltCalibrationEventsFilter = cms.EDFilter("HLTTriggerTypeFilter",
    SelectedTriggerType = cms.int32(2)
)


hltPreActivityPixelClusters = cms.EDFilter("HLTPrescaler")


hltL1EventNumber = cms.EDFilter("HLTL1NumberFilter",
    invert = cms.bool(True),
    period = cms.uint32(4096),
    rawInput = cms.InputTag("source")
)


hltL1NonIsoHLTLEITISinglePhotonEt20L1MatchFilterRegional = cms.EDFilter("HLTEgammaL1MatchFilterRegional",
    doIsolated = cms.bool(False),
    endcap_end = cms.double(2.65),
    candNonIsolatedTag = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    region_eta_size_ecap = cms.double(1.0),
    barrel_end = cms.double(1.4791),
    l1IsolatedTag = cms.InputTag("hltL1extraParticles","Isolated"),
    candIsolatedTag = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    region_phi_size = cms.double(1.044),
    region_eta_size = cms.double(0.522),
    L1SeedFilterTag = cms.InputTag("hltL1sL1SingleEG8"),
    ncandcut = cms.int32(1),
    l1NonIsolatedTag = cms.InputTag("hltL1extraParticles","NonIsolated")
)


hltSameSignMuL1Filtered = cms.EDFilter("HLTMuonL1Filter",
    PreviousCandTag = cms.InputTag("hltL1sL1DoubleMu3"),
    MinPt = cms.double(0.0),
    MinN = cms.int32(2),
    MaxEta = cms.double(2.5),
    SelectQualities = cms.vint32(),
    CandTag = cms.InputTag("hltL1extraParticles")
)


hltPreRPCMuonNorma = cms.EDFilter("HLTPrescaler")


hltHIMML3Filter = cms.EDFilter("HLTHIMuL1L2L3Filter",
    NavigationPSet = cms.PSet(
        ComponentName = cms.string('SimpleNavigationSchool')
    ),
    PrimaryVertexTag = cms.InputTag("hltHIPixelVertices"),
    L2CandTag = cms.InputTag("hltL2MuonCandidates"),
    rphiRecHits = cms.InputTag("siStripMatchedRecHits","rphiRecHit"),
    TTRHBuilder = cms.string('HITTRHBuilderWithoutRefit')
)


hltPreEle20SWL1R = cms.EDFilter("HLTPrescaler")


hltL1sZeroBias = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('4'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(True)
)


hltIsolPixelTrackL3FilterHB8E29 = cms.EDFilter("HLTPixelIsolTrackFilter",
    MaxPtNearby = cms.double(2.0),
    MinEtaTrack = cms.double(0.0),
    MinEnergyTrack = cms.double(20.0),
    MinPtTrack = cms.double(20.0),
    DropMultiL2Event = cms.bool(False),
    NMaxTrackCandidates = cms.int32(15),
    MaxEtaTrack = cms.double(2.0),
    candTag = cms.InputTag("hltHITIPTCorrectorHB8E29"),
    filterTrackEnergy = cms.bool(True)
)


hlt1jet180regional = cms.EDFilter("HLT1CaloJet",
    MaxEta = cms.double(5.0),
    saveTag = cms.untracked.bool(True),
    inputTag = cms.InputTag("hltMCJetCorJetIcone5Regional"),
    MinPt = cms.double(180.0),
    MinN = cms.int32(1)
)


hltZMML2Filtered = cms.EDFilter("HLTMuonDimuonL2Filter",
    FastAccept = cms.bool(False),
    MinPtMax = cms.double(7.0),
    PreviousCandTag = cms.InputTag("hltZMML1Filtered"),
    ChargeOpt = cms.int32(0),
    MaxPtBalance = cms.double(999999.0),
    MaxDz = cms.double(9999.0),
    MaxAcop = cms.double(3.15),
    MinNhits = cms.int32(0),
    MinPtMin = cms.double(7.0),
    NSigmaPt = cms.double(0.0),
    MinPtBalance = cms.double(-1.0),
    MaxInvMass = cms.double(9999.0),
    BeamSpotTag = cms.InputTag("hltOfflineBeamSpot"),
    MinPtPair = cms.double(0.0),
    CandTag = cms.InputTag("hltL2MuonCandidates"),
    MaxDr = cms.double(100.0),
    MinInvMass = cms.double(0.0),
    MaxEta = cms.double(2.5),
    MinAcop = cms.double(-1.0)
)


hltL1NonIsoHLTNonIsoSingleElectronEt10L1MatchFilterRegional = cms.EDFilter("HLTEgammaL1MatchFilterRegional",
    doIsolated = cms.bool(False),
    endcap_end = cms.double(2.65),
    candNonIsolatedTag = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    region_eta_size_ecap = cms.double(1.0),
    barrel_end = cms.double(1.4791),
    l1IsolatedTag = cms.InputTag("hltL1extraParticles","Isolated"),
    candIsolatedTag = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    region_phi_size = cms.double(1.044),
    region_eta_size = cms.double(0.522),
    L1SeedFilterTag = cms.InputTag("hltL1sL1SingleEG5"),
    ncandcut = cms.int32(1),
    l1NonIsolatedTag = cms.InputTag("hltL1extraParticles","NonIsolated")
)


hltPreAlCaEcalEta1E31 = cms.EDFilter("HLTPrescaler")


hltMinBiasPixelFilter2 = cms.EDFilter("HLTPixlMBFilt",
    pixlTag = cms.InputTag("hltPixelCands"),
    MinTrks = cms.uint32(2),
    MinPt = cms.double(0.0),
    MinSep = cms.double(1.0)
)


hltMinBiasPixelFilter1 = cms.EDFilter("HLTPixlMBFilt",
    pixlTag = cms.InputTag("hltPixelCands"),
    MinTrks = cms.uint32(1),
    MinPt = cms.double(0.0),
    MinSep = cms.double(1.0)
)


hltMuonTauIsoL3IsoFilteredNoL1Tau = cms.EDFilter("HLTMuonIsoFilter",
    PreviousCandTag = cms.InputTag("hltMuonTauIsoL3PreFilteredNoL1Tau"),
    MinN = cms.int32(1),
    SaveTag = cms.untracked.bool(True),
    CandTag = cms.InputTag("hltL3MuonCandidates"),
    IsolatorPSet = cms.PSet(

    ),
    DepTag = cms.VInputTag("hltL3MuonIsolations")
)


hltHI1jet50U = cms.EDFilter("HLT1CaloJet",
    MaxEta = cms.double(5.0),
    saveTag = cms.untracked.bool(True),
    inputTag = cms.InputTag("hltIterativeCone5PileupSubtractionCaloJets"),
    MinPt = cms.double(50.0),
    MinN = cms.int32(1)
)


hlt1jet30U = cms.EDFilter("HLT1CaloJet",
    MaxEta = cms.double(5.0),
    saveTag = cms.untracked.bool(True),
    inputTag = cms.InputTag("hltMCJetCorJetIcone5HF07"),
    MinPt = cms.double(30.0),
    MinN = cms.int32(1)
)


hltPreNoL2IsoMu8Jet40 = cms.EDFilter("HLTPrescaler")


hltMuJetsL3IsoFiltered = cms.EDFilter("HLTMuonIsoFilter",
    PreviousCandTag = cms.InputTag("hltMuJetsL3PreFiltered"),
    MinN = cms.int32(1),
    SaveTag = cms.untracked.bool(True),
    CandTag = cms.InputTag("hltL3MuonCandidates"),
    IsolatorPSet = cms.PSet(

    ),
    DepTag = cms.VInputTag("hltL3MuonIsolations")
)


hltL1EventNumberNZS = cms.EDFilter("HLTL1NumberFilter",
    invert = cms.bool(False),
    period = cms.uint32(4096),
    rawInput = cms.InputTag("source")
)


hltL1sL1Mu3EG5 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_Mu3_EG5'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltPreJet60MET70Aco = cms.EDFilter("HLTPrescaler")


hltPreDoubleEle10SWL1R = cms.EDFilter("HLTPrescaler")


hltMuJetsHLT1jet50 = cms.EDFilter("HLT1CaloJet",
    MaxEta = cms.double(5.0),
    saveTag = cms.untracked.bool(True),
    inputTag = cms.InputTag("hltMCJetCorJetIcone5"),
    MinPt = cms.double(50.0),
    MinN = cms.int32(1)
)


hltL1NonIsoHLTNonIsoSingleElectronEt20PixelMatchFilter = cms.EDFilter("HLTElectronPixelMatchFilter",
    doIsolated = cms.bool(False),
    L1IsoPixelSeedsTag = cms.InputTag("hltL1IsoStartUpElectronPixelSeeds"),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    L1NonIsoPixelSeedsTag = cms.InputTag("hltL1NonIsoStartUpElectronPixelSeeds"),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    SaveTag = cms.untracked.bool(True),
    npixelmatchcut = cms.double(1.0),
    ncandcut = cms.int32(1),
    candTag = cms.InputTag("hltL1NonIsoHLTNonIsoSingleElectronEt20HcalIsolFilter")
)


hltL1sForwardBSC = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('36 OR 37'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(True)
)


hltPreDoublePhoton10L1R = cms.EDFilter("HLTPrescaler")


hltPreMET60 = cms.EDFilter("HLTPrescaler")


hltL1NonIsoSinglePhotonEt15HTIL1MatchFilterRegional = cms.EDFilter("HLTEgammaL1MatchFilterRegional",
    doIsolated = cms.bool(False),
    endcap_end = cms.double(2.65),
    candNonIsolatedTag = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    region_eta_size_ecap = cms.double(1.0),
    barrel_end = cms.double(1.4791),
    l1IsolatedTag = cms.InputTag("hltL1extraParticles","Isolated"),
    candIsolatedTag = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    region_phi_size = cms.double(1.044),
    region_eta_size = cms.double(0.522),
    L1SeedFilterTag = cms.InputTag("hltL1sL1SingleEG8"),
    ncandcut = cms.int32(1),
    l1NonIsolatedTag = cms.InputTag("hltL1extraParticles","NonIsolated")
)


hltL1sQuadJet30 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_QuadJet15'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltPreFwdJet20U = cms.EDFilter("HLTPrescaler")


hltPreBTagIPJet80 = cms.EDFilter("HLTPrescaler")


hltL1sL1SingleMu20 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleMu20'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltMuJetsL2PreFiltered = cms.EDFilter("HLTMuonL2PreFilter",
    PreviousCandTag = cms.InputTag("hltMuJetsL1Filtered"),
    MinPt = cms.double(5.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    MinNhits = cms.int32(0),
    NSigmaPt = cms.double(0.0),
    MaxDz = cms.double(9999.0),
    BeamSpotTag = cms.InputTag("hltOfflineBeamSpot"),
    CandTag = cms.InputTag("hltL2MuonCandidates"),
    MaxDr = cms.double(9999.0)
)


hltL1sJet50 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleJet30'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltL2Filtered7L2Mu7Photon9L1R = cms.EDFilter("HLTMuonL2PreFilter",
    PreviousCandTag = cms.InputTag("hltL1Mu3EG5L1Filtered0"),
    MinPt = cms.double(7.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    MinNhits = cms.int32(0),
    SaveTag = cms.untracked.bool(True),
    NSigmaPt = cms.double(0.0),
    MaxDz = cms.double(9999.0),
    BeamSpotTag = cms.InputTag("hltOfflineBeamSpot"),
    CandTag = cms.InputTag("hltL2MuonCandidates"),
    MaxDr = cms.double(9999.0)
)


hltL1sHcalNZS8E29 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleEG1 OR L1_SingleEG2 OR L1_SingleEG5 OR L1_SingleEG8 OR L1_SingleEG10 OR L1_SingleEG12 OR L1_SingleEG15 OR L1_SingleEG20 OR L1_SingleIsoEG5 OR L1_SingleIsoEG8 OR L1_SingleIsoEG10 OR L1_SingleIsoEG12 OR L1_SingleIsoEG15 OR L1_SingleJet6U OR L1_SingleJet10U OR L1_SingleJet20U OR L1_SingleJet30U OR L1_SingleJet40U OR L1_SingleJet50U OR L1_SingleJet60U OR L1_SingleTauJet10U OR L1_SingleTauJet20U OR L1_SingleTauJet30U OR L1_SingleTauJet50U OR L1_SingleMuOpen OR L1_SingleMu0 OR L1_SingleMu3 OR L1_SingleMu5 OR L1_SingleMu7 OR L1_SingleMu10 OR L1_SingleMu14 OR L1_SingleMu20 OR L1_MinBias_HTT10 OR L1_ZeroBias'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltMuonTauIsoL3PreFilteredNoL1Tau = cms.EDFilter("HLTMuonL3PreFilter",
    PreviousCandTag = cms.InputTag("hltMuonTauIsoL2IsoFilteredNoL1Tau"),
    MinPt = cms.double(14.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    MinNhits = cms.int32(0),
    NSigmaPt = cms.double(0.0),
    MaxDz = cms.double(9999.0),
    BeamSpotTag = cms.InputTag("hltOfflineBeamSpot"),
    CandTag = cms.InputTag("hltL3MuonCandidates"),
    MaxDr = cms.double(2.0)
)


hltPreSingleIsoTau30Trk5 = cms.EDFilter("HLTPrescaler")


hltFilterL25LeadingTrackPtCutSingleIsoTau30Trk5 = cms.EDFilter("HLT1Tau",
    MaxEta = cms.double(5.0),
    inputTag = cms.InputTag("hltL25TauLeadingTrackPtCutSelector"),
    MinPt = cms.double(30.0),
    MinN = cms.int32(1)
)


hltPreHighMult40 = cms.EDFilter("HLTPrescaler")


hltSingleMu15L2PreFiltered12 = cms.EDFilter("HLTMuonL2PreFilter",
    PreviousCandTag = cms.InputTag("hltSingleMu15L1Filtered0"),
    MinPt = cms.double(12.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    MinNhits = cms.int32(0),
    NSigmaPt = cms.double(0.0),
    MaxDz = cms.double(9999.0),
    BeamSpotTag = cms.InputTag("hltOfflineBeamSpot"),
    CandTag = cms.InputTag("hltL2MuonCandidates"),
    MaxDr = cms.double(9999.0)
)


hltL1Mu20L1Filtered20 = cms.EDFilter("HLTMuonL1Filter",
    PreviousCandTag = cms.InputTag("hltL1sL1SingleMu20"),
    MinPt = cms.double(20.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    SaveTag = cms.untracked.bool(True),
    SelectQualities = cms.vint32(),
    CandTag = cms.InputTag("hltL1extraParticles")
)


hltL1NonIsoHLTNonIsoSinglePhotonEt30L1MatchFilterRegional = cms.EDFilter("HLTEgammaL1MatchFilterRegional",
    doIsolated = cms.bool(False),
    endcap_end = cms.double(2.65),
    candNonIsolatedTag = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    region_eta_size_ecap = cms.double(1.0),
    barrel_end = cms.double(1.4791),
    l1IsolatedTag = cms.InputTag("hltL1extraParticles","Isolated"),
    candIsolatedTag = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    region_phi_size = cms.double(1.044),
    region_eta_size = cms.double(0.522),
    L1SeedFilterTag = cms.InputTag("hltL1sL1SingleEG8"),
    ncandcut = cms.int32(1),
    l1NonIsolatedTag = cms.InputTag("hltL1extraParticles","NonIsolated")
)


hltSingleMu11L2Filtered9 = cms.EDFilter("HLTMuonL2PreFilter",
    PreviousCandTag = cms.InputTag("hltL1SingleMu7L1Filtered0"),
    MinPt = cms.double(9.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    MinNhits = cms.int32(0),
    NSigmaPt = cms.double(0.0),
    MaxDz = cms.double(9999.0),
    BeamSpotTag = cms.InputTag("hltOfflineBeamSpot"),
    CandTag = cms.InputTag("hltL2MuonCandidates"),
    MaxDr = cms.double(9999.0)
)


hltPreDoubleJet60MET60Aco = cms.EDFilter("HLTPrescaler")


hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdOneOEMinusOneOPFilter = cms.EDFilter("HLTElectronOneOEMinusOneOPFilterRegional",
    doIsolated = cms.bool(False),
    electronNonIsolatedProducer = cms.InputTag("hltPixelMatchLargeWindowElectronsL1NonIso"),
    barrelcut = cms.double(999.9),
    electronIsolatedProducer = cms.InputTag("hltPixelMatchLargeWindowElectronsL1Iso"),
    ncandcut = cms.int32(1),
    candTag = cms.InputTag("hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdPixelMatchFilter"),
    endcapcut = cms.double(999.9)
)


hltPreDoubleFwdJet40MET60 = cms.EDFilter("HLTPrescaler")


hltL1MuL1Filtered0 = cms.EDFilter("HLTMuonL1Filter",
    PreviousCandTag = cms.InputTag("hltL1sL1Mu"),
    MinPt = cms.double(0.0),
    MinN = cms.int32(1),
    MaxEta = cms.double(2.5),
    SaveTag = cms.untracked.bool(True),
    SelectQualities = cms.vint32(),
    CandTag = cms.InputTag("hltL1extraParticles")
)


hltL1sL1SingleMu5 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleMu5'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltL1sDoubleJet150 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_SingleJet150 OR L1_DoubleJet70'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltPreSumET120 = cms.EDFilter("HLTPrescaler")


hltL1NonIsoSinglePhotonEt15LEIEcalIsolFilter = cms.EDFilter("HLTEgammaGenericFilter",
    doIsolated = cms.bool(False),
    thrOverE2EE = cms.double(-1.0),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    thrOverE2EB = cms.double(-1.0),
    thrRegularEE = cms.double(3.0),
    thrOverEEE = cms.double(0.1),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    thrOverEEB = cms.double(0.1),
    thrRegularEB = cms.double(3.0),
    lessThan = cms.bool(True),
    useEt = cms.bool(False),
    ncandcut = cms.int32(1),
    isoTag = cms.InputTag("hltL1IsolatedPhotonEcalIsol"),
    candTag = cms.InputTag("hltL1NonIsoSinglePhotonEt15LEIEtFilter"),
    nonIsoTag = cms.InputTag("hltL1NonIsolatedPhotonEcalIsol")
)


hltLevel1Activity = cms.EDFilter("HLTLevel1Activity",
    technicalBits = cms.uint64(8070468124417203968),
    ignoreL1Mask = cms.bool(False),
    physicsLoBits = cms.uint64(9214363754198712304),
    physicsHiBits = cms.uint64(4546451697224322038),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    bunchCrossings = cms.vint32(0, 1, -1, 2, -2)
)


hltPreCSCBeamHalo = cms.EDFilter("HLTPrescaler")


hltL1sFwdJet40 = cms.EDFilter("HLTLevel1GTSeed",
    L1SeedsLogicalExpression = cms.string('L1_IsoEG10_Jet15_ForJet10'),
    L1MuonCollectionTag = cms.InputTag("hltL1extraParticles"),
    L1UseL1TriggerObjectMaps = cms.bool(True),
    L1UseAliasesForSeeding = cms.bool(True),
    L1GtReadoutRecordTag = cms.InputTag("hltGtDigis"),
    L1CollectionsTag = cms.InputTag("hltL1extraParticles"),
    L1NrBxInEvent = cms.int32(5),
    L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
    L1TechTriggerSeeding = cms.bool(False)
)


hltPreQuadJet60 = cms.EDFilter("HLTPrescaler")


hltL1NonIsoDoublePhotonEt5UpsPMMassFilter = cms.EDFilter("HLTPMMassFilter",
    lowerMassCut = cms.double(8.0),
    L1NonIsoCand = cms.InputTag("hltL1NonIsoRecoEcalCandidate"),
    L1IsoCand = cms.InputTag("hltL1IsoRecoEcalCandidate"),
    isElectron1 = cms.untracked.bool(False),
    SaveTag = cms.untracked.bool(True),
    isElectron2 = cms.untracked.bool(False),
    upperMassCut = cms.double(11.0),
    candTag = cms.InputTag("hltL1NonIsoDoublePhotonEt5UpsHcalIsolFilter"),
    nZcandcut = cms.int32(1),
    reqOppCharge = cms.untracked.bool(True)
)


hltPreL1Mu14L1ETM40 = cms.EDFilter("HLTPrescaler")


hltL1GtTrigReport = cms.EDAnalyzer("L1GtTrigReport",
    UseL1GlobalTriggerRecord = cms.bool(False),
    L1GtRecordInputTag = cms.InputTag("hltGtDigis")
)


hltGetRaw = cms.EDAnalyzer("HLTGetRaw",
    RawDataCollection = cms.InputTag("source")
)


hltTrigReport = cms.EDAnalyzer("HLTrigReport",
    HLTriggerResults = cms.InputTag("TriggerResults","","HLT")
)




HLTDoLocalHcalWithoutHOSequence = cms.Sequence(hltHcalDigis+hltHbhereco+hltHfreco)


HLTDoLocalPixelSequence = cms.Sequence(hltSiPixelDigis+hltSiPixelClusters+hltSiPixelRecHits)


HLTL25TauTrackIsolationSequence = cms.Sequence(hltL25TauJetTracksAssociator+hltL25TauConeIsolation+hltL25TauLeadingTrackPtCutSelector)


HLTL1IsoEgammaRegionalRecoTrackerSequence = cms.Sequence(hltL1IsoEgammaRegionalPixelSeedGenerator+hltL1IsoEgammaRegionalCkfTrackCandidates+hltL1IsoEgammaRegionalCTFFinalFitWithMaterial)


HLTL3TauTrackIsolationSequence = cms.Sequence(hltL3TauJetTracksAssociator+hltL3TauConeIsolation+hltL3TauIsolationSelector)


HLTL1IsoElectronsRegionalRecoTrackerSequence = cms.Sequence(hltL1IsoElectronsRegionalPixelSeedGenerator+hltL1IsoElectronsRegionalCkfTrackCandidates+hltL1IsoElectronsRegionalCTFFinalFitWithMaterial)


HLTPixelMatchLargeWindowElectronL1NonIsoTrackingSequence = cms.Sequence(hltCkfL1NonIsoLargeWindowTrackCandidates+hltCtfL1NonIsoLargeWindowWithMaterialTracks+hltPixelMatchLargeWindowElectronsL1NonIso)


HLTDoHIEcalClusSequence = cms.Sequence(hltIslandBasicClustersHI+hltIslandSuperClustersHI+hltCorrectedIslandBarrelSuperClustersHI+hltCorrectedIslandEndcapSuperClustersHI+hltRecoHIEcalCandidate)


HLTDoRegionalJetEcalSequence = cms.Sequence(hltEcalRawToRecHitFacility+hltEcalRegionalJetsFEDs+hltEcalRegionalJetsRecHit)


HLTDoJet30HTRecoSequence = cms.Sequence(hltJet30Ht)


SimL1MuTriggerPrimitives = cms.Sequence(simDtTriggerPrimitiveDigis+simCscTriggerPrimitiveDigis)


DoLWTracking = cms.Sequence(hltCkfL1IsoLWTC+hltCtfL1IsoLW+hltPixelMatchElectronsL1IsoLW+hltCkfL1NonIsoLWTC+hltCtfL1NonIsoLW+hltPixelMatchElectronsL1NonIsoLW+hltL1IsoLWEleRegPSG+hltL1IsoLWEleRegioCkfTC+hltL1IsoLWEleRegioCTF+hltL1IsoLWEleTrackIsol+hltL1NonIsoLWEleRegPSG+hltL1NonIsoLWEleRegioCkfTC+hltL1NonIsoLWEleRegioCTF+hltL1NonIsoLWEleTrackIsol)


DoSSTracking = cms.Sequence(hltCkfL1IsoSSTC+hltCtfL1IsoSS+hltPixelMatchElectronsL1IsoSS+hltCkfL1NonIsoSSTC+hltCtfL1NonIsoSS+hltPixelMatchElectronsL1NonIsoSS+hltL1IsoSSEleRegPSG+hltL1IsoSSEleRegioCkfTC+hltL1IsoSSEleRegioCTF+hltL1IsoSSEleTrackIsol+hltL1NonIsoSSEleRegPSG+hltL1NonIsoSSEleRegioCkfTC+hltL1NonIsoSSEleRegioCTF+hltL1NonIsoSSEleTrackIsol)


OpenHLTL25TauTrackIsolation = cms.Sequence(openhltL25TauJetTracksAssociator+openhltL25TauConeIsolation)


HLTL2HcalIsolTrackSequenceHE = cms.Sequence(HLTDoLocalPixelSequence+hltHITPixelTracksHB+hltHITPixelTracksHE+hltHITPixelVerticesHE)


HLTL2HcalIsolTrackSequenceHB = cms.Sequence(HLTDoLocalPixelSequence+hltHITPixelTracksHB+hltHITPixelVerticesHB)


HLTMulti5x5SuperClusterL1Isolated = cms.Sequence(hltMulti5x5BasicClustersL1Isolated+hltMulti5x5SuperClustersL1Isolated+hltMulti5x5EndcapSuperClustersWithPreshowerL1Isolated+hltCorrectedMulti5x5EndcapSuperClustersWithPreshowerL1Isolated)


HLTL3TauTrackReconstructionSequence = cms.Sequence(hltL3TauPixelSeeds+hltL3TauCkfTrackCandidates+hltL3TauCtfWithMaterialTracks)


HLTDoHTRecoSequence = cms.Sequence(hltHtMet)


HLTL1NonIsoElectronsRegionalRecoTrackerSequence = cms.Sequence(hltL1NonIsoElectronsRegionalPixelSeedGenerator+hltL1NonIsoElectronsRegionalCkfTrackCandidates+hltL1NonIsoElectronsRegionalCTFFinalFitWithMaterial)


HLTHIRecopixelvertexingSequence = cms.Sequence(hltHIPixelTracks+hltHIPixelVertices)


HLTDoRegionalPi0EtaSequence = cms.Sequence(hltESRawToRecHitFacility+hltEcalRawToRecHitFacility+hltEcalRegionalPi0EtaFEDs+hltESRegionalPi0EtaRecHit+hltEcalRegionalPi0EtaRecHit)


SimL1TechnicalTriggers = cms.Sequence(simBscDigis+simRpcTechTrigDigis)


HLTDoLocalHcalSequence = cms.Sequence(hltHcalDigis+hltHbhereco+hltHfreco+hltHoreco)


HLTRecopixelvertexingForMinBiasSequence = cms.Sequence(hltPixelTracksForMinBias+hltPixelVerticesForMinBias)


HLTDoRegionalEgammaEcalSequence = cms.Sequence(hltESRawToRecHitFacility+hltEcalRawToRecHitFacility+hltEcalRegionalEgammaFEDs+hltEcalRegionalEgammaRecHit+hltESRegionalEgammaRecHit)


HLTL2TauEcalIsolationSequence = cms.Sequence(hltL2TauNarrowConeIsolationProducer+hltL2TauRelaxingIsolationSelector)


HLTCosmicMuonReco = cms.Sequence(hltMuonDTDigis+hltDt1DRecHits+hltDt4DSegments+hltMuonCSCDigis+hltCsc2DRecHits+hltCscSegments+hltMuonRPCDigis+hltRpcRecHits+hltCosmicMuonSeedBarrelOnly+hltCosmicMuonBarrelOnly)


HLTEndSequence = cms.Sequence(hltBoolEnd)


HLTMulti5x5SuperClusterL1NonIsolated = cms.Sequence(hltMulti5x5BasicClustersL1NonIsolated+hltMulti5x5SuperClustersL1NonIsolated+hltMulti5x5EndcapSuperClustersWithPreshowerL1NonIsolated+hltCorrectedMulti5x5EndcapSuperClustersWithPreshowerL1NonIsolatedTemp+hltCorrectedMulti5x5EndcapSuperClustersWithPreshowerL1NonIsolated)


HLTDoLocalStripSequence = cms.Sequence(hltSiStripRawToClustersFacility+hltSiStripClusters)


HLTRecoJetRegionalSequence = cms.Sequence(HLTDoRegionalJetEcalSequence+HLTDoLocalHcalSequence+hltTowerMakerForJets+hltIterativeCone5CaloJetsRegional+hltMCJetCorJetIcone5Regional)


HLTRecopixelvertexingSequence = cms.Sequence(hltPixelTracks+hltPixelVertices)


HLTBeginSequenceBPTX = cms.Sequence(hltTriggerType+hltL1EventNumber+hltGtDigis+hltGctDigis+hltL1GtObjectMap+hltL1extraParticles+hltBPTXCoincidence+hltOfflineBeamSpot)


HLTBeginSequence = cms.Sequence(hltTriggerType+hltL1EventNumber+hltGtDigis+hltGctDigis+hltL1GtObjectMap+hltL1extraParticles+hltOfflineBeamSpot)


HLTPixelMatchLargeWindowElectronL1IsoTrackingSequence = cms.Sequence(hltCkfL1IsoLargeWindowTrackCandidates+hltCtfL1IsoLargeWindowWithMaterialTracks+hltPixelMatchLargeWindowElectronsL1Iso)


HLTL25TauTrackReconstructionSequence = cms.Sequence(HLTDoLocalStripSequence+hltL25TauPixelSeeds+hltL25TauCkfTrackCandidates+hltL25TauCtfWithMaterialTracks)


HLTDoLocalTrackerSequence = cms.Sequence(HLTDoLocalPixelSequence+HLTDoLocalStripSequence)


HLTL3muonisorecoSequence = cms.Sequence(hltPixelTracks+hltL3MuonIsolations)


HLTPixelMatchElectronL1NonIsoTrackingSequence = cms.Sequence(hltCkfL1NonIsoTrackCandidates+hltCtfL1NonIsoWithMaterialTracks+hltPixelMatchElectronsL1NonIso)


HLTBTagIPSequenceL25StartupU = cms.Sequence(HLTDoLocalPixelSequence+HLTRecopixelvertexingSequence+hltSelector4JetsU+hltBLifetimeL25JetsStartupU+hltBLifetimeL25AssociatorStartupU+hltBLifetimeL25TagInfosStartupU+hltBLifetimeL25BJetTagsStartupU)


HLTL1NonIsoEgammaRegionalRecoTrackerSequence = cms.Sequence(hltL1NonIsoEgammaRegionalPixelSeedGenerator+hltL1NonIsoEgammaRegionalCkfTrackCandidates+hltL1NonIsoEgammaRegionalCTFFinalFitWithMaterial)


HLTPixelTrackingForMinBiasSequence = cms.Sequence(hltPixelTracksForMinBias)


HLTL1NonIsolatedEcalClustersSequence = cms.Sequence(hltHybridSuperClustersL1NonIsolated+hltCorrectedHybridSuperClustersL1NonIsolatedTemp+hltCorrectedHybridSuperClustersL1NonIsolated+HLTMulti5x5SuperClusterL1NonIsolated)


HLTL2muonisorecoSequence = cms.Sequence(hltEcalRawToRecHitFacility+hltEcalRegionalMuonsFEDs+hltEcalRegionalMuonsRecHit+HLTDoLocalHcalSequence+hltTowerMakerForMuons+hltL2MuonIsolations)


HLTL25TauPixelTrackIsolation = cms.Sequence(HLTDoLocalPixelSequence+HLTRecopixelvertexingSequence+hltL25TauJetPixelTracksAssociator+hltL25TauPixelTracksConeIsolation+hltL25TauPixelTracksIsolationSelector+hltL25TauPixelTracksLeadingTrackPtCutSelector)


HLTCaloTausCreatorRegionalSequence = cms.Sequence(HLTDoRegionalJetEcalSequence+HLTDoLocalHcalSequence+hltTowerMakerForJets+hltCaloTowersTau1Regional+hltIconeTau1Regional+hltCaloTowersTau2Regional+hltIconeTau2Regional+hltCaloTowersTau3Regional+hltIconeTau3Regional+hltCaloTowersTau4Regional+hltIconeTau4Regional+hltCaloTowersCentral1Regional+hltIconeCentral1Regional+hltCaloTowersCentral2Regional+hltIconeCentral2Regional+hltCaloTowersCentral3Regional+hltIconeCentral3Regional+hltCaloTowersCentral4Regional+hltIconeCentral4Regional)


HLTOniaPixelTrackSequence = cms.Sequence(HLTDoLocalPixelSequence+hltPixelTracks+hltPixelTrackCands)


HLTOniaTrackSequence = cms.Sequence(HLTDoLocalStripSequence+hltOniaSeeds+hltOniaCkfTrackCandidates+hltOniaCtfTracks+hltOniaCtfTrackCands)


HLTmuonlocalrecoSequence = cms.Sequence(hltMuonDTDigis+hltDt1DRecHits+hltDt4DSegments+hltMuonCSCDigis+hltCsc2DRecHits+hltCscSegments+hltMuonRPCDigis+hltRpcRecHits)


HLTDoJetRecoSequence = cms.Sequence(hltIterativeCone5CaloJets+hltMCJetCorJetIcone5)


HLTDoJet15UHTRecoSequence = cms.Sequence(hltJet15UHt)


OpenHLTBLifetimeL25recoSequence = cms.Sequence(HLTDoLocalPixelSequence+HLTRecopixelvertexingSequence+openHltBLifetimeL25Associator+openHltBLifetimeL25TagInfos+openHltBLifetimeL25BJetTags)


HLTPixelMatchElectronL1IsoTrackingSequence = cms.Sequence(hltCkfL1IsoTrackCandidates+hltCtfL1IsoWithMaterialTracks+hltPixelMatchElectronsL1Iso)


HLTBLifetimeL25recoSequence = cms.Sequence(hltBLifetimeHighestEtJets+hltBLifetimeL25Jets+HLTDoLocalPixelSequence+HLTRecopixelvertexingSequence+hltBLifetimeL25Associator+hltBLifetimeL25TagInfos+hltBLifetimeL25BJetTags)


HLTBeginSequenceNZS = cms.Sequence(hltTriggerType+hltL1EventNumberNZS+hltGtDigis+hltGctDigis+hltL1GtObjectMap+hltL1extraParticles+hltBPTXCoincidence+hltOfflineBeamSpot)


HLTL2TauJetsSequence = cms.Sequence(HLTCaloTausCreatorRegionalSequence+hltL2TauJets)


SimL1MuTrackFinders = cms.Sequence(simCsctfTrackDigis*simDttfDigis*simCsctfDigis)


OpenHLTBLifetimeL3recoSequenceStartup = cms.Sequence(HLTDoLocalPixelSequence+HLTDoLocalStripSequence+openHltBLifetimeRegionalPixelSeedGeneratorStartup+openHltBLifetimeRegionalCkfTrackCandidatesStartup+openHltBLifetimeRegionalCtfWithMaterialTracksStartup+openHltBLifetimeL3AssociatorStartup+openHltBLifetimeL3TagInfosStartup+openHltBLifetimeL3BJetTagsStartup)


HLTBTagIPSequenceL3Startup = cms.Sequence(HLTDoLocalPixelSequence+HLTDoLocalStripSequence+hltBLifetimeL3JetsStartup+hltBLifetimeRegionalPixelSeedGeneratorStartup+hltBLifetimeRegionalCkfTrackCandidatesStartup+hltBLifetimeRegionalCtfWithMaterialTracksStartup+hltBLifetimeL3AssociatorStartup+hltBLifetimeL3TagInfosStartup+hltBLifetimeL3BJetTagsStartup)


HLTL1IsolatedEcalClustersSequence = cms.Sequence(hltHybridSuperClustersL1Isolated+hltCorrectedHybridSuperClustersL1Isolated+HLTMulti5x5SuperClusterL1Isolated)


HLTDoubleElectronEt5JpsiSequence = cms.Sequence(HLTDoRegionalEgammaEcalSequence+HLTL1IsolatedEcalClustersSequence+HLTL1NonIsolatedEcalClustersSequence+hltL1IsoRecoEcalCandidate+hltL1NonIsoRecoEcalCandidate+hltL1NonIsoDoubleElectronEt5JpsiL1MatchFilterRegional+hltL1NonIsoDoubleElectronEt5JpsiEtFilter+hltL1IsoHLTClusterShape+hltL1NonIsoHLTClusterShape+hltL1NonIsoDoubleElectronEt5JpsiClusterShapeFilter+hltL1IsolatedPhotonEcalIsol+hltL1NonIsolatedPhotonEcalIsol+hltL1NonIsoDoubleElectronEt5JpsiEcalIsolFilter+HLTDoLocalHcalWithoutHOSequence+hltL1IsolatedElectronHcalIsol+hltL1NonIsolatedElectronHcalIsol+hltL1NonIsoDoubleElectronEt5JpsiHcalIsolFilter+HLTDoLocalPixelSequence+HLTDoLocalStripSequence+hltL1IsoStartUpElectronPixelSeeds+hltL1NonIsoStartUpElectronPixelSeeds+hltL1NonIsoDoubleElectronEt5JpsiPixelMatchFilter+hltL1NonIsoDoubleElectronEt5JpsiPMMassFilter)


HLTEle10SWL1RTripleJet30EGSequence = cms.Sequence(HLTDoRegionalEgammaEcalSequence+HLTL1IsolatedEcalClustersSequence+HLTL1NonIsolatedEcalClustersSequence+hltL1IsoRecoEcalCandidate+hltL1NonIsoRecoEcalCandidate+hltEle10SWL1RTripleJet30L1MatchFilterRegional+hltEle10SWL1RTripleJet30EleEtFilter+HLTDoLocalHcalWithoutHOSequence+hltL1IsolatedElectronHcalIsol+hltL1NonIsolatedElectronHcalIsol+hltEle10SWL1RTripleJet30EleHcalIsolFilter+HLTDoLocalPixelSequence+HLTDoLocalStripSequence+hltL1IsoStartUpElectronPixelSeeds+hltL1NonIsoStartUpElectronPixelSeeds+hltEle10SWL1RTripleJet30ElePixelMatchFilter)


HLTBTagIPSequenceL3StartupU = cms.Sequence(HLTDoLocalPixelSequence+HLTDoLocalStripSequence+hltBLifetimeL3JetsStartupU+hltBLifetimeRegionalPixelSeedGeneratorStartupU+hltBLifetimeRegionalCkfTrackCandidatesStartupU+hltBLifetimeRegionalCtfWithMaterialTracksStartupU+hltBLifetimeL3AssociatorStartupU+hltBLifetimeL3TagInfosStartupU+hltBLifetimeL3BJetTagsStartupU)


HLTSingleElectronEt20L1NonIsoHLTIsoEleIdSequence = cms.Sequence(HLTDoRegionalEgammaEcalSequence+HLTL1IsolatedEcalClustersSequence+HLTL1NonIsolatedEcalClustersSequence+hltL1IsoRecoEcalCandidate+hltL1NonIsoRecoEcalCandidate+hltL1NonIsoHLTIsoSingleElectronEt20LTIEleIdL1MatchFilterRegional+hltL1NonIsoHLTIsoSingleElectronEt20LTIEleIdEtFilter+hltL1IsoHLTClusterShape+hltL1NonIsoHLTClusterShape+hltL1NonIsoHLTIsoSingleElectronEt20LTIEleIdClusterShapeFilter+HLTDoLocalHcalWithoutHOSequence+hltL1IsolatedElectronHcalIsol+hltL1NonIsolatedElectronHcalIsol+hltL1NonIsoHLTIsoSingleElectronEt20LTIEleIdHcalIsolFilter+HLTDoLocalPixelSequence+HLTDoLocalStripSequence+hltL1IsoStartUpElectronPixelSeeds+hltL1NonIsoStartUpElectronPixelSeeds+hltL1NonIsoHLTIsoSingleElectronEt20LTIEleIdPixelMatchFilter+HLTPixelMatchElectronL1IsoTrackingSequence+HLTPixelMatchElectronL1NonIsoTrackingSequence+hltL1NonIsoHLTIsoSingleElectronEt20LTIEleIdOneOEMinusOneOPFilter+hltElectronL1IsoDetaDphi+hltElectronL1NonIsoDetaDphi+hltL1NonIsoHLTIsoSingleElectronEt20LTIEleIdDetaFilter+hltL1NonIsoHLTIsoSingleElectronEt20LTIEleIdDphiFilter+HLTL1IsoElectronsRegionalRecoTrackerSequence+HLTL1NonIsoElectronsRegionalRecoTrackerSequence+hltL1IsoElectronTrackIsol+hltL1NonIsoElectronTrackIsol+hltL1NonIsoHLTIsoSingleElectronEt20LTIEleIdTrackIsolFilter)


HLTDoCaloSequence = cms.Sequence(hltEcalRawToRecHitFacility+hltEcalRegionalRestFEDs+hltEcalRecHitAll+HLTDoLocalHcalSequence+hltTowerMakerForAll)


HLTDoHIJetRecoSequence = cms.Sequence(HLTDoCaloSequence+hltIterativeCone5PileupSubtractionCaloJets)


HLTSinglePhoton10L1NonIsolatedHLTNonIsoSequence = cms.Sequence(HLTDoRegionalEgammaEcalSequence+HLTL1IsolatedEcalClustersSequence+HLTL1NonIsolatedEcalClustersSequence+hltL1IsoRecoEcalCandidate+hltL1NonIsoRecoEcalCandidate+hltL1NonIsoHLTNonIsoSinglePhotonEt10L1MatchFilterRegional+hltL1NonIsoHLTNonIsoSinglePhotonEt10EtFilter+HLTDoLocalHcalWithoutHOSequence+hltL1IsolatedPhotonHcalIsol+hltL1NonIsolatedPhotonHcalIsol+hltL1NonIsoHLTNonIsoSinglePhotonEt10HcalIsolFilter)


HLTSinglePhoton15L1NonIsolatedHLTLEISequence = cms.Sequence(HLTDoRegionalEgammaEcalSequence+HLTL1IsolatedEcalClustersSequence+HLTL1NonIsolatedEcalClustersSequence+hltL1IsoRecoEcalCandidate+hltL1NonIsoRecoEcalCandidate+hltL1NonIsoSinglePhotonEt15LEIL1MatchFilterRegional+hltL1NonIsoSinglePhotonEt15LEIEtFilter+hltL1IsolatedPhotonEcalIsol+hltL1NonIsolatedPhotonEcalIsol+hltL1NonIsoSinglePhotonEt15LEIEcalIsolFilter+HLTDoLocalHcalWithoutHOSequence+hltL1IsolatedPhotonHcalIsol+hltL1NonIsolatedPhotonHcalIsol+hltL1NonIsoSinglePhotonEt15LEIHcalIsolFilter)


HLTBLifetimeL3recoSequence = cms.Sequence(hltBLifetimeL3Jets+HLTDoLocalPixelSequence+HLTDoLocalStripSequence+hltBLifetimeRegionalPixelSeedGenerator+hltBLifetimeRegionalCkfTrackCandidates+hltBLifetimeRegionalCtfWithMaterialTracks+hltBLifetimeL3Associator+hltBLifetimeL3TagInfos+hltBLifetimeL3BJetTags)


HLTL2muonrecoNocandSequence = cms.Sequence(HLTmuonlocalrecoSequence+hltL2MuonSeeds+hltL2Muons)


HLTL2Mu7Photon9HLTNonIsoEGSequence = cms.Sequence(HLTDoRegionalEgammaEcalSequence+HLTL1IsolatedEcalClustersSequence+HLTL1NonIsolatedEcalClustersSequence+hltL1IsoRecoEcalCandidate+hltL1NonIsoRecoEcalCandidate+hltL2Mu7Photon9L1MatchFilterRegional+hltL2Mu7Photon9EtFilter+HLTDoLocalHcalWithoutHOSequence+hltL1IsolatedPhotonHcalIsol+hltL1NonIsolatedPhotonHcalIsol+hltL2Mu7Photon9HcalIsolFilter)


HLTSinglePhoton25L1NonIsolatedHLTLEITISequence = cms.Sequence(HLTDoRegionalEgammaEcalSequence+HLTL1IsolatedEcalClustersSequence+HLTL1NonIsolatedEcalClustersSequence+hltL1IsoRecoEcalCandidate+hltL1NonIsoRecoEcalCandidate+hltL1NonIsoHLTLEITISinglePhotonEt25L1MatchFilterRegional+hltL1NonIsoHLTLEITISinglePhotonEt25EtFilter+hltL1IsolatedPhotonEcalIsol+hltL1NonIsolatedPhotonEcalIsol+hltL1NonIsoHLTLEITISinglePhotonEt25EcalIsolFilter+HLTDoLocalHcalWithoutHOSequence+hltL1IsolatedPhotonHcalIsol+hltL1NonIsolatedPhotonHcalIsol+hltL1NonIsoHLTLEITISinglePhotonEt25HcalIsolFilter+HLTDoLocalTrackerSequence+HLTL1IsoEgammaRegionalRecoTrackerSequence+HLTL1NonIsoEgammaRegionalRecoTrackerSequence+hltL1IsoPhotonHollowTrackIsol+hltL1NonIsoPhotonHollowTrackIsol+hltL1NonIsoHLTLEITISinglePhotonEt25TrackIsolFilter)


HLTSingleElectronEt15LTIL1NonIsoSequence = cms.Sequence(HLTDoRegionalEgammaEcalSequence+HLTL1IsolatedEcalClustersSequence+HLTL1NonIsolatedEcalClustersSequence+hltL1IsoRecoEcalCandidate+hltL1NonIsoRecoEcalCandidate+hltL1NonIsoHLTNonIsoSingleElectronEt15LTIL1MatchFilterRegional+hltL1NonIsoHLTNonIsoSingleElectronEt15LTIEtFilter+hltL1IsolatedPhotonEcalIsol+hltL1NonIsolatedPhotonEcalIsol+hltL1NonIsoHLTNonIsoSingleElectronEt15LTIEcalIsolFilter+HLTDoLocalHcalWithoutHOSequence+hltL1IsolatedElectronHcalIsol+hltL1NonIsolatedElectronHcalIsol+hltL1NonIsoHLTNonIsoSingleElectronEt15LTIHcalIsolFilter+HLTDoLocalPixelSequence+HLTDoLocalStripSequence+hltL1IsoStartUpElectronPixelSeeds+hltL1NonIsoStartUpElectronPixelSeeds+hltL1NonIsoHLTNonIsoSingleElectronEt15LTIPixelMatchFilter+HLTPixelMatchElectronL1IsoTrackingSequence+HLTPixelMatchElectronL1NonIsoTrackingSequence+hltL1NonIsoHLTNonIsoSingleElectronEt15LTIOneOEMinusOneOPFilter+HLTL1IsoElectronsRegionalRecoTrackerSequence+HLTL1NonIsoElectronsRegionalRecoTrackerSequence+hltL1IsoElectronTrackIsol+hltL1NonIsoElectronTrackIsol+hltL1NonIsoHLTNonIsoSingleElectronEt15LTITrackIsolFilter)


HLTRecoJetSequence = cms.Sequence(HLTDoCaloSequence+hltIterativeCone5CaloJets+hltMCJetCorJetIcone5)


HLTL3muonTkCandidateSequence = cms.Sequence(HLTDoLocalPixelSequence+HLTDoLocalStripSequence+hltL3TrajectorySeed+hltL3TrackCandidateFromL2)


OpenHLTDoCaloSequence = cms.Sequence(hltEcalRawToRecHitFacility+hltEcalRegionalJetsFEDs+hltEcalRegionalJetsRecHit+HLTDoLocalHcalSequence+hltTowerMakerForJets)


HLTSinglePhoton20L1NonIsolatedHLTLEITISequence = cms.Sequence(HLTDoRegionalEgammaEcalSequence+HLTL1IsolatedEcalClustersSequence+HLTL1NonIsolatedEcalClustersSequence+hltL1IsoRecoEcalCandidate+hltL1NonIsoRecoEcalCandidate+hltL1NonIsoHLTLEITISinglePhotonEt20L1MatchFilterRegional+hltL1NonIsoHLTLEITISinglePhotonEt20EtFilter+hltL1IsolatedPhotonEcalIsol+hltL1NonIsolatedPhotonEcalIsol+hltL1NonIsoHLTLEITISinglePhotonEt20EcalIsolFilter+HLTDoLocalHcalWithoutHOSequence+hltL1IsolatedPhotonHcalIsol+hltL1NonIsolatedPhotonHcalIsol+hltL1NonIsoHLTLEITISinglePhotonEt20HcalIsolFilter+HLTDoLocalTrackerSequence+HLTL1IsoEgammaRegionalRecoTrackerSequence+HLTL1NonIsoEgammaRegionalRecoTrackerSequence+hltL1IsoPhotonHollowTrackIsol+hltL1NonIsoPhotonHollowTrackIsol+hltL1NonIsoHLTLEITISinglePhotonEt20TrackIsolFilter)


HLTEle10LWL1RHT200EGSequence = cms.Sequence(HLTDoRegionalEgammaEcalSequence+HLTL1IsolatedEcalClustersSequence+HLTL1NonIsolatedEcalClustersSequence+hltL1IsoRecoEcalCandidate+hltL1NonIsoRecoEcalCandidate+hltEle10LWL1RHT200L1MatchFilterRegional+hltEle10LWL1RHT200EleEtFilter+HLTDoLocalHcalWithoutHOSequence+hltL1IsolatedElectronHcalIsol+hltL1NonIsolatedElectronHcalIsol+hltEle10LWL1RHT200EleHcalIsolFilter+HLTDoLocalPixelSequence+HLTDoLocalStripSequence+hltL1IsoLargeWindowElectronPixelSeeds+hltL1NonIsoLargeWindowElectronPixelSeeds+hltEle10LWL1RHT200ElePixelMatchFilter)


OpenHLTL25TauTrackReconstructionSequence = cms.Sequence(HLTDoLocalStripSequence+openhltL25TauPixelSeeds+openhltL25TauCkfTrackCandidates+openhltL25TauCtfWithMaterialTracks)


OpenHLTBLifetimeL3recoSequence = cms.Sequence(HLTDoLocalPixelSequence+HLTDoLocalStripSequence+openHltBLifetimeRegionalPixelSeedGenerator+openHltBLifetimeRegionalCkfTrackCandidates+openHltBLifetimeRegionalCtfWithMaterialTracks+openHltBLifetimeL3Associator+openHltBLifetimeL3TagInfos+openHltBLifetimeL3BJetTags)


HLTDoubleElectronEt5UpsSequence = cms.Sequence(HLTDoRegionalEgammaEcalSequence+HLTL1IsolatedEcalClustersSequence+HLTL1NonIsolatedEcalClustersSequence+hltL1IsoRecoEcalCandidate+hltL1NonIsoRecoEcalCandidate+hltL1NonIsoDoubleElectronEt5UpsL1MatchFilterRegional+hltL1NonIsoDoubleElectronEt5UpsEtFilter+hltL1IsoHLTClusterShape+hltL1NonIsoHLTClusterShape+hltL1NonIsoDoubleElectronEt5UpsClusterShapeFilter+hltL1IsolatedPhotonEcalIsol+hltL1NonIsolatedPhotonEcalIsol+hltL1NonIsoDoubleElectronEt5UpsEcalIsolFilter+HLTDoLocalHcalWithoutHOSequence+hltL1IsolatedElectronHcalIsol+hltL1NonIsolatedElectronHcalIsol+hltL1NonIsoDoubleElectronEt5UpsHcalIsolFilter+HLTDoLocalPixelSequence+HLTDoLocalStripSequence+hltL1IsoStartUpElectronPixelSeeds+hltL1NonIsoStartUpElectronPixelSeeds+hltL1NonIsoDoubleElectronEt5UpsPixelMatchFilter+hltL1NonIsoDoubleElectronEt5UpsPMMassFilter)


HLTL25TauPixelTracksIsolationNoL2 = cms.Sequence(HLTDoLocalPixelSequence+HLTRecopixelvertexingSequence+hltL25TauJetPixelTracksAssociatorNoL2+hltL25TauPixelTracksConeIsolationNoL2+hltL25TauPixelTracksIsolationSelectorNoL2+hltL25TauPixelTracksLeadingTrackPtCutSelectorNoL2)


HLTSingleElectronEt10L1NonIsoHLTEleIdSequence = cms.Sequence(HLTDoRegionalEgammaEcalSequence+HLTL1IsolatedEcalClustersSequence+HLTL1NonIsolatedEcalClustersSequence+hltL1IsoRecoEcalCandidate+hltL1NonIsoRecoEcalCandidate+hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdL1MatchFilterRegional+hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdEtFilter+hltL1IsoHLTClusterShape+hltL1NonIsoHLTClusterShape+hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdClusterShapeFilter+HLTDoLocalHcalWithoutHOSequence+hltL1IsolatedElectronHcalIsol+hltL1NonIsolatedElectronHcalIsol+hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdHcalIsolFilter+HLTDoLocalPixelSequence+HLTDoLocalStripSequence+hltL1IsoLargeWindowElectronPixelSeeds+hltL1NonIsoLargeWindowElectronPixelSeeds+hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdPixelMatchFilter+HLTPixelMatchLargeWindowElectronL1IsoTrackingSequence+HLTPixelMatchLargeWindowElectronL1NonIsoTrackingSequence+hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdOneOEMinusOneOPFilter+hltElectronL1IsoLargeWindowDetaDphi+hltElectronL1NonIsoLargeWindowDetaDphi+hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdDetaFilter+hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdDphiFilter)


HLTSinglePhoton10L1NonIsolatedHLTLEITISequence = cms.Sequence(HLTDoRegionalEgammaEcalSequence+HLTL1IsolatedEcalClustersSequence+HLTL1NonIsolatedEcalClustersSequence+hltL1IsoRecoEcalCandidate+hltL1NonIsoRecoEcalCandidate+hltL1NonIsoHLTLEITISinglePhotonEt10L1MatchFilterRegional+hltL1NonIsoHLTLEITISinglePhotonEt10EtFilter+hltL1IsolatedPhotonEcalIsol+hltL1NonIsolatedPhotonEcalIsol+hltL1NonIsoHLTLEITISinglePhotonEt10EcalIsolFilter+HLTDoLocalHcalWithoutHOSequence+hltL1IsolatedPhotonHcalIsol+hltL1NonIsolatedPhotonHcalIsol+hltL1NonIsoHLTLEITISinglePhotonEt10HcalIsolFilter+HLTDoLocalTrackerSequence+HLTL1IsoEgammaRegionalRecoTrackerSequence+HLTL1NonIsoEgammaRegionalRecoTrackerSequence+hltL1IsoPhotonHollowTrackIsol+hltL1NonIsoPhotonHollowTrackIsol+hltL1NonIsoHLTLEITISinglePhotonEt10TrackIsolFilter)


HLTL3muonrecoNocandSequence = cms.Sequence(HLTL3muonTkCandidateSequence+hltL3TkTracksFromL2+hltL3Muons)


HLTBTagMuSequenceL3U = cms.Sequence(HLTL3muonrecoNocandSequence+hltBSoftMuonL3TagInfosU+hltBSoftMuonL3BJetTagsUByPt+hltBSoftMuonL3BJetTagsUByDR)


HLTSingleElectronEt15L1NonIsoHLTEleIdSequence = cms.Sequence(HLTDoRegionalEgammaEcalSequence+HLTL1IsolatedEcalClustersSequence+HLTL1NonIsolatedEcalClustersSequence+hltL1IsoRecoEcalCandidate+hltL1NonIsoRecoEcalCandidate+hltL1NonIsoHLTNonIsoSingleElectronEt15EleIdL1MatchFilterRegional+hltL1NonIsoHLTNonIsoSingleElectronEt15EleIdEtFilter+hltL1IsoHLTClusterShape+hltL1NonIsoHLTClusterShape+hltL1NonIsoHLTNonIsoSingleElectronEt15EleIdClusterShapeFilter+HLTDoLocalHcalWithoutHOSequence+hltL1IsolatedElectronHcalIsol+hltL1NonIsolatedElectronHcalIsol+hltL1NonIsoHLTNonIsoSingleElectronEt15EleIdHcalIsolFilter+HLTDoLocalPixelSequence+HLTDoLocalStripSequence+hltL1IsoStartUpElectronPixelSeeds+hltL1NonIsoStartUpElectronPixelSeeds+hltL1NonIsoHLTNonIsoSingleElectronEt15EleIdPixelMatchFilter+HLTPixelMatchElectronL1IsoTrackingSequence+HLTPixelMatchElectronL1NonIsoTrackingSequence+hltL1NonIsoHLTNonIsoSingleElectronEt15EleIdOneOEMinusOneOPFilter+hltElectronL1IsoDetaDphi+hltElectronL1NonIsoDetaDphi+hltL1NonIsoHLTNonIsoSingleElectronEt15EleIdDetaFilter+hltL1NonIsoHLTNonIsoSingleElectronEt15EleIdDphiFilter)


HLTSinglePhoton15L1NonIsolatedHLTNonIsoSequence = cms.Sequence(HLTDoRegionalEgammaEcalSequence+HLTL1IsolatedEcalClustersSequence+HLTL1NonIsolatedEcalClustersSequence+hltL1IsoRecoEcalCandidate+hltL1NonIsoRecoEcalCandidate+hltL1NonIsoHLTNonIsoSinglePhotonEt15L1MatchFilterRegional+hltL1NonIsoHLTNonIsoSinglePhotonEt15EtFilter+HLTDoLocalHcalWithoutHOSequence+hltL1IsolatedPhotonHcalIsol+hltL1NonIsolatedPhotonHcalIsol+hltL1NonIsoHLTNonIsoSinglePhotonEt15HcalIsolFilter)


HLTSingleElectronEt15L1NonIsoHLTNonIsoSequence = cms.Sequence(HLTDoRegionalEgammaEcalSequence+HLTL1IsolatedEcalClustersSequence+HLTL1NonIsolatedEcalClustersSequence+hltL1IsoRecoEcalCandidate+hltL1NonIsoRecoEcalCandidate+hltL1NonIsoHLTNonIsoSingleElectronEt15L1MatchFilterRegional+hltL1NonIsoHLTNonIsoSingleElectronEt15EtFilter+HLTDoLocalHcalWithoutHOSequence+hltL1IsolatedElectronHcalIsol+hltL1NonIsolatedElectronHcalIsol+hltL1NonIsoHLTNonIsoSingleElectronEt15HcalIsolFilter+HLTDoLocalPixelSequence+HLTDoLocalStripSequence+hltL1IsoStartUpElectronPixelSeeds+hltL1NonIsoStartUpElectronPixelSeeds+hltL1NonIsoHLTNonIsoSingleElectronEt15PixelMatchFilter)


HLTSingleElectronEt10L1NonIsoHLTnonIsoSequence = cms.Sequence(HLTDoRegionalEgammaEcalSequence+HLTL1IsolatedEcalClustersSequence+HLTL1NonIsolatedEcalClustersSequence+hltL1IsoRecoEcalCandidate+hltL1NonIsoRecoEcalCandidate+hltL1NonIsoHLTNonIsoSingleElectronEt10L1MatchFilterRegional+hltL1NonIsoHLTNonIsoSingleElectronEt10EtFilter+HLTDoLocalHcalWithoutHOSequence+hltL1IsolatedElectronHcalIsol+hltL1NonIsolatedElectronHcalIsol+hltL1NonIsoHLTNonIsoSingleElectronEt10HcalIsolFilter+HLTDoLocalPixelSequence+HLTDoLocalStripSequence+hltL1IsoStartUpElectronPixelSeeds+hltL1NonIsoStartUpElectronPixelSeeds+hltL1NonIsoHLTNonIsoSingleElectronEt10PixelMatchFilter)


HLTL2muonrecoSequence = cms.Sequence(HLTL2muonrecoNocandSequence+hltL2MuonCandidates)


HLTL3muonTkCandidateSequenceNoVtx = cms.Sequence(HLTDoLocalPixelSequence+HLTDoLocalStripSequence+hltL3TrajectorySeedNoVtx+hltL3TrackCandidateFromL2NoVtx)


HLTDoubleElectronEt10L1NonIsoHLTnonIsoSequence = cms.Sequence(HLTDoRegionalEgammaEcalSequence+HLTL1IsolatedEcalClustersSequence+HLTL1NonIsolatedEcalClustersSequence+hltL1IsoRecoEcalCandidate+hltL1NonIsoRecoEcalCandidate+hltL1NonIsoHLTNonIsoDoubleElectronEt10L1MatchFilterRegional+hltL1NonIsoHLTNonIsoDoubleElectronEt10EtFilter+HLTDoLocalHcalWithoutHOSequence+hltL1IsolatedElectronHcalIsol+hltL1NonIsolatedElectronHcalIsol+hltL1NonIsoHLTNonIsoDoubleElectronEt10HcalIsolFilter+HLTDoLocalPixelSequence+HLTDoLocalStripSequence+hltL1IsoStartUpElectronPixelSeeds+hltL1NonIsoStartUpElectronPixelSeeds+hltL1NonIsoHLTNonIsoDoubleElectronEt10PixelMatchFilter)


HLTDoublePhotonEt15L1NonIsoHLTNonIsoSequence = cms.Sequence(HLTDoRegionalEgammaEcalSequence+HLTL1IsolatedEcalClustersSequence+HLTL1NonIsolatedEcalClustersSequence+hltL1IsoRecoEcalCandidate+hltL1NonIsoRecoEcalCandidate+hltL1NonIsoHLTNonIsoDoublePhotonEt15L1MatchFilterRegional+hltL1NonIsoHLTNonIsoDoublePhotonEt15EtFilter+HLTDoLocalHcalWithoutHOSequence+hltL1IsolatedPhotonHcalIsol+hltL1NonIsolatedPhotonHcalIsol+hltL1NonIsoHLTNonIsoDoublePhotonEt15HcalIsolFilter)


HLTSinglePhoton20L1NonIsolatedHLTNonIsoSequence = cms.Sequence(HLTDoRegionalEgammaEcalSequence+HLTL1IsolatedEcalClustersSequence+HLTL1NonIsolatedEcalClustersSequence+hltL1IsoRecoEcalCandidate+hltL1NonIsoRecoEcalCandidate+hltL1NonIsoHLTNonIsoSinglePhotonEt20L1MatchFilterRegional+hltL1NonIsoHLTNonIsoSinglePhotonEt20EtFilter+HLTDoLocalHcalWithoutHOSequence+hltL1IsolatedPhotonHcalIsol+hltL1NonIsolatedPhotonHcalIsol+hltL1NonIsoHLTNonIsoSinglePhotonEt20HcalIsolFilter)


HLTSinglePhoton30L1NonIsolatedHLTNonIsoSequence = cms.Sequence(HLTDoRegionalEgammaEcalSequence+HLTL1IsolatedEcalClustersSequence+HLTL1NonIsolatedEcalClustersSequence+hltL1IsoRecoEcalCandidate+hltL1NonIsoRecoEcalCandidate+hltL1NonIsoHLTNonIsoSinglePhotonEt30L1MatchFilterRegional+hltL1NonIsoHLTNonIsoSinglePhotonEt30EtFilter+HLTDoLocalHcalWithoutHOSequence+hltL1IsolatedPhotonHcalIsol+hltL1NonIsolatedPhotonHcalIsol+hltL1NonIsoHLTNonIsoSinglePhotonEt30HcalIsolFilter)


HLTBTagMuSequenceL3 = cms.Sequence(HLTL3muonrecoNocandSequence+hltBSoftMuonL3TagInfos+hltBSoftMuonL3BJetTagsByPt+hltBSoftMuonL3BJetTagsByDR)


HLTDoublePhotonEt15L1NonIsoHLTVLEISequence = cms.Sequence(HLTDoRegionalEgammaEcalSequence+HLTL1IsolatedEcalClustersSequence+HLTL1NonIsolatedEcalClustersSequence+hltL1IsoRecoEcalCandidate+hltL1NonIsoRecoEcalCandidate+hltL1NonIsoHLTVLEIDoublePhotonEt15L1MatchFilterRegional+hltL1NonIsoHLTVLEIDoublePhotonEt15EtFilter+hltL1IsolatedPhotonEcalIsol+hltL1NonIsolatedPhotonEcalIsol+hltL1NonIsoHLTVLEIDoublePhotonEt15EcalIsolFilter+HLTDoLocalHcalWithoutHOSequence+hltL1IsolatedPhotonHcalIsol+hltL1NonIsolatedPhotonHcalIsol+hltL1NonIsoHLTVLEIDoublePhotonEt15HcalIsolFilter)


OpenHLTCaloTausCreatorSequence = cms.Sequence(OpenHLTDoCaloSequence+hltCaloTowersTau1Regional+hltIconeTau1Regional+hltCaloTowersTau2Regional+hltIconeTau2Regional+hltCaloTowersTau3Regional+hltIconeTau3Regional+hltCaloTowersTau4Regional+hltIconeTau4Regional+hltCaloTowersCentral1Regional+hltIconeCentral1Regional+hltCaloTowersCentral2Regional+hltIconeCentral2Regional+hltCaloTowersCentral3Regional+hltIconeCentral3Regional+hltCaloTowersCentral4Regional+hltIconeCentral4Regional)


HLTDoubleElectronEt5L1NonIsoHLTnonIsoSequence = cms.Sequence(HLTDoRegionalEgammaEcalSequence+HLTL1IsolatedEcalClustersSequence+HLTL1NonIsolatedEcalClustersSequence+hltL1IsoRecoEcalCandidate+hltL1NonIsoRecoEcalCandidate+hltL1NonIsoHLTNonIsoDoubleElectronEt5L1MatchFilterRegional+hltL1NonIsoHLTNonIsoDoubleElectronEt5EtFilter+HLTDoLocalHcalWithoutHOSequence+hltL1IsolatedElectronHcalIsol+hltL1NonIsolatedElectronHcalIsol+hltL1NonIsoHLTNonIsoDoubleElectronEt5HcalIsolFilter+HLTDoLocalPixelSequence+HLTDoLocalStripSequence+hltL1IsoStartUpElectronPixelSeeds+hltL1NonIsoStartUpElectronPixelSeeds+hltL1NonIsoHLTNonIsoDoubleElectronEt5PixelMatchFilter)


HLTBCommonL2recoSequence = cms.Sequence(HLTDoCaloSequence+HLTDoJetRecoSequence+HLTDoHTRecoSequence)


HLTBSoftmuonL3recoSequence = cms.Sequence(HLTL3muonrecoNocandSequence+hltBSoftmuonL3TagInfos+hltBSoftmuonL3BJetTags+hltBSoftmuonL3BJetTagsByDR)


HLTRecoJetMETSequence = cms.Sequence(HLTDoCaloSequence+hltIterativeCone5CaloJets+hltMCJetCorJetIcone5+hltMet+HLTDoHTRecoSequence)


OpenHLTBSoftmuonL25recoSequence = cms.Sequence(HLTL2muonrecoNocandSequence+openHltBSoftmuonL25TagInfos+openHltBSoftmuonL25BJetTags)


HLTSingleElectronLWEt10L1NonIsoHLTNonIsoSequence = cms.Sequence(HLTDoRegionalEgammaEcalSequence+HLTL1IsolatedEcalClustersSequence+HLTL1NonIsolatedEcalClustersSequence+hltL1IsoRecoEcalCandidate+hltL1NonIsoRecoEcalCandidate+hltL1NonIsoHLTNonIsoSingleElectronLWEt10L1MatchFilterRegional+hltL1NonIsoHLTNonIsoSingleElectronLWEt10EtFilter+HLTDoLocalHcalWithoutHOSequence+hltL1IsolatedElectronHcalIsol+hltL1NonIsolatedElectronHcalIsol+hltL1NonIsoHLTNonIsoSingleElectronLWEt10HcalIsolFilter+HLTDoLocalPixelSequence+HLTDoLocalStripSequence+hltL1IsoLargeWindowElectronPixelSeeds+hltL1NonIsoLargeWindowElectronPixelSeeds+hltL1NonIsoHLTNonIsoSingleElectronLWEt10PixelMatchFilter)


HLTBSoftmuonL25recoSequence = cms.Sequence(hltBSoftmuonHighestEtJets+hltBSoftmuonL25Jets+HLTL2muonrecoNocandSequence+hltBSoftmuonL25TagInfos+hltBSoftmuonL25BJetTags)


HLTSingleElectronLWEt15L1NonIsoHLTNonIsoSequence = cms.Sequence(HLTDoRegionalEgammaEcalSequence+HLTL1IsolatedEcalClustersSequence+HLTL1NonIsolatedEcalClustersSequence+hltL1IsoRecoEcalCandidate+hltL1NonIsoRecoEcalCandidate+hltL1NonIsoHLTNonIsoSingleElectronLWEt15L1MatchFilterRegional+hltL1NonIsoHLTNonIsoSingleElectronLWEt15EtFilter+HLTDoLocalHcalWithoutHOSequence+hltL1IsolatedElectronHcalIsol+hltL1NonIsolatedElectronHcalIsol+hltL1NonIsoHLTNonIsoSingleElectronLWEt15HcalIsolFilter+HLTDoLocalPixelSequence+HLTDoLocalStripSequence+hltL1IsoLargeWindowElectronPixelSeeds+hltL1NonIsoLargeWindowElectronPixelSeeds+hltL1NonIsoHLTNonIsoSingleElectronLWEt15PixelMatchFilter)


HLTSingleElectronEt20L1NonIsoHLTnonIsoSequence = cms.Sequence(HLTDoRegionalEgammaEcalSequence+HLTL1IsolatedEcalClustersSequence+HLTL1NonIsolatedEcalClustersSequence+hltL1IsoRecoEcalCandidate+hltL1NonIsoRecoEcalCandidate+hltL1NonIsoHLTNonIsoSingleElectronEt20L1MatchFilterRegional+hltL1NonIsoHLTNonIsoSingleElectronEt20EtFilter+HLTDoLocalHcalWithoutHOSequence+hltL1IsolatedElectronHcalIsol+hltL1NonIsolatedElectronHcalIsol+hltL1NonIsoHLTNonIsoSingleElectronEt20HcalIsolFilter+HLTDoLocalPixelSequence+HLTDoLocalStripSequence+hltL1IsoStartUpElectronPixelSeeds+hltL1NonIsoStartUpElectronPixelSeeds+hltL1NonIsoHLTNonIsoSingleElectronEt20PixelMatchFilter)


HLTDoublePhotonEt5UpsSequence = cms.Sequence(HLTDoRegionalEgammaEcalSequence+HLTL1IsolatedEcalClustersSequence+HLTL1NonIsolatedEcalClustersSequence+hltL1IsoRecoEcalCandidate+hltL1NonIsoRecoEcalCandidate+hltL1NonIsoDoublePhotonEt5UpsL1MatchFilterRegional+hltL1NonIsoDoublePhotonEt5UpsEtFilter+hltL1IsoHLTClusterShape+hltL1NonIsoHLTClusterShape+hltL1NonIsoDoublePhotonEt5UpsClusterShapeFilter+hltL1IsolatedPhotonEcalIsol+hltL1NonIsolatedPhotonEcalIsol+hltL1NonIsoDoublePhotonEt5UpsEcalIsolFilter+HLTDoLocalHcalWithoutHOSequence+hltL1IsolatedElectronHcalIsol+hltL1NonIsolatedElectronHcalIsol+hltL1NonIsoDoublePhotonEt5UpsHcalIsolFilter+hltL1NonIsoDoublePhotonEt5UpsPMMassFilter)


SimL1Emulator = cms.Sequence(simRctDigis*simGctDigis*SimL1MuTriggerPrimitives*SimL1MuTrackFinders*simRpcTriggerDigis*simGmtDigis*SimL1TechnicalTriggers*simGtDigis)


HLTSinglePhoton15L1NonIsolatedHLTHTISequence = cms.Sequence(HLTDoRegionalEgammaEcalSequence+HLTL1IsolatedEcalClustersSequence+HLTL1NonIsolatedEcalClustersSequence+hltL1IsoRecoEcalCandidate+hltL1NonIsoRecoEcalCandidate+hltL1NonIsoSinglePhotonEt15HTIL1MatchFilterRegional+hltL1NonIsoSinglePhotonEt15HTIEtFilter+HLTDoLocalHcalWithoutHOSequence+hltL1IsolatedPhotonHcalIsol+hltL1NonIsolatedPhotonHcalIsol+hltL1NonIsoSinglePhotonEt15HTIHcalIsolFilter+HLTDoLocalTrackerSequence+HLTL1IsoEgammaRegionalRecoTrackerSequence+HLTL1NonIsoEgammaRegionalRecoTrackerSequence+hltL1IsoPhotonHollowTrackIsol+hltL1NonIsoPhotonHollowTrackIsol+hltL1NonIsoSinglePhotonEt15HTITrackIsolFilter)


HLTBTagIPSequenceL25Startup = cms.Sequence(HLTDoLocalPixelSequence+HLTRecopixelvertexingSequence+hltSelector4JetsRegional+hltBLifetimeL25JetsStartup+hltBLifetimeL25AssociatorStartup+hltBLifetimeL25TagInfosStartup+hltBLifetimeL25BJetTagsStartup)


HLTDoublePhotonEt5JpsiSequence = cms.Sequence(HLTDoRegionalEgammaEcalSequence+HLTL1IsolatedEcalClustersSequence+HLTL1NonIsolatedEcalClustersSequence+hltL1IsoRecoEcalCandidate+hltL1NonIsoRecoEcalCandidate+hltL1NonIsoDoublePhotonEt5JpsiL1MatchFilterRegional+hltL1NonIsoDoublePhotonEt5JpsiEtFilter+hltL1IsoHLTClusterShape+hltL1NonIsoHLTClusterShape+hltL1NonIsoDoublePhotonEt5JpsiClusterShapeFilter+hltL1IsolatedPhotonEcalIsol+hltL1NonIsolatedPhotonEcalIsol+hltL1NonIsoDoublePhotonEt5JpsiEcalIsolFilter+HLTDoLocalHcalWithoutHOSequence+hltL1IsolatedElectronHcalIsol+hltL1NonIsolatedElectronHcalIsol+hltL1NonIsoDoublePhotonEt5JpsiHcalIsolFilter+hltL1NonIsoDoublePhotonEt5JpsiPMMassFilter)


HLTL2muonrecoSequenceNoVtx = cms.Sequence(HLTL2muonrecoNocandSequence+hltL2MuonCandidatesNoVtx)


HLTDoublePhotonEt10L1NonIsoHLTNonIsoSequence = cms.Sequence(HLTDoRegionalEgammaEcalSequence+HLTL1IsolatedEcalClustersSequence+HLTL1NonIsolatedEcalClustersSequence+hltL1IsoRecoEcalCandidate+hltL1NonIsoRecoEcalCandidate+hltL1NonIsoHLTNonIsoDoublePhotonEt10L1MatchFilterRegional+hltL1NonIsoHLTNonIsoDoublePhotonEt10EtFilter+HLTDoLocalHcalWithoutHOSequence+hltL1IsolatedPhotonHcalIsol+hltL1NonIsolatedPhotonHcalIsol+hltL1NonIsoHLTNonIsoDoublePhotonEt10HcalIsolFilter)


HLTBTagMuSequenceL25U = cms.Sequence(HLTL2muonrecoNocandSequence+hltSelector4JetsU+hltBSoftMuonL25JetsU+hltBSoftMuonL25TagInfosU+hltBSoftMuonL25BJetTagsUByDR)


HLTSinglePhoton25L1NonIsolatedHLTNonIsoSequence = cms.Sequence(HLTDoRegionalEgammaEcalSequence+HLTL1IsolatedEcalClustersSequence+HLTL1NonIsolatedEcalClustersSequence+hltL1IsoRecoEcalCandidate+hltL1NonIsoRecoEcalCandidate+hltL1NonIsoHLTNonIsoSinglePhotonEt25L1MatchFilterRegional+hltL1NonIsoHLTNonIsoSinglePhotonEt25EtFilter+HLTDoLocalHcalWithoutHOSequence+hltL1IsolatedPhotonHcalIsol+hltL1NonIsolatedPhotonHcalIsol+hltL1NonIsoHLTNonIsoSinglePhotonEt25HcalIsolFilter)


HLTDoublePhotonEt5eeResSequence = cms.Sequence(HLTDoRegionalEgammaEcalSequence+HLTL1IsolatedEcalClustersSequence+HLTL1NonIsolatedEcalClustersSequence+hltL1IsoRecoEcalCandidate+hltL1NonIsoRecoEcalCandidate+hltL1NonIsoDoublePhotonEt5eeResL1MatchFilterRegional+hltL1NonIsoDoublePhotonEt5eeResEtFilter+hltL1IsoHLTClusterShape+hltL1NonIsoHLTClusterShape+hltL1NonIsoDoublePhotonEt5eeResClusterShapeFilter+hltL1IsolatedPhotonEcalIsol+hltL1NonIsolatedPhotonEcalIsol+hltL1NonIsoDoublePhotonEt5eeResEcalIsolFilter+HLTDoLocalHcalWithoutHOSequence+hltL1IsolatedElectronHcalIsol+hltL1NonIsolatedElectronHcalIsol+hltL1NonIsoDoublePhotonEt5eeResHcalIsolFilter+hltL1NonIsoDoublePhotonEt5eeResPMMassFilter)


HLTRecoJetSequenceU = cms.Sequence(HLTDoCaloSequence+hltIterativeCone5CaloJets+hltMCJetCorJetIcone5HF07)


HLTL3muonrecoNocandSequenceNoVtx = cms.Sequence(HLTL3muonTkCandidateSequenceNoVtx+hltL3TkTracksFromL2NoVtx+hltL3MuonsNoVtx)


HLTL3muonrecoSequenceNoVtx = cms.Sequence(HLTL3muonrecoNocandSequenceNoVtx+hltL3MuonCandidatesNoVtx)


HLTRecoMETSequence = cms.Sequence(HLTDoCaloSequence+hltMet)


HLTBTagMuSequenceL25 = cms.Sequence(HLTL2muonrecoNocandSequence+hltSelector4Jets+hltBSoftMuonL25Jets+hltBSoftMuonL25TagInfos+hltBSoftMuonL25BJetTagsByDR)


HLTCaloTausCreatorSequence = cms.Sequence(HLTDoCaloSequence+hltCaloTowersTau1+hltIcone5Tau1+hltCaloTowersTau2+hltIcone5Tau2+hltCaloTowersTau3+hltIcone5Tau3+hltCaloTowersTau4+hltIcone5Tau4+hltCaloTowersCentral1+hltIcone5Central1+hltCaloTowersCentral2+hltIcone5Central2+hltCaloTowersCentral3+hltIcone5Central3+hltCaloTowersCentral4+hltIcone5Central4)


HLTL3muonrecoSequence = cms.Sequence(HLTL3muonrecoNocandSequence+hltL3MuonCandidates)


OpenHLTBSoftmuonL3recoSequence = cms.Sequence(HLTL3muonrecoNocandSequence+openHltBSoftmuonL3TagInfos+openHltBSoftmuonL3BJetTags+openHltBPerfMeasL3BJetTags)


HLTL2TauEcalIsolationNoCutSequence = cms.Sequence(HLTCaloTausCreatorSequence+hltL2TauJets+hltL2TauIsolationProducer+hltL2TauIsolationSelectorNoCut)


DoHLTAlCaPi0Eta1E31 = cms.Path(HLTBeginSequence+hltL1sAlCaEcalPi0Eta1E31+HLTDoRegionalPi0EtaSequence+HLTEndSequence)


HLT_Ele15_SW_EleId_L1R = cms.Path(HLTBeginSequenceBPTX+hltL1sL1SingleEG8+hltPreEle15SWEleIdL1R+HLTSingleElectronEt15L1NonIsoHLTEleIdSequence+HLTEndSequence)


HLT_DoubleEle5_SW_L1R = cms.Path(HLTBeginSequenceBPTX+hltL1sL1DoubleEG5+hltPreDoubleEle5SWL1R+HLTDoubleElectronEt5L1NonIsoHLTnonIsoSequence+HLTEndSequence)


HLT_HT240 = cms.Path(HLTBeginSequenceBPTX+hltL1sHT240+hltPreHT240+HLTRecoJetSequence+HLTDoJet30HTRecoSequence+hltHT240+HLTEndSequence)


HLT_Ele15_SW_L1R = cms.Path(HLTBeginSequenceBPTX+hltL1sL1SingleEG8+hltPreEle15SWL1R+HLTSingleElectronEt15L1NonIsoHLTNonIsoSequence+HLTEndSequence)


HLT_IsoMu7_BTagIP_Jet35 = cms.Path(HLTBeginSequenceBPTX+hltL1sMuB+hltPreIsoMu7BTagIPJet35+hltMuBLifetimeL1Filtered+HLTL2muonrecoSequence+hltMuBLifetimeIsoL2PreFiltered+HLTL2muonisorecoSequence+hltMuBLifetimeIsoL2IsoFiltered+HLTBCommonL2recoSequence+HLTBLifetimeL25recoSequence+hltBLifetimeL25filter+HLTL3muonrecoSequence+hltMuBLifetimeIsoL3PreFiltered+HLTL3muonisorecoSequence+hltMuBLifetimeIsoL3IsoFiltered+HLTBLifetimeL3recoSequence+hltBLifetimeL3filter+HLTEndSequence)


HLT_HIJet75U = cms.Path(HLTBeginSequenceBPTX+hltHIL1sJet75U+hltHIPreJet75U+HLTDoHIJetRecoSequence+hltHI1jet75U+HLTEndSequence)


AlCa_RPCMuonNormalisation = cms.Path(HLTBeginSequenceBPTX+hltL1sL1SingleMuOpenL1SingleMu0+hltPreRPCMuonNorma+hltRPCMuonNormaL1Filtered0+HLTmuonlocalrecoSequence+HLTEndSequence)


HLT_HIJet35U = cms.Path(HLTBeginSequenceBPTX+hltHIL1sJet35U+hltHIPreJet35U+HLTDoHIJetRecoSequence+hltHI1jet35U+HLTEndSequence)


HLT_IsoMu14_LooseIsoTau15_NoL25 = cms.Path(HLTBeginSequenceBPTX+hltL1sMuon5Tau20+hltPreIsoMu14LooseIsoTauNoL25+hltMuonTauL1Filtered+HLTL2muonrecoSequence+hltMuonTauIsoL2PreFiltered+HLTL2muonisorecoSequence+hltMuonTauIsoL2IsoFiltered+HLTDoLocalStripSequence+HLTL3muonrecoSequence+HLTL3muonisorecoSequence+hltMuonTauIsoL3PreFiltered+hltMuonTauIsoL3IsoFiltered+HLTL2TauJetsSequence+HLTL2TauEcalIsolationSequence+hltFilterL2EcalIsolationMuonTauNoL25+HLTEndSequence)


HLT_QuadJet15U = cms.Path(HLTBeginSequenceBPTX+hltL1sQuadJet15U+hltPreQuadJet15U+HLTRecoJetSequenceU+hlt4jet15U+HLTEndSequence)


HLT_DiJetAve30U_8E29 = cms.Path(HLTBeginSequenceBPTX+hltL1sDiJetAve30U8E29+hltPreDiJetAve30U8E29+HLTDoCaloSequence+hltIterativeCone5CaloJets+hltDiJetAve30U8E29+HLTEndSequence)


HLT_HIJet90U = cms.Path(HLTBeginSequenceBPTX+hltHIL1sJet90U+hltHIPreJet90U+HLTDoHIJetRecoSequence+hltHI1jet90U+HLTEndSequence)


HLT_L1Mu14_L1SingleEG10 = cms.Path(HLTBeginSequenceBPTX+hltL1sL1Mu14L1SingleEG10+hltPreL1Mu14L1SingleEG10+HLTEndSequence)


DoHLTMinBiasPixelTracks = cms.Path(HLTBeginSequence+hltSiPixelDigis+hltSiPixelClusters+hltSiPixelRecHits+hltPixelTracksForMinBias+hltPixelCands)


HLT_DoubleMu3_Psi2S = cms.Path(HLTBeginSequenceBPTX+hltL1sL1DoubleMu3+hltPreDoubleMu3Psi2S+hltJpsiMML1Filtered+HLTL2muonrecoSequence+hltPsi2SMML2Filtered+HLTL3muonrecoSequence+hltPsi2SMML3Filtered+HLTEndSequence)


HLT_L2Mu8_HT50 = cms.Path(HLTBeginSequenceBPTX+hltL1sL1Mu5Jet20+hltPreL2Mu5HT50+hltL1Mu3Jet15L1Filtered0+HLTL2muonrecoSequence+hltL2Filtered3Mu8HT50+HLTRecoJetSequence+HLTDoJet30HTRecoSequence+hltHT50Mu5HT50+HLTEndSequence)


HLT_BTagIP_Jet50U = cms.Path(HLTBeginSequenceBPTX+hltL1sBTagIPJet50U+hltPreBTagIPJet50U+HLTRecoJetSequenceU+hltBJet50U+HLTBTagIPSequenceL25StartupU+hltBLifetimeL25FilterStartupU+HLTBTagIPSequenceL3StartupU+hltBLifetimeL3FilterStartupU+HLTEndSequence)


HLT_SingleIsoTau30_Trk5 = cms.Path(HLTBeginSequenceBPTX+hltL1sSingleIsoTau30Trk5+hltPreSingleIsoTau30Trk5+HLTL2TauJetsSequence+hltFilterL2EtCutSingleIsoTau30Trk5+HLTL2TauEcalIsolationSequence+hltFilterL2EcalIsolationSingleIsoTau30Trk5+HLTDoLocalPixelSequence+HLTRecopixelvertexingSequence+HLTL25TauTrackReconstructionSequence+HLTL25TauTrackIsolationSequence+hltFilterL25LeadingTrackPtCutSingleIsoTau30Trk5+HLTL3TauTrackReconstructionSequence+HLTL3TauTrackIsolationSequence+hltL1HLTSingleIsoTau30JetsMatch+hltFilterL3IsolationCutSingleIsoTau30Trk5+HLTEndSequence)


HLT_MinBiasBSC = cms.Path(HLTBeginSequenceBPTX+hltL1sMinBiasBSC+hltPreMinBiasBSC+HLTEndSequence)


HLT_HIPhoton30 = cms.Path(HLTBeginSequenceBPTX+hltHIL1sPhoton30+hltHIPrePhoton30+HLTDoCaloSequence+HLTDoHIEcalClusSequence+hltHIPhoton30+HLTEndSequence)


HLT_Jet100_MET60_Aco = cms.Path(HLTBeginSequenceBPTX+hltL1sJet100MET60Aco+hltPreJet100MET60Aco+HLTRecoJetMETSequence+hlt1MET60+hlt1jet100+hlt1jet1METAco+HLTEndSequence)


HLT_HIDoubleMu = cms.Path(HLTBeginSequenceBPTX+hltHIPreMML1+hltHIMML1Seed+HLTL2muonrecoSequence+HLTDoLocalPixelSequence+HLTHIRecopixelvertexingSequence+HLTDoLocalStripSequence+hltHIMML3Filter+HLTEndSequence)


HLT_L1Mu = cms.Path(HLTBeginSequenceBPTX+hltL1sL1Mu+hltPreL1Mu+hltL1MuL1Filtered0+HLTEndSequence)


HLT_TripleJet60_MET60 = cms.Path(HLTBeginSequenceBPTX+hltL1sTripleJet60MET60+hltPreTripleJet60MET60+HLTRecoJetMETSequence+hlt1MET60+hlt3jet60+HLTEndSequence)


HLT_Mu13 = cms.Path(HLTBeginSequenceBPTX+hltL1sL1SingleMu10+hltPreMu13+hltL1SingleMu10L1Filtered0+HLTL2muonrecoSequence+hltSingleMu13L2Filtered11+HLTL3muonrecoSequence+hltSingleMu13L3Filtered13+HLTEndSequence)


HLT_Mu11 = cms.Path(HLTBeginSequenceBPTX+hltL1sL1SingleMu7+hltPreMu11+hltL1SingleMu7L1Filtered0+HLTL2muonrecoSequence+hltSingleMu11L2Filtered9+HLTL3muonrecoSequence+hltSingleMu11L3Filtered11+HLTEndSequence)


HLT_Mu15 = cms.Path(HLTBeginSequenceBPTX+hltL1sL1SingleMu10+hltPreMu15+hltSingleMu15L1Filtered0+HLTL2muonrecoSequence+hltSingleMu15L2PreFiltered12+HLTL3muonrecoSequence+hltSingleMu15L3PreFiltered15+HLTEndSequence)


HLT_L1MuOpen = cms.Path(HLTBeginSequenceBPTX+hltL1sL1SingleMuOpenL1SingleMu0+hltPreL1MuOpen+hltL1MuOpenL1Filtered0+HLTEndSequence)


HLT_Ele10_LW_L1R_HT200 = cms.Path(HLTBeginSequenceBPTX+hltL1sL1EG5HTT100+hltPreEle10LWL1RHT200+HLTEle10LWL1RHT200EGSequence+HLTRecoJetSequence+HLTDoJet30HTRecoSequence+hltEle10LWL1RHT200+HLTEndSequence)


HLT_Jet30U = cms.Path(HLTBeginSequenceBPTX+hltL1sJet30U+hltPreJet30U+HLTRecoJetSequenceU+hlt1jet30U+HLTEndSequence)


HLT_MET35 = cms.Path(HLTBeginSequenceBPTX+hltL1sMET35+hltPreMET35+HLTRecoMETSequence+hlt1MET35+HLTEndSequence)


HLT_Calibration = cms.Path(hltCalibrationEventsFilter+hltL1EventNumber+hltPreCalibration+HLTEndSequence)


HLT_EgammaSuperClusterOnly_L1R = cms.Path(HLTBeginSequenceBPTX+hltL1sEgammaOnlySC+hltPreEgammaOnlySC+HLTDoRegionalEgammaEcalSequence+HLTL1IsolatedEcalClustersSequence+HLTL1NonIsolatedEcalClustersSequence+hltL1IsoRecoEcalCandidate+hltL1NonIsoRecoEcalCandidate+hltEgammaSCOnlyL1MatchFilterRegional+hltEgammaSCOnlyEtFilter+HLTEndSequence)


HLT_MinBiasPixel_SingleTrack = cms.Path(HLTBeginSequenceBPTX+hltPreMinBiasPixelSingleTrack+HLTDoLocalPixelSequence+HLTPixelTrackingForMinBiasSequence+hltPixelCands+hltMinBiasPixelFilter1+HLTEndSequence)


HLT_SingleLooseIsoTau20 = cms.Path(HLTBeginSequenceBPTX+hltL1sSingleLooseIsoTau20+hltPreSingleLooseIsoTau20+HLTL2TauJetsSequence+hltFilterL2EtCutSingleLooseIsoTau20+HLTL2TauEcalIsolationSequence+hltL1HLTSingleLooseIsoTau20JetsMatch+hltFilterL2EcalIsolationSingleLooseIsoTau20+HLTEndSequence)


HLT_L1Tech_RPC_TTU_RBst1_collisions = cms.Path(HLTBeginSequenceBPTX+hltL1sL1TechRPCTTURBst1collisions+hltPreL1TechRPCTTURBst1collisions+HLTEndSequence)


HLT_PixelFEDSize = cms.Path(hltRandomEventsFilter+hltL1EventNumber+hltPrePixelFEDSize+hltPixelFEDSizeFilter+HLTEndSequence)


HLT_Photon20_L1R = cms.Path(HLTBeginSequenceBPTX+hltL1sL1SingleEG8+hltPrePhoton20L1R+HLTSinglePhoton20L1NonIsolatedHLTNonIsoSequence+HLTEndSequence)


HLT_MinBiasEcal = cms.Path(HLTBeginSequenceBPTX+hltL1sMinBiasEcal+hltPreMinBiasEcal+HLTEndSequence)


HLT_Physics = cms.Path(HLTBeginSequence+hltPrePhysics+HLTEndSequence)


DoHLTElectronStartUpWindows = cms.Path(HLTBeginSequence+HLTDoRegionalEgammaEcalSequence+HLTL1IsolatedEcalClustersSequence+HLTL1NonIsolatedEcalClustersSequence+hltL1IsoRecoEcalCandidate+hltL1NonIsoRecoEcalCandidate+HLTDoLocalHcalWithoutHOSequence+hltL1IsolatedElectronHcalIsol+hltL1NonIsolatedElectronHcalIsol+HLTDoLocalPixelSequence+HLTDoLocalStripSequence+hltL1IsoStartUpElectronPixelSeeds+hltL1NonIsoStartUpElectronPixelSeeds+HLTPixelMatchElectronL1IsoTrackingSequence+HLTPixelMatchElectronL1NonIsoTrackingSequence+HLTL1IsoElectronsRegionalRecoTrackerSequence+HLTL1NonIsoElectronsRegionalRecoTrackerSequence+hltL1IsoElectronTrackIsol+hltL1NonIsoElectronTrackIsol)


HLT_HFThreshold3 = cms.Path(HLTBeginSequenceBPTX+hltPreHFThreshold3+hltHcalDigis+hltHfreco+hltHcalSimpleRecHitFilter+HLTEndSequence)


HLT_DoubleIsoMu3 = cms.Path(HLTBeginSequenceBPTX+hltL1sL1DoubleMu3+hltPreDoubleIsoMu3+hltDiMuonIsoL1Filtered+HLTL2muonrecoSequence+hltDiMuonIsoL2PreFiltered+HLTL2muonisorecoSequence+hltDiMuonIsoL2IsoFiltered+HLTL3muonrecoSequence+hltDiMuonIsoL3PreFiltered+HLTL3muonisorecoSequence+hltDiMuonIsoL3IsoFiltered+HLTEndSequence)


HLT_Photon15_LooseEcalIso_L1R = cms.Path(HLTBeginSequenceBPTX+hltL1sL1SingleEG8+hltPrePhoton15LEIL1R+HLTSinglePhoton15L1NonIsolatedHLTLEISequence+HLTEndSequence)


HLT_DoubleJet125_Aco = cms.Path(HLTBeginSequenceBPTX+hltL1sDoubleJet125Aco+hltPreDoubleJet125Aco+HLTRecoJetRegionalSequence+hlt2jet125+hlt2jetAco+HLTEndSequence)


HLT_HIMinBiasCalo = cms.Path(HLTBeginSequenceBPTX+hltL1sHIMinBiasCalo+hltPreHIMinBiasCalo+HLTEndSequence)


HLT_DoubleEle5_SW_Jpsi_L1R = cms.Path(HLTBeginSequenceBPTX+hltL1sL1SingleEG8orL1DoubleEG5+hltPreDoubleEle5JpsiL1R+HLTDoubleElectronEt5JpsiSequence+HLTEndSequence)


HLT_L1Mu14_L1SingleJet6U = cms.Path(HLTBeginSequenceBPTX+hltL1sL1Mu14L1SingleJet6U+hltPreL1Mu14L1SingleJet6U+HLTEndSequence)


HLT_Mu5_TripleJet30 = cms.Path(HLTBeginSequenceBPTX+hltL1sMuNoIsoJets30+hltPreMu5TripleJet30+hltMuJetsMinPt4L1Filtered+HLTL2muonrecoSequence+hltMuJetsMinPt4L2PreFiltered+HLTL3muonrecoSequence+hltMuJetsMinPtL3PreFiltered+HLTRecoJetSequence+hltMuHLTJets3jet30+HLTEndSequence)


HLT_GlobalRunHPDNoise = cms.Path(HLTBeginSequence+hltL1sGlobalRunHPDNoise+hltPreGlobalRunHPDNoise+HLTEndSequence)


HLT_MET75 = cms.Path(HLTBeginSequenceBPTX+hltL1sMET75+hltPreMET75+HLTRecoMETSequence+hlt1MET75+HLTEndSequence)


HLT_HIJet50U = cms.Path(HLTBeginSequenceBPTX+hltHIL1sJet50U+hltHIPreJet50U+HLTDoHIJetRecoSequence+hltHI1jet50U+HLTEndSequence)


DoHLTAlCaPi0Eta8E29 = cms.Path(HLTBeginSequence+hltL1sAlCaEcalPi0Eta8E29+HLTDoRegionalPi0EtaSequence+HLTEndSequence)


HLT_L1_BPTX_PlusOnly = cms.Path(HLTBeginSequence+hltL1sL1BPTX+hltL1sL1BPTXPlusOnly+hltPreL1BPTXPlusOnly+HLTEndSequence)


HLT_Photon30_L1R_1E31 = cms.Path(HLTBeginSequenceBPTX+hltL1sL1SingleEG8+hltPrePhoton30L1R1E31+HLTSinglePhoton30L1NonIsolatedHLTNonIsoSequence+HLTEndSequence)


HLT_L1_BSC = cms.Path(HLTBeginSequenceBPTX+hltL1sL1BSC+hltPreL1BSC+HLTEndSequence)


HLT_DoubleEle10_SW_L1R = cms.Path(HLTBeginSequenceBPTX+hltL1sL1DoubleEG5+hltPreDoubleEle10SWL1R+HLTDoubleElectronEt10L1NonIsoHLTnonIsoSequence+HLTEndSequence)


HLT_Ele25_SW_EleId_LooseTrackIso_L1R = cms.Path(HLTBeginSequenceBPTX+hltL1sL1SingleEG8+hltPreEle25SWEleIdLTIL1R+HLTSingleElectronEt15L1NonIsoHLTEleIdSequence+hltL1NonIsoHLTNonIsoSingleElectronEt15EleIdEtFilterESet25LTI+HLTL1IsoElectronsRegionalRecoTrackerSequence+HLTL1NonIsoElectronsRegionalRecoTrackerSequence+hltL1IsoElectronTrackIsol+hltL1NonIsoElectronTrackIsol+hltL1NonIsoHLTNonIsoSingleElectronEt15EleIdTrackIsolFilterESet25LTI+HLTEndSequence)


HLT_IsoMu14_IsoTau15_Trk3 = cms.Path(HLTBeginSequenceBPTX+hltL1sMuon5Tau20+hltPreIsoMu14IsoTauTrk3+hltMuonTauL1Filtered+HLTL2muonrecoSequence+hltMuonTauIsoL2PreFiltered+HLTL2muonisorecoSequence+hltMuonTauIsoL2IsoFiltered+HLTDoLocalStripSequence+HLTL3muonrecoSequence+HLTL3muonisorecoSequence+hltMuonTauIsoL3PreFiltered+hltMuonTauIsoL3IsoFiltered+HLTL2TauJetsSequence+HLTL2TauEcalIsolationSequence+hltFilterL2EcalIsolationMuonTau+HLTL25TauPixelTrackIsolation+hltFilterL25PixelTracksLeadingTrackPtCutMuonTau+hltFilterL25PixelTracksIsolationMuonTau+HLTEndSequence)


HLT_Ele15_LW_L1R = cms.Path(HLTBeginSequenceBPTX+hltL1sL1SingleEG8+hltPreEle15LWL1R+HLTSingleElectronLWEt15L1NonIsoHLTNonIsoSequence+HLTEndSequence)


HLT_Photon10_L1R = cms.Path(HLTBeginSequenceBPTX+hltL1sL1SingleEG5+hltPrePhoton10L1R+HLTSinglePhoton10L1NonIsolatedHLTNonIsoSequence+HLTEndSequence)


HLT_HighMultiplicityBSC = cms.Path(HLTBeginSequenceBPTX+hltL1sHighMultiplicityBSC+hltPreHighMultiplicityBSC+HLTEndSequence)


HLT_CSCBeamHaloOverlapRing1 = cms.Path(HLTBeginSequenceBPTX+hltL1sCSCBeamHaloOverlapRing1+hltPreCSCBeamHaloOverlapRing1+hltMuonCSCDigis+hltCsc2DRecHits+hltOverlapsHLTCSCBeamHaloOverlapRing1+HLTEndSequence)


HLT_CSCBeamHaloOverlapRing2 = cms.Path(HLTBeginSequenceBPTX+hltL1sCSCBeamHaloOverlapRing2+hltPreCSCBeamHaloOverlapRing2+hltMuonCSCDigis+hltCsc2DRecHits+hltOverlapsHLTCSCBeamHaloOverlapRing2+HLTEndSequence)


HLT_L2Mu11 = cms.Path(HLTBeginSequenceBPTX+hltL1sL1SingleMu7+hltPreL2Mu11+hltL1SingleMu7L1Filtered0+HLTL2muonrecoSequence+hltL2Mu11L2Filtered11+HLTEndSequence)


DoHLTJetsU = cms.Path(HLTBeginSequence+HLTRecoJetSequenceU+hltMet+HLTDoJet15UHTRecoSequence)


HLT_BTagMu_Jet20 = cms.Path(HLTBeginSequenceBPTX+hltL1sBTagMuJet20+hltPreBTagMuJet20+HLTRecoJetSequence+hltBJet20+HLTBTagMuSequenceL25+hltBSoftMuonL25FilterByDR+HLTBTagMuSequenceL3+hltBSoftMuonL3FilterByDR+HLTEndSequence)


DoHLTJets = cms.Path(HLTBeginSequence+HLTRecoJetMETSequence+HLTDoJet30HTRecoSequence)


HLT_Ele15_SiStrip_L1R = cms.Path(HLTBeginSequenceBPTX+hltL1sL1SingleEG8+hltPreEle15SiStripL1R+HLTDoRegionalEgammaEcalSequence+HLTL1IsolatedEcalClustersSequence+HLTL1NonIsolatedEcalClustersSequence+hltL1IsoRecoEcalCandidate+hltL1NonIsoRecoEcalCandidate+hltL1NonIsoHLTNonIsoSingleElectronSiStripEt15L1MatchFilterRegional+hltL1NonIsoHLTNonIsoSingleElectronSiStripEt15EtFilter+HLTDoLocalHcalWithoutHOSequence+hltL1IsolatedElectronHcalIsol+hltL1NonIsolatedElectronHcalIsol+hltL1NonIsoHLTNonIsoSingleElectronSiStripEt15HcalIsolFilter+HLTDoLocalPixelSequence+HLTDoLocalStripSequence+hltL1IsoSiStripElectronPixelSeeds+hltL1NonIsoSiStripElectronPixelSeeds+hltL1NonIsoHLTNonIsoSingleElectronSiStripEt15PixelMatchFilter+HLTEndSequence)


HLT_DoubleMu7_Z = cms.Path(HLTBeginSequenceBPTX+hltL1sL1DoubleMu3+hltPreDoubleMu7Z+hltZMML1Filtered+HLTL2muonrecoSequence+hltZMML2Filtered+HLTL3muonrecoSequence+hltZMML3Filtered+HLTEndSequence)


HLT_Photon25_LooseEcalIso_TrackIso_L1R = cms.Path(HLTBeginSequenceBPTX+hltL1sL1SingleEG8+hltPrePhoton25HLTLEITIL1R+HLTSinglePhoton25L1NonIsolatedHLTLEITISequence+HLTEndSequence)


HLT_DoublePhoton10_L1R = cms.Path(HLTBeginSequenceBPTX+hltL1sL1DoubleEG5+hltPreDoublePhoton10L1R+HLTDoublePhotonEt10L1NonIsoHLTNonIsoSequence+HLTEndSequence)


HLT_BTagMu_Jet10U = cms.Path(HLTBeginSequenceBPTX+hltL1sBTagMuJet10U+hltPreBTagMuJet10U+HLTRecoJetSequenceU+hltBJet10U+HLTBTagMuSequenceL25U+hltBSoftMuonL25FilterUByDR+HLTBTagMuSequenceL3U+hltBSoftMuonL3FilterUByDR+HLTEndSequence)


HLT_L1SingleForJet = cms.Path(HLTBeginSequenceBPTX+hltL1sL1SingleForJet+hltPreL1SingleForJet+HLTEndSequence)


HLT_HT100U = cms.Path(HLTBeginSequenceBPTX+hltL1sHT100+hltPreHT100+HLTRecoJetSequenceU+HLTDoJet15UHTRecoSequence+hltHT100U+HLTEndSequence)


HLT_HcalCalibration = cms.Path(hltCalibrationEventsFilter+hltL1EventNumber+hltHcalCalibTypeFilter+hltPreHcalCalibration+HLTEndSequence)


HLT_HFThreshold10 = cms.Path(HLTBeginSequenceBPTX+hltPreHFThreshold10+hltHcalDigis+hltHfreco+hltHcalSimpleRecHitFilter10+HLTEndSequence)


HLT_L1Mu20 = cms.Path(HLTBeginSequenceBPTX+hltL1sL1SingleMu20+hltPreL1Mu20+hltL1Mu20L1Filtered20+HLTEndSequence)


HLT_L1Jet15 = cms.Path(HLTBeginSequenceBPTX+hltL1sL1Jet15+hltPreL1Jet15+HLTEndSequence)


HLT_DoublePhoton5_eeRes_L1R = cms.Path(HLTBeginSequenceBPTX+hltL1sL1SingleEG8orL1DoubleEG5+hltPreDoublePhoton5eeResL1R+HLTDoublePhotonEt5eeResSequence+HLTEndSequence)


HLT_DoublePhoton15_L1R = cms.Path(HLTBeginSequenceBPTX+hltL1sL1DoubleEG5+hltPreDoublePhoton15L1R+HLTDoublePhotonEt15L1NonIsoHLTNonIsoSequence+HLTEndSequence)


HLT_Ele20_SC15_SW_L1R = cms.Path(HLTBeginSequenceBPTX+hltL1sL1SingleEG8+hltPreEle20SC15SWL1R+HLTSingleElectronEt20L1NonIsoHLTnonIsoSequence+hltL1NonIsoHLTNonIsoSingleElectronEt20ESscWrapper+hltL1NonIsoHLTNonIsoSingleElectronEt20ESDoubleSC15+HLTEndSequence)


HLT_Ele15_SC10_LW_L1R = cms.Path(HLTBeginSequenceBPTX+hltL1sL1SingleEG8+hltPreEle15SC10LWL1R+HLTSingleElectronLWEt15L1NonIsoHLTNonIsoSequence+hltL1NonIsoHLTNonIsoSingleElectronLWEt15ESscWrapper+hltL1NonIsoHLTNonIsoSingleElectronLWEt15ESDoubleSC10+HLTEndSequence)


HLT_CSCBeamHalo = cms.Path(HLTBeginSequence+hltL1sCSCBeamHalo+hltPreCSCBeamHalo+HLTEndSequence)


HLT_Mu14_Jet50 = cms.Path(HLTBeginSequenceBPTX+hltL1sMuNoIsoJets+hltPreMu14Jet50+hltMuNoIsoJetsL1Filtered+HLTL2muonrecoSequence+hltMuNoIsoJetsL2PreFiltered+HLTL3muonrecoSequence+hltMuNoIsoJetsL3PreFiltered+HLTRecoJetSequence+hltMuJetsHLT1jet50+HLTEndSequence)


HLT_Jet110 = cms.Path(HLTBeginSequenceBPTX+hltL1sJet110+hltPreJet110+HLTRecoJetRegionalSequence+hlt1jet110+HLTEndSequence)


HLT_IsoTrackHE_8E29 = cms.Path(HLTBeginSequenceBPTX+hltL1sIsoTrack8E29+hltPreIsoTrackHE8E29+HLTL2HcalIsolTrackSequenceHE+hltIsolPixelTrackProdHE8E29+hltIsolPixelTrackL2FilterHE8E29+HLTDoLocalStripSequence+hltHITPixelTripletSeedGeneratorHE8E29+hltHITCkfTrackCandidatesHE8E29+hltHITCtfWithMaterialTracksHE8E29+hltHITIPTCorrectorHE8E29+hltIsolPixelTrackL3FilterHE8E29+HLTEndSequence)


HLT_DoubleJet40_MET70_Aco = cms.Path(HLTBeginSequenceBPTX+hltL1sDoubleJet40MET70Aco+hltPreDoubleJet40MET70Aco+HLTRecoJetMETSequence+hlt1MET70+hltPhiJet1Jet2Aco+HLTEndSequence)


HLT_DoubleJet125_MET60 = cms.Path(HLTBeginSequenceBPTX+hltL1sDoubleJet125MET60+hltPreDoubleJet125MET60+HLTRecoJetMETSequence+hlt1MET60+hlt2jet125New+HLTEndSequence)


HLT_MET25 = cms.Path(HLTBeginSequenceBPTX+hltL1sMET25+hltPreMET25+HLTRecoMETSequence+hlt1MET25+HLTEndSequence)


AlCa_RPCMuonNoHits = cms.Path(HLTBeginSequenceBPTX+hltL1sL1SingleMuOpenL1SingleMu0+hltPreRPCMuonNoHits+hltRPCMuonNoHitsL1Filtered0+HLTmuonlocalrecoSequence+HLTEndSequence)


HLT_L2Mu7_Photon9_L1R = cms.Path(HLTBeginSequenceBPTX+hltL1sL1Mu3EG5+hltPreL2Mu7Photon9L1R+hltL1Mu3EG5L1Filtered0+HLTL2muonrecoSequence+hltL2Filtered7L2Mu7Photon9L1R+HLTL2Mu7Photon9HLTNonIsoEGSequence+HLTEndSequence)


HLT_Ele25_SW_L1R = cms.Path(HLTBeginSequenceBPTX+hltL1sL1SingleEG8+hltPreEle25SWL1R+HLTSingleElectronEt15L1NonIsoHLTNonIsoSequence+hltL1NonIsoHLTNonIsoSingleElectronEt15EtFilterESet25+HLTEndSequence)


HLT_Ele15_SW_LooseTrackIso_L1R = cms.Path(HLTBeginSequenceBPTX+hltL1sL1SingleEG8+hltPreEle15SWLTIL1R+HLTSingleElectronEt15LTIL1NonIsoSequence+HLTEndSequence)


HLT_L1Mu14_L1SingleJet20 = cms.Path(HLTBeginSequenceBPTX+hltL1sL1Mu14L1Mu5Jet20+hltPreL1Mu14L1SingleJet15+HLTEndSequence)


HLT_IsoMu7_Jet40 = cms.Path(HLTBeginSequenceBPTX+hltL1sMuJets+hltPreIsoMu7Jet40+hltMuJetsL1Filtered+HLTL2muonrecoSequence+hltMuJetsL2PreFiltered+HLTL2muonisorecoSequence+hltMuJetsL2IsoFiltered+HLTL3muonrecoSequence+hltMuJetsL3PreFiltered+HLTL3muonisorecoSequence+hltMuJetsL3IsoFiltered+HLTDoCaloSequence+HLTDoJetRecoSequence+hltMuJetsHLT1jet40+HLTEndSequence)


HLT_DoublePhoton15_VeryLooseEcalIso_L1R = cms.Path(HLTBeginSequenceBPTX+hltL1sL1DoubleEG5+hltPreDoublePhoton15VLEIL1R+HLTDoublePhotonEt15L1NonIsoHLTVLEISequence+HLTEndSequence)


DoHLTElectronSiStrip = cms.Path(HLTBeginSequence+HLTDoRegionalEgammaEcalSequence+HLTL1IsolatedEcalClustersSequence+HLTL1NonIsolatedEcalClustersSequence+hltL1IsoRecoEcalCandidate+hltL1NonIsoRecoEcalCandidate+HLTDoLocalHcalWithoutHOSequence+hltL1IsolatedElectronHcalIsol+hltL1NonIsolatedElectronHcalIsol+HLTDoLocalPixelSequence+HLTDoLocalStripSequence+hltL1IsoSiStripElectronPixelSeeds+hltL1NonIsoSiStripElectronPixelSeeds+DoSSTracking)


HLT_IsoMu9 = cms.Path(HLTBeginSequenceBPTX+hltL1sL1SingleMu7+hltPreIsoMu9+hltSingleMuIsoL1Filtered7+HLTL2muonrecoSequence+hltSingleMuIsoL2PreFiltered7+HLTL2muonisorecoSequence+hltSingleMuIsoL2IsoFiltered7+HLTL3muonrecoSequence+hltSingleMuIsoL3PreFiltered9+HLTL3muonisorecoSequence+hltSingleMuIsoL3IsoFiltered9+HLTEndSequence)


HLT_IsoTrackHE_1E31 = cms.Path(HLTBeginSequenceBPTX+hltL1sIsoTrack1E31+hltPreIsoTrackHE1E31+HLTL2HcalIsolTrackSequenceHE+hltIsolPixelTrackProdHE1E31+hltIsolPixelTrackL2FilterHE1E31+HLTDoLocalStripSequence+hltHITPixelTripletSeedGeneratorHE1E31+hltHITCkfTrackCandidatesHE1E31+hltHITCtfWithMaterialTracksHE1E31+hltHITIPTCorrectorHE1E31+hltIsolPixelTrackL3FilterHE1E31+HLTEndSequence)


HLT_IsoMu3 = cms.Path(HLTBeginSequenceBPTX+hltL1sL1SingleMu3+hltPreIsoMu3+hltSingleMuIsoL1Filtered3+HLTL2muonrecoSequence+hltSingleMuIsoL2PreFiltered3+HLTL2muonisorecoSequence+hltSingleMuIsoL2IsoFiltered3+HLTL3muonrecoSequence+hltSingleMuIsoL3PreFiltered3+HLTL3muonisorecoSequence+hltSingleMuIsoL3IsoFiltered3+HLTEndSequence)


HLT_Ele10_LW_L1R = cms.Path(HLTBeginSequenceBPTX+hltL1sL1SingleEG5+hltPreEle10LWL1R+HLTSingleElectronLWEt10L1NonIsoHLTNonIsoSequence+HLTEndSequence)


HLT_HIPhoton10 = cms.Path(HLTBeginSequenceBPTX+hltHIL1sPhoton10+hltHIPrePhoton10+HLTDoCaloSequence+HLTDoHIEcalClusSequence+hltHIPhoton10+HLTEndSequence)


HLT_HIPhoton15 = cms.Path(HLTBeginSequenceBPTX+hltHIL1sPhoton15+hltHIPrePhoton15+HLTDoCaloSequence+HLTDoHIEcalClusSequence+hltHIPhoton15+HLTEndSequence)


HLT_IsoMu14_IsoTau15_Trk3_NoL1 = cms.Path(HLTBeginSequenceBPTX+hltL1sMuon5Jet20+hltPreIsoMu14IsoTauTrk3NoL1+hltMuonTauL1FilteredNoL1Tau+HLTL2muonrecoSequence+hltMuonTauIsoL2PreFilteredNoL1Tau+HLTL2muonisorecoSequence+hltMuonTauIsoL2IsoFilteredNoL1Tau+HLTDoLocalStripSequence+HLTL3muonrecoSequence+HLTL3muonisorecoSequence+hltMuonTauIsoL3PreFilteredNoL1Tau+hltMuonTauIsoL3IsoFilteredNoL1Tau+HLTL2TauJetsSequence+HLTL2TauEcalIsolationSequence+hltFilterL2EcalIsolationMuonTauNoL1Tau+HLTL25TauPixelTrackIsolation+hltFilterL25PixelTracksLeadingTrackPtCutMuonTauNoL1+hltFilterL25PixelTracksIsolationMuonTauNoL1Tau+HLTEndSequence)


HLT_HighMult40 = cms.Path(HLTBeginSequenceBPTX+hltL1sHighMult40+hltPreHighMult40+HLTDoLocalPixelSequence+HLTRecopixelvertexingForMinBiasSequence+hltPixelCands+hlt1HighMult40+HLTEndSequence)


HLT_L1SingleEG5 = cms.Path(HLTBeginSequenceBPTX+hltL1sL1SingleEG5+hltPreL1SingleEG5+HLTEndSequence)


HLT_Mu3 = cms.Path(HLTBeginSequenceBPTX+hltL1sL1SingleMuOpenL1SingleMu0L1SingleMu3+hltPreMu3+hltL1SingleMu0L1Filtered0+HLTL2muonrecoSequence+hltSingleMu3L2Filtered3+HLTL3muonrecoSequence+hltSingleMu3L3Filtered3+HLTEndSequence)


HLT_Mu5 = cms.Path(HLTBeginSequenceBPTX+hltL1sL1SingleMu3+hltPreMu5+hltL1SingleMu3L1Filtered0+HLTL2muonrecoSequence+hltSingleMu5L2Filtered4+HLTL3muonrecoSequence+hltSingleMu5L3Filtered5+HLTEndSequence)


HLT_Mu7 = cms.Path(HLTBeginSequenceBPTX+hltL1sL1SingleMu5+hltPreMu7+hltL1SingleMu5L1Filtered0+HLTL2muonrecoSequence+hltSingleMu7L2Filtered5+HLTL3muonrecoSequence+hltSingleMu7L3Filtered7+HLTEndSequence)


HLT_L2Mu0_NoVertex = cms.Path(HLTBeginSequence+hltL1sL1SingleMu0+hltPreL2Mu0NoVertex+hltSingleMu0L1Filtered+HLTL2muonrecoSequenceNoVtx+hltSingleL2Mu0L2PreFilteredNoVtx+HLTEndSequence)


HLT_L1SingleEG8 = cms.Path(HLTBeginSequenceBPTX+hltL1sL1SingleEG8+hltPreL1SingleEG8+HLTEndSequence)


HLT_Ele25_LW_L1R = cms.Path(HLTBeginSequenceBPTX+hltL1sL1SingleEG8+hltPreEle25LWL1R+HLTSingleElectronLWEt15L1NonIsoHLTNonIsoSequence+hltL1NonIsoHLTNonIsoSingleElectronLWEt15EtFilterESet25+HLTEndSequence)


HLT_DTErrors = cms.Path(hltPreAlCaDTErrors+hltDTROMonitorFilter+hltDynAlCaDTErrors+HLTEndSequence)


HLT_LogMonitor = cms.Path(hltPreLogMonitor+hltLogMonitorFilter+HLTEndSequence)


HLT_MinBiasPixel_DoubleIsoTrack5 = cms.Path(HLTBeginSequenceBPTX+hltPreMinBiasPixelDoubleIsoTrack5+HLTDoLocalPixelSequence+HLTPixelTrackingForMinBiasSequence+hltPixelCands+hltPixelMBForAlignment+HLTEndSequence)


HLT_L1_BPTX = cms.Path(HLTBeginSequence+hltL1sL1BPTX+hltPreL1BPTX+HLTEndSequence)


HLT_Ele10_SW_L1R_TripleJet30 = cms.Path(HLTBeginSequenceBPTX+hltL1sEG5TripleJet15+hltPreEle10SWL1RTripleJet30+HLTEle10SWL1RTripleJet30EGSequence+HLTRecoJetSequence+hlt3jet30Ele10SWL1RTripleJet30+HLTEndSequence)


HLT_IsoMu14_LooseIsoTau15_Trk3_NoL2 = cms.Path(HLTBeginSequenceBPTX+hltL1sMuon5Tau20+hltPreIsoMu14IsoTauTrk3NoL2+hltMuonTauL1Filtered+HLTL2muonrecoSequence+hltMuonTauIsoL2PreFiltered+HLTL2muonisorecoSequence+hltMuonTauIsoL2IsoFiltered+HLTDoLocalStripSequence+HLTL3muonrecoSequence+HLTL3muonisorecoSequence+hltMuonTauIsoL3PreFiltered+hltMuonTauIsoL3IsoFiltered+HLTL2TauJetsSequence+HLTL2TauEcalIsolationNoCutSequence+HLTL25TauPixelTracksIsolationNoL2+hltFilterL25PixelTracksLeadingTrackPtCutMuonTauNoL2+hltFilterL25PixelTracksIsolationMuonTauNoL2+HLTEndSequence)


HLT_Activity_PixelClusters = cms.Path(HLTBeginSequenceBPTX+hltPreActivityPixelClusters+HLTDoLocalPixelSequence+hltPixelActivityFilter+HLTEndSequence)


HLT_FwdJet40 = cms.Path(HLTBeginSequenceBPTX+hltL1sFwdJet40+hltPreFwdJet40+HLTRecoJetSequence+hltRapGap40+HLTEndSequence)


HLT_PhysicsDeclared = cms.Path(HLTBeginSequence+hltPhysicsDeclared+HLTEndSequence)


HLT_Ele10_SW_L1R = cms.Path(HLTBeginSequenceBPTX+hltL1sL1SingleEG5+hltPreEle10SWL1R+HLTSingleElectronEt10L1NonIsoHLTnonIsoSequence+HLTEndSequence)


DoHLT_Onia_1E31 = cms.Path(HLTBeginSequence+HLTL2muonrecoSequence+HLTL3muonrecoSequence+HLTOniaPixelTrackSequence+hltPixelTrackCands+hltOniaPixelTrackSelector+hltOniaPixelTrackCands+HLTOniaTrackSequence+HLTEndSequence)


HLT_Photon30_L1R_8E29 = cms.Path(HLTBeginSequenceBPTX+hltL1sL1SingleEG8+hltPrePhoton30L1R8E29+HLTSinglePhoton15L1NonIsolatedHLTNonIsoSequence+hltL1NonIsoHLTNonIsoSinglePhotonEt15EtFilterESet308E29+HLTEndSequence)


HLT_MinBiasPixel_DoubleTrack = cms.Path(HLTBeginSequenceBPTX+hltPreMinBiasPixelDoubleTrack+HLTDoLocalPixelSequence+HLTPixelTrackingForMinBiasSequence+hltPixelCands+hltMinBiasPixelFilter2+HLTEndSequence)


HLT_L1SingleEG20_NoBPTX = cms.Path(HLTBeginSequence+hltL1sL1SingleEG20+hltPreL1SingleEG20+HLTEndSequence)


HLT_Ele20_SW_EleId_LooseTrackIso_L1R = cms.Path(HLTBeginSequenceBPTX+hltL1sL1SingleEG8+hltPreEle20SWEleIdLTIL1R+HLTSingleElectronEt20L1NonIsoHLTIsoEleIdSequence+HLTEndSequence)


HLT_SumET120 = cms.Path(HLTBeginSequenceBPTX+hltL1sSumET120+hltPreSumET120+HLTRecoMETSequence+hlt1SumET120+HLTEndSequence)


HLT_MET60 = cms.Path(HLTBeginSequenceBPTX+hltL1sMET60+hltPreMET60+HLTRecoMETSequence+hlt1MET60+HLTEndSequence)


HLT_Ele20_SW_L1R = cms.Path(HLTBeginSequenceBPTX+hltL1sL1SingleEG8+hltPreEle20SWL1R+HLTSingleElectronEt20L1NonIsoHLTnonIsoSequence+HLTEndSequence)


HLT_IsoTrackHB_1E31 = cms.Path(HLTBeginSequenceBPTX+hltL1sIsoTrack1E31+hltPreIsoTrackHB1E31+HLTL2HcalIsolTrackSequenceHB+hltIsolPixelTrackProdHB1E31+hltIsolPixelTrackL2FilterHB1E31+HLTDoLocalStripSequence+hltHITPixelTripletSeedGeneratorHB1E31+hltHITCkfTrackCandidatesHB1E31+hltHITCtfWithMaterialTracksHB1E31+hltHITIPTCorrectorHB1E31+hltIsolPixelTrackL3FilterHB1E31+HLTEndSequence)


HLT_TrackPointing = cms.Path(HLTBeginSequenceBPTX+hltL1sTrackPointing+hltPreTrackPointing+HLTCosmicMuonReco+hltMuonPointingFilter+HLTEndSequence)


HLT_DoubleJet60_MET60_Aco = cms.Path(HLTBeginSequenceBPTX+hltL1sDoubleJet60MET60Aco+hltPreDoubleJet60MET60Aco+HLTRecoJetMETSequence+hlt1MET60+hltPhi2metAco+HLTEndSequence)


HLT_Activity_L1A = cms.Path(HLTBeginSequenceBPTX+hltLevel1Activity+hltPreActivityL1A+HLTEndSequence)


HLT_Jet30 = cms.Path(HLTBeginSequenceBPTX+hltL1sJet30+hltPreJet30+HLTRecoJetSequence+hlt1jet30+HLTEndSequence)


HLT_L2Mu9_DiJet30 = cms.Path(HLTBeginSequenceBPTX+hltL1sL1Mu5Jet20+hltPreL2Mu9DoubleJet30+hltL1Mu5Jet15L1Filtered0+HLTL2muonrecoSequence+hltL2Filtered9L2Mu9DoubleJet30+HLTRecoJetSequence+hlt2jet30L2Mu9DoubleJet30+HLTEndSequence)


HLT_RPCBarrelCosmics = cms.Path(HLTBeginSequence+hltL1sRPCBarrelCosmics+hltPreRPCBarrelCosmics+HLTEndSequence)


HLT_Photon15_L1R = cms.Path(HLTBeginSequenceBPTX+hltL1sL1SingleEG8+hltPrePhoton15L1R+HLTSinglePhoton15L1NonIsolatedHLTNonIsoSequence+HLTEndSequence)


HLT_Jet140 = cms.Path(HLTBeginSequenceBPTX+hltL1sJet140+hltPreJet140+HLTRecoJetRegionalSequence+hlt1jet140+HLTEndSequence)


AlCa_EcalEta_8E29 = cms.Path(HLTBeginSequenceBPTX+hltL1sAlCaEcalPi0Eta8E29+hltPreAlCaEcalEta8E29+HLTDoRegionalPi0EtaSequence+hltAlCaEtaRegRecHits+HLTEndSequence)


HLT_DoublePhoton5_Jpsi_L1R = cms.Path(HLTBeginSequenceBPTX+hltL1sL1SingleEG8orL1DoubleEG5+hltPreDoublePhoton5JpsiL1R+HLTDoublePhotonEt5JpsiSequence+HLTEndSequence)


HLT_DoubleMu3_SameSign = cms.Path(HLTBeginSequenceBPTX+hltL1sL1DoubleMu3+hltPreDoubleMu3SameSign+hltSameSignMuL1Filtered+HLTL2muonrecoSequence+hltSameSignMuL2PreFiltered+HLTL3muonrecoSequence+hltSameSignMuL3PreFiltered+HLTEndSequence)


HLT_HcalPhiSym = cms.Path(HLTBeginSequenceNZS+hltL1sHcalPhiSym+hltPreHcalPhiSym+HLTEndSequence)


HLT_HT300_MHT100 = cms.Path(HLTBeginSequenceBPTX+hltL1sHTMHT+hltPreHTMHT+HLTRecoJetSequence+HLTDoJet30HTRecoSequence+hltHT300+hltMhtHtFilter+HLTEndSequence)


HLT_Activity_EcalREM = cms.Path(HLTBeginSequenceBPTX+hltVetoL1SingleEG2+hltPreActivityEcal+hltEcalDigis+hltEcalFixedAlphaBetaFitUncalibRecHit+hltEcalRecHitAB+hltTowerMakerForEcalABFit+hltEcalTowerFilter+HLTEndSequence)


HLT_L1Jet6U = cms.Path(HLTBeginSequenceBPTX+hltL1sL1Jet6U+hltPreL1Jet6U+HLTEndSequence)


HLT_TkMu3_NoVertex = cms.Path(HLTBeginSequence+hltL1sL1SingleMu0+hltPreTkMu3NoVertex+hltSingleMu0L1Filtered+HLTL2muonrecoSequenceNoVtx+hltSingleMu3L2PreFilteredNoVtx+HLTL3muonrecoSequenceNoVtx+hltSingleMu3L3PreFilterNoVtx+HLTEndSequence)


HLT_L1DoubleMuOpen = cms.Path(HLTBeginSequenceBPTX+hltL1sL1DoubleMuOpen+hltPreL1DoubleMuOpen+hltDoubleMuLevel1PathL1OpenFiltered+HLTEndSequence)


HLT_MET60_HT350 = cms.Path(HLTBeginSequenceBPTX+hltL1sMET60HT350+hltPreMET60HT350+HLTRecoJetMETSequence+hlt1MET60+hlt1HT350+HLTEndSequence)


HLT_CSCBeamHaloRing2or3 = cms.Path(HLTBeginSequenceBPTX+hltL1sCSCBeamHaloRing2or3+hltPreCSCBeamHaloRing2or3+hltMuonCSCDigis+hltCsc2DRecHits+hltFilter23HLTCSCBeamHaloRing2or3+HLTEndSequence)


HLT_DiJetAve30U_1E31 = cms.Path(HLTBeginSequenceBPTX+hltL1sDiJetAve30U1E31+hltPreDiJetAve30U1E31+HLTDoCaloSequence+hltIterativeCone5CaloJets+hltDiJetAve30U1E31+HLTEndSequence)


HLT_Jet15U = cms.Path(HLTBeginSequenceBPTX+hltL1sJet15U+hltPreJet15U+HLTRecoJetSequenceU+hlt1jet15U+HLTEndSequence)


HLT_ZeroBias1kHz = cms.Path(HLTBeginSequence+hltL1sZeroBias+hltPreZeroBias1kHz+HLTEndSequence)


HLT_ForwardBSC = cms.Path(HLTBeginSequenceBPTX+hltL1sForwardBSC+hltPreForwardBSC+HLTEndSequence)


HLT_L1Mu30 = cms.Path(HLTBeginSequenceBPTX+hltL1sL1SingleMu20+hltPreL1Mu30+hltL1Mu30L1Filtered30+HLTEndSequence)


HLT_StoppedHSCP_8E29 = cms.Path(HLTBeginSequence+hltL1sStoppedHSCP8E29+hltPreStoppedHSCP8E29+hltHcalDigis+hltHbhereco+hltStoppedHSCPHpdFilter+hltStoppedHSCPTowerMakerForAll+hltStoppedHSCPIterativeCone5CaloJets+hltStoppedHSCP1CaloJetEnergy+HLTEndSequence)


DoHLTTau = cms.Path(HLTBeginSequence+openhltTauPrescaler+OpenHLTCaloTausCreatorSequence+openhltL2TauJets+openhltL2TauIsolationProducer+HLTDoLocalPixelSequence+HLTRecopixelvertexingSequence+OpenHLTL25TauTrackReconstructionSequence+OpenHLTL25TauTrackIsolation+TauOpenHLT+HLTEndSequence)


HLT_EcalCalibration = cms.Path(hltCalibrationEventsFilter+hltL1EventNumber+hltPreEcalCalibration+hltEcalCalibrationRaw+HLTEndSequence)


HLT_Photon25_L1R = cms.Path(HLTBeginSequenceBPTX+hltL1sL1SingleEG8+hltPrePhoton25L1R+HLTSinglePhoton25L1NonIsolatedHLTNonIsoSequence+HLTEndSequence)


HLT_Jet180_MET60 = cms.Path(HLTBeginSequenceBPTX+hltL1sJet180MET60+hltPreJet180MET60+HLTRecoJetMETSequence+hlt1MET60+hlt1jet180+HLTEndSequence)


HLT_NoL2IsoMu8_Jet40 = cms.Path(HLTBeginSequenceBPTX+hltL1sMuNoL2IsoJets+hltPreNoL2IsoMu8Jet40+hltMuNoL2IsoJetsL1Filtered+HLTL2muonrecoSequence+hltMuNoL2IsoJetsL2PreFiltered+HLTL3muonrecoSequence+hltMuNoL2IsoJetsL3PreFiltered+HLTL3muonisorecoSequence+hltMuNoL2IsoJetsL3IsoFiltered+HLTDoCaloSequence+HLTDoJetRecoSequence+hltMuNoL2IsoJetsHLT1jet40+HLTEndSequence)


HLT_TrackerCosmics = cms.Path(HLTBeginSequence+hltL1sTrackerCosmics+hltPreTrackerCosmics+HLTEndSequence)


HLT_BackwardBSC = cms.Path(HLTBeginSequenceBPTX+hltL1sBackwardBSC+hltPreBackwardBSC+HLTEndSequence)


HLT_HcalNZS_1E31 = cms.Path(HLTBeginSequenceNZS+hltL1sHcalNZS1E31+hltPreHcalNZS1E31+HLTEndSequence)


AlCa_EcalEta_1E31 = cms.Path(HLTBeginSequenceBPTX+hltL1sAlCaEcalPi0Eta1E31+hltPreAlCaEcalEta1E31+HLTDoRegionalPi0EtaSequence+hltAlCaEtaRegRecHits+HLTEndSequence)


HLT_DoubleMu3_Upsilon = cms.Path(HLTBeginSequenceBPTX+hltL1sL1DoubleMu3+hltPreDoubleMu3Upsilon+hltUpsilonMML1Filtered+HLTL2muonrecoSequence+hltUpsilonMML2Filtered+HLTL3muonrecoSequence+hltUpsilonMML3Filtered+HLTEndSequence)


DoHLTAlCaECALPhiSym = cms.Path(HLTBeginSequence+hltEcalRawToRecHitFacility+hltESRawToRecHitFacility+hltEcalRegionalRestFEDs+hltEcalRecHitAll+HLTEndSequence)


HLT_L1SingleEG2_NoBPTX = cms.Path(HLTBeginSequence+hltL1sL1SingleEG2+hltPreSingleEG2NoBPTX+HLTEndSequence)


HLT_MET50 = cms.Path(HLTBeginSequenceBPTX+hltL1sMET50+hltPreMET50+HLTRecoMETSequence+hlt1MET50+HLTEndSequence)


HLT_DiJetAve70U = cms.Path(HLTBeginSequenceBPTX+hltL1sDiJetAve70U+hltPreDiJetAve70U+HLTDoCaloSequence+hltIterativeCone5CaloJets+hltDiJetAve70U+HLTEndSequence)


HLT_DiJetAve15U_8E29 = cms.Path(HLTBeginSequenceBPTX+hltL1sDiJetAve15U8E29+hltPreDiJetAve15U8E29+HLTDoCaloSequence+hltIterativeCone5CaloJets+hltDiJetAve15U8E29+HLTEndSequence)


HLT_Jet180 = cms.Path(HLTBeginSequenceBPTX+hltL1sJet180+hltPreJet180+HLTRecoJetRegionalSequence+hlt1jet180regional+HLTEndSequence)


HLT_Jet60_MET70_Aco = cms.Path(HLTBeginSequenceBPTX+hltL1sJet60MET70Aco+hltPreJet60MET70Aco+HLTRecoJetMETSequence+hlt1MET70+hltPhiJet1metAco+HLTEndSequence)


HLT_Photon10_LooseEcalIso_TrackIso_L1R = cms.Path(HLTBeginSequenceBPTX+hltL1sL1SingleEG5+hltPrePhoton10HLTLEITIL1R+HLTSinglePhoton10L1NonIsolatedHLTLEITISequence+HLTEndSequence)


HLT_L2Mu9 = cms.Path(HLTBeginSequenceBPTX+hltL1sL1SingleMu7+hltPreL2Mu9+hltL1SingleMu7L1Filtered0+HLTL2muonrecoSequence+hltL2Mu9L2Filtered9+HLTEndSequence)


HLT_IsoMu7_BTagMu_Jet20 = cms.Path(HLTBeginSequenceBPTX+hltL1sMuB+hltPreIsoMu7BTagMuJet20+hltMuBSoftL1Filtered+HLTL2muonrecoSequence+hltMuBSoftIsoL2PreFiltered+HLTL2muonisorecoSequence+hltMuBSoftIsoL2IsoFiltered+HLTBCommonL2recoSequence+HLTBSoftmuonL25recoSequence+hltBSoftmuonL25filter+HLTL3muonrecoSequence+hltMuBSoftIsoL3PreFiltered+HLTL3muonisorecoSequence+hltMuBSoftIsoL3IsoFiltered+HLTBSoftmuonL3recoSequence+hltBSoftmuonL3filter+HLTEndSequence)


HLT_DoubleFwdJet50 = cms.Path(HLTBeginSequenceBPTX+hltL1sDoubleFwdJet50+hltPreDoubleFwdJet50+HLTRecoJetSequence+hlt2jetGapFilter+HLTEndSequence)


HLT_L1Tech_HCAL_HF_coincidence_PM = cms.Path(HLTBeginSequenceBPTX+hltL1sL1TechHFcoincidencePM+hltPreL1TechHFcoincidencePM+HLTEndSequence)


DoHLTBTag = cms.Path(HLTBeginSequence+HLTBCommonL2recoSequence+OpenHLTBLifetimeL25recoSequence+OpenHLTBSoftmuonL25recoSequence+OpenHLTBLifetimeL3recoSequence+OpenHLTBLifetimeL3recoSequenceStartup+OpenHLTBSoftmuonL3recoSequence+HLTEndSequence)


AlCa_EcalPi0_1E31 = cms.Path(HLTBeginSequenceBPTX+hltL1sAlCaEcalPi0Eta1E31+hltPreAlCaEcalPi01E31+HLTDoRegionalPi0EtaSequence+hltAlCaPi0RegRecHits+HLTEndSequence)


HLT_Jet250 = cms.Path(HLTBeginSequenceBPTX+hltL1sJet250+hltPreJet250+HLTRecoJetRegionalSequence+hlt1jet250+HLTEndSequence)


HLT_IsoMu15 = cms.Path(HLTBeginSequenceBPTX+hltL1sL1SingleMu10+hltPreIsoMu15+hltSingleMuIsoL1Filtered10+HLTL2muonrecoSequence+hltSingleMuIsoL2PreFiltered12+HLTL2muonisorecoSequence+hltSingleMuIsoL2IsoFiltered12+HLTL3muonrecoSequence+hltSingleMuIsoL3PreFiltered15+HLTL3muonisorecoSequence+hltSingleMuIsoL3IsoFiltered15+HLTEndSequence)


HLT_IsoMu11 = cms.Path(HLTBeginSequenceBPTX+hltL1sL1SingleMu7+hltPreIsoMu11+hltSingleMuIsoL1Filtered7+HLTL2muonrecoSequence+hltSingleMuIsoL2PreFiltered9+HLTL2muonisorecoSequence+hltSingleMuIsoL2IsoFiltered9+HLTL3muonrecoSequence+hltSingleMuIsoL3PreFiltered11+HLTL3muonisorecoSequence+hltSingleMuIsoL3IsoFiltered11+HLTEndSequence)


HLT_IsoMu13 = cms.Path(HLTBeginSequenceBPTX+hltL1sL1SingleMu10+hltPreIsoMu13+hltSingleMuIsoL1Filtered10+HLTL2muonrecoSequence+hltSingleMuIsoL2PreFiltered11+HLTL2muonisorecoSequence+hltSingleMuIsoL2IsoFiltered11+HLTL3muonrecoSequence+hltSingleMuIsoL3PreFiltered13+HLTL3muonisorecoSequence+hltSingleMuIsoL3IsoFiltered13+HLTEndSequence)


HLT_MinBiasHcal = cms.Path(HLTBeginSequenceBPTX+hltL1sMinBiasHcal+hltPreMinBiasHcal+HLTEndSequence)


HLT_L1DoubleEG5 = cms.Path(HLTBeginSequenceBPTX+hltL1sL1DoubleEG5+hltPreL1DoubleEG5+HLTEndSequence)


HLTriggerFinalPath = cms.Path(hltTriggerSummaryAOD+hltPreTriggerSummaryRAW+hltTriggerSummaryRAW+hltBoolFinalPath)


HLT_IsoTrackHB_8E29 = cms.Path(HLTBeginSequenceBPTX+hltL1sIsoTrack8E29+hltPreIsoTrackHB8E29+HLTL2HcalIsolTrackSequenceHB+hltIsolPixelTrackProdHB8E29+hltIsolPixelTrackL2FilterHB8E29+HLTDoLocalStripSequence+hltHITPixelTripletSeedGeneratorHB8E29+hltHITCkfTrackCandidatesHB8E29+hltHITCtfWithMaterialTracksHB8E29+hltHITIPTCorrectorHB8E29+hltIsolPixelTrackL3FilterHB8E29+HLTEndSequence)


HLT_Photon15_TrackIso_L1R = cms.Path(HLTBeginSequenceBPTX+hltL1sL1SingleEG8+hltPrePhoton15HTIL1R+HLTSinglePhoton15L1NonIsolatedHLTHTISequence+HLTEndSequence)


HLT_BTagIP_Jet80 = cms.Path(HLTBeginSequenceBPTX+hltL1sBTagIPJet80+hltPreBTagIPJet80+HLTRecoJetRegionalSequence+hltBJet80+HLTBTagIPSequenceL25Startup+hltBLifetimeL25FilterStartup+HLTBTagIPSequenceL3Startup+hltBLifetimeL3FilterStartup+HLTEndSequence)


HLT_QuadJet35_MET60 = cms.Path(HLTBeginSequenceBPTX+hltL1sQuadJet35MET60+hltPreQuadJet35MET60+HLTRecoJetMETSequence+hlt1MET60+hlt4jet35+HLTEndSequence)


HLT_HIPhoton20 = cms.Path(HLTBeginSequenceBPTX+hltHIL1sPhoton20+hltHIPrePhoton20+HLTDoCaloSequence+HLTDoHIEcalClusSequence+hltHIPhoton20+HLTEndSequence)


AlCa_EcalPhiSym = cms.Path(HLTBeginSequenceBPTX+hltL1sAlCaEcalPhiSym+hltPreAlCaEcalPhiSym+hltEcalRawToRecHitFacility+hltESRawToRecHitFacility+hltEcalRegionalRestFEDs+hltEcalRecHitAll+hltAlCaPhiSymStream+HLTEndSequence)


HLT_L1_HFtech = cms.Path(HLTBeginSequenceBPTX+hltL1sL1HFtech+hltPreL1HFTech+HLTEndSequence)


HLT_L1Mu14_L1ETM30 = cms.Path(HLTBeginSequenceBPTX+hltL1sL1Mu14L1ETM30+hltPreL1Mu14L1ETM30+HLTEndSequence)


HLT_QuadJet60 = cms.Path(HLTBeginSequenceBPTX+hltL1sQuadJet60+hltPreQuadJet60+HLTRecoJetSequence+hlt4jet60+HLTEndSequence)


HLT_DoubleLooseIsoTau15_Trk5 = cms.Path(HLTBeginSequenceBPTX+hltL1sDoubleLooseIsoTau15Trk5+hltPreDoubleLooseIsoTau15Trk5+HLTL2TauJetsSequence+hltFilterL2EtCutDoubleLooseIsoTau15Trk5+HLTL2TauEcalIsolationSequence+hltFilterL2EcalIsolationDoubleLooseIsoTau15Trk5+HLTDoLocalPixelSequence+HLTRecopixelvertexingSequence+HLTL25TauTrackReconstructionSequence+HLTL25TauTrackIsolationSequence+hltL1HLTDoubleLooseIsoTau15Trk5JetsMatch+hltFilterL25LeadingTrackPtCutDoubleLooseIsoTau15Trk5+HLTL3TauTrackReconstructionSequence+HLTL3TauTrackIsolationSequence+hltL1HLTDoubleLooseIsoTau15Trk5L3JetsMatch+hltFilterL3IsolationCutDoubleLooseIsoTau15Trk5+HLTEndSequence)


HLT_DoubleJet50_MET70_Aco = cms.Path(HLTBeginSequenceBPTX+hltL1sDoubleJet50MET70Aco+hltPreDoubleJet50MET70Aco+HLTRecoJetMETSequence+hlt1MET70+hltPhiJet2metAco+HLTEndSequence)


HLT_Ele15_SC15_SW_EleId_L1R = cms.Path(HLTBeginSequenceBPTX+hltL1sL1SingleEG8+hltPreEle15SWEleIdSC15L1R+HLTSingleElectronEt15L1NonIsoHLTEleIdSequence+hltL1NonIsoHLTNonIsoSingleElectronEt15LTIESscWrapper+hltL1NonIsoHLTNonIsoSingleElectronEt15EleIdESDoubleSC15+HLTEndSequence)


DoHLTPhoton = cms.Path(HLTBeginSequence+HLTDoRegionalEgammaEcalSequence+HLTL1IsolatedEcalClustersSequence+HLTL1NonIsolatedEcalClustersSequence+hltL1IsoRecoEcalCandidate+hltL1NonIsoRecoEcalCandidate+hltL1IsolatedPhotonEcalIsol+hltL1NonIsolatedPhotonEcalIsol+HLTDoLocalHcalWithoutHOSequence+hltL1IsolatedPhotonHcalIsol+hltL1NonIsolatedPhotonHcalIsol+HLTDoLocalTrackerSequence+HLTL1IsoEgammaRegionalRecoTrackerSequence+HLTL1NonIsoEgammaRegionalRecoTrackerSequence+hltL1IsoPhotonHollowTrackIsol+hltL1NonIsoPhotonHollowTrackIsol)


HLT_L2Mu15 = cms.Path(HLTBeginSequenceBPTX+hltL1sL1SingleMu7+hltPreL2Mu15+hltL1SingleMu7L1Filtered0+HLTL2muonrecoSequence+hltL2Mu15L2Filtered15+HLTEndSequence)


HLT_QuadJet30 = cms.Path(HLTBeginSequenceBPTX+hltL1sQuadJet30+hltPreQuadJet30+HLTRecoJetSequence+hlt4jet30+HLTEndSequence)


HLT_MultiTowerEcal = cms.Path(HLTBeginSequenceBPTX+hltPreMultiTowerEcal+hltL1sSingleEG1+HLTDoCaloSequence+hltTowerMakerForEcal+hltMultiTowerFilter+HLTEndSequence)


HLT_DoublePhoton5_Upsilon_L1R = cms.Path(HLTBeginSequenceBPTX+hltL1sL1SingleEG8orL1DoubleEG5+hltPreDoublePhoton5UpsL1R+HLTDoublePhotonEt5UpsSequence+HLTEndSequence)


AlCa_EcalPi0_8E29 = cms.Path(HLTBeginSequenceBPTX+hltL1sAlCaEcalPi0Eta8E29+hltPreAlCaEcalPi08E29+HLTDoRegionalPi0EtaSequence+hltAlCaPi0RegRecHits+HLTEndSequence)


HLT_MinBiasBSC_OR = cms.Path(HLTBeginSequenceBPTX+hltL1sMinBiasBSCOR+hltPreMinBiasBSCOR+HLTEndSequence)


HLT_L1_BscMinBiasOR_BptxPlusORMinus = cms.Path(HLTBeginSequenceBPTX+hltL1sL1BscMinBiasORBptxPlusORMinus+hltPreL1BscMinBiasORBptxPlusORMinus+HLTEndSequence)


HLT_DoubleFwdJet40_MET60 = cms.Path(HLTBeginSequenceBPTX+hltL1sDoubleFwdJet40MET60+hltPreDoubleFwdJet40MET60+HLTRecoJetMETSequence+hlt1MET60+hlt2jetvbf+HLTEndSequence)


HLT_BTagIP_Jet120 = cms.Path(HLTBeginSequenceBPTX+hltL1sBTagIPJet120+hltPreBTagIPJet120+HLTRecoJetRegionalSequence+hltBJet120+HLTBTagIPSequenceL25Startup+hltBLifetimeL25FilterStartup+HLTBTagIPSequenceL3Startup+hltBLifetimeL3FilterStartup+HLTEndSequence)


HLT_DoubleMu3 = cms.Path(HLTBeginSequenceBPTX+hltL1sL1DoubleMu3+hltPreDoubleMu3+hltDiMuonL1Filtered+HLTL2muonrecoSequence+hltDiMuonL2PreFiltered+HLTL3muonrecoSequence+hltDiMuonL3PreFiltered+HLTEndSequence)


HLT_DoubleMu0 = cms.Path(HLTBeginSequenceBPTX+hltL1sL1DoubleMuOpen+hltPreDoubleMu0+hltDiMuonL1Filtered0+HLTL2muonrecoSequence+hltDiMuonL2PreFiltered0+HLTL3muonrecoSequence+hltDiMuonL3PreFiltered0+HLTEndSequence)


HLT_Activity_DT = cms.Path(HLTBeginSequenceBPTX+hltPreActivityDT+hltMuonDTDigis+hltDTTFUnpacker+hltDTActivityFilter+HLTEndSequence)


HLT_Random = cms.Path(hltRandomEventsFilter+hltL1EventNumber+hltPreRandom+HLTEndSequence)


HLT_SplashBSC = cms.Path(HLTBeginSequenceBPTX+hltL1sSplashBSC+hltPreSplashBSC+HLTEndSequence)


HLT_DoubleEle5_SW_Upsilon_L1R = cms.Path(HLTBeginSequenceBPTX+hltL1sL1SingleEG8orL1DoubleEG5+hltPreDoubleEle5UpsL1R+HLTDoubleElectronEt5UpsSequence+HLTEndSequence)


HLT_Jet50 = cms.Path(HLTBeginSequenceBPTX+hltL1sJet50+hltPreJet50+HLTRecoJetSequence+hlt1jet50+HLTEndSequence)


HLT_L1SingleEG5_NoBPTX = cms.Path(HLTBeginSequence+hltL1sL1SingleEG5+hltPreL1SingleEG5_NoBPTX+HLTEndSequence)


HLT_HT200 = cms.Path(HLTBeginSequenceBPTX+hltL1sHT200+hltPreHT200+HLTRecoJetSequence+HLTDoJet30HTRecoSequence+hltHT200+HLTEndSequence)


HLT_DoubleMu3_JPsi = cms.Path(HLTBeginSequenceBPTX+hltL1sL1DoubleMu3+hltPreDoubleMu3JPsi+hltJpsiMML1Filtered+HLTL2muonrecoSequence+hltJpsiMML2Filtered+HLTL3muonrecoSequence+hltJpsiMML3Filtered+HLTEndSequence)


DoHLTElectronLargeWindows = cms.Path(HLTBeginSequence+HLTDoRegionalEgammaEcalSequence+HLTL1IsolatedEcalClustersSequence+HLTL1NonIsolatedEcalClustersSequence+hltL1IsoRecoEcalCandidate+hltL1NonIsoRecoEcalCandidate+HLTDoLocalHcalWithoutHOSequence+hltL1IsolatedElectronHcalIsol+hltL1NonIsolatedElectronHcalIsol+HLTDoLocalPixelSequence+HLTDoLocalStripSequence+hltL1IsoLargeWindowElectronPixelSeeds+hltL1NonIsoLargeWindowElectronPixelSeeds+DoLWTracking)


HLT_Ele10_LW_EleId_L1R = cms.Path(HLTBeginSequenceBPTX+hltL1sL1SingleEG5+hltPreEle10LWEleIdL1R+HLTSingleElectronEt10L1NonIsoHLTEleIdSequence+HLTEndSequence)


HLT_DoubleMu3_Vtx2mm = cms.Path(HLTBeginSequenceBPTX+hltL1sL1DoubleMu3+hltPreDoubleMu3Vtx2mm+hltDiMuonL1Filtered+HLTL2muonrecoSequence+hltDiMuonL2PreFiltered+HLTL3muonrecoSequence+hltDiMuonL3PreFilteredRelaxedVtx2mm+HLTEndSequence)


HLT_Photon70_L1R = cms.Path(HLTBeginSequenceBPTX+hltL1sL1SingleEG8+hltPrePhoton70L1R+HLTSinglePhoton30L1NonIsolatedHLTNonIsoSequence+hltL1NonIsoHLTNonIsoSinglePhotonEt30EtFilterESet70+HLTEndSequence)


HLT_MinBias = cms.Path(HLTBeginSequenceBPTX+hltL1sMinBias+hltPreMinBias+HLTEndSequence)


HLT_FwdJet20U = cms.Path(HLTBeginSequenceBPTX+hltL1sFwdJet20U+hltPreFwdJet20U+HLTRecoJetSequenceU+hltRapGap20U+HLTEndSequence)


HLT_L1MET20 = cms.Path(HLTBeginSequenceBPTX+hltL1sL1MET20+hltPreL1MET20+HLTEndSequence)


HLT_DiJetAve50U = cms.Path(HLTBeginSequenceBPTX+hltL1sDiJetAve50U+hltPreDiJetAve50U+HLTDoCaloSequence+hltIterativeCone5CaloJets+hltDiJetAve50U+HLTEndSequence)


DoHltMuon = cms.Path(HLTBeginSequence+HLTL2muonrecoSequence+HLTL2muonisorecoSequence+HLTL3muonrecoSequence+HLTL3muonisorecoSequence+HLTEndSequence)


HLT_L1SingleEG2 = cms.Path(HLTBeginSequenceBPTX+hltL1sL1SingleEG2+hltPreL1SingleEG2+HLTEndSequence)


HLT_L1Mu14_L1ETM40 = cms.Path(HLTBeginSequenceBPTX+hltL1sL1Mu14L1ETM40+hltPreL1Mu14L1ETM40+HLTEndSequence)


HLT_Mu9 = cms.Path(HLTBeginSequenceBPTX+hltL1sL1SingleMu7+hltPreMu9+hltL1SingleMu7L1Filtered0+HLTL2muonrecoSequence+hltSingleMu9L2Filtered7+HLTL3muonrecoSequence+hltSingleMu9L3Filtered9+HLTEndSequence)


HLT_L1_BPTX_MinusOnly = cms.Path(HLTBeginSequence+hltL1sL1BPTX+hltL1sL1BPTXMinusOnly+hltPreL1BPTXMinusOnly+HLTEndSequence)


HLT_Photon20_LooseEcalIso_TrackIso_L1R = cms.Path(HLTBeginSequenceBPTX+hltL1sL1SingleEG8+hltPrePhoton20HLTLEITIL1R+HLTSinglePhoton20L1NonIsolatedHLTLEITISequence+HLTEndSequence)


HLT_DoubleLooseIsoTau15 = cms.Path(HLTBeginSequenceBPTX+hltL1sDoubleLooseIsoTau15+hltPreDoubleLooseIsoTau15+HLTL2TauJetsSequence+hltFilterL2EtCutDoubleLooseIsoTau15+HLTL2TauEcalIsolationSequence+hltL1HLTDoubleLooseIsoTau15JetsMatch+hltFilterL2EcalIsolationDoubleLooseIsoTau15+HLTEndSequence)


HLT_Ele20_LW_L1R = cms.Path(HLTBeginSequenceBPTX+hltL1sL1SingleEG8+hltPreEle20LWL1R+HLTSingleElectronLWEt15L1NonIsoHLTNonIsoSequence+hltL1NonIsoHLTNonIsoSingleElectronLWEt15EtFilterESet20+HLTEndSequence)


HLT_L1Mu20HQ = cms.Path(HLTBeginSequenceBPTX+hltL1sL1SingleMu20+hltPreL1Mu20HQ+hltL1Mu20HQL1Filtered20+HLTEndSequence)


HLT_Jet80 = cms.Path(HLTBeginSequenceBPTX+hltL1sJet80+hltPreJet80+HLTRecoJetRegionalSequence+hlt1jet80+HLTEndSequence)


HLT_SplashEcalSumET = cms.Path(HLTBeginSequence+hltL1sSplashEcalSumET+hltPreSplashEcalSumET+HLTRecoMETSequence+hltSplashEcalEtSum+HLTEndSequence)


HLT_StoppedHSCP_1E31 = cms.Path(HLTBeginSequence+hltL1sStoppedHSCP1E31+hltPreStoppedHSCP1E31+hltHcalDigis+hltHbhereco+hltStoppedHSCPHpdFilter+hltStoppedHSCPTowerMakerForAll+hltStoppedHSCPIterativeCone5CaloJets+hltStoppedHSCP1CaloJetEnergy+HLTEndSequence)


HLT_Ele15_SC15_SW_LooseTrackIso_L1R = cms.Path(HLTBeginSequenceBPTX+hltL1sL1SingleEG8+hltPreEle15SC15SWLTIL1R+HLTSingleElectronEt15LTIL1NonIsoSequence+hltL1NonIsoHLTNonIsoSingleElectronEt15LTIESscWrapper+hltL1NonIsoHLTNonIsoSingleElectronEt15LTIESDoubleSC15+HLTEndSequence)


HLT_MET100 = cms.Path(HLTBeginSequenceBPTX+hltL1sMET100+hltPreMET100+HLTRecoMETSequence+hlt1MET100+HLTEndSequence)


HLT_DiJetAve15U_1E31 = cms.Path(HLTBeginSequenceBPTX+hltL1sDiJetAve15U1E31+hltPreDiJetAve15U1E31+HLTDoCaloSequence+hltIterativeCone5CaloJets+hltDiJetAve15U1E31+HLTEndSequence)


HLT_TechTrigHCALNoise = cms.Path(HLTBeginSequence+hltL1sTechTrigHCALNoise+hltPreTechTrigHCALNoise+HLTEndSequence)


HLT_DoubleJet150 = cms.Path(HLTBeginSequenceBPTX+hltL1sDoubleJet150+hltPreDoubleJet150+HLTRecoJetRegionalSequence+hlt2jet150+HLTEndSequence)


HLT_Jet50U = cms.Path(HLTBeginSequenceBPTX+hltL1sJet50U+hltPreJet50U+HLTRecoJetSequenceU+hlt1jet50U+HLTEndSequence)


HLT_ZeroBiasPixel_SingleTrack = cms.Path(HLTBeginSequence+hltL1sZeroBias+hltPreZeroBiasPixelSingleTrack+HLTDoLocalPixelSequence+HLTPixelTrackingForMinBiasSequence+hltPixelCands+hltMinBiasPixelFilter1+HLTEndSequence)


HLTriggerFirstPath = cms.Path(hltGetRaw+HLTBeginSequenceBPTX+hltPreFirstPath+hltBoolFirstPath)


HLT_DiJetAve130U = cms.Path(HLTBeginSequenceBPTX+hltL1sDiJetAve130U+hltPreDiJetAve130U+HLTDoCaloSequence+hltIterativeCone5CaloJets+hltDiJetAve130U+HLTEndSequence)


HLT_MET45 = cms.Path(HLTBeginSequenceBPTX+hltL1sMET45+hltPreMET45+HLTRecoMETSequence+hlt1MET45+HLTEndSequence)


HLT_TripleJet85 = cms.Path(HLTBeginSequenceBPTX+hltL1sTripleJet85+hltPreTripleJet85+HLTRecoJetRegionalSequence+hlt3jet85+HLTEndSequence)


HLT_Activity_Ecal = cms.Path(HLTBeginSequenceBPTX+hltVetoL1SingleEG2+hltPreActivityCaloTower+HLTDoCaloSequence+hltTowerMakerForEcal+hltCaloTowerFilter+HLTEndSequence)


HLT_HcalNZS_8E29 = cms.Path(HLTBeginSequenceNZS+hltL1sHcalNZS8E29+hltPreHcalNZS8E29+HLTEndSequence)


HLT_Ele20_SiStrip_L1R = cms.Path(HLTBeginSequenceBPTX+hltL1sL1SingleEG8+hltPreEle20SiStripL1R+HLTDoRegionalEgammaEcalSequence+HLTL1IsolatedEcalClustersSequence+HLTL1NonIsolatedEcalClustersSequence+hltL1IsoRecoEcalCandidate+hltL1NonIsoRecoEcalCandidate+hltL1NonIsoHLTNonIsoSingleElectronSiStripEt20L1MatchFilterRegional+hltL1NonIsoHLTNonIsoSingleElectronSiStripEt20EtFilter+HLTDoLocalHcalWithoutHOSequence+hltL1IsolatedElectronHcalIsol+hltL1NonIsolatedElectronHcalIsol+hltL1NonIsoHLTNonIsoSingleElectronSiStripEt20HcalIsolFilter+HLTDoLocalPixelSequence+HLTDoLocalStripSequence+hltL1IsoSiStripElectronPixelSeeds+hltL1NonIsoSiStripElectronPixelSeeds+hltL1NonIsoHLTNonIsoSingleElectronSiStripEt20PixelMatchFilter+HLTEndSequence)


HLT_L1MuOpen_NoBPTX = cms.Path(HLTBeginSequence+hltL1sL1SingleMuOpenL1SingleMu0+hltPreL1MuOpen+hltL1MuOpenL1Filtered0+HLTEndSequence)


HLT_ZeroBias = cms.Path(HLTBeginSequence+hltL1sZeroBias+hltPreZeroBias+HLTEndSequence)


#analyzeThis = cms.Path(hltanalysis)


HLTAnalyzerEndpath = cms.EndPath(hltL1GtTrigReport+hltTrigReport)


# MessageLogger = cms.Service("MessageLogger",
#     suppressInfo = cms.untracked.vstring(),
#     debugs = cms.untracked.PSet(
#         placeholder = cms.untracked.bool(True)
#     ),
#     suppressDebug = cms.untracked.vstring(),
#     cout = cms.untracked.PSet(
#         placeholder = cms.untracked.bool(True)
#     ),
#     cerr_stats = cms.untracked.PSet(
#         threshold = cms.untracked.string('WARNING'),
#         output = cms.untracked.string('cerr'),
#         optionalPSet = cms.untracked.bool(True)
#     ),
#     warnings = cms.untracked.PSet(
#         placeholder = cms.untracked.bool(True)
#     ),
#     default = cms.untracked.PSet(

#     ),
#     statistics = cms.untracked.vstring('cerr_stats'),
#     cerr = cms.untracked.PSet(
#         INFO = cms.untracked.PSet(
#             limit = cms.untracked.int32(0)
#         ),
#         noTimeStamps = cms.untracked.bool(False),
#         FwkReport = cms.untracked.PSet(
#             reportEvery = cms.untracked.int32(100),
#             optionalPSet = cms.untracked.bool(True),
#             limit = cms.untracked.int32(10000000)
#         ),
#         default = cms.untracked.PSet(
#             limit = cms.untracked.int32(10000000)
#         ),
#         Root_NoDictionary = cms.untracked.PSet(
#             optionalPSet = cms.untracked.bool(True),
#             limit = cms.untracked.int32(0)
#         ),
#         threshold = cms.untracked.string('INFO'),
#         FwkJob = cms.untracked.PSet(
#             optionalPSet = cms.untracked.bool(True),
#             limit = cms.untracked.int32(0)
#         ),
#         FwkSummary = cms.untracked.PSet(
#             reportEvery = cms.untracked.int32(1),
#             optionalPSet = cms.untracked.bool(True),
#             limit = cms.untracked.int32(10000000)
#         ),
#         optionalPSet = cms.untracked.bool(True)
#     ),
#     FrameworkJobReport = cms.untracked.PSet(
#         default = cms.untracked.PSet(
#             limit = cms.untracked.int32(0)
#         ),
#         optionalPSet = cms.untracked.bool(True),
#         FwkJob = cms.untracked.PSet(
#             optionalPSet = cms.untracked.bool(True),
#             limit = cms.untracked.int32(10000000)
#         )
#     ),
#     suppressWarning = cms.untracked.vstring(),
#     errors = cms.untracked.PSet(
#         placeholder = cms.untracked.bool(True)
#     ),
#     destinations = cms.untracked.vstring('warnings', 
#         'errors', 
#         'infos', 
#         'debugs', 
#         'cout', 
#         'cerr'),
#     debugModules = cms.untracked.vstring(),
#     infos = cms.untracked.PSet(
#         optionalPSet = cms.untracked.bool(True),
#         Root_NoDictionary = cms.untracked.PSet(
#             optionalPSet = cms.untracked.bool(True),
#             limit = cms.untracked.int32(0)
#         ),
#         placeholder = cms.untracked.bool(True)
#     ),
#     categories = cms.untracked.vstring('FwkJob', 
#         'FwkReport', 
#         'FwkSummary', 
#         'Root_NoDictionary'),
#     fwkJobReports = cms.untracked.vstring('FrameworkJobReport')
# )


DTDataIntegrityTask = cms.Service("DTDataIntegrityTask",
    hltMode = cms.untracked.bool(True),
    getSCInfo = cms.untracked.bool(True)
)


DQMStore = cms.Service("DQMStore")


UpdaterService = cms.Service("UpdaterService")


DQM = cms.Service("DQM")


Chi2MeasurementEstimator = cms.ESProducer("Chi2MeasurementEstimatorESProducer",
    MaxChi2 = cms.double(30.0),
    appendToDataLabel = cms.string(''),
    nSigma = cms.double(3.0),
    ComponentName = cms.string('Chi2')
)


CaloGeometryBuilder = cms.ESProducer("CaloGeometryBuilder",
    SelectedCalos = cms.vstring('HCAL', 
        'ZDC', 
        'CASTOR', 
        'EcalBarrel', 
        'EcalEndcap', 
        'EcalPreshower', 
        'TOWER')
)


CastorHardcodeGeometryEP = cms.ESProducer("CastorHardcodeGeometryEP")


SmartPropagatorOpposite = cms.ESProducer("SmartPropagatorESProducer",
    appendToDataLabel = cms.string(''),
    PropagationDirection = cms.string('oppositeToMomentum'),
    Epsilon = cms.double(5.0),
    MuonPropagator = cms.string('SteppingHelixPropagatorOpposite'),
    ComponentName = cms.string('SmartPropagatorOpposite'),
    TrackerPropagator = cms.string('PropagatorWithMaterialOpposite')
)


KFSmootherForRefitInsideOut = cms.ESProducer("KFTrajectorySmootherESProducer",
    appendToDataLabel = cms.string(''),
    minHits = cms.int32(3),
    ComponentName = cms.string('KFSmootherForRefitInsideOut'),
    errorRescaling = cms.double(100.0),
    Estimator = cms.string('Chi2EstimatorForRefit'),
    Updator = cms.string('KFUpdator'),
    Propagator = cms.string('SmartPropagatorAnyOpposite'),
    RecoGeometry = cms.string('DummyDetLayerGeometry')
)


SteppingHelixPropagatorOpposite = cms.ESProducer("SteppingHelixPropagatorESProducer",
    appendToDataLabel = cms.string(''),
    endcapShiftInZNeg = cms.double(0.0),
    PropagationDirection = cms.string('oppositeToMomentum'),
    useMatVolumes = cms.bool(True),
    useTuningForL2Speed = cms.bool(False),
    useIsYokeFlag = cms.bool(True),
    NoErrorPropagation = cms.bool(False),
    SetVBFPointer = cms.bool(False),
    AssumeNoMaterial = cms.bool(False),
    endcapShiftInZPos = cms.double(0.0),
    useInTeslaFromMagField = cms.bool(False),
    VBFName = cms.string('VolumeBasedMagneticField'),
    useEndcapShiftsInZ = cms.bool(False),
    sendLogWarning = cms.bool(False),
    ComponentName = cms.string('SteppingHelixPropagatorOpposite'),
    debug = cms.bool(False),
    ApplyRadX0Correction = cms.bool(True),
    useMagVolumes = cms.bool(True),
    returnTangentPlane = cms.bool(True)
)


DTGeometryESModule = cms.ESProducer("DTGeometryESModule",
    appendToDataLabel = cms.string(''),
    fromDDD = cms.bool(True),
    applyAlignment = cms.bool(True),
    alignmentsLabel = cms.string('')
)


VolumeBasedMagneticFieldESProducer = cms.ESProducer("VolumeBasedMagneticFieldESProducer",
    scalingVolumes = cms.vint32(14100, 14200, 17600, 17800, 17900, 
        18100, 18300, 18400, 18600, 23100, 
        23300, 23400, 23600, 23800, 23900, 
        24100, 28600, 28800, 28900, 29100, 
        29300, 29400, 29600, 28609, 28809, 
        28909, 29109, 29309, 29409, 29609, 
        28610, 28810, 28910, 29110, 29310, 
        29410, 29610, 28611, 28811, 28911, 
        29111, 29311, 29411, 29611),
    scalingFactors = cms.vdouble(1, 1, 0.994, 1.004, 1.004, 
        1.005, 1.004, 1.004, 0.994, 0.965, 
        0.958, 0.958, 0.953, 0.958, 0.958, 
        0.965, 0.918, 0.924, 0.924, 0.906, 
        0.924, 0.924, 0.918, 0.991, 0.998, 
        0.998, 0.978, 0.998, 0.998, 0.991, 
        0.991, 0.998, 0.998, 0.978, 0.998, 
        0.998, 0.991, 0.991, 0.998, 0.998, 
        0.978, 0.998, 0.998, 0.991),
    overrideMasterSector = cms.bool(False),
    useParametrizedTrackerField = cms.bool(True),
    label = cms.untracked.string(''),
    version = cms.string('grid_1103l_090322_3_8t'),
    debugBuilder = cms.untracked.bool(False),
    paramLabel = cms.string('parametrizedField'),
    cacheLastVolume = cms.untracked.bool(True)
)


oniaTrajectoryFilter = cms.ESProducer("TrajectoryFilterESProducer",
    filterPset = cms.PSet(
        minimumNumberOfHits = cms.int32(5),
        minHitsMinPt = cms.int32(3),
        ComponentType = cms.string('CkfBaseTrajectoryFilter'),
        maxLostHits = cms.int32(1),
        maxNumberOfHits = cms.int32(8),
        maxConsecLostHits = cms.int32(1),
        chargeSignificance = cms.double(-1.0),
        nSigmaMinPt = cms.double(5.0),
        minPt = cms.double(1.0)
    ),
    ComponentName = cms.string('oniaTrajectoryFilter'),
    appendToDataLabel = cms.string('')
)


HITTRHBuilderWithoutRefit = cms.ESProducer("TkTransientTrackingRecHitBuilderESProducer",
    StripCPE = cms.string('Fake'),
    appendToDataLabel = cms.string(''),
    Matcher = cms.string('Fake'),
    ComputeCoarseLocalPositionFromDisk = cms.bool(False),
    ComponentName = cms.string('HITTRHBuilderWithoutRefit'),
    PixelCPE = cms.string('Fake')
)


hltKFFitter = cms.ESProducer("KFTrajectoryFitterESProducer",
    appendToDataLabel = cms.string(''),
    minHits = cms.int32(3),
    ComponentName = cms.string('hltKFFitter'),
    Estimator = cms.string('Chi2'),
    Updator = cms.string('KFUpdator'),
    Propagator = cms.string('PropagatorWithMaterial'),
    RecoGeometry = cms.string('DummyDetLayerGeometry')
)


KFFitterSmootherForL2Muon = cms.ESProducer("KFFittingSmootherESProducer",
    EstimateCut = cms.double(-1.0),
    appendToDataLabel = cms.string(''),
    Fitter = cms.string('KFTrajectoryFitterForL2Muon'),
    MinNumberOfHits = cms.int32(5),
    Smoother = cms.string('KFTrajectorySmootherForL2Muon'),
    BreakTrajWith2ConsecutiveMissing = cms.bool(False),
    NoInvalidHitsBeginEnd = cms.bool(False),
    ComponentName = cms.string('KFFitterSmootherForL2Muon'),
    RejectTracks = cms.bool(True)
)


bJetRegionalTrajectoryBuilder = cms.ESProducer("CkfTrajectoryBuilderESProducer",
    propagatorAlong = cms.string('PropagatorWithMaterial'),
    appendToDataLabel = cms.string(''),
    trajectoryFilterName = cms.string('bJetRegionalTrajectoryFilter'),
    maxCand = cms.int32(1),
    ComponentName = cms.string('bJetRegionalTrajectoryBuilder'),
    intermediateCleaning = cms.bool(True),
    MeasurementTrackerName = cms.string(''),
    estimator = cms.string('Chi2'),
    TTRHBuilder = cms.string('WithTrackAngle'),
    updator = cms.string('KFUpdator'),
    alwaysUseInvalidHits = cms.bool(False),
    propagatorOpposite = cms.string('PropagatorWithMaterialOpposite'),
    lostHitPenalty = cms.double(30.0)
)


CaloTopologyBuilder = cms.ESProducer("CaloTopologyBuilder",
    appendToDataLabel = cms.string('')
)


SmartPropagator = cms.ESProducer("SmartPropagatorESProducer",
    appendToDataLabel = cms.string(''),
    PropagationDirection = cms.string('alongMomentum'),
    Epsilon = cms.double(5.0),
    MuonPropagator = cms.string('SteppingHelixPropagatorAlong'),
    ComponentName = cms.string('SmartPropagator'),
    TrackerPropagator = cms.string('PropagatorWithMaterial')
)


pixellayerpairs = cms.ESProducer("SeedingLayersESProducer",
    appendToDataLabel = cms.string(''),
    ComponentName = cms.string('PixelLayerPairs'),
    TEC = cms.PSet(

    ),
    layerList = cms.vstring('BPix1+BPix2', 
        'BPix1+BPix3', 
        'BPix2+BPix3', 
        'BPix1+FPix1_pos', 
        'BPix1+FPix1_neg', 
        'BPix1+FPix2_pos', 
        'BPix1+FPix2_neg', 
        'BPix2+FPix1_pos', 
        'BPix2+FPix1_neg', 
        'BPix2+FPix2_pos', 
        'BPix2+FPix2_neg', 
        'FPix1_pos+FPix2_pos', 
        'FPix1_neg+FPix2_neg'),
    FPix = cms.PSet(
        hitErrorRZ = cms.double(0.0036),
        hitErrorRPhi = cms.double(0.0051),
        TTRHBuilder = cms.string('TTRHBuilderPixelOnly'),
        HitProducer = cms.string('hltSiPixelRecHits'),
        useErrorsFromParam = cms.bool(True)
    ),
    BPix = cms.PSet(
        hitErrorRZ = cms.double(0.006),
        hitErrorRPhi = cms.double(0.0027),
        TTRHBuilder = cms.string('TTRHBuilderPixelOnly'),
        HitProducer = cms.string('hltSiPixelRecHits'),
        useErrorsFromParam = cms.bool(True)
    )
)


hcalRecAlgos = cms.ESProducer("HcalRecAlgoESProducer",
    RecoveredRecHitBits = cms.vstring(),
    appendToDataLabel = cms.string(''),
    SeverityLevels = cms.VPSet(cms.PSet(
        RecHitFlags = cms.vstring(),
        ChannelStatus = cms.vstring(),
        Level = cms.int32(0)
    )),
    DropChannelStatusBits = cms.vstring()
)


RPCConeBuilder = cms.ESProducer("RPCConeBuilder",
    towerEnd = cms.int32(16),
    towerBeg = cms.int32(0)
)


CkfTrajectoryBuilder = cms.ESProducer("CkfTrajectoryBuilderESProducer",
    propagatorAlong = cms.string('PropagatorWithMaterial'),
    appendToDataLabel = cms.string(''),
    trajectoryFilterName = cms.string('ckfBaseTrajectoryFilter'),
    maxCand = cms.int32(5),
    ComponentName = cms.string('CkfTrajectoryBuilder'),
    intermediateCleaning = cms.bool(True),
    MeasurementTrackerName = cms.string(''),
    estimator = cms.string('Chi2'),
    TTRHBuilder = cms.string('WithTrackAngle'),
    updator = cms.string('KFUpdator'),
    alwaysUseInvalidHits = cms.bool(True),
    propagatorOpposite = cms.string('PropagatorWithMaterialOpposite'),
    lostHitPenalty = cms.double(30.0)
)


CSCGeometryESModule = cms.ESProducer("CSCGeometryESModule",
    appendToDataLabel = cms.string(''),
    useDDD = cms.bool(True),
    debugV = cms.untracked.bool(False),
    useGangedStripsInME1a = cms.bool(True),
    alignmentsLabel = cms.string(''),
    useOnlyWiresInME1a = cms.bool(False),
    useRealWireGeometry = cms.bool(True),
    useCentreTIOffsets = cms.bool(False),
    applyAlignment = cms.bool(True)
)


MuonTransientTrackingRecHitBuilderESProducer = cms.ESProducer("MuonTransientTrackingRecHitBuilderESProducer",
    ComponentName = cms.string('MuonRecHitBuilder'),
    appendToDataLabel = cms.string('')
)


CaloTowerHardcodeGeometryEP = cms.ESProducer("CaloTowerHardcodeGeometryEP")


sistripconn = cms.ESProducer("SiStripConnectivity")


EcalEndcapGeometryEP = cms.ESProducer("EcalEndcapGeometryEP",
    applyAlignment = cms.bool(False)
)


navigationSchoolESProducer = cms.ESProducer("NavigationSchoolESProducer",
    ComponentName = cms.string('SimpleNavigationSchool'),
    appendToDataLabel = cms.string('')
)


pixellayertriplets = cms.ESProducer("SeedingLayersESProducer",
    appendToDataLabel = cms.string(''),
    ComponentName = cms.string('PixelLayerTriplets'),
    TEC = cms.PSet(

    ),
    layerList = cms.vstring('BPix1+BPix2+BPix3', 
        'BPix1+BPix2+FPix1_pos', 
        'BPix1+BPix2+FPix1_neg', 
        'BPix1+FPix1_pos+FPix2_pos', 
        'BPix1+FPix1_neg+FPix2_neg'),
    FPix = cms.PSet(
        hitErrorRZ = cms.double(0.0036),
        hitErrorRPhi = cms.double(0.0051),
        TTRHBuilder = cms.string('TTRHBuilderPixelOnly'),
        HitProducer = cms.string('hltSiPixelRecHits'),
        useErrorsFromParam = cms.bool(True)
    ),
    BPix = cms.PSet(
        hitErrorRZ = cms.double(0.006),
        hitErrorRPhi = cms.double(0.0027),
        TTRHBuilder = cms.string('TTRHBuilderPixelOnly'),
        HitProducer = cms.string('hltSiPixelRecHits'),
        useErrorsFromParam = cms.bool(True)
    )
)


EcalUnpackerWorkerESProducer = cms.ESProducer("EcalUnpackerWorkerESProducer",
    CalibRHAlgo = cms.PSet(
        flagsMapDBReco = cms.vint32(0, 0, 0, 0, 4, 
            -1, -1, -1, 4, 4, 
            6, 6, 6, 7, 8),
        Type = cms.string('EcalRecHitWorkerSimple'),
        killDeadChannels = cms.bool(True),
        ChannelStatusToBeExcluded = cms.vint32(10, 11, 12, 13, 14, 
            78, 142),
        laserCorrection = cms.bool(False)
    ),
    appendToDataLabel = cms.string(''),
    ComponentName = cms.string(''),
    UncalibRHAlgo = cms.PSet(
        Type = cms.string('EcalUncalibRecHitWorkerWeights')
    ),
    DCCDataUnpacker = cms.PSet(
        tccUnpacking = cms.bool(True),
        orderedDCCIdList = cms.vint32(1, 2, 3, 4, 5, 
            6, 7, 8, 9, 10, 
            11, 12, 13, 14, 15, 
            16, 17, 18, 19, 20, 
            21, 22, 23, 24, 25, 
            26, 27, 28, 29, 30, 
            31, 32, 33, 34, 35, 
            36, 37, 38, 39, 40, 
            41, 42, 43, 44, 45, 
            46, 47, 48, 49, 50, 
            51, 52, 53, 54),
        srpUnpacking = cms.bool(False),
        syncCheck = cms.bool(False),
        feIdCheck = cms.bool(True),
        headerUnpacking = cms.bool(False),
        orderedFedList = cms.vint32(601, 602, 603, 604, 605, 
            606, 607, 608, 609, 610, 
            611, 612, 613, 614, 615, 
            616, 617, 618, 619, 620, 
            621, 622, 623, 624, 625, 
            626, 627, 628, 629, 630, 
            631, 632, 633, 634, 635, 
            636, 637, 638, 639, 640, 
            641, 642, 643, 644, 645, 
            646, 647, 648, 649, 650, 
            651, 652, 653, 654),
        feUnpacking = cms.bool(True),
        forceKeepFRData = cms.bool(False),
        memUnpacking = cms.bool(False)
    ),
    ElectronicsMapper = cms.PSet(
        numbXtalTSamples = cms.uint32(10),
        numbTriggerTSamples = cms.uint32(1)
    )
)


SteppingHelixPropagatorAlong = cms.ESProducer("SteppingHelixPropagatorESProducer",
    appendToDataLabel = cms.string(''),
    endcapShiftInZNeg = cms.double(0.0),
    PropagationDirection = cms.string('alongMomentum'),
    useMatVolumes = cms.bool(True),
    useTuningForL2Speed = cms.bool(False),
    useIsYokeFlag = cms.bool(True),
    NoErrorPropagation = cms.bool(False),
    SetVBFPointer = cms.bool(False),
    AssumeNoMaterial = cms.bool(False),
    endcapShiftInZPos = cms.double(0.0),
    useInTeslaFromMagField = cms.bool(False),
    VBFName = cms.string('VolumeBasedMagneticField'),
    useEndcapShiftsInZ = cms.bool(False),
    sendLogWarning = cms.bool(False),
    ComponentName = cms.string('SteppingHelixPropagatorAlong'),
    debug = cms.bool(False),
    ApplyRadX0Correction = cms.bool(True),
    useMagVolumes = cms.bool(True),
    returnTangentPlane = cms.bool(True)
)


KFTrajectorySmootherForL2Muon = cms.ESProducer("KFTrajectorySmootherESProducer",
    appendToDataLabel = cms.string(''),
    minHits = cms.int32(3),
    ComponentName = cms.string('KFTrajectorySmootherForL2Muon'),
    errorRescaling = cms.double(100.0),
    Estimator = cms.string('Chi2'),
    Updator = cms.string('KFUpdator'),
    Propagator = cms.string('FastSteppingHelixPropagatorOpposite'),
    RecoGeometry = cms.string('DummyDetLayerGeometry')
)


KFTrajectoryFitterForL2Muon = cms.ESProducer("KFTrajectoryFitterESProducer",
    appendToDataLabel = cms.string(''),
    minHits = cms.int32(3),
    ComponentName = cms.string('KFTrajectoryFitterForL2Muon'),
    Estimator = cms.string('Chi2'),
    Updator = cms.string('KFUpdator'),
    Propagator = cms.string('FastSteppingHelixPropagatorAny'),
    RecoGeometry = cms.string('DummyDetLayerGeometry')
)


siStripGainESProducer = cms.ESProducer("SiStripGainESProducer",
    printDebug = cms.untracked.bool(False),
    appendToDataLabel = cms.string(''),
    APVGain = cms.string(''),
    AutomaticNormalization = cms.bool(False),
    NormalizationFactor = cms.double(1.0)
)


HcalHardcodeGeometryEP = cms.ESProducer("HcalHardcodeGeometryEP")


oniaTrajectoryBuilder = cms.ESProducer("CkfTrajectoryBuilderESProducer",
    propagatorAlong = cms.string('PropagatorWithMaterial'),
    appendToDataLabel = cms.string(''),
    trajectoryFilterName = cms.string('oniaTrajectoryFilter'),
    maxCand = cms.int32(1),
    ComponentName = cms.string('oniaTrajectoryBuilder'),
    intermediateCleaning = cms.bool(True),
    MeasurementTrackerName = cms.string(''),
    estimator = cms.string('Chi2'),
    TTRHBuilder = cms.string('WithTrackAngle'),
    updator = cms.string('KFUpdator'),
    alwaysUseInvalidHits = cms.bool(False),
    propagatorOpposite = cms.string('PropagatorWithMaterialOpposite'),
    lostHitPenalty = cms.double(30.0)
)


RPCGeometryESModule = cms.ESProducer("RPCGeometryESModule",
    useDDD = cms.untracked.bool(True),
    compatibiltyWith11 = cms.untracked.bool(True)
)


idealForDigiDTGeometry = cms.ESProducer("DTGeometryESModule",
    appendToDataLabel = cms.string('idealForDigi'),
    fromDDD = cms.bool(True),
    applyAlignment = cms.bool(False),
    alignmentsLabel = cms.string('fakeForIdeal')
)


SmootherRK = cms.ESProducer("KFTrajectorySmootherESProducer",
    appendToDataLabel = cms.string(''),
    minHits = cms.int32(3),
    ComponentName = cms.string('SmootherRK'),
    errorRescaling = cms.double(100.0),
    Estimator = cms.string('Chi2'),
    Updator = cms.string('KFUpdator'),
    Propagator = cms.string('RungeKuttaTrackerPropagator'),
    RecoGeometry = cms.string('DummyDetLayerGeometry')
)


EcalElectronicsMappingBuilder = cms.ESProducer("EcalElectronicsMappingBuilder")


hltKFSmoother = cms.ESProducer("KFTrajectorySmootherESProducer",
    appendToDataLabel = cms.string(''),
    minHits = cms.int32(3),
    ComponentName = cms.string('hltKFSmoother'),
    errorRescaling = cms.double(100.0),
    Estimator = cms.string('Chi2'),
    Updator = cms.string('KFUpdator'),
    Propagator = cms.string('PropagatorWithMaterial'),
    RecoGeometry = cms.string('DummyDetLayerGeometry')
)


CaloTowerConstituentsMapBuilder = cms.ESProducer("CaloTowerConstituentsMapBuilder",
    appendToDataLabel = cms.string(''),
    MapFile = cms.untracked.string('Geometry/CaloTopology/data/CaloTowerEEGeometric.map.gz')
)


pixellayertripletsHITHB = cms.ESProducer("SeedingLayersESProducer",
    appendToDataLabel = cms.string(''),
    ComponentName = cms.string('PixelLayerTripletsHITHB'),
    TEC = cms.PSet(

    ),
    layerList = cms.vstring('BPix1+BPix2+BPix3'),
    FPix = cms.PSet(
        hitErrorRZ = cms.double(0.0036),
        hitErrorRPhi = cms.double(0.0051),
        TTRHBuilder = cms.string('TTRHBuilderPixelOnly'),
        HitProducer = cms.string('hltSiPixelRecHits'),
        useErrorsFromParam = cms.bool(True)
    ),
    BPix = cms.PSet(
        hitErrorRZ = cms.double(0.006),
        hitErrorRPhi = cms.double(0.0027),
        TTRHBuilder = cms.string('TTRHBuilderPixelOnly'),
        HitProducer = cms.string('hltSiPixelRecHits'),
        useErrorsFromParam = cms.bool(True)
    )
)


SiStripRecHitMatcherESProducer = cms.ESProducer("SiStripRecHitMatcherESProducer",
    ComponentName = cms.string('StandardMatcher'),
    NSigmaInside = cms.double(3.0)
)


StripCPEfromTrackAngleESProducer = cms.ESProducer("StripCPEESProducer",
    ComponentName = cms.string('StripCPEfromTrackAngle'),
    OutOfTime = cms.PSet(
        TOBlateFP = cms.double(0),
        TIDlateFP = cms.double(0),
        TOBlateBP = cms.double(0),
        TEClateBP = cms.double(0),
        TIBlateFP = cms.double(0),
        TIBlateBP = cms.double(0),
        TIDlateBP = cms.double(0),
        TEClateFP = cms.double(0)
    )
)


TrackerDigiGeometryESModule = cms.ESProducer("TrackerDigiGeometryESModule",
    appendToDataLabel = cms.string(''),
    fromDDD = cms.bool(True),
    applyAlignment = cms.bool(True),
    alignmentsLabel = cms.string('')
)


EcalBarrelGeometryEP = cms.ESProducer("EcalBarrelGeometryEP",
    applyAlignment = cms.bool(False)
)


EcalRegionCablingESProducer = cms.ESProducer("EcalRegionCablingESProducer",
    appendToDataLabel = cms.string(''),
    esMapping = cms.PSet(
        LookupTable = cms.FileInPath('EventFilter/ESDigiToRaw/data/ES_lookup_table.dat')
    )
)


TransientTrackBuilderESProducer = cms.ESProducer("TransientTrackBuilderESProducer",
    ComponentName = cms.string('TransientTrackBuilder'),
    appendToDataLabel = cms.string('')
)


pixellayertripletsHITHE = cms.ESProducer("SeedingLayersESProducer",
    appendToDataLabel = cms.string(''),
    ComponentName = cms.string('PixelLayerTripletsHITHE'),
    TEC = cms.PSet(

    ),
    layerList = cms.vstring('BPix1+BPix2+FPix1_pos', 
        'BPix1+BPix2+FPix1_neg', 
        'BPix1+FPix1_pos+FPix2_pos', 
        'BPix1+FPix1_neg+FPix2_neg'),
    FPix = cms.PSet(
        hitErrorRZ = cms.double(0.0036),
        hitErrorRPhi = cms.double(0.0051),
        TTRHBuilder = cms.string('TTRHBuilderPixelOnly'),
        HitProducer = cms.string('hltSiPixelRecHits'),
        useErrorsFromParam = cms.bool(True)
    ),
    BPix = cms.PSet(
        hitErrorRZ = cms.double(0.006),
        hitErrorRPhi = cms.double(0.0027),
        TTRHBuilder = cms.string('TTRHBuilderPixelOnly'),
        HitProducer = cms.string('hltSiPixelRecHits'),
        useErrorsFromParam = cms.bool(True)
    )
)


MuonDetLayerGeometryESProducer = cms.ESProducer("MuonDetLayerGeometryESProducer",
    appendToDataLabel = cms.string('')
)


OppositeMaterialPropagator = cms.ESProducer("PropagatorWithMaterialESProducer",
    appendToDataLabel = cms.string(''),
    PropagationDirection = cms.string('oppositeToMomentum'),
    ComponentName = cms.string('PropagatorWithMaterialOpposite'),
    Mass = cms.double(0.105),
    ptMin = cms.double(-1.0),
    MaxDPhi = cms.double(1.6),
    useRungeKutta = cms.bool(False)
)


hltCkfTrajectoryBuilderMumu = cms.ESProducer("CkfTrajectoryBuilderESProducer",
    propagatorAlong = cms.string('PropagatorWithMaterial'),
    appendToDataLabel = cms.string(''),
    trajectoryFilterName = cms.string('hltCkfTrajectoryFilterMumu'),
    maxCand = cms.int32(3),
    ComponentName = cms.string('hltCkfTrajectoryBuilderMumu'),
    intermediateCleaning = cms.bool(True),
    MeasurementTrackerName = cms.string(''),
    estimator = cms.string('Chi2'),
    TTRHBuilder = cms.string('WithTrackAngle'),
    updator = cms.string('KFUpdator'),
    alwaysUseInvalidHits = cms.bool(False),
    propagatorOpposite = cms.string('PropagatorWithMaterialOpposite'),
    lostHitPenalty = cms.double(30.0)
)


SteppingHelixPropagatorAny = cms.ESProducer("SteppingHelixPropagatorESProducer",
    appendToDataLabel = cms.string(''),
    endcapShiftInZNeg = cms.double(0.0),
    PropagationDirection = cms.string('anyDirection'),
    useMatVolumes = cms.bool(True),
    useTuningForL2Speed = cms.bool(False),
    useIsYokeFlag = cms.bool(True),
    NoErrorPropagation = cms.bool(False),
    SetVBFPointer = cms.bool(False),
    AssumeNoMaterial = cms.bool(False),
    endcapShiftInZPos = cms.double(0.0),
    useInTeslaFromMagField = cms.bool(False),
    VBFName = cms.string('VolumeBasedMagneticField'),
    useEndcapShiftsInZ = cms.bool(False),
    sendLogWarning = cms.bool(False),
    ComponentName = cms.string('SteppingHelixPropagatorAny'),
    debug = cms.bool(False),
    ApplyRadX0Correction = cms.bool(True),
    useMagVolumes = cms.bool(True),
    returnTangentPlane = cms.bool(True)
)


muonCkfTrajectoryFilter = cms.ESProducer("TrajectoryFilterESProducer",
    filterPset = cms.PSet(
        minPt = cms.double(0.9),
        minHitsMinPt = cms.int32(3),
        ComponentType = cms.string('CkfBaseTrajectoryFilter'),
        maxLostHits = cms.int32(1),
        maxNumberOfHits = cms.int32(-1),
        maxConsecLostHits = cms.int32(1),
        chargeSignificance = cms.double(-1.0),
        nSigmaMinPt = cms.double(5.0),
        minimumNumberOfHits = cms.int32(5)
    ),
    ComponentName = cms.string('muonCkfTrajectoryFilter'),
    appendToDataLabel = cms.string('')
)


SmartPropagatorAny = cms.ESProducer("SmartPropagatorESProducer",
    appendToDataLabel = cms.string(''),
    PropagationDirection = cms.string('alongMomentum'),
    Epsilon = cms.double(5.0),
    MuonPropagator = cms.string('SteppingHelixPropagatorAny'),
    ComponentName = cms.string('SmartPropagatorAny'),
    TrackerPropagator = cms.string('PropagatorWithMaterial')
)


MuonCkfTrajectoryBuilder = cms.ESProducer("MuonCkfTrajectoryBuilderESProducer",
    propagatorAlong = cms.string('PropagatorWithMaterial'),
    appendToDataLabel = cms.string(''),
    trajectoryFilterName = cms.string('muonCkfTrajectoryFilter'),
    maxCand = cms.int32(5),
    ComponentName = cms.string('muonCkfTrajectoryBuilder'),
    intermediateCleaning = cms.bool(False),
    useSeedLayer = cms.bool(False),
    deltaEta = cms.double(0.1),
    deltaPhi = cms.double(0.1),
    estimator = cms.string('Chi2'),
    rescaleErrorIfFail = cms.double(1.0),
    propagatorProximity = cms.string('SteppingHelixPropagatorAny'),
    updator = cms.string('KFUpdator'),
    alwaysUseInvalidHits = cms.bool(True),
    TTRHBuilder = cms.string('WithTrackAngle'),
    MeasurementTrackerName = cms.string(''),
    propagatorOpposite = cms.string('PropagatorWithMaterialOpposite'),
    lostHitPenalty = cms.double(30.0)
)


trackCounting3D2nd = cms.ESProducer("TrackCountingESProducer",
    appendToDataLabel = cms.string(''),
    maximumDistanceToJetAxis = cms.double(0.07),
    impactParameterType = cms.int32(0),
    deltaR = cms.double(-1.0),
    trackQualityClass = cms.string('any'),
    maximumDecayLength = cms.double(5.0),
    nthTrack = cms.int32(2)
)


Chi2EstimatorForRefit = cms.ESProducer("Chi2MeasurementEstimatorESProducer",
    MaxChi2 = cms.double(100000.0),
    appendToDataLabel = cms.string(''),
    nSigma = cms.double(3.0),
    ComponentName = cms.string('Chi2EstimatorForRefit')
)


TrackerRecoGeometryESProducer = cms.ESProducer("TrackerRecoGeometryESProducer",
    appendToDataLabel = cms.string('')
)


hltCkfTrajectoryFilterMumu = cms.ESProducer("TrajectoryFilterESProducer",
    filterPset = cms.PSet(
        minPt = cms.double(3.0),
        minHitsMinPt = cms.int32(3),
        ComponentType = cms.string('CkfBaseTrajectoryFilter'),
        maxLostHits = cms.int32(1),
        maxNumberOfHits = cms.int32(5),
        maxConsecLostHits = cms.int32(1),
        minimumNumberOfHits = cms.int32(5),
        nSigmaMinPt = cms.double(5.0),
        chargeSignificance = cms.double(-1.0)
    ),
    ComponentName = cms.string('hltCkfTrajectoryFilterMumu'),
    appendToDataLabel = cms.string('')
)


KFSmootherForMuonTrackLoader = cms.ESProducer("KFTrajectorySmootherESProducer",
    appendToDataLabel = cms.string(''),
    minHits = cms.int32(3),
    ComponentName = cms.string('KFSmootherForMuonTrackLoader'),
    errorRescaling = cms.double(10.0),
    Estimator = cms.string('Chi2'),
    Updator = cms.string('KFUpdator'),
    Propagator = cms.string('SmartPropagatorAnyOpposite'),
    RecoGeometry = cms.string('DummyDetLayerGeometry')
)


trajFilterL3 = cms.ESProducer("TrajectoryFilterESProducer",
    filterPset = cms.PSet(
        minPt = cms.double(0.9),
        minHitsMinPt = cms.int32(3),
        ComponentType = cms.string('CkfBaseTrajectoryFilter'),
        maxLostHits = cms.int32(1),
        maxNumberOfHits = cms.int32(7),
        maxConsecLostHits = cms.int32(1),
        minimumNumberOfHits = cms.int32(5),
        nSigmaMinPt = cms.double(5.0),
        chargeSignificance = cms.double(-1.0)
    ),
    ComponentName = cms.string('trajFilterL3'),
    appendToDataLabel = cms.string('')
)


softLeptonByPt = cms.ESProducer("LeptonTaggerByPtESProducer",
    appendToDataLabel = cms.string(''),
    ipSign = cms.string('any')
)


SmartPropagatorAnyOpposite = cms.ESProducer("SmartPropagatorESProducer",
    appendToDataLabel = cms.string(''),
    PropagationDirection = cms.string('oppositeToMomentum'),
    Epsilon = cms.double(5.0),
    MuonPropagator = cms.string('SteppingHelixPropagatorAny'),
    ComponentName = cms.string('SmartPropagatorAnyOpposite'),
    TrackerPropagator = cms.string('PropagatorWithMaterialOpposite')
)


GlobalTrackingGeometryESProducer = cms.ESProducer("GlobalTrackingGeometryESProducer",
    appendToDataLabel = cms.string('')
)


KFUpdatorESProducer = cms.ESProducer("KFUpdatorESProducer",
    ComponentName = cms.string('KFUpdator'),
    appendToDataLabel = cms.string('')
)


bJetRegionalTrajectoryFilter = cms.ESProducer("TrajectoryFilterESProducer",
    filterPset = cms.PSet(
        minPt = cms.double(1.0),
        minHitsMinPt = cms.int32(3),
        ComponentType = cms.string('CkfBaseTrajectoryFilter'),
        maxLostHits = cms.int32(1),
        maxNumberOfHits = cms.int32(8),
        maxConsecLostHits = cms.int32(1),
        minimumNumberOfHits = cms.int32(5),
        nSigmaMinPt = cms.double(5.0),
        chargeSignificance = cms.double(-1.0)
    ),
    ComponentName = cms.string('bJetRegionalTrajectoryFilter'),
    appendToDataLabel = cms.string('')
)


EcalLaserCorrectionService = cms.ESProducer("EcalLaserCorrectionService")


hcal_db_producer = cms.ESProducer("HcalDbProducer",
    file = cms.untracked.string(''),
    dump = cms.untracked.vstring('')
)


MeasurementTracker = cms.ESProducer("MeasurementTrackerESProducer",
    StripCPE = cms.string('StripCPEfromTrackAngle'),
    inactivePixelDetectorLabels = cms.VInputTag(),
    appendToDataLabel = cms.string(''),
    PixelCPE = cms.string('PixelCPEGeneric'),
    stripLazyGetterProducer = cms.string('hltSiStripRawToClustersFacility'),
    OnDemand = cms.bool(True),
    Regional = cms.bool(True),
    UsePixelModuleQualityDB = cms.bool(True),
    pixelClusterProducer = cms.string('hltSiPixelClusters'),
    switchOffPixelsIfEmpty = cms.bool(True),
    inactiveStripDetectorLabels = cms.VInputTag(),
    MaskBadAPVFibers = cms.bool(True),
    UseStripStripQualityDB = cms.bool(True),
    UsePixelROCQualityDB = cms.bool(True),
    DebugPixelROCQualityDB = cms.untracked.bool(False),
    UseStripAPVFiberQualityDB = cms.bool(True),
    stripClusterProducer = cms.string('hltSiStripClusters'),
    DebugStripAPVFiberQualityDB = cms.untracked.bool(False),
    DebugStripStripQualityDB = cms.untracked.bool(False),
    SiStripQualityLabel = cms.string(''),
    badStripCuts = cms.PSet(
        TID = cms.PSet(
            maxConsecutiveBad = cms.uint32(9999),
            maxBad = cms.uint32(9999)
        ),
        TOB = cms.PSet(
            maxConsecutiveBad = cms.uint32(9999),
            maxBad = cms.uint32(9999)
        ),
        TEC = cms.PSet(
            maxConsecutiveBad = cms.uint32(9999),
            maxBad = cms.uint32(9999)
        ),
        TIB = cms.PSet(
            maxConsecutiveBad = cms.uint32(9999),
            maxBad = cms.uint32(9999)
        )
    ),
    DebugStripModuleQualityDB = cms.untracked.bool(False),
    ComponentName = cms.string(''),
    DebugPixelModuleQualityDB = cms.untracked.bool(False),
    HitMatcher = cms.string('StandardMatcher'),
    UseStripModuleQualityDB = cms.bool(True)
)


WithTrackAngle = cms.ESProducer("TkTransientTrackingRecHitBuilderESProducer",
    StripCPE = cms.string('StripCPEfromTrackAngle'),
    appendToDataLabel = cms.string(''),
    Matcher = cms.string('StandardMatcher'),
    ComputeCoarseLocalPositionFromDisk = cms.bool(False),
    ComponentName = cms.string('WithTrackAngle'),
    PixelCPE = cms.string('PixelCPEGeneric')
)


FastSteppingHelixPropagatorAny = cms.ESProducer("SteppingHelixPropagatorESProducer",
    appendToDataLabel = cms.string(''),
    endcapShiftInZNeg = cms.double(0.0),
    PropagationDirection = cms.string('anyDirection'),
    useMatVolumes = cms.bool(True),
    useTuningForL2Speed = cms.bool(True),
    useIsYokeFlag = cms.bool(True),
    NoErrorPropagation = cms.bool(False),
    SetVBFPointer = cms.bool(False),
    AssumeNoMaterial = cms.bool(False),
    endcapShiftInZPos = cms.double(0.0),
    useInTeslaFromMagField = cms.bool(False),
    VBFName = cms.string('VolumeBasedMagneticField'),
    useEndcapShiftsInZ = cms.bool(False),
    sendLogWarning = cms.bool(False),
    ComponentName = cms.string('FastSteppingHelixPropagatorAny'),
    debug = cms.bool(False),
    ApplyRadX0Correction = cms.bool(True),
    useMagVolumes = cms.bool(True),
    returnTangentPlane = cms.bool(True)
)


trajectoryCleanerBySharedHits = cms.ESProducer("TrajectoryCleanerESProducer",
    ComponentName = cms.string('TrajectoryCleanerBySharedHits'),
    appendToDataLabel = cms.string(''),
    fractionShared = cms.double(0.5)
)


ZdcHardcodeGeometryEP = cms.ESProducer("ZdcHardcodeGeometryEP")


ckfBaseTrajectoryFilter = cms.ESProducer("TrajectoryFilterESProducer",
    filterPset = cms.PSet(
        minPt = cms.double(0.9),
        minHitsMinPt = cms.int32(3),
        ComponentType = cms.string('CkfBaseTrajectoryFilter'),
        maxLostHits = cms.int32(1),
        maxNumberOfHits = cms.int32(-1),
        maxConsecLostHits = cms.int32(1),
        minimumNumberOfHits = cms.int32(5),
        nSigmaMinPt = cms.double(5.0),
        chargeSignificance = cms.double(-1.0)
    ),
    ComponentName = cms.string('ckfBaseTrajectoryFilter'),
    appendToDataLabel = cms.string('')
)


SiStripRegionConnectivity = cms.ESProducer("SiStripRegionConnectivity",
    EtaMax = cms.untracked.double(2.5),
    PhiDivisions = cms.untracked.uint32(20),
    EtaDivisions = cms.untracked.uint32(20)
)


EcalTrigTowerConstituentsMapBuilder = cms.ESProducer("EcalTrigTowerConstituentsMapBuilder",
    appendToDataLabel = cms.string(''),
    MapFile = cms.untracked.string('Geometry/EcalMapping/data/EndCap_TTMap.txt')
)


idealForDigiCSCGeometry = cms.ESProducer("CSCGeometryESModule",
    appendToDataLabel = cms.string('idealForDigi'),
    useDDD = cms.bool(True),
    debugV = cms.untracked.bool(False),
    useGangedStripsInME1a = cms.bool(True),
    alignmentsLabel = cms.string('fakeForIdeal'),
    useOnlyWiresInME1a = cms.bool(False),
    useRealWireGeometry = cms.bool(True),
    useCentreTIOffsets = cms.bool(False),
    applyAlignment = cms.bool(False)
)


MaterialPropagator = cms.ESProducer("PropagatorWithMaterialESProducer",
    appendToDataLabel = cms.string(''),
    PropagationDirection = cms.string('alongMomentum'),
    ComponentName = cms.string('PropagatorWithMaterial'),
    Mass = cms.double(0.105),
    ptMin = cms.double(-1.0),
    MaxDPhi = cms.double(1.6),
    useRungeKutta = cms.bool(False)
)


DummyDetLayerGeometry = cms.ESProducer("DetLayerGeometryESProducer",
    ComponentName = cms.string('DummyDetLayerGeometry'),
    appendToDataLabel = cms.string('')
)


RungeKuttaTrackerPropagator = cms.ESProducer("PropagatorWithMaterialESProducer",
    appendToDataLabel = cms.string(''),
    PropagationDirection = cms.string('alongMomentum'),
    ComponentName = cms.string('RungeKuttaTrackerPropagator'),
    Mass = cms.double(0.105),
    ptMin = cms.double(-1.0),
    MaxDPhi = cms.double(1.6),
    useRungeKutta = cms.bool(True)
)


ESUnpackerWorkerESProducer = cms.ESProducer("ESUnpackerWorkerESProducer",
    RHAlgo = cms.PSet(
        ESMIPkeV = cms.double(81.08),
        ESGain = cms.int32(1),
        Type = cms.string('ESRecHitWorker'),
        ESMIPADC = cms.double(9.0),
        ESBaseline = cms.int32(1000)
    ),
    appendToDataLabel = cms.string(''),
    DCCDataUnpacker = cms.PSet(
        LookupTable = cms.FileInPath('EventFilter/ESDigiToRaw/data/ES_lookup_table.dat')
    ),
    ComponentName = cms.string('esRawToRecHit')
)


SiPixelTemplateDBObjectESProducer = cms.ESProducer("SiPixelTemplateDBObjectESProducer",
    appendToDataLabel = cms.string('')
)


TTRHBuilderPixelOnly = cms.ESProducer("TkTransientTrackingRecHitBuilderESProducer",
    StripCPE = cms.string('Fake'),
    appendToDataLabel = cms.string(''),
    Matcher = cms.string('StandardMatcher'),
    ComputeCoarseLocalPositionFromDisk = cms.bool(False),
    ComponentName = cms.string('TTRHBuilderPixelOnly'),
    PixelCPE = cms.string('PixelCPEGeneric')
)


softLeptonByDistance = cms.ESProducer("LeptonTaggerByDistanceESProducer",
    distance = cms.double(0.5),
    appendToDataLabel = cms.string('')
)


KFFitterForRefitInsideOut = cms.ESProducer("KFTrajectoryFitterESProducer",
    appendToDataLabel = cms.string(''),
    minHits = cms.int32(3),
    ComponentName = cms.string('KFFitterForRefitInsideOut'),
    Estimator = cms.string('Chi2EstimatorForRefit'),
    Updator = cms.string('KFUpdator'),
    Propagator = cms.string('SmartPropagatorAny'),
    RecoGeometry = cms.string('DummyDetLayerGeometry')
)


L3MuKFFitter = cms.ESProducer("KFTrajectoryFitterESProducer",
    appendToDataLabel = cms.string(''),
    minHits = cms.int32(3),
    ComponentName = cms.string('L3MuKFFitter'),
    Estimator = cms.string('Chi2'),
    Updator = cms.string('KFUpdator'),
    Propagator = cms.string('SmartPropagatorAny'),
    RecoGeometry = cms.string('DummyDetLayerGeometry')
)


siStripQualityESProducer = cms.ESProducer("SiStripQualityESProducer",
    appendToDataLabel = cms.string(''),
    PrintDebugOutput = cms.bool(False),
    ThresholdForReducedGranularity = cms.double(0.3),
    UseEmptyRunInfo = cms.bool(False),
    ReduceGranularity = cms.bool(False),
    ListOfRecordToMerge = cms.VPSet(cms.PSet(
        record = cms.string('SiStripDetVOffRcd'),
        tag = cms.string('')
    ), 
        cms.PSet(
            record = cms.string('SiStripDetCablingRcd'),
            tag = cms.string('')
        ), 
        cms.PSet(
            record = cms.string('RunInfoRcd'),
            tag = cms.string('')
        ), 
        cms.PSet(
            record = cms.string('SiStripBadChannelRcd'),
            tag = cms.string('')
        ), 
        cms.PSet(
            record = cms.string('SiStripBadFiberRcd'),
            tag = cms.string('')
        ), 
        cms.PSet(
            record = cms.string('SiStripBadModuleRcd'),
            tag = cms.string('')
        ))
)


ParametrizedMagneticFieldProducer = cms.ESProducer("ParametrizedMagneticFieldProducer",
    version = cms.string('OAE_1103l_071212'),
    parameters = cms.PSet(
        BValue = cms.string('3_8T')
    ),
    label = cms.untracked.string('parametrizedField')
)


HcalTopologyIdealEP = cms.ESProducer("HcalTopologyIdealEP")


hltKFFittingSmoother = cms.ESProducer("KFFittingSmootherESProducer",
    EstimateCut = cms.double(-1.0),
    appendToDataLabel = cms.string(''),
    Fitter = cms.string('hltKFFitter'),
    MinNumberOfHits = cms.int32(5),
    Smoother = cms.string('hltKFSmoother'),
    BreakTrajWith2ConsecutiveMissing = cms.bool(False),
    NoInvalidHitsBeginEnd = cms.bool(False),
    ComponentName = cms.string('hltKFFittingSmoother'),
    RejectTracks = cms.bool(True)
)


AnyDirectionAnalyticalPropagator = cms.ESProducer("AnalyticalPropagatorESProducer",
    MaxDPhi = cms.double(1.6),
    ComponentName = cms.string('AnyDirectionAnalyticalPropagator'),
    appendToDataLabel = cms.string(''),
    PropagationDirection = cms.string('anyDirection')
)


FittingSmootherRK = cms.ESProducer("KFFittingSmootherESProducer",
    EstimateCut = cms.double(-1.0),
    appendToDataLabel = cms.string(''),
    Fitter = cms.string('FitterRK'),
    MinNumberOfHits = cms.int32(5),
    Smoother = cms.string('SmootherRK'),
    BreakTrajWith2ConsecutiveMissing = cms.bool(False),
    NoInvalidHitsBeginEnd = cms.bool(False),
    ComponentName = cms.string('FittingSmootherRK'),
    RejectTracks = cms.bool(True)
)


mixedlayerpairs = cms.ESProducer("SeedingLayersESProducer",
    appendToDataLabel = cms.string(''),
    ComponentName = cms.string('MixedLayerPairs'),
    TEC = cms.PSet(
        useRingSlector = cms.bool(True),
        TTRHBuilder = cms.string('WithTrackAngle'),
        minRing = cms.int32(1),
        maxRing = cms.int32(1)
    ),
    layerList = cms.vstring('BPix1+BPix2', 
        'BPix1+BPix3', 
        'BPix2+BPix3', 
        'BPix1+FPix1_pos', 
        'BPix1+FPix1_neg', 
        'BPix1+FPix2_pos', 
        'BPix1+FPix2_neg', 
        'BPix2+FPix1_pos', 
        'BPix2+FPix1_neg', 
        'BPix2+FPix2_pos', 
        'BPix2+FPix2_neg', 
        'FPix1_pos+FPix2_pos', 
        'FPix1_neg+FPix2_neg', 
        'FPix2_pos+TEC1_pos', 
        'FPix2_pos+TEC2_pos', 
        'TEC1_pos+TEC2_pos', 
        'TEC2_pos+TEC3_pos', 
        'FPix2_neg+TEC1_neg', 
        'FPix2_neg+TEC2_neg', 
        'TEC1_neg+TEC2_neg', 
        'TEC2_neg+TEC3_neg'),
    FPix = cms.PSet(
        hitErrorRZ = cms.double(0.0036),
        hitErrorRPhi = cms.double(0.0051),
        TTRHBuilder = cms.string('TTRHBuilderPixelOnly'),
        HitProducer = cms.string('hltSiPixelRecHits'),
        useErrorsFromParam = cms.bool(True)
    ),
    BPix = cms.PSet(
        hitErrorRZ = cms.double(0.006),
        hitErrorRPhi = cms.double(0.0027),
        TTRHBuilder = cms.string('TTRHBuilderPixelOnly'),
        HitProducer = cms.string('hltSiPixelRecHits'),
        useErrorsFromParam = cms.bool(True)
    )
)


fakeForIdealAlignment = cms.ESProducer("FakeAlignmentProducer",
    appendToDataLabel = cms.string('fakeForIdeal')
)


StraightLinePropagator = cms.ESProducer("StraightLinePropagatorESProducer",
    ComponentName = cms.string('StraightLinePropagator'),
    appendToDataLabel = cms.string(''),
    PropagationDirection = cms.string('alongMomentum')
)


TrackerGeometricDetESModule = cms.ESProducer("TrackerGeometricDetESModule",
    fromDDD = cms.bool(True)
)


FitterRK = cms.ESProducer("KFTrajectoryFitterESProducer",
    appendToDataLabel = cms.string(''),
    minHits = cms.int32(3),
    ComponentName = cms.string('FitterRK'),
    Estimator = cms.string('Chi2'),
    Updator = cms.string('KFUpdator'),
    Propagator = cms.string('RungeKuttaTrackerPropagator'),
    RecoGeometry = cms.string('DummyDetLayerGeometry')
)


trajBuilderL3 = cms.ESProducer("CkfTrajectoryBuilderESProducer",
    propagatorAlong = cms.string('PropagatorWithMaterial'),
    appendToDataLabel = cms.string(''),
    trajectoryFilterName = cms.string('trajFilterL3'),
    maxCand = cms.int32(5),
    ComponentName = cms.string('trajBuilderL3'),
    intermediateCleaning = cms.bool(True),
    MeasurementTrackerName = cms.string(''),
    estimator = cms.string('Chi2'),
    TTRHBuilder = cms.string('WithTrackAngle'),
    updator = cms.string('KFUpdator'),
    alwaysUseInvalidHits = cms.bool(False),
    propagatorOpposite = cms.string('PropagatorWithMaterialOpposite'),
    lostHitPenalty = cms.double(30.0)
)


AnalyticalPropagator = cms.ESProducer("AnalyticalPropagatorESProducer",
    MaxDPhi = cms.double(1.6),
    ComponentName = cms.string('AnalyticalPropagator'),
    appendToDataLabel = cms.string(''),
    PropagationDirection = cms.string('alongMomentum')
)


EcalPreshowerGeometryEP = cms.ESProducer("EcalPreshowerGeometryEP",
    applyAlignment = cms.bool(False)
)


MuonNumberingInitialization = cms.ESProducer("MuonNumberingInitialization")


FastSteppingHelixPropagatorOpposite = cms.ESProducer("SteppingHelixPropagatorESProducer",
    appendToDataLabel = cms.string(''),
    endcapShiftInZNeg = cms.double(0.0),
    PropagationDirection = cms.string('oppositeToMomentum'),
    useMatVolumes = cms.bool(True),
    useTuningForL2Speed = cms.bool(True),
    useIsYokeFlag = cms.bool(True),
    NoErrorPropagation = cms.bool(False),
    SetVBFPointer = cms.bool(False),
    AssumeNoMaterial = cms.bool(False),
    endcapShiftInZPos = cms.double(0.0),
    useInTeslaFromMagField = cms.bool(False),
    VBFName = cms.string('VolumeBasedMagneticField'),
    useEndcapShiftsInZ = cms.bool(False),
    sendLogWarning = cms.bool(False),
    ComponentName = cms.string('FastSteppingHelixPropagatorOpposite'),
    debug = cms.bool(False),
    ApplyRadX0Correction = cms.bool(True),
    useMagVolumes = cms.bool(True),
    returnTangentPlane = cms.bool(True)
)


idealForDigiTrackerGeometry = cms.ESProducer("TrackerDigiGeometryESModule",
    appendToDataLabel = cms.string('idealForDigi'),
    fromDDD = cms.bool(True),
    applyAlignment = cms.bool(False),
    alignmentsLabel = cms.string('fakeForIdeal')
)


PixelCPEGenericESProducer = cms.ESProducer("PixelCPEGenericESProducer",
    EdgeClusterErrorX = cms.double(50.0),
    DoCosmics = cms.bool(False),
    appendToDataLabel = cms.string(''),
    LoadTemplatesFromDB = cms.bool(True),
    UseErrorsFromTemplates = cms.bool(True),
    eff_charge_cut_highX = cms.double(1.0),
    TruncatePixelCharge = cms.bool(True),
    size_cutY = cms.double(3.0),
    size_cutX = cms.double(3.0),
    inflate_all_errors_no_trk_angle = cms.bool(False),
    IrradiationBiasCorrection = cms.bool(False),
    TanLorentzAnglePerTesla = cms.double(0.106),
    inflate_errors = cms.bool(False),
    Alpha2Order = cms.bool(True),
    eff_charge_cut_highY = cms.double(1.0),
    ClusterProbComputationFlag = cms.int32(0),
    EdgeClusterErrorY = cms.double(85.0),
    ComponentName = cms.string('PixelCPEGeneric'),
    eff_charge_cut_lowY = cms.double(0.0),
    PixelErrorParametrization = cms.string('NOTcmsim'),
    eff_charge_cut_lowX = cms.double(0.0)
)


L1DTConfig = cms.ESProducer("DTConfigTrivialProducer",
    DTTPGMap = cms.untracked.PSet(
        wh0st2se11 = cms.untracked.vint32(60, 58, 60, 15),
        wh0st2se10 = cms.untracked.vint32(60, 58, 60, 15),
        wh0st2se12 = cms.untracked.vint32(60, 58, 60, 15),
        whm2st1se4 = cms.untracked.vint32(50, 58, 50, 13),
        whm1st1se12 = cms.untracked.vint32(50, 58, 50, 13),
        whm1st1se11 = cms.untracked.vint32(50, 58, 50, 13),
        whm1st1se10 = cms.untracked.vint32(50, 58, 50, 13),
        whm2st1se1 = cms.untracked.vint32(50, 58, 50, 13),
        whm2st1se2 = cms.untracked.vint32(50, 58, 50, 13),
        whm1st3se4 = cms.untracked.vint32(72, 58, 72, 18),
        wh1st3se4 = cms.untracked.vint32(72, 48, 72, 18),
        whm2st1se8 = cms.untracked.vint32(50, 58, 50, 13),
        whm2st1se9 = cms.untracked.vint32(50, 58, 50, 13),
        wh1st1se1 = cms.untracked.vint32(50, 58, 50, 13),
        wh0st2se6 = cms.untracked.vint32(60, 58, 60, 15),
        whm2st1se12 = cms.untracked.vint32(50, 58, 50, 13),
        whm2st1se10 = cms.untracked.vint32(50, 58, 50, 13),
        whm2st1se11 = cms.untracked.vint32(50, 58, 50, 13),
        whm1st2se12 = cms.untracked.vint32(60, 58, 60, 15),
        wh0st2se1 = cms.untracked.vint32(60, 58, 60, 15),
        wh1st1se3 = cms.untracked.vint32(50, 58, 50, 13),
        wh1st1se2 = cms.untracked.vint32(50, 58, 50, 13),
        whm1st3se3 = cms.untracked.vint32(72, 48, 72, 18),
        whm1st1se7 = cms.untracked.vint32(50, 58, 50, 13),
        wh0st2se2 = cms.untracked.vint32(60, 58, 60, 15),
        whm1st1se6 = cms.untracked.vint32(50, 58, 50, 13),
        wh0st4se8 = cms.untracked.vint32(92, 0, 92, 23),
        whm1st3se11 = cms.untracked.vint32(72, 58, 72, 18),
        wh0st2se9 = cms.untracked.vint32(60, 58, 60, 15),
        wh0st2se8 = cms.untracked.vint32(60, 58, 60, 15),
        whm1st3se10 = cms.untracked.vint32(72, 58, 72, 18),
        whm1st3se5 = cms.untracked.vint32(72, 58, 72, 18),
        wh0st2se4 = cms.untracked.vint32(60, 58, 60, 15),
        wh0st2se7 = cms.untracked.vint32(60, 58, 60, 15),
        whm1st3se6 = cms.untracked.vint32(72, 58, 72, 18),
        whm1st3se1 = cms.untracked.vint32(72, 58, 72, 18),
        whm1st1se9 = cms.untracked.vint32(50, 58, 50, 13),
        wh0st2se3 = cms.untracked.vint32(60, 58, 60, 15),
        whm1st3se2 = cms.untracked.vint32(72, 58, 72, 18),
        whm2st4se14 = cms.untracked.vint32(60, 0, 60, 15),
        whm2st4se13 = cms.untracked.vint32(72, 0, 72, 18),
        whm2st4se12 = cms.untracked.vint32(92, 0, 92, 23),
        whm2st4se11 = cms.untracked.vint32(48, 0, 48, 12),
        whm2st4se10 = cms.untracked.vint32(60, 0, 60, 15),
        whm2st1se6 = cms.untracked.vint32(50, 58, 50, 13),
        wh2st1se10 = cms.untracked.vint32(50, 58, 50, 13),
        wh2st1se11 = cms.untracked.vint32(50, 58, 50, 13),
        wh2st1se12 = cms.untracked.vint32(50, 58, 50, 13),
        whm2st1se7 = cms.untracked.vint32(50, 58, 50, 13),
        whm1st3se7 = cms.untracked.vint32(72, 58, 72, 18),
        whm2st4se9 = cms.untracked.vint32(48, 0, 48, 12),
        whm2st4se8 = cms.untracked.vint32(92, 0, 92, 23),
        whm1st4se12 = cms.untracked.vint32(92, 0, 92, 23),
        whm1st4se13 = cms.untracked.vint32(72, 0, 72, 18),
        whm1st4se10 = cms.untracked.vint32(60, 0, 60, 15),
        whm1st4se11 = cms.untracked.vint32(48, 0, 48, 12),
        whm2st4se3 = cms.untracked.vint32(96, 0, 96, 24),
        whm2st4se2 = cms.untracked.vint32(96, 0, 96, 24),
        whm1st4se14 = cms.untracked.vint32(60, 0, 60, 15),
        whm2st3se8 = cms.untracked.vint32(72, 58, 72, 18),
        whm2st3se9 = cms.untracked.vint32(72, 58, 72, 18),
        whm2st3se6 = cms.untracked.vint32(72, 58, 72, 18),
        whm2st3se7 = cms.untracked.vint32(72, 58, 72, 18),
        whm2st3se4 = cms.untracked.vint32(72, 58, 72, 18),
        whm2st3se5 = cms.untracked.vint32(72, 58, 72, 18),
        whm2st3se2 = cms.untracked.vint32(72, 58, 72, 18),
        whm2st3se3 = cms.untracked.vint32(72, 58, 72, 18),
        whm2st3se1 = cms.untracked.vint32(72, 58, 72, 18),
        wh1st3se12 = cms.untracked.vint32(72, 58, 72, 18),
        wh1st3se11 = cms.untracked.vint32(72, 58, 72, 18),
        wh1st3se10 = cms.untracked.vint32(72, 58, 72, 18),
        wh0st1se12 = cms.untracked.vint32(50, 58, 50, 13),
        wh0st1se10 = cms.untracked.vint32(50, 58, 50, 13),
        wh0st1se11 = cms.untracked.vint32(50, 58, 50, 13),
        wh1st4se6 = cms.untracked.vint32(96, 0, 96, 24),
        wh1st4se7 = cms.untracked.vint32(96, 0, 96, 24),
        wh1st4se4 = cms.untracked.vint32(72, 0, 72, 18),
        wh1st4se5 = cms.untracked.vint32(96, 0, 96, 24),
        wh1st4se2 = cms.untracked.vint32(96, 0, 96, 24),
        wh2st2se12 = cms.untracked.vint32(60, 58, 60, 15),
        wh2st2se11 = cms.untracked.vint32(60, 58, 60, 15),
        wh2st2se10 = cms.untracked.vint32(60, 58, 60, 15),
        wh1st3se3 = cms.untracked.vint32(72, 58, 72, 18),
        wh1st3se2 = cms.untracked.vint32(72, 58, 72, 18),
        wh1st3se1 = cms.untracked.vint32(72, 58, 72, 18),
        wh1st3se7 = cms.untracked.vint32(72, 58, 72, 18),
        wh1st3se6 = cms.untracked.vint32(72, 58, 72, 18),
        wh1st3se5 = cms.untracked.vint32(72, 58, 72, 18),
        wh1st4se9 = cms.untracked.vint32(48, 0, 48, 12),
        whm1st1se3 = cms.untracked.vint32(50, 48, 50, 13),
        whm1st1se2 = cms.untracked.vint32(50, 58, 50, 13),
        whm1st1se1 = cms.untracked.vint32(50, 58, 50, 13),
        wh1st2se8 = cms.untracked.vint32(60, 58, 60, 15),
        wh1st2se9 = cms.untracked.vint32(60, 58, 60, 15),
        whm1st1se5 = cms.untracked.vint32(50, 58, 50, 13),
        whm1st1se4 = cms.untracked.vint32(50, 58, 50, 13),
        wh1st2se4 = cms.untracked.vint32(60, 48, 60, 15),
        wh1st2se5 = cms.untracked.vint32(60, 58, 60, 15),
        wh1st2se6 = cms.untracked.vint32(60, 58, 60, 15),
        wh1st2se7 = cms.untracked.vint32(60, 58, 60, 15),
        wh1st2se1 = cms.untracked.vint32(60, 58, 60, 15),
        wh1st2se2 = cms.untracked.vint32(60, 58, 60, 15),
        wh1st4se8 = cms.untracked.vint32(92, 0, 92, 23),
        wh2st3se9 = cms.untracked.vint32(72, 58, 72, 18),
        wh1st1se8 = cms.untracked.vint32(50, 58, 50, 13),
        whm2st4se7 = cms.untracked.vint32(96, 0, 96, 24),
        whm2st4se6 = cms.untracked.vint32(96, 0, 96, 24),
        whm1st4se4 = cms.untracked.vint32(72, 0, 72, 18),
        whm2st4se5 = cms.untracked.vint32(96, 0, 96, 24),
        wh0st1se8 = cms.untracked.vint32(50, 58, 50, 13),
        wh2st1se7 = cms.untracked.vint32(50, 58, 50, 13),
        wh2st1se4 = cms.untracked.vint32(50, 58, 50, 13),
        whm2st4se4 = cms.untracked.vint32(72, 0, 72, 18),
        wh2st1se2 = cms.untracked.vint32(50, 58, 50, 13),
        wh2st1se3 = cms.untracked.vint32(50, 58, 50, 13),
        wh2st1se1 = cms.untracked.vint32(50, 58, 50, 13),
        wh0st1se1 = cms.untracked.vint32(50, 58, 50, 13),
        wh0st1se2 = cms.untracked.vint32(50, 58, 50, 13),
        wh0st1se3 = cms.untracked.vint32(50, 58, 50, 13),
        wh0st1se4 = cms.untracked.vint32(50, 58, 50, 13),
        wh0st1se5 = cms.untracked.vint32(50, 58, 50, 13),
        wh2st1se8 = cms.untracked.vint32(50, 58, 50, 13),
        wh2st1se9 = cms.untracked.vint32(50, 58, 50, 13),
        wh2st4se1 = cms.untracked.vint32(96, 0, 96, 24),
        wh2st4se3 = cms.untracked.vint32(96, 0, 96, 24),
        wh2st4se2 = cms.untracked.vint32(96, 0, 96, 24),
        wh2st4se5 = cms.untracked.vint32(96, 0, 96, 24),
        wh2st4se4 = cms.untracked.vint32(72, 0, 72, 18),
        wh2st4se7 = cms.untracked.vint32(96, 0, 96, 24),
        wh2st4se6 = cms.untracked.vint32(96, 0, 96, 24),
        wh0st4se3 = cms.untracked.vint32(96, 0, 96, 24),
        wh2st4se8 = cms.untracked.vint32(92, 0, 92, 23),
        wh0st4se1 = cms.untracked.vint32(96, 0, 96, 24),
        wh0st4se7 = cms.untracked.vint32(96, 0, 96, 24),
        wh0st4se6 = cms.untracked.vint32(96, 0, 96, 24),
        wh0st4se5 = cms.untracked.vint32(96, 0, 96, 24),
        wh0st4se4 = cms.untracked.vint32(72, 0, 72, 18),
        wh0st3se10 = cms.untracked.vint32(72, 58, 72, 18),
        wh0st3se11 = cms.untracked.vint32(72, 58, 72, 18),
        wh0st3se12 = cms.untracked.vint32(72, 58, 72, 18),
        wh2st4se11 = cms.untracked.vint32(48, 0, 48, 12),
        wh2st4se10 = cms.untracked.vint32(60, 0, 60, 15),
        wh2st4se13 = cms.untracked.vint32(72, 0, 72, 18),
        wh2st4se12 = cms.untracked.vint32(92, 0, 92, 23),
        wh2st4se14 = cms.untracked.vint32(60, 0, 60, 15),
        wh1st1se5 = cms.untracked.vint32(50, 58, 50, 13),
        whm1st4se1 = cms.untracked.vint32(96, 0, 96, 24),
        whm1st4se2 = cms.untracked.vint32(96, 0, 96, 24),
        whm1st4se3 = cms.untracked.vint32(96, 0, 96, 24),
        wh1st2se12 = cms.untracked.vint32(60, 58, 60, 15),
        whm1st4se5 = cms.untracked.vint32(96, 0, 96, 24),
        wh1st2se10 = cms.untracked.vint32(60, 58, 60, 15),
        whm1st4se7 = cms.untracked.vint32(96, 0, 96, 24),
        whm1st4se8 = cms.untracked.vint32(92, 0, 92, 23),
        whm1st4se9 = cms.untracked.vint32(48, 0, 48, 12),
        whm1st2se11 = cms.untracked.vint32(60, 58, 60, 15),
        wh0st3se6 = cms.untracked.vint32(72, 58, 72, 18),
        wh0st3se7 = cms.untracked.vint32(72, 58, 72, 18),
        wh0st3se4 = cms.untracked.vint32(72, 58, 72, 18),
        wh0st3se5 = cms.untracked.vint32(72, 58, 72, 18),
        wh0st3se2 = cms.untracked.vint32(72, 58, 72, 18),
        wh0st3se3 = cms.untracked.vint32(72, 58, 72, 18),
        wh0st3se1 = cms.untracked.vint32(72, 58, 72, 18),
        wh2st3se7 = cms.untracked.vint32(72, 58, 72, 18),
        whm2st1se5 = cms.untracked.vint32(50, 58, 50, 13),
        wh0st3se8 = cms.untracked.vint32(72, 58, 72, 18),
        wh0st3se9 = cms.untracked.vint32(72, 58, 72, 18),
        wh2st1se6 = cms.untracked.vint32(50, 58, 50, 13),
        whm1st1se8 = cms.untracked.vint32(50, 58, 50, 13),
        wh0st1se9 = cms.untracked.vint32(50, 58, 50, 13),
        wh2st3se12 = cms.untracked.vint32(72, 58, 72, 18),
        wh2st3se10 = cms.untracked.vint32(72, 58, 72, 18),
        wh2st3se11 = cms.untracked.vint32(72, 58, 72, 18),
        wh1st1se11 = cms.untracked.vint32(50, 58, 50, 13),
        wh1st1se10 = cms.untracked.vint32(50, 58, 50, 13),
        wh1st1se12 = cms.untracked.vint32(50, 58, 50, 13),
        wh1st2se11 = cms.untracked.vint32(60, 58, 60, 15),
        whm2st2se5 = cms.untracked.vint32(60, 58, 60, 15),
        whm2st2se4 = cms.untracked.vint32(60, 58, 60, 15),
        whm2st2se7 = cms.untracked.vint32(60, 58, 60, 15),
        whm2st2se6 = cms.untracked.vint32(60, 58, 60, 15),
        whm2st2se1 = cms.untracked.vint32(60, 58, 60, 15),
        whm2st2se3 = cms.untracked.vint32(60, 58, 60, 15),
        whm2st2se2 = cms.untracked.vint32(60, 58, 60, 15),
        whm2st2se9 = cms.untracked.vint32(60, 58, 60, 15),
        whm2st2se8 = cms.untracked.vint32(60, 58, 60, 15),
        whm1st4se6 = cms.untracked.vint32(96, 0, 96, 24),
        wh2st1se5 = cms.untracked.vint32(50, 58, 50, 13),
        whm1st2se6 = cms.untracked.vint32(60, 58, 60, 15),
        whm1st2se7 = cms.untracked.vint32(60, 58, 60, 15),
        whm1st2se4 = cms.untracked.vint32(60, 58, 60, 15),
        whm1st2se5 = cms.untracked.vint32(60, 58, 60, 15),
        whm1st2se2 = cms.untracked.vint32(60, 58, 60, 15),
        whm1st2se3 = cms.untracked.vint32(60, 48, 60, 15),
        whm1st2se1 = cms.untracked.vint32(60, 58, 60, 15),
        whm1st2se8 = cms.untracked.vint32(60, 58, 60, 15),
        whm1st2se9 = cms.untracked.vint32(60, 58, 60, 15),
        wh2st2se7 = cms.untracked.vint32(60, 58, 60, 15),
        wh2st2se6 = cms.untracked.vint32(60, 58, 60, 15),
        wh2st2se5 = cms.untracked.vint32(60, 58, 60, 15),
        wh2st2se4 = cms.untracked.vint32(60, 58, 60, 15),
        wh2st2se3 = cms.untracked.vint32(60, 58, 60, 15),
        wh2st2se2 = cms.untracked.vint32(60, 58, 60, 15),
        wh2st2se1 = cms.untracked.vint32(60, 58, 60, 15),
        wh0st1se7 = cms.untracked.vint32(50, 58, 50, 13),
        wh2st2se9 = cms.untracked.vint32(60, 58, 60, 15),
        wh2st2se8 = cms.untracked.vint32(60, 58, 60, 15),
        wh0st4se13 = cms.untracked.vint32(72, 0, 72, 18),
        wh0st1se6 = cms.untracked.vint32(50, 58, 50, 13),
        wh0st4se11 = cms.untracked.vint32(48, 0, 48, 12),
        wh0st4se10 = cms.untracked.vint32(60, 0, 60, 15),
        whm1st3se12 = cms.untracked.vint32(72, 58, 72, 18),
        wh0st4se14 = cms.untracked.vint32(60, 0, 60, 15),
        whm2st1se3 = cms.untracked.vint32(50, 58, 50, 13),
        wh0st4se9 = cms.untracked.vint32(48, 0, 48, 12),
        wh1st4se10 = cms.untracked.vint32(60, 0, 60, 15),
        wh1st4se11 = cms.untracked.vint32(48, 0, 48, 12),
        wh1st4se12 = cms.untracked.vint32(92, 0, 92, 23),
        wh1st4se13 = cms.untracked.vint32(72, 0, 72, 18),
        wh1st4se14 = cms.untracked.vint32(60, 0, 60, 15),
        wh1st1se4 = cms.untracked.vint32(50, 48, 50, 13),
        wh1st1se7 = cms.untracked.vint32(50, 58, 50, 13),
        wh1st1se6 = cms.untracked.vint32(50, 58, 50, 13),
        wh1st1se9 = cms.untracked.vint32(50, 58, 50, 13),
        whm2st3se10 = cms.untracked.vint32(72, 58, 72, 18),
        wh1st2se3 = cms.untracked.vint32(60, 58, 60, 15),
        whm2st3se11 = cms.untracked.vint32(72, 58, 72, 18),
        whm2st3se12 = cms.untracked.vint32(72, 58, 72, 18),
        wh0st4se12 = cms.untracked.vint32(92, 0, 92, 23),
        wh1st3se9 = cms.untracked.vint32(72, 58, 72, 18),
        wh2st3se8 = cms.untracked.vint32(72, 58, 72, 18),
        wh2st4se9 = cms.untracked.vint32(48, 0, 48, 12),
        wh1st3se8 = cms.untracked.vint32(72, 58, 72, 18),
        wh0st4se2 = cms.untracked.vint32(96, 0, 96, 24),
        wh2st3se1 = cms.untracked.vint32(72, 58, 72, 18),
        wh2st3se2 = cms.untracked.vint32(72, 58, 72, 18),
        wh2st3se3 = cms.untracked.vint32(72, 58, 72, 18),
        wh2st3se4 = cms.untracked.vint32(72, 58, 72, 18),
        wh2st3se5 = cms.untracked.vint32(72, 58, 72, 18),
        wh2st3se6 = cms.untracked.vint32(72, 58, 72, 18),
        wh1st4se3 = cms.untracked.vint32(96, 0, 96, 24),
        whm2st4se1 = cms.untracked.vint32(96, 0, 96, 24),
        wh1st4se1 = cms.untracked.vint32(96, 0, 96, 24),
        whm1st2se10 = cms.untracked.vint32(60, 58, 60, 15),
        whm2st2se11 = cms.untracked.vint32(60, 58, 60, 15),
        whm2st2se10 = cms.untracked.vint32(60, 58, 60, 15),
        whm2st2se12 = cms.untracked.vint32(60, 58, 60, 15),
        wh0st2se5 = cms.untracked.vint32(60, 58, 60, 15),
        whm1st3se9 = cms.untracked.vint32(72, 58, 72, 18),
        whm1st3se8 = cms.untracked.vint32(72, 58, 72, 18)
    ),
    DTTPGParameters = cms.PSet(
        SectCollParameters = cms.PSet(
            SCCSP5 = cms.int32(0),
            SCCSP2 = cms.int32(0),
            SCCSP3 = cms.int32(0),
            SCECF4 = cms.bool(False),
            SCCSP1 = cms.int32(0),
            SCECF2 = cms.bool(False),
            SCECF3 = cms.bool(False),
            SCCSP4 = cms.int32(0),
            SCECF1 = cms.bool(False),
            Debug = cms.untracked.bool(False)
        ),
        Debug = cms.untracked.bool(False),
        TUParameters = cms.PSet(
            TracoParameters = cms.PSet(
                SPRGCOMP = cms.int32(2),
                FHTMSK = cms.int32(0),
                DD = cms.int32(18),
                SSLMSK = cms.int32(0),
                LVALIDIFH = cms.int32(0),
                Debug = cms.untracked.int32(0),
                FSLMSK = cms.int32(0),
                SHTPRF = cms.int32(1),
                SHTMSK = cms.int32(0),
                TRGENB3 = cms.int32(1),
                SHISM = cms.int32(0),
                IBTIOFF = cms.int32(0),
                KPRGCOM = cms.int32(255),
                KRAD = cms.int32(0),
                FLTMSK = cms.int32(0),
                LTS = cms.int32(0),
                SLTMSK = cms.int32(0),
                FPRGCOMP = cms.int32(2),
                TRGENB9 = cms.int32(1),
                TRGENB8 = cms.int32(1),
                FHTPRF = cms.int32(1),
                LTF = cms.int32(0),
                TRGENB1 = cms.int32(1),
                TRGENB0 = cms.int32(1),
                FHISM = cms.int32(0),
                TRGENB2 = cms.int32(1),
                TRGENB5 = cms.int32(1),
                TRGENB4 = cms.int32(1),
                TRGENB7 = cms.int32(1),
                TRGENB6 = cms.int32(1),
                TRGENB15 = cms.int32(1),
                TRGENB14 = cms.int32(1),
                TRGENB11 = cms.int32(1),
                TRGENB10 = cms.int32(1),
                TRGENB13 = cms.int32(1),
                TRGENB12 = cms.int32(1),
                REUSEO = cms.int32(1),
                REUSEI = cms.int32(1),
                BTIC = cms.int32(32)
            ),
            TSPhiParameters = cms.PSet(
                TSMNOE1 = cms.bool(True),
                TSMNOE2 = cms.bool(False),
                TSSMSK1 = cms.int32(312),
                TSTREN9 = cms.bool(True),
                TSTREN8 = cms.bool(True),
                TSTREN11 = cms.bool(True),
                TSTREN3 = cms.bool(True),
                TSTREN2 = cms.bool(True),
                TSTREN1 = cms.bool(True),
                TSTREN0 = cms.bool(True),
                TSTREN7 = cms.bool(True),
                TSTREN6 = cms.bool(True),
                TSTREN5 = cms.bool(True),
                TSTREN4 = cms.bool(True),
                TSSCCE1 = cms.bool(True),
                TSSCCE2 = cms.bool(False),
                TSMCCE2 = cms.bool(False),
                TSTREN19 = cms.bool(True),
                TSMCCE1 = cms.bool(True),
                TSTREN17 = cms.bool(True),
                TSTREN16 = cms.bool(True),
                TSTREN15 = cms.bool(True),
                TSTREN14 = cms.bool(True),
                TSTREN13 = cms.bool(True),
                TSTREN12 = cms.bool(True),
                TSSMSK2 = cms.int32(312),
                TSTREN10 = cms.bool(True),
                TSMMSK2 = cms.int32(312),
                TSMMSK1 = cms.int32(312),
                TSMHSP = cms.int32(1),
                TSSNOE2 = cms.bool(False),
                TSSNOE1 = cms.bool(True),
                TSSCGS2 = cms.bool(True),
                TSSCCEC = cms.bool(False),
                TSMCCEC = cms.bool(False),
                TSMHTE2 = cms.bool(False),
                Debug = cms.untracked.bool(False),
                TSSHTE2 = cms.bool(False),
                TSMCGS1 = cms.bool(True),
                TSMCGS2 = cms.bool(True),
                TSSHTE1 = cms.bool(True),
                TSTREN22 = cms.bool(True),
                TSSNOEC = cms.bool(False),
                TSTREN20 = cms.bool(True),
                TSTREN21 = cms.bool(True),
                TSMGS1 = cms.int32(1),
                TSMGS2 = cms.int32(1),
                TSSHTEC = cms.bool(False),
                TSMWORD = cms.int32(255),
                TSMHTEC = cms.bool(False),
                TSSCGS1 = cms.bool(True),
                TSTREN23 = cms.bool(True),
                TSSGS2 = cms.int32(1),
                TSMNOEC = cms.bool(False),
                TSSGS1 = cms.int32(1),
                TSTREN18 = cms.bool(True),
                TSMHTE1 = cms.bool(True)
            ),
            TSThetaParameters = cms.PSet(
                Debug = cms.untracked.bool(False)
            ),
            Debug = cms.untracked.bool(False),
            DIGIOFFSET = cms.int32(500),
            SINCROTIME = cms.int32(0),
            BtiParameters = cms.PSet(
                KACCTHETA = cms.int32(1),
                WEN8 = cms.int32(1),
                ACH = cms.int32(1),
                DEAD = cms.int32(31),
                ACL = cms.int32(2),
                PTMS20 = cms.int32(1),
                Debug = cms.untracked.int32(0),
                PTMS22 = cms.int32(1),
                PTMS23 = cms.int32(1),
                PTMS24 = cms.int32(1),
                PTMS25 = cms.int32(1),
                PTMS26 = cms.int32(1),
                PTMS27 = cms.int32(1),
                PTMS28 = cms.int32(1),
                PTMS29 = cms.int32(1),
                SET = cms.int32(7),
                RON = cms.bool(True),
                WEN2 = cms.int32(1),
                LL = cms.int32(2),
                LH = cms.int32(21),
                WEN3 = cms.int32(1),
                RE43 = cms.int32(2),
                WEN0 = cms.int32(1),
                RL = cms.int32(42),
                WEN1 = cms.int32(1),
                RH = cms.int32(61),
                LTS = cms.int32(3),
                CH = cms.int32(41),
                CL = cms.int32(22),
                PTMS15 = cms.int32(1),
                PTMS14 = cms.int32(1),
                PTMS17 = cms.int32(1),
                PTMS16 = cms.int32(1),
                PTMS11 = cms.int32(1),
                PTMS10 = cms.int32(1),
                PTMS13 = cms.int32(1),
                PTMS12 = cms.int32(1),
                XON = cms.bool(False),
                WEN7 = cms.int32(1),
                WEN4 = cms.int32(1),
                WEN5 = cms.int32(1),
                PTMS19 = cms.int32(1),
                PTMS18 = cms.int32(1),
                PTMS31 = cms.int32(0),
                PTMS30 = cms.int32(0),
                PTMS5 = cms.int32(1),
                PTMS4 = cms.int32(1),
                PTMS7 = cms.int32(1),
                PTMS6 = cms.int32(1),
                PTMS1 = cms.int32(0),
                PTMS0 = cms.int32(0),
                PTMS3 = cms.int32(0),
                WEN6 = cms.int32(1),
                PTMS2 = cms.int32(0),
                PTMS9 = cms.int32(1),
                PTMS8 = cms.int32(1),
                ST43 = cms.int32(42),
                AC2 = cms.int32(3),
                AC1 = cms.int32(0),
                KMAX = cms.int32(64),
                PTMS21 = cms.int32(1)
            )
        )
    )
)


magfield = cms.ESSource("XMLIdealGeometryESSource",
    geomXMLFiles = cms.vstring('Geometry/CMSCommonData/data/normal/cmsextent.xml', 
        'Geometry/CMSCommonData/data/cms.xml', 
        'Geometry/CMSCommonData/data/cmsMagneticField.xml', 
        'MagneticField/GeomBuilder/data/MagneticFieldVolumes_1103l.xml', 
        'MagneticField/GeomBuilder/data/MagneticFieldParameters_07_2pi.xml', 
        'Geometry/CMSCommonData/data/materials.xml'),
    rootNodeName = cms.string('cmsMagneticField:MAGF')
)


eegeom = cms.ESSource("EmptyESSource",
    iovIsRunNotTime = cms.bool(True),
    recordName = cms.string('EcalMappingRcd'),
    firstValid = cms.vuint32(1)
)


XMLIdealGeometryESSource = cms.ESSource("XMLIdealGeometryESSource",
    geomXMLFiles = (cms.vstring('Geometry/CMSCommonData/data/materials.xml', 'Geometry/CMSCommonData/data/rotations.xml', 'Geometry/CMSCommonData/data/extend/cmsextent.xml', 'Geometry/CMSCommonData/data/cms.xml', 'Geometry/CMSCommonData/data/cmsMother.xml', 'Geometry/CMSCommonData/data/cmsTracker.xml', 'Geometry/CMSCommonData/data/caloBase.xml', 'Geometry/CMSCommonData/data/cmsCalo.xml', 'Geometry/CMSCommonData/data/muonBase.xml', 'Geometry/CMSCommonData/data/cmsMuon.xml', 'Geometry/CMSCommonData/data/mgnt.xml', 'Geometry/CMSCommonData/data/beampipe.xml', 'Geometry/CMSCommonData/data/cmsBeam.xml', 'Geometry/CMSCommonData/data/muonMB.xml', 'Geometry/CMSCommonData/data/muonMagnet.xml', 'Geometry/CMSCommonData/data/cavern.xml', 'Geometry/TrackerCommonData/data/pixfwdMaterials.xml', 'Geometry/TrackerCommonData/data/pixfwdCommon.xml', 'Geometry/TrackerCommonData/data/pixfwdPlaq.xml', 'Geometry/TrackerCommonData/data/pixfwdPlaq1x2.xml', 'Geometry/TrackerCommonData/data/pixfwdPlaq1x5.xml', 'Geometry/TrackerCommonData/data/pixfwdPlaq2x3.xml', 'Geometry/TrackerCommonData/data/pixfwdPlaq2x4.xml', 'Geometry/TrackerCommonData/data/pixfwdPlaq2x5.xml', 'Geometry/TrackerCommonData/data/pixfwdPanelBase.xml', 'Geometry/TrackerCommonData/data/pixfwdPanel.xml', 'Geometry/TrackerCommonData/data/pixfwdBlade.xml', 'Geometry/TrackerCommonData/data/pixfwdNipple.xml', 'Geometry/TrackerCommonData/data/pixfwdDisk.xml', 'Geometry/TrackerCommonData/data/pixfwdCylinder.xml', 'Geometry/TrackerCommonData/data/pixfwd.xml', 'Geometry/TrackerCommonData/data/pixbarmaterial.xml', 'Geometry/TrackerCommonData/data/pixbarladder.xml', 'Geometry/TrackerCommonData/data/pixbarladderfull.xml', 'Geometry/TrackerCommonData/data/pixbarladderhalf.xml', 'Geometry/TrackerCommonData/data/pixbarlayer.xml', 'Geometry/TrackerCommonData/data/pixbarlayer0.xml', 'Geometry/TrackerCommonData/data/pixbarlayer1.xml', 'Geometry/TrackerCommonData/data/pixbarlayer2.xml', 'Geometry/TrackerCommonData/data/pixbar.xml', 'Geometry/TrackerCommonData/data/tibtidcommonmaterial.xml', 'Geometry/TrackerCommonData/data/tibmaterial.xml', 'Geometry/TrackerCommonData/data/tibmodpar.xml', 'Geometry/TrackerCommonData/data/tibmodule0.xml', 'Geometry/TrackerCommonData/data/tibmodule0a.xml', 'Geometry/TrackerCommonData/data/tibmodule0b.xml', 'Geometry/TrackerCommonData/data/tibmodule2.xml', 'Geometry/TrackerCommonData/data/tibstringpar.xml', 'Geometry/TrackerCommonData/data/tibstring0ll.xml', 'Geometry/TrackerCommonData/data/tibstring0lr.xml', 'Geometry/TrackerCommonData/data/tibstring0ul.xml', 'Geometry/TrackerCommonData/data/tibstring0ur.xml', 'Geometry/TrackerCommonData/data/tibstring0.xml', 'Geometry/TrackerCommonData/data/tibstring1ll.xml', 'Geometry/TrackerCommonData/data/tibstring1lr.xml', 'Geometry/TrackerCommonData/data/tibstring1ul.xml', 'Geometry/TrackerCommonData/data/tibstring1ur.xml', 'Geometry/TrackerCommonData/data/tibstring1.xml', 'Geometry/TrackerCommonData/data/tibstring2ll.xml', 'Geometry/TrackerCommonData/data/tibstring2lr.xml', 'Geometry/TrackerCommonData/data/tibstring2ul.xml', 'Geometry/TrackerCommonData/data/tibstring2ur.xml', 'Geometry/TrackerCommonData/data/tibstring2.xml', 'Geometry/TrackerCommonData/data/tibstring3ll.xml', 'Geometry/TrackerCommonData/data/tibstring3lr.xml', 'Geometry/TrackerCommonData/data/tibstring3ul.xml', 'Geometry/TrackerCommonData/data/tibstring3ur.xml', 'Geometry/TrackerCommonData/data/tibstring3.xml', 'Geometry/TrackerCommonData/data/tiblayerpar.xml', 'Geometry/TrackerCommonData/data/tiblayer0.xml', 'Geometry/TrackerCommonData/data/tiblayer1.xml', 'Geometry/TrackerCommonData/data/tiblayer2.xml', 'Geometry/TrackerCommonData/data/tiblayer3.xml', 'Geometry/TrackerCommonData/data/tib.xml', 'Geometry/TrackerCommonData/data/tidmaterial.xml', 'Geometry/TrackerCommonData/data/tidmodpar.xml', 'Geometry/TrackerCommonData/data/tidmodule0.xml', 'Geometry/TrackerCommonData/data/tidmodule0r.xml', 'Geometry/TrackerCommonData/data/tidmodule0l.xml', 'Geometry/TrackerCommonData/data/tidmodule1.xml', 'Geometry/TrackerCommonData/data/tidmodule1r.xml', 'Geometry/TrackerCommonData/data/tidmodule1l.xml', 'Geometry/TrackerCommonData/data/tidmodule2.xml', 'Geometry/TrackerCommonData/data/tidringpar.xml', 'Geometry/TrackerCommonData/data/tidring0.xml', 'Geometry/TrackerCommonData/data/tidring0f.xml', 'Geometry/TrackerCommonData/data/tidring0b.xml', 'Geometry/TrackerCommonData/data/tidring1.xml', 'Geometry/TrackerCommonData/data/tidring1f.xml', 'Geometry/TrackerCommonData/data/tidring1b.xml', 'Geometry/TrackerCommonData/data/tidring2.xml', 'Geometry/TrackerCommonData/data/tid.xml', 'Geometry/TrackerCommonData/data/tidf.xml', 'Geometry/TrackerCommonData/data/tidb.xml', 'Geometry/TrackerCommonData/data/tibtidservices.xml', 'Geometry/TrackerCommonData/data/tibtidservicesf.xml', 'Geometry/TrackerCommonData/data/tibtidservicesb.xml', 'Geometry/TrackerCommonData/data/tobmaterial.xml', 'Geometry/TrackerCommonData/data/tobmodpar.xml', 'Geometry/TrackerCommonData/data/tobmodule0.xml', 'Geometry/TrackerCommonData/data/tobmodule2.xml', 'Geometry/TrackerCommonData/data/tobmodule4.xml', 'Geometry/TrackerCommonData/data/tobrodpar.xml', 'Geometry/TrackerCommonData/data/tobrod0c.xml', 'Geometry/TrackerCommonData/data/tobrod0l.xml', 'Geometry/TrackerCommonData/data/tobrod0h.xml', 'Geometry/TrackerCommonData/data/tobrod0.xml', 'Geometry/TrackerCommonData/data/tobrod1l.xml', 'Geometry/TrackerCommonData/data/tobrod1h.xml', 'Geometry/TrackerCommonData/data/tobrod1.xml', 'Geometry/TrackerCommonData/data/tobrod2c.xml', 'Geometry/TrackerCommonData/data/tobrod2l.xml', 'Geometry/TrackerCommonData/data/tobrod2h.xml', 'Geometry/TrackerCommonData/data/tobrod2.xml', 'Geometry/TrackerCommonData/data/tobrod3l.xml', 'Geometry/TrackerCommonData/data/tobrod3h.xml', 'Geometry/TrackerCommonData/data/tobrod3.xml', 'Geometry/TrackerCommonData/data/tobrod4c.xml', 'Geometry/TrackerCommonData/data/tobrod4l.xml', 'Geometry/TrackerCommonData/data/tobrod4h.xml', 'Geometry/TrackerCommonData/data/tobrod4.xml', 'Geometry/TrackerCommonData/data/tobrod5l.xml', 'Geometry/TrackerCommonData/data/tobrod5h.xml', 'Geometry/TrackerCommonData/data/tobrod5.xml', 'Geometry/TrackerCommonData/data/tob.xml', 'Geometry/TrackerCommonData/data/tecmaterial.xml', 'Geometry/TrackerCommonData/data/tecmodpar.xml', 'Geometry/TrackerCommonData/data/tecmodule0.xml', 'Geometry/TrackerCommonData/data/tecmodule0r.xml', 'Geometry/TrackerCommonData/data/tecmodule0s.xml', 'Geometry/TrackerCommonData/data/tecmodule1.xml', 'Geometry/TrackerCommonData/data/tecmodule1r.xml', 'Geometry/TrackerCommonData/data/tecmodule1s.xml', 'Geometry/TrackerCommonData/data/tecmodule2.xml', 'Geometry/TrackerCommonData/data/tecmodule3.xml', 'Geometry/TrackerCommonData/data/tecmodule4.xml', 'Geometry/TrackerCommonData/data/tecmodule4r.xml', 'Geometry/TrackerCommonData/data/tecmodule4s.xml', 'Geometry/TrackerCommonData/data/tecmodule5.xml', 'Geometry/TrackerCommonData/data/tecmodule6.xml', 'Geometry/TrackerCommonData/data/tecpetpar.xml', 'Geometry/TrackerCommonData/data/tecring0.xml', 'Geometry/TrackerCommonData/data/tecring1.xml', 'Geometry/TrackerCommonData/data/tecring2.xml', 'Geometry/TrackerCommonData/data/tecring3.xml', 'Geometry/TrackerCommonData/data/tecring4.xml', 'Geometry/TrackerCommonData/data/tecring5.xml', 'Geometry/TrackerCommonData/data/tecring6.xml', 'Geometry/TrackerCommonData/data/tecring0f.xml', 'Geometry/TrackerCommonData/data/tecring1f.xml', 'Geometry/TrackerCommonData/data/tecring2f.xml', 'Geometry/TrackerCommonData/data/tecring3f.xml', 'Geometry/TrackerCommonData/data/tecring4f.xml', 'Geometry/TrackerCommonData/data/tecring5f.xml', 'Geometry/TrackerCommonData/data/tecring6f.xml', 'Geometry/TrackerCommonData/data/tecring0b.xml', 'Geometry/TrackerCommonData/data/tecring1b.xml', 'Geometry/TrackerCommonData/data/tecring2b.xml', 'Geometry/TrackerCommonData/data/tecring3b.xml', 'Geometry/TrackerCommonData/data/tecring4b.xml', 'Geometry/TrackerCommonData/data/tecring5b.xml', 'Geometry/TrackerCommonData/data/tecring6b.xml', 'Geometry/TrackerCommonData/data/tecpetalf.xml', 'Geometry/TrackerCommonData/data/tecpetalb.xml', 'Geometry/TrackerCommonData/data/tecpetal0.xml', 'Geometry/TrackerCommonData/data/tecpetal0f.xml', 'Geometry/TrackerCommonData/data/tecpetal0b.xml', 'Geometry/TrackerCommonData/data/tecpetal3.xml', 'Geometry/TrackerCommonData/data/tecpetal3f.xml', 'Geometry/TrackerCommonData/data/tecpetal3b.xml', 'Geometry/TrackerCommonData/data/tecpetal6f.xml', 'Geometry/TrackerCommonData/data/tecpetal6b.xml', 'Geometry/TrackerCommonData/data/tecpetal8f.xml', 'Geometry/TrackerCommonData/data/tecpetal8b.xml', 'Geometry/TrackerCommonData/data/tecwheel.xml', 'Geometry/TrackerCommonData/data/tecwheela.xml', 'Geometry/TrackerCommonData/data/tecwheelb.xml', 'Geometry/TrackerCommonData/data/tecwheelc.xml', 'Geometry/TrackerCommonData/data/tecwheeld.xml', 'Geometry/TrackerCommonData/data/tecwheel6.xml', 'Geometry/TrackerCommonData/data/tecservices.xml', 'Geometry/TrackerCommonData/data/tecbackplate.xml', 'Geometry/TrackerCommonData/data/tec.xml', 'Geometry/TrackerCommonData/data/trackermaterial.xml', 'Geometry/TrackerCommonData/data/tracker.xml', 'Geometry/TrackerCommonData/data/trackerpixbar.xml', 'Geometry/TrackerCommonData/data/trackerpixfwd.xml', 'Geometry/TrackerCommonData/data/trackertibtidservices.xml', 'Geometry/TrackerCommonData/data/trackertib.xml', 'Geometry/TrackerCommonData/data/trackertid.xml', 'Geometry/TrackerCommonData/data/trackertob.xml', 'Geometry/TrackerCommonData/data/trackertec.xml', 'Geometry/TrackerCommonData/data/trackerbulkhead.xml', 'Geometry/TrackerCommonData/data/trackerother.xml', 'Geometry/EcalCommonData/data/eregalgo.xml', 'Geometry/EcalCommonData/data/ebalgo.xml', 'Geometry/EcalCommonData/data/ebcon.xml', 'Geometry/EcalCommonData/data/ebrot.xml', 'Geometry/EcalCommonData/data/eecon.xml', 'Geometry/EcalCommonData/data/eefixed.xml', 'Geometry/EcalCommonData/data/eehier.xml', 'Geometry/EcalCommonData/data/eealgo.xml', 'Geometry/EcalCommonData/data/escon.xml', 'Geometry/EcalCommonData/data/esalgo.xml', 'Geometry/EcalCommonData/data/eeF.xml', 'Geometry/EcalCommonData/data/eeB.xml', 'Geometry/HcalCommonData/data/hcalrotations.xml', 'Geometry/HcalCommonData/data/hcalalgo.xml', 'Geometry/HcalCommonData/data/hcalbarrelalgo.xml', 'Geometry/HcalCommonData/data/hcalendcapalgo.xml', 'Geometry/HcalCommonData/data/hcalouteralgo.xml', 'Geometry/HcalCommonData/data/hcalforwardalgo.xml', 'Geometry/HcalCommonData/data/hcalforwardfibre.xml', 'Geometry/HcalCommonData/data/hcalforwardmaterial.xml', 'Geometry/MuonCommonData/data/mbCommon.xml', 'Geometry/MuonCommonData/data/mb1.xml', 'Geometry/MuonCommonData/data/mb2.xml', 'Geometry/MuonCommonData/data/mb3.xml', 'Geometry/MuonCommonData/data/mb4.xml', 'Geometry/MuonCommonData/data/muonYoke.xml', 'Geometry/MuonCommonData/data/mf.xml', 'Geometry/ForwardCommonData/data/forward.xml', 'Geometry/ForwardCommonData/data/forwardshield.xml', 'Geometry/ForwardCommonData/data/brmrotations.xml', 'Geometry/ForwardCommonData/data/brm.xml', 'Geometry/ForwardCommonData/data/totemMaterials.xml', 'Geometry/ForwardCommonData/data/totemRotations.xml', 'Geometry/ForwardCommonData/data/totemt1.xml', 'Geometry/ForwardCommonData/data/totemt2.xml', 'Geometry/ForwardCommonData/data/ionpump.xml', 'Geometry/ForwardCommonData/data/castor.xml', 'Geometry/ForwardCommonData/data/zdcmaterials.xml', 'Geometry/ForwardCommonData/data/lumimaterials.xml', 'Geometry/ForwardCommonData/data/zdcrotations.xml', 'Geometry/ForwardCommonData/data/lumirotations.xml', 'Geometry/ForwardCommonData/data/zdc.xml', 'Geometry/ForwardCommonData/data/zdclumi.xml', 'Geometry/ForwardCommonData/data/cmszdc.xml', 'Geometry/MuonCommonData/data/muonNumbering.xml', 'Geometry/TrackerCommonData/data/trackerStructureTopology.xml', 'Geometry/TrackerSimData/data/trackersens.xml', 'Geometry/TrackerRecoData/data/trackerRecoMaterial.xml', 'Geometry/EcalSimData/data/ecalsens.xml', 'Geometry/HcalCommonData/data/hcalsens.xml', 'Geometry/HcalSimData/data/CaloUtil.xml', 'Geometry/MuonSimData/data/muonSens.xml', 'Geometry/DTGeometryBuilder/data/dtSpecsFilter.xml', 'Geometry/CSCGeometryBuilder/data/cscSpecsFilter.xml', 'Geometry/CSCGeometryBuilder/data/cscSpecs.xml', 'Geometry/RPCGeometryBuilder/data/RPCSpecs.xml', 'Geometry/ForwardCommonData/data/brmsens.xml', 'Geometry/ForwardSimData/data/castorsens.xml', 'Geometry/ForwardSimData/data/zdcsens.xml', 'Geometry/HcalSimData/data/CaloProdCuts.xml', 'Geometry/HcalSimData/data/HcalProdCuts.xml')+cms.vstring('Geometry/EcalSimData/data/EcalProdCuts.xml', 'Geometry/TrackerSimData/data/trackerProdCuts.xml', 'Geometry/TrackerSimData/data/trackerProdCutsBEAM.xml', 'Geometry/MuonSimData/data/muonProdCuts.xml', 'Geometry/ForwardSimData/data/CastorProdCuts.xml', 'Geometry/ForwardSimData/data/zdcProdCuts.xml', 'Geometry/ForwardSimData/data/ForwardShieldProdCuts.xml', 'Geometry/CMSCommonData/data/FieldParameters.xml')),
    rootNodeName = cms.string('cms:OCMS')
)


MCJetCorrectorIcone5HF07 = cms.ESSource("L2RelativeCorrectionService",
    appendToDataLabel = cms.string(''),
    tagName = cms.string('HLT_L2Relative'),
    label = cms.string('MCJetCorrectorIcone5HF07')
)


es_hardcode = cms.ESSource("HcalHardcodeCalibrations",
    toGet = cms.untracked.vstring('GainWidths')
)


BTagRecord = cms.ESSource("EmptyESSource",
    iovIsRunNotTime = cms.bool(True),
    recordName = cms.string('JetTagComputerRecord'),
    firstValid = cms.vuint32(1),
    appendToDataLabel = cms.string('')
)


MCJetCorrectorIcone5 = cms.ESSource("JetCorrectionServiceChain",
    correctors = cms.vstring('L2RelativeJetCorrector', 
        'L3AbsoluteJetCorrector'),
    appendToDataLabel = cms.string(''),
    label = cms.string('MCJetCorrectorIcone5')
)


essourceSev = cms.ESSource("EmptyESSource",
    iovIsRunNotTime = cms.bool(True),
    recordName = cms.string('HcalSeverityLevelComputerRcd'),
    firstValid = cms.vuint32(1),
    appendToDataLabel = cms.string('')
)


L3AbsoluteCorrectionService = cms.ESSource("L3AbsoluteCorrectionService",
    appendToDataLabel = cms.string(''),
    tagName = cms.string('Summer09_7TeV_L3Absolute_IC5Calo'),
    label = cms.string('L3AbsoluteJetCorrector')
)


rcdsrc = cms.ESSource("EmptyESSource",
    iovIsRunNotTime = cms.bool(True),
    recordName = cms.string('DTConfigManagerRcd'),
    firstValid = cms.vuint32(1)
)


HepPDTESSource = cms.ESSource("HepPDTESSource",
    pdtFileName = cms.FileInPath('SimGeneral/HepPDTESSource/data/pythiaparticle.tbl')
)


L2RelativeCorrectionService = cms.ESSource("L2RelativeCorrectionService",
    appendToDataLabel = cms.string(''),
    tagName = cms.string('Summer09_7TeV_L2Relative_IC5Calo'),
    label = cms.string('L2RelativeJetCorrector')
)


rpcconesrc = cms.ESSource("EmptyESSource",
    iovIsRunNotTime = cms.bool(True),
    recordName = cms.string('L1RPCConeBuilderRcd'),
    firstValid = cms.vuint32(1)
)


#GlobalTag = cms.ESSource("PoolDBESSource",
#    DBParameters = cms.PSet(
#        authenticationPath = cms.untracked.string('.'),
#        enableReadOnlySessionOnUpdateConnection = cms.untracked.bool(False),
#        idleConnectionCleanupPeriod = cms.untracked.int32(10),
#        messageLevel = cms.untracked.int32(0),
#        enablePoolAutomaticCleanUp = cms.untracked.bool(False),
#        enableConnectionSharing = cms.untracked.bool(True),
#        connectionRetrialTimeOut = cms.untracked.int32(60),
#        connectionTimeOut = cms.untracked.int32(60),
#        connectionRetrialPeriod = cms.untracked.int32(10)
#    ),
#    BlobStreamerName = cms.untracked.string('TBufferBlobStreamingService'),
#    connect = cms.string('frontier://FrontierProd/CMS_COND_31X_GLOBALTAG'),
#    globaltag = cms.string('GR09_R_34X_V5::All')
#)


MCJetCorrectorIcone5Unit = cms.ESSource("L2RelativeCorrectionService",
    appendToDataLabel = cms.string(''),
    tagName = cms.string('HLT_L2RelativeFlat'),
    label = cms.string('MCJetCorrectorIcone5Unit')
)


#prefer("magfield")

BtiParametersBlock = cms.PSet(
    BtiParameters = cms.PSet(
        KACCTHETA = cms.int32(1),
        WEN8 = cms.int32(1),
        ACH = cms.int32(1),
        DEAD = cms.int32(31),
        ACL = cms.int32(2),
        PTMS20 = cms.int32(1),
        Debug = cms.untracked.int32(0),
        PTMS22 = cms.int32(1),
        PTMS23 = cms.int32(1),
        PTMS24 = cms.int32(1),
        PTMS25 = cms.int32(1),
        PTMS26 = cms.int32(1),
        PTMS27 = cms.int32(1),
        PTMS28 = cms.int32(1),
        PTMS29 = cms.int32(1),
        SET = cms.int32(7),
        RON = cms.bool(True),
        WEN2 = cms.int32(1),
        LL = cms.int32(2),
        LH = cms.int32(21),
        WEN3 = cms.int32(1),
        RE43 = cms.int32(2),
        WEN0 = cms.int32(1),
        RL = cms.int32(42),
        WEN1 = cms.int32(1),
        RH = cms.int32(61),
        LTS = cms.int32(3),
        CH = cms.int32(41),
        CL = cms.int32(22),
        PTMS15 = cms.int32(1),
        PTMS14 = cms.int32(1),
        PTMS17 = cms.int32(1),
        PTMS16 = cms.int32(1),
        PTMS11 = cms.int32(1),
        PTMS10 = cms.int32(1),
        PTMS13 = cms.int32(1),
        PTMS12 = cms.int32(1),
        XON = cms.bool(False),
        WEN7 = cms.int32(1),
        WEN4 = cms.int32(1),
        WEN5 = cms.int32(1),
        PTMS19 = cms.int32(1),
        PTMS18 = cms.int32(1),
        PTMS31 = cms.int32(0),
        PTMS30 = cms.int32(0),
        PTMS5 = cms.int32(1),
        PTMS4 = cms.int32(1),
        PTMS7 = cms.int32(1),
        PTMS6 = cms.int32(1),
        PTMS1 = cms.int32(0),
        PTMS0 = cms.int32(0),
        PTMS3 = cms.int32(0),
        WEN6 = cms.int32(1),
        PTMS2 = cms.int32(0),
        PTMS9 = cms.int32(1),
        PTMS8 = cms.int32(1),
        ST43 = cms.int32(42),
        AC2 = cms.int32(3),
        AC1 = cms.int32(0),
        KMAX = cms.int32(64),
        PTMS21 = cms.int32(1)
    )
)

TracoParametersBlock = cms.PSet(
    TracoParameters = cms.PSet(
        SPRGCOMP = cms.int32(2),
        FHTMSK = cms.int32(0),
        DD = cms.int32(18),
        SSLMSK = cms.int32(0),
        LVALIDIFH = cms.int32(0),
        Debug = cms.untracked.int32(0),
        FSLMSK = cms.int32(0),
        SHTPRF = cms.int32(1),
        SHTMSK = cms.int32(0),
        TRGENB3 = cms.int32(1),
        SHISM = cms.int32(0),
        IBTIOFF = cms.int32(0),
        KPRGCOM = cms.int32(255),
        KRAD = cms.int32(0),
        FLTMSK = cms.int32(0),
        LTS = cms.int32(0),
        SLTMSK = cms.int32(0),
        FPRGCOMP = cms.int32(2),
        TRGENB9 = cms.int32(1),
        TRGENB8 = cms.int32(1),
        FHTPRF = cms.int32(1),
        LTF = cms.int32(0),
        TRGENB1 = cms.int32(1),
        TRGENB0 = cms.int32(1),
        FHISM = cms.int32(0),
        TRGENB2 = cms.int32(1),
        TRGENB5 = cms.int32(1),
        TRGENB4 = cms.int32(1),
        TRGENB7 = cms.int32(1),
        TRGENB6 = cms.int32(1),
        TRGENB15 = cms.int32(1),
        TRGENB14 = cms.int32(1),
        TRGENB11 = cms.int32(1),
        TRGENB10 = cms.int32(1),
        TRGENB13 = cms.int32(1),
        TRGENB12 = cms.int32(1),
        REUSEO = cms.int32(1),
        REUSEI = cms.int32(1),
        BTIC = cms.int32(32)
    )
)

DTTPGMapBlock = cms.PSet(
    DTTPGMap = cms.untracked.PSet(
        wh0st2se11 = cms.untracked.vint32(60, 58, 60, 15),
        wh0st2se10 = cms.untracked.vint32(60, 58, 60, 15),
        wh0st2se12 = cms.untracked.vint32(60, 58, 60, 15),
        whm2st1se4 = cms.untracked.vint32(50, 58, 50, 13),
        whm1st1se12 = cms.untracked.vint32(50, 58, 50, 13),
        whm1st1se11 = cms.untracked.vint32(50, 58, 50, 13),
        whm1st1se10 = cms.untracked.vint32(50, 58, 50, 13),
        whm2st1se1 = cms.untracked.vint32(50, 58, 50, 13),
        whm2st1se2 = cms.untracked.vint32(50, 58, 50, 13),
        whm1st3se4 = cms.untracked.vint32(72, 58, 72, 18),
        wh1st3se4 = cms.untracked.vint32(72, 48, 72, 18),
        whm2st1se8 = cms.untracked.vint32(50, 58, 50, 13),
        whm2st1se9 = cms.untracked.vint32(50, 58, 50, 13),
        wh1st1se1 = cms.untracked.vint32(50, 58, 50, 13),
        wh0st2se6 = cms.untracked.vint32(60, 58, 60, 15),
        whm2st1se12 = cms.untracked.vint32(50, 58, 50, 13),
        whm2st1se10 = cms.untracked.vint32(50, 58, 50, 13),
        whm2st1se11 = cms.untracked.vint32(50, 58, 50, 13),
        whm1st2se12 = cms.untracked.vint32(60, 58, 60, 15),
        wh0st2se1 = cms.untracked.vint32(60, 58, 60, 15),
        wh1st1se3 = cms.untracked.vint32(50, 58, 50, 13),
        wh1st1se2 = cms.untracked.vint32(50, 58, 50, 13),
        whm1st3se3 = cms.untracked.vint32(72, 48, 72, 18),
        whm1st1se7 = cms.untracked.vint32(50, 58, 50, 13),
        wh0st2se2 = cms.untracked.vint32(60, 58, 60, 15),
        whm1st1se6 = cms.untracked.vint32(50, 58, 50, 13),
        wh0st4se8 = cms.untracked.vint32(92, 0, 92, 23),
        whm1st3se11 = cms.untracked.vint32(72, 58, 72, 18),
        wh0st2se9 = cms.untracked.vint32(60, 58, 60, 15),
        wh0st2se8 = cms.untracked.vint32(60, 58, 60, 15),
        whm1st3se10 = cms.untracked.vint32(72, 58, 72, 18),
        whm1st3se5 = cms.untracked.vint32(72, 58, 72, 18),
        wh0st2se4 = cms.untracked.vint32(60, 58, 60, 15),
        wh0st2se7 = cms.untracked.vint32(60, 58, 60, 15),
        whm1st3se6 = cms.untracked.vint32(72, 58, 72, 18),
        whm1st3se1 = cms.untracked.vint32(72, 58, 72, 18),
        whm1st1se9 = cms.untracked.vint32(50, 58, 50, 13),
        wh0st2se3 = cms.untracked.vint32(60, 58, 60, 15),
        whm1st3se2 = cms.untracked.vint32(72, 58, 72, 18),
        whm2st4se14 = cms.untracked.vint32(60, 0, 60, 15),
        whm2st4se13 = cms.untracked.vint32(72, 0, 72, 18),
        whm2st4se12 = cms.untracked.vint32(92, 0, 92, 23),
        whm2st4se11 = cms.untracked.vint32(48, 0, 48, 12),
        whm2st4se10 = cms.untracked.vint32(60, 0, 60, 15),
        whm2st1se6 = cms.untracked.vint32(50, 58, 50, 13),
        wh2st1se10 = cms.untracked.vint32(50, 58, 50, 13),
        wh2st1se11 = cms.untracked.vint32(50, 58, 50, 13),
        wh2st1se12 = cms.untracked.vint32(50, 58, 50, 13),
        whm2st1se7 = cms.untracked.vint32(50, 58, 50, 13),
        whm1st3se7 = cms.untracked.vint32(72, 58, 72, 18),
        whm2st4se9 = cms.untracked.vint32(48, 0, 48, 12),
        whm2st4se8 = cms.untracked.vint32(92, 0, 92, 23),
        whm1st4se12 = cms.untracked.vint32(92, 0, 92, 23),
        whm1st4se13 = cms.untracked.vint32(72, 0, 72, 18),
        whm1st4se10 = cms.untracked.vint32(60, 0, 60, 15),
        whm1st4se11 = cms.untracked.vint32(48, 0, 48, 12),
        whm2st4se3 = cms.untracked.vint32(96, 0, 96, 24),
        whm2st4se2 = cms.untracked.vint32(96, 0, 96, 24),
        whm1st4se14 = cms.untracked.vint32(60, 0, 60, 15),
        whm2st3se8 = cms.untracked.vint32(72, 58, 72, 18),
        whm2st3se9 = cms.untracked.vint32(72, 58, 72, 18),
        whm2st3se6 = cms.untracked.vint32(72, 58, 72, 18),
        whm2st3se7 = cms.untracked.vint32(72, 58, 72, 18),
        whm2st3se4 = cms.untracked.vint32(72, 58, 72, 18),
        whm2st3se5 = cms.untracked.vint32(72, 58, 72, 18),
        whm2st3se2 = cms.untracked.vint32(72, 58, 72, 18),
        whm2st3se3 = cms.untracked.vint32(72, 58, 72, 18),
        whm2st3se1 = cms.untracked.vint32(72, 58, 72, 18),
        wh1st3se12 = cms.untracked.vint32(72, 58, 72, 18),
        wh1st3se11 = cms.untracked.vint32(72, 58, 72, 18),
        wh1st3se10 = cms.untracked.vint32(72, 58, 72, 18),
        wh0st1se12 = cms.untracked.vint32(50, 58, 50, 13),
        wh0st1se10 = cms.untracked.vint32(50, 58, 50, 13),
        wh0st1se11 = cms.untracked.vint32(50, 58, 50, 13),
        wh1st4se6 = cms.untracked.vint32(96, 0, 96, 24),
        wh1st4se7 = cms.untracked.vint32(96, 0, 96, 24),
        wh1st4se4 = cms.untracked.vint32(72, 0, 72, 18),
        wh1st4se5 = cms.untracked.vint32(96, 0, 96, 24),
        wh1st4se2 = cms.untracked.vint32(96, 0, 96, 24),
        wh2st2se12 = cms.untracked.vint32(60, 58, 60, 15),
        wh2st2se11 = cms.untracked.vint32(60, 58, 60, 15),
        wh2st2se10 = cms.untracked.vint32(60, 58, 60, 15),
        wh1st3se3 = cms.untracked.vint32(72, 58, 72, 18),
        wh1st3se2 = cms.untracked.vint32(72, 58, 72, 18),
        wh1st3se1 = cms.untracked.vint32(72, 58, 72, 18),
        wh1st3se7 = cms.untracked.vint32(72, 58, 72, 18),
        wh1st3se6 = cms.untracked.vint32(72, 58, 72, 18),
        wh1st3se5 = cms.untracked.vint32(72, 58, 72, 18),
        wh1st4se9 = cms.untracked.vint32(48, 0, 48, 12),
        whm1st1se3 = cms.untracked.vint32(50, 48, 50, 13),
        whm1st1se2 = cms.untracked.vint32(50, 58, 50, 13),
        whm1st1se1 = cms.untracked.vint32(50, 58, 50, 13),
        wh1st2se8 = cms.untracked.vint32(60, 58, 60, 15),
        wh1st2se9 = cms.untracked.vint32(60, 58, 60, 15),
        whm1st1se5 = cms.untracked.vint32(50, 58, 50, 13),
        whm1st1se4 = cms.untracked.vint32(50, 58, 50, 13),
        wh1st2se4 = cms.untracked.vint32(60, 48, 60, 15),
        wh1st2se5 = cms.untracked.vint32(60, 58, 60, 15),
        wh1st2se6 = cms.untracked.vint32(60, 58, 60, 15),
        wh1st2se7 = cms.untracked.vint32(60, 58, 60, 15),
        wh1st2se1 = cms.untracked.vint32(60, 58, 60, 15),
        wh1st2se2 = cms.untracked.vint32(60, 58, 60, 15),
        wh1st4se8 = cms.untracked.vint32(92, 0, 92, 23),
        wh2st3se9 = cms.untracked.vint32(72, 58, 72, 18),
        wh1st1se8 = cms.untracked.vint32(50, 58, 50, 13),
        whm2st4se7 = cms.untracked.vint32(96, 0, 96, 24),
        whm2st4se6 = cms.untracked.vint32(96, 0, 96, 24),
        whm1st4se4 = cms.untracked.vint32(72, 0, 72, 18),
        whm2st4se5 = cms.untracked.vint32(96, 0, 96, 24),
        wh0st1se8 = cms.untracked.vint32(50, 58, 50, 13),
        wh2st1se7 = cms.untracked.vint32(50, 58, 50, 13),
        wh2st1se4 = cms.untracked.vint32(50, 58, 50, 13),
        whm2st4se4 = cms.untracked.vint32(72, 0, 72, 18),
        wh2st1se2 = cms.untracked.vint32(50, 58, 50, 13),
        wh2st1se3 = cms.untracked.vint32(50, 58, 50, 13),
        wh2st1se1 = cms.untracked.vint32(50, 58, 50, 13),
        wh0st1se1 = cms.untracked.vint32(50, 58, 50, 13),
        wh0st1se2 = cms.untracked.vint32(50, 58, 50, 13),
        wh0st1se3 = cms.untracked.vint32(50, 58, 50, 13),
        wh0st1se4 = cms.untracked.vint32(50, 58, 50, 13),
        wh0st1se5 = cms.untracked.vint32(50, 58, 50, 13),
        wh2st1se8 = cms.untracked.vint32(50, 58, 50, 13),
        wh2st1se9 = cms.untracked.vint32(50, 58, 50, 13),
        wh2st4se1 = cms.untracked.vint32(96, 0, 96, 24),
        wh2st4se3 = cms.untracked.vint32(96, 0, 96, 24),
        wh2st4se2 = cms.untracked.vint32(96, 0, 96, 24),
        wh2st4se5 = cms.untracked.vint32(96, 0, 96, 24),
        wh2st4se4 = cms.untracked.vint32(72, 0, 72, 18),
        wh2st4se7 = cms.untracked.vint32(96, 0, 96, 24),
        wh2st4se6 = cms.untracked.vint32(96, 0, 96, 24),
        wh0st4se3 = cms.untracked.vint32(96, 0, 96, 24),
        wh2st4se8 = cms.untracked.vint32(92, 0, 92, 23),
        wh0st4se1 = cms.untracked.vint32(96, 0, 96, 24),
        wh0st4se7 = cms.untracked.vint32(96, 0, 96, 24),
        wh0st4se6 = cms.untracked.vint32(96, 0, 96, 24),
        wh0st4se5 = cms.untracked.vint32(96, 0, 96, 24),
        wh0st4se4 = cms.untracked.vint32(72, 0, 72, 18),
        wh0st3se10 = cms.untracked.vint32(72, 58, 72, 18),
        wh0st3se11 = cms.untracked.vint32(72, 58, 72, 18),
        wh0st3se12 = cms.untracked.vint32(72, 58, 72, 18),
        wh2st4se11 = cms.untracked.vint32(48, 0, 48, 12),
        wh2st4se10 = cms.untracked.vint32(60, 0, 60, 15),
        wh2st4se13 = cms.untracked.vint32(72, 0, 72, 18),
        wh2st4se12 = cms.untracked.vint32(92, 0, 92, 23),
        wh2st4se14 = cms.untracked.vint32(60, 0, 60, 15),
        wh1st1se5 = cms.untracked.vint32(50, 58, 50, 13),
        whm1st4se1 = cms.untracked.vint32(96, 0, 96, 24),
        whm1st4se2 = cms.untracked.vint32(96, 0, 96, 24),
        whm1st4se3 = cms.untracked.vint32(96, 0, 96, 24),
        wh1st2se12 = cms.untracked.vint32(60, 58, 60, 15),
        whm1st4se5 = cms.untracked.vint32(96, 0, 96, 24),
        wh1st2se10 = cms.untracked.vint32(60, 58, 60, 15),
        whm1st4se7 = cms.untracked.vint32(96, 0, 96, 24),
        whm1st4se8 = cms.untracked.vint32(92, 0, 92, 23),
        whm1st4se9 = cms.untracked.vint32(48, 0, 48, 12),
        whm1st2se11 = cms.untracked.vint32(60, 58, 60, 15),
        wh0st3se6 = cms.untracked.vint32(72, 58, 72, 18),
        wh0st3se7 = cms.untracked.vint32(72, 58, 72, 18),
        wh0st3se4 = cms.untracked.vint32(72, 58, 72, 18),
        wh0st3se5 = cms.untracked.vint32(72, 58, 72, 18),
        wh0st3se2 = cms.untracked.vint32(72, 58, 72, 18),
        wh0st3se3 = cms.untracked.vint32(72, 58, 72, 18),
        wh0st3se1 = cms.untracked.vint32(72, 58, 72, 18),
        wh2st3se7 = cms.untracked.vint32(72, 58, 72, 18),
        whm2st1se5 = cms.untracked.vint32(50, 58, 50, 13),
        wh0st3se8 = cms.untracked.vint32(72, 58, 72, 18),
        wh0st3se9 = cms.untracked.vint32(72, 58, 72, 18),
        wh2st1se6 = cms.untracked.vint32(50, 58, 50, 13),
        whm1st1se8 = cms.untracked.vint32(50, 58, 50, 13),
        wh0st1se9 = cms.untracked.vint32(50, 58, 50, 13),
        wh2st3se12 = cms.untracked.vint32(72, 58, 72, 18),
        wh2st3se10 = cms.untracked.vint32(72, 58, 72, 18),
        wh2st3se11 = cms.untracked.vint32(72, 58, 72, 18),
        wh1st1se11 = cms.untracked.vint32(50, 58, 50, 13),
        wh1st1se10 = cms.untracked.vint32(50, 58, 50, 13),
        wh1st1se12 = cms.untracked.vint32(50, 58, 50, 13),
        wh1st2se11 = cms.untracked.vint32(60, 58, 60, 15),
        whm2st2se5 = cms.untracked.vint32(60, 58, 60, 15),
        whm2st2se4 = cms.untracked.vint32(60, 58, 60, 15),
        whm2st2se7 = cms.untracked.vint32(60, 58, 60, 15),
        whm2st2se6 = cms.untracked.vint32(60, 58, 60, 15),
        whm2st2se1 = cms.untracked.vint32(60, 58, 60, 15),
        whm2st2se3 = cms.untracked.vint32(60, 58, 60, 15),
        whm2st2se2 = cms.untracked.vint32(60, 58, 60, 15),
        whm2st2se9 = cms.untracked.vint32(60, 58, 60, 15),
        whm2st2se8 = cms.untracked.vint32(60, 58, 60, 15),
        whm1st4se6 = cms.untracked.vint32(96, 0, 96, 24),
        wh2st1se5 = cms.untracked.vint32(50, 58, 50, 13),
        whm1st2se6 = cms.untracked.vint32(60, 58, 60, 15),
        whm1st2se7 = cms.untracked.vint32(60, 58, 60, 15),
        whm1st2se4 = cms.untracked.vint32(60, 58, 60, 15),
        whm1st2se5 = cms.untracked.vint32(60, 58, 60, 15),
        whm1st2se2 = cms.untracked.vint32(60, 58, 60, 15),
        whm1st2se3 = cms.untracked.vint32(60, 48, 60, 15),
        whm1st2se1 = cms.untracked.vint32(60, 58, 60, 15),
        whm1st2se8 = cms.untracked.vint32(60, 58, 60, 15),
        whm1st2se9 = cms.untracked.vint32(60, 58, 60, 15),
        wh2st2se7 = cms.untracked.vint32(60, 58, 60, 15),
        wh2st2se6 = cms.untracked.vint32(60, 58, 60, 15),
        wh2st2se5 = cms.untracked.vint32(60, 58, 60, 15),
        wh2st2se4 = cms.untracked.vint32(60, 58, 60, 15),
        wh2st2se3 = cms.untracked.vint32(60, 58, 60, 15),
        wh2st2se2 = cms.untracked.vint32(60, 58, 60, 15),
        wh2st2se1 = cms.untracked.vint32(60, 58, 60, 15),
        wh0st1se7 = cms.untracked.vint32(50, 58, 50, 13),
        wh2st2se9 = cms.untracked.vint32(60, 58, 60, 15),
        wh2st2se8 = cms.untracked.vint32(60, 58, 60, 15),
        wh0st4se13 = cms.untracked.vint32(72, 0, 72, 18),
        wh0st1se6 = cms.untracked.vint32(50, 58, 50, 13),
        wh0st4se11 = cms.untracked.vint32(48, 0, 48, 12),
        wh0st4se10 = cms.untracked.vint32(60, 0, 60, 15),
        whm1st3se12 = cms.untracked.vint32(72, 58, 72, 18),
        wh0st4se14 = cms.untracked.vint32(60, 0, 60, 15),
        whm2st1se3 = cms.untracked.vint32(50, 58, 50, 13),
        wh0st4se9 = cms.untracked.vint32(48, 0, 48, 12),
        wh1st4se10 = cms.untracked.vint32(60, 0, 60, 15),
        wh1st4se11 = cms.untracked.vint32(48, 0, 48, 12),
        wh1st4se12 = cms.untracked.vint32(92, 0, 92, 23),
        wh1st4se13 = cms.untracked.vint32(72, 0, 72, 18),
        wh1st4se14 = cms.untracked.vint32(60, 0, 60, 15),
        wh1st1se4 = cms.untracked.vint32(50, 48, 50, 13),
        wh1st1se7 = cms.untracked.vint32(50, 58, 50, 13),
        wh1st1se6 = cms.untracked.vint32(50, 58, 50, 13),
        wh1st1se9 = cms.untracked.vint32(50, 58, 50, 13),
        whm2st3se10 = cms.untracked.vint32(72, 58, 72, 18),
        wh1st2se3 = cms.untracked.vint32(60, 58, 60, 15),
        whm2st3se11 = cms.untracked.vint32(72, 58, 72, 18),
        whm2st3se12 = cms.untracked.vint32(72, 58, 72, 18),
        wh0st4se12 = cms.untracked.vint32(92, 0, 92, 23),
        wh1st3se9 = cms.untracked.vint32(72, 58, 72, 18),
        wh2st3se8 = cms.untracked.vint32(72, 58, 72, 18),
        wh2st4se9 = cms.untracked.vint32(48, 0, 48, 12),
        wh1st3se8 = cms.untracked.vint32(72, 58, 72, 18),
        wh0st4se2 = cms.untracked.vint32(96, 0, 96, 24),
        wh2st3se1 = cms.untracked.vint32(72, 58, 72, 18),
        wh2st3se2 = cms.untracked.vint32(72, 58, 72, 18),
        wh2st3se3 = cms.untracked.vint32(72, 58, 72, 18),
        wh2st3se4 = cms.untracked.vint32(72, 58, 72, 18),
        wh2st3se5 = cms.untracked.vint32(72, 58, 72, 18),
        wh2st3se6 = cms.untracked.vint32(72, 58, 72, 18),
        wh1st4se3 = cms.untracked.vint32(96, 0, 96, 24),
        whm2st4se1 = cms.untracked.vint32(96, 0, 96, 24),
        wh1st4se1 = cms.untracked.vint32(96, 0, 96, 24),
        whm1st2se10 = cms.untracked.vint32(60, 58, 60, 15),
        whm2st2se11 = cms.untracked.vint32(60, 58, 60, 15),
        whm2st2se10 = cms.untracked.vint32(60, 58, 60, 15),
        whm2st2se12 = cms.untracked.vint32(60, 58, 60, 15),
        wh0st2se5 = cms.untracked.vint32(60, 58, 60, 15),
        whm1st3se9 = cms.untracked.vint32(72, 58, 72, 18),
        whm1st3se8 = cms.untracked.vint32(72, 58, 72, 18)
    )
)

OutOfTime = cms.PSet(
    TOBlateFP = cms.double(0),
    TIDlateFP = cms.double(0),
    TOBlateBP = cms.double(0),
    TEClateBP = cms.double(0),
    TIBlateFP = cms.double(0),
    TIBlateBP = cms.double(0),
    TIDlateBP = cms.double(0),
    TEClateFP = cms.double(0)
)

CondDBSetup = cms.PSet(
    DBParameters = cms.PSet(
        authenticationPath = cms.untracked.string('.'),
        enableReadOnlySessionOnUpdateConnection = cms.untracked.bool(False),
        idleConnectionCleanupPeriod = cms.untracked.int32(10),
        messageLevel = cms.untracked.int32(0),
        enablePoolAutomaticCleanUp = cms.untracked.bool(False),
        enableConnectionSharing = cms.untracked.bool(True),
        connectionRetrialTimeOut = cms.untracked.int32(60),
        connectionTimeOut = cms.untracked.int32(60),
        connectionRetrialPeriod = cms.untracked.int32(10)
    )
)

fieldScaling = cms.PSet(
    scalingVolumes = cms.vint32(14100, 14200, 17600, 17800, 17900, 
        18100, 18300, 18400, 18600, 23100, 
        23300, 23400, 23600, 23800, 23900, 
        24100, 28600, 28800, 28900, 29100, 
        29300, 29400, 29600, 28609, 28809, 
        28909, 29109, 29309, 29409, 29609, 
        28610, 28810, 28910, 29110, 29310, 
        29410, 29610, 28611, 28811, 28911, 
        29111, 29311, 29411, 29611),
    scalingFactors = cms.vdouble(1, 1, 0.994, 1.004, 1.004, 
        1.005, 1.004, 1.004, 0.994, 0.965, 
        0.958, 0.958, 0.953, 0.958, 0.958, 
        0.965, 0.918, 0.924, 0.924, 0.906, 
        0.924, 0.924, 0.918, 0.991, 0.998, 
        0.998, 0.978, 0.998, 0.998, 0.991, 
        0.991, 0.998, 0.998, 0.978, 0.998, 
        0.998, 0.991, 0.991, 0.998, 0.998, 
        0.978, 0.998, 0.998, 0.991)
)

CSCCommonTrigger = cms.PSet(
    MaxBX = cms.int32(9),
    MinBX = cms.int32(3)
)

TSPhiParametersBlock = cms.PSet(
    TSPhiParameters = cms.PSet(
        TSMNOE1 = cms.bool(True),
        TSMNOE2 = cms.bool(False),
        TSSMSK1 = cms.int32(312),
        TSTREN9 = cms.bool(True),
        TSTREN8 = cms.bool(True),
        TSTREN11 = cms.bool(True),
        TSTREN3 = cms.bool(True),
        TSTREN2 = cms.bool(True),
        TSTREN1 = cms.bool(True),
        TSTREN0 = cms.bool(True),
        TSTREN7 = cms.bool(True),
        TSTREN6 = cms.bool(True),
        TSTREN5 = cms.bool(True),
        TSTREN4 = cms.bool(True),
        TSSCCE1 = cms.bool(True),
        TSSCCE2 = cms.bool(False),
        TSMCCE2 = cms.bool(False),
        TSTREN19 = cms.bool(True),
        TSMCCE1 = cms.bool(True),
        TSTREN17 = cms.bool(True),
        TSTREN16 = cms.bool(True),
        TSTREN15 = cms.bool(True),
        TSTREN14 = cms.bool(True),
        TSTREN13 = cms.bool(True),
        TSTREN12 = cms.bool(True),
        TSSMSK2 = cms.int32(312),
        TSTREN10 = cms.bool(True),
        TSMMSK2 = cms.int32(312),
        TSMMSK1 = cms.int32(312),
        TSMHSP = cms.int32(1),
        TSSNOE2 = cms.bool(False),
        TSSNOE1 = cms.bool(True),
        TSSCGS2 = cms.bool(True),
        TSSCCEC = cms.bool(False),
        TSMCCEC = cms.bool(False),
        TSMHTE2 = cms.bool(False),
        Debug = cms.untracked.bool(False),
        TSSHTE2 = cms.bool(False),
        TSMCGS1 = cms.bool(True),
        TSMCGS2 = cms.bool(True),
        TSSHTE1 = cms.bool(True),
        TSTREN22 = cms.bool(True),
        TSSNOEC = cms.bool(False),
        TSTREN20 = cms.bool(True),
        TSTREN21 = cms.bool(True),
        TSMGS1 = cms.int32(1),
        TSMGS2 = cms.int32(1),
        TSSHTEC = cms.bool(False),
        TSMWORD = cms.int32(255),
        TSMHTEC = cms.bool(False),
        TSSCGS1 = cms.bool(True),
        TSTREN23 = cms.bool(True),
        TSSGS2 = cms.int32(1),
        TSMNOEC = cms.bool(False),
        TSSGS1 = cms.int32(1),
        TSTREN18 = cms.bool(True),
        TSMHTE1 = cms.bool(True)
    )
)

#maxEvents = cms.untracked.PSet(
#    input = cms.untracked.int32(100),
#    skipBadFiles = cms.bool(True)
#)

DTTPGParametersBlock = cms.PSet(
    DTTPGParameters = cms.PSet(
        SectCollParameters = cms.PSet(
            SCCSP5 = cms.int32(0),
            SCCSP2 = cms.int32(0),
            SCCSP3 = cms.int32(0),
            SCECF4 = cms.bool(False),
            SCCSP1 = cms.int32(0),
            SCECF2 = cms.bool(False),
            SCECF3 = cms.bool(False),
            SCCSP4 = cms.int32(0),
            SCECF1 = cms.bool(False),
            Debug = cms.untracked.bool(False)
        ),
        Debug = cms.untracked.bool(False),
        TUParameters = cms.PSet(
            TracoParameters = cms.PSet(
                SPRGCOMP = cms.int32(2),
                FHTMSK = cms.int32(0),
                DD = cms.int32(18),
                SSLMSK = cms.int32(0),
                LVALIDIFH = cms.int32(0),
                Debug = cms.untracked.int32(0),
                FSLMSK = cms.int32(0),
                SHTPRF = cms.int32(1),
                SHTMSK = cms.int32(0),
                TRGENB3 = cms.int32(1),
                SHISM = cms.int32(0),
                IBTIOFF = cms.int32(0),
                KPRGCOM = cms.int32(255),
                KRAD = cms.int32(0),
                FLTMSK = cms.int32(0),
                LTS = cms.int32(0),
                SLTMSK = cms.int32(0),
                FPRGCOMP = cms.int32(2),
                TRGENB9 = cms.int32(1),
                TRGENB8 = cms.int32(1),
                FHTPRF = cms.int32(1),
                LTF = cms.int32(0),
                TRGENB1 = cms.int32(1),
                TRGENB0 = cms.int32(1),
                FHISM = cms.int32(0),
                TRGENB2 = cms.int32(1),
                TRGENB5 = cms.int32(1),
                TRGENB4 = cms.int32(1),
                TRGENB7 = cms.int32(1),
                TRGENB6 = cms.int32(1),
                TRGENB15 = cms.int32(1),
                TRGENB14 = cms.int32(1),
                TRGENB11 = cms.int32(1),
                TRGENB10 = cms.int32(1),
                TRGENB13 = cms.int32(1),
                TRGENB12 = cms.int32(1),
                REUSEO = cms.int32(1),
                REUSEI = cms.int32(1),
                BTIC = cms.int32(32)
            ),
            TSPhiParameters = cms.PSet(
                TSMNOE1 = cms.bool(True),
                TSMNOE2 = cms.bool(False),
                TSSMSK1 = cms.int32(312),
                TSTREN9 = cms.bool(True),
                TSTREN8 = cms.bool(True),
                TSTREN11 = cms.bool(True),
                TSTREN3 = cms.bool(True),
                TSTREN2 = cms.bool(True),
                TSTREN1 = cms.bool(True),
                TSTREN0 = cms.bool(True),
                TSTREN7 = cms.bool(True),
                TSTREN6 = cms.bool(True),
                TSTREN5 = cms.bool(True),
                TSTREN4 = cms.bool(True),
                TSSCCE1 = cms.bool(True),
                TSSCCE2 = cms.bool(False),
                TSMCCE2 = cms.bool(False),
                TSTREN19 = cms.bool(True),
                TSMCCE1 = cms.bool(True),
                TSTREN17 = cms.bool(True),
                TSTREN16 = cms.bool(True),
                TSTREN15 = cms.bool(True),
                TSTREN14 = cms.bool(True),
                TSTREN13 = cms.bool(True),
                TSTREN12 = cms.bool(True),
                TSSMSK2 = cms.int32(312),
                TSTREN10 = cms.bool(True),
                TSMMSK2 = cms.int32(312),
                TSMMSK1 = cms.int32(312),
                TSMHSP = cms.int32(1),
                TSSNOE2 = cms.bool(False),
                TSSNOE1 = cms.bool(True),
                TSSCGS2 = cms.bool(True),
                TSSCCEC = cms.bool(False),
                TSMCCEC = cms.bool(False),
                TSMHTE2 = cms.bool(False),
                Debug = cms.untracked.bool(False),
                TSSHTE2 = cms.bool(False),
                TSMCGS1 = cms.bool(True),
                TSMCGS2 = cms.bool(True),
                TSSHTE1 = cms.bool(True),
                TSTREN22 = cms.bool(True),
                TSSNOEC = cms.bool(False),
                TSTREN20 = cms.bool(True),
                TSTREN21 = cms.bool(True),
                TSMGS1 = cms.int32(1),
                TSMGS2 = cms.int32(1),
                TSSHTEC = cms.bool(False),
                TSMWORD = cms.int32(255),
                TSMHTEC = cms.bool(False),
                TSSCGS1 = cms.bool(True),
                TSTREN23 = cms.bool(True),
                TSSGS2 = cms.int32(1),
                TSMNOEC = cms.bool(False),
                TSSGS1 = cms.int32(1),
                TSTREN18 = cms.bool(True),
                TSMHTE1 = cms.bool(True)
            ),
            TSThetaParameters = cms.PSet(
                Debug = cms.untracked.bool(False)
            ),
            Debug = cms.untracked.bool(False),
            DIGIOFFSET = cms.int32(500),
            SINCROTIME = cms.int32(0),
            BtiParameters = cms.PSet(
                KACCTHETA = cms.int32(1),
                WEN8 = cms.int32(1),
                ACH = cms.int32(1),
                DEAD = cms.int32(31),
                ACL = cms.int32(2),
                PTMS20 = cms.int32(1),
                Debug = cms.untracked.int32(0),
                PTMS22 = cms.int32(1),
                PTMS23 = cms.int32(1),
                PTMS24 = cms.int32(1),
                PTMS25 = cms.int32(1),
                PTMS26 = cms.int32(1),
                PTMS27 = cms.int32(1),
                PTMS28 = cms.int32(1),
                PTMS29 = cms.int32(1),
                SET = cms.int32(7),
                RON = cms.bool(True),
                WEN2 = cms.int32(1),
                LL = cms.int32(2),
                LH = cms.int32(21),
                WEN3 = cms.int32(1),
                RE43 = cms.int32(2),
                WEN0 = cms.int32(1),
                RL = cms.int32(42),
                WEN1 = cms.int32(1),
                RH = cms.int32(61),
                LTS = cms.int32(3),
                CH = cms.int32(41),
                CL = cms.int32(22),
                PTMS15 = cms.int32(1),
                PTMS14 = cms.int32(1),
                PTMS17 = cms.int32(1),
                PTMS16 = cms.int32(1),
                PTMS11 = cms.int32(1),
                PTMS10 = cms.int32(1),
                PTMS13 = cms.int32(1),
                PTMS12 = cms.int32(1),
                XON = cms.bool(False),
                WEN7 = cms.int32(1),
                WEN4 = cms.int32(1),
                WEN5 = cms.int32(1),
                PTMS19 = cms.int32(1),
                PTMS18 = cms.int32(1),
                PTMS31 = cms.int32(0),
                PTMS30 = cms.int32(0),
                PTMS5 = cms.int32(1),
                PTMS4 = cms.int32(1),
                PTMS7 = cms.int32(1),
                PTMS6 = cms.int32(1),
                PTMS1 = cms.int32(0),
                PTMS0 = cms.int32(0),
                PTMS3 = cms.int32(0),
                WEN6 = cms.int32(1),
                PTMS2 = cms.int32(0),
                PTMS9 = cms.int32(1),
                PTMS8 = cms.int32(1),
                ST43 = cms.int32(42),
                AC2 = cms.int32(3),
                AC1 = cms.int32(0),
                KMAX = cms.int32(64),
                PTMS21 = cms.int32(1)
            )
        )
    )
)

TSThetaParametersBlock = cms.PSet(
    TSThetaParameters = cms.PSet(
        Debug = cms.untracked.bool(False)
    )
)

TUParamsBlock = cms.PSet(
    Debug = cms.untracked.bool(False),
    DIGIOFFSET = cms.int32(500),
    SINCROTIME = cms.int32(0)
)

options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(False)
)

HLTConfigVersion = cms.PSet(
    tableName = cms.string('/dev/CMSSW_3_4_0/HLT/V5')
)

SectCollParametersBlock = cms.PSet(
    SectCollParameters = cms.PSet(
        SCCSP5 = cms.int32(0),
        SCCSP2 = cms.int32(0),
        SCCSP3 = cms.int32(0),
        SCECF4 = cms.bool(False),
        SCCSP1 = cms.int32(0),
        SCECF2 = cms.bool(False),
        SCECF3 = cms.bool(False),
        SCCSP4 = cms.int32(0),
        SCECF1 = cms.bool(False),
        Debug = cms.untracked.bool(False)
    )
)

schedule = cms.Schedule(DoHLTJetsU,DoHltMuon,DoHLTPhoton,DoHLTElectronStartUpWindows,DoHLTElectronLargeWindows,DoHLTElectronSiStrip,DoHLTTau,DoHLTBTag,DoHLTAlCaECALPhiSym,DoHLTAlCaPi0Eta8E29,DoHLTMinBiasPixelTracks)

# Beginning HLTAnalyzer Analysis 

# Setting HltTree weight to 1 = 1*1 (cross section * gen filter efficiency)

