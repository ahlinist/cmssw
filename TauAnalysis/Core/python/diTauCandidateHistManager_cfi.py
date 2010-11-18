import FWCore.ParameterSet.Config as cms

diTauCandidateHistManager = cms.PSet(    
    pluginName = cms.string('diTauCandidateHistManager'),
    pluginType = cms.string('DiCandidatePairHistManager'),
      
    diTauCandidateSource = cms.InputTag(''),

    vertexSource = cms.InputTag('selectedPrimaryVertexHighestPtTrackSum'),
    genParticleSource = cms.InputTag('genParticles'),
    pfCandidateSource = cms.InputTag('particleFlow'),

    dqmDirectory_store = cms.string('DiTauCandidateQuantities'),

    makeMEtProjResolutionHistograms = cms.bool(True),
    #makeMEtProjResolutionHistograms = cms.bool(False),

    #requireGenMatch = cms.bool(True),
    requireGenMatch = cms.bool(False),

    #normalization = cms.string("diTauCandidates"),
    normalization = cms.string("events")
)
