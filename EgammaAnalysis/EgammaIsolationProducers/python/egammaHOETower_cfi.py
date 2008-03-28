import FWCore.ParameterSet.Config as cms

egammaHOETower = cms.EDProducer("EgammaTowerIsolationProducer",
    absolut = cms.bool(False),
    intRadius = cms.double(0.0),
    extRadius = cms.double(0.15),
    towerProducer = cms.InputTag("towerMaker"),
    etMin = cms.double(0.0),
    emObjectProducer = cms.InputTag("pixelMatchGsfElectrons")
)


