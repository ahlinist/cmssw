import FWCore.ParameterSet.Config as cms

recoZllRecoilCorrectionParameter = cms.PSet(
    parameter = cms.PSet(
        data = cms.PSet(
            d = cms.PSet(
                value = cms.double(0),
                uncertainty = cms.double(0)
            ),
            k = cms.PSet(
                value = cms.double(-0.869397),
                uncertainty = cms.double(0.00147881)
            ),
            sigma1 = cms.PSet(
                value = cms.double(8.7954),
                uncertainty = cms.double(0.0356848)
            ),
            b1 = cms.PSet(
                value = cms.double(0.00870004),
                uncertainty = cms.double(0.000376955)
            ),
            c1 = cms.PSet(
                value = cms.double(2.00108e-05),
                uncertainty = cms.double(4.42684e-06)
            ),
            sigma2 = cms.PSet(
                value = cms.double(8.81514),
                uncertainty = cms.double(0.0343841)
            ),
            b2 = cms.PSet(
                value = cms.double(0.00632672),
                uncertainty = cms.double(0.00033543)
            ),
            c2 = cms.PSet(
                value = cms.double(2.01774e-06),
                uncertainty = cms.double(3.76993e-06)
            )
        ),
        mc = cms.PSet(
            d = cms.PSet(
                value = cms.double(0),
                uncertainty = cms.double(0)
            ),
            k = cms.PSet(
                value = cms.double(-0.913219),
                uncertainty = cms.double(0.000259489)
            ),
            sigma1 = cms.PSet(
                value = cms.double(9.57377),
                uncertainty = cms.double(0.00743434)
            ),
            b1 = cms.PSet(
                value = cms.double(0.00373926),
                uncertainty = cms.double(6.39034e-05)
            ),
            c1 = cms.PSet(
                value = cms.double(1.95287e-05),
                uncertainty = cms.double(7.46333e-07)
            ),
            sigma2 = cms.PSet(
                value = cms.double(9.62499),
                uncertainty = cms.double(0.00705773)
            ),
            b2 = cms.PSet(
                value = cms.double(0.00163067),
                uncertainty = cms.double(5.40255e-05)
            ),
            c2 = cms.PSet(
                value = cms.double(-4.36799e-06),
                uncertainty = cms.double(5.81473e-07)
            )
        )
    ),
    shiftByUncertainty = cms.double(0.),
    srcGenParticles = cms.InputTag('genParticles'),
    genParticlePdgIds = cms.vint32( 36, 35, 25, 23, 24 ),
    srcGenMET = cms.InputTag('genMetTrue')
)
