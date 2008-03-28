import FWCore.ParameterSet.Config as cms

egammaHOE = cms.EDProducer("EgammaHcalIsolationProducer",
    absolut = cms.bool(False),
    intRadius = cms.double(0.0),
    hcalRecHitProducer = cms.InputTag("hbhereco"),
    extRadius = cms.double(0.15),
    etMin = cms.double(0.0),
    emObjectProducer = cms.InputTag("pixelMatchGsfElectrons")
)


