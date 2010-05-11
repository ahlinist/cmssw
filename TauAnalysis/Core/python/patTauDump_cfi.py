import FWCore.ParameterSet.Config as cms

patTauDump = cms.PSet(
    pluginName = cms.string('patTauDump'),
    pluginType = cms.string('PATTauDump'),

    tauSource = cms.InputTag('cleanPatTaus'),
    genParticleSource = cms.InputTag('genParticles'),

    skipPdgIdsGenParticleMatch = cms.vint32(12, 14, 16)
)
