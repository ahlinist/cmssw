import FWCore.ParameterSet.Config as cms

Jul022010_passthru_selection = cms.PSet(version = cms.string("Jul022010"),
                                     BarrelJurrasicECALIsoConst = cms.double(100.0),
                                     BarrelJurrasicECALIsoSlope = cms.double(0.004),
                                     BarrelTowerHCALIsoConst = cms.double(100.0),
                                     BarrelTowerHCALIsoSlope = cms.double(0.001),
                                     BarrelMaxHadronicOverEm = cms.double(0.5),
                                     BarrelHollowConeTrkIsoConst = cms.double(100.0),
                                     BarrelHollowConeTrkIsoSlope = cms.double(0.001),
                                     EndcapJurrasicECALIsoConst = cms.double(100.0),
                                     EndcapJurrasicECALIsoSlope = cms.double(0.004),
                                     EndcapTowerHCALIsoConst = cms.double(100.0),
                                     EndcapTowerHCALIsoSlope = cms.double(0.001),
                                     EndcapMaxHadronicOverEm = cms.double(0.5),
                                     EndcapHollowConeTrkIsoConst = cms.double(100.0),
                                     EndcapHollowConeTrkIsoSlope = cms.double(0.001),
                                     minPt = cms.double(10),
                                     maxEta = cms.double(2.5),
                                     BarrelMaxSigmaIetaIeta = cms.double(0.012), #ignored!
                                     EndcapMaxSigmaIetaIeta = cms.double(0.1)  #ignored!
                                     )

Jul022010_loose_selection = cms.PSet(version = cms.string("Jul022010"),
                                     BarrelJurrasicECALIsoConst = cms.double(4.2),
                                     BarrelJurrasicECALIsoSlope = cms.double(0.004),
                                     BarrelTowerHCALIsoConst = cms.double(2.2),
                                     BarrelTowerHCALIsoSlope = cms.double(0.001),
                                     BarrelMaxHadronicOverEm = cms.double(0.05),
                                     BarrelHollowConeTrkIsoConst = cms.double(2.0),
                                     BarrelHollowConeTrkIsoSlope = cms.double(0.001),
                                     EndcapJurrasicECALIsoConst = cms.double(4.2),
                                     EndcapJurrasicECALIsoSlope = cms.double(0.004),
                                     EndcapTowerHCALIsoConst = cms.double(2.2),
                                     EndcapTowerHCALIsoSlope = cms.double(0.001),
                                     EndcapMaxHadronicOverEm = cms.double(0.05),
                                     EndcapHollowConeTrkIsoConst = cms.double(2.0),
                                     EndcapHollowConeTrkIsoSlope = cms.double(0.001),
                                     minPt = cms.double(10),
                                     maxEta = cms.double(2.5),
                                     BarrelMaxSigmaIetaIeta = cms.double(0.012), #ignored!
                                     EndcapMaxSigmaIetaIeta = cms.double(0.1)  #ignored!
                                     )

Jul022010_poter95_selection = cms.PSet(version = cms.string("Jul022010_poter"),
                                       BarrelJurrasicECALIsoConst = cms.double(50),
                                       BarrelJurrasicECALIsoSlope = cms.double(0.004),
                                       BarrelTowerHCALIsoConst = cms.double(50),
                                       BarrelTowerHCALIsoSlope = cms.double(0.001),
                                       BarrelMaxHadronicOverEm = cms.double(0.5),
                                       BarrelHollowConeTrkIsoConst = cms.double(3.9),
                                       BarrelHollowConeTrkIsoSlope = cms.double(0.001),
                                       BarrelMaxSigmaIetaIeta = cms.double(0.012),
                                       EndcapJurrasicECALIsoConst = cms.double(50),
                                       EndcapJurrasicECALIsoSlope = cms.double(0.004),
                                       EndcapTowerHCALIsoConst = cms.double(50),
                                       EndcapTowerHCALIsoSlope = cms.double(0.001),
                                       EndcapMaxHadronicOverEm = cms.double(0.045),
                                       EndcapHollowConeTrkIsoConst = cms.double(2.6),
                                       EndcapHollowConeTrkIsoSlope = cms.double(0.001),
                                       EndcapMaxSigmaIetaIeta = cms.double(0.1),
                                       minPt = cms.double(10),
                                       maxEta = cms.double(2.5)
                                       )

