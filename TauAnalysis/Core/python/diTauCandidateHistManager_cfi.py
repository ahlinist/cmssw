import FWCore.ParameterSet.Config as cms

diTauCandidateHistManager = cms.PSet(    
  pluginName = cms.string('diTauCandidateHistManager'),
  pluginType = cms.string('DiCandidatePairHistManager'),
      
  diTauCandidateSource = cms.InputTag(''),

  vertexSource = cms.InputTag('selectedPrimaryVertexPosition'),

  dqmDirectory_store = cms.string('DiTauCandidateQuantities'),

  #requireGenMatch = cms.bool(True),
  requireGenMatch = cms.bool(False),

  #normalization = cms.string("diTauCandidates"),
  normalization = cms.string("events"),

  # upper limit on acoplanarity angle between muon and tau-jet
  # for which muon + tau-jet mass is computed via collinear approximation
  # (value in units of degrees)
  maxDPhi12collinearApprox = cms.double(160.),

  makeCollinearApproxMassDepHistograms = cms.bool(True)
)
