import FWCore.ParameterSet.Config as cms

hcalTPGValidation = cms.EDAnalyzer('HcalTPGValidation',
      DQMFolder = cms.string('L1Trigger/HcalTPGValidation'),
      HcalTPLabel = cms.InputTag('simHcalTriggerPrimitiveDigis'),
      SimHitsLabel = cms.InputTag('g4SimHits:HcalHits'),
      SamplingFactorHB = cms.double(110),
      SamplingFactorHE = cms.double(160),
      SamplingFactorHF1 = cms.double(1.0 / 0.383),
      SamplingFactorHF2 = cms.double(1.0 / 0.368),
      EtThreshold = cms.double(2.0)
)
