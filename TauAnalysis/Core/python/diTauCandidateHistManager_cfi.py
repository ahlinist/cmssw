import FWCore.ParameterSet.Config as cms

diTauCandidateHistManager = cms.PSet(    
    pluginName = cms.string('diTauCandidateHistManager'),
    pluginType = cms.string('DiCandidatePairHistManager'),
      
    diTauCandidateSource = cms.InputTag(''),

    vertexSource = cms.InputTag('selectedPrimaryVertexPosition'),
    genParticleSource = cms.InputTag('genParticles'),

    dqmDirectory_store = cms.string('DiTauCandidateQuantities'),

    fileName_sqrtX1X2VsDPhi12 = cms.string('/afs/cern.ch/user/v/veelken/public/TauAnalysis/sqrtX1X2_vs_DPhi12_ZtoMuTau.root'),
    meName_sqrtX1X2VsDPhi12 = cms.string('GenSqrtX1X2VsDPhi12'),

    #requireGenMatch = cms.bool(True),
    requireGenMatch = cms.bool(False),

    #normalization = cms.string("diTauCandidates"),
    normalization = cms.string("events")
)
