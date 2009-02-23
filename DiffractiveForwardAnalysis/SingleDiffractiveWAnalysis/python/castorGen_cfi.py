import FWCore.ParameterSet.Config as cms

castorGen = cms.EDProducer("CASTORGenInfoDumper",
    GenParticlesLabel = cms.InputTag("genParticlesCalo"),
    CASTORParticleEnergyThreshold = cms.untracked.double(10.0),
    CASTORPhiSliceEnergyThreshold = cms.untracked.double(10.0),
    CASTOREtaMin = cms.untracked.double(5.2),
    CASTOREtaMax = cms.untracked.double(6.6)
)
