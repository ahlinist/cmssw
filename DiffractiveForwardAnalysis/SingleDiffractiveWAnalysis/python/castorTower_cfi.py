import FWCore.ParameterSet.Config as cms

castorTower = cms.EDProducer("CastorTowerEdmNtupleDumper",
    CastorTowersTag = cms.InputTag("CastorTowerReco"),
    TowerEnergyTresholdMin = cms.untracked.double(0.0),
    TowerEnergyTresholdMax = cms.untracked.double(20.0),
    NumberOfTresholds = cms.untracked.uint32(50)
)
