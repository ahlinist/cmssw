import FWCore.ParameterSet.Config as cms

hcalTPGValidation = cms.EDAnalyzer('HcalTPGValidation',
   hcalTag = cms.InputTag("simHcalTriggerPrimitiveDigis"),
   ecalTag = cms.InputTag("simEcalTriggerPrimitiveDigis"),
   genTag = cms.InputTag("genParticles"),
   jetPtThreshold = cms.untracked.double(20.)
)