Jul022010_poter90_selection = cms.PSet(version = cms.string("Jul022010_poter"),
                                       BarrelJurrasicECALIsoConst = cms.double(2.6),
                                       BarrelJurrasicECALIsoSlope = cms.double(0.004),
                                       BarrelTowerHCALIsoConst = cms.double(50),
                                       BarrelTowerHCALIsoSlope = cms.double(0.001),
                                       BarrelMaxHadronicOverEm = cms.double(0.04),
                                       BarrelHollowConeTrkIsoConst = cms.double(2.5),
                                       BarrelHollowConeTrkIsoSlope = cms.double(0.001),
                                       BarrelMaxSigmaIetaIeta = cms.double(0.012),
                                       EndcapJurrasicECALIsoConst = cms.double(50),
                                       EndcapJurrasicECALIsoSlope = cms.double(0.004),
                                       EndcapTowerHCALIsoConst = cms.double(50),
                                       EndcapTowerHCALIsoSlope = cms.double(0.001),
                                       EndcapMaxHadronicOverEm = cms.double(0.045),
                                       EndcapHollowConeTrkIsoConst = cms.double(1.5),
                                       EndcapHollowConeTrkIsoSlope = cms.double(0.001),
                                       EndcapMaxSigmaIetaIeta = cms.double(0.038),
                                       minPt = cms.double(10),
                                       maxEta = cms.double(2.5)
                                       )

Jul022010_poter85_selection = cms.PSet(version = cms.string("Jul022010_poter"),
                                       BarrelJurrasicECALIsoConst = cms.double(2.6),
                                       BarrelJurrasicECALIsoSlope = cms.double(0.004),
                                       BarrelTowerHCALIsoConst = cms.double(50),
                                       BarrelTowerHCALIsoSlope = cms.double(0.001),
                                       BarrelMaxHadronicOverEm = cms.double(0.04),
                                       BarrelHollowConeTrkIsoConst = cms.double(1.6),
                                       BarrelHollowConeTrkIsoSlope = cms.double(0.001),
                                       BarrelMaxSigmaIetaIeta = cms.double(0.012),
                                       EndcapJurrasicECALIsoConst = cms.double(50),
                                       EndcapJurrasicECALIsoSlope = cms.double(0.004),
                                       EndcapTowerHCALIsoConst = cms.double(1.4),
                                       EndcapTowerHCALIsoSlope = cms.double(0.001),
                                       EndcapMaxHadronicOverEm = cms.double(0.045),
                                       EndcapHollowConeTrkIsoConst = cms.double(1.2),
                                       EndcapHollowConeTrkIsoSlope = cms.double(0.001),
                                       EndcapMaxSigmaIetaIeta = cms.double(0.038),
                                       minPt = cms.double(10),
                                       maxEta = cms.double(2.5)
                                       )

Jul022010_poter80_selection = cms.PSet(version = cms.string("Jul022010_poter"),
                                       BarrelJurrasicECALIsoConst = cms.double(2.6),
                                       BarrelJurrasicECALIsoSlope = cms.double(0.004),
                                       BarrelTowerHCALIsoConst = cms.double(1.2),
                                       BarrelTowerHCALIsoSlope = cms.double(0.001),
                                       BarrelMaxHadronicOverEm = cms.double(0.04),
                                       BarrelHollowConeTrkIsoConst = cms.double(1.3),
                                       BarrelHollowConeTrkIsoSlope = cms.double(0.001),
                                       BarrelMaxSigmaIetaIeta = cms.double(0.011),
                                       EndcapJurrasicECALIsoConst = cms.double(1.4),
                                       EndcapJurrasicECALIsoSlope = cms.double(0.004),
                                       EndcapTowerHCALIsoConst = cms.double(0.8),
                                       EndcapTowerHCALIsoSlope = cms.double(0.001),
                                       EndcapMaxHadronicOverEm = cms.double(0.045),
                                       EndcapHollowConeTrkIsoConst = cms.double(1.0),
                                       EndcapHollowConeTrkIsoSlope = cms.double(0.001),
                                       EndcapMaxSigmaIetaIeta = cms.double(0.038),
                                       minPt = cms.double(10),
                                       maxEta = cms.double(2.5)
                                       )

