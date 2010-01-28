import FWCore.ParameterSet.Config as cms
import copy

dataBinner = cms.PSet(
    pluginName = cms.string('dataBinner'),
    pluginType = cms.string('DataBinner'),
    binning = cms.PSet(
        name = cms.string("dataBinning"),
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
        )
    ),
    binningService = cms.PSet(
        pluginType = cms.string("DataBinningService")
    ),
    dqmDirectory_store = cms.string('dataBinningResults')
)
