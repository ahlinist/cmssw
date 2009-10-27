import FWCore.ParameterSet.Config as cms

eventWeightHistManager = cms.PSet(
    pluginName = cms.string('eventWeightHistManager'),
    pluginType = cms.string('EventWeightHistManager'),

    dqmDirectory_store = cms.string('EventWeights')
)
