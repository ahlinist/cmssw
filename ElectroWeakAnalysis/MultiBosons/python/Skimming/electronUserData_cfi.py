import FWCore.ParameterSet.Config as cms

electronUserData = cms.EDFilter("ElectronUserDataProducer",
  src = cms.InputTag("gsfElectrons"),
#   ebRechits = cms.InputTag("ecalRecHit","EcalRecHitsEB"),
#   eeRechits = cms.InputTag("ecalRecHit","EcalRecHitsEE"),
  ebRechits = cms.InputTag("reducedEcalRecHitsEB"),
  eeRechits = cms.InputTag("reducedEcalRecHitsEE"),
)
