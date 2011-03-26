import FWCore.ParameterSet.Config as cms

diTauCandidateNSVfitHistManager = cms.PSet(
    pluginName = cms.string('diTauCandidateNSVfitHistManager'),
    pluginType = cms.string('DiCandidatePairNSVfitHistManager'),

    diTauCandidateSource = cms.InputTag(''),

    genParticleSource = cms.InputTag('genParticles'),

    nSVfitEventHypotheses = cms.PSet(),
   
    dqmDirectory_store = cms.string('DiTauCandidateNSVfitQuantities'),

    #requireGenMatch = cms.bool(True),
    requireGenMatch = cms.bool(False),

    #normalization = cms.string("diTauCandidates"),
    normalization = cms.string("events")
)
