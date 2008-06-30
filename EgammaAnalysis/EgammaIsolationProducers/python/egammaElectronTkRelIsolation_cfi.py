import FWCore.ParameterSet.Config as cms

egammaElectronTkRelIsolation = cms.EDProducer("EgammaElectronTkIsolationProducer",
    absolut = cms.bool(False),
    trackProducer = cms.InputTag("generalTracks"),
    intRadius = cms.double(0.02),
    electronProducer = cms.InputTag("pixelMatchGsfElectrons"),
    extRadius = cms.double(0.2),
    ptMin = cms.double(1.5),
    maxVtxDist = cms.double(0.1)
)


