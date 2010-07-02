import FWCore.ParameterSet.Config as cms

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
