import FWCore.ParameterSet.Config as cms

xiFromJets = cms.EDProducer("XiEdmNtupleDumper",
    comEnergy = cms.double(10000.),
    ParticlesTag = cms.InputTag("sisCone5CaloJets"),
    UseMuonInfo = cms.bool(True),
    MuonTag = cms.InputTag("muons"),
    UseMETInfo = cms.bool(True),
    CaloMETTag = cms.InputTag("met"),
    UseCaloTowers = cms.bool(False) 
)
