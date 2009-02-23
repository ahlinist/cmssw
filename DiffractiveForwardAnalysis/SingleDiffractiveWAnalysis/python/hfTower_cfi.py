import FWCore.ParameterSet.Config as cms

hfTower = cms.EDProducer("HFTowerEdmNtupleDumperAOD",
    CaloTowersTag = cms.InputTag("towerMaker"),
    TowerEnergyTresholdMin = cms.untracked.double(0.0),
    TowerEnergyTresholdMax = cms.untracked.double(10.0),
    NumberOfTresholds = cms.untracked.uint32(50)
)
