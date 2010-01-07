import FWCore.ParameterSet.Config as cms
import copy

sysUncertaintyBinner = cms.PSet(
    pluginName = cms.string('sysUncertaintyBinner'),
    pluginType = cms.string('SysUncertaintyBinner'),
    binning = cms.PSet(
        name = cms.string("sysUncertaintyBinning"),
        config = cms.VPSet(
            cms.PSet(
                extractor = cms.PSet(
                    pluginType = cms.string("ConstObjValExtractor"),
                    value = cms.double(0)
                ),
                binning = cms.PSet(
                    min = cms.double(-0.5),
                    max = cms.double(+0.5)
                )
            )
        ),
        systematics = cms.vstring()
    ),
    binningService = cms.PSet(
        pluginType = cms.string("SysUncertaintyBinningService")
    ),
    dqmDirectory_store = cms.string('sysUncertaintyBinningResults')
)
