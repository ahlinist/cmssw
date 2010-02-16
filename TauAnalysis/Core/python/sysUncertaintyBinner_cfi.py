import FWCore.ParameterSet.Config as cms
import copy

sysUncertaintyBinner = cms.PSet(
    pluginName = cms.string('sysUncertaintyBinner'),
    pluginType = cms.string('SysUncertaintyBinner'),
    binnerPlugins = cms.VPSet(),
    systematics = cms.vstring(),
    dqmDirectory_store = cms.string('sysUncertaintyBinningResults')
)