Jul022010_poter75_selection = cms.PSet(version = cms.string("Jul022010_poter"),
                                       BarrelJurrasicECALIsoConst = cms.double(2.6),
                                       BarrelJurrasicECALIsoSlope = cms.double(0.004),
                                       BarrelTowerHCALIsoConst = cms.double(1.2),
                                       BarrelTowerHCALIsoSlope = cms.double(0.001),
                                       BarrelMaxHadronicOverEm = cms.double(0.04),
                                       BarrelHollowConeTrkIsoConst = cms.double(0.9),
                                       BarrelHollowConeTrkIsoSlope = cms.double(0.001),
                                       BarrelMaxSigmaIetaIeta = cms.double(0.011),
                                       EndcapJurrasicECALIsoConst = cms.double(1.4),
                                       EndcapJurrasicECALIsoSlope = cms.double(0.004),
                                       EndcapTowerHCALIsoConst = cms.double(0.8),
                                       EndcapTowerHCALIsoSlope = cms.double(0.001),
                                       EndcapMaxHadronicOverEm = cms.double(0.045),
                                       EndcapHollowConeTrkIsoConst = cms.double(0.8),
                                       EndcapHollowConeTrkIsoSlope = cms.double(0.001),
                                       EndcapMaxSigmaIetaIeta = cms.double(0.035),
                                       minPt = cms.double(10),
                                       maxEta = cms.double(2.5)
                                       )

Jul022010_poter65_selection = cms.PSet(version = cms.string("Jul022010_poter"),
                                       BarrelJurrasicECALIsoConst = cms.double(1.6),
                                       BarrelJurrasicECALIsoSlope = cms.double(0.004),
                                       BarrelTowerHCALIsoConst = cms.double(1.2),
                                       BarrelTowerHCALIsoSlope = cms.double(0.001),
                                       BarrelMaxHadronicOverEm = cms.double(0.01),
                                       BarrelHollowConeTrkIsoConst = cms.double(0.7),
                                       BarrelHollowConeTrkIsoSlope = cms.double(0.001),
                                       BarrelMaxSigmaIetaIeta = cms.double(0.011),
                                       EndcapJurrasicECALIsoConst = cms.double(1.4),
                                       EndcapJurrasicECALIsoSlope = cms.double(0.004),
                                       EndcapTowerHCALIsoConst = cms.double(0.6),
                                       EndcapTowerHCALIsoSlope = cms.double(0.001),
                                       EndcapMaxHadronicOverEm = cms.double(0.025),
                                       EndcapHollowConeTrkIsoConst = cms.double(0.8),
                                       EndcapHollowConeTrkIsoSlope = cms.double(0.001),
                                       EndcapMaxSigmaIetaIeta = cms.double(0.03),
                                       minPt = cms.double(10),
                                       maxEta = cms.double(2.5)
                                       )

Jul022010_poterrel95_selection = cms.PSet(version = cms.string("Jul022010_poterrel"),
                                          BarrelJurrasicECALIsoConst = cms.double(5),
                                          BarrelTowerHCALIsoConst = cms.double(5),
                                          BarrelMaxHadronicOverEm = cms.double(0.5),
                                          BarrelHollowConeTrkIsoConst = cms.double(.22),
                                          BarrelMaxSigmaIetaIeta = cms.double(0.012),
                                          EndcapJurrasicECALIsoConst = cms.double(5),
                                          EndcapTowerHCALIsoConst = cms.double(5),
                                          EndcapMaxHadronicOverEm = cms.double(0.05),
                                          EndcapHollowConeTrkIsoConst = cms.double(.17),
                                          EndcapMaxSigmaIetaIeta = cms.double(0.1),
                                          minPt = cms.double(10),
                                          maxEta = cms.double(2.5),
                                          BarrelJurrasicECALIsoSlope = cms.double(0.004), #ignored!
                                          BarrelTowerHCALIsoSlope = cms.double(0.001), # ignored
                                          BarrelHollowConeTrkIsoSlope = cms.double(0.001), #ignored
                                          EndcapJurrasicECALIsoSlope = cms.double(0.001), #ignored
                                          EndcapTowerHCALIsoSlope = cms.double(0.001), #ignored
                                          EndcapHollowConeTrkIsoSlope = cms.double(0.001) #ignored
                                          )

