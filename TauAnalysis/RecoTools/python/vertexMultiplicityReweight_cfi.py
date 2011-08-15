import FWCore.ParameterSet.Config as cms

vertexMultiplicityReweight = cms.EDProducer("VertexMultiplicityReweightProducer",
    inputFileName = cms.FileInPath("TauAnalysis/RecoTools/data/genPileUpReweight.root"),
    lutName = cms.string('pileup'),
    src = cms.InputTag('addPileupInfo'),
    type = cms.string("gen")                            
)

