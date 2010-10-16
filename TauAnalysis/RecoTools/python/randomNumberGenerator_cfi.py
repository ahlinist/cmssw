import FWCore.ParameterSet.Config as cms

RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
    saveFileName = cms.untracked.string('')
)

randomEngineStateProducer = cms.EDProducer("RandomEngineStateProducer")