Jul022010_poterrel90_selection = cms.PSet(version = cms.string("Jul022010_poterrel"),
                                          BarrelJurrasicECALIsoConst = cms.double(5),
                                          BarrelTowerHCALIsoConst = cms.double(5),
                                          BarrelMaxHadronicOverEm = cms.double(0.045),
                                          BarrelHollowConeTrkIsoConst = cms.double(2.5),
                                          BarrelMaxSigmaIetaIeta = cms.double(0.012),
                                          EndcapJurrasicECALIsoConst = cms.double(5),
                                          EndcapTowerHCALIsoConst = cms.double(5),
                                          EndcapMaxHadronicOverEm = cms.double(0.05),
                                          EndcapHollowConeTrkIsoConst = cms.double(0.09),
                                          EndcapMaxSigmaIetaIeta = cms.double(0.038),
                                          minPt = cms.double(10),
                                          maxEta = cms.double(2.5),
                                          BarrelJurrasicECALIsoSlope = cms.double(0.004), #ignored!
                                          BarrelTowerHCALIsoSlope = cms.double(0.001), # ignored
                                          BarrelHollowConeTrkIsoSlope = cms.double(0.001), #ignored
                                          EndcapJurrasicECALIsoSlope = cms.double(0.001), #ignored
                                          EndcapTowerHCALIsoSlope = cms.double(0.001), #ignored
                                          EndcapHollowConeTrkIsoSlope = cms.double(0.001) #ignored
                                          )

Jul022010_poterrel85_selection = cms.PSet(version = cms.string("Jul022010_poterrel"),
                                          BarrelJurrasicECALIsoConst = cms.double(0.16),
                                          BarrelTowerHCALIsoConst = cms.double(5),
                                          BarrelMaxHadronicOverEm = cms.double(0.045),
                                          BarrelHollowConeTrkIsoConst = cms.double(0.09),
                                          BarrelMaxSigmaIetaIeta = cms.double(0.012),
                                          EndcapJurrasicECALIsoConst = cms.double(5),
                                          EndcapTowerHCALIsoConst = cms.double(0.08),
                                          EndcapMaxHadronicOverEm = cms.double(0.05),
                                          EndcapHollowConeTrkIsoConst = cms.double(0.07),
                                          EndcapMaxSigmaIetaIeta = cms.double(0.038),
                                          minPt = cms.double(10),
                                          maxEta = cms.double(2.5),
                                          BarrelJurrasicECALIsoSlope = cms.double(0.004), #ignored!
                                          BarrelTowerHCALIsoSlope = cms.double(0.001), # ignored
                                          BarrelHollowConeTrkIsoSlope = cms.double(0.001), #ignored
                                          EndcapJurrasicECALIsoSlope = cms.double(0.001), #ignored
                                          EndcapTowerHCALIsoSlope = cms.double(0.001), #ignored
                                          EndcapHollowConeTrkIsoSlope = cms.double(0.001) #ignored
                                          )

Jul022010_poterrel80_selection = cms.PSet(version = cms.string("Jul022010_poterrel"),
                                          BarrelJurrasicECALIsoConst = cms.double(0.16),
                                          BarrelTowerHCALIsoConst = cms.double(5),
                                          BarrelMaxHadronicOverEm = cms.double(0.045),
                                          BarrelHollowConeTrkIsoConst = cms.double(0.06),
                                          BarrelMaxSigmaIetaIeta = cms.double(0.011),
                                          EndcapJurrasicECALIsoConst = cms.double(0.11),
                                          EndcapTowerHCALIsoConst = cms.double(0.08),
                                          EndcapMaxHadronicOverEm = cms.double(0.025),
                                          EndcapHollowConeTrkIsoConst = cms.double(0.07),
                                          EndcapMaxSigmaIetaIeta = cms.double(0.034),
                                          minPt = cms.double(10),
                                          maxEta = cms.double(2.5),
                                          BarrelJurrasicECALIsoSlope = cms.double(0.004), #ignored!
                                          BarrelTowerHCALIsoSlope = cms.double(0.001), # ignored
                                          BarrelHollowConeTrkIsoSlope = cms.double(0.001), #ignored
                                          EndcapJurrasicECALIsoSlope = cms.double(0.001), #ignored
                                          EndcapTowerHCALIsoSlope = cms.double(0.001), #ignored
                                          EndcapHollowConeTrkIsoSlope = cms.double(0.001) #ignored
                                          )

