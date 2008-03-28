import FWCore.ParameterSet.Config as cms

egammaTowerIsolation = cms.EDProducer("EgammaTowerIsolationProducer",
    absolut = cms.bool(True),
    intRadius = cms.double(0.15),
    extRadius = cms.double(0.3),
    towerProducer = cms.InputTag("towerMaker"),
    etMin = cms.double(0.0),
    emObjectProducer = cms.InputTag("pixelMatchGsfElectrons")
)


