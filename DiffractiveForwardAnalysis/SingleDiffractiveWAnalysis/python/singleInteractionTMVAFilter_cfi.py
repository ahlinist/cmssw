import FWCore.ParameterSet.Config as cms

singleInteractionTMVAFilter = cms.EDFilter("SingleInteractionTMVAFilter",
    VertexTag = cms.InputTag("offlinePrimaryVertices"),
    WeightsFile = cms.FileInPath("DiffractiveForwardAnalysis/SingleDiffractiveWAnalysis/data/MVAnalysis_Likelihood.weights.txt"),
    CutOnClassifier = cms.double(0.5)
)
