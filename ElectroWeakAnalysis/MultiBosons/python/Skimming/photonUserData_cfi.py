import FWCore.ParameterSet.Config as cms

photonUserData = cms.EDProducer("PhotonUserDataProducer",
  src = cms.InputTag("photons"),
#   ebRechits = cms.InputTag("ecalRecHit","EcalRecHitsEB"),
#   eeRechits = cms.InputTag("ecalRecHit","EcalRecHitsEE"),
  ebRechits = cms.InputTag("reducedEcalRecHitsEB"),
  eeRechits = cms.InputTag("reducedEcalRecHitsEE"),
)
