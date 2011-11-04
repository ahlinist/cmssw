import FWCore.ParameterSet.Config as cms

#------------------------------------------------------------------------------------------------------------------------
# CV: Z-recoil correction parameters valid for Type 1 corrected PFMEt in EPS dataset compared to Summer'11 MC
#
#     The MC has been reweighted by "3d" pile-up reweigthing procedure developed by Mike Hildreth,
#     using a pp inelastic cross-section of 73.5mb.
#
#     The Z-recoil correction parameters given for MC below are to be applied "on top" of PFJet MC smearing,
#     using the MC-to-Data correction factors given in JME-10-014, and after rhoNeutral reweigthing is applied
#    (uncertainty weighted average of di-jet assymmetry and photon+jet balance numbers,
#     as implemented in TauAnalysis/RecoTools/data/pfJetResolutionMCtoDataCorrLUT.root, version 1.1)
#------------------------------------------------------------------------------------------------------------------------

recoZllRecoilCorrectionParameter = cms.PSet(
    parameter = cms.PSet(
        data = cms.PSet(
            d = cms.PSet(
                value = cms.double(0),
                uncertainty = cms.double(0)
            ),
            k1 = cms.PSet(
                value = cms.double(-1.00578),
                uncertainty = cms.double(0.00109648)
            ),
            k2 = cms.PSet(
                value = cms.double(0.0912136),
                uncertainty = cms.double(0.0032137)
            ),
            k3 = cms.PSet(
                value = cms.double(0.735812),
                uncertainty = cms.double(0.0121634)
            ),
            sigma1 = cms.PSet(
                value = cms.double(9.4435),
                uncertainty = cms.double(0.0174418)
            ),
            b1 = cms.PSet(
                value = cms.double(0.00828456),
                uncertainty = cms.double(0.000139938)
            ),
            c1 = cms.PSet(
                value = cms.double(-1.30224e-05),
                uncertainty = cms.double(1.20458e-06)
            ),
            sigma2 = cms.PSet(
                value = cms.double(9.56251),
                uncertainty = cms.double(0.0161353)
            ),
            b2 = cms.PSet(
                value = cms.double(0.00449073),
                uncertainty = cms.double(0.00010625)
            ),
            c2 = cms.PSet(
                value = cms.double(-1.21508e-05),
                uncertainty = cms.double(7.69793e-07)
            )
        ),
        mc = cms.PSet(
            d = cms.PSet(
                value = cms.double(0),
                uncertainty = cms.double(0)
            ),
            k1 = cms.PSet(
                value = cms.double(-1.00366),
                uncertainty = cms.double(0.000252342)
            ),
            k2 = cms.PSet(
                value = cms.double(0.109886),
                uncertainty = cms.double(0.000994663)
            ),
            k3 = cms.PSet(
                value = cms.double(0.713593),
                uncertainty = cms.double(0.00322698)
            ),
            sigma1 = cms.PSet(
                value = cms.double(9.20828),
                uncertainty = cms.double(0.00440238)
            ),
            b1 = cms.PSet(
                value = cms.double(0.00610907),
                uncertainty = cms.double(3.47759e-05)
            ),
            c1 = cms.PSet(
                value = cms.double(-3.61939e-06),
                uncertainty = cms.double(3.08691e-07)
            ),
            sigma2 = cms.PSet(
                value = cms.double(9.31562),
                uncertainty = cms.double(0.00400581)
            ),
            b2 = cms.PSet(
                value = cms.double(0.00236688),
                uncertainty = cms.double(2.53168e-05)
            ),
            c2 = cms.PSet(
                value = cms.double(-6.51136e-06),
                uncertainty = cms.double(1.81095e-07)
            )
        )
    ),
    shiftByUncertainty = cms.double(0.),
    srcGenParticles = cms.InputTag('genParticles'),
    genParticlePdgIds = cms.vint32( 36, 35, 25, 23, 24 ),
    srcGenMET = cms.InputTag('genMetTrue')
)