Jul022010_poterrel75_selection = cms.PSet(version = cms.string("Jul022010_poterrel"),
                                          BarrelJurrasicECALIsoConst = cms.double(0.16),
                                          BarrelTowerHCALIsoConst = cms.double(0.07),
                                          BarrelMaxHadronicOverEm = cms.double(0.045),
                                          BarrelHollowConeTrkIsoConst = cms.double(0.05),
                                          BarrelMaxSigmaIetaIeta = cms.double(0.011),
                                          EndcapJurrasicECALIsoConst = cms.double(0.11),
                                          EndcapTowerHCALIsoConst = cms.double(0.05),
                                          EndcapMaxHadronicOverEm = cms.double(0.015),
                                          EndcapHollowConeTrkIsoConst = cms.double(0.07),
                                          EndcapMaxSigmaIetaIeta = cms.double(0.034),
                                          minPt = cms.double(10),
                                          maxEta = cms.double(2.5),
                                          BarrelJurrasicECALIsoSlope = cms.double(0.004), #ignored!
                                          BarrelTowerHCALIsoSlope = cms.double(0.001), # ignored
                                          BarrelHollowConeTrkIsoSlope = cms.double(0.001), #ignored
                                          EndcapJurrasicECALIsoSlope = cms.double(0.001), #ignored
                                          EndcapTowerHCALIsoSlope = cms.double(0.001), #ignored
                                          EndcapHollowConeTrkIsoSlope = cms.double(0.001) #ignored
                                          )

Jul022010_poterrel65_selection = cms.PSet(version = cms.string("Jul022010_poterrel"),
                                          BarrelJurrasicECALIsoConst = cms.double(0.11),
                                          BarrelTowerHCALIsoConst = cms.double(0.07),
                                          BarrelMaxHadronicOverEm = cms.double(0.01),
                                          BarrelHollowConeTrkIsoConst = cms.double(0.03),
                                          BarrelMaxSigmaIetaIeta = cms.double(0.011),
                                          EndcapJurrasicECALIsoConst = cms.double(0.11),
                                          EndcapTowerHCALIsoConst = cms.double(0.05),
                                          EndcapMaxHadronicOverEm = cms.double(0.01),
                                          EndcapHollowConeTrkIsoConst = cms.double(0.01),
                                          EndcapMaxSigmaIetaIeta = cms.double(0.032),
                                          minPt = cms.double(10),
                                          maxEta = cms.double(2.5),
                                          BarrelJurrasicECALIsoSlope = cms.double(0.004), #ignored!
                                          BarrelTowerHCALIsoSlope = cms.double(0.001), # ignored
                                          BarrelHollowConeTrkIsoSlope = cms.double(0.001), #ignored
                                          EndcapJurrasicECALIsoSlope = cms.double(0.001), #ignored
                                          EndcapTowerHCALIsoSlope = cms.double(0.001), #ignored
                                          EndcapHollowConeTrkIsoSlope = cms.double(0.001) #ignored
                                       )

photonSelection_Fsr2011Apr11 = cms.PSet(
    version = cms.string("Fsr2011Apr11"),
    ## 1. maximum super cluster pseudo-rapidity absolute value
    maxAbsEtaSC = cms.double(2.5),
    ## 2. exclude EB EE transition region SC |eta| in [1.4442, 1.566]
    excludeEBEEGap = cms.bool(True),
    ## 3. super cluster seed reco flag != EcalRecHit::kOutOfTime=2
    excludeOutOfTimeReco = cms.bool(True),
    ## 4. super cluster seed severity level != EcalSeverityLevelAlgo::kWeird=4
    excludeWeirdSeverity = cms.bool(True),
    ## 5. super cluster seed severity level != EcalSeverityLevelAlgo::kBad=5
    excludeBadSeverity = cms.bool(True),
    ## 6. maximum ECAL isolation
    ecalIsoConst = cms.double(4.2),
    ecalIsoSlope = cms.double(0.006),
    ## 7. minimum transverse momentum
    minPt = cms.double(10),
    cutsToIgnore = cms.vstring("ecalIsolation"),
)

