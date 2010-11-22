import FWCore.ParameterSet.Config as cms

selectedPrimaryVerticesTrackPtSumGt10 = cms.EDFilter("VertexByTrackPtSumSelector",
    src = cms.InputTag('selectedPrimaryVertexPosition'),
    trackPtSumMin = cms.double(10.),
    filter = cms.bool(False)                                                 
)    

vertexMultiplicityReweight = cms.EDProducer("VertexMultiplicityReweightProducer",
    inputFileName = cms.FileInPath("TauAnalysis/RecoTools/data/vertexMultiplicityReweight.root"),
    lutName = cms.string('histoReweight'),
    src = cms.InputTag('selectedPrimaryVerticesTrackPtSumGt10')
)

