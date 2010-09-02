import FWCore.ParameterSet.Config as cms

photonUserData = cms.EDProducer("PhotonUserDataProducer",
  src = cms.InputTag("photons"),
#   ebRecHits = cms.InputTag("ecalRecHit","EcalRecHitsEB"),
#   eeRecHits = cms.InputTag("ecalRecHit","EcalRecHitsEE"),
  ebRecHits = cms.InputTag("reducedEcalRecHitsEB"),
  eeRecHits = cms.InputTag("reducedEcalRecHitsEE"),
)
