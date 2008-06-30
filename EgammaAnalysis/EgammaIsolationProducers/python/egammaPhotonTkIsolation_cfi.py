import FWCore.ParameterSet.Config as cms

egammaPhotonTkIsolation = cms.EDProducer("EgammaPhotonTkIsolationProducer",
    absolut = cms.bool(True),
    trackProducer = cms.InputTag("generalTracks"),
    intRadius = cms.double(0.0),
    extRadius = cms.double(0.3),
    ptMin = cms.double(1.5),
    maxVtxDist = cms.double(1.0),
    photonProducer = cms.InputTag("photons")
)