photonSelection_Fsr2011Apr11_PixelMatchVeto = cms.PSet(
    version = cms.string("Fsr2011Apr11"),
    ## 1. maximum super cluster pseudo-rapidity absolute value
    maxAbsEtaSC = cms.double(2.5),
    ## 2. exclude EB EE transition region SC |eta| in [1.4442, 1.566]
    excludeEBEEGap = cms.bool(True),
    ## 3. super cluster seed reco flag != EcalRecHit::kOutOfTime=2
    excludeOutOfTimeReco = cms.bool(True),
    ## 4. super cluster seed severity level != EcalSeverityLevelAlgo::kWeird=4
    excludeWeirdSeverity = cms.bool(True),
    ## 5. super cluster seed severity level != EcalSeverityLevelAlgo::kBad=5
    excludeBadSeverity = cms.bool(True),
    ## 6. maximum ECAL isolation
    ecalIsoConst = cms.double(4.2),
    ecalIsoSlope = cms.double(0.006),
    ## 7. minimum transverse momentum
    minPt = cms.double(5)
)


photonSelection_egammaTight2011May6 = cms.PSet(
    version = cms.string("Jul022010_poter"),
    BarrelJurrasicECALIsoConst = cms.double(4.2),
    BarrelJurrasicECALIsoSlope = cms.double(0.006),
    BarrelTowerHCALIsoConst = cms.double(2.2),
    BarrelTowerHCALIsoSlope = cms.double(0.0025),
    BarrelMaxHadronicOverEm = cms.double(0.05),
    BarrelHollowConeTrkIsoConst = cms.double(2),
    BarrelHollowConeTrkIsoSlope = cms.double(0.001),
    BarrelMaxSigmaIetaIeta = cms.double(0.013),
    EndcapJurrasicECALIsoConst = cms.double(4.2),
    EndcapJurrasicECALIsoSlope = cms.double(0.006),
    EndcapTowerHCALIsoConst = cms.double(2.2),
    EndcapTowerHCALIsoSlope = cms.double(0.0025),
    EndcapMaxHadronicOverEm = cms.double(0.05),
    EndcapHollowConeTrkIsoConst = cms.double(2),
    EndcapHollowConeTrkIsoSlope = cms.double(0.001),
    EndcapMaxSigmaIetaIeta = cms.double(0.03),
    minPt = cms.double(20),
    maxEta = cms.double(2.5)
    )


photonSelection_veryLoose2011May11 = photonSelection_egammaTight2011May6.clone(
    ignoreCuts = cms.vstring( "PassSigmaIetaIeta" )
)

## Relax all the cuts by 50%
for cut in """BarrelJurrasicECALIsoConst
              BarrelJurrasicECALIsoSlope
              BarrelTowerHCALIsoConst
              BarrelTowerHCALIsoSlope
              BarrelMaxHadronicOverEm
              BarrelHollowConeTrkIsoConst
              BarrelHollowConeTrkIsoSlope
              EndcapJurrasicECALIsoConst
              EndcapJurrasicECALIsoSlope
              EndcapTowerHCALIsoConst
              EndcapTowerHCALIsoSlope
              EndcapMaxHadronicOverEm
              EndcapHollowConeTrkIsoConst
              EndcapHollowConeTrkIsoSlope""".split():
    default = getattr( photonSelection_egammaTight2011May6, cut )
    setattr( photonSelection_veryLoose2011May11,
             cut,
             1.5 * default.value() )


photonSelection_Fsr2011June24_photonId = cms.PSet(
    version = cms.string("Fsr2011Apr11"),
    ## 1. maximum super cluster pseudo-rapidity absolute value
    maxAbsEtaSC = cms.double(2.5),
    ## 2. exclude EB EE transition region SC |eta| in [1.4442, 1.566]
    excludeEBEEGap = cms.bool(True),
    ## 3. super cluster seed reco flag != EcalRecHit::kOutOfTime=2
    excludeOutOfTimeReco = cms.bool(True),
    ## 4. super cluster seed severity level != EcalSeverityLevelAlgo::kWeird=4
    excludeWeirdSeverity = cms.bool(True),
    ## 5. super cluster seed severity level != EcalSeverityLevelAlgo::kBad=5
    excludeBadSeverity = cms.bool(True),
    ## 6. maximum ECAL isolation
    ecalIsoConst = cms.double(4.2),
    ecalIsoSlope = cms.double(0.006),
    ## 7. minimum transverse momentum
    minPt = cms.double(5),
    cutsToIgnore = cms.vstring( "maxAbsEtaSC",
                                "excludeEBEEGap",
                                "ecalIsolation" )
)


