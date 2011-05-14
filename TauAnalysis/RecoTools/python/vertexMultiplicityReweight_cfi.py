import FWCore.ParameterSet.Config as cms

selectedPrimaryVerticesTrackPtSumGt10 = cms.EDFilter("VertexByTrackPtSumSelector",
    src = cms.InputTag('selectedPrimaryVertexPosition'),
    trackPtSumMin = cms.double(10.),
    filter = cms.bool(False)                                                 
)    

vertexMultiplicityReweight = cms.EDProducer("VertexMultiplicityReweightProducer",
    # enable the next 4 lines in order to reweight MC events from Spring'11 production to 2011 Data,
    # based on multiplicity of reconstructed vertices
    #inputFileName = cms.FileInPath("TauAnalysis/RecoTools/data/vertexMultiplicityReweight.root"),
    #lutName = cms.string('histoReweight'),
    #src = cms.InputTag('selectedPrimaryVerticesTrackPtSumGt10'),
    #type = cms.string("rec")
    # enable the next 4 lines in order to reweight MC events from Spring'11 production to 2011 Data,
    # based on number of pile-up interactions (min. bias events) added on generator level
    inputFileName = cms.FileInPath("TauAnalysis/RecoTools/data/genPileUpReweight.root"),
    lutName = cms.string('pileup'),
    src = cms.InputTag('addPileupInfo'),
    type = cms.string("gen")                            
)

