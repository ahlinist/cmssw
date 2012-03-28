import FWCore.ParameterSet.Config as cms

selectedPrimaryVerticesTrackPtSumGt10 = cms.EDFilter("VertexByTrackPtSumSelector",
    src = cms.InputTag('selectedPrimaryVertexPosition'),
    trackPtSumMin = cms.double(10.)
)

vertexMultiplicityReweight = cms.EDProducer("VertexMultiplicityReweightProducer",
    inputFileName = cms.FileInPath("TauAnalysis/RecoTools/data/expPUpoissonMean_runs160404to180252.root"),
    lutName = cms.string('pileup'),
    src = cms.InputTag('addPileupInfo'),
    type = cms.string("gen3d")
)

