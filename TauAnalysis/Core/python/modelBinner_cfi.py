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

modelBinner3mZbins = cms.PSet(
    pluginName = cms.string('modelBinner3mZbins'),
    pluginType = cms.string('ModelBinner'),
    binning = cms.PSet(
        name = cms.string("modelBinning3mZbins"),
        config = cms.VPSet(
            cms.PSet(
                extractor = cms.PSet(
                    pluginType = cms.string("PATMuTauPairValExtractor"),
                    src = cms.InputTag(''),
                    value = cms.string("svFitSolution('psKine_MEt_ptBalance').mass()")
                ),
                binning = cms.PSet(
                    boundaries = cms.vdouble(20., 60., 120.),
                    min = cms.double(-1.01),
                    max = cms.double(1000.)
                )
            )
        )
    ),
    genBinning = cms.PSet(
        config = cms.VPSet(
            cms.PSet(
                extractor = cms.PSet(
                    pluginType = cms.string("CandidateValExtractor"),
                    src = cms.InputTag('genZdecayToTaus'),
                    value = cms.string("mass()")
                )
            )
        )
    ),
    srcGenFlag = cms.InputTag(""),
    srcRecFlag = cms.InputTag(""),
    binningService = cms.PSet(
        pluginType = cms.string("ModelBinningService")
    ),
    dqmDirectory_store = cms.string('modelBinningResults3mZbins')
)
