import FWCore.ParameterSet.Config as cms

patElectronDump = cms.PSet(
    pluginName = cms.string('patElectronDump'),
    pluginType = cms.string('PATElectronDump'),

    muonSource = cms.InputTag('cleanLayer1Electrons'),
    genParticleSource = cms.InputTag('genParticles'),

    skipPdgIdsGenParticleMatch = cms.vint32(12, 14, 16)
)

