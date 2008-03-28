import FWCore.ParameterSet.Config as cms

egammaHcalIsolation = cms.EDProducer("EgammaHcalIsolationProducer",
    absolut = cms.bool(True),
    intRadius = cms.double(0.15),
    hcalRecHitProducer = cms.InputTag("hbhereco"),
    extRadius = cms.double(0.3),
    etMin = cms.double(0.0),
    emObjectProducer = cms.InputTag("pixelMatchGsfElectrons")
)


