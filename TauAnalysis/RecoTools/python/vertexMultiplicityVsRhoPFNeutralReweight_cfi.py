import FWCore.ParameterSet.Config as cms

vertexMultiplicityVsRhoPFNeutralReweight = cms.EDProducer("VertexMultiplicityVsRhoPFNeutralReweightProducer",
    srcVertices = cms.InputTag('selectedPrimaryVertexPosition'),
    srcRhoPFNeutral = cms.InputTag('kt6PFJets', 'rho'),
                                                          
    inputFileName = cms.FileInPath('TauAnalysis/RecoTools/data/vertexMultiplicityVsRhoPFNeutralReweight.root'),
    lutName = cms.string('histoReweight'),
                                                          
    minPUreweight = cms.double(1.e-1),
    maxPUreweight = cms.double(1.e+1)
)
