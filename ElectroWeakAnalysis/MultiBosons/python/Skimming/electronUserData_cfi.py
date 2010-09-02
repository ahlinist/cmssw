import FWCore.ParameterSet.Config as cms

electronUserData = cms.EDProducer("ElectronUserDataProducer",
  src = cms.InputTag("gsfElectrons"),
#   ebRecHits = cms.InputTag("ecalRecHit","EcalRecHitsEB"),
#   eeRecHits = cms.InputTag("ecalRecHit","EcalRecHitsEE"),
  ebRecHits = cms.InputTag("reducedEcalRecHitsEB"),
  eeRecHits = cms.InputTag("reducedEcalRecHitsEE"),
)
