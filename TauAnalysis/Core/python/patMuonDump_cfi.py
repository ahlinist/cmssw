import FWCore.ParameterSet.Config as cms

patMuonDump = cms.PSet(
    pluginName = cms.string('patMuonDump'),
    pluginType = cms.string('PATMuonDump'),

    muonSource = cms.InputTag('cleanPatTaus'),
    genParticleSource = cms.InputTag('genParticles'),

    skipPdgIdsGenParticleMatch = cms.vint32(12, 14, 16)
)

