import FWCore.ParameterSet.Config as cms

recoZllRecoilCorrectionParameter = cms.PSet(
    parameter = cms.PSet(
        data = cms.PSet(
            d = cms.double(0.),
            k = cms.double(-0.943492),
            sigma1 = cms.double(10.0368),
            b1 = cms.double(-1.6628e-2),
            c1 = cms.double(+1.12634e-4),
            sigma2 = cms.double(8.17098),
            b2 = cms.double(-1.08149e-2),
            c2 = cms.double(+3.32754e-05)
        ),
        mc = cms.PSet(
            d = cms.double(0.),
            k = cms.double(-0.947483),
            sigma1 = cms.double(9.32262),
            b1 = cms.double(-1.33763e-2),
            c1 = cms.double(+6.50963e-5),
            sigma2 = cms.double(7.44516),
            b2 = cms.double(-1.01039e-2),
            c2 = cms.double(+2.11601e-05)
        )
    ),
    uncertainties = cms.PSet(
        data = cms.PSet(
            d = cms.double(0.),
            k = cms.double(0.003905),
            sigma1 = cms.double(0.2019),
            b1 = cms.double(0.0419e-2),
            c1 = cms.double(0.04284e-4),
            sigma2 = cms.double(0.15132),
            b2 = cms.double(0.02622e-2),
            c2 = cms.double(0.19576e-5)
        ),
        mc = cms.PSet(
            d = cms.double(0.),
            k = cms.double(0.002881),
            sigma1 = cms.double(0.16254),
            b1 = cms.double(0.04032e-2),
            c1 = cms.double(0.40988e-5),
            sigma2 = cms.double(0.10048),
            b2 = cms.double(0.01746e-2),
            c2 = cms.double(0.13773e-5)
        )
    ),
    shiftByUncertainty = cms.double(0.),
    srcGenParticles = cms.InputTag('genParticles'),
    genParticlePdgIds = cms.vint32( 36, 35, 25, 23, 24 ),
    srcGenMET = cms.InputTag('genMetTrue')
)
