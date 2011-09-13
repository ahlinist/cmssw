import FWCore.ParameterSet.Config as cms

recoZllRecoilCorrectionParameter = cms.PSet(
    parameter = cms.PSet(
        data = cms.PSet(
            d = cms.PSet(
                value = cms.double(0),
                uncertainty = cms.double(0)
            ),
            k = cms.PSet(
                value = cms.double(-0.874173),
                uncertainty = cms.double(0.00075441)
            ),
            sigma1 = cms.PSet(
                value = cms.double(9.1567),
                uncertainty = cms.double(0.0186172)
            ),
            b1 = cms.PSet(
                value = cms.double(0.00780813),
                uncertainty = cms.double(0.000187149)
            ),
            c1 = cms.PSet(
                value = cms.double(2.34183e-05),
                uncertainty = cms.double(2.21508e-06)
            ),
            sigma2 = cms.PSet(
                value = cms.double(9.18864),
                uncertainty = cms.double(0.0179193)
            ),
            b2 = cms.PSet(
                value = cms.double(0.00594046),
                uncertainty = cms.double(0.000165015)
            ),
            c2 = cms.PSet(
                value = cms.double(-3.73481e-06),
                uncertainty = cms.double(1.82988e-06)
            )
        ),
        mc = cms.PSet(
            d = cms.PSet(
                value = cms.double(0),
                uncertainty = cms.double(0)
            ),
            k = cms.PSet(
                value = cms.double(-0.91035),
                uncertainty = cms.double(0.000177905)
            ),
            sigma1 = cms.PSet(
                value = cms.double(8.90269),
                uncertainty = cms.double(0.00487887)
            ),
            b1 = cms.PSet(
                value = cms.double(0.00430534),
                uncertainty = cms.double(4.66588e-05)
            ),
            c1 = cms.PSet(
                value = cms.double(2.07653e-05),
                uncertainty = cms.double(5.55936e-07)
            ),
            sigma2 = cms.PSet(
                value = cms.double(8.95895),
                uncertainty = cms.double(0.00460745)
            ),
            b2 = cms.PSet(
                value = cms.double(0.00179307),
                uncertainty = cms.double(3.87975e-05)
            ),
            c2 = cms.PSet(
                value = cms.double(-4.80025e-06),
                uncertainty = cms.double(4.23705e-07)
            )
        )
    ),
    shiftByUncertainty = cms.double(0.),
    srcGenParticles = cms.InputTag('genParticles'),
    genParticlePdgIds = cms.vint32( 36, 35, 25, 23, 24 ),
    srcGenMET = cms.InputTag('genMetTrue')
)
