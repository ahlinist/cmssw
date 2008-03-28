import FWCore.ParameterSet.Config as cms

egammaEcalRelIsolation = cms.EDProducer("EgammaEcalIsolationProducer",
    absolut = cms.bool(False),
    basicClusterProducer = cms.InputTag("egammaBasicClusterMerger"),
    superClusterProducer = cms.InputTag("egammaSuperClusterMerger"),
    extRadius = cms.double(0.3),
    etMin = cms.double(0.0),
    emObjectProducer = cms.InputTag("pixelMatchGsfElectrons")
)


