import FWCore.ParameterSet.Config as cms

edmDumpAnalysis = cms.EDAnalyzer("EdmDumpAnalyzer",
    CastorGenInfoTag = cms.InputTag("castorGen"),
    CastorTowerInfoTag = cms.InputTag("castorTower"),
    VertexTag = cms.InputTag("offlinePrimaryVertices"),
    TracksTag = cms.InputTag("generalTracks"),
    GapSide = cms.int32(1),
    ThresholdIndexHF = cms.uint32(22),
    ThresholdIndexCastor = cms.uint32(25),
    # If accessing directly the castor towers
    AccessCastorTowers = cms.bool(False),
    CastorTowersTag = cms.InputTag("CastorTowerReco"),
    TowerThreshold = cms.double(15.0),
    # If accessing the pile-up info
    AccessPileUpInfo = cms.bool(True),
    # Saves or not ROOT TTree
    SaveROOTTree = cms.untracked.bool(False)
)
