import FWCore.ParameterSet.Config as cms
import copy

modelBinner = cms.PSet(
    pluginName = cms.string('modelBinner'),
    pluginType = cms.string('ModelBinner'),
    binning = cms.PSet(
        name = cms.string("modelBinning"),
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
    genBinning = cms.PSet(
        config = cms.VPSet(
            cms.PSet(
                extractor = cms.PSet(
                    pluginType = cms.string("ConstObjValExtractor"),
                    value = cms.double(0)
                )
            )
        )
    ),
    srcGenFlag = cms.InputTag(""),
    srcRecFlag = cms.InputTag(""),
    binningService = cms.PSet(
        pluginType = cms.string("ModelBinningService")
    ),
    dqmDirectory_store = cms.string('modelBinningResults')
)
