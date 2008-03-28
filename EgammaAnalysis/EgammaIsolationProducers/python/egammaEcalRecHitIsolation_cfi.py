import FWCore.ParameterSet.Config as cms

egammaEcalRecHitIsolation = cms.EDProducer("EgammaEcalRecHitIsolationProducer",
    absolut = cms.bool(True),
    ecalBarrelRecHitProducer = cms.InputTag("ecalRecHit"),
    ecalEndcapRecHitCollection = cms.InputTag("EcalRecHitsEE"),
    intRadius = cms.double(0.0),
    ecalEndcapRecHitProducer = cms.InputTag("ecalRecHit"),
    extRadius = cms.double(0.4),
    useIsolEt = cms.bool(False),
    ecalBarrelRecHitCollection = cms.InputTag("EcalRecHitsEB"),
    etMin = cms.double(-999.0),
    emObjectProducer = cms.InputTag("pixelMatchGsfElectrons")
)


