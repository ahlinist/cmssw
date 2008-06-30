import FWCore.ParameterSet.Config as cms

egammaElectronTkIsolation = cms.EDProducer("EgammaElectronTkIsolationProducer",
    absolut = cms.bool(True),
    trackProducer = cms.InputTag("generalTracks"),
    intRadius = cms.double(0.02),
    electronProducer = cms.InputTag("pixelMatchGsfElectrons"),
    extRadius = cms.double(0.2),
    ptMin = cms.double(1.5),
    maxVtxDist = cms.double(0.1)
)


