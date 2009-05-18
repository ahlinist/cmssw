import FWCore.ParameterSet.Config as cms

singleInteractionTMVAFilter = cms.EDFilter("SingleInteractionTMVAFilter",
    VertexTag = cms.InputTag("offlinePrimaryVertices"),
    TracksTag = cms.InputTag("generalTracks"),
    WeightsFile = cms.FileInPath("DiffractiveForwardAnalysis/SingleDiffractiveWAnalysis/data/MVAnalysis_MLP.weights.txt"),
    CutOnClassifier = cms.double(-0.23),
    ThresholdIndexHF = cms.uint32(10),
    InputVariables = cms.vstring("nVertex",
                                 "xiPlus",
                                 "nTracksAwayFromPV")
)
