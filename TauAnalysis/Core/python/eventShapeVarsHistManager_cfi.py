import FWCore.ParameterSet.Config as cms

eventShapeVarsHistManager = cms.PSet(
    pluginName = cms.string('eventShapeVarsHistManager'),
    pluginType = cms.string('EventShapeVarsHistManager'),

    src = cms.InputTag('caloEventShapeVars'),

    dqmDirectory_store = cms.string('EventShapeVars')
)
